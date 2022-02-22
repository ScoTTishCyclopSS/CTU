/*
 * @author: timusfed
 */
 
#define STDOUT_FILENO 1
#define STDIN_FILENO 0
 
#define SYS_exit 1
#define SYS_read 3
#define SYS_write 4
 
void myExit(int val)
{ // exit(int status);
  asm("int $0x80"
      :
      : "a"(SYS_exit), "b"(val)); // in
}
 
int write(int fd, char *buf, int count)
{ // write(int fd, const void *buf, size_t count);
  int result;
  asm("int $0x80"
      : "=a"(result) // out
      : "a"(SYS_write), "b"(fd), "c"(buf), "d"(count)
      : "memory"); // in
  return result;
}
 
int read(int fd, char *buf, int count)
{ // read(int fd, void *buf, size_t count);
  int result;
  asm("int $0x80"
      : "=a"(result) // out
      : "a"(SYS_read), "b"(fd), "c"(buf), "d"(count)
      : "memory"); // in
  return result;
}
 
int isnum(char ch)
{
  return ch >= '0' && ch <= '9';
}
 
int isspc(char ch)
{
  return ch == ' ' || ch == '\n';
}
 
int lower(int num)
{
  if (num >= 65 && num <= 90)
  {
    num += 32;
  }
  return num;
}
 
static void print(unsigned num)
{
  // todo : get num, make hex, add 0, add x, vise versa buf
  char buf[20], hex[20];
  int counter = 0;
 
  buf[counter++] = '\n';
 
  if (num == 0)
  {
    buf[counter++] = '0';
  }
 
  while (num > 0)
  {
    int local = num % 16;
    local += local < 10 ? 48 : 55;
    buf[counter++] = lower(local);
    num /= 16;
  }
 
  buf[counter++] = 'x';
  buf[counter++] = '0';
 
  for (int i = counter, j = 0; i > 0; i--, j++)
  {
    hex[i] = buf[j];
  }
 
  int ret = write(STDOUT_FILENO, hex, counter + 1);
  if (ret == -1)
    myExit(1); // TODO your new exit
}
 
/* TODO: main() is called by libc. Real entry point is _start(). */
int _start()
{
  char buf[20];
  unsigned num = 0;
  int i;
  int num_digits = 0;
  unsigned chars_in_buffer = 0;
 
  for (/* no init */; /* no end condition */; i++, chars_in_buffer--)
  {
    if (chars_in_buffer == 0)
    {
      int ret = read(STDIN_FILENO, buf, sizeof(buf));
      if (ret < 0)
        myExit(1); // TODO replace by exit
      i = 0;
      chars_in_buffer = ret;
    }
    if (
        num_digits > 0 && (chars_in_buffer == 0 /* EOF */ || !isnum(buf[i])))
    {
      print(num);
      num_digits = 0;
      num = 0;
    }
    if (
        chars_in_buffer == 0 /* EOF */
        || (!isspc(buf[i]) && !isnum(buf[i])))
      myExit(0); // TODO: replace by exit
 
    if (isnum(buf[i]))
    {
      num = num * 10 + buf[i] - '0';
      num_digits++;
    }
  }
}