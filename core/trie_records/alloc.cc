#include "insect/records.h"

static void
__record(Record *);

#include <assert.h>

#include <stdlib.h>
#include <string.h>

static void
__path_copy(RecordPath path, RecordPath *ref)
{
	int len = strnlen(path, RECORD_PATH_MAX);
	assert(ref);
	*ref = malloc(len + 1);
	assert(*ref);
	(void) strncpy(*ref, path, len + 1);
}

Record *
record_new(RecordPath path)
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
	free(record->path);
	free(record);
}
