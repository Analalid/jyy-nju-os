#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <regex.h>
#include <stdint.h>
//TODO
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
 #define ARRAY_SIZE(arr) (sizeof((arr)) / sizeof((arr)[0]))
static const char *const re = "<([0-9]+)>";
//TODO
int isFileOpen(int fd) {
    int flags = fcntl(fd, F_GETFL);
    if (flags == -1) {
        perror("Failed to get file flags");
        return -1;
    }

    struct stat fileStat;
    fstat(fd, &fileStat);
    off_t fileSize = fileStat.st_size;
    printf("File size: %ld bytes\n", fileSize);
    return (flags & O_ACCMODE) != O_RDONLY;
}

void insertString(char *str){
  char* key;
  char* value;

  char *s = str;
  regex_t     regex;
  regmatch_t  pmatch[1];
  regoff_t    off, len;


  for (unsigned int i = 0; ; i++) {
    if (regexec(&regex, s, ARRAY_SIZE(pmatch), pmatch, 0))
        break;
    off = pmatch[0].rm_so + (s - str);
    len = pmatch[0].rm_eo - pmatch[0].rm_so;
  //    printf("#%zu:\n", i);
    printf("offset = %jd; length = %jd\n", (intmax_t) off,
            (intmax_t) len);
    printf("substring = \"%.*s\"\n", len, s + pmatch[0].rm_so);
    s += pmatch[0].rm_eo;
  }
}
void readTmpOutFile(int fd){
    printf("%d\n", fd);
    char line[4096];  // 用于存储读取的行数据
    char ch;
    ssize_t bytesRead;
    int index = 0;
      printf("ashdgafjsgdfasj\n================================================\n\n\n");
      printf("%ld\n",(long)(bytesRead = read(fd, &ch, 1)));
      fprintf(stdout, "Error reading file: %s\n", strerror(errno));


    while ((bytesRead = read(fd, &ch, 1)) > 0) {
      if (ch == '\n') {
          line[index] = '\0';  // 添加字符串结尾标志
          // printf("读取的行数据: %s\n", line);
          char* substring = strndup(&line[0], index);
          insertString(substring);
          free(substring);
          index = 0;  // 重置索引
      } else {
          line[index] = ch;
          index++;
      }
  }

}
int main(int argc, char *argv[]) {
  char *exec_argv[] = {  "strace","-T","wc","sperf.c",NULL, };
  char *exec_envp[] = { "PATH=/bin", NULL, };
  close(2);
  // open("./sperf_tmp.output", O_CREAT|O_WRONLY|O_TRUNC,S_IRWXU);
  setbuf(stdout, NULL);
  int fd = open("./sperf_tmp.output", O_CREAT|O_WRONLY,S_IRWXU);  
  
  if(fd < 0) perror("open file faild!\n");
  int p = fork();
  if(p < 0){
    perror("create child process error!\n");
  }else if(p > 0){
    wait(NULL);
    // close(fd);
    // fd = open("./sperf_tmp.output", O_CREAT|O_WRONLY,S_IRWXU);  
    printf("father process begin!\n");
    int t = isFileOpen(fd);
    printf("isopen: %d\n", t);
    readTmpOutFile(fd);
    close(fd);
  }else{
    execve("/bin/strace",     exec_argv, exec_envp);
    perror(argv[0]);
  }
}
