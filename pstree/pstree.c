#include <stdio.h>
#include <assert.h>

int main(int argc, char *argv[]) {
  for (int i = 0; i < argc; i++) {
    assert(argv[i]);
    printf("argv[%d] = %s\n", i, argv[i]);
  }
  assert(!argv[argc]);
  return 0;
}
// void Dfs(){
//   FILE *fp = fopen(filename, "r");
//   if (fp) {
//   // 用fscanf, fgets等函数读取
//   fclose(fp);
//   } else {
//   // 错误处理
//   }
// }
