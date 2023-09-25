#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
int main(int argc, char *argv[]) {
  char *exec_argv[] = { "strace", "ls", NULL, };
  char *exec_envp[] = { "PATH=/bin", NULL, };
  // execve("strace",          exec_argv, exec_envp);
  // execve("/usr/bin/strace", exec_argv, exec_envp);
  int p = fork();
  if(p < 0){

  }else if(p > 0){
    wait(NULL);
  }else{
    close(STDOUT_FILENO);
    open("./sperf_tmp.output", O_CREAT|O_WRONLY|O_TRUNC,S_IRWXU);  
    // execve("/bin/strace",     exec_argv, exec_envp);
    // printf("Hello, I am son\n");
  }
  perror(argv[0]);
  exit(EXIT_FAILURE);
}
