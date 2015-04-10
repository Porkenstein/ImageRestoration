/***************************************************************************//**
 * Filters.cpp
 *
 * Author - Derek Stotz
 *
 * Date - April 8, 2015
 *
 * Method declarations for the Filters class.
 ******************************************************************************/

#include "Filters.h"
#include <cmath>
#include <string>
#include <qfiledialog.h>
#include <stdlib.h>

/***************************************************************************//**
 * Menu_Filters_FourierTransform
 * Author - Derek Stotz
 *
 * Applies the fast Fourier transform to an image and normalizes the resulting
 * frequency data to be displayed to the user. Overwrites the original image.
 *
 * Parameters -
 *          image - the image object to manipulate.
 *
 * Returns
 *          true if successful, false if not
 ******************************************************************************/
bool Filters::Menu_Filters_FourierTransform(Image& image)
{
  // allocate the frequency array
  double ** frequencies = new double*[image.Height()];
  for (int i = 0; i < image.Height(); i++)
  {
      frequencies[i] = new double[image.Width()];
  }

  // transform to the frequency domain
  FourierTransform(image, frequencies);

  // find the min and max for normalization
  double fmin = log(frequencies[0][0]);
  double fmax = log(frequencies[0][0]);
  for (int c = 0; c < image.Width(); c++)
  {
      for (int r = 0; r < image.Height(); r++)
      {
        frequencies[c][r] = log(frequencies[c][r]);
        if(frequencies[c][r] < fmin)
            fmin = frequencies[c][r];
        if(frequencies[c][r] > fmax)
            fmax = frequencies[c][r];
      }
  }
  double scalar = (fmax - fmin) / 256.0;

  // normalize and create a new frequency image
  for (int c = 0; c < image.Width(); c++)
  {
      for (int r = 0; r < image.Height(); r++)
      {
        image[c][r].SetIntensity((frequencies[c][r] - fmin) * scalar);
      }
  }

  return false;
}

/***************************************************************************//**
 * Menu_Filters_WienerFilter
 * Author - Derek Stotz
 *
 *
 *
 *
 *
 * Parameters -
 *          image - the image object to manipulate.
 *
 * Returns
 *          true if successful, false if not
 ******************************************************************************/
bool Filters::Menu_Filters_WienerFilter(Image& image)
{
  // TODO
  return false;
}

/***************************************************************************//**
 * Menu_Filters_InverseFilter
 * Author - Derek Stotz
 *
 *
 *
 *
 *
 * Parameters -
 *          image - the image object to manipulate.
 *
 * Returns
 *          true if successful, false if not
 ******************************************************************************/
bool Filters::Menu_Filters_InverseFilter(Image& image)
{
  // TODO
  return false;
}

/***************************************************************************//**
 * Menu_Filters_BandRejectFilter
 * Author - Derek Stotz
 *
 * Applies a band-reject filter on an image. Prompts the user to enter a lower
 * bound between 0 and 1 and an upper bound between 0 and 1. The image will then
 * be run through the Fourier transform and a band reject filter will be applied
 * to it so that all data between the lower and upper bound is erased. The image
 * is then run through the inverse Fourier transform.
 *
 * Parameters -
 *          image - the image object to manipulate.
 *
 * Returns
 *          true if successful, false if not
 ******************************************************************************/
bool Filters::Menu_Filters_BandRejectFilter(Image& image)
{
  // TODO
  return false;
}

