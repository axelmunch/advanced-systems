#ifndef CONSTANTS_H
#define CONSTANTS_H

#define STR_SIZE 256
#define BUFFER_SIZE 4096
#define MAX_PATH_LENGTH 4096
#define USAGE_SYNTAX "[OPTIONS] [-f | -r <program_name> | -p | -v | -h]"
#define USAGE_PARAMS "OPTIONS:\n\
  -f, --fork : fork yourself \n\
  -r, --redirect <program_name> : redirect a program\n\
  -p, --pipe : pipe processes\n\
  -v, --verbose : enable *verbose* mode\n\
  -h, --help    : display this help\n\
"

#endif // CONSTANTS_H
