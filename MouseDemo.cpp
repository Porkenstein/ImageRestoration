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
    if ( event.buttons() & Qt::LeftButton )
    {
        Image &image = hnd.CopyImage();
        image[event.pos().y()][event.pos().x()] = color;
        return true;
    }
    return false;
}
