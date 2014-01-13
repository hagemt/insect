#include "insect/records.h"

#include <libcalg/slist.h>

static SListEntry *
__record_head = SLIST_NULL;

#ifdef USE_THREADS
#include <pthread.h>
static pthread_mutex_t
__record_mutex = PTHREAD_MUTEX_INITIALIZER;
#define LOCK   ((void) pthread_mutex_lock(&__record_mutex));
#define UNLOCK ((void) pthread_mutex_unlock(&__record_mutex));
#else /* single thread */
#define LOCK   ((void) 0);
#define UNLOCK ((void) 0);
#endif /* USE_THREADS */

#include "slist_records/alloc.cc"
#include "slist_records/sort.cc"

int
record_count(void)
{
	/* FIXME(teh): should use LOCK/UNLOCK? */
	return slist_length(__record_head);
}

#include <assert.h>

static void
__record(Record *record)
{
	SListEntry *new_head;
	assert(record);
	LOCK {
		new_head = slist_prepend(&__record_head, record);
	} UNLOCK
	assert(new_head == __record_head);
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
record_purge(Visitor optional)
{
	if (optional) record_each(optional);
	record_each(record_free);
	LOCK {
		slist_free(__record_head);
		__record_head = SLIST_NULL;
	} UNLOCK
}
