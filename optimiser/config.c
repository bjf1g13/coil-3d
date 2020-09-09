#include <stdio.h>
#include <stdlib.h>
#include <libconfig.h>

#include "config.h"

int read_config(double * cfg_options, char * cf_fname)
{
	config_t cfg, *cf;
	const config_setting_t *retries;

	cf = &cfg;
	config_init(cf);

	if (!config_read_file(cf, cf_fname)) {
		fprintf(stderr, "%s:%d - %s\n",
			config_error_file(cf),
			config_error_line(cf),
			config_error_text(cf));
		config_destroy(cf);
		return(EXIT_FAILURE);
	}

  // Get configuration values out of the cfg file
	config_lookup_float(cf, "D", &(cfg_options[0]));
	config_lookup_float(cf, "mu", &(cfg_options[1]));
	config_lookup_float(cf, "d_i", &(cfg_options[2]));
	config_lookup_float(cf, "d", &(cfg_options[3]));
	config_lookup_float(cf, "w_min", &(cfg_options[4]));
	config_lookup_float(cf, "w_max", &(cfg_options[5]));
	config_lookup_float(cf, "s_min", &(cfg_options[6]));
	config_lookup_float(cf, "s_max", &(cfg_options[7]));
	config_lookup_float(cf, "f", &(cfg_options[8]));
	config_lookup_float(cf, "RL", &(cfg_options[9]));
	config_destroy(cf);
  return 0;
}
