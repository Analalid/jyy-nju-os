#include <common.h>
#include <pmm.h>
#define MB (1 << 20)
#define MAXSIZE (16 * MB) //16MiB 能接受的最大内存分配
#define MINSIZE (1 << 12)
#define MAX_BUDDY_BLOCK_SIZE (16 * MB)//伙伴系统最大块

// #define uintptr_t int
//BUDDYS数组的长度
static int BUDDY_SIZE;
//空闲列表结构体
typedef struct node_t{
   int size;
   struct node_t * next;
} node_t;
typedef struct buddy_block{
  char *head;
  int status;
  //锁
}buddy_block;
//伙伴系统数组
struct buddy_block *buddysArray;
//向上对齐到最接近的幂, tips:(之后替换为二分解法)
static int get2PowSize(int size){
  int newSize = 1;
  while(newSize < size) newSize <<= 1;
  return newSize;
}
//对应实验要求中的 kalloc；
static void *kalloc(size_t size) {
  //超过MAXSIZE的是不合法的申请
  if(size >= MAXSIZE || size < 0) return NULL;
  //组装头部
  // size = size + sizeof(node_t); 
  return NULL;
}
//对应实验要求中的 kfree。
static void kfree(void *ptr) {
}
//初始化 pmm 模块，它应当在多处理器启动前 (os->init() 中) 调用。你会在这里完成数据结构、锁的初始化等
static void pmm_init() {
  uintptr_t pmsize = ((uintptr_t)heap.end - (uintptr_t)heap.start);
  printf("Got %d MiB heap: [%p, %p)\n", pmsize >> 20, heap.start, heap.end);
  get2PowSize(10);
  //伙伴系统的初始化
  buddy_init((uintptr_t)heap.start, (uintptr_t)heap.end);
}
static void buddy_init(uintptr_t start, uintptr_t end){
  BUDDY_SIZE = (end - start) / MAX_BUDDY_BLOCK_SIZE;
  int idx = 0;
  for(uintptr_t i = start; i < end; i += MAX_BUDDY_BLOCK_SIZE){
    buddysArray = (buddy_block*)i;
    ++idx;
  #ifdef TESTHEAP
    printf("%d       %p\n", idx, (uintptr_t)i);
  #endif
  }
}
//1 
//2 3 
//4 5 6 7
//8 9 10 11 12 13 14 15
MODULE_DEF(pmm) = {
  .init  = pmm_init,
  .alloc = kalloc,
  .free  = kfree,
};
