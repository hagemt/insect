#ifndef __INSECT_RECORDS_H__
#define __INSECT_RECORDS_H__

/* TODO(teh): use platform PATH_MAX */
#define RECORD_PATH_MAX 4096
typedef char *RecordPath;

struct record_t {
	RecordPath path;
	int hits;
};

typedef struct record_t Record;

extern Record *
record_new(RecordPath);

extern void
record_free(Record *);

extern int
record_count(void);

/* functional interface (visitors) */

typedef void (*Visitor)(Record *);

extern void
record_each(Visitor);

extern void
record_purge(Visitor);

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
