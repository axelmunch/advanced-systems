#ifndef LS_LIKE_H
#define LS_LIKE_H

#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>

#include "print.h"
#include "constants.h"


#define STR_SIZE 100
#define MAX_PATH_LENGTH 4096


void ls_like(const char *path);
void list_directory(DIR *dir, const char *path);
char* parse_time(time_t time);
char get_filetype(mode_t mode);
char* get_permissions(mode_t mode);
char* get_owner(uid_t uid);
char* get_group(gid_t gid);

#endif // LS_LIKE_H