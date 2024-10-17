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
        perror("Couldn't open the directory");
    return 0;
}

void list_directory(DIR *dir)
{
    struct dirent *file;
    struct stat file_stat;

    while (file = readdir(dir))
    {
        if (stat(file->d_name, &file_stat) == -1)
        {
            perror("Couldn't get file status");
            return;
        }
        printf("%d %d %ld %s %s\n", file_stat.st_mode, file_stat.st_uid, file_stat.st_size, parse_time(file_stat.st_mtime), file->d_name);
    }
}

char* parse_time(time_t time)
{
    char *time_str;
    time_str = ctime(&time);
    time_str[strlen(time_str) - 1] = '\0'; // Pour aucun retour à la ligne
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
}

char* get_groupname()
{
}