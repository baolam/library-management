#include <stdio.h>
#include "books.h"
#include "reader.h"
#include "borrow_return.h"

int main()
{
    load_reader_management();

    int readerId = 1001;

    // Mượn 2 cuốn
    borrow_book(readerId, 2001, 1);
    borrow_book(readerId, 2002, 2);

    // Hiển thị sách đã mượn
    search_borrow_record_by_reader(readerId);

    // Trả sách
    return_books(readerId);

    // Thống kê
    stat_total_books_by_reader(readerId);

    // save_reader_management();
    return 0;
}
