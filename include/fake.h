/**
 * @file fake.h
 * @brief Những hàm sinh dữ liệu cơ bản, ngẫu nhiên phục vụ cho vấn đề của nhóm
 * @authors Bùi Hải Đăng
 * @version 1.0
 *
 * Chứa chương trình sinh dữ liệu Book, Reader
 */

#ifndef FAKE_H
#define FAKE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "books.h"
#include "reader.h"

/**
 * @brief Thể loại sách tối đa
 */
#define TOTAL_GENRE 12

/**
 * @brief Tiêu đề của sách
 */
extern char *titles[];

/**
 * @brief Tên tác giả
 */
extern char *authors[];

/**
 * @brief Thể loại sách
 */
extern char *genres[TOTAL_GENRE];

/**
 * @brief Sinh dữ liệu Book
 *
 * Hàm này sinh dữ liệu Book ngẫu nhiên và sẽ gán id (khoá tìm kiếm)
 * ứng với dữ liệu được sinh ra
 *
 * @param id khoá tìm kiếm được chỉ định
 */
Book generate_book(int id);

/**
 * @brief Sinh dữ liệu Reader
 *
 * Hàm này sinh dữ liệu Reader giả và sẽ gán id (khoá tìm kiếm)
 * ứng với Reader được sinh ra
 *
 * @param id khoá tìm kiếm được chỉ định
 */
Readers generate_reader(int id);

#endif