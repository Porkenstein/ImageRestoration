#include <qtimagelib.h>
#include "DanProcessor.h"
#include "DerekProcessor.h"
#include "ImageLibProcessor.h"

/*****************************************************************

Program: Prog1 (Point Processes)

Class: CSC 442, Introduction to Image Processing

Professor: Dr. John Weiss

Meeting Time & Place: 9:00 am M-W-F in McLaury 305

Authors: Derek Stotz, Dan Andrus (QtImageLib by Dr. John Weiss)

Last Modified: Feb 6, 2015

Compiling: Run qmake to compile.

Usage:   ./prog1

Details - The purpose of this assignment was to implement most of the point
 *         processess described in class.  While the GUI elements were provided
 *         by Qt and QtImageLib, the only code we had to implement was for
 *         modifying the images themselves.  Each function of ours was passed
 *         a QtImageLib image, which acted as a 2 dimensional vector of pixel
 *         information.  We were able to either generate and analyze the image
 *         histogram to modify the intensity distributions or simply modify each
 *         individual pixel value to implement the point proccesses required.

 *         We split our work roughly in half.  Dan's processes are under a different
 *         Menu than Derek's, and the ImageLib functions which we used for reference
 *         remain under and Image Lib menu.


 Recommended Usage -
 *           Simply open an image using the open icon and modify it using the functions
 *           found under the two menus.  To reset the image, press the back arrow in the
 *           image window.

 Issues and Bugs - Our histogram equalization gives slightly (barely detectably) different
 *           results from the ImageLib equalization.  Actually, in some cases of clipping
 *           values between 0 and 1, our equalization worked correctly while the ImageLib
 *           equalization only gave partially equalized results.
 *
 *           Some processes are not included in the ImageLibProcessor, but this was not
 *           required and we were merely using it for reference.
 *****************************************************************/

/***************************************************************************//**
 * main
 * Author - Derek Stotz, Dan Andrus
 *
 * Sets up the image app, adds the menu classes and starts the GUI.
 *
 * Parameters -
            argc - the number of command line arguments (unused)
            argv - the command line arguments (unused)
 *
 * Returns
 *          an error code returned by app.Start();
 ******************************************************************************/
int main(int argc, char** argv)
{
  DanProcessor dan;
  DerekProcessor derek;
  ImageLibProcessor ilp;
  ImageApp app(argc, argv);

  app.AddActions(&dan);
  app.AddActions(&derek);
  app.AddActions(&ilp);
  return app.Start();
}

