#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t  BYTE;

int main(int argc, char *argv[])
{
    char *infile = argv[1];
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not open %s.\n", infile);
        return 1;
    }
    char file_name[10];
    FILE* img;
    BYTE buffer[512];
    int jpeg_count = 0;
    while (fread(&buffer, 512, 1, inptr))
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8
            && buffer[2] == 0xff && buffer[3] > 0xe)
        {
            jpeg_count++;
            // printf("JPEG COUNT: %i ...\n", jpeg_count);
            if (jpeg_count > 1)
            {
                fclose(img);
            }
            sprintf(file_name, "%03i.jpg", jpeg_count - 1);
            img = fopen(file_name, "a");
            // printf("Byte 0: 0x%02x - Byte 1: 0x%02x\n", buffer[0], buffer[1]);
        }

        if (jpeg_count > 0)
        {
            fwrite(&buffer, 512, 1, img);    
        }
    }
    printf("There are in total, %i JPEG files in this memory card...\n", jpeg_count);
}

