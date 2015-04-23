#include "Toolbox.h"

float ** Image_Freal;
float ** Image_Fimag;
Image Image_Spatial;
int Mouse_X;
int Mouse_Y;
int Mouse_Buttons;
Image Image_Original;

/***************************************************************************//**
 * drawCircle
 * Author - Dan Andrus
 *
 * Draws a circle on an image object. Circle is an inverted color circle,
 * meaning each pixel that lies on the circle will have its colors and
 * intensities inverted.
 *
 * Parameters -
 *          image - The image object to manipulate
 *          x - The column that the origin of the circle lies on
 *          y - The row that the origin of the circle lies on
 *          radius - The radius of the circle to draw
 *          thickness - The thickness in pixels to draw the border of the circle
 ******************************************************************************/
void drawCircle(Image& image, int x, int y, int radius, double thickness)
{
  int i, j;
  int olimit, ilimit;   // outer limit vs inner limit
  
  olimit = radius + thickness + 1;
  ilimit = sqrt(pow(radius,2) / 2) - thickness - 1;
  
  for (i = (y - olimit); i <= (y + olimit); i++)
  {
    // Double-check y boundaries
    if (i < 0 || i >= (int) image.Height()) continue;
    
    for (j = (x - olimit); j <= (x + olimit); j++)
    {
      // Double check x boundaries
      if (j < 0 || j >= (int) image.Width()) continue;
      
      // Don't waste time in interior
      if (i >= y - ilimit && i <= y + ilimit && j >= x - ilimit && j <= x + ilimit)
      {
        j = x + ilimit;
        continue;
      }
      
      if (abs(sqrt(pow(i - y, 2) + pow(j - x, 2)) - radius) <= (thickness / 2.0))
      {
        image[i][j].SetRGB(255-image[i][j].Red(),
                          255-image[i][j].Green(),
                          255-image[i][j].Blue());
      }
    }
  }
}

/***************************************************************************//**
 * alloc2d
 * Author - Dan Andrus
 *
 * Dynamically allocates a 2-dimensional array of doubles. Resulting array is
 * row-major.
 *
 * Parameters -
 *          rows - the number of rows in the array
 *          columns - the number of columns per row
 *
 * Returns
 *          The pointer into the array
 ******************************************************************************/
double** alloc2d(int rows, int columns)
{
  double** array = new double*[rows];
  for (int i = 0; i < rows; i++)
  {
    array[i] = new double[columns];
  }
  return array;
}

/***************************************************************************//**
 * alloc2d_f
 * Author - Dan Andrus
 *
 * Dynamically allocates a 2-dimensional array of floats. Resulting array is
 * row-major.
 *
 * Parameters -
 *          rows - the number of rows in the array
 *          columns - the number of columns per row
 *
 * Returns
 *          The pointer into the array
 ******************************************************************************/
float** alloc2d_f(int rows, int columns)
{
  float** array = new float*[rows];
  for (int i = 0; i < rows; i++)
  {
    array[i] = new float[columns];
  }
  return array;
}

/***************************************************************************//**
 * dealloc2d
 * Author - Dan Andrus
 *
 * Dynamically deallocates a 2-dimensional array of doubles.
 *
 * Parameters -
 *          array - The pointer to the array to deallocate
 *          rows - the number of rows in the array
 ******************************************************************************/
void dealloc2d(double** array, int rows)
{
  for (int i = 0; i < rows; i++)
  {
    delete [] array[i];
  }
  delete [] array;
}

/***************************************************************************//**
 * dealloc2d_f
 * Author - Dan Andrus
 *
 * Dynamically deallocates a 2-dimensional array of floats.
 *
 * Parameters -
 *          array - The pointer to the array to deallocate
 *          rows - the number of rows in the array
 ******************************************************************************/
void dealloc2d_f(float** array, int rows)
{
  for (int i = 0; i < rows; i++)
  {
    delete [] array[i];
  }
  delete [] array;
}

