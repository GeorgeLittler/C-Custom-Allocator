#include "allocator.h"
#include <stdio.h>
#include <string.h>

int main(void) {
    if (allocator_init(1024 * 1024) != 0) { // 1 MiB
        fprintf(stderr, "allocator_init failed\n");
        return 1;
    }

    void* a = a_malloc(64);
    void* b = a_malloc(200);
    void* c = a_malloc(8000);

#ifdef ALLOCATOR_DEBUG
    allocator_dump();
#endif

    a_free(b);           // free middle
    a_free(a);           // free adjacent -> should coalesce with b
    a_free(c);

#ifdef ALLOCATOR_DEBUG
    allocator_dump();
#endif

    allocator_destroy();
    return 0;
}
