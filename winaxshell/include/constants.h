#ifndef CONSTANTS_H
#define CONSTANTS_H

/**
 * @brief Shell name
 */
#define SHELL_NAME "winaxshell"

/**
 * @brief Maximum input size of the shell
 */
#define MAX_INPUT_LENGTH 1024

/**
 * @brief Maximum path length
 */
#define MAX_PATH_LENGTH 4096

/**
 * @brief Maximum buffer size for the shell
 */
#define BUFFER_SIZE 1024

/**
 * @brief Maximum number arguments
 */
#define MAX_ARGS 256

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
 * @brief Shell reset color
 */
#define RESET_COLOR "\033[0m"

/**
 * @brief Shell red color
 */
#define RED_COLOR "\033[0;31m"

/**
 * @brief Shell green color
 */
#define GREEN_COLOR "\033[1;32m"

/**
 * @brief Shell blue color
 */
#define BLUE_COLOR "\033[1;34m"

/**
 * @brief Shell cyan color
 */
 #define CYAN_COLOR "\033[1;36m"

/**
 * @brief Shell yellow color
 */
 #define YELLOW_COLOR "\033[1;33m"

/**
 * @brief Command delimiters
 */
#define CMD_DELIMITER " \t\n"

/**
 * @brief String macros: double quotes
 */
#define DOUBLE_QUOTES '\"'

/**
 * @brief String macros: single quote
 */
#define SINGLE_QUOTE '\''

/**
 * @brief Null character
 */
#define NULL_CHAR '\0'

/**
 * @brief Dollar sign character
 */
#define DOLLAR_SIGN '$'

/**
 * @brief Equal sign character
 */
#define EQUAL_SIGN '='

/**
 * @brief History file path
 */
#define HISTORY_FILE ".winaxshell_history"

/**
 * @brief Read end of the pipe
 */
#define READ_END 0

/**
 * @brief Write end of the pipe
 */
#define WRITE_END 1

/**
 * @brief String size
 */

#define STR_SIZE 100

/**
 * @brief Permission size
 */
#define PERM_SIZE sizeof("rwxrwxrwx")

#endif // CONSTANTS_H
