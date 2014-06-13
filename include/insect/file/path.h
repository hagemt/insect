#ifndef __INSECT_PATH_H__
#define __INSECT_PATH_H__

/**
 * \brief Insect expects paths to be at most this length and no longer.
 *
 * TODO(teh): move to a more-generic area for constant declarations?
 * TODO(teh): find/use system PATH_MAX? is a macro the best choice?
 */
#define INSECT_PATH_MAX 4096

/**
 * \brief All paths in insect are regarded to be of this type.
 */
typedef const char *Path;

/**
 * \brief Essentially a (generic) callback for insect crawls.
 */
typedef int (*Crawlback)(Path);

#endif /* __INSECT_PATH_H__ */
