#include "Toolbox.h"

/***************************************************************************//**
 * FourierTransform
 * Author - Derek Stotz, Dan Andrus
 *
 * Transforms a given image from the spatial domain to the frequency domain.
 *
 * Parameters -
            image - the image object to transform.
 *
 * Returns
 *          true if successful, false if not
 ******************************************************************************/
bool FourierTransform(Image& image, double ** frequencies)
{
    double theta;
    
    // Allocate 2D arrays
    double ** real = new double * [image.Height()];
    double ** imag = new double * [image.Height()];
    double ** i_frequencies = new double * [image.Height()];
    for (int i = 0; i < image.Height(); i++)
    {
        real[i] = new double[image.Width()];
        imag[i] = new double[image.Width()];
        i_frequencies = new double[image.Width()];
    }

    // Perform Fourier transform along the rows
    for (int r = 0; r < image.Height(); r++)
    {
        for (int c = 0; c < image.Width(); c++)
        {
            real[r][c] = 0;
            imag[r][c] = 0;
            for (int i = 0; i <image.Width(); i++)
            {
                theta = (2 * M_PI * c * i )/ image.Width();
                real[r][c] += image[r][c].Intensity() * cos(theta);
                imag[r][c] += -1 * image[r][c].Intensity() * sin(theta);
            }
        }
    }

    // Perform Fourier transform along the columns
    for (int r = 0; r < image.Height(); r++)
    {
        for (int c = 0; c < image.Width(); c++)
        {
            frequencies[r][c] = 0;
            i_frequencies[r][c] = 0;
            for (int i = 0; i <image.Height(); i++)
            {
                theta = (2 * M_PI * r * i )/ image.Height();
                frequencies[r][c] += real[r][c] * cos(theta) + imag[r][c] * sin(theta);
                i_frequencies[r][c] += -1 * real[r][c] * sin(theta) + imag[r][c] * cos(theta);
            }
        }
    }
    
    // Deallocate 2D arrays
    for (int i = 0; i < image.Height(); i++)
    {
        delete [] real[i];
        delete [] imag[i];
        delete [] i_frequencies[i];
    }
    delete [] real;
    delete [] imag;
    delete [] i_frequencies;
}

/***************************************************************************//**
 * InverseFourierTransform
 * Author - Derek Stotz, Dan Andrus
 *
 * Transforms a given image from the frequency domain to the spatial domain.
 *
 * Parameters -
            image - the image object to transform.
 *
 * Returns
 *          true if successful, false if not
 ******************************************************************************/
bool InverseFourierTransform(Image& image, double ** frequencies)
{
    double theta;
    
    // Allocate 2D arrays
    double ** real = new double * [image.Height()];
    double ** imag = new double * [image.Height()];
    double ** i_frequencies = new double * [image.Height()];
    for (int i = 0; i < image.Height(); i++)
    {
        real[i] = new double[image.Width()];
        imag[i] = new double[image.Width()];
        i_frequencies = new double[image.Width()];
    }
    
    // Perform inverse Fourier transform along the rows
    for (int r = 0; r < image.Height(); r++)
    {
        for (int c = 0; c < image.Width(); c++)
        {
            real[r][c] = 0;
            imag[r][c] = 0;
            for (int i = 0; i <image.Width(); i++)
            {
                theta = (2 * M_PI * c * i )/ image.Width();
                real[r][c] += -1 * image[r][c].Intensity() * cos(theta);
                imag[r][c] += image[r][c].Intensity() * sin(theta);
            }
            real[r][c] /= image.Width();
            imag[r][c] /= image.Width();
        }
    }

    // Perform inverse Fourier transform along the columns
    for (int r = 0; r < image.Height(); r++)
    {
        for (int c = 0; c < image.Width(); c++)
        {
            frequencies[r][c] = 0;
            i_frequencies[r][c] = 0;
            for (int i = 0; i <image.Height(); i++)
            {
                theta = (2 * M_PI * r * i )/ image.Height();
                frequencies[r][c] += -1 * real[r][c] * cos(theta) + imag[r][c] * sin(theta);
                i_frequencies[r][c] += real[r][c] * sin(theta) + imag[r][c] * cos(theta);
            }
            frequencies[r][c] /= image.Height();
            i_frequencies[r][c] /= image.Height();
        }
    }
    
    // Deallocate 2D arrays
    for (int i = 0; i < image.Height(); i++)
    {
        delete [] real[i];
        delete [] imag[i];
        delete [] i_frequencies[i];
    }
    delete [] real;
    delete [] imag;
    delete [] i_frequencies;
}

