#include <stdio.h>
#include <assert.h>

#include <string.h>

void mkTree(char * dirName, int pid);
int main(int argc, char *argv[]) {
  char *processesName[65536];
  // int * processesFatherId[65536];
  // processesFatherId[1] = 0;
  for (int i = 0; i < argc; i++) {
    assert(argv[i]);
    printf("argv[%d] = %s\n", i, argv[i]);
  }
  assert(!argv[argc]);
  for(int i = 0; i < 65536; ++i){
    //"proc"
    mkTree(argv[1], i);
  }
  // dfs(0);
  return 0;
}
void mkTree(char *dirName, int pid){
  char filename[100];
  char num[10];
  sprintf(num, "%d", pid);
  strcat(filename, dirName);
  strcat(filename, num);
  puts("=========================");
  puts(filename);
  
  FILE *fp = fopen(filename, "r");
  char buf[1024];
  if (fp) {
  // 用fscanf, fgets等函数读取
  fscanf(fp, "%d", &pid);
  printf("pid:%d\n", pid);
  fgets(buf, sizeof(buf), fp);
  printf("Read line: %s\n", buf);
  fclose(fp);
  } else {
  // 错误处理
  puts("error!!!!!!!!!!!!!!!!!!");
  }
}