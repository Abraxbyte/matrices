#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>

//self explanatory funcitons

Matrix *matrix_add(const Matrix *a, const Matrix *b){
  if(!a || !b || a->rows != b->rows || a->cols != b->cols) return NULL; //check dims
  Matrix *sum = matrix_create(a->rows, a->cols);
  if (!sum) return NULL;
  sum->rows = a->rows;
  sum->cols = a->cols;
  sum->data = malloc(a->rows * a->cols * sizeof(double));

  for (size_t i = 0; i < a->rows * a->cols; i++){
    sum->data[i] = a->data[i] + b->data[i];
  }
  return sum;
}

Matrix *matrix_sub(const Matrix *a, const Matrix *b){ //subtract b from a
  if(!a || !b || a->rows != b->rows || a->cols != b->cols) return NULL; //compare dims
  Matrix *diff = matrix_create(a->rows, a->cols);
  if (!diff) return NULL;
  for (size_t i = 0; i < a->rows * a->cols; i++){
    diff->data[i] = a->data[i] - b->data[i];
  }
  return diff;
}
//Multiply two matrices, check n x m and m x n
Matrix *matrix_mul(const Matrix *a, const Matrix *b){
  if(!a || !b || a->cols != b->rows) return NULL;
  Matrix *prod = matrix_create(a->rows, b->cols);
  if (!prod) return NULL;
  double num = 0;
  for (size_t  k = 0; k < a->rows; k++){
    for (size_t i = 0; i < b->cols; i++){
      for (size_t j= 0; j < a->cols; j++){ 
      num += a->data[k * a->cols + j] * b->data[j * b->cols + i];
      }
      prod->data[k * prod->cols + i] = num;
      num = 0;
    }
  }
  return prod;
}
Matrix *matrix_scalar_mul(const Matrix *a, double scalar){
  if(!a) return NULL; 
  Matrix *prod = matrix_create(a->rows, a->cols);
  if (!prod) return NULL;
  for (size_t i = 0; i < a->rows * a->cols; i++){
    prod->data[i] = a->data[i] * scalar;
  }
  return prod;
}

