#ifndef FAKE_H
#define FAKE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "books.h"
#include "reader.h"

#define TOTAL_GENRE 12

/// @brief tiêu đề của cuốn sách
extern char *titles[];

/// @brief tên tác giả
extern char *authors[];

/// @brief thể loại sách
extern char *genres[TOTAL_GENRE];

/// @brief sinh sách giả để quản lí
/// @param id khoá id của cuốn sách
/// @return quyển sách
Book generate_book(int id);

/// @brief sinh reader giả để quản lí
/// @param id khoá id của reader
/// @return reader
Readers generate_reader(int id);

#endif