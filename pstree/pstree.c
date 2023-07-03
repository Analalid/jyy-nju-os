#include <stdio.h>
#include <assert.h>
void mkTree(char * filename);
int main(int argc, char *argv[]) {
  char *processes[65536];
  for (int i = 0; i < argc; i++) {
    assert(argv[i]);
    printf("argv[%d] = %s\n", i, argv[i]);
  }
  assert(!argv[argc]);
  mkTree(argv[1]);
  // dfs(1);
  return 0;
}
// void dfs(int node){
//   FILE *fp = fopen(filename, "r");
//   if (fp) {
//   // 用fscanf, fgets等函数读取
//   fclose(fp);
//   } else {
//   // 错误处理
//   }
// }
void mkTree(char * filename){
  FILE *fp = fopen(filename, "r");
  if (fp) {
  // 用fscanf, fgets等函数读取
  puts(filename);
  fclose(fp);
  } else {
  // 错误处理
  puts("error!!!!!!!!!!!!!!!!!!");
  }
}