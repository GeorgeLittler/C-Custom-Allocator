#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>

int  allocator_init(size_t arena_bytes);  // returns 0 on success
void allocator_destroy(void);

void* a_malloc(size_t size);              // allocate memory
void  a_free(void* ptr);                  // free memory

#ifdef ALLOCATOR_DEBUG
void allocator_dump(void);                // prints heap layout
#endif

#endif // ALLOCATOR_H
