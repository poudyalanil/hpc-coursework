#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define size 800

struct timespec start, end;
long long int time_used;

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

// initlizing matrix
void create_matrix(int matrix[size][size])
{
    int i, j;
    for (i = 0; i < size; i++)
        for (j = 0; j < size; j++)

            matrix[i][j] = rand() % 10;
}

int main()
{
    //using ijk algorithm
    int matrix1[size][size], matrix2[size][size], result[size][size];
    create_matrix(A);
    create_matrix(B);

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    for (int i = 0; i < size; i++)
    {
        for (int k = 0; k < size; k++)
        {
            for (int j = 0; j < size; j++)
            {
                result[i][j] = result[i][j] + matrix1[i][k] * matrix2[k][j];
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    calculate_time(&start, &end, &time_used);

    printf("%f\n", (time_used / 1.0e9));
}
