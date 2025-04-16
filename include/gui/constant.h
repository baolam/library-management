#ifndef CONSTANT_H
#define CONSTANT_H

/**
 * @brief Số lượng hiển thị tối đa ở một dòng
 */
#define MAX_ROW_ONEPAGE 15

#define MAX_ROW_BORROWER_PAGE 10

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

/**
 * @brief Mã trạng thái độc giả (không có hành động)
 */
#define READER_NO_ACTION -1

/**
 * @brief Mã trạng thái độc giả (thêm độc giả)
 */
#define READER_ADD_STATUS 0

/**
 * @brief Mã trạng thái độc giả (cập nhật thông tin)
 */
#define READER_UPDATE_STATUS 1

#define BORROW_NO_ACTION -1

#define BORROW_ADD_STATUS 0

#define BORROW_UPDATE_STATUS 1

#include <gtk/gtk.h>

#include "ui_loader.h"
#include "books.h"
#include "reader.h"
#include "borrow_return.h"
#include "statistic.h"

#include "gui/constant.h"

#include "events/callbacks.h"
#include "events/book_treeview.h"
#include "events/reader_treeview.h"
#include "events/borrow_return_treeview.h"
#include "events/borrow_book_return_treeview.h"
#include "events/books_entry.h"
#include "events/readers_entry.h"
#include "events/book_borrow_entry.h"
#include "events/search_toggle.h"
#include "events/choose_option.h"
#include "events/search.h"
#include "events/statistic_show.h"

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

/**
 * @brief Trạng thái borrow_menu
 */
extern GtkWidget *borrow_menu;

extern GtkWidget *book_borrow_menu;

/// Nhóm biến trạng thái chạy của chương trình
// ===========================================

/**
 * @brief id được chọn
 */
extern gint book_chosen_id;

/**
 * @brief Hành động được chọn
 */
extern int book_chosen_action;

/**
 * @brief Id của Reader được chọn
 */
extern gint reader_chosen_id;

/**
 * @brief Hành động được chọn (đối với Reader)
 */
extern int reader_chosen_action;

/**
 * @brief Việc chọn thao tác đối với Id nào
 */
extern gint borrow_chosen_id;

/**
 * @brief Hành động được chọn (đối với Borrow)
 */
extern int borrow_chosen_action;

/**
 * @brief Lượng hiển thị hiện tại
 */
extern int current_book_page;

/**
 * @brief Lượng hiển thị reader hiện tại
 */
extern int current_reader_page;

extern int current_statistic_borrower_page;

/**
 * @brief Lượng hiển thị borrow hiện tại
 */
extern int current_borrow_page;

extern int current_book_borrow_page;

extern BorrowReturn *temp_borrow;

#endif