#include <dlfcn.h>
#include <unistd.h>

#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include <vector>

#include "draw.hpp"

enum class Exception {
    LIB_LOAD_ERROR,
    LIB_AGENT_ERROR,
};

class Simulation {
public:
    Simulation(const char* lib_path) {
        m_handle = dlopen(lib_path, RTLD_LAZY);
        if (m_handle == nullptr) {
            printf("Cannot open library %s\n", lib_path);
            throw Exception::LIB_LOAD_ERROR;
        }

        void (*get_handle_f)(Agent*) = (void(*)(Agent*)) dlsym(m_handle, "GetHandle");
        get_handle_f(&m_agent);
        if (m_agent.Next == nullptr) {
            printf("Cannot get agent\n");
            throw Exception::LIB_AGENT_ERROR;
        }
    }

    ~Simulation() {
        dlclose(m_handle);
    }

    void Run(const char* filename, uint32_t sleep_ms, uint64_t max_steps) {
        FILE *f = fopen(filename, "w");
        fprintf(f,"%d; %d\n", m_position.x, m_position.y);

        for (uint64_t step = 0; step < max_steps; ++step) {
            DirectionChange nextDirection = m_agent.Next(step, m_position, m_heading);
            m_heading = GetNextHeading(nextDirection);

            m_position.x += m_heading.x;
            m_position.y += m_heading.y;

            fprintf(f,"%d; %d\n", m_position.x, m_position.y);
            printf("Step %d\n", step);
            usleep(1000 * sleep_ms);
        }

        fclose(f);
    }

    Heading GetNextHeading(DirectionChange nextDirection) {
        switch (nextDirection) {
            case DirectionChange::LEFT:
                return {-m_heading.y, m_heading.x};

            case DirectionChange::RIGHT:
                return {m_heading.y, -m_heading.x};

            default:
                return m_heading;
        }
    }

private:
    void* m_handle = nullptr;
    Agent m_agent;

    Position m_position = {0, 0};
    Heading m_heading = {0, 1};
};

int main(int argc, char* argv[]) {
    if ((argc - 1) != 4) {
        printf("%s {lib_path} {csv_file} {sleep_ms} {max_steps} \n", argv[0]);
        return -1;
    }

    Simulation* simulation;
    const char* lib_path = argv[1];
    const char* filename = argv[2];
    uint32_t sleep_ms = atoi(argv[3]);
    uint32_t max_steps = atoi(argv[4]);

    try {
        simulation = new Simulation(lib_path);
    } catch (Exception ex) {
        switch (ex) {
            case Exception::LIB_LOAD_ERROR:
            case Exception::LIB_AGENT_ERROR:
                return -1;
        }
    }

    simulation->Run(filename, sleep_ms, max_steps);

    delete simulation;
    return 0;
}
