#include "ls_like.h"

void ls_like(const char *path)
{
    DIR *dir;

    dir = opendir(path);
    if (dir != NULL)
    {
        list_directory(dir);
    }
    else
    {
        perror("Couldn't open the directory");
        closedir(dir);
        exit(EXIT_FAILURE);
    }
}

void list_directory(DIR *dir)
{
    struct dirent *file;
    struct stat file_stat;

    while ((file = readdir(dir)) != NULL)
    {
        if (stat(file->d_name, &file_stat) == -1)
        {
            perror("Couldn't get file status");
            continue;
        }
        printf("%d %d %ld %s %s\n", file_stat.st_mode, file_stat.st_uid, file_stat.st_size, parse_time(file_stat.st_mtime), file->d_name);
    }
}


/**
 * Parse time to string
 * @param time
 * @return string
 * Format time: %b %d %H:%M (ex: Jan 01 00:00)
 */
char* parse_time(time_t time)
{
    static char time_str[STR_SIZE];
    struct tm *broken_time = localtime(&time);
    
    strftime(time_str, STR_SIZE, "%b %d %H:%M", broken_time);
    
    return time_str;
}

void show_filetype()
{
    
}

void show_permissions()
{

}

char* get_username()
{
    return "null";
}

char* get_groupname()
{
    return "null";
}