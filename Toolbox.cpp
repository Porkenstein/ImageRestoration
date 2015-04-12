#include "Toolbox.h"

/***************************************************************************//**
 * FourierTransform
 * Author - Derek Stotz, Dan Andrus
 *
 * Transforms a given image from the spatial domain to the frequency domain.
 *
 * Parameters -
            image - the image object to transform.
 *
 * Returns
 *          true if successful, false if not
 ******************************************************************************/
void FourierTransform(Image& image, double ** frequencies)
{
    double theta;
    
    // Temp variables
    unsigned int i, r, c;
    
    // Allocate 2D arrays
    double ** real = alloc2d(image.Height(), image.Width());
    double ** imag = alloc2d(image.Height(), image.Width());
    double ** i_frequencies = alloc2d(image.Height(), image.Width());

    // Perform Fourier transform along the rows
    for (r = 0; r < image.Height(); r++)
    {
        for (c = 0; c < image.Width(); c++)
        {
            real[r][c] = 0;
            imag[r][c] = 0;
            for (i = 0; i <image.Width(); i++)
            {
                theta = (2 * M_PI * c * i )/ image.Width();
                real[r][c] += image[r][c].Intensity() * cos(theta);
                imag[r][c] += -1 * image[r][c].Intensity() * sin(theta);
            }
        }
    }

    // Perform Fourier transform along the columns
    for (r = 0; r < image.Height(); r++)
    {
        for (c = 0; c < image.Width(); c++)
        {
            frequencies[r][c] = 0;
            i_frequencies[r][c] = 0;
            for (i = 0; i <image.Height(); i++)
            {
                theta = (2 * M_PI * r * i )/ image.Height();
                frequencies[r][c] += real[r][c] * cos(theta) + imag[r][c] * sin(theta);
                i_frequencies[r][c] += -1 * real[r][c] * sin(theta) + imag[r][c] * cos(theta);
            }
        }
    }
    
    // Deallocate 2D arrays
    dealloc2d(real, image.Height());
    dealloc2d(imag, image.Height());
    dealloc2d(i_frequencies, image.Height());
}

/***************************************************************************//**
 * InverseFourierTransform
 * Author - Derek Stotz, Dan Andrus
 *
 * Transforms a given image from the frequency domain to the spatial domain.
 *
 * Parameters -
            image - the image object to transform.
 *
 * Returns
 *          true if successful, false if not
 ******************************************************************************/
void InverseFourierTransform(Image& image, double ** frequencies)
{
    double theta;
    
    // Temp variables
    unsigned int i, r, c;
    
    // Allocate 2D arrays
    double ** real = alloc2d(image.Height(), image.Width());
    double ** imag = alloc2d(image.Height(), image.Width());
    double ** i_frequencies = alloc2d(image.Height(), image.Width());
    
    // Perform inverse Fourier transform along the rows
    for (r = 0; r < image.Height(); r++)
    {
        for (c = 0; c < image.Width(); c++)
        {
            real[r][c] = 0;
            imag[r][c] = 0;
            for (i = 0; i <image.Width(); i++)
            {
                theta = (2 * M_PI * c * i )/ image.Width();
                real[r][c] += -1 * image[r][c].Intensity() * cos(theta);
                imag[r][c] += image[r][c].Intensity() * sin(theta);
            }
            real[r][c] /= image.Width();
            imag[r][c] /= image.Width();
        }
    }

    // Perform inverse Fourier transform along the columns
    for (r = 0; r < image.Height(); r++)
    {
        for (c = 0; c < image.Width(); c++)
        {
            frequencies[r][c] = 0;
            i_frequencies[r][c] = 0;
            for (i = 0; i <image.Height(); i++)
            {
                theta = (2 * M_PI * r * i )/ image.Height();
                frequencies[r][c] += -1 * real[r][c] * cos(theta) + imag[r][c] * sin(theta);
                i_frequencies[r][c] += real[r][c] * sin(theta) + imag[r][c] * cos(theta);
            }
            frequencies[r][c] /= image.Height();
            i_frequencies[r][c] /= image.Height();
        }
    }
    
    // Deallocate 2D arrays
    dealloc2d(real, image.Height());
    dealloc2d(imag, image.Height());
    dealloc2d(i_frequencies, image.Height());
}

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

