#ifndef CONSTANTS_H
#define CONSTANTS_H

/**
 * @brief Size of the array to search min/max
 */
#define SIZE (int)1e8

/**
 * @brief Maximum string size
 */
#define STR_SIZE 1024

/**
 * @brief Maximum buffer size to read/write
 */
#define BUFFER_SIZE 1024

/**
 * @brief Time format in seconds
 */
#define TIME_SEC 1e6

/**
 * @brief Time format in milliseconds
 */
#define TIME_MSEC 1e3

/**
 * @brief Binary usage syntax
 */
#define USAGE_SYNTAX "[COMMAND] [OPTIONS] "

/**
 * @brief Binary usage parameters
 */
#define USAGE_PARAMS "OPTIONS:\n\
  -t, --thread  : set the number of threads to use [num_threads] like -t 2 \n\
  -v, --verbose : enable *verbose* mode\n\
  -h, --help    : display this help\n\
"
/**
 * @brief Binary options string (linked to option declaration)
 * @see man 3 getopt_long or getopt
 */
#define BINARY_OPTION_STR "vht:"

#endif // CONSTANTS_H
