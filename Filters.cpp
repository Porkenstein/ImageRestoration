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

extern float ** Image_Freal;
extern float ** Image_Fimag;
extern Image Image_Spatial;
extern int Mouse_X;
extern int Mouse_Y;
extern int Mouse_Buttons;
extern Image Image_Original;

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
    // allocate memory for the frequency information and store the original RBG image
    Image_Freal = alloc2d_f(image.Width(), image.Height());
    Image_Fimag = alloc2d_f(image.Width(), image.Height());
    Image_Spatial = image;

    for (unsigned int r = 0; r < image.Height(); r++ )
    {
        for (unsigned int c = 0; c < image.Width(); c++)
        {
            Image_Freal[r][c] = image[r][c].Intensity();
            Image_Fimag[r][c] = 0;
        }
    }

    fft2D(1, image.Height(), image.Width(), Image_Freal, Image_Fimag);

  return dftMagnitude(image);
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
    fft2D(-1, image.Height(), image.Width(), Image_Freal, Image_Fimag);

    for (unsigned int r = 0; r < image.Height(); r++ )
    {
        for (unsigned int c = 0; c < image.Width(); c++)
        {
            Image_Spatial[r][c].SetIntensity(Image_Freal[r][c]);
        }
    }
    dealloc2d_f(Image_Freal, image.Height());
    dealloc2d_f(Image_Fimag, image.Height());
    image = Image_Spatial;

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

