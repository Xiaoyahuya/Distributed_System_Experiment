
# 分布式系统第一次实验

## 第一阶段(experiment_1目录)：
### 实验 1.0：环境配置

### 实验 1.1：竞争条件的“漏洞窗口”探测
*   **对象**：d2_increment.c
*   **任务**：
    编写一个Shell脚本，循环运行d2_increment程序100 次。统计最终结果分布。
    尝试修改 `INCREMENTS` 的值（比如从 100万 降到 1000，或升到 1000万）。
    *   *记录*：绘制一张图表，X轴是`INCREMENTS`，Y轴是实际结果与预期结果的偏差率(平均值)。

### 实验 1.2：原子操作的性能代价
*   **对象**：d2_increment.c vs d3_atomic.c
*   **任务**：
    使用 `time ./program` 测量两个版本的运行时间。
*   **回答以下问题**：
    *   两个版本的程序运行时间有什么差距？
    *   如果有较大差距，请解释原因？

---

## 第二阶段：功能实现


### 实验 2.1：实现“自旋锁” (Spinlock) —— 硬件基础篇
**背景**：d3_atomic.c使用了 GCC 的原子加法。但在操作系统内核中，我们常使用原子指令来构建锁。
**目标**：利用原子指令 `__atomic_test_and_set` 或 `__atomic_compare_exchange` (CAS) 实现一个简单的自旋锁，替换d3_atomic.c中的直接原子加法，来保护临界区。

**代码补全**：
```c
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
```

### 实验 2.2：从“混乱”到“有序” (信号量/条件变量)
**背景**：d1_interleaving.c的输出是乱序的。但在很多场景下，我们需要线程按特定顺序执行。
**目标**：d1_interleaving.c，强制要求线程必须按照 `Thread 0 -> Thread 1 -> Thread 2 -> Thread 3` 的顺序打印。

**实现要求**：
1.  不能使用 `sleep()` 。
2.  **方法 A (Semaphores)**：使用 `sem_t` 数组。Thread `i` 等待 `sem[i]`，打印后 signal `sem[i+1]`。
3.  **方法 B (Condition Variables)**：使用 `pthread_cond_t` 和 `pthread_mutex_t`。所有线程检查一个全局变量 `next_thread_id`，如果不轮到自己就 `cond_wait`。

**补全代码**：
```c
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
```


### 实验 2.3：经典设计模式 —— 生产者-消费者 (Producer-Consumer)
**背景**：这是课程大纲中明确提到的 "Common design patterns"。
**目标**：结合多线程和共享数据，实现生产者消费者实验(p_c.c)。

**功能需求**：
1.  创建一个固定大小的数组 `int buffer[5]`。
2.  **生产者线程**：不断生成随机数写入数组。如果数组满了，必须阻塞等待。
3.  **消费者线程**：不断从数组读取数据并求和。如果数组空了，必须阻塞等待。
4.  必须保证数据**不覆盖、不重复读取**，且没有死锁。

**实现核心**：
你需要同时使用：
*   1 个 Mutex：保护缓冲区的索引（index）。
*   2 个 Semaphores（或 CondVars）：一个表示 `items_available`（初值为0），一个表示 `space_available`（初值为5）。

---

## 实验总结与报告

### 实验环境

操作系统，编译器版本，CPU核数

### 第一部分：竞争条件与原子性

#### 1.1 竞争条件的观测

- **图表绘制**：请在此处给出绘制的图表（X轴=INCREMENTS，Y轴=偏差率）。

- **现象分析**：分析图表信息，解释为什么最终结果总是小于或等于预期值

#### 1.2 原子操作的性能代价

- **实验数据**
- **原因分析**

### 第二部分：同步原语的实现与应用 (Exp 2)

#### 2.1 自旋锁 (Spinlock) 的实现

- **核心代码**
- **思考题回答**

#### 2.2 有序控制

- 在代码中，你使用了 while (next_turn != my_id) 还是 if (next_turn != my_id) 进行等待？请解释为什么要使用 while 循环？如果使用 if 可能会发生什么？

#### 2.3 生产者-消费者模型

**死锁预防**：

- 在你的代码中，消费者线程是“先加锁 (Mutex) 后等信号量 (Sem)”，还是“先等信号量后加锁”？
- 如果反过来写，会发生什么后果？请简述死锁发生的场景。

### 第三部分：总结

请根据你的实验体验和理论知识，完成下表的对比和分析：

| 机制                    | CPU开销 (忙等 vs 挂起) | 响应速度 (延迟) | 适用场景举例 | 缺点/风险 |
| :---------------------- | :--------------------- | :-------------- | :----------- | :-------- |
| **原子变量** (Atomic)   |                        |                 |              |           |
| **自旋锁** (Spinlock)   |                        |                 |              |           |
| **互斥锁** (Mutex)      |                        |                 |              |           |
| **信号量/CV** (CondVar) |                        |                 |              |           |

### 第四部分：从单机到分布式 (思考题)

- 本实验中，所有线程都能访问同一个全局变量 my_spinlock 或 mutex。如果这 4 个线程运行在 4 台不同的机器上（通过网络连接），原本的 pthread_mutex_t 还能用吗？为什么？
- 实验 2.3 (生产者-消费者) 通过共享内存 (buffer 数组) 来传递数据。在分布式系统中，如果两个进程不共享内存，可以使用什么中间件来实现类似“生产者-消费者”的功能？