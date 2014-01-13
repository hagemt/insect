#include "insect/crawler.h"

static int
__expand(Path, Crawlback);

#include <assert.h>

#include "insect/file/stat.h"

int
crawl(Path path, Crawlback call)
{
	FileInfo info;
	assert(path);
	if (file_info(path, &info)) {
		/* FIXME file_info failed */
		return 0;
	}
	if (info.file_type == FILETYPE_DIRECTORY) {
		return __expand(path, call);
	} else if (call) {
		return (*call)(path);
	}
	return 0;
}

/* recursive workhorse */

#include <stdio.h>

/* TODO(teh): move out of core, unix? */
#include <dirent.h>
#include <unistd.h>

static int
__expand(Path path, Crawlback call)
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
		crawled += crawl(buffer, call);
	}
	if (closedir(dir)) {
		(void) fprintf(stderr, "[%s] %s (%s)\n",
				"WARNING", path, "closedir failed");
	}
	return crawled;
}
