// Graphics Libraries for viewer
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <cairo.h>
#include <cairo-xlib.h>

#ifndef OPTIMISER_H
#define OPTIMISER_H

#include "../viewer/viewer.h"
#include "../solver/solver.h"

// Function to round float value var
int Round(float var);
int optimiser();

#endif // OPTIMISER_H
