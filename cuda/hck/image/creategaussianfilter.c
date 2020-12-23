#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIGMA 1.0

double *CreateGaussianFilter(int filterSize){

  int halfSize = filterSize/2;
  double x2, y2;
  double sigma = SIGMA;
  double e, g;
  double *filter;
  double sum = 0.0;
  
  filter = malloc( filterSize*filterSize*sizeof(double));
  for(int x = -halfSize; x <= halfSize; x++){
    for(int y = -halfSize; y <= halfSize; y++){
      x2 = x * x;
      y2 = y * y;
      e = exp(-(x2+y2)/(2.0*sigma*sigma));
      g = (1/(2*M_PI*sigma*sigma))*e;
      sum = sum + g;
      filter[(x+halfSize) + ((y+halfSize)*filterSize)] = g;
    }
  }
  for(int i=0; i < filterSize*filterSize; i++){
    filter[i] = filter[i]/sum;
  }
  return filter;
}

void main(int argc, char **argv){

  int filterSize = atoi(argv[1]);
  double *filter = CreateGaussianFilter(filterSize);

  for(int row = 0; row < filterSize ; row++){
    for(int col = 0; col < filterSize ; col++){
      printf("\t%lf,", filter[row*filterSize + col]);
    }
    printf("\n");
  }

}

