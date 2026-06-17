#include "timing.hpp"
#include <time.h>

static struct timespec g_start, g_end;

void start_timer() {
    clock_gettime(CLOCK_MONOTONIC, &g_start);
}

double stop_timer(int iterations) {
    clock_gettime(CLOCK_MONOTONIC, &g_end);
    
    double elapsed_ms =
        (g_end.tv_sec - g_start.tv_sec) * 1000.0 +
        (g_end.tv_nsec - g_start.tv_nsec) / 1000000.0;
        
    return elapsed_ms / iterations;
}