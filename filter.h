#ifndef INCLUDED_FILTER_H
#define INCLUDED_FILTER_H

typedef struct { unsigned char r, g, b; } RGB;

RGB *readPPM(const char *file, int *width, int *height, int *max);

void writePPM(const char *file, int width, int height, int max,
              const RGB *image);

RGB *Convolution( int width,int height,RGB *image,float kernel[],int dim);


#endif
