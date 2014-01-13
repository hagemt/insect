#include "insect.h"

static struct {
	Trie *trie;
	Crawlback call;
	FILE *target;
} __session = {
	TRIE_NULL,
	&remember,
	NULL
};

#include <assert.h>
#include <stdlib.h>

static void
__dump_record(Record *);

Crawlback
reminisce(Trie *trie)
{
	assert(__session.trie == TRIE_NULL);
	__session.trie = (trie == TRIE_NULL) ? trie_new() : trie;
	return __session.call = &remember;
}

int
remember(Path path)
{
	Record *record;
	assert(path && __session.trie != TRIE_NULL);
	record = (Record *) trie_lookup(__session.trie, (char *) path);
	if (record) {
		++record->hits;
		__dump_record(record);
	} else {
		record = record_new((RecordPath) path);
		assert(record);
		return trie_insert(__session.trie, (char *) path, record) ? 1 : 0;
	}
	return 0;
}

void
regurgitate(FILE *file)
{
	assert(file && __session.trie != TRIE_NULL);
	__session.target = file;
	/* clean up records / state */
	record_purge(&__dump_record);
	trie_free(__session.trie);
	__session.trie = TRIE_NULL;
	__session.call = NULL;
}

/* */

static void
__dummy_dump_record(FILE *file, Record *record)
{
	assert(file && record);
	(void) fprintf(file, "[RECORD] %p (%s:%d)\n",
			(void *) record, record->path, record->hits);
}

static void
__dump_record(Record *record)
{
	__dummy_dump_record(__session.target, record);
}
