#ifndef __INSECT_H__
#define __INSECT_H__

#include "insect/crawler.h"
#include "insect/records.h"

/* basic file API */
#include "insect/file/stat.h"

#ifdef USE_PTHREAD
#include "concurrent/thread.h"
#endif /* USE_PTHREAD */

#endif /* __INSECT_H__ */
