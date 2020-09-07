/***************************************************/
/* Cairo library functions used for COIL-3D viewer */
/***************************************************/

// Graphics Libraries for viewer
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <cairo.h>
#include <cairo-xlib.h>

#ifndef CAIRO_FUNCTIONS_H 
#define CAIRO_FUNCTIONS_H

int cairo_check_event(cairo_surface_t *sfc, int block);
static void fullscreen(Display* dpy, Window win);
cairo_surface_t *cairo_create_x11_surface(int *x, int *y);
void cairo_close_x11_surface(cairo_surface_t *sfc);

#endif // CAIRO_FUNCTIONS_H
