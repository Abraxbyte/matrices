#include "matrix.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

static int nearly_eq(double a, double b, double eps){ return fabs(a-b) <= eps; }

static Matrix* mat_from_vals(size_t r, size_t c, const double *v){
  Matrix *m = matrix_create(r,c);
  assert(m);
  for(size_t i=0;i<r*c;i++) m->data[i]=v[i];
  return m;
}

static int mat_eq(const Matrix *a, const Matrix *b, double eps){
  if(!a || !b) return 0;
  if(a->rows!=b->rows || a->cols!=b->cols) return 0;
  for(size_t i=0;i<a->rows*a->cols;i++)
    if(!nearly_eq(a->data[i], b->data[i], eps)) return 0;
  return 1;
}

static void test_core(void){
  Matrix *m = matrix_create(3,4);
  assert(m && m->rows==3 && m->cols==4);
  matrix_fill(m, 2.5);
  for(size_t i=0;i<12;i++) assert(nearly_eq(m->data[i],2.5,1e-12));

  matrix_set(m, 1, 2, 9.0);
  assert(nearly_eq(matrix_get(m,1,2), 9.0, 1e-12));

  matrix_identity(m); // square required, expect no-op because 3x4
  // turn into square and try again
  matrix_free(m);
  m = matrix_create(3,3);
  matrix_fill(m, 0.0);
  matrix_identity(m);
  for(size_t i=0;i<3;i++)
    for(size_t j=0;j<3;j++)
      assert(nearly_eq(m->data[i*m->cols + j], i==j?1.0:0.0, 1e-12));

  Matrix *cp = matrix_copy(m);
  assert(cp && mat_eq(m, cp, 1e-12));

  matrix_free(cp);
  matrix_free(m);
}

static void test_add_sub_scalar(void){
  const double av[] = {1,2,3,4};
  const double bv[] = {5,6,7,8};
  Matrix *A = mat_from_vals(2,2,av);
  Matrix *B = mat_from_vals(2,2,bv);

  Matrix *S = matrix_add(A,B);
  const double sv[] = {6,8,10,12};
  Matrix *Sexp = mat_from_vals(2,2,sv);
  assert(S && mat_eq(S,Sexp,1e-12));

  Matrix *D = matrix_sub(B,A);
  const double dv[] = {4,4,4,4};
  Matrix *Dexp = mat_from_vals(2,2,dv);
  assert(D && mat_eq(D,Dexp,1e-12));

  Matrix *K = matrix_scalar_mul(A, 3.0);
  const double kv[] = {3,6,9,12};
  Matrix *Kexp = mat_from_vals(2,2,kv);
  assert(K && mat_eq(K,Kexp,1e-12));

  matrix_free(S); matrix_free(Sexp);
  matrix_free(D); matrix_free(Dexp);
  matrix_free(K); matrix_free(Kexp);
  matrix_free(A); matrix_free(B);
}

static void test_mul(void){
  const double av[] = {1,2,3,4,5,6}; // 2x3
  const double bv[] = {7,8,9,10,11,12}; // 3x2
  Matrix *A = mat_from_vals(2,3,av);
  Matrix *B = mat_from_vals(3,2,bv);

  Matrix *P = matrix_mul(A,B);
  // Expected 2x2:
  // [1*7+2*9+3*11, 1*8+2*10+3*12] = [58,64]
  // [4*7+5*9+6*11, 4*8+5*10+6*12] = [139,154]
  const double pv[] = {58,64,139,154};
  Matrix *Pexp = mat_from_vals(2,2,pv);
  assert(P && mat_eq(P,Pexp,1e-12));

  // mismatch should return NULL
  Matrix *bad = matrix_mul(B,A); // 3x2 * 2x3 -> 3x3 valid
  assert(bad && bad->rows==3 && bad->cols==3); // sanity
  Matrix *nullmul = matrix_mul(A,A); // 2x3 * 2x3 invalid
  assert(nullmul == NULL);

  matrix_free(P); matrix_free(Pexp);
  matrix_free(bad);
  matrix_free(A); matrix_free(B);
}

static void test_trace_det(void){
  const double av[] = {1,2,3,4};
  Matrix *A = mat_from_vals(2,2,av);
  double tr = matrix_trace(A);      // 1 + 4
  assert(nearly_eq(tr, 5.0, 1e-12));
  double det = matrix_det(A);       // 1*4 - 2*3 = -2
  assert(nearly_eq(det, -2.0, 1e-10));

  const double cv[] = {6,1,1,4,-2,5,2,8,7}; // 3x3 det = -306
  Matrix *C = mat_from_vals(3,3,cv);
  double detC = matrix_det(C);
  assert(nearly_eq(detC, -306.0, 1e-8));

  matrix_free(A); matrix_free(C);
}

static void test_io(void){
  const double av[] = {1.5,2.5,3.5,4.5,5.5,6.5};
  Matrix *A = mat_from_vals(2,3,av);
  int rc = matrix_to_file(A, "test_matrix_out.txt");
  assert(rc == 0);
  Matrix *B = matrix_from_file("test_matrix_out.txt");
  assert(B && mat_eq(A,B,1e-10));
  matrix_free(A); matrix_free(B);
}

static void test_errors(void){
  Matrix *A = matrix_create(2,2);
  Matrix *B = matrix_create(3,3);
  Matrix *N1 = matrix_add(A,B);
  Matrix *N2 = matrix_sub(A,B);
  assert(N1 == NULL && N2 == NULL);
  Matrix *N3 = matrix_mul(A,B);
  assert(N3 == NULL);
  matrix_free(A); matrix_free(B);
}

int main(void){
  printf("Running matrix tests...\n");
  test_core();
  test_add_sub_scalar();
  test_mul();
  test_trace_det();
  test_io();
  test_errors();
  printf("OK\n");
  return 0;
}

