#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>

// #include "lodepng.h"

__global__ void square(int *height, int *width, int *result){
	*result = *height * *width;
}

int main(void){
	int width, height, result;
	int *gpuWidth, *gpuHeight, *gpuResult;

	cudaMalloc(&gpuWidth, sizeof(int));
	cudaMalloc(&gpuHeight, sizeof(int));
	cudaMalloc(&gpuResult, sizeof(int));

	width = 2;
	height = 10;


	cudaMemcpy(gpuWidth, &width, sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(gpuHeight, &height, sizeof(int), cudaMemcpyHostToDevice);
	
	square<<<height, width>>>(gpuHeight, gpuWidth, gpuResult);
	cudaMemcpy(&result, gpuResult, sizeof(int), cudaMemcpyDeviceToHost);

	printf("%d\n", result);
	
	return 0;
}
