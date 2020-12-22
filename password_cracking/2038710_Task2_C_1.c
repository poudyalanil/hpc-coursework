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
  struct timespec start, end;
  long long int time_used;

  clock_gettime(CLOCK_MONOTONIC_RAW, &start);

  crack("$6$AS$1nd8twt1KXW5uy.D.tkQ2xg.77h7IsAZdVMfZ8AHeELNWhtDojYhhCGZoViDefGvBRUgzuoBWr/EuVSa3Mnn6/"); //Copy and Paste your ecrypted password here using EncryptShA512 program

  clock_gettime(CLOCK_MONOTONIC_RAW, &end);

  printf("%d solutions explored\n", count);

  calculate_time(&start, &end, &time_used);

  printf("Time taken: %f seconds OR %lld Nano Seconds\n", (time_used / 1.0e9), (time_used));
  return 0;
}
