#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <unistd.h>

// Time header
#include <time.h>

int count = 0; // A counter used to track the number of combinations explored so far

void substr(char *dest, char *src, int start, int length)
{
    memcpy(dest, src + start, length);
    *(dest + length) = '\0';
}

void crack(char *salt_and_encrypted)
{
    int x, y, z;   // Loop counters
    char salt[7];  // String used in hashing the password. Need space for \0 // incase you have modified the salt value, then should modifiy the number accordingly
    char plain[7]; // The combination of letters currently being checked // Please modifiy the number when you enlarge the encrypted password.
    char *enc;     // Pointer to the encrypted password

    substr(salt, salt_and_encrypted, 0, 6);

    for (x = 'A'; x <= 'M'; x++)
    {
        for (y = 'A'; y <= 'M'; y++)
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
                    return;
                }
                for (x = 'A'; x <= 'Z'; x++)
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
                                return;
                            }
                        }
                    }
                }
            }
        }
    }
}

void crack2(char *salt_and_encrypted)
{
    for (x = 'N'; x <= 'Z'; x++)
    {
        for (y = 'N'; y <= 'Z'; y++)
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
                    return;
                }
            }
        }
    }
}
int main(int argc, char *argv[])
{
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    crack("$6$AS$P.Wy8B/NjpVgwGDKZ1uafxVzLNC7UpfX4yBca4BB03TvxHd0hRhjo0.qr1SpHDU2tzOTwTaVB5/8wm8f6Wgcf."); //Copy and Paste your ecrypted password here using EncryptShA512 program
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC / 10;
    printf("%d solutions explored\n", count);

    printf("Time Taken:%f\n", cpu_time_used);
    return 0;
}
