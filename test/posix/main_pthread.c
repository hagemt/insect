#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "insect.h"
#ifndef USE_THREADS
#error "This file requires insect's concurrency (thread) features"
#endif /* USE_THREADS */

static Thread
__threads;

static void *
crawl_worker(void *data);

static int
spawn_worker(Thread *);

static int
collect_worker(Thread *);

int
main(int argc, char *argv[])
{
	Thread *current, *next;
	if (argc < 2) {
		(void) fprintf(stderr, "[%s] %s (%s)\n",
				"USAGE", *argv, "path/to/crawl *");
		return EXIT_FAILURE;
	}
	__threads.id = pthread_self();
	__threads.next = NULL;
	(void) reminisce(NULL);
	while (--argc) {
		if (!file_exists(argv[argc])) continue;
		current = malloc(sizeof(Thread));
		if (!current) continue;
		current->worker = &crawl_worker;
		current->input = argv[argc];
		current->next = __threads.next;
		if (spawn_worker(current)) {
			/* FIXME pthread_create failed */
			free(current);
		} else {
			__threads.next = current;
		}
	}
	/* wait, join, cleanup */
	next = __threads.next;
	while ((current = next)) {
		if (collect_worker(current)) {
			/* FIXME pthread_join failed */
		}
		next = current->next;
		free(current);
	}
	(void) regurgitate(stderr, NULL);
	return EXIT_SUCCESS;
}

#include <assert.h>

static void *
crawl_worker(void *data)
{
	Thread *thread = (Thread *) data;
	assert(thread && thread->input);
	(void) crawl((Path) thread->input, &remember);
	return thread->input;
}

static int
spawn_worker(Thread *thread)
{
	int result;
	pthread_attr_t attr;
	result = pthread_attr_init(&attr);
	assert(thread && result == 0);
	result = pthread_create(&thread->id, &attr, thread->worker, thread);
	(void) pthread_attr_destroy(&attr);
	return result;
}

static int
collect_worker(Thread *thread)
{
	assert(thread);
	return pthread_join(thread->id, &thread->output);
}
