#include "co.h"
#include "linkedList.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>

threadNode *head, *tail, *currentThread;
int idCount = 0;	// initial coroutinu number

void initCoroutine()
{
	currentThread = tail = head = init();
}

// sp->$rsp; arg->%rdi; jmp *entry;
static inline void stack_switch_call(void *sp, void *entry, void *arg) {
  asm volatile (
#if __x86_64__
    "movq %0, %%rsp; movq %2, %%rdi; jmp *%1"
      : : "b"((unsigned char*)sp), "d"(entry), "a"(arg) : "memory"
#else
    "movl %0, %%esp; movl %2, 4(%0); jmp *%1"
      : : "b"((unsigned char*)sp - 8), "d"(entry), "a"(arg) : "memory"
#endif
  );
}

co *co_start(const char *name, void (*func)(void *), void *arg) {
	// initialize a co struct of coroutine
	co *thread = malloc(sizeof(co));
	strcpy(thread->name, name);
	thread->func = func;
	thread->arg = arg;
	thread->cid = ++idCount;	
	co_status threadStatus = CO_NEW; 
	thread->status = threadStatus;
	thread->waiter = NULL;
	thread->env = (jmp_buf*)malloc(sizeof(jmp_buf));
	threadNode * localThread = (threadNode*)malloc(sizeof(threadNode));
	localThread->thread = thread;
	tail = insertTail(tail, localThread);

	return thread; 
}

void co_wait(co *co) {

	currentThread->status == OC_WAITTING;	// 当前线程在等待co
	while(co->status != CO_DEAD) {
		co_yield();
	}
	assert(co->status == CO_DEAD);
	// release resource
	free(co);
}

// Search the next thread to run if the thread's status is CO_NEW
co *searchNextNewThread()
{
	threadNode *nextThread = head;
	srand(time(NULL));
	int randomThreadIndex = rand() % idCount + 1;
	while(randomThreadIndex--) 
		nextThread = nextThread->next;
	
	return nextThread->thread; 
}

void co_yield() {
	int val = setjmp(currentThread->thread->env);
	if(val == 0) {
		// which indicats it was called by setjmp directly
		co *currentThread = searchNextNewThread();		 
		if(currentThread->status == CO_NEW) {
			currentThread->status = CO_RUNNING;
			stack_switch_call(currentThread->stack, currentThread->func, currentThread->arg);			
		} else if(currentThread->status == CO_RUNNING) {
			longjmp(currentThread->env, currentThread->cid);
		}

		currentThread->status == CO_DEAD;	// 到这里应该证明当前协程已经执行完毕，返回了	
	}
}
