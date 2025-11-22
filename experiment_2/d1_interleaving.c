#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define NUMTHREADS 4

void *threadfn(void *ptr) {
    int my_id = (int)ptr;
    printf("Thread %d is executing strictly in order.\n", my_id);
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
