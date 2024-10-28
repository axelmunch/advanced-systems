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

/**
 * List all files in a directory
 * @param dir
 * @param path
 * @return void
 *
 * Format: permissions owner group size time filename
 * Example: rwxr-xr-- root root 4096 Jan 01 00:00 file.txt
 */
void list_directory(DIR *dir, const char *path)
{
    struct dirent *file;
    struct stat file_stat;
    char full_path[MAX_PATH_LENGTH];

    printf("[INFO] Listing directory %s\n", path);

    while ((file = readdir(dir)) != NULL)
    {
        snprintf(full_path, MAX_PATH_LENGTH, "%s/%s", path, file->d_name);
        if (lstat(full_path, &file_stat) == -1)
        {
            perror("[ERROR] stat");
            continue;
        }
        printf("%c%s %s %s %ld %s %s\n",
               get_filetype(file_stat.st_mode),
               get_permissions(file_stat.st_mode),
               get_owner(file_stat.st_uid),
               get_group(file_stat.st_gid), file_stat.st_size,
               parse_time(file_stat.st_mtime), file->d_name);
    }

    closedir(dir);
}

/**
 * Parse time to string
 * @param time
 * @return string
 * Format time: %b %d %H:%M (ex: Jan 01 00:00)
 */
char *parse_time(time_t time)
{
    static char time_str[STR_SIZE];
    struct tm *broken_time = localtime(&time);

    strftime(time_str, STR_SIZE, "%b %d %H:%M", broken_time);

    return time_str;
}

/**
 * Get file type
 * @param perm
 * @return char
 *
 * Get more info via man 2 stat code example
 */
char get_filetype(mode_t perm)
{
    switch (perm & S_IFMT)
    {
    case S_IFREG:
        return '-';
    case S_IFDIR:
        return 'd';
    case S_IFLNK:
        return 'l';
    case S_IFBLK:
        return 'b';
    case S_IFCHR:
        return 'c';
    case S_IFIFO:
        return 'p';
    case S_IFSOCK:
        return 's';
    default:
        return '?';
    }
}

/**
 * Get permissions of a file
 * @param perm
 * @return string
 *
 * Format permissions: rwxrwxrwx
 * Example: rwxr-xr--
 */
char *get_permissions(mode_t perm)
{
    static char str[PERM_SIZE];
    snprintf(str, PERM_SIZE, "%c%c%c%c%c%c%c%c%c",
             (perm & S_IRUSR) ? 'r' : '-',
             (perm & S_IWUSR) ? 'w' : '-',
             (perm & S_IXUSR) ? 'x' : '-',
             (perm & S_IRGRP) ? 'r' : '-',
             (perm & S_IWGRP) ? 'w' : '-',
             (perm & S_IXGRP) ? 'x' : '-',
             (perm & S_IROTH) ? 'r' : '-',
             (perm & S_IWOTH) ? 'w' : '-',
             (perm & S_IXOTH) ? 'x' : '-');
    return str;
}

/**
 * Get owner and group of the file by uid and gid with struct passwd and struct group
 * @return owner
 * @return group
 * @return null if not found
 */
char *get_owner(uid_t uid)
{
    return getpwuid(uid)->pw_name;
}

char *get_group(gid_t gid)
{
    return getgrgid(gid)->gr_name;
}