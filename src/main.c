#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>
#include "gui/gui.h"
#include "books.h"
#include "reader.h"
#include "statistic.h"

jmp_buf jump_buffer;  // Lưu trạng thái để quay lại sau khi có lỗi

// Hàm xử lý khi gặp các tín hiệu lỗi
void signal_handler(int signum) {
    switch (signum) {
        case SIGSEGV:
            printf("Segmentation fault\n");
            break;
        case SIGABRT:
            printf("SIGABRT\n");
            break;
        case SIGFPE:
            printf("SIGFPE\n");
            break;
        default:
            printf("Undetected fault\n");
    }
    longjmp(jump_buffer, 1);  // Quay lại điểm đã lưu trong setjmp
}

int main(int argc, char *argv[])
{
    // Đăng ký handler cho tất cả các tín hiệu nguy hiểm
    signal(SIGSEGV, signal_handler);  // Segmentation fault
    signal(SIGABRT, signal_handler);  // Chương trình dừng
    signal(SIGFPE, signal_handler);   // Lỗi toán học

    if (setjmp(jump_buffer) == 0) {
        // Nếu không có lỗi, tiếp tục chạy bình thường
        auto_update_time();

        printf("Nam hien tai: %d\n", system_current_year);
        printf("Ngay hien tai: %d\n", system_date);

        load_book_management();
        load_reader_management();
        load_borrow_return_management();

        calc_statistic_book(book_management);
        calc_statistic_reader(reader_management);
        calc_statistic_borrowed_books(borrow_return_management);
        collect_late_borrowers(borrow_return_management);
        collect_genres();

        // list_late_borrowers();

        // printf("Total books : %d\n", total_books);
        // printf("Total readers : %d\n", total_readers);

        init_gui(argc, argv);
    } else {
        // Điểm quay lại sau khi có lỗi, chương trình không bị crash
    }

    return 0;
}
