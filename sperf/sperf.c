#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <regex.h>
#include <stdint.h>
 #define ARRAY_SIZE(arr) (sizeof((arr)) / sizeof((arr)[0]))
static const char *const re = "<([0-9]+)>";
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
    char line[4096];  // 用于存储读取的行数据
    char ch;
    ssize_t bytesRead;
    int index = 0;
    while ((bytesRead = read(fd, &ch, 1)) > 0) {
      if (ch == '\n') {
          line[index] = '\0';  // 添加字符串结尾标志
          // printf("读取的行数据: %s\n", line);
          char* substring = strndup(&line[0], index);
    printf("ashdgafjsgdfasj\n================================================\n\n\n");
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
  int fd = open("./sperf_tmp.output", O_CREAT|O_WRONLY|O_TRUNC,S_IRWXU);  
  if(fd < 0) perror("open file faild!\n");
  int p = fork();
  if(p < 0){
    perror("create child process error!\n");

  }else if(p > 0){
    wait(NULL);
    printf("father process begin!\n");
    readTmpOutFile(fd);
    close(fd);
  }else{
    execve("/bin/strace",     exec_argv, exec_envp);
    perror(argv[0]);
  }
}
