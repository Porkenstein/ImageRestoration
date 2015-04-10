/***************************************************************************//**
 * Filters.h
 *
 * Author - Derek Stotz
 *
 * Date - April 8, 2015
 *
 * Details - Contains the declaration for the Filters class.
 ******************************************************************************/

#pragma once
#include <qtimagelib.h>
#include "Toolbox.h"

/***************************************************************************//**
 * Filters
 *
 * Author - Derek Stotz
 *
 * Child of QObject class.
 *
 * This class is responsible for applying filters to images using the Fourier
 * transforms, including the ability to view the Fourier transform of an image.
 * All filters defined in this class primarily operate in the frequency domain.
 ******************************************************************************/
class Filters : public QObject
{
  Q_OBJECT;

  public slots:
    bool Menu_Filters_FourierTransform(Image& image);
    bool Menu_Filters_WienerFilter(Image& image);
    bool Menu_Filters_InverseFilter(Image& image);
    bool Menu_Filters_BandRejectFilter(Image& image);
};

