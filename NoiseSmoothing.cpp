/***************************************************************************//**
 * DanProcessor.cpp
 *
 * Author - Dan Andrus
 *
 * Date - January 30, 2015
 *
 * Details - Defines some basic point processes that can be applied to images.
 * To be used with QT image library, or QT for short.
 *
 ******************************************************************************/

#include "DanProcessor.h"


/***************************************************************************//**
 * Menu_DanFunctions_Negate
 * Author - Dan Andrus
 *
 * Negates both color and grayscale images.
 *
 * Parameters - 
            image - the image object to manipulate.
 *
 * Returns
 *          true if successful, false if not
 ******************************************************************************/
bool DanProcessor::Menu_DanFunctions_Negate(Image& image)
{
  if (image.IsNull())
    return false;
  
  int rows = image.Height();
  int cols = image.Width();
  
  unsigned char lutable[256] = {0};
  
  // Build lookup table
  for (int i = 0; i < 256; i++)
    lutable[i] = 255-i;
  
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      // Negate each pixel
      image[i][j].SetRGB((lutable[image[i][j].Red()]),
                         (lutable[image[i][j].Green()]),
                         (lutable[image[i][j].Blue()]));
    }
  }
  
  return true;
}


/***************************************************************************//**
 * Menu_DanFunctions_Grayscale
 * Author - Dan Andrus
 *
 * Converts a color image to grayscale.
 *
 * Parameters - 
            image - the image object to manipulate.
 *
 * Returns
 *          true if successful, false if not
 ******************************************************************************/
bool DanProcessor::Menu_DanFunctions_Grayscale(Image& image)
{
  if (image.IsNull())
    return false;
  
  int rows = image.Height();
  int cols = image.Width();
  
  int gray;
  
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      // Calculate weighted averages
      gray = image[i][j].Red() * 30;
      gray += image[i][j].Green() * 59;
      gray += image[i][j].Blue() * 11;
      gray /= 100;
      
      // Set gray value
      image[i][j].SetGray(gray);
    }
  }
  
  return true;
}


/***************************************************************************//**
 * Menu_DanFunctions_BinaryThreshold
 * Author - Dan Andrus
 *
 * Converts both color and grayscale images to binary images. Allows the user
 * to specify a threshold value, where every pixel with intensity greater than
 * or equal to that threshold is set to full-bright (white), whereas all pixels
 * with intensity less than the threshold will be set to full-dark (black).
 *
 * Parameters - 
            image - the image object to manipulate.
 *
 * Returns
 *          true if successful, false if not
 ******************************************************************************/
bool DanProcessor::Menu_DanFunctions_BinaryThreshold(Image& image)
{
  if (image.IsNull())
    return false;
  
  int rows = image.Height();
  int cols = image.Width();
  
  int threshold;
  unsigned char lutable[256] = {0};
  
  // Propt user for threshold value
  threshold = 128;
  if (!Dialog("Binary Threshold").Add(threshold, "Threshold", 0, 255).Show())
    return false;
  
  // Convert image to grayscale
  if (!this -> Menu_DanFunctions_Grayscale(image))
    return false;
  
  // Build lookup table
  for (int i = 0; i < 256; i++)
    lutable[i] = (i < threshold ? 0 : 255);
  
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      // Perform binary thresholding
      image[i][j].SetGray(lutable[image[i][j].Red()]);
    }
  }
  
  return true;
}


/***************************************************************************//**
 * Menu_DanFunctions_Posterize
 * Author - Dan Andrus
 *
 * Posterizes an image, gradients and giving the image a more cartoonish look.
 * Essentially limits the color pallette to a small number of colors. Promps
 * the user for the number of levels to use, with 4 being the default.
 *
 * Parameters - 
            image - the image object to manipulate.
 *
 * Returns
 *          true if successful, false if not
 ******************************************************************************/
bool DanProcessor::Menu_DanFunctions_Posterize(Image& image)
{
  if (image.IsNull())
    return false;
  
  int rows = image.Height();
  int cols = image.Width();
  
  int level;
  int levels;
  double temp;
  double lsize;
  unsigned char lutable[256] = {0};
  
  // Propt user for threshold value
  levels = 4;
  if (!Dialog("Posterize Levels").Add(levels, "Levels", 1, 256).Show())
    return false;
  
  // Couple calculations
  lsize = 256.0 / levels;
  
  // Build lookup table
  for (int i = 0; i < 256; i++)
  {
    // Determine current level
    level = (int) (i / lsize);
    
    // Scale to range and truncate
    temp = (int) (level * lsize);
    
    // Avoid overall darkening by distributing poserize levels
    // We aren't using floor, ceil, or median; posterize level is determined by
    //   the level itself.
    temp += lsize * ((double) level / ((double) levels - 1));
    
    // Avoid overflow in either direction just to be safe
    if (temp > 255) temp = 255;
    if (temp < 0)   temp = 0;
    
    // Save to lookup table (and truncate)
    lutable[i] = (unsigned char) temp;
  }

  // Perform posterization on each color of each pixel  
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      image[i][j].SetRGB((lutable[image[i][j].Red()]),
                         (lutable[image[i][j].Green()]),
                         (lutable[image[i][j].Blue()]));
    }
  }
  
  return true;
}


/***************************************************************************//**
 * Menu_DanFunctions_Brighten
 * Author - Dan Andrus
 *
 * Prompts the user for a brightness/darkness value to apply to the image.
 * Performs a simple linear brighten function, which may cause colors to be
 * washed out.
 *
 * Parameters - 
            image - the image object to manipulate.
 *
 * Returns
 *          true if successful, false if not
 ******************************************************************************/
bool DanProcessor::Menu_DanFunctions_Brighten(Image& image)
{
  if (image.IsNull())
    return false;
  
  int rows = image.Height();
  int cols = image.Width();
  
  int brightness;
  int maxpixel;
  int minpixel;
  unsigned char lutable[256] = {0};
  
  // Propt user for threshold value
  brightness = 0;
  if (!Dialog("Brightness").Add(brightness, "Brightness", -255, 255).Show())
    return false;
  
  maxpixel = 255 - brightness;
  minpixel = 0 - brightness;
  
  // Build lookup table
  for (int i = 0; i < 256; i++)
    lutable[i] = (unsigned char) (i <= minpixel ? 0 :
                                  i >= maxpixel ? 255 :
                                  i + brightness);
  
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      // Brighten each color
      image[i][j].SetRGB((lutable[image[i][j].Red()]),
                         (lutable[image[i][j].Green()]),
                         (lutable[image[i][j].Blue()]));
    }
  }
  
  return true;
}


/***************************************************************************//**
 * Menu_DanFunctions_LinearContrast
 * Author - Dan Andrus
 *
 * Applies a linear contrast stretch to the image. Prompts the user for a
 * minimum intensity and a maximum intensity, then pixels with intensities
 * within that range will be scaled to stretch the full range of intensities.
 * Pixels with intensities greater than the max will be set to full-bright
 * (white), whereas pixels with intensities lower than the min will be set to
 * full-dark (black).
 *
 * Parameters - 
            image - the image object to manipulate.
 *
 * Returns
 *          true if successful, false if not
 ******************************************************************************/
bool DanProcessor::Menu_DanFunctions_LinearContrast(Image& image)
{
  if (image.IsNull())
    return false;
  
  int rows = image.Height();
  int cols = image.Width();
  
  int lowbound;
  int upbound;
  double scale;
  unsigned char lutable[256] = {0};
  
  // Propt user for threshold value
  lowbound = 15;
  upbound = 240;
  if (!Dialog("Linear Contrast").Add(lowbound, "Minimum Intensity", 0, 255)
        .Add(upbound, "Maximum Intensity", 0, 255).Show())
    return false;
  
  // If reversed, flip 'em
  if (lowbound > upbound)
  {
    lowbound ^= upbound;
    upbound ^= lowbound;
    lowbound ^= upbound;
  }
  
  // Calculate scale
  if (lowbound != upbound)
    scale = 255 / (upbound - lowbound);
  else
    scale = 255;
  
  // Build lookup table
  for (int i = 0; i < 256; i++)
    lutable[i] = (unsigned char) (i <= lowbound ? 0 :
                                  i >= upbound ? 255 :
                                  ((i - lowbound) * scale) + 0.5);
  
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      // Add contrast to each color
      image[i][j].SetRGB((lutable[image[i][j].Red()]),
                         (lutable[image[i][j].Green()]),
                         (lutable[image[i][j].Blue()]));
    }
  }
  
  return true;
}


/***************************************************************************//**
 * Menu_DanFunctions_8LevelPseudocolor
 * Author - Dan Andrus
 *
 * Applies an 8-level pseudocolor to an image. Each pixel is given a color based
 * on intensity. The color order from dark to light is as follows: black, red,
 * yellow, green, cyan, blue, magenta, and white.
 * 
 *
 * Parameters - 
            image - the image object to manipulate.
 *
 * Returns
 *          true if successful, false if not
 ******************************************************************************/
bool DanProcessor::Menu_DanFunctions_8LevelPseudocolor(Image& image)
{
  if (image.IsNull())
    return false;
  
  int rows = image.Height();
  int cols = image.Width();
  
  unsigned char lutable[256][3];
  unsigned char colortable[8][3] = {
    {0, 0, 0},
    {255, 0, 0},
    {255, 255, 0},
    {0, 255, 0},
    {0, 255, 255},
    {0, 0, 255},
    {255, 0, 255},
    {255, 255, 255}
  };
  
  // Build lookup table
  for (int i = 0; i < 256; i++)
  {
    lutable[i][0] = colortable[i / 32][0];
    lutable[i][1] = colortable[i / 32][1];
    lutable[i][2] = colortable[i / 32][2];
  }
  
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      // Pseudocolor each pixel based on intensity
      image[i][j].SetRGB(lutable[image[i][j].Intensity()][0],
                         lutable[image[i][j].Intensity()][1],
                         lutable[image[i][j].Intensity()][2]);
    }
  }
  
  return true;
}



/***************************************************************************//**
 * Menu_DanFunctions_HistogramEqualization
 * Author - Dan Andrus
 *
 * Applies a histogram equalization to an image.
 * 
 *
 * Parameters - 
            image - the image object to manipulate.
 *
 * Returns
 *          true if successful, false if not
 ******************************************************************************/
bool DanProcessor::Menu_DanFunctions_Equalize(Image& image)
{
  if (image.IsNull())
    return false;
  
  int rows = image.Height();
  int cols = image.Width();
  int total = rows * cols;
  int tally = 0;
  int tmp;
  
  unsigned char lutable[256];
  int histogram[256] = {0};
  
  // Build histogram
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      histogram[image[i][j].Intensity()]++;
    }
  }
  
  
  // Build lookup table based on histogram
  for (int i = 0; i < 256; i++)
  {
    tally += histogram[i];
    tmp = (int) (tally / (total / 256.0));
    if (tmp < 0) tmp = 0;
    if (tmp > 255) tmp = 255;
    lutable[i] = tmp;
  }
  
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      // Pseudocolor each pixel based on intensity
      image[i][j].SetIntensity(lutable[image[i][j].Intensity()]);
    }
  }
  
  return true;
}



/***************************************************************************//**
 * Menu_DanFunctions_HistogramEqualizationWithClipping
 * Author - Dan Andrus
 *
 * Applies a histogram equalization with optional clipping to an image.
 * 
 *
 * Parameters - 
            image - the image object to manipulate.
 *
 * Returns
 *          true if successful, false if not
 ******************************************************************************/
bool DanProcessor::Menu_DanFunctions_EqualizeWithClipping
  (Image& image)
{
  if (image.IsNull())
    return false;
  
  int rows = image.Height();
  int cols = image.Width();
  int total = rows * cols;
  int tally = 0;
  int tmp;
  double max;
  
  unsigned char lutable[256];
  int histogram[256] = {0};
  
  // Propt user for threshold value
  max = 0;
  if (!Dialog("Ignore Percentage").Add(max, "Percentage", 0, 100).Show())
    return false;
  
  // Make sure max is greater than 0, otherwise we get a division by 0
  if (max == 0)
    return false;
  
  // Build histogram
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      histogram[image[i][j].Intensity()]++;
    }
  }
  
  // Format max for histogram
  max = (int) (total * (max / 100));
  
  // Clip histogram and recalculate total
  total = 0;
  for (int i = 0; i < 256; i++)
  {
    if (histogram[i] > max)
      histogram[i] = max;
    total += histogram[i];
  }
  
  // Build lookup table based on histogram
  for (int i = 0; i < 256; i++)
  {
    tally += histogram[i];
    tmp = (int) (tally / (total / 256.0));
    if (tmp < 0) tmp = 0;
    if (tmp > 255) tmp = 255;
    lutable[i] = (unsigned char) tmp;
  }
  
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      // Pseudocolor each pixel based on intensity
      image[i][j].SetIntensity(lutable[image[i][j].Intensity()]);
    }
  }
  
  return true;
}



