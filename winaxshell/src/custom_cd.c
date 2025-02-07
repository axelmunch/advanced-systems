#include "custom_cd.h"

static char *latest_cd_path = NULL;

bool cd(int argc, char **args)
{
    char *home = getenv(HOME_ENV_VAR);
    if (home == NULL)
    {
        print_error("[ERROR] HOME environment variable not set");
        return false;
    }

    // Default path
    char *param = home;

    if (latest_cd_path == NULL)
    {
        char cwd[MAX_PATH_LENGTH];
        if (getcwd(cwd, sizeof(cwd)) == NULL)
        {
            print_error("[ERROR] getcwd");
            return false;
        }

        latest_cd_path = strdup(cwd);
    }

    if (argc > 2)
    {
        errno = EINVAL;
        print_error("[ERROR] Invalid number of arguments for cd");
        return false;
    }

    // Input path
    if (argc == 2)
    {
        // -
        if (strcmp(args[1], "-") == 0)
            param = latest_cd_path;
        else if (strcmp(args[1], "~") == 0)
            param = home;
        else
            param = args[1];
    }

    // Change latest path
    char cwd[MAX_PATH_LENGTH];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        print_error("[ERROR] getcwd");
        return false;
    }
    if (strcmp(cwd, latest_cd_path) != 0)
        latest_cd_path = strdup(cwd);

    if (chdir(param) == -1)
    {
        print_error("[ERROR] cd");
        return false;
    }

    return true;
}
