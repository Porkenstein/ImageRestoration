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
 * NoiseSmoothing
 * Author - Derek Stotz
 *
 * Constructs the NoiseSmoothing menu by defining the frequency arrays.
 *
 * Parameters -
 *          freal - the real frequency values
 *          fimag - the imaginary frequency values
 *
 ******************************************************************************/
NoiseSmoothing::NoiseSmoothing(float **freal, float **fimag)
{
    this->Freal = freal;
    this->Fimag = fimag;
}

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
bool NoiseSmoothing::Menu_NoiseAndSmoothing_Smooth(Image& image)
{
    //TODO
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
bool NoiseSmoothing::Menu_NoiseAndSmoothing_PeriodicNoise(Image& image)
{
    //TODO
    return false;
}
