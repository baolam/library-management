#include <stdio.h>
#include "books.h"
#include "borrow_return.h"
#include "reader.h"

// Giả định bạn có các hàm này để khởi tạo hoặc giả lập dữ liệu
void setup_test_data()
{
    BorrowReturn br = {
        .readerId = 1,
        .totalBooks = 2,
        .bookIds = {100, 102},
        .quantities = {1, 2},
        .status = ON_BORROWING,
        .onTime = true};

    add_borrow_record(&br);
}

int main()
{
    load_reader_management();
    load_book_management();
    load_borrow_return_management();

    /// Thử hiển thị thông tin sách
    search_book_by_id(101);

    // Setup dữ liệu giả
    // setup_test_data();

    // ===== TEST 1: gui_add_borrow_record =====
    BorrowReturn testBorrow = {
        .readerId = 2,
        .totalBooks = 1,
        .bookIds = {101},
        .quantities = {1},
        .status = ON_BORROWING,
        .onTime = true};

    int addResult = gui_add_borrow_record(&testBorrow);
    printf("TEST 1 - Add borrow status : %d\n", addResult);
    printf("TEST 1 - Add borrow: %s\n", addResult == BORROW_SUCCESS ? "OK" : "FAIL");
    printf("\n");

    // ===== TEST 2: gui_return_books =====
    int returnResult = gui_return_books(2);
    printf("TEST 2 - Return books status : %d\n", returnResult);
    printf("TEST 2 - Return books: %s\n", returnResult == RETURN_SUCCESS ? "OK" : "FAIL");
    printf("\n");

    printf("TEST SEARCHING ..... \n");
    search_borrow_record_by_reader(2);

    // ===== TEST 3: gui_is_book_borrowed =====
    bool isBorrowed = gui_is_book_borrowed(101);
    printf("TEST 3 - Book 101 borrow status : %d\n", isBorrowed);
    printf("TEST 3 - Book 101 is borrowed? %s\n", isBorrowed ? "YES" : "NO");

    // ===== TEST 4: gui_stat_total_books_by_reader =====
    int totalBooks = gui_stat_total_books_by_reader(2);
    printf("TEST 4 - Total books borrowed by reader 2: %d\n", totalBooks);

    return 0;
}
