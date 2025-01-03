#ifndef CONSTANTS_H
#define CONSTANTS_H

#define SHELL_NAME "winaxshell"
#define MAX_INPUT 1024
#define BUFFER_SIZE 1024
#define MAX_ARGS 64
#define USAGE_SYNTAX "[COMMAND] [OPTIONS] "
#define USAGE_PARAMS "OPTIONS:\n\
  -v, --verbose : enable *verbose* mode\n\
  -h, --help    : display this help\n\
"
#define RESET_COLOR "\033[0m"
#define RED_COLOR "\033[0;31m"
#define GREEN_COLOR "\033[1;32m"
#define BLUE_COLOR "\033[1;34m"

#endif // CONSTANTS_H
