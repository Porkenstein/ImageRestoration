/***************************************************************************//**
 * NoiseSmoothing.h
 *
 * Author - Daniel Andrus
 *
 * Date - April 8, 2015
 *
 * Details - Contains the declaration for the NoiseSmoothing class.
 ******************************************************************************/

#pragma once
#include "Toolbox.h"
#include <QMessageBox>

/***************************************************************************//**
 * NoiseSmoothing
 *
 * Author - Daniel Andrus
 *
 * Child of QObject class.
 *
 * This class is responsible for generating different types of noise and
 * applying smoothing filters to images.
 ******************************************************************************/
class NoiseSmoothing : public QObject
{
  Q_OBJECT;

  public slots:
    bool Menu_LowPass( ImageHnd &hnd, QMouseEvent event );
    bool Menu_SpecifiedLowPass( Image &image);
    bool Menu_AddNoise( ImageHnd &hnd, QMouseEvent event );
    bool Menu_Noise_GaussianNoise(Image &image);
};

