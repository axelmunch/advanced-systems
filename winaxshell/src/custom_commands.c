#include "custom_commands.h"

static const char *custom_commands_list[] = {
    "ls_custom",
    "cd",
    "pwd",
    "echo",
    "exit",
    "alias",
    "unalias",
    NULL};

static const char *custom_commands_main_process_list[] = {
    "cd",
    "alias",
    "unalias",
    "help",
    NULL};

bool is_custom_command(char *command)
{
    for (int i = 0; custom_commands_list[i] != NULL; i++)
    {
        if (strcmp(command, custom_commands_list[i]) == 0)
            return true;
    }

    return false;
}

bool is_custom_command_main_process(char *command)
{
    for (int i = 0; custom_commands_main_process_list[i] != NULL; i++)
    {
        if (strcmp(command, custom_commands_main_process_list[i]) == 0)
            return true;
    }

    return false;
}

void execute_custom_command(char *command, char **args)
{
    if (!is_custom_command(command))
    {
        errno = EINVAL;
        print_error("[ERROR] Command is not a custom command");
        exit(EXIT_FAILURE);
        return;
    }

    int argc = 0;

    while (args[argc] != NULL)
        argc++;

    int status = EXIT_SUCCESS;

    if (strcmp(command, "ls_custom") == 0)
    {
        char *param = ".";
        if (argc > 2)
        {
            errno = EINVAL;
            print_error("[ERROR] Too many arguments for ls_custom");
            exit(EXIT_FAILURE);
            return;
        }

        if (argc == 2)
            param = args[1];

        status = ls(param);
    }
    else if (strcmp(command, "pwd") == 0)
    {
        char cwd[MAX_PATH_LENGTH];
        if (getcwd(cwd, sizeof(cwd)) == NULL)
        {
            print_error("[ERROR] getcwd");
            exit(EXIT_FAILURE);
        }

        print_generic(STDOUT_FILENO, "%s\n", cwd);
    }
    else if (strcmp(command, "echo") == 0)
    {
        for (int i = 1; i < argc; i++)
        {
            print_generic(STDOUT_FILENO, "%s", args[i]);
            if (i < argc - 1)
                print_generic(STDOUT_FILENO, " ");
        }
        print_generic(STDOUT_FILENO, "\n");
    }
    else if (strcmp(command, "exit") == 0)
    {
        int exit_status = EXIT_FAILURE;

        if (argc == 2)
        {
            if(strcmp(args[1], "0") == 0)
            {
                exit_status = EXIT_SUCCESS;
            }
        }

        exit(exit_status);
    }

    exit(status);
}

bool execute_custom_command_main_process(char *command, char **args)
{
    if (!is_custom_command_main_process(command))
    {
        errno = EINVAL;
        print_error("[ERROR] Command is not a custom command (main process)");
        return false;
    }

    int argc = 0;

    while (args[argc] != NULL)
        argc++;

    if (strcmp(command, "cd") == 0)
        return cd(argc, args);
    else if (strcmp(command, "alias") == 0)
    {
        if(argc == 1)
        {
            for(int i = 0; i < get_alias_count(); i++) {
                char name[MAX_INPUT_LENGTH];
                get_alias_name_by_index(i, name);
                char *command = get_alias_command(name);
                print_generic(STDOUT_FILENO, "alias %s='%s'\n", name, command);
                free_if_needed(command);
            }
            return true;
        }

        if(argc > 2)
        {
            errno = EINVAL;
            print_error("[ERROR] Invalid number of arguments for alias");
            return false;
        }

        char name[MAX_INPUT_LENGTH];
        char value[MAX_INPUT_LENGTH];
        int ret = get_var_values(args[1], name, value);
        if (ret < 0)
            return false;

        int status = set_alias(name, value);
        if(status == -1)
        {
            errno = EINVAL;
            print_error("[ERROR] Cannot set alias");
            return false;
        }

        return true;
    }
    else if (strcmp(command, "unalias") == 0)
    {
        if(argc != 2)
        {
            errno = EINVAL;
            print_error("[ERROR] Invalid number of arguments for unalias");
            return false;
        }

        int status = unset_alias(args[1]);
        if(status == -1)
        {
            errno = EINVAL;
            print_error("[ERROR] Alias not found");
            return false;
        }

        return true;
    }

    return true;
}