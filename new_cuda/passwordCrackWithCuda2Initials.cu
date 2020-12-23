#include <stdio.h>

#include <cuda_runtime_api.h>

#include <time.h>

__device__ char* is_a_match(char * attempt) {
  char password1[] = "OKNXRT3171";

  char * newPassword = (char *) malloc(sizeof(char) * 11);

  newPassword[0] = password1[0] - 2;
	newPassword[1] = password1[0] + 2;
	newPassword[2] = password1[0] - 1;
	newPassword[3] = password1[1] - 3;
	newPassword[4] = password1[1] + 3;
	newPassword[5] = password1[1] + 1;
	newPassword[6] = password1[2] - 2;
	newPassword[7] = password1[2] + 2;
	newPassword[8] = password1[3] - 4;
	newPassword[9] = password1[3] + 4;
  newPassword[10] = '\0';

  printf("------");
  // 
  for(int i =0; i<10; i++){
    printf("%s\n", newPassword[i]);
    if(i >= 0 && i < 6){ //checking all lower case letter limits
      printf("%s", newPassword[i]);
			if(newPassword[i] > 122){
				newPassword[i] = (newPassword[i] - 122) + 97;
			}else if(newPassword[i] < 97){
				newPassword[i] = (97 - newPassword[i]) + 97;
			}
		}else{ //checking number section
			if(newPassword[i] > 57){
				newPassword[i] = (newPassword[i] - 57) + 48;
			}else if(newPassword[i] < 48){
				newPassword[i] = (48 - newPassword[i]) + 48;
			}
		}
	}
  // char * a = attempt;
  // char * pass1 = password1;

  // while ( * a == * pass1) {
  //   if ( * a == '\0') {
  //     printf("password:%s\n", password1);
  //     break;
  //   }
  //   a++;
  //   pass1++;
  // }
  // return 0;
  return newPassword;
}

__global__ void kernel() {
  char i1, i2;

  char password[7];
  password[6] = '\0';
  password[0] = blockIdx.x + 65;
  password[1] = threadIdx.x + 65;
  for (i1 = '0'; i1 <= '9'; i1++) {
    for (i2 = '0'; i2 <= '9'; i2++) {
      password[2] = i1;
      password[3] = i2;
      // if (is_a_match(password)) {
      //   printf("%s \n Encrypted\n", is_a_match(password));
      // } else {
      //   //printf("tried: %s\n",password);
      // }
      is_a_match(password);
    }
  }
}

int time_difference(struct timespec * start, struct timespec * finish, long long int * difference) {
  long long int ds = finish -> tv_sec - start -> tv_sec;
  long long int dn = finish -> tv_nsec - start -> tv_nsec;

  if (dn < 0) {
    ds--;
    dn += 1000000000;
  }
  * difference = ds * 1000000000 + dn;
  return !( * difference > 0);
}

int main() {

  struct timespec start, finish;
  long long int time_elapsed;

  clock_gettime(CLOCK_MONOTONIC, & start);

  kernel <<< 26, 26 >>> ();
  cudaThreadSynchronize();

  clock_gettime(CLOCK_MONOTONIC, & finish);
  time_difference( & start, & finish, & time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed,
    (time_elapsed / 1.0e9));
  return 0;
}