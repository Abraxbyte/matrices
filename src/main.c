#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"

#define MAX_MATRICES 20 //maybe
#define NAME_LEN 32

//named matrices for interfacing with user
typedef struct {
  char name[NAME_LEN];
  Matrix *ptr;
} NamedMatrix;

//make sure we dont have too many matrices
static NamedMatrix store[MAX_MATRICES]; //static makes everything A LOT simpler
static size_t count = 0; 

void list_matrices(void) {
  printf("Stored matrices:\n");
  for (size_t i = 0; i < count; i++)
    printf("%zu: %s (%zux%zu)\n",i, store[i].name,
    store[i].ptr->rows, store[i].ptr->cols);
}

void create_matrix(void) {
  if (count >= MAX_MATRICES) {
   printf("Storage full.\n");
   return;
  }
  char name[NAME_LEN];
  size_t r, c;
  printf("Name: ");  scanf("%31s", name);
  printf("Rows Cols: ");  scanf("%zu %zu", &r, &c);
  Matrix *m = matrix_create(r, c);
  if (!m) { printf("Allocation failed.\n"); return; }
  strcpy(store[count].name, name);
  store[count].ptr = m;
  count++;

}
//Deletes a matrix
void delete_matrix(void){
  char choice[NAME_LEN];
  printf("Which matrix do you wish to delete?");
  scanf("%31s", choice);
  for (size_t i=0; i < count; i++){
    if(strcmp (store[i].name, choice) == 0){
      matrix_free(store[i].ptr);
      //shift down remaining matrices
      for (size_t j= i; j < count; j++) store[j] = store[j+1];
      count--;
      printf("Matrix has been Deleted!\n");
      return;
    }
    printf("Matrix not found!\n");
  }
}
void print_matrix(void){
  char choice[NAME_LEN];
  printf("Which matrix do you wish to print?");
  scanf("%31s", choice);
  for (size_t i=0; i < count; i++){
    if(strcmp (store[i].name, choice) == 0){
    matrix_print(store[i].ptr);
    return;
    }
  }
}

void make_identity(void){
  char choice[NAME_LEN];
  printf("Which matrix do you wish to make identity?\n");
  scanf("%31s", choice);
  for (size_t i=0; i < count; i++){
    if(strcmp (store[i].name, choice) == 0){
    matrix_identity(store[i].ptr);
    return;
    }
  }
}



void add_matrices(void){
  char a[NAME_LEN];
  char b[NAME_LEN];
  printf("First Matrix");
  scanf("%31s", a);
  printf("Second Matrix ");
  scanf("%31s", b);
  size_t x = MAX_MATRICES;
  size_t y = MAX_MATRICES;
  for (size_t i=0; i < count; i++){
    if (strcmp(store[i].name, a) == 0) { x = i;}
    if (strcmp(store[i].name, b) == 0) { y = i;}
  }
  if(x == MAX_MATRICES || y == MAX_MATRICES)   printf("Matrix doesnt exist!\n");
  Matrix* sum = matrix_add(store[x].ptr, store[y].ptr);
  matrix_print(sum);
  matrix_free(sum);
}

int main(void) {
  int choice;
  while (1) {
    //ask user for input
    printf("\nMatrix Manager\n");
    printf("1. Create matrix\n");
    printf("2. Delete matrix\n");
    printf("3. List matrices\n");
    printf("4. Print matrix\n");
    printf("5. Add two matrices\n");
    printf("6. Make identity\n");
    printf("0. Exit\n");
    printf("Choice: ");
    if (scanf("%d", &choice) != 1) break;

    // read user input
    switch (choice) {
    case 1: create_matrix(); break;
    case 2: delete_matrix(); break;
    case 3: list_matrices(); break;
    case 4: print_matrix(); break;
    case 5: add_matrices(); break;
    case 6: make_identity(); break;
    case 0: goto done; //goto justification:  need to exit switch and while (break wont work)
    default: printf("Invalid choice\n");
    }
  }
  //quit and free up memory
  done:
    for (size_t i = 0; i < count; i++)
    matrix_free(store[i].ptr);
    return 0;
}
