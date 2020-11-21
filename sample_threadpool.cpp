#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <err.h>
#include <semaphore.h>
#include <unistd.h>

struct Thread {
    sem_t mutex;
    int cl;
};
typedef struct Thread Thread;

sem_t mainMutex;

// Simulate the processing of a file descriptor. 
void process(int cl) {
    printf("Processing %d\n",cl);
    sleep(1);
    printf("Processing %d complete\n",cl);
}

// Thread 
void* start(void* arg) {
    Thread* thread = (Thread*)arg;
    while (true) {
        printf("Thread waiting\n");
        if (0 != sem_wait(&thread->mutex)) err(2,"sem_wait in thread");
        process(thread->cl);
        thread->cl = 0;
        if (0 != sem_post(&mainMutex)) err(2,"sem_post of main by thread");
    }
    return 0;
}

int main() {
    printf("Started\n");

    int numThread = 16;
    Thread threads[numThread];

    if (0 != sem_init(&mainMutex, 0, 0)) err(2,"sem_init mainMutex");
    pthread_t threadPointer; // value not used in program.

    // initialize and start the threads
    for (int i = 0; i < numThread; i++) {
        Thread* thread = threads+i;
        if (0 != sem_init(&(thread->mutex), 0, 0)) err(2,"sem_init for thread");
        thread->cl = 0;
        if (0 != pthread_create(&threadPointer,0,start,thread)) err(2,"pthread_create");
    }

    // simulate an accept completing with cl == 42 to thread 3. You must create your accept loop
    Thread* thread = threads+3;
    thread->cl = 42;
    if (0 != sem_post(&(thread->mutex))) err(2,"sem_post for thread");

    // simulate having to wait for any thread to complete
    if (0 != sem_wait(&mainMutex)) err(2,"sem_wait in main");
    printf("ended\n");
}