#include "allocator.h"
#include <stdio.h>
#include <string.h>

int main(void) {
    if (allocator_init(1024 * 1024) != 0) { // 1 MiB arena
        fprintf(stderr, "allocator_init failed\n");
        return 1;
    }

    void* p = a_malloc(64);
    void* q = a_malloc(200);
    void* r = a_malloc(8000);

    if (!p || !q || !r) {
        fprintf(stderr, "allocation failed\n");
        allocator_destroy();
        return 1;
    }

    strcpy((char*)p, "Hello from a_malloc!");
    printf("%s\n", (char*)p);

    a_free(q);
    a_free(p);
    a_free(r);

    allocator_destroy();
    return 0;
}
