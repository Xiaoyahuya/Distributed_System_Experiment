#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NUMTHREADS 2
#define INCREMENTS 1000000

volatile static int counter = 0;

// 定义一个锁变量
volatile int my_spinlock = 0;

void lock(volatile int *l) {
    // TODO: 使用 while 循环和 __atomic_test_and_set (或 CAS)
    // 直到成功获取锁为止
}

void unlock(volatile int *l) {
    // TODO: 释放锁，将变量置为0
    // 思考：这里需要原子操作吗？还是普通的赋值就够了？
}

void *threadfn(void *ptr) {
    for (int i = 0; i < INCREMENTS; i++) {
        lock(&my_spinlock);
        counter++; // 现在这里是安全的临界区
        unlock(&my_spinlock);
    }
    return NULL;
}

int main(void) {
  pthread_t threads[NUMTHREADS];  // Thread IDs
  for (int i = 0; i < NUMTHREADS; i++) pthread_create(&threads[i], 0, threadfn, NULL);
  for (int i = 0; i < NUMTHREADS; i++) pthread_join(threads[i], 0);

  printf("Final counter value: %d\n", counter);
  return 0;
}
