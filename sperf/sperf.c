#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
int main(int argc, char *argv[]) {
  char *exec_argv[] = { "strace", "pwd", NULL, };
  char *exec_envp[] = { "PATH=/bin", NULL, };
  // execve("strace",          exec_argv, exec_envp);
  // execve("/usr/bin/strace", exec_argv, exec_envp);
  setbuf(stdout, NULL);
  close(STDOUT_FILENO);
  int fd = open("./sperf_tmp.output", O_CREAT|O_WRONLY|O_TRUNC,S_IRWXU);  
  if(fd < 0) perror("open file faild!\n");
  int p = fork();
  if(p < 0){
    perror("create child process error!\n");
  }else if(p > 0){
    wait(NULL);
  }else{
    printf("Hello, I am son\n");
    execve("/bin/strace",     exec_argv, exec_envp);
    perror(argv[0]);
  }
  // exit(EXIT_FAILURE);
}
