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
    bool Menu_NoiseAndSmoothing_Smooth(Image& image);
    bool Menu_NoiseAndSmoothing_PeriodicNoise(Image& image);
};

