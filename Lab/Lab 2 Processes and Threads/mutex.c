///
// mutex.c - sum a vector of values using multi-threading and a mutex
// compile with: gcc mutex.c -o mutex --std=c99 -lpthread
//
//

/* 注释
互斥锁要在线程中调用的函数里上锁与解锁，而不是在线程之外。
 pthread_create 函数我们调用了 run_summation 这个指针函数，
所以我们的上锁与解锁其实是写在 run_summation 这个函数里的 for。
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


#define NUM_THREADS 16
#define N 10000


typedef struct {
    float *total;
    int n;

    pthread_mutex_t *lock;
    pthread_t thread;

} Worker;


void *run_summation(void *ptr)
{
    Worker *worker = (Worker*)ptr;
    
    pthread_mutex_lock(worker->lock);   // 给老子上锁
    for (int i = 0; i < worker->n; ++i) {
        (*worker->total)++;
    }
    pthread_mutex_unlock(worker->lock);   // 任务完成，给老子解锁
    
    return NULL;
}



int main()
{

    // Global variable for total summation so far
    float total = 0;

    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    Worker workers[NUM_THREADS];

    
    for (int i = 0; i < NUM_THREADS; ++i) {
        // What would be the problem declaring Worker w here?

        Worker *worker = &workers[i];
        worker->total = &total; // Pass the global total into each thread

        worker->lock = &lock;
        worker->n = N;

        pthread_mutex_init(worker->lock, NULL);   // 动态初始化互斥锁

        // 在这个循环体内，每次创建一条线程
        pthread_create(&(worker->thread), NULL, run_summation, worker);
        printf("Total: %f \n", *worker->total);
        //run_summation((void*)worker);

        pthread_mutex_destroy(worker->lock);  // 销毁互斥锁
    }
    


    ////////////////////////////////
    // Wait for all the threads we created
    // for(i = ...)
    ////////////////////////////////
    for (int i=0; i<NUM_THREADS; i++) {
	    pthread_join(workers[i].thread, NULL);
    }

    printf("Final total: %f \n", total);

    return 0;
}

