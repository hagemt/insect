#include "insect/records.h"

#include <libcalg/slist.h>

static SListEntry *
__record_head;

#ifdef USE_PTHREAD
#include <pthread.h>
static pthread_mutex_t
__record_head__ = PTHREAD_MUTEX_INITALIZER;
#define LOCK   ((void) pthread_mutex_lock(&__record_head__);)
#define UNLOCK ((void) pthread_mutex_unlock(&__record_head__);)
#else
#define LOCK   ((void) 0);
#define UNLOCK ((void) 0);
#endif /* USE_PTHREAD */

#include <assert.h>

void
record_create(Record *record)
{
	assert(record);
	record->hits = 0;
}

void
record_destroy(Record *record)
{
	assert(record);
	record->hits = 0;
}

int
record_count(void)
{
	/* FIXME(teh): should use LOCK/UNLOCK? */
	return slist_length(__record_head);
}

void
record_each(Visitor visitor)
{
	SListIterator iterator;
	assert(visitor);
	LOCK {
		slist_iterate(&__record_head, &iterator);
		while (slist_iter_has_more(&iterator)) {
			(*visitor)(slist_iter_next(&iterator));
		}
	} UNLOCK
}

void
record_record(Record *record)
{
	SListEntry *new_head;
	assert(record);
	LOCK {
		new_head = slist_prepend(&__record_head, record);
	} UNLOCK
	assert(new_head == __record_head);
}

#include "trie_record_set.cc"
#include "trie_record_sorted.cc"