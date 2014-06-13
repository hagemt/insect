#ifndef __INSECT_PTHREAD_H__
#define __INSECT_PTHREAD_H__

#include <pthread.h>

struct thread_t {
	pthread_t id;
	void * (*worker)(void *);
	void *input, *output;
	struct thread_t *next;
};

/**
 * \brief Essentially a single- (forward-) linked node w/ context.
 */
typedef struct thread_t Thread;

#endif /* __INSECT_PTHREAD_H__ */
