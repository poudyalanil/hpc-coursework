#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Time header
#include <time.h>
 
//Password Cracking using CUDA 

__device__ char* encryptDecrypt(char* tempPassword){

	char * generatedPassword = (char *) malloc(sizeof(char) * 11);

	generatedPassword[0] = tempPassword[0] + 2;
	generatedPassword[1] = tempPassword[0] - 2;
	generatedPassword[2] = tempPassword[0] + 1;
	generatedPassword[3] = tempPassword[1] + 3;
	generatedPassword[4] = tempPassword[1] - 3;
	generatedPassword[5] = tempPassword[1] - 1;
	generatedPassword[6] = tempPassword[2] + 2;
	generatedPassword[7] = tempPassword[2] - 2;
	generatedPassword[8] = tempPassword[3] + 4;
	generatedPassword[9] = tempPassword[3] - 4;
	generatedPassword[10] = '\0';

	for(int i =0; i<10; i++){
		if(i >= 0 && i < 6){ 
			if(generatedPassword[i] > 122){
				generatedPassword[i] = (generatedPassword[i] - 122) + 97;
			}else if(generatedPassword[i] < 97){
				generatedPassword[i] = (97 - generatedPassword[i]) + 97;
			}
		}else{ 
			if(generatedPassword[i] > 57){
				generatedPassword[i] = (generatedPassword[i] - 57) + 48;
			}else if(generatedPassword[i] < 48){
				generatedPassword[i] = (48 - generatedPassword[i]) + 48;
			}
		}
	}
	return generatedPassword;
}

__global__ void crack(char * alphabet, char * numbers){

char matchedPassword[4];

matchedPassword[0] = alphabet[blockIdx.x];
matchedPassword[1] = alphabet[blockIdx.y];

matchedPassword[2] = numbers[threadIdx.x];
matchedPassword[3] = numbers[threadIdx.y];


char* encryptedPassword = "xtwcvx5171"; //vy33
char* search = encryptDecrypt(matchedPassword);
int iter = 0;
int is_match = 0;
while (*encryptedPassword != '\0' || *search != '\0') {
	if (*encryptedPassword == *search) {
		encryptedPassword++;
		search++;
	} else if ((*encryptedPassword == '\0' && *search != '\0') || (*encryptedPassword != '\0' && *search == '\0') || *encryptedPassword != *search) {
		is_match = 1;
	
		break;
	}
}
if (is_match == 0) {
	printf("Password Found: %c%c%c%c \n", matchedPassword[0],matchedPassword[1],matchedPassword[2],matchedPassword[3]);
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

int main(int argc, char ** argv){

    struct timespec start, end;
    long long int time_used;

	char cpuLetters[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
	char cpuDigits[26] = {'0','1','2','3','4','5','6','7','8','9'};

	char * gpuLetters;
	cudaMalloc( (void**) &gpuLetters, sizeof(char) * 26); 
	cudaMemcpy(gpuLetters, cpuLetters, sizeof(char) * 26, cudaMemcpyHostToDevice);

	char * gpuDigits;
	cudaMalloc( (void**) &gpuDigits, sizeof(char) * 26); 
	cudaMemcpy(gpuDigits, cpuDigits, sizeof(char) * 26, cudaMemcpyHostToDevice);
    
  clock_gettime(CLOCK_MONOTONIC_RAW, &start);
	crack<<< dim3(26,26,1), dim3(10,10,1) >>>( gpuLetters, gpuDigits );
  cudaThreadSynchronize();

  clock_gettime(CLOCK_MONOTONIC_RAW, &end);
  calculate_time(&start, &end, &time_used);

  printf("Time taken: %f seconds OR %lld Nano Seconds\n", (time_used / 1.0e9), (time_used));
  
    
	return 0;
}












