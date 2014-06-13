#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "cstring.h"

static void
print_usage(FILE *, cstring);

static void
cstrings_sort(cstrings, size_t);

int
main(int argc, cstring argv[])
{
	if (argc < 2) {
		print_usage(stderr, (cstring) *argv);
		return EXIT_FAILURE;
	}
	cstrings_sort((cstrings) (argv + 1), argc - 1);
	return EXIT_SUCCESS;
}

/* printing: */

static void
print_usage(FILE *file, cstring name)
{
	assert(file);
	(void) fprintf(file, "[USAGE] %s strings...\n", (char *) name);
}

static void
print_cstring(cstring input)
{
	assert(input);
	(void) fprintf(stdout, "[CSTRING %p]\t'%s'\n",
			(void *) input, (char *) input);
}

static int
print_cstrings(cstring prefix, cstrings input, size_t count)
{
	register size_t i;
	assert(input);
	for (i = 0; i < count; ++i) {
		(void) fprintf(stdout, "%s: ", prefix);
		print_cstring(input[i]);
	}
	return 0;
}

/* sorting: */

int cstring_cmp_ptr(const void *, const void *);
typedef int (*comparator)(const void *, const void *);
static void merge_sort(void *, size_t, size_t, comparator);
static void quick_sort(void *, size_t, size_t, comparator);
static void radix_sort(void *, size_t, size_t, size_t);

static int
cstrings_sorted(cstrings input, size_t count)
{
	register size_t i;
	for (i = 1; i < count; ++i) {
		if (cstring_cmp(input[i], input[i - 1]) < 0) {
			return 0;
		}
	}
	return 1;
}

#define SORTED(S,X,N) assert(cstrings_sorted(X,N) || print_cstrings(S,X,N))

static void
cstrings_sort(cstrings input, size_t count)
{
	cstrings args;
	assert(input);
	print_cstrings("before-sorts", input, count);

	args = cstrings_args(input, count);
	qsort(args, count, sizeof(cstring), &cstring_cmp_ptr);
	SORTED("qsort", args, count);
	free(args);

	args = cstrings_args(input, count);
	merge_sort(args, count, sizeof(cstring), &cstring_cmp_ptr);
	SORTED("merge", args, count);
	free(args);

	args = cstrings_args(input, count);
	quick_sort(args, count, sizeof(cstring), &cstring_cmp_ptr);
	SORTED("quick", args, count);
	free(args);

	args = cstrings_args(input, count);
	radix_sort(args, count, sizeof(cstring), count);
	/* SORTED("radix", args, count); */
	free(args);
}

void * memcpy(void *, void *, size_t);

#define ADD(OFFSET, BASE, SIZE) (((char *) BASE) + ((OFFSET) * (SIZE)))
#define COPY(SOURCE, SIZE, TARGET, OFFSET) \
	(void) memcpy(ADD(OFFSET, TARGET, SIZE), SOURCE, SIZE)

/* merge sort: */

static inline void
merge_into(void *base, size_t numel, size_t size, comparator cmp,
		size_t o1, size_t n1, size_t o2, size_t n2)
{
	void *copy, *left, *right;
	register size_t i1, i2;
	assert(o1 == 0 && o2 == n1);
	assert(0 < n1 && 0 < n2 && n1 + n2 == numel);
	copy = malloc(numel * size);
	if (!copy) return;
	i1 = i2 = 0;
	while (i1 < n1 && i2 < n2) {
		left = ADD(o1 + i1, base, size);
		right = ADD(o2 + i2, base, size);
		if (cmp(left, right) < 0) {
			COPY(left, size, copy, i1 + i2);
			++i1;
		} else {
			COPY(right, size, copy, i1 + i2);
			++i2;
		}
	}
	while (i1 < n1) {
		left = ADD(o1 + i1, base, size);
		COPY(left, size, copy, i1 + i2);
		++i1;
	}
	while (i2 < n2) {
		right = ADD(o2 + i2, base, size);
		COPY(right, size, copy, i1 + i2);
		++i2;
	}
	(void) memcpy(base, copy, numel * size);
	free(copy);
}

static void
merge_sort(void *base, size_t numel, size_t size, comparator cmp)
{
	size_t n;
	assert(base);
	assert(0 < size);
	assert(cmp);
	if (numel < 2) return;
	n = numel / 2;
	merge_sort(ADD(0, base, size),         n, size, cmp);
	merge_sort(ADD(n, base, size), numel - n, size, cmp);
	merge_into(base, numel, size, cmp, 0, n, n, numel - n);
}

/* quick sort: */

static inline void
quick_swap(void *one, void *two, size_t size)
{
	char tmp[size];
	assert(one && two);
	assert(0 < size);
	if (one == two) return;
	COPY(one, size, tmp, 0);
	COPY(two, size, one, 0);
	COPY(tmp, size, two, 0);
}

static inline void *
quick_pick(void *base, size_t numel, size_t size, comparator cmp, size_t half)
{
	void *one, *two;
	one = ADD(half, base, size);
	two = ADD(numel - 1, base, size);
	if (cmp(two, one) < 0) {
		one = ADD(numel - 1, base, size);
		two = ADD(half, base, size);
	}
	if (cmp(base, one) < 0) return one;
	if (cmp(two, base) < 0) return two;
	return base;
}

static inline size_t
quick_part(void *base, size_t numel, size_t size, comparator cmp, void *pivot)
{
	size_t l, r, p;
	void *left, *right;
	assert(pivot);
	quick_swap(base, pivot, size);
	pivot = base;
	l = 1, r = numel - 1, p = 0;
	while (l < r) {
		left  = ADD(l, base, size);
		right = ADD(r, base, size);
		if (cmp(right, left) < 0) {
			quick_swap(left, right, size);
			continue;
		}
		if (cmp(left, pivot) < 0) ++l, ++p;
		if (cmp(pivot, right) < 0) --r;
	}
	pivot = ADD(p, base, size);
	quick_swap(base, pivot, size);
	return (p == 0) ? 1 : p;
}

static void
quick_sort(void *base, size_t numel, size_t size, comparator cmp)
{
	size_t n, p;
	void *pivot;
	#define SMALL_SORT 2
	assert(base);
	assert(0 < size);
	assert(cmp);
	if (numel < 2) return;
	n = numel / 2;

	if (numel == SMALL_SORT) {
		pivot = ADD(1, base, size);
		if (cmp(pivot, base) < 0) {
			quick_swap(base, pivot, size);
		}
		return;
	}

	pivot = quick_pick(base, numel, size, cmp, n);
	p = quick_part(base, numel, size, cmp, pivot);
	pivot = ADD(p, base, size);
	quick_sort(base, p, size, cmp);
	quick_sort(pivot, numel - p, size, cmp);
}

/* radix sort: */

static void
radix_sort(void *base, size_t numel, size_t size, size_t buckets)
{
	assert(base);
	assert(0 < size);
	assert(!(buckets < numel));
	/* */
}
