#include "insect/records.h"

struct record_set_t {
	int size;
	Record **data;
};

typedef struct record_set_t RecordSet;

typedef int (*RecordComparator)(Record *, Record *);

/* a record set as defined above hold comparable records */

static int
__record_set_cmp(RecordSet *rs1, RecordSet *rs2, RecordComparator cmp)
{
	register int i1, i2, result;
	assert(rs1 && rs2 && cmp);
	result = rs1->size - rs2->size;
	if (result) return result;
	for (i1 = i2 = 0; i1 < rs1->size && i2 < rs2->size; ++i1, ++i2) {
		result = cmp(rs1->data[i1], rs2->data[i2]);
		if (result) return result;
	}
	return 0;
}

/* two strategies for record set creation: */

static void
__to_array_then_qsort(RecordSet *set, RecordComparator cmp)
{
	assert(set && cmp);
	LOCK {
		set->size = slist_length(__record_head);
		set->data = (Record **) slist_to_array(__record_head);
	} UNLOCK
	qsort(record_set,  sizeof(Record *), cmp);
}

static void
__sort_copy_then_to_array(RecordSet *set, RecordComparator cmp)
{
	SListEntry *head_copy;
	SListIterator iterator;
	assert(set && cmp);
	/* first make a clone of the list to sort */
	head_copy = SLIST_NULL;
	LOCK {
		slist_iterate(&__record_head, &iterator);
		while (slist_iter_has_more(&iterator)) {
			(void) slist_prepend(&head_copy, slist_iter_next(&iterator));
		}
	} UNLOCK
	slist_sort(&head_copy, (SListCompareFunc) cmp);
	/* copy data into record set and delete the copy */
	set->size = slist_length(head_copy);
	set->data = (Record **) slist_to_array(head_copy);
	slist_free(head_copy);
}
