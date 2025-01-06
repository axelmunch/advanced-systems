#ifndef FORK_YOURSELF_H
#define FORK_YOURSELF_H

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "print.h"


/**
 * @brief Fork yourself will fork a child process and print the PID and PPID of the child and the PID of the parent.
 * @param void
 * @return int
 * @retval EXIT_SUCCESS on success
 */
int fork_yourself();

#endif // FORK_YOURSELF_H