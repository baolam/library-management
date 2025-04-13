#include "reader.h"
#include "borrow_return.h"
#include "books.h"
#include <stdio.h>
#include <string.h>

void show_menu()
{
    printf("\n=== Library Management System ===\n");
    printf("1. Update Reader\n");
    printf("2. Search Reader by ID\n");
    printf("3. Update Book\n");
    printf("4. Search Book by ID\n");
    printf("5. Borrow Books\n");
    printf("6. Return Books\n");
    printf("7. Exit\n");
    printf("Enter choice: ");
}

void updateReaderInfor()
{
    Readers r;
    printf("Enter Reader ID: ");
    if (scanf("%d", &r.readerId) != 1 || r.readerId <= 0)
    {
        printf("Error: Invalid Reader ID.\n");
        while (getchar() != '\n')
            ;
        return;
    }

    // Hiển thị reader trước đó
    search_reader_by_id(r.readerId);

    while (getchar() != '\n')
        ;

    printf("Enter Full Name: ");
    fgets(r.fullName, sizeof(r.fullName), stdin);
    r.fullName[strcspn(r.fullName, "\n")] = 0;

    printf("Enter Phone Number: ");
    fgets(r.phoneNumber, sizeof(r.phoneNumber), stdin);
    r.phoneNumber[strcspn(r.phoneNumber, "\n")] = 0;

    printf("Enter Address: ");
    fgets(r.address, sizeof(r.address), stdin);
    r.address[strcspn(r.address, "\n")] = 0;

    int code = update_reader_from_object(&r);
    if (code == UPDATE_SUCCESS)
    {
        printf("Update successfully!\n");
    }
    else
    {
        printf("Update failed!\n");
    }
}

void searchReader()
{
    int id;
    printf("Enter Reader ID: ");
    if (scanf("%d", &id) != 1 || id <= 0)
    {
        printf("Error: Invalid Reader ID.\n");
        while (getchar() != '\n')
            ;
        return;
    }
    search_reader_by_id(id);
}

void updateBookInfor()
{
    Book b;
    printf("Enter Book ID: ");
    if (scanf("%d", &b.bookId) != 1 || b.bookId <= 0)
    {
        printf("Error: Invalid Book ID.\n");
        while (getchar() != '\n')
            ;
        return;
    }

    /// Hiển thị thông tin trước đó
    search_book_by_id(b.bookId);

    while (getchar() != '\n')
        ;

    printf("Enter Title: ");
    fgets(b.title, sizeof(b.title), stdin);
    b.title[strcspn(b.title, "\n")] = 0;

    printf("Enter Author: ");
    fgets(b.author, sizeof(b.author), stdin);
    b.author[strcspn(b.author, "\n")] = 0;

    printf("Enter Genre: ");
    fgets(b.genre, sizeof(b.genre), stdin);
    b.genre[strcspn(b.genre, "\n")] = 0;

    printf("Enter Publication Year: ");
    if (scanf("%d", &b.publicationYear) != 1 || b.publicationYear < 0)
    {
        printf("Error: Invalid Publication Year.\n");
        while (getchar() != '\n')
            ;
        return;
    }

    printf("Enter Stock: ");
    if (scanf("%d", &b.stock) != 1 || b.stock < 0)
    {
        printf("Error: Invalid Stock value.\n");
        while (getchar() != '\n')
            ;
        return;
    }

    int code = update_book_from_object(&b);
    if (code == UPDATE_SUCCESS)
    {
        printf("Update successfully!\n");
    }
    else
    {
        printf("Update failed!\n");
    }
}

void searchBook()
{
    int id;
    printf("Enter Book ID: ");
    if (scanf("%d", &id) != 1 || id <= 0)
    {
        printf("Error: Invalid Book ID.\n");
        while (getchar() != '\n')
            ;
        return;
    }

    search_book_by_id(id);
}

void borrowBooks()
{
    BorrowReturn b;
    memset(&b, 0, sizeof(BorrowReturn));

    printf("Enter Reader ID: ");
    if (scanf("%d", &b.readerId) != 1 || b.readerId <= 0)
    {
        printf("Error: Invalid Reader ID.\n");
        while (getchar() != '\n')
            ;
        return;
    }
    printf("Enter number of books to borrow: ");
    if (scanf("%d", &b.totalBooks) != 1 || b.totalBooks <= 0 || b.totalBooks > 100)
    {
        printf("Error: Invalid number of books (must be between 1 and 100).\n");
        while (getchar() != '\n')
            ;
        return;
    }

    for (int i = 0; i < b.totalBooks; i++)
    {
        printf("Enter Book ID %d: ", i + 1);
        if (scanf("%d", &b.bookIds[i]) != 1 || b.bookIds[i] <= 0)
        {
            printf("Error: Invalid Book ID.\n");
            while (getchar() != '\n')
                ;
            return;
        }
        printf("Enter Quantity: ");
        if (scanf("%d", &b.quantities[i]) != 1 || b.quantities[i] <= 0)
        {
            printf("Error: Invalid Quantity.\n");
            while (getchar() != '\n')
                ;
            return;
        }
    }

    b.status = 0;
    b.onTime = true;

#if DEBUG_MODE
    printf("[DEBUG] Calling add_borrow_record for readerId %d\n", b.readerId);
#endif

    add_borrow_record(&b);
}

void returnBooks()
{
    int id;
    printf("Enter Reader ID: ");
    if (scanf("%d", &id) != 1 || id <= 0)
    {
        printf("Error: Invalid Reader ID.\n");
        while (getchar() != '\n')
            ;
        return;
    }
    return_books(id);
}

int main()
{
    // Khởi tạo
    printf("Initializing reader data...\n");
    prepare_reader();
    load_reader_management();

    prepare_book();
    load_book_management();

    printf("Initializing borrow/return data...\n");
    borrow_return_management = loadTree(borrow_return_management_file);
    if (borrow_return_management == NULL)
    {
        printf("No existing borrow management file. Starting fresh.\n");
        borrow_return_management = NULL;
    }
    else
    {
        printf("Borrow/return management loaded successfully.\n");
    }

    int choice;
    while (1)
    {
        show_menu();
        if (scanf("%d", &choice) != 1) // Kiểm tra lỗi nhập liệu
        {
            printf("Error: Invalid input. Please enter a number.\n");
            while (getchar() != '\n')
                ; // Xóa bộ đệm
            continue;
        }
        while (getchar() != '\n')
            ; // Xóa bộ đệm

        if (choice == 7)
            break;

        switch (choice)
        {
        case 1:
            updateReaderInfor();
            break;
        case 2:
            searchReader();
            break;
        case 3:
            updateBookInfor();
            break;
        case 4:
            searchBook();
            break;
        case 5: // Borrow Books
            borrowBooks();
            break;
        case 6:
            returnBooks();
            break;
        default:
            printf("Invalid choice!\n");
        }
    }

    // Lưu dữ liệu trước khi thoát
    printf("Saving data before exiting...\n");
    // save_reader_management();
    // save_book_management();
    // save_borrow_return_management();
    printf("Data saved successfully. Exiting.\n");
    return 0;
}