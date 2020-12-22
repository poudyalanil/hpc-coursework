#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

// size of matrix
#define size 1024

int thread_count;
clock_t start, end;
double time_used;

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
    start = clock();
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

    end = clock();

    time_used = ((double)(end - start)) / CLOCKS_PER_SEC / 10;
    printf("Time taken: %f\n", time_used);
}