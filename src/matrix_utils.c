#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>



//print matrix to stdout
void matrix_print(const Matrix *m){
  if (!m) return;
  for (size_t i = 0; i < m->rows; i++){
    for (size_t j = 0; j < m->cols; j++){
       printf("%8.2f ", m->data[i * m->cols + j]);
    }
    printf("\n");
  }
}


//save matrix to text file 
int matrix_to_file(const Matrix *m, const char *path) {
    if (!m || !path) return -1;
    FILE *f = fopen(path, "w");
    if (!f) return -1;

    fprintf(f, "%zu %zu\n", m->rows, m->cols);
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++)
            fprintf(f, "%.10g ", m->data[i * m->cols + j]);
        fprintf(f, "\n");
    }
    fclose(f);
    return 0;
}

//Load matrix from txt file
Matrix *matrix_from_file(const char *path) {
    if (!path) return NULL;
    FILE *f = fopen(path, "r");
    if (!f) return NULL;

    size_t rows, cols;
    if (fscanf(f, "%zu %zu", &rows, &cols) != 2) {
        fclose(f);
        return NULL;
    }

    Matrix *m = matrix_create(rows, cols);
    if (!m) {
        fclose(f);
        return NULL;
    }

    for (size_t i = 0; i < rows * cols; i++)
        if (fscanf(f, "%lf", &m->data[i]) != 1) {
            matrix_free(m);
            fclose(f);
            return NULL;
        }

    fclose(f);
    return m;
}
