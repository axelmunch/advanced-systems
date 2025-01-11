#ifndef CONSTANTS_H
#define CONSTANTS_H

/**
 * @brief Shell name
 */
#define SHELL_NAME "winaxshell"

/**
 * @brief Maximum input size
 */
#define MAX_INPUT 1024

/**
 * @brief Maximum buffer size
 */
#define BUFFER_SIZE 1024 

/**
 * @brief Maximum number arguments
 */
#define MAX_ARGS 64

/**
 * @brief Binary options string linked to the binary options declaration (getopt)
 * @see man 3 getopt_long or getopt
 */
#define BINARY_OPTION_STR "vhc:"

/**
 * @brief Usage syntax of the binary
 */
#define USAGE_SYNTAX "[COMMAND] [OPTIONS] "

/**
 * @brief Usage parameters of the shell
 */
#define USAGE_PARAMS "OPTIONS:\n\
  -h, --help    : display this help\n\
  -v, --verbose : enable *verbose* mode\n\
  -c, --command : execute command\n"

/**
 * @brief Shell colors
 */
#define RESET_COLOR "\033[0m"
#define RED_COLOR "\033[0;31m"
#define GREEN_COLOR "\033[1;32m"
#define BLUE_COLOR "\033[1;34m"
#define CMD_DELIMITER " \t\n"

#endif // CONSTANTS_H
