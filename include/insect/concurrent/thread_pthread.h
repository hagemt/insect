#ifndef __INSECT_PTHREAD_H__
#define __INSECT_PTHREAD_H__

#include <pthread.h>

struct thread {
	pthread_t id;
	void * (*worker)(void *);
	void *input, *output;
	struct thread *next;
};

typedef struct thread Thread;

#endif /* __INSECT_PTHREAD_H__ */
