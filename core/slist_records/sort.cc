#include "insect/records.h"

/* two strategies for record set creation: */

#include "method1.ccc"
#include "method2.ccc"

#include <assert.h>

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

#include <stdlib.h>

RecordSet *
record_sort(RecordComparator cmp)
{
	RecordSet method1, method2, *record_set;
	assert(cmp);
	/* holds the final result */
	record_set = malloc(sizeof(RecordSet));
	assert(record_set);
	record_set->size = slist_length(__record_head);
	/* use independent methods */
	method1.size = method2.size = 0;
	method1.data = method2.data = NULL;
	__sort_copy_then_to_array(&method1, cmp);
	assert(method1.data && method1.size == record_set->size);
	__to_array_then_qsort(&method2, cmp);
	assert(method2.data && method2.size == record_set->size);
	/* ensure the results are symmetrical */
	if (__record_set_cmp(&method1, &method2, cmp)) {
		free(record_set);
		record_set = NULL;
	} else {
		record_set->data = method1.data;
		free(method2.data);
	}
	record_set->cmp = cmp;
	return record_set;
}
