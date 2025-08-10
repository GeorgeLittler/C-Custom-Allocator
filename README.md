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

## Build & Quick Start
```bash
# Build & Run
make
./demo

# Quick start (terminal steps)
# 1) Create repo
mkdir c-custom-allocator && cd c-custom-allocator
git init

# 2) Add files from above
# (Create the five files: allocator.h, allocator.c, demo.c, Makefile, README.md)

# 3) Build & run
make
./demo

# 4) First half-commit (Round 1)
git add .
git commit -m "Round 1: basic allocator with fixed arena, first-fit, and free (no split/coalesce)"
