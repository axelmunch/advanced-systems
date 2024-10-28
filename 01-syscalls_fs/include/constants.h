#ifndef CONSTANTS_H
#define CONSTANTS_H

#define STDOUT 1
#define STDERR 2

#define BUFFER_SIZE 4096

#define MAX_PATH_LENGTH 4096

#define USAGE_SYNTAX "[OPTIONS] -i INPUT -o OUTPUT"
#define USAGE_PARAMS "OPTIONS:\n\
  -i, --input  INPUT_FILE  : input file\n\
  -o, --output OUTPUT_FILE : output file\n\
  -r --reverse  : reverse file content\n\
  -v, --verbose : enable *verbose* mode\n\
  -h, --help    : display this help\n\
"

#endif // CONSTANTS_H
