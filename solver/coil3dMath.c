/*
 * COIL-3D Math Functions
 * Ben Fletcher (bjf1g13@soton.ac.uk)
 *
 * Copyright (c) 2020, Arm-ECS Research Centre (arm.ecs.soton.ac.uk) 
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include <unistd.h>

#include "coil3dMath.h"

// Function to compute complete eliptic integral of the first kind
double Complete_Elliptic_Integral_First_Kind(char arg, double x)
{
   long double k;          // modulus 
   long double m;          // parameter 
   long double a;          // average
   long double g;          // geometric mean
   long double a_old;      // previous average
   long double g_old;      // previous geometric mean

   if ( x == 0.0 ) return M_PI_2;

   switch (arg) {
      case 'k': k = fabsl((long double) x);
                m = k * k;
                break;
      case 'm': m = (long double) x;
                k = sqrtl(fabsl(m));
                break;
      case 'a': k = sinl((long double)x);
                m = k * k;
                break;
      default:  k = fabsl((long double) x);
                m = k * k;
   }

   if ( m == 1.0 ) return DBL_MAX;

   a = 1.0L;
   g = sqrtl(1.0L - m);
   while (1) {
      g_old = g;
      a_old = a;
      a = 0.5L * (g_old + a_old);
      g = sqrtl(g_old * a_old);
      if ( fabsl(a_old - g_old) <= (a_old * LDBL_EPSILON) ) break;
   }
   return (double) (PI_2 / g); 
}

// Function to compute complete eliptic integral of the first kind
double Complete_Elliptic_Integral_Second_Kind(char arg, double x)
{
   long double k;      // modulus
   long double m;      // the parameter of the elliptic function m = modulus^2
   long double a;      // arithmetic mean
   long double g;      // geometric mean
   long double a_old;  // previous arithmetic mean
   long double g_old;  // previous geometric mean
   long double two_n;  // power of 2
   long double Ek;

   if ( x == 0.0 ) return M_PI_2;

   switch (arg) {
      case 'k': k = fabsl((long double) x);
                m = k * k;
                break;
      case 'm': m = (long double) x;
                k = sqrtl(fabsl(m));
                break;
      case 'a': k = sinl((long double)x);
                m = k * k;
                break;
      default:  k = fabsl((long double) x);
                m = k * k;
   }

   if ( m == 1.0 ) return 1.0;

   a = 1.0L;
   g = sqrtl(1.0L - m);
   two_n = 1.0L;
   Ek = 2.0L - m;

   while (1) {
      g_old = g;
      a_old = a;
      a = 0.5L * (g_old + a_old);
      g = a_old * g_old;
      two_n += two_n;
      Ek -= two_n * (a * a - g);
      if ( fabsl(a_old - g_old) <= (a_old * LDBL_EPSILON) ) break;
      g = sqrtl(g);
   }

   return (double) ((PI_4 / a) * Ek); 
}

