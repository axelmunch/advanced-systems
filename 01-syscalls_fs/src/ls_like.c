#include "ls_like.h"

int max_size_user;
int max_size_group;
int max_size_file_size;

/**
 * Returns number of digits in a number
 * @param value
 * @return int
 */
int count_digits(long long value)
{
    if (value == 0)
    {
        return 1;
    }

    value = llabs(value);
    int count = 0;

    while (value > 0)
    {
        value /= 10;
        count++;
    }

    return count;
}

/**
 * Print file information
 * @param full_path
 * @param file_name
 * @return void
 */
void file_info(const char *full_path, const char *file_name)
{
    int space_quantity;

    struct stat file_stat;

    if (lstat(full_path, &file_stat) == -1)
    {
        print_error("[ERROR] stat");
        return;
    }

    // User
    print_generic(STDOUT_FILENO, "%c%s ",
                  get_filetype(file_stat.st_mode),
                  get_permissions(file_stat.st_mode));

    space_quantity = max_size_user - strlen(get_owner(file_stat.st_uid));
    for (int i = 0; i < space_quantity && space_quantity > 0; i++)
    {
        print_generic(STDOUT_FILENO, " ",
                      get_owner(file_stat.st_uid));
    }
    print_generic(STDOUT_FILENO, "%s ",
                  get_owner(file_stat.st_uid));

    // Group
    space_quantity = max_size_group - strlen(get_group(file_stat.st_gid));
    for (int i = 0; i < space_quantity && space_quantity > 0; i++)
    {
        print_generic(STDOUT_FILENO, " ",
                      get_group(file_stat.st_gid));
    }
    print_generic(STDOUT_FILENO, "%s ",
                  get_group(file_stat.st_gid));

    // Size
    space_quantity = max_size_file_size - count_digits(file_stat.st_size);
    for (int i = 0; i < space_quantity && space_quantity > 0; i++)
    {
        print_generic(STDOUT_FILENO, " ");
    }
    print_generic(STDOUT_FILENO, "%ld ",
                  file_stat.st_size);

    print_generic(STDOUT_FILENO, "%s ", parse_time(file_stat.st_mtime));

    // File name with color
    if (get_filetype(file_stat.st_mode) == 'd')
    {
        // Blue
        print_generic(STDOUT_FILENO, "\033[0;36m%s\033[0m\n", file_name);
    }
    else
    {
        print_generic(STDOUT_FILENO, "%s\n", file_name);
    }
}

/**
 * List all files in a directory
 * @param path
 * @return void
 *
 * Format: permissions owner group size time filename
 * Example: rwxr-xr-- root root 4096 Jan 01 00:00 file.txt
 */
void list_directory(const char *path)
{
    char full_path[MAX_PATH_LENGTH];

    DIR *dir_count_size = opendir(path);
    if (dir_count_size == NULL)
    {
        print_error("[ERROR] Couldn't open the directory");
        return;
    }

    // Count the max size of user, group and file size
    struct dirent *file_count_size;
    while ((file_count_size = readdir(dir_count_size)) != NULL)
    {
        snprintf(full_path, MAX_PATH_LENGTH, "%s/%s", path, file_count_size->d_name);

        struct stat file_stat;

        if (lstat(full_path, &file_stat) == -1)
        {
            print_error("[ERROR] stat");
            return;
        }

        max_size_user = strlen(get_owner(file_stat.st_uid)) > max_size_user ? strlen(get_owner(file_stat.st_uid)) : max_size_user;
        max_size_group = strlen(get_group(file_stat.st_gid)) > max_size_group ? strlen(get_group(file_stat.st_gid)) : max_size_group;
        max_size_file_size = count_digits(file_stat.st_size) > max_size_file_size ? count_digits(file_stat.st_size) : max_size_file_size;
    }

    closedir(dir_count_size);

    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        print_error("[ERROR] Couldn't open the directory");
        return;
    }

    struct dirent *file;

    print("[INFO] Listing directory %s\n", path);

    while ((file = readdir(dir)) != NULL)
    {
        snprintf(full_path, MAX_PATH_LENGTH, "%s/%s", path, file->d_name);
        file_info(full_path, file->d_name);
    }

    closedir(dir);
}

/**
 * List information about a file or directory
 * @param path
 * @return void
 */
void ls_like(const char *path)
{
    max_size_user = 0;
    max_size_group = 0;
    max_size_file_size = 0;

    struct stat path_stat;

    if (path == NULL)
    {
        path = ".";
    }

    if (lstat(path, &path_stat) == -1)
    {
        print_error("[ERROR] stat");
        return;
    }

    if (S_ISDIR(path_stat.st_mode))
    {
        list_directory(path);
    }
    else
    {
        file_info(path, path);
    }
}

/**
 * Parse time to string
 * @param time
 * @return string
 *
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
