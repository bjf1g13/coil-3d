/*
 * COIL-3D Optimiser Loop
 * Ben Fletcher (bjf1g13@soton.ac.uk)
 *
 * Copyright (c) 2020, Arm-ECS Research Centre (arm.ecs.soton.ac.uk) 
 */

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
#include "../viewer/viewer_defines.h"
#include "../solver/solver_defines.h"

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
int optimiser(double * cfg_opts, int verbose, char * output_dir)
{
   cairo_surface_t *sfc;
   cairo_t *ctx;
   int x, y;
   struct timespec ts = {0, 5000000};

   double x0 = 20, y0 = 20, x1 = 200, y1 = 400, x2 = 450, y2 = 100;
   double dx0 = 1, dx1 = 1.5, dx2 = 2;
   double dy0 = 2, dy1 = 1.5, dy2 = 1;

   // Setup Parameters from configuration file
    
   double d_o   = cfg_opts[0];
   double mu    = cfg_opts[1];
   double d_i   = cfg_opts[2]; 
   double d     = cfg_opts[3]; 
   double g     = PARAM_G; 
   double w     = DEF_W;
   int n        = DEF_N;
   double s     = DEF_S;
   double f     = cfg_opts[8];
   double RL    = cfg_opts[9];
   double w_min = cfg_opts[4];
   double w_max = cfg_opts[5];
   double s_min = cfg_opts[6];
   double s_max = cfg_opts[7];
   
   time_t ltime; 
   ltime=time(NULL);

   if(verbose) {
     printf("\n#################################################\n");
     printf("### COIL-3D | Inductive Link Layout Optimiser ###\n");
     printf("#################################################\n");

     printf("# %s# Successfully read config file \n",asctime( localtime(&ltime)));
   }
   
   int logging = 0;
   int threed  = 0;

   int running = 1;

   int z = 0;
   char input_area[50];
   char input_freq[50];
   char input_techfile[100];
   double area = 0.00;

   if(d_o == 0)
   {
      printf("> Error - Please Enter area\n");
      return 1;
   }

   x = WIDTH;
   y = HEIGHT;
   sfc = cairo_create_x11_surface(&x, &y);
   ctx = cairo_create(sfc);

   cairo_move_to (ctx, 0, 0);

   nanosleep(&ts, NULL);
   if(verbose) {
     printf("> %s# Starting  Optimisation \n",asctime( localtime(&ltime)));
   }
   
   double high_score = 0.00;
   double score = 0.00;
   int    optimal_n = 0;
   double optimal_w = 0.0;
   double optimal_s = 0.0;
   
   while (running)
   {
         if(verbose) {
          printf("> %s# Optimising Fill-Factor \n",asctime( localtime(&ltime)));
         }
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
            usleep(DELAY);
      }
      d_i = d_o - (optimal_n * 2 * (w + s));
      double fill_factor = (d_o - d_i)/(d_o + d_i);
      if(verbose) {
        printf("> %s# Optimising Fill-Factor Complete - Best Value is : %f \n",asctime( localtime(&ltime)), fill_factor);
        printf("# Optimising Line Width ... \n");
      }
      high_score = 0;

      double width = w_min;
      while((width<((d_o - d_i)/2)) && (width < w_max))
      {
            w = width;
            n = (int)Round(((d_o - d_i)/2)/(w+s));
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
      if(verbose) {
        printf("# Optimised Track Width: %.10e\n", optimal_w );
      }
      high_score = 0;

      double spacing = s_min;
      while((spacing<(((d_o - d_i)/2)-optimal_w)) && (spacing < s_max))
      {
            s = spacing;
            w = optimal_w;
            n = (int)Round(((d_o - d_i)/2)/(w+s));
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
      if(verbose) {
        printf("# Optimised Track Spacing: %.10e\n", optimal_s);
      }

      if(logging)
      {
         if(verbose) {
           printf("# Optimising width/spacing ... \n");
         }
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
            }  

         }
      }

      s = optimal_s;
      w = optimal_w;
      n = (int)Round(((d_o - d_i)/2)/(w+s));
      double tab = d_o /50;
      double l = draw_inductor(ctx, n, d_o/SCALE, w/SCALE, s/SCALE, tab/SCALE);

      /* Generate Netlist */

      char netlist_fname[] = "/netlist.net";
      char netlist_bname[500];
      strcpy(netlist_bname,output_dir);
      strcat(netlist_bname, netlist_fname);
      FILE *netlist_fd = fopen(netlist_bname, "w");
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
      fprintf(netlist_fd, "K1 L1 L2 %.10e", mutual_inductance(d_o, d, mu, w, s, n)/self_inductance(d_o, d_i, n, mu));
      fprintf(netlist_fd, ".end\n");
      fclose(netlist_fd);
      
      /* Generate Outputfile */

      char output_fname[] = "/resuts.txt";
      char output_cname[500];
      strcpy(output_cname,output_dir);
      strcat(output_cname, output_fname);
      FILE *output_fd = fopen(output_cname, "w");
      fprintf(output_fd, "Optimal Track Width     : %f m\n", optimal_w );
      fprintf(output_fd, "Optimal Track Spacing   : %f m\n", optimal_s );
      fprintf(output_fd, "Optimal Outer Diameter  : %f m\n", d_o );
      fprintf(output_fd, "Optimal Number of Turns : %d\n", optimal_n );
      fclose(output_fd);
      
      if(verbose){
        printf("# Complete!\n# >");
      }

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

