#include <common.h>
#include <stdio.h>


#define alloc(sz) pmm->alloc(sz)
#define free(addr) pmm->free(addr)
void alloc_and_free() {
	void *a;
	a = alloc(1);				
  printf("%p", a);
  	// free(a);
	// a = alloc(128);				free(a);
	// a = alloc(4096);			free(a);
	// a = alloc(1 << 16);		free(a);
	// a = alloc(1 << 17);		free(a);
}
void pmm_test(){
  alloc_and_free();
  printf("endl\n");
}


static void os_init() {
  pmm->init();
}

static void os_run() {
  for (const char *s = "Hello World from CPU #*\n"; *s; s++) {
    putch(*s == '*' ? '0' + cpu_current() : *s);
  }
   pmm_test();
  while (1) ;
}

MODULE_DEF(os) = {
  .init = os_init,
  .run  = os_run,
};
