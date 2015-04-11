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
#define _USE_MATH_DEFINES
#include <qtimagelib.h>
#include <iostream>
#include <cmath>
#include <string>
#include <qfiledialog.h>
#include <stdlib.h>
#include <cmath>
#include <sstream>

using namespace std;

void FourierTransform(Image& image, double ** frequencies);
void InverseFourierTransform(Image& image, double ** frequencies);
