#include "insect.h"

#include <libcalg/trie.h>

#include <stdio.h>
#include <stdlib.h>

static struct {
	Trie *trie;
	FILE *target;
	Crawlback filter;
} __session = {
	TRIE_NULL,
	NULL, NULL
};

#ifdef USE_THREADS
#include <pthread.h>
static pthread_mutex_t
__session_mutex = PTHREAD_MUTEX_INITIALIZER;
#define LOCK   ((void) pthread_mutex_lock(&__session_mutex));
#define UNLOCK ((void) pthread_mutex_unlock(&__session_mutex));
#else /* single thread */
#define LOCK   ((void) 0);
#define UNLOCK ((void) 0);
#endif /* USE_THREADS */

#include <assert.h>

int
reminisce(void *trie)
{
	assert(__session.trie == TRIE_NULL);
	__session.trie = (trie == TRIE_NULL) ? trie_new() : (Trie *) trie;
	return (__session.trie == TRIE_NULL);
}

int
remember(Path path)
{
	Record *record;
	int inserted = 0;
	assert(path && __session.trie != TRIE_NULL);
	LOCK {
		record = (Record *) trie_lookup(__session.trie, (char *) path);
		if (record) {
			++record->hits;
		} else {
			record = record_new(path);
			assert(record);
			if (trie_insert(__session.trie, (char *) path, record)) {
				inserted = record->hits;
			}
		}
	} UNLOCK
	return inserted;
}

static int
__filter_path_true(Path);

static int
__filter_record_by_path(Record *);

static void
__free_record_from_trie(Record *);

static void
__dump_record(Record *);

int
regurgitate(void *target, Crawlback filter)
{
	int before, after;
	assert(target && __session.trie != TRIE_NULL);
	if (!filter) filter = &__filter_path_true;
	__session.target = (FILE *) target;
	__session.filter = filter;
	before = trie_num_entries(__session.trie);
	record_each(&__filter_record_by_path, &__free_record_from_trie);
	after = trie_num_entries(__session.trie);
	if (before != after) {
		/* invalidate records/cache */
	}
	if (after == 0) {
		/* clean up records / state */
		trie_free(__session.trie);
		__session.trie = TRIE_NULL;
		/* TODO re-consider...? */
		record_purge(NULL, &__dump_record);
	}
	return after;
}

/* TODO(teh): use LOCK / UNLOCK for __session_mutex */

static int
__filter_path_true(Path path)
{
	assert(path);
	return 1;
}

static int
__filter_record_by_path(Record *record)
{
	assert(record && record->path);
	return (__session.filter)(record->path);
}

static void
__free_record_from_trie(Record *record)
{
	int result;
	assert(record && record->path);
	result = trie_remove(__session.trie, (char *) record->path);
	assert(result);
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
	if (!__session.target) __session.target = stderr;
	__dummy_dump_record(__session.target, record);
}
