#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct
{
    int *balance;
    pthread_mutex_t *mutex;
} DadosThread;

void *rotina_thread(void *arg)
{
    DadosThread *dados = (DadosThread *)arg;
    int i = 0;

    while (i < 1000000)
    {
        pthread_mutex_lock(dados->mutex);
        (*(dados->balance))++;
        pthread_mutex_unlock(dados->mutex);
        i++;
    }

    return NULL;
}

int main()
{
    pthread_t t1, t2;
    int balance = 0;
    pthread_mutex_t mutex;
    DadosThread dados;

    dados.balance = &balance;
    dados.mutex = &mutex;

    pthread_mutex_init(&mutex, NULL);

    pthread_create(&t1, NULL, rotina_thread, &dados);
    pthread_create(&t2, NULL, rotina_thread, &dados);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&mutex);

    printf("balance = %d\n", balance);
    printf("Threads finished\n");

    return 0;
}
