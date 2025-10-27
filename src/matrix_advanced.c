// MIssing functionality: inverse of matrix, rank, maybe eigen/decomp/jordan stuff
#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h> //for fabs (abs value of double)

//Small helpers for this file, maybe better in util
static void swap_rows(double *data, size_t cols, size_t r1, size_t r2){
  if(r1 == r2) return;
  for(size_t j = 0; j < cols; j++){
    double tmp = data[r1*cols + j];
    data[r1*cols + j] = data[r2*cols + j];
    data[r2*cols + j] = tmp;
  }
}

//Finds trace of matrix (sum of diagonal)
//Returns 0.0 on NULL or non-square
double matrix_trace(const Matrix *a){
  if(!a || a->rows != a->cols) return 0.0;
  double trace = 0.0;
  for(size_t i = 0; i < a->rows; i++){
    trace += a->data[i * a->cols + i];
  }
  return trace;
}

//Calculate determinant via Gaussian elimination with partial pivoting (to reduce truncation errors due to dividing)
//copied matrix is made to be upper-triangular, row swaps change sign, once triangular determinant trivial
//returns 0 if singular or error
//
double matrix_det(const Matrix *a){
  if(!a || a->rows != a->cols) return 0.0; //quick catch for singular matrices
  const size_t n = a->rows;
  const double EPS = 1e-12;
  Matrix *m = matrix_copy(a);
  if(!m) return 0.0;
  int swap_count = 0;

  for(size_t k = 0; k < n; k++){
    //find largest pivot (absolute)
    size_t pivot_row = k;
    double pivot_abs = fabs(m->data[k*n + k]);
    for(size_t r = k + 1; r < n; r++){
      double v = fabs(m->data[r*n + k]);
      if(v > pivot_abs){
        pivot_abs = v;
        pivot_row = r;
      }
    }

    //if pivot is 0, matrix singular 
    if(pivot_abs <= EPS){
      matrix_free(m);
      return 0.0;
    }

    //
    if(pivot_row != k){
      swap_rows(m->data, n, k, pivot_row);
      swap_count++;
    }

    //eliminate entries below pivot col k, update row to form U triangle 
    double pivot = m->data[k*n + k];
    for(size_t i = k + 1; i < n; i++){
      double factor = m->data[i*n + k] / pivot;
      if(factor == 0.0) continue;
      m->data[i*n + k] = 0.0; 
      for(size_t j = k + 1; j < n; j++){
        m->data[i*n + j] -= factor * m->data[k*n + j];
      }
    }
  }
  //product of diagonal (with correct sign from swap count)
  double det = (swap_count % 2 == 0) ? 1.0 : -1.0;
  for(size_t i = 0; i < n; i++){
    det *= m->data[i*n + i];
  }
  matrix_free(m);
  return det;
}
