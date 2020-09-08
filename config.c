#include <stdio.h>
#include <stdlib.h>
#include <libconfig.h>

#include "config.h"

float read_config()
{
	config_t cfg, *cf;
	const config_setting_t *retries;
	double d_o;

	cf = &cfg;
	config_init(cf);

	if (!config_read_file(cf, "optimiser.cfg")) {
		fprintf(stderr, "%s:%d - %s\n",
			config_error_file(cf),
			config_error_line(cf),
			config_error_text(cf));
		config_destroy(cf);
		return(EXIT_FAILURE);
	}

	config_lookup_float(cf, "D", &d_o);
	config_destroy(cf);
	return d_o;
}
