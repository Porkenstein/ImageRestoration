/***************************************************************************//**
 * Toolbox.h
 *
 * Author - Daniel Andrus, Derek Stotz
 *
 * Date - April 8, 2015
 *
 * Details - Contains various functions which can be used by the filter
 *           and NoiseSmoothing menus.
 ******************************************************************************/

#pragma once
#include <qtimagelib.h>
#include <iostream>

bool FourierTransform(Image& image);
bool InverseFourierTransform(Image& image);
