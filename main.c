#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

int hackers = 0;
int serfs = 0;

//int semvalue;
//sem_getvalue(mutex, &semvalue);
//printf("Sem value");
//printf(semvalue, "%d");

void board()
{
    printf("Board \n");
}
void rowBoat(){
    printf("Row Boat \n");
}

void *hacker_func(void *x_void_ptr)
{
    printf((char *) (x_void_ptr), "%s");
    printf(" OK\n");

    sem_t *mutex = sem_open("/mutex", O_CREAT);
    if(mutex == SEM_FAILED){
        printf("Error opening semaphore mutex hacker_func \n");
    }
    sem_t *hackerQueue = sem_open("/hackerQueue", O_CREAT);
    if(hackerQueue == SEM_FAILED){
        printf("Error opening semaphore hackerQueue hacker_func \n");
    }
    sem_t *serfQueue = sem_open("/serfQueue", O_CREAT);
    if(serfQueue == SEM_FAILED){
        printf("Error opening semaphore serfQueue hacker_func \n");
    }
    sem_t *barrier = sem_open("/barrier", O_CREAT);
    if(barrier == SEM_FAILED){
        printf("Error opening semaphore barrier hacker_func \n");
    }

    int isCaptain = 0;

    sem_wait(mutex);

    hackers += 1;
    if (hackers == 4){
        for(int i = 0; i < 4; i++){
            sem_post(hackerQueue);
        }
        hackers = 0;
        isCaptain = 1;
    }
    else if (hackers == 2 && serfs >= 2){
        for (int i = 0; i < 2; ++i) {
            sem_post(hackerQueue);
            sem_post(serfQueue);
        }
        serfs -= 2;
        hackers = 0;
        isCaptain = 1;
    }
    else{
        sem_post(mutex);
    }

    sem_wait(hackerQueue);

    board();
    sem_wait(barrier);

    if (isCaptain){
        rowBoat();
        sem_post(mutex);
    }

    sem_post(barrier);
    return NULL;
}

void *serf_func(void *x_void_ptr)
{
    printf((char *) (x_void_ptr), "%s");
    printf(" OK\n");

    sem_t *mutex = sem_open("/mutex", O_CREAT);
    if(mutex == SEM_FAILED){
        printf("Error opening semaphore mutex serf_func \n");
    }
    sem_t *hackerQueue = sem_open("/hackerQueue", O_CREAT);
    if(hackerQueue == SEM_FAILED){
        printf("Error opening semaphore hackerQueue serf_func \n");
    }
    sem_t *serfQueue = sem_open("/serfQueue", O_CREAT);
    if(serfQueue == SEM_FAILED){
        printf("Error opening semaphore serfQueue serf_func \n");
    }
    sem_t *barrier = sem_open("/barrier", O_CREAT);
    if(barrier == SEM_FAILED){
        printf("Error opening semaphore barrier serf_func \n");
    }

    int isCaptain = 0;

    sem_wait(mutex);

    serfs += 1;
    if (serfs == 4){
        for(int i = 0; i < 4; i++){
            sem_post(serfQueue);
        }
        serfs = 0;
        isCaptain = 1;
    }
    else if (hackers == 2 && serfs >= 2){
        for (int i = 0; i < 2; ++i) {
            sem_post(hackerQueue);
            sem_post(serfQueue);
        }
        hackers -= 2;
        serfs = 0;
        isCaptain = 1;
    }
    else{
        sem_post(mutex);
    }

    sem_wait(serfQueue);

    board();
    sem_wait(barrier);

    if (isCaptain){
        rowBoat();
        sem_post(mutex);
    }

    sem_post(barrier);
    return NULL;
}

int main()
{

    sem_t *mutex = sem_open("/mutex", O_CREAT, S_IRUSR | S_IWUSR, 1);
    if(mutex == SEM_FAILED){
        printf("Error creating semaphore mutex \n");
    }
    sem_post(mutex);

    sem_t *hackerQueue = sem_open("/hackerQueue", O_CREAT, S_IRUSR | S_IWUSR, 0);
    if(hackerQueue == SEM_FAILED){
        printf("Error creating semaphore hackerQueue\n");
    }

    sem_t *serfQueue = sem_open("/serfQueue", O_CREAT, S_IRUSR | S_IWUSR, 0);
    if(serfQueue == SEM_FAILED){
        printf("Error creating semaphore serfQueue\n");
    }

    sem_t *barrier = sem_open("/barrier", O_CREAT, S_IRUSR | S_IWUSR, 0);
    if(barrier == SEM_FAILED){
        printf("Error creating semaphore barrier\n");
    }
    for(int i = 0; i < 4; i++){
        sem_post(barrier);
    }

    printf("Start\n");

    char* thread0param = "Thread 0";
    char* thread1param = "Thread 1";
    char* thread2param = "Thread 2";
    char* thread3param = "Thread 3";
    pthread_t thread0;
    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;

    pthread_create(&thread0, NULL, serf_func, thread0param);
    pthread_create(&thread1, NULL, serf_func, thread1param);
    pthread_create(&thread2, NULL, hacker_func, thread2param);
    pthread_create(&thread3, NULL, hacker_func, thread3param);

    pthread_join(thread0, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

    return 0;
}
