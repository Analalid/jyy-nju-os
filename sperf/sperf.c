#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <regex.h>
int main(int argc, char *argv[]) {
  char *exec_argv[] = {  "strace","-T","wc","sperf.c",NULL, };
  char *exec_envp[] = { "PATH=/bin", NULL, };
  close(2);
  // open("./sperf_tmp.output", O_CREAT|O_WRONLY|O_TRUNC,S_IRWXU);
  setbuf(stdout, NULL);
  int fd = open("./sperf_tmp.output", O_CREAT|O_WRONLY|O_TRUNC,S_IRWXU);  
  if(fd < 0) perror("open file faild!\n");
  int p = fork();
  if(p < 0){
    perror("create child process error!\n");
  }else if(p > 0){
    wait(NULL);
    printf("father process begin!\n");
    
  }else{
    execve("/bin/strace",     exec_argv, exec_envp);
    perror(argv[0]);
  }
}
