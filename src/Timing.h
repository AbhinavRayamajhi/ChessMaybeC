#ifndef TIMING_H
#define TIMING_H

#include <stdint.h>
#include <stddef.h>
#include <sys/time.h>

static inline uint64_t getCurrentTime() {

    struct timeval time;
    gettimeofday(&time, NULL);
    return ((uint64_t)time.tv_sec * 1000ULL) + ((uint64_t)time.tv_usec / 1000ULL);
}

#endif
