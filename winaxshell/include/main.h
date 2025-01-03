#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <getopt.h>
#include <unistd.h>
#include "typedef.h"
#include "print.h"

void free_if_needed(void *to_free);
char *dup_optarg_str();
void show_help(char **argv);
void show_parameters(bool verbose_mode);
void check_requirements();
void parse_options(int argc, char **argv);
void interactive_mode();
void batch_mode(int argc, char **argv);

#endif // MAIN_H