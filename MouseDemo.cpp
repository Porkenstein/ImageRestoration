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

// use message box to output RGB color at position of mouse click
// (QPoint is a Qt class)
bool MouseDemo::Menu_ColorPicker( Image &image, QPoint pt )
{
    color = image[pt.y()][pt.x()];
    std::stringstream ss;
    ss  << "Color: (" << ( int )color.Red() << ", " << ( int )color.Green() << ", " << ( int )color.Blue() << ")";
    messageBox( ss.str() );
    return false;
}

// draw point on image at position of mouse click, using chosen color
// (QMouseEvent is a Qt class)
bool MouseDemo::Menu_Draw( ImageHnd &hnd, QMouseEvent event )
{
    if ( event.type() == event.MouseButtonRelease )
    {
        Image &image = hnd.CopyImage();
        image[event.pos().y()][event.pos().x()] = color;
        return true;
    }
    if ( event.type() == event.MouseMove )
    {
        Image &image = hnd.CopyImage();
        
        if (x == -1 || y == -1)
        {
          x = event.pos().x();
          y = event.pos().y();
          image[x][y] = color;
        }
        else
        {
          do
          {
            if (abs(x - event.pos().x()) < abs(y - event.pos().y()))
            {
              if (event.pos().y() < y)
                y--;
              else
                y++;
            }
            else
            {
              if (event.pos().x() < x)
                x--;
              else
                x++;
            }
            image[y][x] = color;
          }
          while (x != event.pos().x() || y != event.pos().y());
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
      && !(buttons & Qt::LeftButton))
  {
    x = event.pos().x();
    y = event.pos().y();
    
    // Store original image
    original = hnd.CopyImage();
  }
  
  // Release
  if (event.button() == Qt::LeftButton
      && !(event.buttons() & Qt::LeftButton)
      && buttons & Qt::LeftButton)
  {
    // Draw circle on original image
    Image copy = original;
    
    double radius = sqrt(pow(x - event.pos().x(), 2) + pow(y - event.pos().y(), 2));
    drawCircle(copy, x, y, radius, 1.0);
    
    hnd.CopyImage() = copy;
    buttons = event.buttons();
    return true;
  }

  // Check for left mouse drag
  if (event.button() == Qt::NoButton && event.buttons() & Qt::LeftButton)
  {
    // Draw circle on original image
    Image copy = original;
    
    double radius = sqrt(pow(x - event.pos().x(), 2) + pow(y - event.pos().y(), 2));
    drawCircle(copy, x, y, radius, 1.0);
    
    hnd.CopyImage() = copy;
    buttons = event.buttons();
    return true;
  }
  
  buttons = event.buttons();
  
  return false;
}


