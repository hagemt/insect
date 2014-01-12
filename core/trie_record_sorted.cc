#include <stdlib.h>

static RecordSet
record_sorted(RecordComparator cmp)
{
	RecordSet method1, method2, *record_set;
	assert(cmp);
	/* holds the final result */
	record_set = malloc(sizeof(RecordSet));
	assert(record_set);
	record_set->size = slist_length(__record_set);
	/* use independent methods */
	method1.size = method2.size = 0;
	method1.data = method2.data = NULL;
	__sort_copy_then_to_array(&method1, cmp);
	assert(method1.data && method1.size == record_set->size);
	__to_array_then_qsort(&method2, cmp);
	assert(method2.data && method2.size == record_set->size);
	/* ensure the results are symmetrical */
	if (__record_set_cmp(&method1, &method2)) {
		free(record_set);
		record_set = NULL;
	} else {
		record_set->data = method1.data;
		free(method2.data);
	}
	return record_set;
}
