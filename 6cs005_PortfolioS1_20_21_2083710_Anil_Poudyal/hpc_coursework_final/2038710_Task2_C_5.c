#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <unistd.h>
#include <pthread.h>

// Time header
#include <time.h>

// Password Cracking using multi threading

pthread_t thread_1, thread_2;

struct crypt_data *crypt_data1, *crypt_data2;

int count = 0;

void substr(char *dest, char *src, int start, int length)
{
    memcpy(dest, src + start, length);
    *(dest + length) = '\0';
}

void *kernel_function_1(char *salt_and_encrypted)
{
    int x, y, z;
    char salt[7];
    char plain[7];
    char *enc;

    substr(salt, salt_and_encrypted, 0, 6);

    for (x = 'A'; x <= 'M'; x++)
    {
        for (y = 'A'; y <= 'Z'; y++)
        {
            for (z = 0; z <= 99; z++)
            {
                sprintf(plain, "%c%c%02d", x, y, z);
                enc = (char *)crypt_r(plain, salt, crypt_data1);

                count++;
                if (strcmp(salt_and_encrypted, enc) == 0)
                {
                    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
                    printf("#%-8d%s %s\n", count, plain, enc);

                    pthread_cancel(thread_1);
                    pthread_cancel(thread_2);
                    pthread_exit(NULL);
                    return 0;
                }
                else
                {
                    sleep(0);
                }
            }
        }
    }
}

void *kernel_function_2(char *salt_and_encrypted)
{
    int x, y, z;
    char salt[7];
    char plain[7];
    char *enc;
    substr(salt, salt_and_encrypted, 0, 6);

    for (x = 'N'; x <= 'Z'; x++)
    {
        for (y = 'A'; y <= 'Z'; y++)
        {
            for (z = 0; z <= 99; z++)
            {
                sprintf(plain, "%c%c%02d", x, y, z);
                enc = (char *)crypt_r(plain, salt, crypt_data2);
                count++;

                if (strcmp(salt_and_encrypted, enc) == 0)
                {

                    printf("#%-8d%s %s\n", count, plain, enc);
                    pthread_cancel(thread_1);
                    pthread_cancel(thread_2);
                    pthread_exit(NULL);
                    return 0;
                }
                else
                {
                    sleep(0);
                }
            }
        }
    }
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

int main(int argc, char *argv[])
{
    int i;

    crypt_data1 = (struct crypt_data *)malloc(sizeof(struct crypt_data));
    crypt_data1->initialized = 0;

    crypt_data2 = (struct crypt_data *)malloc(sizeof(struct crypt_data));
    crypt_data2->initialized = 0;

    struct timespec start, end;
    long long int time_used;

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    pthread_create(&thread_1, NULL, (void *)kernel_function_1, "$6$AS$1nd8twt1KXW5uy.D.tkQ2xg.77h7IsAZdVMfZ8AHeELNWhtDojYhhCGZoViDefGvBRUgzuoBWr/EuVSa3Mnn6/");

    pthread_create(&thread_2, NULL, (void *)kernel_function_2, "$6$AS$1nd8twt1KXW5uy.D.tkQ2xg.77h7IsAZdVMfZ8AHeELNWhtDojYhhCGZoViDefGvBRUgzuoBWr/EuVSa3Mnn6/");

    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    calculate_time(&start, &end, &time_used);

    printf("Time taken: %f seconds OR %lld Nano Seconds\n", (time_used / 1.0e9), (time_used));
    return 0;
}
