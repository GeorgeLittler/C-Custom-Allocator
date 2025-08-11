#include "allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

static double now_sec(void) {
#if defined(_POSIX_TIMERS) && _POSIX_TIMERS > 0 && defined(CLOCK_MONOTONIC)
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
#else
    // Fallback for platforms without CLOCK_MONOTONIC
    return (double)clock() / CLOCKS_PER_SEC;
#endif
}

int main(void) {
    const int N = 100000;
    const int SZ[] = {16, 32, 64, 128, 256, 512};
    const int K = (int)(sizeof(SZ)/sizeof(SZ[0]));

    // libc malloc/free
    void** p = malloc(sizeof(void*) * N);
    double t0 = now_sec();
    for (int i = 0; i < N; ++i) {
        p[i] = malloc(SZ[i % K]);
        if (!p[i]) { fprintf(stderr, "malloc failed\n"); return 1; }
    }
    for (int i = 0; i < N; ++i) free(p[i]);
    double t1 = now_sec();

    // our allocator
    if (allocator_init(32 * 1024 * 1024) != 0) { fprintf(stderr, "allocator_init failed\n"); return 1; } // 32 MiB
    double t2 = now_sec();
    for (int i = 0; i < N; ++i) {
        p[i] = a_malloc(SZ[i % K]);
        if (!p[i]) { fprintf(stderr, "a_malloc failed at i=%d\n", i); return 1; }
        // write a byte to touch memory
        ((char*)p[i])[0] = (char)(i & 0xFF);
    }
    for (int i = 0; i < N; ++i) a_free(p[i]);
    double t3 = now_sec();
    allocator_destroy();

    printf("malloc/free: %.3f ms\n", (t1 - t0) * 1000.0);
    printf("a_malloc/a_free: %.3f ms\n", (t3 - t2) * 1000.0);

    free(p);
    return 0;
}
