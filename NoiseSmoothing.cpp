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
 * an ideal low-pass filter to the frequency information.  Asks the user for the
 * frequency to filter at.
 *
 * Parameters - 
 *          hnd - The handle of thei mage object to smooth
 *          event - The mouse event object
 *
 * Returns
 *          true if successful, false if not
 ******************************************************************************/
bool NoiseSmoothing::Menu_NoiseAndSmoothing_IdealLPF( ImageHnd &hnd, QMouseEvent event )
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
                    
                    T_Image_Freal
                        [(y + origin_y) % copy.Height()]
                        [(x + origin_x) % copy.Width()] = 0;
                    T_Image_Fimag
                        [(y + origin_y) % copy.Height()]
                        [(x + origin_x) % copy.Width()] = 0;
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
 * Menu_NoiseAndSmoothing_Smooth
 * Author - Dan Andrus
 *
 * Smooths an image by transforming it into the frequency domain and applying
 * a Gaussian low-pass filter to the frequency information.  Asks the user for
 * the frequency to filter at.
 *
 * Parameters - 
 *          hnd - The handle of thei mage object to smooth
 *          event - The mouse event object
 *
 * Returns
 *          true if successful, false if not
 ******************************************************************************/
bool NoiseSmoothing::Menu_NoiseAndSmoothing_GaussianLPF( ImageHnd &hnd, QMouseEvent event )
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
        double radius_div = pow(radius, 2.0) * 2.0;
        float adjustment;
        
        // Zero out any pixel outside of radius
        for (unsigned int y = 0; y < copy.Height(); y++)
        {
            for (unsigned int x = 0; x < copy.Width(); x++)
            {
                r = sqrt(
                    pow(abs((double) origin_y - y), 2.0) +
                    pow(abs((double) origin_x - x), 2.0)
                );
                
                adjustment = exp(-pow(r, 2.0) / radius_div); 
                
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
bool NoiseSmoothing::Menu_NoiseAndSmoothing_AddNoise( ImageHnd &hnd, QMouseEvent event )
{
    static const int preview_radius = 5;
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
        
        // Draw an inverted circle on the image under mouse
        //   and mirror circle across center of image
        drawCircle(copy,
            event.pos().x(),
            event.pos().y(),
            preview_radius, 1.0
        );
        drawCircle(copy,
            copy.Width() - event.pos().x(),
            copy.Height() - event.pos().y(),
            preview_radius, 1.0
        );
        
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
        double rad;
        double radius = 1;
        double intensity = 0.5;

        if (!Dialog("Size of Periodic Noise").Add(radius, "Radius").Show()
            || !Dialog("Intensity of Noise").Add(intensity, "Intensity", 0.0, 1.0).Show())
        {
            // Cancel everything
        }
        else
        {
            for (unsigned int y = (unsigned int) (event.pos().y() - radius - 1);
                y < (unsigned int) (event.pos().y() + (int) radius + 1);
                y++)
            {
                for (unsigned int x = (unsigned int) (event.pos().x() - radius - 1);
                    x < (unsigned int) (event.pos().x() + (int) radius + 1);
                    x++)
                {
                    rad = sqrt(
                        pow(abs((double) event.pos().y() - y), 2.0) +
                        pow(abs((double) event.pos().x() - x), 2.0)
                    );
                    if (rad <= radius)
                    {
                        int r,c;
                        
                        r = y;
                        c = x;
                    
                        // Apply linear gardient
                        copy[r][c].SetIntensity(
                            MAX((255*intensity)*(1.0-rad/radius), copy[r][c].Intensity()));
                        
                        // Cooresponding place in frequency data
                        r = (r + origin_y) % copy.Height();
                        c = (c + origin_x) % copy.Width();
                        
                        // Apply linear gradient
                        T_Image_Freal[r][c] = MAX((65535.0*intensity)*(1.0-rad/radius),
                                                  T_Image_Freal[r][c]);
                        T_Image_Fimag[r][c] = MAX((65535.0*intensity)*(1.0-rad/radius),
                                                  T_Image_Freal[r][c]);
                        
                        // Mirror across center of image
                        r = copy.Height() - y;
                        c = copy.Width() - x;
                    
                        // Apply linear gardient
                        copy[r][c].SetIntensity(
                            MAX((255*intensity)*(1.0-rad/radius), copy[r][c].Intensity()));
                        
                        // Cooresponding place in frequency data
                        r = (r + origin_y) % copy.Height();
                        c = (c + origin_x) % copy.Width();
                        
                        // Apply linear gradient
                        T_Image_Freal[r][c] = MAX((65535.0*intensity)*(1.0-rad/radius),
                                                  T_Image_Freal[r][c]);
                        T_Image_Fimag[r][c] = MAX((65535.0*intensity)*(1.0-rad/radius),
                                                  T_Image_Freal[r][c]);
                    }
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

