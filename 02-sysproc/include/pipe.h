#ifndef PIPE_H
#define PIPE_H

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "print.h"
#include "pipe.h"

/**
 * @brief Execute the ps command with the given arguments using execlp
 * @param arg Arguments to pass to the ps command as a string
 * @return int
 */
int exec_ps(char *arg, ...);

/**
 * @brief Execute the grep command with the given arguments using execlp
 * @param arg Arguments to pass to the grep command as a string
 * @return int
 */
int exec_grep(char *arg, ...);

/**
 * @brief Execute the echo command with the given arguments using execlp
 * @param arg Arguments to pass to the echo command as a string
 * @return int
 */
int exec_echo(char *arg, ...);

/**
 * @brief Handle the ps command, close the read end of the pipe, duplicate the write end of the pipe to the STDOUT and close the write end of the pipe
 * @param pipe_fd[2] Pipe file descriptors
 * @return void
 */
void handle_ps(int pipe_fd[2]);

/**
 * @brief Handle the grep command, close the write end of the pipe, duplicate the read end of the pipe to the STDIN and close the read end of the pipe
 * @param pipe_fd[2] Pipe file descriptors
 * @return void
 */
void handle_grep(int pipe_fd[2]);

/**
 * @brief Handle the parent process, waui for the grep process to finish and check the exit status. If the exit status is 0, print "root est connecté", otherwise print "root n'est pas connecté"
 * @param grep_pid PID of the grep process
 * @return void
 */
void handle_parent(pid_t grep_pid);

/**
 * @brief Pipe processes, create a pipe, fork two processes, one for the ps command and one for the grep command. The parent process will wait for the grep process to finish and print the result.
 * @return void
 */
void pipe_process();

#endif // PIPE_H
