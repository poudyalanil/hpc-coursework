#include <stdio.h>
#include <stdlib.h>

#include "lodepng.h"

int get_time(struct timespec *start, struct timespec *end,
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

__device__ unsigned char getRed(unsigned char *image, unsigned int row, unsigned int col, unsigned int* width)
{
		char chaudai = *width;
    unsigned int i = (row * chaudai * 4) + (col * 4);
    return image[i];
}

__device__ unsigned char getGreen(unsigned char *image, unsigned int row, unsigned int col, unsigned int* width)
{
		char chaudai = *width;
    unsigned int i = (row * chaudai * 4) + (col * 4) + 1;
    return image[i];
}

__device__ unsigned char getBlue(unsigned char *image, unsigned int row, unsigned int col, unsigned int* width)
{
		char chaudai = *width;
    unsigned int i = (row * chaudai * 4) + (col * 4) + 2;
    return image[i];
}

__device__ unsigned char getAlpha(unsigned char *image, unsigned int row, unsigned int col, unsigned int* width)
{
		char chaudai = *width;
    unsigned int i = (row * chaudai * 4) + (col * 4) + 3;
    return image[i];
}

__device__ void setRed(unsigned char *image, unsigned int row, unsigned int col, unsigned char red, unsigned int* width)
{
		char chaudai = *width;
    unsigned int i = (row * chaudai * 4) + (col * 4);
    image[i] = red;
}

__device__ void setGreen(unsigned char *image, unsigned int row, unsigned int col, unsigned char green, unsigned int* width)
{
		char chaudai = *width;
    unsigned int i = (row * chaudai * 4) + (col * 4) + 1;
    image[i] = green;
}

__device__ void setBlue(unsigned char *image, unsigned int row, unsigned int col, unsigned char blue, unsigned int* width)
{
		char chaudai = *width;
    unsigned int i = (row * chaudai * 4) + (col * 4) + 2;
    image[i] = blue;
}

__device__ void setAlpha(unsigned char *image, unsigned int row, unsigned int col, unsigned char alpha, unsigned int * width)
{
		char chaudai = *width;
		unsigned int i = (row * chaudai * 4) + (col * 4) + 3;
		image[i] = alpha;
}


__global__ void blur(unsigned int *height, unsigned int *width, unsigned int *result, unsigned char * newImage, unsigned char * image){
		unsigned redTL, redTC, redTR;
    unsigned redL, redC, redR;
    unsigned redBL, redBC, redBR;
    unsigned newRed;

    unsigned greenTL, greenTC, greenTR;
    unsigned greenL, greenC, greenR;
    unsigned greenBL, greenBC, greenBR;
    unsigned newGreen;

    unsigned blueTL, blueTC, blueTR;
    unsigned blueL, blueC, blueR;
    unsigned blueBL, blueBC, blueBR;
		unsigned newBlue;
	

		// float filter[3][3] = {
		// 		{1.0 / 10, 1.0 / 10, 1.0 / 10},
		// 		{1.0 / 10, 1.0 / 10, 1.0 / 10},
		// 		{1.0 / 10, 1.0 / 10, 1.0 / 10}};


		float filter[3][3] = {
				{1.0 / 16, 2.0 / 16, 1.0/ 16},
				{2.0 / 16, 4.0 / 16, 2.0 / 16},
				{1.0 / 16, 2.0 / 16, 1.0 / 16}};

	for (unsigned int row = 1; row < *height - 1; row++)
    {
        for (unsigned int col = 1; col < *width - 1; col++)
        {
            setGreen(newImage, row, col, getGreen(image, row, col, width), width);
            setBlue(newImage, row, col, getBlue(image, row, col, width), width);
            setAlpha(newImage, row, col, 255, width);

            redTL = getRed(image, row - 1, col - 1, width);
            redTC = getRed(image, row - 1, col, width);
            redTR = getRed(image, row - 1, col + 1, width);

            redL = getRed(image, row, col - 1, width);
            redC = getRed(image, row, col, width);
            redR = getRed(image, row, col + 1, width);

            redBL = getRed(image, row + 1, col - 1, width);
            redBC = getRed(image, row + 1, col, width);
            redBR = getRed(image, row + 1, col + 1, width);

            newRed = redTL * filter[0][0] + redTC * filter[0][1] + redTR * filter[0][2] + redL * filter[1][0] + redC * filter[1][1] + redR * filter[1][2] + redBL * filter[2][0] + redBC * filter[2][1] + redBR * filter[2][2];

            setRed(newImage, row, col, newRed, width);

            greenTL = getGreen(image, row - 1, col - 1,width);
            greenTC = getGreen(image, row - 1, col,width);
            greenTR = getGreen(image, row - 1, col + 1,width);

            greenL = getGreen(image, row, col - 1,width);
            greenC = getGreen(image, row, col,width);
            greenR = getGreen(image, row, col + 1,width);

            greenBL = getGreen(image, row + 1, col - 1, width);
            greenBC = getGreen(image, row + 1, col, width);
            greenBR = getGreen(image, row + 1, col + 1,width);

            newGreen = greenTL * filter[0][0] + greenTC * filter[0][1] + greenTR * filter[0][2] + greenL * filter[1][0] + greenC * filter[1][1] + greenR * filter[1][2] + greenBL * filter[2][0] + greenBC * filter[2][1] + greenBR * filter[2][2];

            setGreen(newImage, row, col, newGreen, width);

            blueTL = getBlue(image, row - 1, col - 1, width);
            blueTC = getBlue(image, row - 1, col, width);
            blueTR = getBlue(image, row - 1, col + 1, width);

            blueL = getBlue(image, row, col - 1, width);
            blueC = getBlue(image, row, col, width);
            blueR = getBlue(image, row, col + 1, width);

            blueBL = getBlue(image, row + 1, col - 1, width);
            blueBC = getBlue(image, row + 1, col, width);
            blueBR = getBlue(image, row + 1, col + 1, width);
            newBlue = blueTL * filter[0][0] + blueTC * filter[0][1] + blueTR * filter[0][2] + blueL * filter[1][0] + blueC * filter[1][1] + blueR * filter[1][2] + blueBL * filter[2][0] + blueBC * filter[2][1] + blueBR * filter[2][2];
            setBlue(newImage, row, col, newBlue, width);
        }
    }
}

int main(void){
	// haleko
	unsigned int error;
	unsigned int encError;
	unsigned char* image;
	unsigned int width, height;
	const char* filename = "image.png";
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

	// haleko end

	unsigned int *gpuWidth, *gpuHeight, *gpuResult;

	cudaMalloc(&gpuWidth, sizeof(unsigned int));
	cudaMalloc(&gpuHeight, sizeof(unsigned int));
	cudaMalloc(&gpuResult, sizeof(unsigned int));

	// width = 2;
	// height = 10;


	cudaMemcpy(gpuWidth, &width, sizeof(unsigned int), cudaMemcpyHostToDevice);
	cudaMemcpy(gpuHeight, &height, sizeof(unsigned int), cudaMemcpyHostToDevice);
  
  struct timespec start, end;
  long long int elasped_time;

  clock_gettime(CLOCK_MONOTONIC_RAW, &start);
	blur<<<height, width>>>(gpuHeight, gpuWidth, gpuResult, d_out, d_in);
	// cudaMemcpy(&result, gpuResult, sizeof(int), cudaMemcpyDeviceToHost);
	// copy back the result array to the CPU
	cudaMemcpy(host_imageOutput, d_out, ARRAY_BYTES, cudaMemcpyDeviceToHost);
  clock_gettime(CLOCK_MONOTONIC_RAW, &end);


  get_time(&start, &end, &elasped_time);


	encError = lodepng_encode32_file(newFileName, host_imageOutput, width, height);
	if(encError){
		printf("error %u: %s\n", error, lodepng_error_text(encError));
	}
  printf("elasped time  %f s or %lld ns\n", (elasped_time / 1.0e9), (elasped_time));
  
	//free(image);
	//free(host_imageInput);
	cudaFree(d_in);
	cudaFree(d_out);

	return 0;
}
