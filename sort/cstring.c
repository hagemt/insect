#include "cstring.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

size_t __CS_PUBLIC__
cstring_len(cstring input)
{
	assert(input);
	return strnlen(input, CS_MAX);
}

cstring __CS_PUBLIC__
cstring_cpy(cstring input)
{
	size_t len;
	char_t *output;
	assert(input);
	len = cstring_len(input);
	output = calloc(len + 1, sizeof(char_t));
	output[len] = CS_TERM;
	return (cstring) (output ? strncpy(output, input, len) : CS_NULL);
}

int __CS_PUBLIC__
cstring_cmp(cstring left, cstring right)
{
	assert(left);
	assert(right);
	return strncmp(left, right, CS_MAX);
}

int __CS_PRIVATE__
cstring_cmp_ptr(const void *left, const void *right)
{
	return cstring_cmp(* (cstring *) left, * (cstring *) right);
}
