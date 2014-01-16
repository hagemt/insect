#ifndef __INSECT_RECORDS_H__
#define __INSECT_RECORDS_H__

#include "file/path.h"

struct record_t {
	Path path;
	int hits;
};

typedef struct record_t Record;

extern Record *
record_new(Path);

extern void
record_free(Record *);

extern int
record_count(void);

/* functional interface (visitors) */

typedef int (*Filter)(Record *);
typedef void (*Visitor)(Record *);

extern void
record_each(Filter, Visitor);

extern int
record_purge(Filter, Visitor);

/* sorted records (incomplete) */

typedef int (*RecordComparator)(Record *, Record *);

struct record_set_t {
	int size;
	Record **data;
	RecordComparator cmp;
};

typedef struct record_set_t RecordSet;

RecordSet *
record_sort(RecordComparator);

#endif /* __INSECT_RECORDS_H__ */
