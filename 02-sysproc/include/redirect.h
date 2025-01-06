#ifndef REDIRECT_H
#define REDIRECT_H

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "print.h"

/**
 * @brief Execute a program using execlp
 * @param program_name The name of the program to execute
 * @return void
 */
void exec_program(char *program_name);

/**
 * @brief Open a temporary file and redirect the file descriptor
 * @param redirect_fd The file descriptor to redirect
 * @return void
 */
void open_temp(int redirect_fd);

/**
 * @brief Show a message with the program first argument
 * @param message The message to show
 * @return int
 * @retval EXIT_SUCCESS
 */
int show_msg(char *message);

/**
 * @brief Process the child process
 * @param program_name The name of the program to execute
 * @param redirect_fd The file descriptor to redirect
 * @return void
 */
void process_child(char *program_name, int redirect_fd);

/**
 * @brief Process the parent process, wait for the child process to finish and print "That's All Folks!"
 * @return void
 */
void process_parent();

/**
 * @brief Redirect the file descriptor
 * @param program_name The name of the program to execute
 * @param redirect_fd The file descriptor to redirect
 * @return void
 */
void redirect(char *program_name, int redirect_fd);

#endif // REDIRECT_H
