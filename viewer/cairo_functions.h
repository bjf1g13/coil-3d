/***************************************************/
/* Cairo library functions used for COIL-3D viewer */
/***************************************************/

#ifndef CAIRO_H 
#define CAIRO_H

int cairo_check_event(cairo_surface_t *sfc, int block);
static void fullscreen(Display* dpy, Window win);
cairo_surface_t *cairo_create_x11_surface(int *x, int *y);
void cairo_close_x11_surface(cairo_surface_t *sfc);

#endif // CAIRO_H
