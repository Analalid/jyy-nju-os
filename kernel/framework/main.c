#include <kernel.h>
#include <klib.h>

int main() {
  printf("============================");
  os->init();
  mpe_init(os->run);
  return 1;
}
