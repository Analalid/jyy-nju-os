#include "co.h"
#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>
#include <string.h>

//支持unit_8
// typedef unsigned char uint8_t;
//支持STACK_SIZE
// #define STACK_SIZE 1024

#define NTHREAD 64

typedef unsigned long int	uintptr_t;

static inline void stack_switch_call(void *sp, void *entry, uintptr_t arg) {
  asm volatile (
#if __x86_64__
    "movq %0, %%rsp; movq %2, %%rdi; jmp *%1" : : "b"((uintptr_t)sp),     "d"(entry), "a"(arg)
#else
    "movl %0, %%esp; movl %2, 4(%0); jmp *%1" : : "b"((uintptr_t)sp - 8), "d"(entry), "a"(arg)
#endif
  );
}
enum co_status {
  CO_NEW = 1, // 新创建，还未执行过
  CO_RUNNING, // 已经执行过
  CO_WAITING, // 在 co_wait 上等待
  CO_DEAD,    // 已经结束，但还未释放资源
};
struct co {
  char *name;
  void (*func)(void *); // co_start 指定的入口地址和参数
  void *arg;
  enum co_status status;  // 协程的状态
  struct co *    pre;
  struct co *    next;
  jmp_buf        context; // 寄存器现场 (setjmp.h)
  // uint8_t        stack[STACK_SIZE]; // 协程的堆栈

};
//co_main作为新插入协程的哨兵节点， current协程指向正在运行的协程
struct co* co_main, *co_current;

__attribute__((constructor)) void co_init() {
  co_main = malloc(sizeof(struct co));
  co_main->name = "main";
  co_main->status = CO_RUNNING;
  co_main->next = co_main;
  co_main->pre = co_main;
  co_current = co_main;
  printf("main_init\n");
}



void *wrapper(void *arg) {
  co_current -> status = CO_RUNNING;
  co_current -> func(co_current->arg);
  co_current -> status = CO_DEAD;
  // if(co_current->waiter != NULL){
  //   co_current->waiter->status = CO_RUNNING;
  //   co_current->waiter = NULL;
  // }
  co_yield();
  return NULL;
}

struct co *co_start(const char *name, void (*func)(void *), void *arg) {
  struct co *my_co = malloc(sizeof(struct co));
  my_co->name = malloc(sizeof(name));
  strcpy(my_co->name, name);
  my_co->func = func;
  my_co->arg = arg;
  my_co->status = CO_NEW;
  //插入新节点
  my_co->next = co_main;
  my_co->pre = co_main->pre;
  co_main->pre->next = my_co;
  co_main->pre = my_co;
  printf("=========in\n");
  if(setjmp(my_co->context) == 0){
    //初次初始化
    printf("init->%s\n", my_co->name);
  }else{
    //error!
    printf("cann't init->%s twice !!!\n", my_co->name);
  }
  printf("=========out\n");
  return my_co;
}

void co_wait(struct co *co) {
  co_current -> status = CO_WAITING;
  //直到这个协程还没死，就一直循环
  while(co->status != CO_DEAD){
    co_yield();
  }
  //死了再释放
  free(co);
  return;
}

void co_yield() {
  int val = setjmp(co_current->context);
  if(val == 0){
    struct co *nextNode = co_current->next;
    printf("hear!!!!!!!!!!!");
    while(nextNode -> status == CO_WAITING || nextNode -> status == CO_DEAD) nextNode = nextNode->next;
    co_current = nextNode;
    //如果尚未执行过则先初始化
    if(nextNode -> status == CO_NEW){
      //栈顶指针的位置由计算得出
      void *stackTop = (void*)((char*)nextNode + sizeof(struct co));
      if(sizeof(void*) == 4) stack_switch_call(stackTop, wrapper, (uintptr_t)NULL);
      else{
        asm volatile("mov %0,%%rsp"::"b"((uintptr_t)stackTop));
        wrapper(NULL);
      }
    }
    else{
      longjmp(nextNode -> context,0);
    } 
  }
  return;
}