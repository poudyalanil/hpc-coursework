#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <unistd.h>
#include <pthread.h>

// Time header
#include <time.h>

int count = 0; // A counter used to track the number of combinations explored so far

void substr(char *dest, char *src, int start, int length)
{
    memcpy(dest, src + start, length);
    *(dest + length) = '\0';
}

void *crack1(void *salt_and_encrypted)
{
    int x, y, z;   // Loop counters
    char salt[7];  // String used in hashing the password. Need space for \0 // incase you have modified the salt value, then should modifiy the number accordingly
    char plain[7]; // The combination of letters currently being checked // Please modifiy the number when you enlarge the encrypted password.
    char *enc;     // Pointer to the encrypted password

    substr(salt, salt_and_encrypted, 0, 6);

    for (x = 'A'; x <= 'M'; x++)
    {
        for (y = 'A'; y <= 'Z'; y++)
        {
            for (z = 0; z <= 99; z++)
            {
                sprintf(plain, "%c%c%02d", x, y, z);
                enc = (char *)crypt(plain, salt);
                count++;
                if (strcmp(salt_and_encrypted, enc) == 0)
                {
                    printf("#%-8d%s %s\n", count, plain, enc);
                    // exits when solution is found
                    // return;
                }
            }
        }
    }
}

void *crack2(void *salt_and_encrypted)
{
    int x, y, z;   // Loop counters
    char salt[7];  // String used in hashing the password. Need space for \0 // incase you have modified the salt value, then should modifiy the number accordingly
    char plain[7]; // The combination of letters currently being checked // Please modifiy the number when you enlarge the encrypted password.
    char *enc;     // Pointer to the encrypted password
    substr(salt, salt_and_encrypted, 0, 6);

    for (x = 'N'; x <= 'Z'; x++)
    {
        for (y = 'A'; y <= 'Z'; y++)
        {
            for (z = 0; z <= 99; z++)
            {
                sprintf(plain, "%c%c%02d", x, y, z);
                enc = (char *)crypt(plain, salt);
                count++;
                if (strcmp(salt_and_encrypted, enc) == 0)
                {
                    printf("#%-8d%s %s\n", count, plain, enc);
                    // exits when solution is found
                    // return;
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
    pthread_t thread_1, thread_2;
    int t1, t2;

    struct timespec start, end;
    long long int time_used;

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    t1 = pthread_create(&thread_1, NULL, crack1, "6$AS$P.Wy8B/NjpVgwGDKZ1uafxVzLNC7UpfX4yBca4BB03TvxHd0hRhjo0.qr1SpHDU2tzOTwTaVB5/8wm8f6Wgcf.");
    if (t1)
    {
        printf("Thread Creation failed:%d\n", t1);
    }

    t2 = pthread_create(&thread_2, NULL, crack2, "6$AS$P.Wy8B/NjpVgwGDKZ1uafxVzLNC7UpfX4yBca4BB03TvxHd0hRhjo0.qr1SpHDU2tzOTwTaVB5/8wm8f6Wgcf.");
    if (t2)
    {
        printf("Thread Creation failed:%d\n", t2);
    }

    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    printf("%d solutions explored\n", count);

    calculate_time(&start, &end, &time_used);

    printf("Time taken: %lld\n", time_used);
    return 0;
}
