// #include <cmath>
#include <common.h>
// #include <cstring>
#include <pmm.h>
#define MB (1 << 20)
#define MAXSIZE (16 * MB) //16MiB 能接受的最大内存分配
#define MINSIZE (1 << 12)
#define MAX_BUDDY_BLOCK_SIZE (16 * MB)//伙伴系统最大块

//testDefine
#define TESTHEAP

// #define uintptr_t int
//BUDDYS数组的长度
static int BUDDY_SIZE;
//空闲列表结构体
typedef struct node_t{
   int size;
   struct node_t * next;
} node_t;
typedef struct buddy_block{
  char *head;//保留字
  int status;//0 代表空闲
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
//伙伴系统分配
static void* balloc(size_t size){
  //TODO
  //尚未完成slab时的代替方案
  size = size > MINSIZE ? size : MINSIZE;
  //递归查找
  int v = dfs(size, MAXSIZE);
  if(v == -1) return (void *)-1;
  return (void *)0;
}
//递归查找，详情看伙伴系统的实现, 无可用空间的时候返回-1
static int dfs(size_t size, size_t curSize){
  if(curSize < size){
    //找不到可分配的空间了
    #ifdef BUDDY_SYS_DEBUG
    panic("no more space error!!!");
    #endif
    return -1;
  }
  return -1;
  //找到了对应的内存块
  // if(size == curSize){

  // }
}
//对应实验要求中的 kalloc；
static void *kalloc(size_t size) {
  //超过MAXSIZE的是不合法的申请
  if(size >= MAXSIZE || size < 0) return NULL;
  //组装头部
  size = size + sizeof(buddy_block);
  //向上对齐
  size = get2PowSize(size);
  //通过伙伴系统分配
  for(int i = 0; i < BUDDY_SIZE; ++i){
    void* res = balloc(size);
    if(res != (void*)-1) return (void*)res;
  }
  return (void*)-1;
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
static void buddy_Block_Init(buddy_block* block, int size){
  memset(block, 0, size);
}
static void buddy_init(uintptr_t start, uintptr_t end){
  BUDDY_SIZE = (end - start) / MAX_BUDDY_BLOCK_SIZE;
  int idx = 0;
  buddysArray = (buddy_block*)start;
  for(uintptr_t i = start; i < end; i += MAX_BUDDY_BLOCK_SIZE){
    buddy_Block_Init((buddy_block*)i, MAX_BUDDY_BLOCK_SIZE);
    ++idx;
  #ifdef TESTHEAP
    printf("%d       %p\n", idx, (uintptr_t)i);
  #endif
  }
  BUDDY_SIZE = idx;
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
