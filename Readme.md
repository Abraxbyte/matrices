# Matrix Library (WIP)
For personal use.

Small C matrix library with dynamic allocation and basic linear algebra

## Features
- Core: create, free, copy, fill, identity, get, set
- Math: add, sub, matmul, scalar-mul
- Utils: print, save to file, load from file
- Advanced: trace, determinant (Gaussian elimination with partial pivoting)
+More to come

## Installation: 
make clean

To run test:
make test

## Layout
include/
matrix.h
src/
matrix_core.c
matrix_math.c
matrix_utils.c
matrix_advanced.c
main.c # optional CLI demo
tests/
test_matrix.c # see below
