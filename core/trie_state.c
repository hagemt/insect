#include "insect.h"

static struct {
	Trie *trie;
	Crawlback call;
} __state;

#include <assert.h>
#include <stdlib.h>

Crawlback
reminisce(Trie *trie)
{
	assert(__state.trie == TRIE_NULL);
	__state.trie = (trie == TRIE_NULL) ? trie_new() : trie;
	return __state.call = &remember;
}

int
remember(Path path)
{
	Record *record;
	assert(path);
	record = (Record *) trie_lookup(__state.trie, (char *) path);
	if (record) {
		(void) fprintf(stderr, "[RECORD] %p (%s:%d)\n",
				record, path, record->hits);
		++record->hits;
	} else {
		record = malloc(sizeof(Record));
		record_new(record);
		record_record(record);
	}
	return trie_insert(__state.trie, (char *) path, record) ? 1 : 0;
}

void
regurgitate(FILE *file)
{
	assert(file);
	record_each((Visitor) free);
	trie_free(__state.trie);
}
