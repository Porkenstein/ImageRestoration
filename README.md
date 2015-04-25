# Prog3 - Image Restoration

- **Class:** CSC 442, Introduction to Image Processing
- **Professor:** Dr. John Weiss
- **Meeting Time &amp; Place:** 9:00 am M-W-F in McLaury 305
- **Authors:** Derek Stotz, Dan Andrus (QtImageLib by Dr. John Weiss)
- **Last Modified:** Apr 26, 2015
- **Compiling:** Run `qmake` to build makefile, then `make` to compile program.
- **Usage:**   ./prog3

## Details
The purpose of this assignment was to implement many of the image
restoration techniques described in class.  Since the GUI elements
were provided by Qt and QtImageLib, the only code we had to implement
was for modifying the images themselves.  Each function of ours was passed
a QtImageLib image, which acted as a 2 dimensional vector of pixel
information.  We were able to then apply our transformations and filters to
the image data.  Generally, we would use a fourier transform to move the image
into the frequency domain, then filter the information (or add noise) before
transforming it back through a reverse fourier transform.

We split our work roughly in half.  The restoration filters were all put under a single
menu, while the noise and smoothing functions were given their own menu.


## Recommended Usage
Simply open an image using the open icon and modify it using the functions
found under the two menus.  To reset the image, press the back arrow in the
image window.

