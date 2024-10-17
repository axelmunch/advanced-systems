#include "ls_like.h"

void list_contents(const char *path)
{
    DIR *dp;
    struct dirent *ep;

    dp = opendir(path);
    if (dp != NULL) {
        while (ep = readdir(dp))
           puts(ep->d_name);
        closedir(dp); 
    }
    else
        perror("Couldn't open the directory");
    return 0;
}

void show_permissions()
{
}

void get_username()
{
}

void get_groupname()
{
}

void show_parameters()
{
}

void show_atime()
{
}
