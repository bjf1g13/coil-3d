/*
 * COIL-3D Optimiser Loop
 * Ben Fletcher (bjf1g13@soton.ac.uk)
 *
 * Copyright (c) 2020, Arm-ECS Research Centre (arm.ecs.soton.ac.uk) 
 */

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
// Run Main Optimiser Loop
int optimiser(double * cfg_opts, int verbose, char * output_dir);

#endif // OPTIMISER_H
