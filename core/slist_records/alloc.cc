#include "insect/records.h"

static void
__record(Record *);

static void
__path_copy(Path, Path *);
/* FIMXE(teh): ^ inline? */

#include <assert.h>

#include <stdlib.h>

Record *
record_new(Path path)
{
	Record *record;
	assert(path);
	record = malloc(sizeof(Record));
	assert(record);
	record->hits = 1;
	record->path = NULL;
	__path_copy(path, &record->path);
	__record(record);
	return record;
}

void
record_free(Record *record)
{
	assert(record);
	record->hits = 0;
	free((void *) record->path);
	free(record);
}

/* */

#include <string.h>

static void
__path_copy(Path old_path, Path *new_copy)
{
	int len;
	assert(old_path && new_copy);
	len = strnlen(old_path, INSECT_PATH_MAX);
	*new_copy = malloc(len + 1);
	assert(*new_copy);
	(void) strncpy((char *) *new_copy, old_path, len + 1);
}
