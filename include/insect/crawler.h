#ifndef __INSECT_CRAWLER_H__
#define __INSECT_CRAWLER_H__

/* FIXME(teh): use path from file/stat.h? */
typedef const char *Path;
typedef int (*Crawlback)(Path);

/* TODO(teh): find system pathmax? */
#define PATH_MAX 4096
int crawl(Path, Crawlback);

#include <libcalg/trie.h>
/* state seed, prior optional */
Crawlback reminisce(Trie *);

typedef TrieValue Datum;
/* stores path into state */
int remember(Path);

#include <stdio.h>
/* dumps state to file */
void regurgitate(FILE *);

#endif /* __INSECT_CRAWLER_H__ */
