/*
               ***** MouseDemo.h *****

Demo program to illustrate mouse use in qtImageLib.
Mousdemo shows how to get low-level mouse input.

Author: John M. Weiss, Ph.D.
Written Jan 14, 2013 for SDSM&T CSC464/564 DIP course.

Modifications:
*/

#pragma once
#include "Toolbox.h"

class MouseDemo : public QObject
{
  Q_OBJECT;

  public slots:
    bool Menu_Palette_Negate( Image & );
    bool Menu_Palette_Grayscale( Image & );

    bool Menu_Draw( ImageHnd &hnd, QMouseEvent event );
    bool Menu_Circle( ImageHnd &hnd, QMouseEvent event );

};
