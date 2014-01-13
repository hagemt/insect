#include "insect/file/stat.h"

enum file_info_request_t {
	FILEIREQ_NONE = 0x00,
	FILEIREQ_TYPE = 0x01,
	FILEIREQ_PERM = 0x02,
	FILEIREQ_MASK = 0x03
};

typedef enum file_info_request_t
FileIreq; /* private API */

static int
__file_info(FilePath, FileInfo *, FileIreq);

#define FILE_INFO(PATH, NAME, IREQ) \
	FileInfo NAME; \
	if (__file_info(PATH, &NAME, IREQ)) { \
		/* FIXME __file_info failed */ \
	}

#define __FILE_INFO_CHECK(CHECK, MASK, NONE) ((CHECK & MASK) != NONE)
#define FILEIREQ_CHECK(LVAL, RVAL) __FILE_INFO_CHECK(LVAL, RVAL, FILEIREQ_NONE)
#define FILEPERM_CHECK(LVAL, RVAL) __FILE_INFO_CHECK(LVAL, RVAL, FILEPERM_NONE)

#include <assert.h>

FileType
file_type(FilePath path)
{
	FILE_INFO(path, info, FILEIREQ_TYPE);
	return info.file_type;
}

int
file_info(FilePath path, FileInfo *info)
{
	return __file_info(path, info, FILEIREQ_MASK);
}

int
file_exists(FilePath path)
{
	return __file_info(path, ((FileInfo *) 0), FILEIREQ_NONE) == 0;
}

int
file_can_read(FilePath path)
{
	FILE_INFO(path, info, FILEIREQ_PERM);
	return FILEPERM_CHECK(info.file_perm, FILEPERM_R);
}

int
file_can_write(FilePath path)
{
	FILE_INFO(path, info, FILEIREQ_PERM);
	return FILEPERM_CHECK(info.file_perm, FILEPERM_W);
}

#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

static int
__file_info(FilePath path, FileInfo *info, FileIreq req)
{
	struct stat __info;
	assert(path && (info || req == FILEIREQ_NONE));
	if (lstat(path, &__info)) {
		return errno;
	}
	if (FILEIREQ_CHECK(req, FILEIREQ_TYPE)) {
		info->file_type = FILETYPE_UNKNOWN;
		if (S_ISREG(__info.st_mode)) {
			info->file_type = FILETYPE_REGULAR;
		} else if (S_ISDIR(__info.st_mode)) {
			info->file_type = FILETYPE_DIRECTORY;
		} else {
			info->file_type = FILETYPE_UNKNOWN;
		}
	}
	if (FILEIREQ_CHECK(req, FILEIREQ_PERM)) {
		info->file_perm = FILEPERM_NONE;
		/* TODO(teh): use st_mode in combination with getuid */
	}
	return 0;
}
