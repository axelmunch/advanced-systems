#ifndef CONSTANTS_H
#define CONSTANTS_H

/**
 * @brief Maximum string size
 */
#define STR_SIZE 1024

/**
 * @brief Maximum buffer size to read/write
 */
#define BUFFER_SIZE 1024

/**
 * @brief Maximum path length for tmp files
 */
#define MAX_PATH_LENGTH 4096

/**
 * @brief Usage syntax of the binary
 */
#define USAGE_SYNTAX "[COMMAND] [OPTIONS] [-f | -r <program_name> | -p | -v | -h]"

/**
 * @brief Usage options explanation of the binary
 */
#define USAGE_PARAMS "OPTIONS:\n\
  -f, --fork : fork yourself \n\
  -r, --redirect <program_name> : redirect a program\n\
  -p, --pipe : pipe processes\n\
  -v, --verbose : enable *verbose* mode\n\
  -h, --help    : display this help\n\
"

#endif // CONSTANTS_H
