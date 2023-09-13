#include <common.h>
#include <stdio.h>
#define MB (1 << 20)
#define MAXSIZE (16 * MB) //16MiB 能接受的最大内存分配
#define MAX_BUDDY_BLOCK_SIZE (16 * MB)//伙伴系统最大块
#define MIN_BUDDY_BLOCK_SIZE (1 << 12)//伙伴系统最小块
#define BUDDY_HEAD_SIZE ((MAX_BUDDY_BLOCK_SIZE / MIN_BUDDY_BLOCK_SIZE) * 2 - 1)//伙伴头节点的个数
  
//testDefine
#define TESTHEAP 
#define BUDDY_SYS_DEBUG

// 伙伴头:记录使用情况
typedef struct buddy_head{
  void* content;//指向伙伴系统的地址
  uintptr_t status;
            //0代表完全空闲
            //1代表存在空闲
            //2代表全满
  //锁
}buddy_head;
//BUDDYS数组的长度
static int BUDDY_SIZE;
//堆的开头也是伙伴系统块数组的开头
static uintptr_t HEAP_START;
static uintptr_t BUDDY_START;
//堆的结尾
static uintptr_t HEAP_END;
//伙伴系统的结尾
static uintptr_t BUDDY_END;
//伙伴头数组的开头
static uintptr_t BUDDY_HEAD_START;
//伙伴头数组的结尾
static uintptr_t BUDDY_HEAD_END;

/*
函数声明
*/
//伙伴系统数组

//向上对齐到最接近的幂, tips:(之后替换为二分解法)
static uintptr_t get2PowSize(uintptr_t size){
  uintptr_t newSize = 1;
  while(newSize < size) newSize <<= 1;
  return newSize;
}
//维护二叉树节点
static void setBuddyHead(uintptr_t idx, buddy_head* buddy_head_base){
  uintptr_t lIdx = idx << 1, rIdx = (idx << 1) + 1;
  buddy_head* lS = buddy_head_base + lIdx * sizeof(buddy_head);
  buddy_head* rS = buddy_head_base + rIdx * sizeof(buddy_head);
  buddy_head* self = buddy_head_base + idx * sizeof(buddy_head);
  // if(lS->status == 0 && rS->status == 0) self->status = 0;
  if(lS->status == 2 && rS->status == 2) self->status = 2;
  else self->status = 1;
  // printf("%d        %d           %d\n", self->status, lS ->status, rS->status);
}
//递归查找，详情看伙伴系统的实现, 无可用空间的时候返回-1, idx用于定位伙伴头数组的位置
static uintptr_t dfs(size_t size, size_t curSize, void* baseAddr, buddy_head* buddy_head_base, uintptr_t idx){
  //找不到可分配的空间了
  if(curSize < size){
    #ifdef BUDDY_SYS_DEBUG
    printf("no more space error!!!");
    #endif
    return -1;
  }
  // printf("%d      %d      %d\n",idx, size, curSize);
  buddy_head* node = (buddy_head*)(BUDDY_HEAD_START + (idx - 1) * sizeof(buddy_head) * BUDDY_HEAD_SIZE);
  int status = node->status;
  //内存块完全被占用
  if(status == 2) return -1;
  //找到了合适大小的内存块
  if(size == curSize){
    printf("head in: %p\n", node);
    if(status == 0){
      node->status = 2;
      return idx;
    }
    return -1;
  }
  uintptr_t lIdx = idx << 1, rIdx = (idx << 1) + 1;
  if((lIdx = dfs(size, curSize >> 1, baseAddr, buddy_head_base, lIdx))){
    setBuddyHead(idx, buddy_head_base);
    return lIdx;
  }else if((rIdx = dfs(size, curSize >> 1, baseAddr, buddy_head_base, rIdx))){
    setBuddyHead(idx, buddy_head_base);
    return rIdx;
  }
  return -1;
}
//伙伴系统分配
static void* balloc(size_t size, size_t idx){
  //递归查找
  uintptr_t res = dfs(size, MAX_BUDDY_BLOCK_SIZE, (void*)BUDDY_START + idx * MAX_BUDDY_BLOCK_SIZE, (buddy_head*)(BUDDY_HEAD_START), 1);
  //BUDDY_START + size * res - (BUDDY_END - BUDDY_START)位置是算出来
  printf("init in block: %d , section: %d \n", idx, res);
  return res == -1 ? (void*)-1 : (void*)BUDDY_START + size * res - (BUDDY_END - BUDDY_START) + idx * MAX_BUDDY_BLOCK_SIZE;
}
//对应实验要求中的 kalloc；
static void *kalloc(size_t size) {
  // return (void*)HEAP_START;
  //超过MAXSIZE的是不合法的申请
  if(size >= MAXSIZE || size < 0) return NULL;
  //TODO
  //尚未完成slab时的代替方案
  size = size > MIN_BUDDY_BLOCK_SIZE ? size : MIN_BUDDY_BLOCK_SIZE;
  //向上对齐
  size = get2PowSize(size);
  //通过伙伴系统分配
  for(int i = 0; i < BUDDY_SIZE; i++){
    void* res = balloc(size, i);
    if(res != (void*)-1){
      printf("init in: %p\n", res);
      return (void*)res;
    }
  }
  return (void*)-1;
}
static void buddy_Block_Init(void* block, int size){
  memset(block, 0, size);
}
//对应实验要求中的 kfree。
static void kfree(void *ptr) {
}
static void buddy_sys_init(uintptr_t start, uintptr_t end){
  //伙伴数组的个数
  BUDDY_SIZE = (end - start) / (MAX_BUDDY_BLOCK_SIZE + BUDDY_HEAD_SIZE * sizeof(buddy_head));
  //计算出伙伴数组开头和结尾的地址
  BUDDY_START = (uintptr_t)start;
  BUDDY_END = (uintptr_t)start + BUDDY_SIZE * MAX_BUDDY_BLOCK_SIZE;
  //计算出伙伴头数组
  BUDDY_HEAD_START = (uintptr_t)BUDDY_END + MAX_BUDDY_BLOCK_SIZE;
  BUDDY_HEAD_END = (uintptr_t)BUDDY_HEAD_START + BUDDY_HEAD_SIZE * sizeof(buddy_head);
  #ifdef TESTHEAP
  int idx = 0;
  #endif
  for(uintptr_t i = BUDDY_START; i < end; i += MAX_BUDDY_BLOCK_SIZE){
    buddy_Block_Init((void*)i, MAX_BUDDY_BLOCK_SIZE);
  #ifdef TESTHEAP
    printf("%d       %p\n", idx, (uintptr_t)i);
    ++idx;
  #endif
  }
}
#ifdef TEST
static void pmm_init() {
  printf("go to TEST");
}
#else
//初始化 pmm 模块，它应当在多处理器启动前 (os->init() 中) 调用。你会在这里完成数据结构、锁的初始化等
static void pmm_init() {
  uintptr_t pmsize = ((uintptr_t)heap.end - (uintptr_t)heap.start);
  printf("Got %d MiB heap: [%p, %p)\n", pmsize >> 20, heap.start, heap.end);
  //TODO
  get2PowSize(10);
  //初始化堆
  HEAP_START = (uintptr_t)heap.start;
  HEAP_END = (uintptr_t)heap.end;

  //伙伴系统的初始化
  buddy_sys_init((uintptr_t)heap.start, (uintptr_t)heap.end);
  // printf("%p", (uintptr_t)kalloc(100));
  printf("BUDDY_END: %p\n", BUDDY_END);
}
#endif
//1 
//2 3 
//4 5 6 7
//8 9 10 11 12 13 14 15
MODULE_DEF(pmm) = {
  .init  = pmm_init,
  .alloc = kalloc,
  .free  = kfree,
};
void pWord(){
  printf("pWold!\n");
}
