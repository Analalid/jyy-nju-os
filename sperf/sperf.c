#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
int main(int argc, char *argv[]) {
  char *exec_argv[] = { "strace","-T", "ls", NULL, };
  // printf("%s\n", );
  char *pathVal = getenv("PATH");
  char *path;
  path = (char*)malloc(strlen("PATH=") + strlen(pathVal) + 1);
  strcpy(path, "PATH=");
  strcat(path, pathVal);
  char *exec_envp[] = { path, NULL, };
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
