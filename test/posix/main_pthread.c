#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define USE_PTHREAD
#include "insect.h"

static struct {
	Thread threads;
	Crawlback trigger;
} __session;

static void *
crawl_worker(void *data);

static int
spawn_worker(Thread *);

static int
collect_worker(Thread *);

int
main(int argc, char *argv[])
{
	Thread *thread;
	if (argc < 2) {
		(void) fprintf(stderr, "[%s] %s (%s)\n",
				"USAGE", *argv, "path/to/crawl *");
		return EXIT_FAILURE;
	}
	__session.threads.id = pthread_self();
	__session.threads.next = NULL;
	__session.trigger = reminisce(TRIE_NULL);
	while (--argc) {
		if (file_exists(argv[argc])) continue;
		thread = malloc(sizeof(Thread));
		if (!thread) continue;
		thread->worker = &crawl_worker;
		thread->input = argv[argc];
		thread->next = __session.threads.next;
		if (spawn_worker(thread)) {
			/* FIXME pthread_create failed */
			free(thread);
		} else {
			__session.threads.next = thread;
		}
	}
	/*
		(void) fprintf(stdout, "[INFO] crawled %d entries (for '%s')\n",
				crawl((const char *) argv[argc], trigger), argv[argc]);
	*/
	thread = __session.threads.next;
	while (thread) {
		Thread *temp = thread->next;
		if (collect_worker(thread)) {
			/* FIXME pthread_join failed */
		}
		free(thread);
		thread = temp;
	}
	regurgitate(stdout);
	return EXIT_SUCCESS;
}

#include <assert.h>

static void *
crawl_worker(void *data)
{
	Thread *thread = (Thread *) data;
	assert(thread && thread->input && __session.trigger);
	(void) crawl((const char *) thread->input, __session.trigger);
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
