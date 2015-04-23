/*
               ***** mousedemo.cpp *****

Demo program to illustrate qtImageLib.
Mousdemo shows how to get low-level mouse input.

Author: John M. Weiss, Ph.D.
Written Jan 14, 2013 for SDSM&T CSC464/564 DIP course.

Modifications:
*/

#include "MouseDemo.h"

// ------------------------------- menu bar additions ------------------------------------

// the following methods add actions to the menu bar
// note the prototype and naming convention: Menu_Menubar_Menuitem

// ------------------------------- palette manipulations ---------------------------------

// negate the image
bool MouseDemo::Menu_Palette_Negate( Image &image )
{
    negatePalette( image );
    return true;
}

// convert a color image to grayscale
bool MouseDemo::Menu_Palette_Grayscale( Image &image )
{
    grayscale( image );
    return true;
}

// ------------------------------- child window toolbar additions -------------------------

// draw point on image at position of mouse click, using chosen color
// (QMouseEvent is a Qt class)
bool MouseDemo::Menu_Draw( ImageHnd &hnd, QMouseEvent event )
{
    if ( event.type() == event.MouseButtonRelease )
    {
        Image &image = hnd.CopyImage();
        image[event.pos().y()][event.pos().x()] = 255;
        return true;
    }
    if ( event.type() == event.MouseMove )
    {
        Image &image = hnd.CopyImage();
        
        if (Mouse_X == -1 || Mouse_Y == -1)
        {
          Mouse_X = event.pos().x();
          Mouse_Y = event.pos().y();
          image[Mouse_X][Mouse_Y] = 255;
        }
        else
        {
          do
          {
            if (abs(Mouse_Y - event.pos().x()) < abs(Mouse_Y - event.pos().y()))
            {
              if (event.pos().y() < Mouse_Y)
                Mouse_Y--;
              else
                Mouse_Y++;
            }
            else
            {
              if (event.pos().x() < Mouse_X)
                Mouse_X--;
              else
                Mouse_X++;
            }
            image[Mouse_Y][Mouse_X] = 255;
          }
          while (Mouse_X != event.pos().x() || Mouse_Y != event.pos().y());
        }
        
        return true;
    }
    return false;
}

bool MouseDemo::Menu_Circle( ImageHnd &hnd, QMouseEvent event )
{
  // Initial press
  if (event.button() == Qt::LeftButton
      && event.buttons() & Qt::LeftButton
      && !(Mouse_Buttons & Qt::LeftButton))
  {
    Mouse_X = event.pos().x();
    Mouse_Y = event.pos().y();
    
    // Store original image
    Image_Original = hnd.CopyImage();
  }
  
  // Release
  if (event.button() == Qt::LeftButton
      && !(event.buttons() & Qt::LeftButton)
      && Mouse_Buttons & Qt::LeftButton)
  {
    // Draw circle on original image
    Image copy = Image_Original;
    
    double radius = sqrt(pow(Mouse_X - event.pos().x(), 2) + pow(Mouse_Y - event.pos().y(), 2));
    drawCircle(copy, Mouse_X, Mouse_Y, radius, 1.0);
    
    hnd.CopyImage() = copy;
    Mouse_Buttons = event.buttons();
    return true;
  }

  // Check for left mouse drag
  if (event.button() == Qt::NoButton && event.buttons() & Qt::LeftButton)
  {
    // Draw circle on original image
    Image copy = Image_Original;
    
    double radius = sqrt(pow(Mouse_X - event.pos().x(), 2) + pow(Mouse_Y - event.pos().y(), 2));
    drawCircle(copy, Mouse_X, Mouse_Y, radius, 1.0);
    
    hnd.CopyImage() = copy;
    Mouse_Buttons = event.buttons();
    return true;
  }
  
  Mouse_Buttons = event.buttons();
  
  return false;
}


