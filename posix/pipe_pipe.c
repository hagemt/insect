#include "insect/file/pipe.h"

#include <assert.h>
#include <unistd.h>

int
create_fpipe(FILE *ifp, FILE *ofp)
{
	int fds[2], fps[2];
	assert(ifp && ofp);
	if (pipe(fds)) {
		/* FIXME pipe failed? */
		return 1;
	}
	fps[0] = fileno(ifp);
	fps[1] = fileno(ofp);
	/* TODO dup2 x2 */
	return 0;
}
