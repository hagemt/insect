#include <stdlib.h>
#include <stdio.h>

#include "insect.h"
#define LOG_FILE "insect.log"

static int
record_dump(Path);

static const char *
time_stamp(void);

int
main(int argc, char *argv[])
{
	FILE *log_file;
	if (argc < 2) {
		(void) fprintf(stderr, "[%s] %s (%s)\n",
				"USAGE", *argv, "path/to/crawl *");
		return EXIT_FAILURE;
	}

	/* prepare to record diagnostics */
	if ((log_file = fopen(LOG_FILE, "a+b"))) {
		(void) fprintf(log_file, "[insect] started logging at %s", time_stamp());
	} else {
		(void) fprintf(stderr, "[FATAL] %s (fopen failed)\n", LOG_FILE);
		return EXIT_FAILURE;
	}
	/* TODO don't need this?
	if (create_pipe(DUMP_TARGET, true_output)) {
		(void) fprintf(log_file, "[insect] %s (failed to establish pipe)\n",
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
					crawl((const char *) argv[argc], &remember), argv[argc]);
		}
	}
	/* dump all the info into a file */
	if (regurgitate(log_file, &record_dump)) {
		(void) fprintf(stderr, "[WARNING] entries remaining (after crawl)\n");
	}

	/* clean up resources */
	(void) fprintf(log_file, "[insect] stopped logging at %s", time_stamp());
	if (fclose(log_file)) {
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

static const char *
time_stamp(void)
{
	time_t unix_time;
	(void) time(&unix_time);
	return ctime(&unix_time);
}
