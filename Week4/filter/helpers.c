#include "helpers.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

// Convert image to grayscale

void PixelsAveraged(BYTE *rgbtRed, BYTE *rgbtGreen, BYTE *rgbtBlue)
{
    float averaged = (float) (*rgbtRed + *rgbtGreen + *rgbtBlue) / 3;
    int averaged_rounded = (int)round(averaged);
    *rgbtRed = averaged_rounded;
    *rgbtGreen = averaged_rounded;
    *rgbtBlue = averaged_rounded;
}

void PixelReflected(int row, int column, int width, RGBTRIPLE image[][width])
{
    int reflected_column = width - 1 - column;
    RGBTRIPLE(*temp) = calloc(1, sizeof(RGBTRIPLE));

    *temp = image[row][reflected_column];
    image[row][reflected_column] = image[row][column];
    image[row][column] = *temp;


}

void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // printf("Height: %i - width: %i\n", height, width);

    for (int row=0; row < height; row++)
    {
        for (int column=0; column < width; column++)
        {
            PixelsAveraged(&image[row][column].rgbtRed,
                           &image[row][column].rgbtGreen,
                           &image[row][column].rgbtBlue);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // FILE *af = fopen("after.txt", "w");
    // FILE *bf = fopen("before.txt", "w");
    for (int row=0; row < height; row++)
    {
        printf("At row: %i ...\n", row);
        for (int column=0; column < width/2; column++)
        {
            // printf("BEFORE SWITCH INSIDE REFLECT ...\n");
            // printf("Temp Red: %d...\n", image[row][column].rgbtRed);
            // printf("Temp Green: %d...\n", image[row][column].rgbtGreen);
            // printf("Temp Blue: %d...\n\n", image[row][column].rgbtBlue);
            // fprintf(bf, "(%i)=%i-%i-%i,",column, image[row][column].rgbtRed, image[row][column].rgbtGreen, image[row][column].rgbtBlue);
            PixelReflected(row, column, width, image);
            // fprintf(af, "(%i)=%i-%i-%i,",width-column-1, image[row][column].rgbtRed, image[row][column].rgbtGreen, image[row][column].rgbtBlue);
        }
    }
    // fclose(af);
    // fclose(bf);
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}
