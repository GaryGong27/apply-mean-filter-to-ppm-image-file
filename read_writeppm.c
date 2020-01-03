#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filter.h"
RGB *readPPM(const char *file, int *width, int *height, int *max)
{
    FILE *fp;
    RGB *rgb;
    fp = fopen(file, "r");
    int i;

    fgetc(fp);
    fgetc(fp);
    fscanf(fp, "%d %d", width, height);
    fscanf(fp, "%d", max);
    rgb = (RGB *)malloc(sizeof(RGB) * (*(height) * (*width)));
    if (fp != NULL)
    {

        for (i = 0; i < ((*height) * (*width)); i++)
        {
            fscanf(fp, "%hhu%hhu%hhu", &rgb[i].r, &rgb[i].g, &rgb[i].b);
        }
        fclose(fp);
    }
    else
    {
        printf("error");
    }
    return rgb;
}
void writePPM(const char *file, int width, int height, int max, const RGB *image)
{
    int i;
    FILE *fp;
    fp = fopen(file, "w");

    fprintf(fp, "P3\n");
    fprintf(fp, "%d %d\n", width, height);
    fprintf(fp, "%d\n", max);

    for (i = 0; i < ((height) * (width)); i++)
    {
        fprintf(fp, "%d %d %d\n", image[i].r, image[i].g, image[i].b);
    }

    fclose(fp);
}
