#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
char make_alphabet(char *);
void print_array(char *, int);
int is_inside_abc(char *, int, char *);
void shift(char *, int, int, char *);
int compare(char *, char *, int);
 
/* The main structure. */
int main(int argc, char *argv[])
{
  char alphabet[52];
  char src, src2;
  int size = 0, size2 = 0, max = 0, offset = 0;
  char *main_arr = NULL, *check_arr = NULL;
   
  make_alphabet(alphabet);
 
  while ((scanf("%c", &src)) == 1 && src != '\n') {
    main_arr = (char *)realloc(main_arr, (size + 1) * sizeof(char));
    main_arr[size] = src;
    size++;
  }
  while ((scanf("%c", &src2)) == 1 && src2 != '\n') {
    check_arr = (char *)realloc(check_arr, (size2 + 1) * sizeof(char));
    check_arr[size2] = src2;
    size2++;
  }
 
  if (is_inside_abc(main_arr, size, alphabet) != size || is_inside_abc(check_arr, size2, alphabet) != size2) {
    fprintf(stderr, "Error: Chybny vstup!\n");
    free(main_arr);
    free(check_arr);
    return 100;
  } else if (size2 != size) {
    fprintf(stderr, "Error: Chybna delka vstupu!\n");
    free(main_arr);
    free(check_arr);
    return 101;
  }
 
  for (int i = 0; i <= 52; i++) {
    shift(main_arr, 1, size, alphabet);
    int c = compare(main_arr, check_arr, size);
    if (c > max) {
      max = c;
      offset = i;
    }
  }
 
  shift(main_arr, offset, size, alphabet);
  print_array(main_arr, size);
  free(main_arr);
  free(check_arr);
  return 0;
}
 
 
/* Making alphabet, then returns it. */
char make_alphabet(char *alphabet)
{
  int letter = 65;
  for (int i = 0; i < 26; i++) {
    alphabet[i] = (char)letter;
    letter++;
  }
  letter = 97;
  for (int i = 26; i < 52; i++) {
    alphabet[i] = (char)letter;
    letter++;
  }
  return *alphabet;
}
 
/* Outputs the incoming array. */
void print_array(char *arr, int size)
{
  for (int i = 0; i < size; i++) {
    printf("%c", arr[i]);
  }
  printf("\n");
}
 
/* Shifts the received message alphabetically. */
void shift(char *src, int offset, int size, char *alphabet)
{
  char *p = alphabet + 52;
  int d = 0, j;
  for (int i = 0; i < size; i++) {
    for (j = 0; j <= 51; j++) {
      if (src[i] == alphabet[j]) {
        break;
      }
    }
    d = j - offset;
    if (d < 0) {
      src[i] = p[d];
    } else {
      src[i] = alphabet[d];
    }
  }
}
 
/* Compares a received message with a partially listened message. */
int compare(char *str1, char *str2, int size)
{
  int counter = 0;
  for (int i = 0; i < size; i++) {
    if (str1[i] == str2[i]) {
      counter++;
    }
  }
  return counter;
}
 
/* Checks if there is a character in the set of valid characters for reading. */
int is_inside_abc(char *arr, int size, char *alphabet)
{
  int checker = 0;
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < 52; j++) {
      if (arr[i] == alphabet[j]) {
        checker++;
      }
    }
  }
  return checker;
}
