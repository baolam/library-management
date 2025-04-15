#ifndef CONSTANT_H
#define CONSTANT_H

/**
 * @brief Số lượng hiển thị tối đa ở một dòng
 */
#define MAX_ROW_ONEPAGE 15

/**
 * @brief Không thực hiện hành động nào hết
 */
#define BOOK_NO_ACTION -1

/**
 * @brief Mã trạng thái thêm sách mới
 */
#define BOOK_ADD_STATUS 0

/**
 * @brief Mã trạng thái cập nhật sách
 */
#define BOOK_UPDATE_STATUS 1

#include <gtk/gtk.h>

#include "ui_loader.h"
#include "books.h"
#include "reader.h"
#include "statistic.h"

#include "gui/constant.h"

#include "events/callbacks.h"
#include "events/book_treeview.h"
#include "events/reader_treeview.h"
#include "events/books_entry.h"
#include "events/search_toggle.h"
#include "events/choose_option.h"
#include "events/search.h"

/**
 * @brief Cấu trúc giao diện chính
 */
extern GtkBuilder *builder;

/// Nhóm biến trạng thái Menu
/**
 * @brief Trạng thái book_menu
 */
extern GtkWidget *book_menu;

/**
 * @brief Trạng thái reader_menu
 */
extern GtkWidget *reader_menu;

/// Nhóm biến trạng thái chạy của chương trình
// ===========================================

/**
 * @brief id được chọn
 */
extern gint chosen_id;

/**
 * @brief Hành động được chọn
 */
extern int chosen_action;

/**
 * @brief Lượng hiển thị hiện tại
 */
extern int current_book_page;

/**
 * @brief Lượng hiển thị reader hiện tại
 */
extern int current_reader_page;

#endif