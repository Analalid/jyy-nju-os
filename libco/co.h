#ifndef __CO_C__
#define __CO_C__
#define STACK_SIZE	1024*256

#include <setjmp.h>

typedef enum {
	CO_NEW = 1,	// new coroutine,还未执行
	CO_RUNNING, // 已经执行过的coroutinue
	CO_WAITTING,	// 在co_wait上等待 
	CO_DEAD		// 已经结束，但还未释放资源 	
}co_status;

typedef struct {
	char *name;
	int cid;
	void (*func)(void *arg);
	void *arg;

	co_status status;	struct co *waiter;	// 是否有其他coroutine在等待当前coroutine
	jmp_buf *env;	// context
	unsigned char stack[STACK_SIZE];	// stack of coroutine
}co;

// function prototype
void initCoroutinr();
co* co_start(const char *name, void (*func)(void *), void *arg);
void co_yield();
void co_wait(co *co);

#endif
