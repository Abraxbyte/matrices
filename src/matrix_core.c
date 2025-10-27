#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//creates matrix of given size with all elelemens = 0 
Matrix *matrix_create(size_t rows, size_t cols){
  Matrix* m = malloc(sizeof(Matrix));
  if (!m) return NULL;
  m->rows = rows;
  m->cols = cols;
  m->data = calloc(rows * cols, sizeof(double));
  //checks if data init failed
  if (!m->data) {
    free(m); 
    return NULL;
  }
  return m;
}
//frees memory used by matrix and its data. Safe for NULL
void matrix_free(Matrix *m){
  if (!m) return;
  free(m->data);
  free(m);
}

//Creates a deep copy of matrix (same elements), safe for NULL
Matrix *matrix_copy(const Matrix *src){
  if (!src) return NULL;
  Matrix *copy = matrix_create(src->rows, src->cols);
  if (!copy) return NULL;
  memcpy(copy->data, src->data, src->rows * src->cols * sizeof(double)); //copies the bits from source to copy
  return copy;
}

//Sets every element of matrix to a given value
void matrix_fill(Matrix *m, double value){
  if (!m) return;
  size_t n = m->rows * m->cols;
  for (size_t i = 0; i < n; i++){
    m->data[i] = value;
  }
}

//converts matrix to identity form (only works for square), non-square issues error
void matrix_identity(Matrix *m){
  if (!m || m->rows != m->cols) return;
  matrix_fill(m, 0.0);//make all 0
  for (size_t i = 0; i < m->rows; i++){
    m->data[i * m->rows + i] = 1.0;
  }
}

//Returns value at (row, col)
double matrix_get(const Matrix *m, size_t row, size_t col) {
  //if (!m || m->rows != m->cols) return NULL; avoided for performance
    return m->data[row * m->cols + col];
}

//Set element
void matrix_set(Matrix *m, size_t row, size_t col, double value) {
    m->data[row * m->cols + col] = value;
}
