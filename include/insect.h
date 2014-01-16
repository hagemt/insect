#ifndef __INSECT_H__
#define __INSECT_H__

#ifdef __cplusplus
extern "C" {
#endif /* C++ */

#include "insect/crawler.h"
#include "insect/records.h"

/* basic file API */
#include "insect/file/stat.h"

#ifdef USE_THREADS
#include "insect/concurrent/thread.h"
#endif /* USE_THREADS */

#ifdef __cplusplus
} /* extern "C" */
#endif /* C++ */

#endif /* __INSECT_H__ */
