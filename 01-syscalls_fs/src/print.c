#include "../include/print.h"

void print(const char* format, ...)
{
    va_list args;
    
    va_start(args, format);
    
    char buffer[BUFFER_SIZE];
    vsnprintf(buffer, sizeof(buffer), format, args);
    
    write(STDOUT, buffer, strlen(buffer));
    
    va_end(args);
}
