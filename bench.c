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

static int parse_int(const char* s, int fallback) {
    if (!s) return fallback;
    char* end = NULL;
    long v = strtol(s, &end, 10);
    return (end && *end == '\0' && v > 0) ? (int)v : fallback;
}

int main(int argc, char** argv) {
    // CLI: ./bench [arenaMiB] [N] [batch]
    int arena_mib = (argc > 1) ? parse_int(argv[1], 32) : 32;    // default 32 MiB
    int N         = (argc > 2) ? parse_int(argv[2], 100000) : 100000;
    int BATCH     = (argc > 3) ? parse_int(argv[3], 5000)   : 5000;

    const int SZ[] = {16, 32, 64, 128, 256, 512};
    const int K = (int)(sizeof(SZ)/sizeof(SZ[0]));

    // libc malloc/free (all-at-once)
    void** p = malloc(sizeof(void*) * (size_t)BATCH);
    double t0 = now_sec();
    {
        void** q = malloc(sizeof(void*) * (size_t)N);
        for (int i = 0; i < N; ++i) {
            q[i] = malloc(SZ[i % K]);
            if (!q[i]) { fprintf(stderr, "malloc failed\n"); return 1; }
        }
        for (int i = 0; i < N; ++i) free(q[i]);
        free(q);
    }
    double t1 = now_sec();

    // our allocator (batched reuse to fit arena)
    if (allocator_init((size_t)arena_mib * 1024 * 1024) != 0) {
        fprintf(stderr, "allocator_init failed\n"); return 1;
    }
    double t2 = now_sec();
    for (int i = 0; i < N; i += BATCH) {
        int m = (i + BATCH <= N) ? BATCH : (N - i);
        for (int j = 0; j < m; ++j) {
            p[j] = a_malloc(SZ[(i + j) % K]);
            if (!p[j]) { fprintf(stderr, "a_malloc failed at i=%d\n", i + j); return 1; }
            ((char*)p[j])[0] = (char)((i + j) & 0xFF);
        }
        for (int j = 0; j < m; ++j) a_free(p[j]);
    }
    double t3 = now_sec();
    allocator_destroy();

    printf("malloc/free: %.3f ms\n", (t1 - t0) * 1000.0);
    printf("a_malloc/a_free (arena=%d MiB, batch=%d): %.3f ms\n",
           arena_mib, BATCH, (t3 - t2) * 1000.0);

    free(p);
    return 0;
}
