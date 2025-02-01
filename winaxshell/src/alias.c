#include "alias.h"

alias_t aliases[MAX_ALIAS];
int alias_count = 0;

int set_alias(const char *name, const char *command)
{
    if (alias_count >= MAX_ALIAS)
    {
        return -1;
    }

    aliases[alias_count].name = strdup(name);
    aliases[alias_count].command = strdup(command);

    alias_count++;

    return 0;
}

int unset_alias(const char *name)
{
    int index = get_alias_index(name);
    if (index == -1)
    {
        return -1;
    }

    free_if_needed(aliases[index].name);
    free_if_needed(aliases[index].command);

    for (int i = index; i < alias_count - 1; i++)
    {
        aliases[i] = aliases[i + 1];
    }

    alias_count--;

    return 0;
}

char *get_alias_command(const char *name)
{
    for (int i = 0; i < alias_count; i++)
    {
        if (strcmp(aliases[i].name, name) == 0)
        {
            return strdup(aliases[i].command);
        }
    }

    return NULL;
}

int get_alias_count()
{
    return alias_count;
}

int get_alias_index(const char *name)
{
    for (int i = 0; i < alias_count; i++)
    {
        if (strcmp(aliases[i].name, name) == 0)
        {
            return i;
        }
    }

    return -1;
}

int get_alias_name_by_index(int index, char *name)
{
    if (index < 0 || index >= alias_count)
    {
        return -1;
    }

    strcpy(name, aliases[index].name);

    return 0;
}
