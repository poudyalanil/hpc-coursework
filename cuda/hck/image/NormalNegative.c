#include "lodepng.h"

#include <stdio.h>
#include <stdlib.h>

//compile with lodepng.c 

//gcc filename.c lodepng.c
int main(int argc, char **argv){

	unsigned int error;
	unsigned int encError;
	unsigned char* image;
	unsigned int width;
	unsigned int height;
	const char* filename = argv[1];
	const char* newFileName = "generated.png";

	int r;
	int g;
	int b;
	int t; //transparency

	error = lodepng_decode32_file(&image, &width, &height, filename);
	if(error){
		printf("error %u: %s\n", error, lodepng_error_text(error));
	}

	unsigned char newImage[height*width*4];

	printf("width = %d height = %d\n", width, height);
	for(int i = 0; i<height*width*4; i=i+4){
		r = image[i];
		g = image[1+i];
		b = image[2+i];
		t = image[3+i];

		printf("%d %d %d %d\n", r, g, b, t);

		newImage[i] = 255-r;
		newImage[1+i] = 255-g;
		newImage[2+i] = 255-b;
		newImage[3+i] = t;

	}
	
	encError = lodepng_encode32_file(newFileName, newImage, width, height);
	if(encError){
		printf("error %u: %s\n", error, lodepng_error_text(encError));
	}
	free(image);
		

	return 0;
}

