#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

// size of matrix
#define size 1024

int thread_count;
struct timespec start, end;
long long int time_used;

struct param
{
    int begin, finish;
};

int matrix1[size][size], matrix2[size][size], result[size][size];

// thread function for multiplying 2 matrices
void *multiply(void *arg)
{
    struct param *p = (struct param *)arg;

    int i, j, k;
    for (i = p->begin; i < p->finish; i++)
        for (j = 0; j < size; j++)
        {
            int s = 0;
            for (k = 0; k < size; k++)
                s += matrix1[i][k] * matrix2[k][j];
            result[i][j] = s;
        }
}

// initlizing matrix
void create_matrix(int matrix[size][size])
{
    int i, j;
    for (i = 0; i < size; i++)
        for (j = 0; j < size; j++)

            // populating matrix with random integers
            matrix[i][j] = rand() % 10;
}

int calculate_time(struct timespec *start, struct timespec *end,
                   long long int *diff)
{
    long long int in_sec = end->tv_sec - start->tv_sec;
    long long int in_nano = end->tv_nsec - start->tv_nsec;
    if (in_nano < 0)
    {
        in_sec--;
        in_nano += 1000000000;
    }
    *diff = in_sec * 1000000000 + in_nano;
    return !(*diff > 0);
}

int main()
{

    int i;

    printf("Number of threads: ");
    scanf("%d", &thread_count);
    struct param params[thread_count];
    pthread_t t[thread_count];

    //initalizing matrix
    create_matrix(matrix1);
    create_matrix(matrix2);

    //starting initial clock
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    for (i = 0; i < thread_count; i++)
    {
        int code;
        params[i].begin = i * (size / thread_count);
        params[i].finish = (i + 1) * (size / thread_count);

        code = pthread_create(&t[i], NULL, multiply, &params[i]);
        if (code != 0)
            printf("Error starting thread %d\n", i);
    }

    for (i = 0; i < thread_count; i++)
        pthread_join(t[i], NULL);

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    calculate_time(&start, &end, &time_used);

    printf("Time taken: %f s\n", (time_used / 1.0e9));
}