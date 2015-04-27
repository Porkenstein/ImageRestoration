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
#include "Toolbox.h"
#include <QMessageBox>

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
    bool Menu_Transform_FourierTransform(Image& image);
    bool Menu_Transform_InverseFourierTransform(Image& image);
    //bool Menu_Filters_WienerFilter(Image& image);
    bool Menu_Filters_InverseFilter(ImageHnd& hnd, QMouseEvent event);
    bool Menu_Filters_SpecifiedInverseFilter(Image& image);
    bool Menu_BandPass( ImageHnd &hnd, QMouseEvent event );
    bool Menu_BandReject( ImageHnd &hnd, QMouseEvent event );
};

