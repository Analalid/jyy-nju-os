#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  char *exec_argv[] = { "sudo strace", "ls", NULL, };
  char *exec_envp[] = { "PATH=/bin", NULL, };
  execve("/", exec_argv, exec_envp);
  // execve("strace",          exec_argv, exec_envp);
  // execve("/bin/strace",     exec_argv, exec_envp);
  // execve("/usr/bin/strace", exec_argv, exec_envp);
  perror(argv[0]);
  exit(EXIT_FAILURE);
}
