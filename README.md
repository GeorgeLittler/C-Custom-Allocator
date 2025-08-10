# Custom Memory Allocator (Round 1)

A lightweight C memory allocator using a fixed `mmap` arena and a simple free list.  
Designed to demonstrate low-level memory management concepts such as manual allocation, alignment, and free-list traversal.

**Round 1** includes:
- Fixed-size arena (`mmap`)
- First-fit allocation
- `a_free` (no coalescing yet)
- 8-byte alignment

**Planned Round 2**
- Block splitting on allocate
- Coalescing adjacent free blocks on free
- Basic benchmarks vs `malloc`/`free`
- Debug mode to print heap layout

## Build & Run
```bash
make
./demo
