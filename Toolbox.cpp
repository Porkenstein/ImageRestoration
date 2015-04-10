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
    for (int r = 0; r < image.Height(); r++)
    {
        for (int c = 0; c < image.Height(); c++)
        {
            frequencies[r][c] = 0;
            for (int i = 0; i <image.Width(); i++)
            {
                frequencies[r][c] += image[r][c].Intensity() * pow(M_E, M_I * 2 * M_PI * c * i / image.Width());
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
