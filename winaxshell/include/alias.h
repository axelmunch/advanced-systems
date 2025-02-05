#ifndef ALIAS_H
#define ALIAS_H

#include <stdio.h>
#include <string.h>
#include "constants.h"
#include "typedef.h"
#include "utils.h"

/**
 * @brief Sets an alias for a command
 *
 * @param name Name of the alias
 * @param command Command to be aliased
 * @return int 0 on success, -1 on failure
 */
int set_alias(const char *name, const char *command);

/**
 * @brief Unsets an alias
 *
 * @param name Name of the alias
 * @return int 0 on success, -1 on failure
 */
int unset_alias(const char *name);

/**
 * @brief Retrieves the command associated with an alias
 *
 * @param name Name of the alias
 * @return char* The command associated with the alias, NULL if not found
 */
char *get_alias_command(const char *name);

/**
 * @brief Gets the total number of aliases
 *
 * @return int The number of aliases
 */
int get_alias_count();

/**
 * @brief Gets the index of an alias by its name
 *
 * @param name Name of the alias
 * @return int The index of the alias, -1 if not found
 */
int get_alias_index(const char *name);

/**
 * @brief Retrieves the name of an alias by its index
 *
 * @param index The index of the alias
 * @param name Name of the alias
 * @return int 0 on success, -1 on failure
 */
int get_alias_name_by_index(int index, char *name);

/**
 * @brief Frees all aliases
 *
 * @return void
 */
void free_aliases();

#endif // ALIAS_H
