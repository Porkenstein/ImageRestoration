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
    if (!T_Frequency_Set)
    {
        return false;
    }
    
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
bool Filters::Menu_Filters_BandRejectFilter( ImageHnd &hnd, QMouseEvent event )
{
    int origin_x;
    int origin_y;

    // Initial press
    if (event.button() == Qt::LeftButton
      && event.buttons() & Qt::LeftButton
      && !(T_Mouse_Buttons & Qt::LeftButton))
    {
        T_Mouse_X = event.pos().x();
        T_Mouse_Y = event.pos().y();

        // Store original image
        T_Image_Original = hnd.CopyImage();
    }

    // Drag (and initial press)
    if ((event.button() == Qt::NoButton && event.buttons() & Qt::LeftButton)
        || (event.button() == Qt::LeftButton
            && event.buttons() & Qt::LeftButton
            && !(T_Mouse_Buttons & Qt::LeftButton)))
    {
        // Draw circle on original image
        Image copy = T_Image_Original;

        double radius = sqrt(pow(T_Mouse_X - event.pos().x(), 2) + pow(T_Mouse_Y - event.pos().y(), 2));
        drawCircle(copy, T_Mouse_X, T_Mouse_Y, radius, 1.0);

        hnd.CopyImage() = copy;
        T_Mouse_Buttons = event.buttons();
        return true;
    }

    // Release
    if (event.button() == Qt::LeftButton
      && !(event.buttons() & Qt::LeftButton)
      && T_Mouse_Buttons & Qt::LeftButton)
    {
        // Draw circle on original image
        Image copy = T_Image_Original;

        origin_x = copy.Width() / 2;
        origin_y = copy.Height() / 2;

        double rad;
        double radius = sqrt(
            pow(T_Mouse_X - event.pos().x(), 2) +
            pow(T_Mouse_Y - event.pos().y(), 2)
        );
        double radius_div = pow(radius, 2.0) * 2.0;
        float adjustment;
        
        // Gaussian removal
        for (unsigned int y = 0; y < copy.Height(); y++)
        {
            for (unsigned int x = 0; x < copy.Width(); x++)
            {
                rad = sqrt(
                    pow(abs((double) (T_Mouse_X - (int) x)), 2.0) +
                    pow(abs((double) (T_Mouse_Y - (int) y)), 2.0)
                );
                
                // Calculate adjustment (inverse of Guassian LPF)
                adjustment = exp(-pow(rad, 2.0) / radius_div);
                adjustment = 1.0 - adjustment;
                
                // Apply adjustment multiplier to frequency data and image
                copy[y][x].SetIntensity(copy[y][x].Intensity() * adjustment);
                
                T_Image_Freal
                    [(y + origin_y) % copy.Height()]
                    [(x + origin_x) % copy.Width()] *= adjustment;
                T_Image_Fimag
                    [(y + origin_y) % copy.Height()]
                    [(x + origin_x) % copy.Width()] *= adjustment;
            }
        }

        hnd.CopyImage() = copy;
        T_Mouse_Buttons = event.buttons();
        return true;
    }

    T_Mouse_Buttons = event.buttons();

    return false;
}

