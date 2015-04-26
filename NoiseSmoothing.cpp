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
 * Menu_LowPass
 * Author - Dan Andrus
 *
 * Smooths an image by transforming it into the frequency domain and applying
 * a low-pass filter to the frequency information. Allows to interactivly
 * specify the frequency using the mouse. Asks the user whether to use an ideal
 * or gaussian low-pass filter.
 *
 * Parameters
 *          hnd - The handle of thei mage object to smooth
 *          event - The mouse event object
 *
 * Returns
 *          true if successful, false if not
 ******************************************************************************/
bool NoiseSmoothing::Menu_LowPass( ImageHnd &hnd, QMouseEvent event )
{
    // Static variables for keeping track of stuff across runs
    // Prevents us from using global variables
    static Image T_Image_Original;
    static int T_Mouse_Buttons;
    
    Image copy;    
    
    double radius_div;
    double radius;
    double r;
    float adjustment;
    
    int origin_x;
    int origin_y;
    bool ideal;
    
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
        copy = T_Image_Original;
        
        origin_x = copy.Width() / 2;
        origin_y = copy.Height() / 2;
        
        // Calculate distance from mouse to center of image
        radius = sqrt(
            pow(abs(origin_y - event.pos().y()), 2.0) +
            pow(abs(origin_x - event.pos().x()), 2.0)
        );
        
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
            radius_div = pow(radius, 2.0) * 2.0;
        }
        
        // Apply low-pass filter to data
        for (y = 0; y < copy.Height(); y++)
        {
            for (x = 0; x < copy.Width(); x++)
            {
                r = sqrt(
                    pow(abs((double) origin_y - y), 2.0) +
                    pow(abs((double) origin_x - x), 2.0)
                );
                
                if (ideal)
                {
                    // Zero out data outside of range
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
                else
                {
                    // Apply Gaussian adjustment to imagebased on range
                    adjustment = exp(-pow(r, 2.0) / radius_div); 
                    
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
        
        cout << "Low-pass radius: " << radius << endl;
        
        hnd.CopyImage() = copy;
        T_Mouse_Buttons = event.buttons();
        return true;
    }
    
    T_Mouse_Buttons = event.buttons();
    
    return false;
}

/***************************************************************************//**
 * Menu_PeriodicNoise
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
bool NoiseSmoothing::Menu_AddNoise( ImageHnd &hnd, QMouseEvent event )
{
    // Static variables for keeping track of stuff across runs
    // Prevents us from using global variables
    static Image T_Image_Original;
    static int T_Mouse_Buttons;
    
    static const int preview_radius = 5;
    
    Image copy;
    
    int origin_x;
    int origin_y;
    
    double rad;
    double radius;
    double intensity;
    
    unsigned int x;
    unsigned int y;
    int r;
    int c;
    
    
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
        copy = T_Image_Original;
        
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
        copy = T_Image_Original;
        
        origin_x = copy.Width() / 2;
        origin_y = copy.Height() / 2;
        
        // Calculate distance from mouse to center of image
        radius = 1;
        intensity = 0.5;

        if (!Dialog("Size of Periodic Noise").Add(radius, "Radius").Show()
            || !Dialog("Intensity of Noise").Add(intensity, "Intensity", 0.0, 1.0).Show())
        {
            // Cancel everything
        }
        else
        {
            for (y = (unsigned int) (event.pos().y() - radius - 1);
                y < (unsigned int) (event.pos().y() + radius + 1);
                y++)
            {
                for (x = (unsigned int) (event.pos().x() - radius - 1);
                    x < (unsigned int) (event.pos().x() + radius + 1);
                    x++)
                {
                    rad = sqrt(
                        pow(abs((double) event.pos().y() - y), 2.0) +
                        pow(abs((double) event.pos().x() - x), 2.0)
                    );
                    if (rad <= radius)
                    {
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

