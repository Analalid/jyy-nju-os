#include <stdio.h>
#include <assert.h>
void mkTree(char * filename);
int main(int argc, char *argv[]) {
  char *processesName[65536];
  // int * processesFatherId[65536];
  // processesFatherId[1] = 0;
  for (int i = 0; i < argc; i++) {
    assert(argv[i]);
    printf("argv[%d] = %s\n", i, argv[i]);
  }
  assert(!argv[argc]);
  //"/proc"
  mkTree(argv[1]);
  // dfs(0);
  return 0;
}
// void dfs(int pid){
    // if(pid == -1) return;
// }
void mkTree(char * filename){
  FILE *fp = fopen(filename, "r");
  int pid;
  if (fp) {
  // 用fscanf, fgets等函数读取
  puts(filename);
  fscanf(fp, "%d", &pid);
  printf("pid:%d", pid);
  fclose(fp);
  } else {
  // 错误处理
  puts("error!!!!!!!!!!!!!!!!!!");
  }
}