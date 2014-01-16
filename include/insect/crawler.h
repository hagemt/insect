#ifndef __INSECT_CRAWLER_H__
#define __INSECT_CRAWLER_H__

#include "file/path.h"
typedef int (*Crawlback)(Path);

/* custom directory walker */
int crawl(Path, Crawlback);

/* begin to remember, seed optional */
int reminisce(void *);

/* stores path into state (records) */
int remember(Path);

/* sometimes memories are hard */
int regurgitate(void *, Crawlback);

#endif /* __INSECT_CRAWLER_H__ */
