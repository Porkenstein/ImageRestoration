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
 * Menu_BandReject
 * Author - Daniel Andrus
 *
 * Allows the user to interactivally apply a band-reject filter on the frequency
 * data of an image. After using the mouse to specify a band range, the user
 * asks whether to use a Gaussian function or an ideal band-reject filter. Then
 * applies either an ideal band-pass filter or a Gaussian band-reject filter
 * based on the user's choise.
 *
 * Parameters
 *          hnd - Handle for the image object to manipulate
 *          event - The mouse event object containing event data
 *
 * Returns
 *          true if successful, false if not
 ******************************************************************************/
bool Filters::Menu_BandReject( ImageHnd &hnd, QMouseEvent event )
{
    // Static variables for keeping track of radii across runs
    // Prevents us from using global variables
    static double lower_bound;
    static double upper_bound;
    static double middl_bound;

    Image copy;
    
    int origin_x;
    int origin_y;
    bool ideal;
    
    double radius;
    double radius_div;
    float adjustment;
    
    unsigned int x;
    unsigned int y;
    
    
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
        
        origin_x = T_Image_Original.Width() / 2;
        origin_y = T_Image_Original.Height() / 2;
        
        // Calculate radius from center of image
        radius = sqrt(
            pow(abs(origin_y - event.pos().y()), 2.0) +
            pow(abs(origin_x - event.pos().x()), 2.0)
        );
        
        // Store lower bound
        lower_bound = radius;
    }

    // Left click drag OR initial press
    if ((event.button() == Qt::NoButton && event.buttons() & Qt::LeftButton)
        || (event.button() == Qt::LeftButton
            && event.buttons() & Qt::LeftButton
            && !(T_Mouse_Buttons & Qt::LeftButton)))
    {
        // Draw circle on stored original image
        copy = T_Image_Original;
        
        origin_x = copy.Width() / 2;
        origin_y = copy.Height() / 2;
        
        // Calculate distance from mouse to center of image
        radius = sqrt(
            pow(abs(origin_y - event.pos().y()), 2.0) +
            pow(abs(origin_x - event.pos().x()), 2.0)
        );
        
        // Draw inverted circles on image
        drawCircle(copy, origin_x, origin_y, radius, 1.0);
        drawCircle(copy, origin_x, origin_y, lower_bound, 1.0);
        
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
        copy = T_Image_Original;
        
        origin_x = copy.Width() / 2;
        origin_y = copy.Height() / 2;
        
        // Calculate distance from mouse to center of image
        radius = sqrt(
            pow(abs(origin_y - event.pos().y()), 2.0) +
            pow(abs(origin_x - event.pos().x()), 2.0)
        );
        
        // Make sure lower_bound is min radius and upper_bound is max radius
        upper_bound = MAX(radius, lower_bound);
        lower_bound = MIN(radius, lower_bound);
        middl_bound = (upper_bound + lower_bound) / 2.0;
        
        // Ask user whether or not to use Gaussian or ideal band-pass filter
        QMessageBox msgBox;
        msgBox.setWindowTitle("Choose mode");
        msgBox.setText("Gaussian or Ideal? (Yes for Gaussian)");
        msgBox.setStandardButtons(QMessageBox::Yes
                                  |QMessageBox::No
                                  |QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Yes);
        
        // Display popup box to user
        int result = msgBox.exec();
        
        // If user cancelled out, cancel everything
        if (result == QMessageBox::Cancel)
        {
            hnd.CopyImage() = T_Image_Original;
            T_Mouse_Buttons = event.buttons();
            return true;
        }
        
        ideal = (result == QMessageBox::No);
        
        // Calculate divisor for gaussian reject filter
        if (!ideal)
        {
            radius_div = pow(upper_bound - lower_bound, 2.0) * 2.0;
        }
        
        // Apply band-reject fitler to image
        for (y = 0; y < copy.Height(); y++)
        {
            for (x = 0; x < copy.Width(); x++)
            {
                radius = sqrt(
                    pow(abs((double) origin_y - y), 2.0) +
                    pow(abs((double) origin_x - x), 2.0)
                );
                
                if (ideal)
                {
                    // Zero out all data within band
                    if (lower_bound <= radius && radius <= upper_bound)
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
                else
                {
                    // exp(-pow(abs(radius - middl_bound), 2.0) / radius_div)
                    adjustment = abs(radius - middl_bound);
                    adjustment *= adjustment;
                    adjustment *= -1;
                    adjustment /= radius_div;
                    adjustment = exp(adjustment);
                    adjustment = 1.0 - adjustment;    // invert gaussian blur
                    
                    // Apply adjustment (using Gaussian function) to data
                    copy[y][x] = copy[y][x] * adjustment;
                        
                    T_Image_Freal
                        [(y + origin_y) % copy.Height()]
                        [(x + origin_x) % copy.Width()] *= adjustment;
                    T_Image_Fimag
                        [(y + origin_y) % copy.Height()]
                        [(x + origin_x) % copy.Width()] *= adjustment;
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
 * Menu_SpotReject
 * Author - Daniel Andrus
 *
 * Allows the user to interactivly remove spots of frequency data from an image.
 * Users can use this to manually remove periodic noise from an image
 *
 * Parameters
 *          hnd - Handle for the image object to manipulate
 *          event - The mouse event object containing event data
 *
 * Returns
 *          True if successful, false if not
 ******************************************************************************/
bool Filters::Menu_SpotReject( ImageHnd &hnd, QMouseEvent event )
{
    Image copy;

    double radius_div;
    double radius;
    double rad;
    float adjustment;

    int origin_x;
    int origin_y;
    
    unsigned int x;
    unsigned int y;
    
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
        copy = T_Image_Original;

        radius = sqrt(
            pow(T_Mouse_X - event.pos().x(), 2) +
            pow(T_Mouse_Y - event.pos().y(), 2)
        );
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
        copy = T_Image_Original;

        origin_x = copy.Width() / 2;
        origin_y = copy.Height() / 2;

        radius = sqrt(
            pow(T_Mouse_X - event.pos().x(), 2) +
            pow(T_Mouse_Y - event.pos().y(), 2)
        );
        radius_div = pow(radius, 2.0) * 2.0;
        
        // Gaussian removal
        for (y = 0; y < copy.Height(); y++)
        {
            for (x = 0; x < copy.Width(); x++)
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

