#include <time.h>

// The bare-metal <time.h> includes 'struct timespec', but omits 
// POSIX declarations. We explicitly declare them here to satisfy the compiler:
#ifndef CLOCK_MONOTONIC
#define CLOCK_MONOTONIC 1
#endif

extern "C" int clock_gettime(clockid_t clk_id, struct timespec *tp);

static struct timespec g_start, g_end;

void start_timer() {
    clock_gettime(CLOCK_MONOTONIC, &g_start);
}

double stop_timer(int iterations) {        
    clock_gettime(CLOCK_MONOTONIC, &g_end);
    
    double seconds = (g_end.tv_sec  - g_start.tv_sec) +
                     (g_end.tv_nsec - g_start.tv_nsec) / 1e9;
    
    return (seconds * 1000.0) / iterations;  
}