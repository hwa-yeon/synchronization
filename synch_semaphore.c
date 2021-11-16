#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#define ITER 1000

void *thread_increment(void *arg);
void *thread_decrement(void *arg);

sem_t empty;
sem_t full;
int x;

int main() {
    pthread_t tid1, tid2;
    sem_init(&empty, 0, 30);
    sem_init(&full, 0, 0);

    pthread_create(&tid1, NULL, thread_increment, NULL);
    pthread_create(&tid2, NULL, thread_decrement, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    if(x != 0) printf("BOOM! counter=%d\n", x);
    else       printf("OK counter=%d\n", x);

    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}

void *thread_increment(void *arg) {
    int i, val;
    for(i=0; i<ITER; i++) {
        sem_wait(&empty);
        val = x;
        printf("%u: %d\n", (unsigned int)pthread_self(), val);
        x = val + 1;
        sem_post(&full);
    }
    return NULL;
}

void *thread_decrement(void *arg) {
    int i, val;
    for(i=0; i<ITER; i++) {
        sem_wait(&full);
        val = x;
        printf("%u: %d\n", (unsigned int)pthread_self(), val);
        x = val - 1;
        sem_post(&empty);
    }
    return NULL;
}