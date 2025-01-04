#ifndef CONSTANTS_H
#define CONSTANTS_H

#define SIZE (int)1e8
#define STR_SIZE 1024
#define BUFFER_SIZE 1024
#define TIME_SEC 1e6
#define TIME_MSEC 1e3
#define USAGE_SYNTAX "[COMMAND] num_thread [OPTIONS] "
#define USAGE_PARAMS "OPTIONS:\n\
  -t, --thread  : set the number of threads to use\n\
  -v, --verbose : enable *verbose* mode\n\
  -h, --help    : display this help\n\
"
#define RESET_COLOR "\033[0m"
/**
 * @brief Binary options string (linked to optionn declaration)
 * @see man 3 getopt_long or getopt
 */
#define BINARY_OPTION_STR "vht:"

#endif // CONSTANTS_H
