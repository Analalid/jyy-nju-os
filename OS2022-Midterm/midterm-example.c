#include "thread.h"
#include "thread-sync.h"
#include <pthread.h>
#include <stdio.h>
// #include <cstdio>

#define MAX (100000)
int buffer[MAX];
int fill_ptr = 0;
int use_ptr = 0;
int count = 0;


cond_t empty, fill;
mutex_t mutex, result_mutex;
int isOver = 0;

void put(int value){
  buffer[fill_ptr] = value;
  fill_ptr = (fill_ptr + 1) % MAX;
  count++;
}

int get(){
  int tmp = buffer[use_ptr];
  use_ptr = (use_ptr + 1) % MAX;
  count--;
  return tmp;
}

long result = 0;
void producer(void *arg){
  int x;
  while (!feof(stdin) && scanf("%d", &x) == 1) {
    mutex_lock(&mutex);
    while(count == MAX) pthread_cond_wait(&empty, &mutex);
    put(x);
    pthread_cond_signal(&fill);
    mutex_unlock(&mutex);
  }
  isOver = 1;
}
void consumer(void *arg){
  long res;
  while(!isOver){
    pthread_mutex_lock(&mutex);
    while(count == 0) pthread_cond_wait(&fill, &mutex);
    res += get();
    pthread_cond_signal(&empty);
    pthread_mutex_unlock(&mutex);
  }
  mutex_lock(&result_mutex);
  result += res;
  mutex_unlock(&result_mutex);
}
int main() {
  // printf("ss");
  create(producer);
  create(consumer);
  create(consumer);
  create(consumer);
  create(consumer);
  join();
  printf("%ld\n", result);
}

/* 
答案是2
*/
