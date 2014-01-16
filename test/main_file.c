#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "insect.h"

/* Tests two functions, taking path:
 * file_exists
 * file_type
 */

#define TEMP_DIR "/tmp"
#define TEMP_PRE "insect"
#define TEMP_XXX "XXXXXX"
#define TEMP_MAX 16

int
main(void)
{
	int failures;
	char d_name[TEMP_MAX];
	char f_name[TEMP_MAX];
	char o_name[TEMP_MAX];
	memset(d_name, '\0', TEMP_MAX);
	memset(f_name, '\0', TEMP_MAX);
	memset(o_name, '\0', TEMP_MAX);
	strncpy(d_name, TEMP_PRE, sizeof(TEMP_PRE));
	strncpy(f_name, TEMP_PRE, sizeof(TEMP_PRE));
	strncpy(o_name, TEMP_PRE, sizeof(TEMP_PRE));
	strncpy(d_name + sizeof(TEMP_PRE), TEMP_XXX, sizeof(TEMP_XXX));
	strncpy(f_name + sizeof(TEMP_PRE), TEMP_XXX, sizeof(TEMP_XXX));
	strncpy(o_name + sizeof(TEMP_PRE), TEMP_XXX, sizeof(TEMP_XXX));
	failures = 0;
	fprintf(stdout, "Using:\n"
			"\tDIRNAME = %s\n"
			"\tREGULAR = %s\n"
			"\tUNKNOWN = %s\n"
			"\n", d_name, f_name, o_name);

	/* negative tests */
	if (file_exists(o_name)) {
		++failures;
	}

	/* create some dummies */
	(void) mkdtemp(d_name);
	(void) mkstemp(f_name);
	/* (void) symlink(o_name, o_name); */

	/* positive tests */
	if (!file_exists(o_name)) {
		++failures;
	}

	/* TODO(teh): cleanup */
	return failures;
}
