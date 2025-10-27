#pragma once
#include <stddef.h>

typedef struct Matrix{

  size_t rows;
  size_t cols;
  double* data;
}Matrix;

///////////// CORE FUNCTIONS AND HELPERS
///in matrix_core.c
//creates matrix of given size with all elelemens = 0 
Matrix *matrix_create(size_t rows, size_t cols); 
//frees memory used by matrix and its data. Safe for NULL
void matrix_free(Matrix *m);
//Creates a deep copy of matrix (same elements), safe for NULL
Matrix *matrix_copy(const Matrix *src);
//Sets every element of matrix to a given value
void matrix_fill(Matrix *m, double value);
//converts matrix to identity form (only works for square), non-square issues error
void matrix_identity(Matrix *m);
//Returns value at (row, col)
double matrix_get(const Matrix *m, size_t row, size_t col);
//sets element (row,col) to value.
void matrix_set(Matrix *m, size_t row, size_t col, double value);


/////// ARITHMATIC ///////////////////
///in matrix_math.c
Matrix *matrix_add(const Matrix *a, const Matrix *b); //add
Matrix *matrix_sub(const Matrix *a, const Matrix *b); //subtract
Matrix *matrix_mul(const Matrix *a, const Matrix *b); //multipy
Matrix *matrix_scalar_mul(const Matrix *a, double scalar); //scalar mult


//utility functions in matrix_utils.c
void    matrix_print(const Matrix *m);
Matrix *matrix_from_file(const char *path);
int     matrix_to_file(const Matrix *m, const char *path);



//advanced
//Square only
double matrix_trace(const Matrix *a); 
double matrix_det(const Matrix *a); 
Matrix *matrix_inverse(const Matrix *a);

size_t matrix_rank(const Matrix *a, double tol); // Gaussian elimination with tol
