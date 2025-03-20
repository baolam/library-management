#ifndef FILES_H
#define FILES_H

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>

char *get_file_path(const char *folder_path, const char *file);

bool is_exist_folder(const char *path);
bool is_exist_file(const char *folder_path, const char *file_name);

bool delete_file(const char *folder_path, const char *filename);

#endif