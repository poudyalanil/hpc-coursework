#include "lodepng.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

unsigned int width;
unsigned int height;

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

unsigned char getRed(unsigned char *image, unsigned int row, unsigned int col)
{
    unsigned int i = (row * width * 4) + (col * 4);
    return image[i];
}

unsigned char getGreen(unsigned char *image, unsigned int row, unsigned int col)
{
    unsigned int i = (row * width * 4) + (col * 4) + 1;
    return image[i];
}

unsigned char getBlue(unsigned char *image, unsigned int row, unsigned int col)
{
    unsigned int i = (row * width * 4) + (col * 4) + 2;
    return image[i];
}

unsigned char getAlpha(unsigned char *image, unsigned int row, unsigned int col)
{
    unsigned int i = (row * width * 4) + (col * 4) + 3;
    return image[i];
}

void setRed(unsigned char *image, unsigned int row, unsigned int col, unsigned char red)
{
    unsigned int i = (row * width * 4) + (col * 4);
    image[i] = red;
}

void setGreen(unsigned char *image, unsigned int row, unsigned int col, unsigned char green)
{
    unsigned int i = (row * width * 4) + (col * 4) + 1;
    image[i] = green;
}

void setBlue(unsigned char *image, unsigned int row, unsigned int col, unsigned char blue)
{
    unsigned int i = (row * width * 4) + (col * 4) + 2;
    image[i] = blue;
}

void setAlpha(unsigned char *image, unsigned int row, unsigned int col, unsigned char alpha)
{
    unsigned int i = (row * width * 4) + (col * 4) + 3;
    image[i] = alpha;
}

float filter[3][3] = {
    {1.0 / 10, 1.0 / 10, 1.0 / 10},
    {1.0 / 10, 1.0 / 10, 1.0 / 10},
    {1.0 / 10, 1.0 / 10, 1.0 / 10}};

int main(int argc, char **argv)
{
    unsigned char *image;
    const char *filename = argv[1];
    const char *newFileName = "filtered.png";
    unsigned char *newImage;

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

    struct timespec start, end;
    long long int elasped_time;

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    lodepng_decode32_file(&image, &width, &height, filename);
    newImage = malloc(height * width * 4 * sizeof(unsigned char));

    printf("Image width = %d height = %d\n", width, height);

    for (int row = 1; row < height - 1; row++)
    {
        for (int col = 1; col < width - 1; col++)
        {
            setGreen(newImage, row, col, getGreen(image, row, col));
            setBlue(newImage, row, col, getBlue(image, row, col));
            setAlpha(newImage, row, col, 255);

            redTL = getRed(image, row - 1, col - 1);
            redTC = getRed(image, row - 1, col);
            redTR = getRed(image, row - 1, col + 1);

            redL = getRed(image, row, col - 1);
            redC = getRed(image, row, col);
            redR = getRed(image, row, col + 1);

            redBL = getRed(image, row + 1, col - 1);
            redBC = getRed(image, row + 1, col);
            redBR = getRed(image, row + 1, col + 1);

            newRed = redTL * filter[0][0] + redTC * filter[0][1] + redTR * filter[0][2] + redL * filter[1][0] + redC * filter[1][1] + redR * filter[1][2] + redBL * filter[2][0] + redBC * filter[2][1] + redBR * filter[2][2];

            setRed(newImage, row, col, newRed);

            greenTL = getGreen(image, row - 1, col - 1);
            greenTC = getGreen(image, row - 1, col);
            greenTR = getGreen(image, row - 1, col + 1);

            greenL = getGreen(image, row, col - 1);
            greenC = getGreen(image, row, col);
            greenR = getGreen(image, row, col + 1);

            greenBL = getGreen(image, row + 1, col - 1);
            greenBC = getGreen(image, row + 1, col);
            greenBR = getGreen(image, row + 1, col + 1);

            newGreen = greenTL * filter[0][0] + greenTC * filter[0][1] + greenTR * filter[0][2] + greenL * filter[1][0] + greenC * filter[1][1] + greenR * filter[1][2] + greenBL * filter[2][0] + greenBC * filter[2][1] + greenBR * filter[2][2];

            setGreen(newImage, row, col, newGreen);

            blueTL = getBlue(image, row - 1, col - 1);
            blueTC = getBlue(image, row - 1, col);
            blueTR = getBlue(image, row - 1, col + 1);

            blueL = getBlue(image, row, col - 1);
            blueC = getBlue(image, row, col);
            blueR = getBlue(image, row, col + 1);

            blueBL = getBlue(image, row + 1, col - 1);
            blueBC = getBlue(image, row + 1, col);
            blueBR = getBlue(image, row + 1, col + 1);

            newBlue = blueTL * filter[0][0] + blueTC * filter[0][1] + blueTR * filter[0][2] + blueL * filter[1][0] + blueC * filter[1][1] + blueR * filter[1][2] + blueBL * filter[2][0] + blueBC * filter[2][1] + blueBR * filter[2][2];

            setBlue(newImage, row, col, newBlue);
        }
    }

    lodepng_encode32_file(newFileName, newImage, width, height);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    get_time(&start, &end, &elasped_time);
    printf("Time Elasped %f s OR %lld ns\n", (elasped_time / 1.0e9), (elasped_time));

    free(image);

    return 0;
}
