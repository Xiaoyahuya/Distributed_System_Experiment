#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>

// 宏定义
#define BUFFER_SIZE 5
#define TOTAL_ITEMS 100 // 总共生产/消费多少个数据
#define EMPTY_SLOT -1   // 标记缓冲区位置为空

// 全局共享资源
int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

// =============================================================
// 实验任务：定义同步原语
// 提示：你需要 1 个互斥锁 (mutex) 和 2 个信号量 (semaphores)
// =============================================================
pthread_mutex_t mutex;
sem_t sem_empty; // 追踪空闲槽位数量
sem_t sem_full;  // 追踪已有数据数量

// 生产者线程
void *producer(void *arg) {
    for (int i = 0; i < TOTAL_ITEMS; i++) {
        int item = i; // 生产有序数据，方便校验

        // -------------------------------------------------------
        // TODO 1: 进入区 (Entry Section)
        // 1. 等待有空位 (P操作 / wait)
        // 2. 获取互斥锁以保护共享变量
        // -------------------------------------------------------
        
        
        
        // -------------------------------------------------------
        // 临界区 (Critical Section) - 请勿修改校验逻辑
        // -------------------------------------------------------
        // [自检逻辑] 检查是否覆盖了未读取的数据
        if (buffer[in] != EMPTY_SLOT) {
            printf("\033[31m[ERROR] OVERFLOW! Producer overwrote unread data at index %d!\033[0m\n", in);
        }

        buffer[in] = item; // 写入数据
        printf("Produced: %d at [%d]\n", item, in);
        
        in = (in + 1) % BUFFER_SIZE;
        
        // 模拟不确定的处理时间，增加竞争概率
        if (rand() % 2 == 0) usleep(100); 

        // -------------------------------------------------------
        // TODO 2: 退出区 (Exit Section)
        // 1. 释放互斥锁
        // 2. 通知消费者有新数据 (V操作 / post)
        // -------------------------------------------------------
        
        
        
    }
    return NULL;
}

// 消费者线程
void *consumer(void *arg) {
    int last_item = -1; // 用于检查数据连续性

    for (int i = 0; i < TOTAL_ITEMS; i++) {
        
        // -------------------------------------------------------
        // TODO 3: 进入区 (Entry Section)
        // 1. 等待有数据 (P操作 / wait)
        // 2. 获取互斥锁
        // -------------------------------------------------------
        
        
        
        // -------------------------------------------------------
        // 临界区 (Critical Section) - 请勿修改校验逻辑
        // -------------------------------------------------------
        // [自检逻辑] 检查是否读取了空槽位
        if (buffer[out] == EMPTY_SLOT) {
            printf("\033[31m[ERROR] UNDERFLOW! Consumer read garbage (empty slot) at index %d!\033[0m\n", out);
        }
        
        int item = buffer[out];
        buffer[out] = EMPTY_SLOT; // 标记为已读/空
        out = (out + 1) % BUFFER_SIZE;

        // [自检逻辑] 检查数据顺序是否正确
        if (item != last_item + 1) {
             printf("\033[33m[WARNING] RACE CONDITION! Expected %d but got %d (Data lost or duplicated)\033[0m\n", last_item + 1, item);
        }
        last_item = item; // 尝试修正，继续检查

        printf("   Consumed: %d from [%d]\n", item, (out - 1 + BUFFER_SIZE) % BUFFER_SIZE);

        // -------------------------------------------------------
        // TODO 4: 退出区 (Exit Section)
        // 1. 释放互斥锁
        // 2. 通知生产者有空位 (V操作 / post)
        // -------------------------------------------------------
        
        
        
    }
    return NULL;
}

int main(void) {
    srand(time(NULL));
    pthread_t prod_tid, cons_tid;

    // 初始化缓冲区为 "EMPTY_SLOT"
    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer[i] = EMPTY_SLOT;
    }

    // =========================================================
    // TODO 5: 初始化同步原语
    // 1. 初始化互斥锁
    // 2. 初始化 sem_empty (初始值应为多少？)
    // 3. 初始化 sem_full (初始值应为多少？)
    // =========================================================
    
    
    

    printf("--- Experiment Start ---\n");

    // 创建线程
    pthread_create(&prod_tid, NULL, producer, NULL);
    pthread_create(&cons_tid, NULL, consumer, NULL);

    // 等待线程结束
    pthread_join(prod_tid, NULL);
    pthread_join(cons_tid, NULL);

    printf("--- Experiment End ---\n");

    // =========================================================
    // TODO 6: 销毁同步原语
    // =========================================================
    
    
    

    return 0;
}