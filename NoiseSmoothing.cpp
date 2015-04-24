/***************************************************************************//**
 * NoiseSmoothing.cpp
 *
 * Author - Dan Andrus
 *
 * Date - April 8, 2015
 *
 * Details - Defines some functions which modify image frequency information in
 * order to smooth the image through a low-pass filter or add periodic noise through
 * the addition of a sinewave.
 *
 ******************************************************************************/

#include "NoiseSmoothing.h"

/***************************************************************************//**
 * Menu_NoiseAndSmoothing_Smooth
 * Author - Dan Andrus
 *
 * Smooths an image by transforming it into the frequency domain and applying
 * a low-pass filter to the frequency information.  Asks the user for the
 * frequency to filter at.
 *
 * Parameters - 
            image - the image object to smooth.
 *
 * Returns
 *          true if successful, false if not
 ******************************************************************************/
bool NoiseSmoothing::Menu_NoiseAndSmoothing_Smooth( ImageHnd &hnd, QMouseEvent event )
{
    int origin_x;
    int origin_y;
    
    // Only work with Fourier transformed images
    if (!T_Frequency_Set)
    {
      return false;
    }
    
    // Initial press
    if (event.button() == Qt::LeftButton
        && event.buttons() & Qt::LeftButton
        && !(T_Mouse_Buttons & Qt::LeftButton))
    {
        // Store original image
        T_Image_Original = hnd.CopyImage();
    }

    // Left click drag OR initial press
    if ((event.button() == Qt::NoButton && event.buttons() & Qt::LeftButton)
        || (event.button() == Qt::LeftButton
            && event.buttons() & Qt::LeftButton
            && !(T_Mouse_Buttons & Qt::LeftButton)))
    {
        // Draw circle on stored original image
        Image copy = T_Image_Original;
        
        origin_x = copy.Width() / 2;
        origin_y = copy.Height() / 2;
        
        // Calculate distance from mouse to center of image
        double radius = sqrt(
            pow(abs(origin_y - event.pos().y()), 2.0) +
            pow(abs(origin_x - event.pos().x()), 2.0)
        );
        
        // Draw an inverted circle on the image
        drawCircle(copy, origin_x, origin_y, radius, 1.0);
        
        // Apply changes to our main image
        hnd.CopyImage() = copy;
        T_Mouse_Buttons = event.buttons();
        return true;
    }
    
    // Release
    if (event.button() == Qt::LeftButton
        && !(event.buttons() & Qt::LeftButton)
        && T_Mouse_Buttons & Qt::LeftButton)
    {
        // Work with copy of original image
        Image copy = T_Image_Original;
        
        origin_x = copy.Width() / 2;
        origin_y = copy.Height() / 2;
        
        // Calculate distance from mouse to center of image
        double r;
        double radius = sqrt(
            pow(abs(origin_y - event.pos().y()), 2.0) +
            pow(abs(origin_x - event.pos().x()), 2.0)
        );
        
        // Zero out any pixel outside of radius
        for (unsigned int y = 0; y < copy.Height(); y++)
        {
            for (unsigned int x = 0; x < copy.Width(); x++)
            {
                r = sqrt(
                    pow(abs((double) origin_y - y), 2.0) +
                    pow(abs((double) origin_x - x), 2.0)
                );
                
                if (r > radius)
                {
                    copy[y][x].SetIntensity(0);
                    
                    T_Image_Freal[(y + origin_y) % copy.Height()][(x + origin_x) % copy.Width()] = 0;
                    T_Image_Fimag[(y + origin_y) % copy.Height()][(x + origin_x) % copy.Width()] = 0;
                }
            }
        }
        
        hnd.CopyImage() = copy;
        T_Mouse_Buttons = event.buttons();
        return true;
    }
    
    T_Mouse_Buttons = event.buttons();
    
    return false;
}

/***************************************************************************//**
 * Menu_NoiseAndSmoothing_PeriodicNoise
 * Author - Dan Andrus
 *
 * Adds periodic noise (a sinewave) to an image in the frequency domain.
 * Asks user for the magnitude and wavelength of the sinewave.
 *
 * Parameters -
            image - the image object to manipulate.
 *
 * Returns
 *          true if successful, false if not
 ******************************************************************************/
bool NoiseSmoothing::Menu_NoiseAndSmoothing_PeriodicNoise( ImageHnd &hnd, QMouseEvent event )
{
  //TODO
  return false;
}

