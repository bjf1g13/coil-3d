// External Includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <stdint.h>
#include <math.h>

// Graphics Libraries for viewer
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <cairo.h>
#include <cairo-xlib.h>

#include "viewer.h"
#include "viewer_defines.h"

// Function to draw the grid used as the viewer background
void draw_grid(cairo_t *ctx)
{
   int x, y = 0;
   cairo_set_source_rgb (ctx, 0.9, 0.9, 0.9);
   for(x = 0; x<= WIDTH;x+=10)
   {
      for(y=0;y<= HEIGHT;y+=10)
      {
         cairo_arc (ctx, x, y, 0.5, 0, 2*M_PI);
         cairo_fill(ctx);
      }
   }
}

// Function to draw an inductor with parameters n, d_o, w and s
// (Returns axial length as a double prevision floating point
double draw_inductor(cairo_t *ctx, int n, double d_o, double w, double s, double tab) 
{

   double orig_x = (WIDTH-d_o)/2;
   double orig_y = (HEIGHT-d_o)/2;

   cairo_set_line_width (ctx, w);
   cairo_set_source_rgb (ctx, 1, 0.2, 0.2);
   cairo_set_line_cap  (ctx, CAIRO_LINE_CAP_ROUND);
   int i = 0;
   
   double x_pos = orig_x;
   double y_pos = orig_y+d_o;
   double side_length = d_o;
   double l = 0.0;

   for(i = 0; i < n; i ++)
   {
      if(i==0)
      {
         cairo_move_to (ctx, x_pos+(side_length/2), y_pos); cairo_line_to (ctx, x_pos+side_length, y_pos);
         l+=side_length/2;
      }
      else
      {
         cairo_move_to (ctx, x_pos, y_pos); cairo_line_to (ctx, x_pos+side_length, y_pos);
         l+=side_length;
      }

      cairo_stroke (ctx);
      cairo_move_to (ctx, x_pos+side_length, y_pos); cairo_line_to (ctx, x_pos+side_length, y_pos - side_length);
      l+= side_length;
      cairo_stroke (ctx);
      x_pos = x_pos + side_length;
      y_pos = y_pos - side_length;
      side_length = side_length - (w + s);
      cairo_move_to (ctx, x_pos, y_pos); cairo_line_to (ctx, x_pos-side_length, y_pos);
      l+= side_length;
      cairo_stroke (ctx);
      cairo_move_to (ctx, x_pos-side_length, y_pos); cairo_line_to (ctx, x_pos-side_length, y_pos + side_length);
      l+= side_length;
      cairo_stroke (ctx);
      x_pos = x_pos - side_length;
      y_pos = y_pos + side_length;
      side_length = side_length - (w +s);

   }
   cairo_move_to (ctx, x_pos, y_pos); cairo_line_to (ctx, x_pos+(side_length/2), y_pos);
   cairo_stroke (ctx);

   /* Add Link Layer */
   cairo_set_source_rgb (ctx, 0.2, 0.2, 1);
   cairo_move_to (ctx, x_pos+(side_length/2), y_pos); cairo_line_to (ctx, x_pos+(side_length/2), orig_y+d_o+tab);
   cairo_stroke (ctx);

   cairo_set_source_rgb (ctx, 1, 0.2, 0.2);
   cairo_move_to (ctx, x_pos+(side_length/2), orig_y+d_o); cairo_line_to (ctx, x_pos+(side_length/2), orig_y+d_o+tab);
   l+=tab;
   cairo_stroke (ctx);
   return l*SCALE;

}
