#include <stdlib.h>
#include <stdio.h>

#include "insect.h"

int
main(int argc, char *argv[])
{
	Crawlback crawlback;
	if (argc < 2) {
		(void) fprintf(stderr, "[%s] %s (%s)\n",
				"USAGE", *argv, "path/to/crawl *");
		return EXIT_FAILURE;
	}
	crawlback = reminisce(TRIE_NULL);
	while (--argc) {
		if (file_exists(argv[argc])) {
			(void) fprintf(stdout, "[INFO] crawled %d entries (for '%s')\n",
					crawl((const char *) argv[argc], crawlback), argv[argc]);
		}
	}
	regurgitate(stdout);
	return EXIT_SUCCESS;
}
