#include <kernel.h>
#include <klib.h>

int main() {
  os->init();
  mpe_init(os->run);
  printf("============================");
  return 1;
}
