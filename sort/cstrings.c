#include "cstring.h"

#include <assert.h>
#include <stdlib.h>

cstrings __CS_PUBLIC__
cstrings_args(cstring input[], size_t count)
{
	register size_t i;
	cstrings output;
	assert(input);
	output = malloc(count * sizeof(cstring));
	if (!output) return (cstrings) CS_NULL;
	for (i = 0; i < count; ++i) {
		output[i] = cstring_cpy((cstring) input[i]);
	}
	return output;
}

void __CS_PUBLIC__
cstrings_free(cstrings input, size_t count)
{
	register size_t i;
	assert(input);
	for (i = 0; i < count; ++i) {
		free((void *) input[i]);
	}
	free((void *) input);
}

void __CS_PUBLIC__
cstrings_visit(cstrings input, size_t count, cstring_visitor visitor)
{
	register size_t i;
	assert(input);
	assert(visitor);
	for (i = 0; i < count; ++i) {
		(*visitor)(input[i]);
	}
}
