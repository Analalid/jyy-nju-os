#include <common.h>
//对应实验要求中的 kalloc；
static void *kalloc(size_t size) {
  return NULL;
}
//对应实验要求中的 kfree。
static void kfree(void *ptr) {
}
//初始化 pmm 模块，它应当在多处理器启动前 (os->init() 中) 调用。你会在这里完成数据结构、锁的初始化等
static void pmm_init() {
  uintptr_t pmsize = ((uintptr_t)heap.end - (uintptr_t)heap.start);
  printf("Got %d MiB heap: [%p, %p)\n", pmsize >> 20, heap.start, heap.end);
  
}

//1 
//2 3 
//4 5 6 7
//8 9 10 11 12 13 14 15
//
MODULE_DEF(pmm) = {
  .init  = pmm_init,
  .alloc = kalloc,
  .free  = kfree,
};
