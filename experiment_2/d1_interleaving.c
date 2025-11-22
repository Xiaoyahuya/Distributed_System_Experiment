#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define NUMTHREADS 4

// 全局状态
int next_turn = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *threadfn(void *ptr) {
    int my_id = (int)ptr;
    
    pthread_mutex_lock(&lock);
    while (next_turn != my_id) {
        // TODO: 等待信号
        pthread_cond_wait(&cond, &lock);
    }
    
    // 临界区操作
    printf("Thread %d is executing strictly in order.\n", my_id);
    next_turn++; // 移交控制权给下一个
    
    // TODO: 通知其他线程醒来检查
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main(void) {
  //sranddev(); 
  srand(time(NULL));
  pthread_t threads[NUMTHREADS];  // Thread IDs

  for (int i = 0; i < NUMTHREADS; i++) pthread_create(&threads[i], 0, threadfn, (void *)i);
  for (int i = 0; i < NUMTHREADS; i++) pthread_join(threads[i], 0);
  return 0;
}
