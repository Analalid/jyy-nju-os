#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
int main(int argc, char *argv[]) {
  char *exec_argv[] = {  "strace","-T","wc","sperf.c",NULL, };
  char *exec_envp[] = { "", NULL, };
  // char *pathVal = getenv("PATH");
  // char *path;
  // path = (char*)malloc(strlen("PATH=") + strlen(pathVal) + 1);
  close(2);
  open("./sperf_tmp.output", O_CREAT|O_WRONLY|O_TRUNC,S_IRWXU);
  // strcpy(path, "PATH=");
  // strcat(path, pathVal);
  // printf("%s\n====================\n", path);
  // execve("strace",          exec_argv, exec_envp);
  // execve("/usr/bin/strace", exec_argv, exec_envp);
  setbuf(stdout, NULL);
  int fd = open("./sperf_tmp.output", O_CREAT|O_WRONLY|O_TRUNC,S_IRWXU);  
  if(fd < 0) perror("open file faild!\n");
  int p = fork();
  if(p < 0){
    perror("create child process error!\n");
  }else if(p > 0){
    wait(NULL);
  }else{
    // close(STDOUT_FILENO);
    execve("/bin/strace",     exec_argv, );
    perror(argv[0]);
  }
  // exit(EXIT_FAILURE);
}
