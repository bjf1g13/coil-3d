/*
 * COIL-3D Main program 
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
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define OPTSTR "vc:t:o:h"
#define USAGE_FMT  "%s [-v] [-c configuration inputfile] [-t technology inputfile] [-o output dir]  [-h] \n"
#define ERR_FOPEN_PARAMETER  "Cant open specified parameter file"
#define ERR_FOPEN_TECHNOLOGY  "Cant open specified technology file"
#define ERR_RUN_OPTIMISER "Optimiser exited with error"
#define ERR_OP_DIR "Output directory cannot be created"
#define DEFAULT_PROGNAME "COIL-3D"

extern int errno;
extern char *optarg;
extern int opterr, optind;

typedef struct {
  int          verbose;
  FILE         *param_input;
  FILE         *tf_input;
  char         cfg_file[100];
  char         output_dir[100];
} options_t;


// COIL-3D Functional includes
#include "viewer/cairo_functions.h"
#include "solver/solver.h"
#include "optimiser/optimiser.h"
#include "viewer/viewer.h"

// Parser to read config file
#include "optimiser/config.h"

void usage(char *progname, int opt);
int run_optimiser(options_t *options);

int main(int argc, char *argv[]) {

    int opt;
    options_t options = { 0, 0x0, stdin };

    opterr = 0;

    while ((opt = getopt(argc, argv, OPTSTR)) != EOF)
       switch(opt) {
           case 'c':
              strcpy(options.cfg_file, optarg);
              if (!(options.param_input = fopen(optarg, "r")) ){
                 perror(ERR_FOPEN_PARAMETER);
                 exit(EXIT_FAILURE);
              }
              break;

           case 't':
              if (!(options.tf_input = fopen(optarg, "r")) ){
                 perror(ERR_FOPEN_TECHNOLOGY);
                 exit(EXIT_FAILURE);
              }
              break;

           case 'o':
              strcpy(options.output_dir, optarg);
              if (mkdir(optarg, 0777)){
                perror(ERR_OP_DIR);
                exit(EXIT_FAILURE);
              }
              break;

           case 'h':
           default:
              usage(basename(argv[0]), opt);
              break;
       }

    if (run_optimiser(&options) != EXIT_SUCCESS) {
       perror(ERR_RUN_OPTIMISER);
       exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}

void usage(char *progname, int opt) {
   fprintf(stderr, USAGE_FMT, progname?progname:DEFAULT_PROGNAME);
   exit(EXIT_FAILURE);
}

int run_optimiser(options_t *options) {

   if (!options) {
     errno = EINVAL;
     return EXIT_FAILURE;
   }

   if (!options->param_input || !options->tf_input) {
     errno = ENOENT;
     return EXIT_FAILURE;
   }
   double cfg_opt[10];
   read_config(cfg_opt, options->cfg_file);
   optimiser(cfg_opt, options->verbose, options->output_dir);
   return EXIT_SUCCESS;
}
