#include "co.h"
#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>
#define NTHREAD 64

typedef unsigned long int	uintptr_t;

enum co_status {
  CO_NEW = 1, // 新创建，还未执行过
  CO_RUNNING, // 已经执行过
  CO_WAITING, // 在 co_wait 上等待
  CO_DEAD,    // 已经结束，但还未释放资源
};
struct co {
  const char *name;
  void (*func)(void *); // co_start 指定的入口地址和参数
  void *arg;

  enum co_status status;  // 协程的状态
  struct co *    waiter;  // 是否有其他协程在等待当前协程
  jmp_buf        context; // 寄存器现场 (setjmp.h)
  // uint8_t        stack[STACK_SIZE]; // 协程的堆栈
};
//协程池
struct co tpool[NTHREAD], *my_co = tpool;

void *wrapper(void *arg) {
  return NULL;
}
struct co *co_start(const char *name, void (*func)(void *), void *arg) {
  *my_co = (struct co) {
    .name = name,
    .func = func,
  };
  printf("=========1");
  void *sp;
  asm volatile ("mov %%rsp, %0" : "=r" (sp));
  asm volatile (
#if __x86_64__
    "movq %0, %%rsp; movq %2, %%rdi; jmp *%1" 
    : 
    : "b"((uintptr_t)sp),     "d"(func), "a"(arg)
#else
    "movl %0, %%esp; movl %2, 4(%0); jmp *%1" : : "b"((uintptr_t)sp - 8), "d"(entry), "a"(arg)
#endif
  );
  printf("=========2");
  return NULL;
}

void co_wait(struct co *co) {
  return;
}

void co_yield() {
  return;
}
