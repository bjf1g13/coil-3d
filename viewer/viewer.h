// Graphical Viewer Libraries
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <cairo.h>
#include <cairo-xlib.h>

#ifndef VIEWER_H
#define VIEWER_H

// Function to draw the grid used as the viewer background
void draw_grid(cairo_t *ctx);
// Function to draw an inductor with parameters n, d_o, w and s
// (Returns axial length as a double prevision floating point
double draw_inductor(cairo_t *ctx, int n, double d_o, double w, double s, double tab);
#endif // VIEWER_H
