#include "co.h"
#include <stdlib.h>

#define NTHREAD 64
enum { T_FREE = 0, T_LIVE, T_DEAD, };

struct co {
  int status;
  pthread_t thread;
  void (*entry)(int);
};

struct co *co_start(const char *name, void (*func)(void *), void *arg) {
  struct co* res;
  *res = (struct co) {
    .status = T_LIVE,
    .entry = func,
  };

  pthread_create(&(res->thread), NULL, NULL, res);

  return res;
}

void co_wait(struct co *co) {
}

void co_yield() {
}
