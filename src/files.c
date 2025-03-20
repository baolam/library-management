#include "files.h"

char *get_file_path(const char *folder_path, const char *file_name)
{
    char temp[100];
    sprintf(temp, "%s/%s", folder_path, file_name);
    return temp;
}

bool is_exist_folder(const char *path)
{
    // struct dirent *entry;
    // DIR *dir = opendir(path);
    // if (dir == NULL)
    // {
    //     return false;
    // }
    // closedir(dir);
    return true;
}

bool is_exist_file(const char *folder_path, const char *file_name)
{
    FILE *f = fopen(get_file_path(folder_path, file_name), "r");
    if (f == NULL)
    {
        return false;
    }
    fclose(f);
    return true;
}

bool delete_file(const char *folder_path, const char *file_name)
{
    if (!is_exist_file(folder_path, file_name))
        return false;
    if (remove(get_file_path(folder_path, file_name)) == 0)
    {
        return true;
    }
    return false;
}