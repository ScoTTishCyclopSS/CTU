#include <stdio.h>
#include <stdlib.h>

enum
{
  NOT_DONE = 0,
  DONE = 1
};

int len_of_msg(char *msg);
char *create_str(FILE *src, int *file_done, int *str_len);
int scan_str(char *str, int str_len, int msg_len, char *message);

/* The main structure. */
int main(int argc, char *argv[])
{
  char *message = argv[1];
  char *file_name = NULL; //
  int path = DONE;

  if (!argv[2])
  {
    path = NOT_DONE;
    char c;
    int size = 0;
    while ((scanf("%c", &c)) == 1 && c != '\n')
    {
      file_name = (char *)realloc(file_name, (size + 2) * sizeof(char));
      file_name[size] = c;
      size++;
    }
    file_name[size] = '\0';
  }
  else
  {
    file_name = argv[2];
  }

  int msg_len = len_of_msg(message), file_done = NOT_DONE, count = 0;

  FILE *src;
  if ((src = fopen(file_name, "r")) == NULL)
  {
    fprintf(stderr, "Crap. Can't open, sry.");
    return 1;
  }

  while (!file_done)
  {
    int str_len = 0;
    char *str = create_str(src, &file_done, &str_len);
    if (scan_str(str, str_len, msg_len, message) != 0)
      count++;
    free(str);
  }
  if (path == NOT_DONE)
  {
    free(file_name);
  }

  fclose(src);
  if (count == 0)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

/* Finds the length of a string. */
int len_of_msg(char *msg)
{
  int i;
  for (i = 0; msg[i] != '\0'; i++)
    ;
  return i;
}

/* Creates the new string. */
char *create_str(FILE *src, int *file_done, int *str_len)
{
  char *str = NULL;
  int size = *str_len;
  int str_done = NOT_DONE;
  while (!str_done)
  {
    str = (char *)realloc(str, size + 1);
    char c = fgetc(src);
    if (c == '\n')
    {
      str_done = DONE;
      break;
    }
    if (c == EOF)
    {
      *file_done = DONE;
      break;
    }
    str[size++] = c;
  }
  str[size] = '\0';
  *str_len = size;
  return str;
}

/* Finds the string with the entered search word. */
int scan_str(char *str, int str_len, int msg_len, char *message)
{
  int j = 0;
  int c = 0;
  for (int i = 0; i < str_len; i++)
  {
    if (str[i] == message[j])
    {
      j++;
      if (j == msg_len)
      {
        printf("%s\n", str);
        c++;
        break;
      }
    }
    else
    {
      i -= j;
      j = 0;
    }
  }
  return c;
}
