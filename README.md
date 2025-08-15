# Custom Memory Allocator

A lightweight C memory allocator that manages a fixed `mmap`’d arena with an implicit list of blocks.  
It supports first-fit allocation, block splitting, coalescing, debug heap dumps, and benchmarking.

## Features
- Fixed-size arena via `mmap`
- 8-byte alignment
- First-fit allocation
- **Block splitting** on allocate (limits internal fragmentation)
- **Coalescing** adjacent free blocks on free
- Optional **heap dump** in debug builds (`-DALLOCATOR_DEBUG`)
- Minimal **benchmark** comparing `a_malloc/a_free` vs `malloc/free`

## Build & Run
```bash
make          # builds demo (debug) and bench (release)
./demo        # quick sanity run with heap dump in debug builds
./bench 32 100000 5000
#          ^arenaMiB ^N allocs  ^batch size (batched reuse)
```

## Example Output

malloc/free: 11.470 ms
a_malloc/a_free (arena=32 MiB, batch=5000): 619.093 ms

(Numbers vary by machine; this allocator is for educational purposes, not speed.)

## Design

- Layout: [block header][payload][next block] … inside a contiguous arena.

- Header: { size, free, next } (the next field is currently unused for the implicit heap walk).

- Allocation: first-fit scan; split large free blocks into [used][free].

- Free: mark as free and coalesce with physical neighbours to reduce fragmentation.

## Possible Improvements

- Next-fit “rover” or explicit free list for speed-up

- Boundary tags (footers) for O(1) backward coalescing

- Double-free detection / canary bytes in debug

- Broader benchmark scenarios