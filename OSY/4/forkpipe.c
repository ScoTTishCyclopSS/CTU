/*
 * autor: @timusfed
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

#define EXIT_SUCC_MSG "OK\n"
#define EXIT_FAILURE_MSG "ERROR\n"
#define SYS_ERR 2

bool endCycle = false;

void genEnd(int signal)
{
  if (signal == SIGTERM)
  {
    endCycle = true;
  }
}

int main(int argc, char *argv[])
{
  int piper[2];
  if (pipe(piper) < 0)
  {
    return SYS_ERR;
  }

  /*=========== Creating 1st Child ==========*/
  pid_t GEN = fork();

  if (GEN < 0)
  {
    return SYS_ERR;
  }

  // code for GEN
  else if (GEN == 0)
  {
    // declare signal
    if (signal(SIGTERM, genEnd) == SIG_ERR)
    {
      return SYS_ERR;
    }
    // replace standard output with pipe output
    if (dup2(piper[1], 1) < 0)
    {
      return SYS_ERR;
    }
    // close unused
    if (close(piper[0]) < 0 || close(piper[1]) < 0)
    {
      return SYS_ERR;
    }
    // print test to stdout
    while (!endCycle)
    {
      if (!printf("%d %d\n", rand(), rand()))
      {
        return SYS_ERR;
      }
      fflush(stdout);
      if (sleep(1) < 0)
      {
        return SYS_ERR;
      }
    }
    fprintf(stderr, "GEN TERMINATED\n");
    return EXIT_SUCCESS;
  }

  /*=========== Creating 2nd Child ==========*/
  pid_t NSD = fork();

  if (NSD < 0)
  {
    return SYS_ERR;
  }

  // code for NSD
  else if (NSD == 0 && GEN)
  {
    // replace standard input with input part of pipe
    if (dup2(piper[0], 0) == -1)
    {
      return SYS_ERR;
    }
    // close unused
    if (close(piper[0]) < 0 || close(piper[1]) < 0)
    {
      return SYS_ERR;
    }
    char *arg = "nsd";
    if (execl(arg, arg, NULL) == -1)
    {
      return SYS_ERR;
    }
  }

  /*=========== The main ==========*/
  // close pipe
  if (close(piper[1]) < 0 || close(piper[0]) < 0)
  {
    return SYS_ERR;
  }
  // wait few sec
  if (sleep(5) < 0)
  {
    return SYS_ERR;
  }
  // KILL gen (R.I.P)
  if (kill(GEN, SIGTERM) == -1)
  {
    return SYS_ERR;
  }
  // wait for killing to end (both of the children)
  int st_gen = 0;
  int st_nsd = 0;
  if (waitpid(GEN, &st_gen, 0) < 0)
  {
    return SYS_ERR;
  }
  if (waitpid(NSD, &st_nsd, 0) < 0)
  {
    return SYS_ERR;
  }
  // check if terminated, god sake 5th try i guess
  int ret_gen = 0;
  int ret_nsd = 0;
  if (WIFEXITED(st_gen))
  {
    ret_gen = WEXITSTATUS(st_gen);
  }
  if (WIFEXITED(st_nsd))
  {
    ret_nsd = WEXITSTATUS(st_nsd);
  }
  // final exit (if both childs terminated successfully)
  if (ret_gen != EXIT_SUCCESS || ret_nsd != EXIT_SUCCESS)
  {
    printf(EXIT_FAILURE_MSG);
    return EXIT_FAILURE;
  }
  else
  {
    printf(EXIT_SUCC_MSG);
    return EXIT_SUCCESS;
  }
}
