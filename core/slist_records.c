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

/*
typedef void (*Processor)(Record *, SListIterator *);

union Action {
	Crawlback simple;
	Processor complex;
};

enum ActionType { SIMPLE, COMPLEX };
*/

static int
__always_true(Record *record)
{
	assert(record);
	return 1;
}

typedef void (*SListVisitor)(SListIterator *);

static int
__record_each(Filter filter, Visitor external, SListVisitor internal)
{
	int x, y, z;
	Record *record;
	SListIterator iterator;
	if (!filter) filter = &__always_true;
	LOCK {
		x = slist_length(__record_head);
		slist_iterate(&__record_head, &iterator);
		for (y = z = 0; slist_iter_has_more(&iterator); ++z) {
			record = slist_iter_next(&iterator);
			assert(record && record->path);
			if ((*filter)(record)) { ++y;
				if (external) (*external)(record);
				if (internal) (*internal)(&iterator);
			}
		}
		assert(x == z);
		assert(y <= z);
	} UNLOCK
	return y;
}

void
record_each(Filter filter, Visitor action)
{
	(void) __record_each(filter, action, NULL);
}

int
record_purge(Filter filter, Visitor action)
{
	int purged = __record_each(filter, action, slist_iter_remove);
	LOCK {
		/* TODO(teh): reconsider...? */
		if (record_count() == 0) {
			slist_free(__record_head);
			__record_head = SLIST_NULL;
		}
	} UNLOCK
	return purged;
}
