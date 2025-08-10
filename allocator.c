#define _DEFAULT_SOURCE
#include "allocator.h"
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

typedef struct block {
    size_t size;           // usable payload bytes in this block
    int    free;           // 1 = free, 0 = used
    struct block* next;    // next block in the free list / heap list
} block_t;

static void*    g_arena       = NULL;
static size_t   g_arena_size  = 0;
static block_t* g_head        = NULL;

static size_t align8(size_t n) {
    return (n + 7u) & ~((size_t)7u);
}

int allocator_init(size_t arena_bytes) {
    if (g_arena) return 0; // already initialised

    arena_bytes = align8(arena_bytes);
    if (arena_bytes < sizeof(block_t) + 8) return -1;

    void* mem = mmap(NULL, arena_bytes, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mem == MAP_FAILED) return -1;

    g_arena = mem;
    g_arena_size = arena_bytes;

    // One big free block spanning the whole arena minus header
    g_head = (block_t*)g_arena;
    g_head->size = arena_bytes - sizeof(block_t);
    g_head->free = 1;
    g_head->next = NULL;

    return 0;
}

void allocator_destroy(void) {
    if (!g_arena) return;
    munmap(g_arena, g_arena_size);
    g_arena = NULL;
    g_arena_size = 0;
    g_head = NULL;
}

// First-fit search, no block splitting yet (added in Round 2)
void* a_malloc(size_t size) {
    if (!g_arena || size == 0) return NULL;

    size = align8(size);
    block_t* cur = g_head;

    while (cur) {
        if (cur->free && cur->size >= size) {
            cur->free = 0;
            // (Round 2: split large blocks here)
            return (void*)((uint8_t*)cur + sizeof(block_t));
        }
        cur = cur->next;
    }
    return NULL; // out of memory
}

// Mark free; (Round 2: coalesce adjacent free blocks)
void a_free(void* ptr) {
    if (!ptr) return;
    block_t* blk = (block_t*)((uint8_t*)ptr - sizeof(block_t));
    blk->free = 1;
}

// (Optional helper for Round 2: iterate blocks)
static block_t* next_block(block_t* blk) {
    if (!blk) return NULL;
    uint8_t* p = (uint8_t*)blk + sizeof(block_t) + blk->size;
    if ((void*)p >= (uint8_t*)g_arena + g_arena_size) return NULL;
    return (block_t*)p;
}

// (Optional sanity check — useful during Round 2)
__attribute__((unused))
static int heap_ok(void) {
    size_t total = 0;
    uint8_t* start = (uint8_t*)g_arena;
    block_t* cur = (block_t*)start;
    while ((void*)cur < start + g_arena_size) {
        total += sizeof(block_t) + cur->size;
        block_t* nb = next_block(cur);
        if (!nb) break;
        cur = nb;
    }
    return total == g_arena_size;
}
