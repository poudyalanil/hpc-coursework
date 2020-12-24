#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>

#include "lodepng.h"

//compile with c++ lodepng file

//nvcc CudaNegative.cu lodepng.cpp

__global__ void square(unsigned char * gpu_imageOutput, unsigned char * gpu_imageInput){

	int r;
	int g;
	int b;
	int t;
	
	int idx = blockDim.x * blockIdx.x + threadIdx.x;

	int pixel = idx*4;
		r = gpu_imageInput[pixel];
		g = gpu_imageInput[1+pixel];
		b = gpu_imageInput[2+pixel];
		t = gpu_imageInput[3+pixel];

		gpu_imageOuput[pixel] = 255-r;
		gpu_imageOuput[1+pixel] = 255-g;
		gpu_imageOuput[2+pixel] = 255-b;
		gpu_imageOuput[3+pixel] = t;
}

int main(int argc, char **argv){

	unsigned int error;
	unsigned int encError;
	unsigned char* image;
	unsigned int width;
	unsigned int height;
	const char* filename = "4x4.png";
	const char* newFileName = "generated.png";

	error = lodepng_decode32_file(&image, &width, &height, filename);
	if(error){
		printf("error %u: %s\n", error, lodepng_error_text(error));
	}

	const int ARRAY_SIZE = width*height*4;
	const int ARRAY_BYTES = ARRAY_SIZE * sizeof(unsigned char);

	unsigned char host_imageInput[ARRAY_SIZE * 4];
	unsigned char host_imageOutput[ARRAY_SIZE * 4];

	for (int i = 0; i < ARRAY_SIZE; i++) {
		host_imageInput[i] = image[i];
	}

	// declare GPU memory pointers
	unsigned char * d_in;
	unsigned char * d_out;

	// allocate GPU memory
	cudaMalloc((void**) &d_in, ARRAY_BYTES);
	cudaMalloc((void**) &d_out, ARRAY_BYTES);

	cudaMemcpy(d_in, host_imageInput, ARRAY_BYTES, cudaMemcpyHostToDevice);

	// launch the kernel
	square<<<height, width>>>(d_out, d_in);

	// copy back the result array to the CPU
	cudaMemcpy(host_imageOutput, d_out, ARRAY_BYTES, cudaMemcpyDeviceToHost);
	
	encError = lodepng_encode32_file(newFileName, host_imageOutput, width, height);
	if(encError){
		printf("error %u: %s\n", error, lodepng_error_text(encError));
	}

	//free(image);
	//free(host_imageInput);
	cudaFree(d_in);
	cudaFree(d_out);

	return 0;
}
