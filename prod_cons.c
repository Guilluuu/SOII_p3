#include "buffer.h"
#include <assert.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define SLEEPTIME rand() % 4
#define ITERS     10

static int PROD_N;
static int CONS_N;

int insert = 0;
int delete = 0;

pthread_mutex_t stdin_mutex;

pthread_barrier_t barrier;

char produce_item() { return ((rand() % 26) + 65); }

void *main_productor(void *arg)
{
    char filename[32];

    pthread_mutex_lock(&stdin_mutex);
    printf("P\t Archivo: ");
    fgets(filename, sizeof(filename) - 1, stdin);
    pthread_mutex_unlock(&stdin_mutex);

    char *c;
    // Trim \n
    if ((c = strchr(filename, '\n')))
        *c = '\0';

    int fd = open(filename, O_CREAT | O_RDONLY, 0600);
    assert(fd >= 0);

    pthread_barrier_wait(&barrier);

    for (int i = 0; i < ITERS; i++)
    {
        char elemento = produce_item();
        buffer_add(elemento);
        printf("Produciendo [%c] en %d\n", elemento, buffer_get_in_index());
        buffer_print();


        sleep(SLEEPTIME);
    }

    close(fd);
    return NULL;
}

void *main_consumidor(void *arg)
{
    char filename[32];
    pthread_mutex_lock(&stdin_mutex);
    printf("C\t Archivo: ");
    fgets(filename, sizeof(filename) - 1, stdin);
    pthread_mutex_unlock(&stdin_mutex);

    char *c;
    // Trim \n
    if ((c = strchr(filename, '\n')))
        *c = '\0';

    int fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0600);
    assert(fd >= 0);

    pthread_barrier_wait(&barrier);

    for (int i = 0; i < ITERS; i++)
    {
        char e;
        do
        {
            e = buffer_pop();
        } while (e == 0);

        printf("Consumiendo [%c] en %d\n", e, buffer_get_out_index());
        buffer_print();

        sleep(SLEEPTIME);
    }

    close(fd);
    return NULL;
}


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        PROD_N = 1;
        CONS_N = 1;
    }

    else
    {
        PROD_N = atoi(argv[1]);
        CONS_N = atoi(argv[2]);
    }

    pthread_t prod_threads[PROD_N];
    pthread_t cons_threads[CONS_N];

    pthread_barrier_init(&barrier, NULL, PROD_N + CONS_N);
    pthread_mutex_init(&stdin_mutex, NULL);

    // Crea
    for (int i = 0; i < PROD_N; i++)
    {
        pthread_create(&cons_threads[i], NULL, main_productor, NULL);
    }

    for (int i = 0; i < CONS_N; i++)
    {
        pthread_create(&cons_threads[i], NULL, main_consumidor, NULL);
    }


    // Espera

    for (int i = 0; i < PROD_N; i++)
    {
        pthread_join(prod_threads[i], NULL);
    }

    for (int i = 0; i < CONS_N; i++)
    {
        pthread_join(cons_threads[i], NULL);
    }

    pthread_barrier_destroy(&barrier);
    pthread_mutex_destroy(&stdin_mutex);
    return 0;
}
