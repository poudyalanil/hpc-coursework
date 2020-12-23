#include <stdio.h>
#include <stdlib.h>

//__global__ --> GPU function which can be launched by many blocks and threads
//__device__ --> GPU function or variables
//__host__ --> CPU function or variables


//This function compares the encrypted values obtained from CudaCrypt method with input encrypted value
__device__ int cracker(char* enc,char* gen){
	int flag=0;
	while (*enc != '\0' || *gen != '\0') { 
        if (*enc == *gen) { 
            enc++; 
            gen++; 
        } 
  
        // If two characters are not same 
        // print the difference and exit 
        else if ((*enc == '\0' && *gen != '\0') 
                 || (*enc != '\0' && *gen == '\0') 
                 || *enc != *gen) { 
            flag = 1; 
			//printf("Uequal Strings\n"); 
            break; 
        } 
    } 
  
    // If two strings are exactly same 
    if (flag == 0) { 
		printf("Equal Strings\n"); 
		return 0;
	} 
	else{
		return 1;
	}
}
__device__ char* CudaCrypt(char* rawPassword){

	char * newPassword = (char *) malloc(sizeof(char) * 11);

	newPassword[0] = rawPassword[0] + 2;
	newPassword[1] = rawPassword[0] - 2;
	newPassword[2] = rawPassword[0] + 1;
	newPassword[3] = rawPassword[1] + 3;
	newPassword[4] = rawPassword[1] - 3;
	newPassword[5] = rawPassword[1] - 1;
	newPassword[6] = rawPassword[2] + 2;
	newPassword[7] = rawPassword[2] - 2;
	newPassword[8] = rawPassword[3] + 4;
	newPassword[9] = rawPassword[3] - 4;
	newPassword[10] = '\0';

	for(int i =0; i<10; i++){
		if(i >= 0 && i < 6){ //checking all lower case letter limits
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
	return newPassword;
}

__global__ void crack(char * alphabet, char * numbers){
int result;
char genRawPass[4];
char encrypted[]= "uqtcvx5144";

genRawPass[0] = alphabet[blockIdx.x];
genRawPass[1] = alphabet[blockIdx.y];

genRawPass[2] = numbers[threadIdx.x];
genRawPass[3] = numbers[threadIdx.y];

char *generated={CudaCrypt(genRawPass)};
result = cracker(encrypted,generated);
int flag=0;
	while (*enc != '\0' || *gen != '\0') { 
        if (*enc == *gen) { 
            enc++; 
            gen++; 
        } 
  
        // If two characters are not same 
        // print the difference and exit 
        else if ((*enc == '\0' && *gen != '\0') 
                 || (*enc != '\0' && *gen == '\0') 
                 || *enc != *gen) { 
            flag = 1; 
			//printf("Uequal Strings\n"); 
            break; 
        } 
    } 
  
    // If two strings are exactly same 
    if (flag == 0) { 
		printf("Equal Strings\n"); 
		return 0;
	} 
	else{
		return 1;
	}


}

int main(int argc, char ** argv){
//char encrypted[]="cxbdwy2734";
char cpuAlphabet[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
char cpuNumbers[26] = {'0','1','2','3','4','5','6','7','8','9'};

char * gpuAlphabet;
cudaMalloc( (void**) &gpuAlphabet, sizeof(char) * 26); 
cudaMemcpy(gpuAlphabet, cpuAlphabet, sizeof(char) * 26, cudaMemcpyHostToDevice);

char * gpuNumbers;
cudaMalloc( (void**) &gpuNumbers, sizeof(char) * 26); 
cudaMemcpy(gpuNumbers, cpuNumbers, sizeof(char) * 26, cudaMemcpyHostToDevice);

crack<<< dim3(26,26,1), dim3(10,10,1) >>>( gpuAlphabet, gpuNumbers);
cudaThreadSynchronize();
return 0;
}