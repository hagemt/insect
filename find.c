#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FIND_PATH_MAX (1024)
#define FIND_PUBLIC_API

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

typedef const char *cstring;
typedef void * (*Tailor)(cstring);
int visit(cstring, Tailor);

static int
expand(cstring dirpath, Tailor visitor) {
	DIR *dir; int count;
	struct dirent *entry;
	char buffer[FIND_PATH_MAX];
	assert(dirpath && visitor);
	if (!(dir = opendir(dirpath))) return 0;
	for (count = 0; (entry = readdir(dir));) {
		if (entry->d_name[0] == '.') continue;
		(void) snprintf(buffer, FIND_PATH_MAX, "%s/%s",
				dirpath, entry->d_name);
		count += visit(buffer, visitor);
	}
	if (closedir(dir)) {
		fprintf(stderr, "[WARNING] closedir failed (%s)\n",
				strerror(errno));
	}
	return count;
}

int FIND_PUBLIC_API
visit(cstring path, Tailor visitor)
{
	struct stat info;
	assert(path && visitor);
	if (lstat(path, &info)) {
		fprintf(stderr, "[WARNING] lstat failed (%s: %s)\n",
				path, strerror(errno));
		return 0;
	}
	if (S_ISDIR(info.st_mode)) {
		return expand(path, visitor);
	}
	return (*visitor)(path) != NULL;
}

struct entry_t {
	cstring path;
};

struct node_t {
	struct entry_t *head;
	struct node_t *tail;
};

typedef struct entry_t Entry;
typedef struct node_t Node;

static Node __ZERO__;

static inline void
free_entry(const Entry *entry) {
	assert(entry);
	if (entry->path) {
		free((char *) entry->path);
	}
	free((Entry *) entry);
}

static inline void *
new_node(const Entry *entry) {
	Node *node;
	assert(entry);
	if (!(node = malloc(sizeof(Node)))) {
		free_entry(entry);
		return NULL;
	}
	node->head = (Entry *) entry;
	node->tail = __ZERO__.tail;
	return __ZERO__.tail = node;
}

static void *
record(cstring path)
{
	size_t len;
	Entry *entry;
	assert(path);
	if (!(entry = malloc(sizeof(Entry)))) return NULL;
	len = strnlen(path, FIND_PATH_MAX);
	if (!(entry->path = calloc(len, sizeof(char)))) {
		free(entry);
		return NULL;
	}
	(void) strncpy((char *) entry->path, (char *) path, len);
	return new_node(entry);
}

static int
compare_entry_path(const void *entry1, const void *entry2)
{
	cstring path1, path2;
	assert(entry1 && entry2);
	path1 = ( * (Entry **) entry1) -> path;
	path2 = ( * (Entry **) entry2) -> path;
	return strncmp(path1, path2, FIND_PATH_MAX);
}

int
main(int argc, cstring argv[])
{
	Entry **entries;
	Node *node, *next;
	int i, num_entries;
	if (argc < 2) {
		fprintf(stderr, "[USAGE] %s path...\n", argv[0]);
		return EXIT_FAILURE;
	}

	num_entries = 0;
	__ZERO__.tail = NULL;
	for (i = 1; i < argc; ++i) {
		num_entries += visit(argv[i], &record);
	}

	next = __ZERO__.tail;
	entries = calloc(num_entries, sizeof(Entry *));
	for (i = 0; i < num_entries; ++i) {
		entries[i] = next->head;
		next = next->tail;
	}
	qsort(entries, num_entries, sizeof(Entry *), &compare_entry_path);
	for (i = 0; i < num_entries; ++i) {
		Entry *entry = entries[i];
		fprintf(stdout, "%p: %s\n", entry, entry->path);
	}
	free(entries);

	next = __ZERO__.tail;
	while ((node = next)) {
		next = next->tail;
		free_entry(node->head);
		free(node);
	}
	return EXIT_SUCCESS;
}
