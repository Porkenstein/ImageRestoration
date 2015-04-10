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
bool FourierTransform(Image& image, int ** frequencies)
{
    int ** real = new int[image.Height()][image.Width()];
    int ** imag = new int[image.Height()][image.Width()];
    double theta;

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

    int ** i_frequencies = new int[image.Height()][image.Width()];

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
bool InverseFourierTransform(Image& image, int ** frequencies)
{

}
