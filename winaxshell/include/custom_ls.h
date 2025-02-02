#ifndef CUSTOM_LS_H
#define CUSTOM_LS_H

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

/**
 * List information about a file or directory
 * @param path
 * @return int Exit status
 */
int ls(const char *path);

/**
 * Returns number of digits in a number
 * @param value
 * @return int
 */
int count_digits(long long value);

/**
 * Print file information
 * @param full_path
 * @param file_name
 * @return void
 */
void file_info(const char *full_path, const char *file_name);

/**
 * List all files in a directory
 * @param path
 * @return void
 *
 * Format: permissions owner group size time filename
 * Example: rwxr-xr-- root root 4096 Jan 01 00:00 file.txt
 */
void list_directory(const char *path);

/**
 * Parse time to string
 * @param time
 * @return string
 *
 * Format time: %b %d %H:%M (ex: Jan 01 00:00)
 */
char *parse_time(time_t time);

/**
 * Get file type
 * @param perm
 * @return char
 *
 * Get more info via man 2 stat code example
 */
char get_filetype(mode_t mode);

/**
 * Get permissions of a file
 * @param perm
 * @return string
 *
 * Format permissions: rwxrwxrwx
 * Example: rwxr-xr--
 */
char *get_permissions(mode_t mode);

/**
 * Get owner
 * @param uid
 * @return user name
 * @return null if not found
 */
char *get_owner(uid_t uid);

/**
 * Get group
 * @param gid
 * @return group name
 * @return null if not found
 */
char *get_group(gid_t gid);

#endif // CUSTOM_LS_H
