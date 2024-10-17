#ifndef LS_LIKE_H
#define LS_LIKE_H

#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include "print.h"
#include "constants.h"

void ls_like(const char *path);
void parse_time(time_t time);
void show_filetype();
void show_permissions();
char* get_username();
char* get_groupname();

#endif // LS_LIKE_H