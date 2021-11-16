#include <pthread.h>
#include <stdio.h>
#define ITER 1000

void *thread_increment(void *arg);
void *thread_decrement(void *arg);

pthread_mutex_t mutex;
pthread_cond_t cond_space;
pthread_cond_t cond_data;

int x;

int main() {
    pthread_t tid1, tid2;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_space, NULL);
    pthread_cond_init(&cond_data, NULL);

    pthread_create(&tid1, NULL, thread_increment, NULL);
    pthread_create(&tid2, NULL, thread_decrement, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    if(x != 0) printf("BOOM! counter=%d\n", x);
    else       printf("OK counter=%d\n", x);
    
    return 0;
}

void *thread_increment(void *arg) {
    int i, val;
    for(i=0; i<ITER; i++) {
        pthread_mutex_lock(&mutex);

        if(x == 30) pthread_cond_wait(&cond_space, &mutex);

        val = x;
        printf("%u: %d\n", (unsigned int)pthread_self(), val);
        x = val + 1;

        pthread_cond_signal(&cond_data);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *thread_decrement(void *arg) {
    int i, val;
    for(i=0; i<ITER; i++) {
        pthread_mutex_lock(&mutex);

        if(x == 0) pthread_cond_wait(&cond_data, &mutex);

        val = x;
        printf("%u: %d\n", (unsigned int)pthread_self(), val);
        x = val - 1;

        pthread_cond_signal(&cond_space);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}