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
#define _USE_MATH_DEFINES
#include <qtimagelib.h>
#include <iostream>
#include <cmath>
#include <string>
#include <qfiledialog.h>
#include <stdlib.h>
#include <cmath>
#include <sstream>

#define MAX(x,y)  ((x)>(y)?(x):(y))
#define MIN(x,y)  ((x)<(y)?(x):(y))

using namespace std;

extern float ** T_Image_Freal;
extern float ** T_Image_Fimag;
extern Image T_Image_Spatial;
extern bool T_Frequency_Set;

void drawCircle(Image& image, int x, int y, int radius, double thickness);

double** alloc2d(int width, int height);
float** alloc2d_f(int width, int height);
void dealloc2d(double** array, int rows);
void dealloc2d_f(float** array, int rows);

