#ifndef __CSTRING_H__
#define __CSTRING_H__

#include <stddef.h>

/*** DEFINES ***/

#define CS_MAX  ((size_t)  4096)
#define CS_NULL ((cstring) NULL)
#define CS_TERM ((char_t)  '\0')

#define __CS_PRIVATE__
#define __CS_PUBLIC__

/*** TYPES ***/

/** Simple **/

#ifdef CS_WCHAR_T
typedef wchar_t char_t;
#else /* default: char */
typedef char char_t;
#endif /* CS_WCHAR_T */

typedef const char_t *cstring;
typedef cstring *cstrings;

typedef void (*cstring_visitor)(cstring);

/** Constructed **/

/* TODO(hagemt): computed-length string (and set of) */

struct string_t {
	size_t len;
	cstring str;
};

typedef struct string_t String;

struct strings_t {
	size_t size, *sizes;
	String *strings;
};

/*** FUNCTIONS ***/

/** cstring(s) **/

size_t __CS_PUBLIC__
cstring_len(cstring);

int __CS_PUBLIC__
cstring_cmp(cstring, cstring);

cstring __CS_PUBLIC__
cstring_cpy(cstring);

cstrings __CS_PUBLIC__
cstrings_args(cstring[], size_t);

void __CS_PUBLIC__
cstrings_free(cstrings, size_t);

void __CS_PUBLIC__
cstrings_visit(cstrings, size_t, cstring_visitor);

/* string */

/* TODO(hagemt): useful string functions */

#endif /* __CSTRING_H__ */
