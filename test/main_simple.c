#include <stdlib.h>
#include <stdio.h>

#include "insect.h"

int
main(int argc, char *argv[])
{
	if (argc < 2) {
		(void) fprintf(stderr, "[%s] %s (%s)\n",
				"USAGE", *argv, "path/to/crawl *");
		return EXIT_FAILURE;
	}
	(void) reminisce(NULL);
	while (--argc) {
		if (file_exists(argv[argc])) {
			(void) fprintf(stdout, "[INFO] crawled %d entries (for '%s')\n",
					crawl((const char *) argv[argc], &remember), argv[argc]);
		}
	}
	(void) regurgitate(stdout, NULL);
	return EXIT_SUCCESS;
}
