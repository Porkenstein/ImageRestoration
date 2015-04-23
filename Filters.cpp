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
bool Filters::Menu_Transform_FourierTransform(Image& image)
{
    // Make sure we don't already have an image's frequency info stored
    if (T_Frequency_Set)
    {
        // TODO Display images
        return false;
    }

    // Allocate arrays for frequency information and store original image
    T_Image_Freal = alloc2d_f(image.Width(), image.Height());
    T_Image_Fimag = alloc2d_f(image.Width(), image.Height());
    T_Image_Spatial = image;

    // Fill dynamically allocated arrays
    for (unsigned int r = 0; r < image.Height(); r++ )
    {
        for (unsigned int c = 0; c < image.Width(); c++)
        {
            T_Image_Freal[r][c] = image[r][c].Intensity();
            T_Image_Fimag[r][c] = 0;
        }
    }

    // Run fast Fourier transform
    fft2D(1, image.Height(), image.Width(), T_Image_Freal, T_Image_Fimag);

    // Attempt to display frequency info
    if (dftMagnitude(image))
    {
        T_Frequency_Set = true;
        return true;
    }
    else
    {
        return false;
    }
}

/***************************************************************************//**
 * Menu_Filters_InverseFourierTransform
 * Author - Derek Stotz
 *
 * Applies the changes to the frequency information and recreates the original image.
 *
 * Parameters -
 *          image - the image object to manipulate.
 *
 * Returns
 *          true if successful, false if not
 ******************************************************************************/
bool Filters::Menu_Transform_InverseFourierTransform(Image& image)
{
    // Run inverse fourier transform on frequency data
    fft2D(-1, image.Height(), image.Width(), T_Image_Freal, T_Image_Fimag);

    // Apply new intensity data to original image
    for (unsigned int r = 0; r < image.Height(); r++ )
    {
        for (unsigned int c = 0; c < image.Width(); c++)
        {
            T_Image_Spatial[r][c].SetIntensity(T_Image_Freal[r][c]);
        }
    }
    
    // Deallocate arrays
    dealloc2d_f(T_Image_Freal, image.Height());
    dealloc2d_f(T_Image_Fimag, image.Height());
    
    // Display modified orignal image
    image = T_Image_Spatial;
    T_Frequency_Set = false;

    return true;
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
  image.Height();
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
  image.Height();
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
bool Filters::Menu_Filters_BandRejectFilter(Image& image, QPoint pt)
{
  // TODO
  image.Height();
  pt.x();
  return true;
}

