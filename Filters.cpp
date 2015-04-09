/***************************************************************************//**
 * DerekProcessor.cpp
 *
 * Author - Derek Stotz
 *
 * Date - January 30, 2015
 *
 * Point process functions implemented by Derek Stotz.  These functions are
 * named for easy addition into the image app created in prog1.cpp.
 ******************************************************************************/

#include "DerekProcessor.h"
#include <cmath>
#include <string>
#include <qfiledialog.h>
#include <stdlib.h>

bool Filters::Menu_Filters_WienerFilter(Image& image);
bool Filters::Menu_Filters_InverseFilter(Image& image);
bool Filters::Menu_Filters_BandRejectFilter(Image& image);
