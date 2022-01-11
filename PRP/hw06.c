#include <stdio.h>
#include <stdlib.h>
 
void print_mat(int row, int col, int mat[][col]);
int scan_mat(int row, int col, int mat[][col]);
int mult_mat(int row, int col, int col2, int mat[][col], int mat2[][col2], int res[][col2]);
int add_mat(int row, int col, int col2, int mat[][col], int mat2[][col2], int res[][col2]);
int sub_mat(int row, int col, int col2, int mat[][col], int mat2[][col2], int res[][col2]);
int op_block(int row, int col, int row2, int col2, int A[][col], int B[][col2], char op);
int err_block(int row, int col, int row2, int col2, char op);
 
/* The main structure. */
int main(int argc, char *argv[])
{
  int row = 0, col = 0;
  int row2 = 0, col2 = 0;
  char op = 0;
 
  if (scanf("%d %d", &row, &col) != 2) {
    fprintf(stderr, "Error: Chybny vstup!\n");
    return 100;
  }
  int A[row][col];
  if (scan_mat(row, col, A) != row * col) {
    fprintf(stderr, "Error: Chybny vstup!\n");
    return 100;
  }
 
  if (scanf(" %c", &op) != 1) {
    fprintf(stderr, "Error: Chybny vstup!\n");
    return 100;
  }
 
  if (scanf("%d %d", &row2, &col2) != 2) {
    fprintf(stderr, "Error: Chybny vstup!\n");
    return 100;
  }
  int B[row2][col2];
  if (scan_mat(row2, col2, B) != row2 * col2) {
    fprintf(stderr, "Error: Chybny vstup!\n");
    return 100;
  }
 
  if (err_block(row, col, row2, col2, op) == 0) {
    op_block(row, col, row2, col2, A, B, op);
  } else {
    return 100;
  }
}
 
 
/* Outputs the matrix. */
void print_mat(int row, int col, int mat[][col])
{
  printf("%d %d\n", row, col);
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      printf("%d", mat[i][j]);
      if (j != col - 1) { //outputs whitespace unless is the last element
        printf(" ");
      }
    }
    printf("\n");
  }
}
 
/* Allows to input the matrix. */
int scan_mat(int row, int col, int mat[][col])
{
  int checker = 0; //сounter for matrix elements
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      if (scanf("%d", &mat[i][j]) == 1) {
        checker++;
      }
    }
  }
  return checker;
}
 
/* Multiplies matrices. */
int mult_mat(int row, int col, int col2, int mat[][col], int mat2[][col2], int res[][col2])
{
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col2; j++) {
      res[i][j] = 0;
      for (int k = 0; k < col; k++) {
        res[i][j] += mat[i][k] * mat2[k][j];
      }
    }
  }
  return res[row][col2];
}
 
/* Adds matrices. */
int add_mat(int row, int col, int col2, int mat[][col], int mat2[][col2], int res[][col2])
{
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col2; j++) {
      res[i][j] = 0; //zeroing the element to avoid errors
      res[i][j] += mat[i][j] + mat2[i][j];
    }
  }
  return res[row][col2];
}
 
/* Subtracts matrices. */
int sub_mat(int row, int col, int col2, int mat[][col], int mat2[][col2], int res[][col2])
{
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col2; j++) {
      res[i][j] = 0; //zeroing the element to avoid errors
      res[i][j] += mat[i][j] - mat2[i][j];
    }
  }
  return res[row][col2];
}
 
/* Block with operations for matrices. */
int op_block(int row, int col, int row2, int col2, int A[][col], int B[][col2], char op)
{
  int res[row][col2];
  switch (op) {
  case '-':
    sub_mat(row, col, col2, A, B, res);
    print_mat(row, col2, res);
    break;
 
  case '*':
    mult_mat(row, col, col2, A, B, res);
    print_mat(row, col2, res);
    break;
 
  case '+':
    add_mat(row, col, col2, A, B, res);
    print_mat(row, col2, res);
    break;
 
  default:
    break;
  }
  return 0;
}
 
/* Block with conditions and errors for every operation. */
int err_block(int row, int col, int row2, int col2, char op)
{
  int status = 0;
  switch (op) {
  case '+':
  case '-':
    if (row != row2 || col != col2) {
      fprintf(stderr, "Error: Chybny vstup!\n");
      status = 1;
    } break;
 
  case '*':
    if (col != row2) {
      fprintf(stderr, "Error: Chybny vstup!\n");
      status = 1;
    } break;
 
  default:
    break;
  }
  return status;
}
