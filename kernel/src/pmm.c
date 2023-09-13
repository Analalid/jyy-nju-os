#include <common.h>
#include <stdint.h>
#include <stdio.h>
#define MB (1 << 20)
#define MAXSIZE (16 * MB) //16MiB 能接受的最大内存分配
#define MAX_BUDDY_BLOCK_SIZE (16 * MB)//伙伴系统最大块
#define MIN_BUDDY_BLOCK_SIZE (1 << 12)//伙伴系统最小块
#define BUDDY_HEAD_SIZE ((MAX_BUDDY_BLOCK_SIZE / MIN_BUDDY_BLOCK_SIZE) * 2 - 1)//每个组的伙伴头节点的个数
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
//传入headBaseAddress和idx， 返回所在的buddy_head
static inline buddy_head* getBuddyHead(void* headBaseAddress, uintptr_t idx){
    return (buddy_head*)(headBaseAddress + (idx - 1) * sizeof(buddy_head));
}
//向上对齐到最接近的幂, tips:(之后替换为二分解法)
static uintptr_t get2PowSize(uintptr_t size){
  uintptr_t newSize = 1;
  while(newSize < size) newSize <<= 1;
  return newSize;
}
//维护二叉树节点
// static void setBuddyHead(uintptr_t idx, buddy_head* buddy_head_base){
//   uintptr_t lIdx = idx << 1, rIdx = (idx << 1) + 1;
//   buddy_head* lS = buddy_head_base + (lIdx - 1) * sizeof(buddy_head);
//   buddy_head* rS = buddy_head_base + (rIdx - 1) * sizeof(buddy_head);
//   buddy_head* self = buddy_head_base + (idx - 1) * sizeof(buddy_head);
//   // if(lS->status == 0 && rS->status == 0) self->status = 0;
//   if(lS->status == 2 || rS->status == 2) self->status = 1;
//   // printf("%d        %d           %d\n", self->status, lS ->status, rS->status);
// }
//递归查找，详情看伙伴系统的实现, 无可用空间的时候返回-1, idx用于定位伙伴头数组的位置
static uintptr_t dfs(size_t size, size_t curSize, void* baseAddr, buddy_head* buddy_head_base, uintptr_t idx){
  //找不到可分配的空间了
  if(curSize < size){
    #ifdef BUDDY_SYS_DEBUG
    printf("no more space error!!!");
    #endif
    return -1;
  }
  buddy_head* node = getBuddyHead(buddy_head_base, idx);
  //内存块完全被占用
  if(node->status == 2) return -1;
  //找到了合适大小的内存块
  if(size == curSize){
    if(node->status == 0){
      node->status = 2;
      printf("head in: %p\n", node);
      return idx;
    }
    return -1;
  }
  uintptr_t lIdx = idx << 1, rIdx = (idx << 1) + 1;
  if((lIdx = dfs(size, curSize >> 1, baseAddr, buddy_head_base, lIdx)) != -1){
    node->status = 1;
    // setBuddyHead(idx, buddy_head_base);
    return lIdx;
  }else if((rIdx = dfs(size, curSize >> 1, baseAddr, buddy_head_base, rIdx)) != -1){
    node->status = 1;
    // setBuddyHead(idx, buddy_head_base);
    return rIdx;
  }
  return -1;
}
//伙伴系统分配
static void* balloc(size_t size, size_t idx){
  //递归查找
  uintptr_t res = dfs(size, MAX_BUDDY_BLOCK_SIZE, (void*)BUDDY_START + idx * MAX_BUDDY_BLOCK_SIZE, (buddy_head*)(BUDDY_HEAD_START + idx * BUDDY_HEAD_SIZE * sizeof(buddy_head)), 1);
  //BUDDY_START + size * res - (BUDDY_END - BUDDY_START)位置是算出来
  if(res != -1) printf("init in block: %d , section: %d \n", idx, res);
  return res == -1 ? (void*)-1 : (void*)BUDDY_START + idx * MAX_BUDDY_BLOCK_SIZE + res * size - MAX_BUDDY_BLOCK_SIZE;
}
//对应实验要求中的 kalloc；
static void *kalloc(size_t size) {
  //超过MAXSIZE的是不合法的申请
  if(size > MAXSIZE || size < 0) return NULL;
  //TODO
  //尚未完成slab时的代替方案
  size = size > MIN_BUDDY_BLOCK_SIZE ? size : MIN_BUDDY_BLOCK_SIZE;
  //向上对齐
  size = get2PowSize(size);
  printf("need space: %d\n", size);
  //通过伙伴系统分配
  for(int i = 0; i < BUDDY_SIZE; i++){
    void* res = balloc(size, i);
    if(res != (void*)-1){
      printf("alloced in address: %p\n", res);
      return (void*)res;
    }
  }
  return (void*)-1;
}
static void buddy_Block_Init(void* block, int size){
  memset(block, 0, size);
}
//递归调用， 释放空间, 成功则返回1
int searchFull(void* ptr, void* baseBuddy, void *baseBuddyHead, int idx, uintptr_t curSize){
  buddy_head* headAddress = getBuddyHead(baseBuddyHead, idx);
  // printf("baseBuddyHead:  %p\n", baseBuddyHead);
  // printf("%b", )
  // printf("%ld = = = = = =\n", headAddress->status == 2);
  // if(headAddress->status == 2){
  //   printf("2\n======\n");
  // }else{
  //   printf("asdfasd\n======\n");
  // }
  // panic("as\n");
  // printf("ptr:%p, baseBuddy:%p   status:%p\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n ", ptr, baseBuddy, (void*)headAddress->status);
  printf("curSize:%d  idx:%d\n", curSize, idx);
  if(ptr == baseBuddy && headAddress->status == 2){
    // printf("free :%p\n", headAddress);
    //释放空间
    headAddress->status = 0;
    return 1;
  }
  // printf("askjdhasd==============================\n");
  int res = 0;
  //search
  if(ptr < baseBuddy + (curSize << 1)){
    res = searchFull(ptr, baseBuddy, baseBuddyHead, idx >> 1, curSize >> 1);
  }else{
    res = searchFull(ptr, baseBuddy + (curSize >> 1), baseBuddyHead, (idx >> 1) + 1, curSize >> 1);
  }
  if(res){
    uintptr_t lStatus = getBuddyHead(baseBuddyHead, idx >> 1)->status;
    uintptr_t rStatus = getBuddyHead(baseBuddyHead, (idx >> 1) + 1)->status;
    headAddress->status = (lStatus == rStatus && lStatus == 0) ? 0 : 1;
  }
  return res;
}
//对应实验要求中的 kfree。
static void kfree(void *ptr) {
  printf("==try free:  %p\n", ptr);
  searchFull(ptr, ptr - (((uintptr_t)ptr - BUDDY_START) % MAX_BUDDY_BLOCK_SIZE), (void*)(BUDDY_HEAD_START + ((uintptr_t)ptr - BUDDY_START) / MAX_BUDDY_BLOCK_SIZE * BUDDY_HEAD_SIZE * sizeof(buddy_head)),1, MAX_BUDDY_BLOCK_SIZE);
  printf("==allready free:  %p\n", ptr);
}
static void buddy_sys_init(uintptr_t start, uintptr_t end){
  //伙伴数组的个数
  BUDDY_SIZE = (end - start) / (MAX_BUDDY_BLOCK_SIZE + BUDDY_HEAD_SIZE * sizeof(buddy_head));
  //计算出伙伴数组开头和结尾的地址
  BUDDY_START = (uintptr_t)start;
  BUDDY_END = (uintptr_t)start + BUDDY_SIZE * MAX_BUDDY_BLOCK_SIZE;
  //计算出伙伴头数组
  BUDDY_HEAD_START = (uintptr_t)BUDDY_END;
  BUDDY_HEAD_END = (uintptr_t)BUDDY_HEAD_START + BUDDY_SIZE * BUDDY_HEAD_SIZE * sizeof(buddy_head);
  #ifdef TESTHEAP
  int idx = 0;
  #endif
  for(uintptr_t i = BUDDY_START; i < BUDDY_END; i += MAX_BUDDY_BLOCK_SIZE){
    buddy_Block_Init((void*)i, MAX_BUDDY_BLOCK_SIZE);
  #ifdef TESTHEAP
    printf("%d       %p\n", idx, (uintptr_t)i);
    ++idx;
  #endif
  }
}

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
  printf("BUDDY_HEAD_START: %p\n", BUDDY_HEAD_START);
  printf("BUDDY_HEAD_END: %p\n", BUDDY_HEAD_END);
  printf("===================================================\n");
}
MODULE_DEF(pmm) = {
  .init  = pmm_init,
  .alloc = kalloc,
  .free  = kfree,
};
void pWord(){
  printf("pWold!\n");
}
