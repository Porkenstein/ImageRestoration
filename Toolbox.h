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

using namespace std;

extern float ** Image_Freal;
extern float ** Image_Fimag;
extern Image Image_Spatial;
extern int Mouse_X;
extern int Mouse_Y;
extern int Mouse_Buttons;
extern Image Image_Original;

void drawCircle(Image& image, int x, int y, int radius, double thickness);

double** alloc2d(int width, int height);
float** alloc2d_f(int width, int height);
void dealloc2d(double** array, int rows);
void dealloc2d_f(float** array, int rows);

