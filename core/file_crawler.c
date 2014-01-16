#include "insect/crawler.h"

static int
__expand(Path, Crawlback);

#include <assert.h>

#include "insect/file/stat.h"

int
crawl(Path path, Crawlback callback)
{
	assert(path);
	switch (file_type(path)) {
	case FILETYPE_DIRECTORY:
		return __expand(path, callback);
	case FILETYPE_REGULAR:
		if (callback) return (*callback)(path);
	default:
		break;
	}
	return 0;
}

/* recursive workhorse */

#include <stdio.h>

/* TODO(teh): move out of core, unix? */
#include <dirent.h>
#include <unistd.h>

static int
__expand(Path path, Crawlback callback)
{
	DIR *dir;
	struct dirent *entry;
	char buffer[PATH_MAX];
	int crawled = 0;
	assert(path);
	dir = opendir(path);
	if (!dir) {
		(void) fprintf(stderr, "[%s] %s (%s)\n",
				"WARNING", path, "opendir failed");
		return 0;
	}
	while ((entry = readdir(dir))) {
		/* skip "hidden" directories */
		if (entry->d_name[0] == '.') continue;
		/* append slug to the path */
		(void) snprintf(buffer, PATH_MAX, "%s/%s", path, entry->d_name);
		buffer[PATH_MAX - 1] = '\0';
		/* aggregate results */
		crawled += crawl(buffer, callback);
	}
	if (closedir(dir)) {
		(void) fprintf(stderr, "[%s] %s (%s)\n",
				"WARNING", path, "closedir failed");
	}
	return crawled;
}
