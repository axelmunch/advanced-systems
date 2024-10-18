#include "ls_like.h"

void ls_like(const char *path)
{
    DIR *dir;

    dir = opendir(path);
    if (dir != NULL)
    {
        list_directory(dir, path);
    }
    else
    {
        perror("Couldn't open the directory");
        closedir(dir);
        exit(EXIT_FAILURE);
    }
}

// TO_DO: gerer le cas fichier, et les chemins absolu j'ai des erreurs

void list_directory(DIR *dir, const char *path)
{
    struct dirent *file;
    struct stat file_stat;
    char full_path[MAX_PATH_LENGTH];

    while ((file = readdir(dir)) != NULL)
    {
        snprintf(full_path, MAX_PATH_LENGTH, "%s/%s", path, file->d_name);

        if (stat(full_path, &file_stat) == -1)
        {
            perror("[ERROR] stat");
            closedir(dir);
            exit(EXIT_FAILURE);
        }
        printf("%d %s %s %ld %s %s\n", file_stat.st_mode, get_owner(file_stat.st_uid), get_group(file_stat.st_gid), file_stat.st_size, parse_time(file_stat.st_mtime), file->d_name);
    }

    closedir(dir);
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

/**
 * Get owner and group of the file by uid and gid with struct passwd and struct group
 * @return owner
 * @return group
 * @return null if not found
 */

char* get_owner(uid_t uid)
{
    return getpwuid(uid)->pw_name;
}
    

char* get_group(gid_t gid)
{
    return getgrgid(gid)->gr_name;
}