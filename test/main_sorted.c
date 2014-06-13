#include <stdlib.h>
#include <stdio.h>

#include "insect.h"

typedef const char *cstring;

static int
record_dump(Path);

static cstring
time_stamp(void);

#define TIME_STAMP (timestamp())

#define LOG_FILE "insect.log"

int
main(int argc, cstring argv[])
{
	FILE *logfp;
	if (argc < 2) {
		(void) fprintf(stderr, "[%s] %s (%s)\n",
				"USAGE", *argv, "path/to/crawl *");
		return EXIT_FAILURE;
	}

	/* prepare to record diagnostics */
	if ((logfp = fopen(LOG_FILE, "a+b"))) {
		(void) fprintf(logfp, "[insect] started logging at %s", TIME_STAMP);
	} else {
		(void) fprintf(stderr, "[FATAL] %s (fopen failed)\n", LOG_FILE);
		return EXIT_FAILURE;
	}
	/* TODO don't need this?
	if (create_pipe(DUMP_TARGET, true_output)) {
		(void) fprintf(logfp, "[insect] %s (failed to establish pipe)\n",
				strerror(errno));
		return EXIT_FAILURE;
	}
	*/

	/* initializes a walk down "memory lane" */
	if (reminisce(NULL)) {
		(void) fprintf(stderr, "[FATAL] couldn't start (reminisce failed)\n");
		return EXIT_FAILURE;
	}
	/* crawl specified directories (in a single thread) */
	while (--argc) {
		if (file_exists(argv[argc])) {
			(void) fprintf(stdout, "[INFO] crawled %d entries (for '%s')\n",
					crawl((cstring) argv[argc], &remember), argv[argc]);
		}
	}
	/* dump all the info into a file */
	if (regurgitate(logfp, &record_dump)) {
		(void) fprintf(stderr, "[WARNING] entries remaining (after crawl)\n");
	}

	/* clean up resources */
	(void) fprintf(logfp, "[insect] stopped logging at %s", TIME_STAMP);
	if (fclose(logfp)) {
		(void) fprintf(stderr, "[WARNING] %s (fclose failed)\n", LOG_FILE);
	}
	return EXIT_SUCCESS;
}

#include <assert.h>

static int
record_dump(Path path)
{
	assert(path);
	(void) fprintf(stdout, "[insect] found '%s'\n", path);
	return 1;
}

#include <time.h>

static cstring
time_stamp(void)
{
	time_t instant;
	(void) time(&instant);
	return ctime(&instant);
}
