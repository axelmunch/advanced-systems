#ifndef LS_LIKE_H
#define LS_LIKE_H

#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include "print.h"
#include "constants.h"

void list_contents(const char *path);
void show_permissions();
void get_username();
void get_groupname();
void show_parameters();
void show_atime();

#endif // LS_LIKE_H