// External Includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <stdint.h>
#include <time.h>
#include <math.h>

// Graphics Libraries for viewer
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <cairo.h>
#include <cairo-xlib.h>

#include "../viewer/viewer.h"
#include "../viewer/cairo_functions.h"
#include "../solver/solver.h"

#include "optimiser.h"
#include "../includes.h"

// Function to round float value var
int Round(float var)
{
  double integral;
  float fraction = (float)modf(var, &integral);
 
  if (fraction >= 0.5)
    integral += 1;
  if (fraction <= -0.5)
    integral -= 1;
 
  return (int)integral;
}

// Run Optimiser
int optimiser(double d_o)
{
   cairo_surface_t *sfc;
   cairo_t *ctx;
   int x, y;
   struct timespec ts = {0, 5000000};

   double x0 = 20, y0 = 20, x1 = 200, y1 = 400, x2 = 450, y2 = 100;
   double dx0 = 1, dx1 = 1.5, dx2 = 2;
   double dy0 = 2, dy1 = 1.5, dy2 = 1;
    
    // Mu0
   double mu  = 1.256E-6;
   double d_i  = 0; // Automatic
   double d   = 70E-6; // Communication Distance
   double g   = 1.1; //1.1
   double w   = 2E-6;
   int n   = 16;
   double s = 2E-6;
   double f = 500E6;
   double RL = 100;
   double w_min = 1E-6;
   double w_max = 10E-6;
   double s_min = 0.1E-6;
   double s_max = 10E-6;

   int logging=0;
   int threed = 0;

   int running = 1;

   int z = 0;
   char input_area[50];
   char input_freq[50];
   char input_techfile[100];
   double area = 0.00;

   printf("\n##############################################\n");
   printf("# COIL-3D | Inductive Link Layout Optimiser ##\n");
   printf("##############################################\n");


   if(d_o == 0)
   {
      printf("# Error - Please Enter area\n");
      return 1;
   }

   x = WIDTH;
   y = HEIGHT;
   sfc = cairo_create_x11_surface(&x, &y);
   ctx = cairo_create(sfc);

   cairo_move_to (ctx, 0, 0);

   nanosleep(&ts, NULL);
   printf("# Performing Optimisation ... \n");
   double high_score = 0.00;
   double score = 0.00;
   int optimal_n = 0;
   double optimal_w = 0.0;
   double optimal_s = 0.0;
   FILE *fill_factor_fd = fopen("fill-factor-log.csv", "w");
   if (fill_factor_fd == NULL)
   {
      printf("Error opening Fill Factor file!\n");
      exit(1);
   }
   FILE *width_space_fd = fopen("width-space-log.csv", "w");
   if (width_space_fd == NULL)
   {
      printf("Error opening Width Space file!\n");
      exit(1);
   }

   if(logging)
   {
      fprintf(fill_factor_fd, "Fill Factor, Efficiency\n");
      fprintf(width_space_fd, "Width, Space, Efficiency\n");
   }
   else
   {
      fclose(fill_factor_fd);
      fclose(width_space_fd);
   }




   while (running)
   {
         printf("# Optimising Fill Factor ... \n");
         int j;
         for(j = 1;j<d_o/((w+s)*2);j++)
         {
            n = j;
            cairo_push_group(ctx);
            cairo_set_source_rgb (ctx, 0, 0, 0); 
            cairo_paint (ctx);
            draw_grid(ctx);
            score = solve(d_o, w, s, n, mu, d, f, RL, ctx);
            cairo_pop_group_to_source(ctx);
            cairo_paint(ctx);
            cairo_surface_flush(sfc);
            d_i = d_o - (n * 2 * (w+s));
            if(score > high_score)
            {
               high_score = score;
               optimal_n = j;
            }
            if(logging)
            {
               fprintf(fill_factor_fd, "%.10e,%lf\n", (d_o - d_i)/(d_o + d_i),score);
            }

            usleep(DELAY);
      }
      if(logging)
      {
         fclose(fill_factor_fd);
      }
      d_i = d_o - (optimal_n * 2 * (w + s));
      double fill_factor = (d_o - d_i)/(d_o + d_i);
      printf("# Optimised Fill Factor: %f\n", fill_factor );
      printf("# Optimising Line Width ... \n");

      high_score = 0;

      double width = w_min;
      while((width<((d_o - d_i)/2)) && (width < w_max))
      {
            w = width;
            n = (int)Round(((d_o - d_i)/2)/(w+s));
            //printf("width:%.10e\n",width );
            //printf("n: %d\n", n);
            cairo_push_group(ctx);
            cairo_set_source_rgb (ctx, 0, 0, 0); 
            cairo_paint (ctx);
            draw_grid(ctx);
            score = solve(d_o, w, s, n, mu, d, f, RL, ctx);
            cairo_pop_group_to_source(ctx);
            cairo_paint(ctx);
            cairo_surface_flush(sfc);

            if(score > high_score)
            {
               high_score = score;
               optimal_w = width;
            }

            width = width + W_PRECISION;
            usleep(DELAY);
      }

      printf("# Optimised Track Width: %.10e\n", optimal_w );
      high_score = 0;

      double spacing = s_min;
      while((spacing<(((d_o - d_i)/2)-optimal_w)) && (spacing < s_max))
      {
            s = spacing;
            w = optimal_w;
            n = (int)Round(((d_o - d_i)/2)/(w+s));
            //printf("width:%.10e\n",width );
            //printf("n: %d\n", n);
            cairo_push_group(ctx);
            cairo_set_source_rgb (ctx, 0, 0, 0); 
            cairo_paint (ctx);
            draw_grid(ctx);
            score = solve(d_o, w, s, n, mu, d, f, RL, ctx);
            cairo_pop_group_to_source(ctx);
            cairo_paint(ctx);
            cairo_surface_flush(sfc);

            if(score > high_score)
            {
               high_score = score;
               optimal_s = spacing;
            }

            spacing = spacing + S_PRECISION;
            usleep(DELAY);
      }

      printf("# Optimised Track Spacing: %.10e\n", optimal_s);

      if(logging)
      {
         printf("# Optimising width/spacing ... \n");
         double eta;

         for(width=w_min;width<w_max;width = width + W_PRECISION)
         {
            d_i = d_o * ((1-fill_factor)/(1 + fill_factor));
            for(n=1;n<(d_o - d_i)/(width *2);n++)
            {

               spacing = (((d_o - d_i)/2) - (n*width))/n;
               w = width;
               s = spacing;
               cairo_push_group(ctx);
               cairo_set_source_rgb (ctx, 0, 0, 0); 
               cairo_paint (ctx);
               draw_grid(ctx);
               eta = solve(d_o, w, s, n, mu, d, f, RL, ctx);
               cairo_pop_group_to_source(ctx);
               cairo_paint(ctx);
               cairo_surface_flush(sfc);
               usleep(DELAY/10);
               if((eta>0)&&(eta<1))
               {
                  printf("%lf, %lf, %lf\n",width/SCALE,spacing/SCALE,eta*100);
                  fprintf(width_space_fd, "%lf, %lf, %lf\n",width/SCALE,spacing/SCALE,eta*100);
               }

            }  

         }
      }

      s = optimal_s;
      w = optimal_w;
      n = (int)Round(((d_o - d_i)/2)/(w+s));
      double tab = d_o /50;
      double l = draw_inductor(ctx, n, d_o/SCALE, w/SCALE, s/SCALE, tab/SCALE);

      /* Generate Netlist */
      FILE *netlist_fd = fopen("netlist.net", "w");
      if (fill_factor_fd == NULL)
      {
         printf("Error opening netlist file!\n");
         exit(1);
      }

      fprintf(netlist_fd, "* 4-Port Model\n");
      fprintf(netlist_fd, "* In: IN+ to IN-\n");
      fprintf(netlist_fd, "* Out: OUT+ to OUT-\n\n");
      char cwd[1024];
      if (getcwd(cwd, sizeof(cwd)) != NULL)
          fprintf(netlist_fd, "* %s\n", cwd);
      else
      fprintf(netlist_fd, "/netlist.net\n\n");
      fprintf(netlist_fd, "L1 IN+ IN- %.10eL Rser=%.10e Cpar=%.10e\n",self_inductance(d_o, d_i, n, mu),resistance(f,l,w),capacitance(l,s));
      fprintf(netlist_fd, "L1 OUT- OUT+ %.10eL Rser=%.10e Cpar=%.10e\n",self_inductance(d_o, d_i, n, mu),resistance(f,l,w),capacitance(l,s));
      fprintf(netlist_fd,"K1 L1 L2 %.10e", mutual_inductance(d_o, d, mu, w, s, n)/self_inductance(d_o, d_i, n, mu));
      fprintf(netlist_fd, ".end\n");

      fclose(netlist_fd);

      printf("# Complete!\n# >");



      while(1)
      {
         cairo_push_group(ctx);
         cairo_set_source_rgb (ctx, 0, 0, 0); 
         cairo_paint (ctx);
         draw_grid(ctx);
         score = solve(d_o, w, s, n, mu, d, f, RL, ctx);
         cairo_pop_group_to_source(ctx);
         cairo_paint(ctx);
         cairo_surface_flush(sfc);
      }





      nanosleep(&ts, NULL);
   }

   cairo_destroy(ctx);
   cairo_close_x11_surface(sfc);

   return 0;
}

