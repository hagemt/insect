#ifndef __STAT_H__
#define __STAT_H__

enum file_type_t {
	FILETYPE_UNKNOWN   = 0,
	FILETYPE_DIRECTORY = 1,
	FILETYPE_REGULAR   = 2,
	FILETYPE_MAX       = 3
};

typedef enum file_type_t
FileType; /* public API */

typedef const char *
FilePath; /* public API */

extern int
file_exists(FilePath);

extern FileType
file_type(FilePath);

/* TODO(teh): future API */

enum file_perm_t {
	FILEPERM_NONE = 0x00,
	FILEPERM_R    = 0x01,
	FILEPERM_W    = 0x02,
	FILEPERM_X    = 0x04,
	FILEPERM_RW   = FILEPERM_R | FILEPERM_W,
	FILEPERM_ALL  = FILEPERM_RW | FILEPERM_X
};

typedef enum file_perm_t
FilePerm;

struct file_info_t {
	FileType file_type;
	FilePerm file_perm;
};

typedef struct file_info_t
FileInfo; /* private API */

#endif /* __STAT_H__ */
