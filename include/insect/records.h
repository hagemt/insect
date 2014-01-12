#ifndef __INSECT_RECORDS_H__
#define __INSECT_RECORDS_H__

struct record_t {
	int hits;
};

typedef struct record_t Record;

void record_create(Record *);
void record_destroy(Record *);

int record_count(void);
void record_record(Record *);

typedef void (*Visitor)(Record *);
void record_each(Visitor);

/* TODO(teh): sorted records */

#endif /* __INSECT_RECORDS_H__ */
