#include <stdio.h>
#include <assert.h>

#include<stdlib.h>
#include <string.h>

//建立目录树
void mkTree(char * dirName, int pid);
//加载进程名和Pid的映射
void loadProcessName(char *buf, int pid);
//确立父子关系
void loadProcessFather(char *buf , int pid);
//迭代打印
void dfs(int curNode, int depth);
char static processesName[65536][100];
int *processesFatherId[65536];
int  processesSonCount[65536];
int main(int argc, char *argv[]) {
  for (int i = 0; i < argc; i++) {
    assert(argv[i]);
    printf("argv[%d] = %s\n", i, argv[i]);
  }
  assert(!argv[argc]);
  for(int i = 0; i < 65536; ++i){
    //"proc/"
    mkTree(argv[1], i);
  }
  for(int i = 0; i < 26; ++i){
    printf("son:%d\n", processesFatherId[5407][i]);
  }
  // dfs(0, 0);
  return 0;
}
void dfs(int curNode, int depth){
  if(curNode == -1) return;
  printf("%d", curNode);
  // for(int i = 0; i < depth; ++i){
  //   printf("---------------");
  // }
  printf("\n");
  for(int i = 0; i < processesSonCount[curNode]; ++i){
    int son = processesFatherId[curNode][i];
    dfs(son, depth + 1);
  }
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
    for(int i = 0; i < 7; ++i){
      fgets(buf, sizeof(buf), fp);
      if(i == 0) loadProcessName(buf, pid);
      if(i == 6) loadProcessFather(buf, pid);
    }
    fclose(fp);
  } else {
    // 错误处理
    // puts("error!!!!!!!!!!!!!!!!!!");
  }
  free(num);
}

void loadProcessName(char *buf, int pid){
  int idx = 0;
  int i = 0;
  while(buf[i] != '\t'){
    ++i;
  }
  ++i;
  while(buf[i]){
    processesName[pid][idx++] = buf[i++];
  }
}

void loadProcessFather(char *buf , int pid){
  int idx = 0;
  int i = 0;
  while(buf[i] != '\t'){
    ++i;
  }
  ++i;
  int PPidV = 0;
  while(buf[i + 1]){
    PPidV *= 10;
    PPidV += buf[i] - '0';
    ++i;
  }
  ++processesSonCount[PPidV];
  int newSPace[processesSonCount[PPidV]];
  for(int i = 0; i < processesSonCount[PPidV] - 1; ++i){
    newSPace[i] = processesFatherId[PPidV][i]; 
  }
  newSPace[processesSonCount[PPidV] - 1] = pid;
  processesFatherId[PPidV] = newSPace;
  // printf("father:%d,    son:  %d,     sonCount:%d\n", PPidV, pid, processesSonCount[PPidV]);
}