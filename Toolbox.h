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
#define _USE_MATH_DEFINES
#include <cmath>

using namespace std;

bool FourierTransform(Image& image, double ** frequencies);
bool InverseFourierTransform(Image& image, double ** frequencies);
