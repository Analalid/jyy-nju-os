#include <stdio.h>
#include <assert.h>

#include<stdlib.h>
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
    //"proc/"
    mkTree(argv[1], i);
  }
  // dfs(0);
  return 0;
}
void mkTree(char *dirName, int pid){
  char *num;
  int pidLen;
  //获取转换后的字符串长度
  pidLen = snprintf(NULL, 0, "%d", pid);
  // 分配足够的空间来容纳转换后的字符串
  num = (char*)malloc(pidLen + 1);
  // 将 num 转换为字符数组
  snprintf(num, pidLen + 1, "%d", pid);

  
  int len = strlen(dirName) + pidLen + 1;
  char filename[len];

  //将dirName复制到filename里
  strcpy(filename, dirName);

  //拼接数字
  for (int i = 0; i <= pidLen; i++) {
    filename[strlen(dirName) + i] = num[i];
  }
  
  FILE *fp = fopen(filename, "r");
  char buf[1024];
  // puts(filename);
  if (fp) {
  // 用fscanf, fgets等函数读取
  fscanf(fp, "%d", &pid);
  printf("pid:%d\n", pid);
  fgets(buf, sizeof(buf), fp);
  printf("Read line: %s\n", buf);
  fclose(fp);
  // puts("error!!!!!!!!!!!!!!!!!!");
  } else {
  // 错误处理
  }
  free(num);
}