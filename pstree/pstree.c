#include <stdio.h>
#include <assert.h>

#include<stdlib.h>
#include <string.h>


void mkTree(char * dirName, int pid);
void loadProcessName(char *buf, int pid);
char processesName[65536][100];
int main(int argc, char *argv[]) {
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
  //尾部
  char status[] = "/status";
  int tairLen = strlen(status);
  //获取转换后的字符串长度
  pidLen = snprintf(NULL, 0, "%d", pid);
  // 分配足够的空间来容纳转换后的字符串
  num = (char*)malloc(pidLen + 1);
  // 将 num 转换为字符数组
  snprintf(num, pidLen + 1, "%d", pid);

  
  int len = strlen(dirName) + pidLen + tairLen + 1;
  char filename[len];

  //将dirName复制到filename里
  strcpy(filename, dirName);

  //拼接数字
  for (int i = 0; i <= pidLen; i++) {
    filename[strlen(dirName) + i] = num[i];
  }
  //拼接尾部
  for(int i = 0; i <= tairLen; ++i){
    filename[strlen(dirName) + pidLen + i] = status[i];
  }

  FILE *fp = fopen(filename, "r");
  char buf[1024];
  if (fp) {
  // 用fscanf, fgets等函数读取
    for(int i = 0; i < 6; ++i){
      fgets(buf, sizeof(buf), fp);
      if(i != 0 && i != 5) continue;
      // if(i == 0){
      // }
      // printf("id:%d\t", pid);
      // printf("%s", buf);
      loadProcessName(buf, pid);
    }
    fclose(fp);
  } else {
    // 错误处理
    // puts("error!!!!!!!!!!!!!!!!!!");
  }
  free(num);
}

void loadProcessName(char *buf, int pid){
  // int i = 0;
  // while(buf){
  //   processesName[pid][i] = buf[i];
  //   buf = buf + 1;
  // }
  int idx = 0;
  int i = 0;
  while(buf[i] != '\t'){
    // printf("%c",buf[i]);
    ++i;
  }
  ++i;
  while(buf[i]){
    processesName[pid][idx++] = buf[i++];
  }
  // processesName[pid][idx] = '\n';
  printf("%s", processesName[pid]);
}