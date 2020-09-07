/********************************************/
/* Math functions for use in COIL-3D Solver */
/********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include <unistd.h>

#ifndef COIL3D_MATH_H
#define COIL3D_MATH_H

// Define Pi/2 constant
static const long double PI_2 =  1.5707963267948966192313216916397514L; // pi/2

// Define Pi/4 contsant
static const long double PI_4 = 0.7853981633974483096156608458198757L; // pi/4

// Function to compute complete eliptic integral of the first kind
double Complete_Elliptic_Integral_First_Kind(char, double);

// Function to compute complete eliptic integral of the first kind
double Complete_Elliptic_Integral_Second_Kind(char, double);

#endif // COIL3D_MATH_H
