#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *pthread_action(void *args)
{
    printf("Hello from pthread!!\n");
    sleep(1);
}

int main()
{
    pthread_t p;
    pthread_create(&p, NULL, pthread_action, NULL);
    printf("Hello from main thread\n");
    pthread_join(p, NULL);
    printf("Hello after pthread join\n");
}
