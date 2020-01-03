#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filter.h"
#define size 30
int main(int argc, char **argv) {
  if (argc != 4 )
    {
      printf("Usage:./filter input.ppm kernel output.ppm\n");
      return 0;
    }
  FILE *fp;
  fp = fopen(argv[2],"r");
  int scale,dim;
  float entries;
  char c;
  int j,i = 0;
  int counters = 0;
  c = getc(fp);
  dim = c - '0';
  float kernel_matrix[dim*dim];
  for (c = getc(fp); c != EOF; c = getc(fp))
  {
    counters++;
    if(c == '\n' || counters == 1)
      continue;
    else if(counters == 3)
    {
      scale = c -'0';
      continue;
    }
    else if(counters == 4 && c != '\n' && c != 13)
    {
      int temp = c - '0';
      scale = scale*10 + temp;
    }
    else if(c == ' ')
      continue;
    else if(c == 13)
      continue;
    else
    {
      if(c == '-')/*negative entries*/
      {
        c = getc(fp);
        entries = 0 - (c - '0');
        kernel_matrix[i++] = entries;
      }
      else
      {
        entries = c - '0';
        kernel_matrix[i++] = entries;
      }
    }
  }
    for(j = 0;j<dim*dim;j++)
    {
      kernel_matrix[j] = (kernel_matrix[j])/scale;
    }

  char *inFile = argv[1], *outFile = argv[3];
  int height, width, max;
  RGB *values = readPPM(inFile, &width, &height, &max);
  values = Convolution(width,height,values,kernel_matrix,dim);
  writePPM(outFile, width, height, max, values);
  return 0;
}
RGB *Convolution( int width, int height, RGB *image,float kernel[],int dim)
{
     RGB *newImg = (RGB *)malloc(sizeof(RGB) * (height * width));
     int i,j;
     float entries[9];
     for(i = 0;i<(height * width);i++)
     {
       if(i == 0)
       {
         entries[4] = kernel[4];
         entries[5] = kernel[5];
         entries[7] = kernel[7];
         entries[8] = kernel[8];
         newImg[i].r = entries[4]*(image[i].r) + entries[5] * (image[i+1].r) + entries[7] * (image[i+width].r) + entries[8] * (image[i+width+1].r);
         newImg[i].g = entries[4]*(image[i].g) + entries[5] * (image[i+1].g) + entries[7] * (image[i+width].g) + entries[8] * (image[i+width+1].g);
         newImg[i].b = entries[4]*(image[i].b) + entries[5] * (image[i+1].b) + entries[7] * (image[i+width].b) + entries[8] * (image[i+width+1].b);
       }
       else if(i % width == 0 && (i + width) == (width * height))
       {
        entries[1] = kernel[1];
        entries[2] = kernel[2];
        entries[4] = kernel[4];
        entries[5] = kernel[5];
        newImg[i].r = entries[1]*(image[i - width].r) + entries[2] * (image[i-width+1].r) + entries[4] * (image[i].r) + entries[5] * (image[i+1].r);
        newImg[i].g = entries[1]*(image[i - width].g) + entries[2] * (image[i-width+1].g) + entries[4] * (image[i].g) + entries[5] * (image[i+1].g);
        newImg[i].b = entries[1]*(image[i - width].b) + entries[2] * (image[i-width+1].b) + entries[4] * (image[i].b) + entries[5] * (image[i+1].b);
       }
       else if(i % width == 0)
       {
         for(j = 0;j<9;j++)
         {
           if(j == 0 || j == 3 || j == 6)
              continue;
          else
            entries[j] = kernel[j];
         }
         newImg[i].r = entries[1]*(image[i - width].r) + entries[2] * (image[i-width+1].r) + entries[4] * (image[i].r) + entries[5] * (image[i+width+1].r) + entries[7] * (image[i+width].r)+entries[8] * (image[i+width-1].r);
         newImg[i].g = entries[1]*(image[i - width].g) + entries[2] * (image[i-width+1].g) + entries[4] * (image[i].g) + entries[5] * (image[i+width+1].g) + entries[7] * (image[i+width].g)+entries[8] * (image[i+width-1].g);
         newImg[i].b = entries[1]*(image[i - width].b) + entries[2] * (image[i-width+1].b) + entries[4] * (image[i].b) + entries[5] * (image[i+width+1].b) + entries[7] * (image[i+width].b)+entries[8] * (image[i+width-1].b);
       }

      else if((i>(width*height)-width) && i < (width*height-1))/*case 4*/
      {
        for(j = 0;j<9;j++)
        {
          if(j == 6 || j == 7 || j == 8)
            continue;
          entries[j] = kernel[j];
        }
        newImg[i].r = 0;
        newImg[i].g = 0;
        newImg[i].b = 0;
        for(j = 0;j<3;j++)
        {
          newImg[i].r += entries[j] * image[i-width-1+j].r;
          newImg[i].g += entries[j] * image[i-width-1+j].g;
          newImg[i].b += entries[j] * image[i-width-1+j].b;
        }
        for(j=0;j<6;j++)
        {
          newImg[i].r += entries[j+3] * image[i-1+j].r;
          newImg[i].g += entries[j+3] * image[i-1+j].g;
          newImg[i].b += entries[j+3] * image[i-1+j].b;
        }

      }
      else if(i == width*height-1)
      {
        for(j = 0;j<9;j++)
        {
          if(j == 0 || j == 1|| j == 3|| j == 4)
            entries[j] = kernel[j];
          else
            continue;
        }
        newImg[i].r = entries[0]*(image[i - width-1].r) + entries[1] * (image[i-width].r) + entries[3] * (image[i-1].r) + entries[4] * (image[i].r);
        newImg[i].g = entries[0]*(image[i - width-1].g) + entries[1] * (image[i-width].g) + entries[3] * (image[i-1].g) + entries[4] * (image[i].g);
        newImg[i].b = entries[0]*(image[i - width-1].b) + entries[1] * (image[i-width].b) + entries[3] * (image[i-1].b) + entries[4] * (image[i].b);
      }
      else if((i+1)%width == 0 && i != width-1)
      {
        for(j = 0;j<2;j++)
        {
          entries[j] = kernel[j];
          entries[j+3] = kernel[j+3];
          entries[j+6] = kernel[j+6];
        }
        newImg[i].r = entries[4]*(image[i].r) + entries[1] * (image[i-width].r)+entries[0] * (image[i-width-1].r) + entries[3] * (image[i-1].r) + entries[7] * (image[i+width].r) + entries[6] * (image[i+width-1].r);
        newImg[i].g = entries[4]*(image[i].g) + entries[1] * (image[i-width].g)+entries[0] * (image[i-width-1].g) + entries[3] * (image[i-1].g) + entries[7] * (image[i+width].g) + entries[6] * (image[i+width-1].g);
        newImg[i].b = entries[4]*(image[i].b) + entries[1] * (image[i-width].b)+entries[0] * (image[i-width-1].b) + entries[3] * (image[i-1].b) + entries[7] * (image[i+width].b) + entries[6] * (image[i+width-1].b);
      }
      else if(i == width -1 )
      {
        for(j = 3;j<5;j++)
        {
          entries[j]=kernel[j];
          entries[j+3] = kernel[j+3];
        }

        newImg[i].r = entries[4] * (image[i].r) + entries[3] * (image[i-1].r) + entries[7] * (image[i + width].r) + entries[6] * (image[i+width-1].r);
        newImg[i].g = entries[4] * (image[i].g) + entries[3] * (image[i-1].g) + entries[7] * (image[i + width].g) + entries[6] * (image[i+width-1].g);
        newImg[i].b = entries[4] * (image[i].b) + entries[3] * (image[i-1].b) + entries[7] * (image[i + width].b) + entries[6] * (image[i+width-1].b);
      }
      else if(i > 0 && i < width)
      {
        for(j = 3;j<9;j++)
          entries[j] = kernel[j];
        newImg[i].r = entries[4] * (image[i].r) + entries[3] * (image[i- 1].r) + entries[5] * (image[i+1].r) + entries[6] * (image[i+width-1].r) + entries[7] * (image[i+width].r) + entries[8] * (image[i+width+1].r);
        newImg[i].g = entries[4] * (image[i].g) + entries[3] * (image[i- 1].g) + entries[5] * (image[i+1].g) + entries[6] * (image[i+width-1].g) + entries[7] * (image[i+width].g) + entries[8] * (image[i+width+1].g);
        newImg[i].b = entries[4] * (image[i].b) + entries[3] * (image[i- 1].b) + entries[5] * (image[i+1].b) + entries[6] * (image[i+width-1].b) + entries[7] * (image[i+width].b) + entries[8] * (image[i+width+1].b);
      }
      else
      {
        newImg[i].r = kernel[0] * image[i-width-1].r + kernel[1] * image[i-width].r + kernel[2]*image[i-width+1].r + kernel[3] * image[i-1].r + kernel[4] * image[i].r + kernel[5] * image[i+1].r + kernel[6] * image[i+width-1].r + kernel[7] * image[i+width].r + kernel[8] * image[i+width+1].r;
        newImg[i].g = kernel[0] * image[i-width-1].g + kernel[1] * image[i-width].g + kernel[2]*image[i-width+1].g + kernel[3] * image[i-1].g + kernel[4] * image[i].g + kernel[5] * image[i+1].g + kernel[6] * image[i+width-1].g + kernel[7] * image[i+width].g + kernel[8] * image[i+width+1].g;
        newImg[i].b = kernel[0] * image[i-width-1].b + kernel[1] * image[i-width].b + kernel[2]*image[i-width+1].b + kernel[3] * image[i-1].b + kernel[4] * image[i].b + kernel[5] * image[i+1].b + kernel[6] * image[i+width-1].b + kernel[7] * image[i+width].b + kernel[8] * image[i+width+1].b;
      }


     }
     return newImg;
}
