/***************************************************************************//**
 * DerekProcessor.cpp
 *
 * Author - Derek Stotz
 *
 * Date - January 30, 2015
 *
 * Point process functions implemented by Derek Stotz.  These functions are
 * named for easy addition into the image app created in prog1.cpp.
 ******************************************************************************/

#include "DerekProcessor.h"
#include <cmath>
#include <string>
#include <qfiledialog.h>
#include <stdlib.h>


/***************************************************************************//**
 * Menu_DerekFunctions_ApplyGamma
 * Author - Derek Stotz
 *
 * Applies a gamma transform to an image.  The gamma is requested as input.
 *
 * Parameters -
            image - the image object to manipulate.
 *
 * Returns
 *          true if successful, false if not
 ******************************************************************************/
bool DerekProcessor::Menu_DerekFunctions_ApplyGamma(Image& image)
{
  static unsigned char memory[3][256] = {{0},{0},{0}};

  double gamma = 0.0;

  // prompt the user for the gamma

  // Propt user for threshold value
  if (!Dialog("Gamma Correction").Add(gamma, "Gamma Value").Show())
    return false;

  // build a lookup table

  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 256; j++)
      memory[i][j] = (char)(pow((j/255.0), gamma)*255.0);

  if (image.IsNull()) return false;

  int rows = image.Height();
  int cols = image.Width();

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      image[i][j].SetRed(memory[0][image[i][j].Red()]);
      image[i][j].SetGreen(memory[1][image[i][j].Green()]);
      image[i][j].SetBlue(memory[2][image[i][j].Blue()]);
    }
  }

  return true;
}

/***************************************************************************//**
 * Menu_DerekFunctions_ApplyLogTransform
 * Author - Derek Stotz
 *
 * Applies an automatic log transform to an image.
 *
 * Parameters -
            image - the image object to manipulate.
 *
 * Returns
 *          true if successful, false if not
 ******************************************************************************/
bool DerekProcessor::Menu_DerekFunctions_ApplyLogTransform(Image& image)
{
  static unsigned char memory[3][256] = {{0},{0},{0}};
  static bool lookup_table = false;

  // Build the lookup table
  if (!lookup_table)
  {
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 256; j++)
        memory[i][j] = log(j+1.0)*(255.0/log(256.0));
    lookup_table = true;
  }

  if (image.IsNull()) return false;

  int rows = image.Height();
  int cols = image.Width();

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      image[i][j].SetRed(memory[0][image[i][j].Red()]);
      image[i][j].SetGreen(memory[1][image[i][j].Green()]);
      image[i][j].SetBlue(memory[2][image[i][j].Blue()]);
    }
  }

  return true;
}

/***************************************************************************//**
 * Menu_DerekFunctions_DisplayImageHistogram
 * Author - Derek Stotz
 *
 * Builds and displays the image histogram.
 *
 * Parameters -
            image - the image object to manipulate.
 *
 * Returns
 *          true if successful, false if not
 ******************************************************************************/
bool DerekProcessor::Menu_DerekFunctions_DisplayImageHistogram(Image& image)
{
  std::vector<uint>histogram = std::vector<uint>(256);

  int rows = image.Height();
  int cols = image.Width();

  //compute the histogram
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
        histogram[image[i][j].Intensity()]++;
    }
  }

  displayHistogram(histogram, "Image Histogram");
  return true;
}

/***************************************************************************//**
 * Menu_DerekFunctions_ApplyContinuousPseudocolor
 * Author - Derek Stotz
 *
 * Applies a continuous pseudocolorization to the image based on intensity values.
 *
 * Parameters -
            image - the image object to manipulate.
 *
 * Returns
 *          true if successful, false if not
 ******************************************************************************/
bool DerekProcessor::Menu_DerekFunctions_ApplyContinuousPseudocolor(Image& image)
{
  static unsigned char memory[3][256] = {{0},{0},{0}};
  static bool lookup_table = false;

  // Build the lookup table
  if (!lookup_table)
  {
    for (int j = 0; j < 256; j++)
    {
      // applying the example pseudocolor from class
      memory[0][j] = j;
      memory[1][j] = 255-abs(255-j*2);
      memory[2][j] = 255-j;
    }
    lookup_table = true;
  }

  if (image.IsNull()) return false;

  int rows = image.Height();
  int cols = image.Width();

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
        image[i][j].SetRed(memory[0][image[i][j].Intensity()]);
        image[i][j].SetGreen(memory[1][image[i][j].Intensity()]);
        image[i][j].SetBlue(memory[2][image[i][j].Intensity()]);
    }
  }

  return true;
}

/***************************************************************************//**
 * Menu_DerekFunctions_SubtractImage
 * Author - Derek Stotz
 *
 * Subtracts the color intensities of a picked image from the
 * intensities of another (the passed image)
 *
 * Parameters -
            image - the image object to manipulate.
 *
 * Returns
 *          true if successful, false if not
 ******************************************************************************/
bool DerekProcessor::Menu_DerekFunctions_SubtractImage(Image& image)
{
  std::string subtractor_filepath = QFileDialog::getOpenFileName().toStdString();
  Image subtractor(subtractor_filepath);

  if (image.IsNull() || subtractor.IsNull()) return false;

  //find the smallest dimension, so we don't step outside of either image
  int rows = std::min(image.Height(), subtractor.Height());
  int cols = std::min(image.Width(), subtractor.Width());

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      image[i][j].SetRed(image[i][j].Red() - subtractor[i][j].Red());
      image[i][j].SetGreen(image[i][j].Green() - subtractor[i][j].Green());
      image[i][j].SetBlue(image[i][j].Blue() - subtractor[i][j].Blue());
    }
  }

  return true;
}

/***************************************************************************//**
 * Menu_DerekFunctions_AutoContrastStretch
 * Author - Derek Stotz
 *
 * Automatically stretches the contrast so max intensity is 255 and min is 0.
 *
 * Parameters -
            image - the image object to manipulate.
 *
 * Returns
 *          true if successful, false if not
 ******************************************************************************/
bool DerekProcessor::Menu_DerekFunctions_AutoContrastStretch(Image& image)
{
    int rows = image.Height();
    int cols = image.Width();

    uchar min_i = 255;
    uchar max_i = 0;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            min_i = std::min(min_i, image[i][j].Intensity());
            max_i = std::max(max_i, image[i][j].Intensity());
        }
    }

    for (int i = 0; i < rows; i++)
    {
      for (int j = 0; j < cols; j++)
      {
          image[i][j].SetRed((255/(max_i - min_i)) * (image[i][j].Red() - min_i));
          image[i][j].SetGreen((255/(max_i - min_i)) * (image[i][j].Green() - min_i));
          image[i][j].SetBlue((255/(max_i - min_i)) * (image[i][j].Blue() - min_i));
      }
    }

    return true;
}

/***************************************************************************//**
 * Menu_DerekFunctions_ModifiedContrastStretch
 * Author - Derek Stotz
 *
 * Stretches the intensity values, clipping the top and bottom n percent of
 * intensity values
 *
 * Parameters -
            image - the image object to manipulate.
 *
 * Returns
 *          true if successful, false if not
 ******************************************************************************/
bool DerekProcessor::Menu_DerekFunctions_ModifiedContrastStretch(Image& image)
{
    int rows = image.Height();
    int cols = image.Width();
    int n_pixels = rows*cols;
    int min_p = 0;
    int max_p = 0;
    std::vector<uint>histogram = std::vector<uint>(256);

    // Propt user for threshold value
    if (!Dialog("Gamma Correction").Add(min_p, "Minimum Percentage", 0, 100).Add(max_p, "Maximum Percentage", 0, 100).Show())
      return false;

    uchar min_i = 255;
    uchar max_i = 0;

    // create list of intensity values
    for (int i = 0; i < rows; i++)
    {
      for (int j = 0; j < cols; j++)
      {
          histogram[image[i][j].Intensity()]++;
      }
    }

    min_p = (min_p /100.0) * n_pixels;
    max_p = (max_p /100.0) * n_pixels;

    // find the min_i by subtracting pixel counts from the pixels to ignore
    bool done = false;
    for (int i = 0; !done && i < 256; i++)
    {
        min_p -= histogram[i];
        if (min_p <= 0)\
        {
            min_i = i;
            done = true;
        }
    }

    // find the max_i by subtracting pixel counts from the pixels to ignore
    done = false;
    for (int i = 255; !done && i > 0; i--)
    {
        max_p -= histogram[i];
        if (max_p <= 0)\
        {
            max_i = i;
            done = true;
        }
    }

    for (int i = 0; i < rows; i++)
    {
      for (int j = 0; j < cols; j++)
      {
          image[i][j].SetRed((255/(max_i - min_i)) * (image[i][j].Red() - min_i));
          image[i][j].SetGreen((255/(max_i - min_i)) * (image[i][j].Green() - min_i));
          image[i][j].SetBlue((255/(max_i - min_i)) * (image[i][j].Blue() - min_i));
      }
    }

    return true;
}
