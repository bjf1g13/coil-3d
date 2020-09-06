/* COIL-3D Main program */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <stdint.h>

#define OPTSTR "vp:t:h"
#define USAGE_FMT  "%s [-v] [-p parameter inputfile] [-t technology inputfile]  [-h] \n"
#define ERR_FOPEN_PARAMETER  "Cant open specified parameter file"
#define ERR_FOPEN_TECHNOLOGY  "Cant open specified technology file"
#define ERR_DO_THE_NEEDFUL "Optimiser exited with error"
#define DEFAULT_PROGNAME "COIL-3D"

extern int errno;
extern char *optarg;
extern int opterr, optind;

typedef struct {
  int           verbose;
  uint32_t      flags;
  FILE         *input;
  FILE         *output;
} options_t;


// COIL-3D Functional includes
#include "solver/solver.h"
#include "optimiser/optimiser.h"
#include "viewer/viewer.h"

int main(int argc, char *argv[]) {

    int opt;
    options_t options = { 0, 0x0, stdin, stdout };

    opterr = 0;

    while ((opt = getopt(argc, argv, OPTSTR)) != EOF)
       switch(opt) {
           case 'p':
              if (!(options.input = fopen(optarg, "r")) ){
                 perror(ERR_FOPEN_PARAMETER);
                 exit(EXIT_FAILURE);
                 /* NOTREACHED */
              }
              break;

           case 't':
              if (!(options.input = fopen(optarg, "r")) ){
                 perror(ERR_FOPEN_TECHNOLOGY);
                 exit(EXIT_FAILURE);
                 /* NOTREACHED */
              }
              break;

           case 'h':
           default:
              usage(basename(argv[0]), opt);
              break;
       }

    if (do_the_needful(&options) != EXIT_SUCCESS) {
       perror(ERR_DO_THE_NEEDFUL);
       exit(EXIT_FAILURE);
       /* NOTREACHED */
    }

    return EXIT_SUCCESS;
}

void usage(char *progname, int opt) {
   fprintf(stderr, USAGE_FMT, progname?progname:DEFAULT_PROGNAME);
   exit(EXIT_FAILURE);
}

int do_the_needful(options_t *options) {

   if (!options) {
     errno = EINVAL;
     return EXIT_FAILURE;
   }

   if (!options->input || !options->output) {
     errno = ENOENT;
     return EXIT_FAILURE;
   }

   /* XXX do needful stuff */

   return EXIT_SUCCESS;
}
