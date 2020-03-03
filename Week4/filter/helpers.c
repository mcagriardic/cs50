#include "helpers.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

// FILE *af = fopen("after.txt", "w");
// FILE *bf = fopen("before.txt", "w");
// fprintf(bf, "(%i)=%i-%i-%i,",column, image[row][column].rgbtRed, image[row][column].rgbtGreen, image[row][column].rgbtBlue);
// fprintf(af, "(%i)=%i-%i-%i,",width-column-1, image[row][column].rgbtRed, image[row][column].rgbtGreen, image[row][column].rgbtBlue);
// fclose(af);
// fclose(bf);

int Gx[3][3] = {
    {-1,0,1},
    {-2,0,2},
    {-1,0,1}};

int Gy[3][3] = {
    {-1,-2,-1},
    {0,0,0},
    {1,2,1}};

void PixelsAveraged(BYTE *rgbtRed, BYTE *rgbtGreen, BYTE *rgbtBlue)
{
    float averaged = (float) (*rgbtRed + *rgbtGreen + *rgbtBlue) / 3;
    int averaged_rounded = (int)round(averaged);
    *rgbtRed = averaged_rounded;
    *rgbtGreen = averaged_rounded;
    *rgbtBlue = averaged_rounded;
}

// Convert image to grayscale
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

void PixelReflected(int row, int column, int width, RGBTRIPLE image[][width])
{
    int reflected_column = width - 1 - column;
    RGBTRIPLE(*temp) = calloc(1, sizeof(RGBTRIPLE));

    *temp = image[row][reflected_column];
    image[row][reflected_column] = image[row][column];
    image[row][column] = *temp;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int row=0; row < height; row++)
    {
        for (int column=0; column < width/2; column++)
        {
            PixelReflected(row, column, width, image);
        }
    }

    return;
}

RGBTRIPLE SurroundedBoxesAveraged(int row, int column, int height, int width, RGBTRIPLE image[][width], RGBTRIPLE temp[][width])
{
    int unsigned bottom = (row + 1) >= height ? height-1 : (row + 1);
    int unsigned top = (row - 1) < 0 ? 0 : (row - 1);

    int unsigned right = (column + 1) >= width ? width-1 : (column + 1);
    int unsigned left = (column - 1) < 0 ? 0: (column - 1);

    int unsigned denominator = (bottom - top + 1) * (right-left + 1) - 1;

    float average_red = 0;
    float average_green = 0;
    float average_blue = 0;
    for (int i=0;  i <= bottom - top; i++)
    {
        for (int j = 0; j <= right - left; j++)
        {
            if (top + i != row || left + j != column)
            {
                average_red += image[top + i][left + j].rgbtRed;
                average_green += image[top + i][left + j].rgbtGreen;
                average_blue += image[top + i][left + j].rgbtBlue;
            }
        }
    }
    temp[row][column].rgbtRed = (int)round(average_red / denominator);
    temp[row][column].rgbtGreen = (int)round(average_green / denominator);
    temp[row][column].rgbtBlue = (int)round(average_blue / denominator);

    return temp[row][column];
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE(*temp)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    for (int row=0; row < height; row++)
    {
        for (int column=0; column < width; column++)
        {
            temp[row][column] = SurroundedBoxesAveraged(row, column, height, width, image, temp);
        }
    }
    
    for (int i=0; i < height; i++)
    {
        for (int j=0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }

    return;
}

RGBTRIPLE DetectEdges(int row, int column, int height, int width, RGBTRIPLE image[][width], RGBTRIPLE temp[][width])
{
    int unsigned bottom = (row + 1) >= height ? height-1 : (row + 1);
    int unsigned top = (row - 1) < 0 ? 0 : (row - 1);

    int unsigned right = (column + 1) >= width ? width-1 : (column + 1);
    int unsigned left = (column - 1) < 0 ? 0: (column - 1);

    float Gx_sum_red = 0;
    float Gx_sum_green = 0;
    float Gx_sum_blue = 0;

    float Gy_sum_red = 0;
    float Gy_sum_green = 0;
    float Gy_sum_blue = 0;

    float combined_red, combined_green, combined_blue;

    for (int i=0;  i <= bottom - top; i++)
    {
        for (int j = 0; j <= right - left; j++)
        {
            if (top + i != row || left + j != column)
            {
                Gx_sum_red += image[top+i][left+j].rgbtRed * Gx[i][j];
                Gx_sum_green += image[top+i][left+j].rgbtGreen * Gx[i][j];
                Gx_sum_blue += image[top+i][left+j].rgbtBlue * Gx[i][j];
                
                Gy_sum_red += image[top+i][left+j].rgbtRed * Gy[i][j];
                Gy_sum_green += image[top+i][left+j].rgbtGreen * Gy[i][j];
                Gy_sum_blue += image[top+i][left+j].rgbtBlue * Gy[i][j];
            }
        }
    }

    combined_red = (int)round(sqrt(pow(Gx_sum_red, 2) + pow(Gy_sum_red, 2)));
    combined_green = (int)round(sqrt(pow(Gx_sum_green, 2) + pow(Gy_sum_green, 2)));
    combined_blue = (int)round(sqrt(pow(Gx_sum_blue, 2) + pow(Gy_sum_blue, 2)));

    if (combined_red > 255)
    {
        combined_red = 255;
    }
    if (combined_green > 255)
    {
        combined_green = 255;
    }
    if (combined_blue > 255)
    {
        combined_blue = 255;
    }
    
    temp[row][column].rgbtRed = combined_red;
    temp[row][column].rgbtGreen = combined_green;
    temp[row][column].rgbtBlue = combined_blue;

    return temp[row][column];
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE(*temp)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    for (int row=0; row < height; row++)
    {
        for (int column=0; column < width; column++)
        {
            temp[row][column] = DetectEdges(row, column, height, width, image, temp);
        }
    }
    
    for (int i=0; i < height; i++)
    {
        for (int j=0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }
    return;
}
