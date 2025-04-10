#include "fake.h"
#include "books.h"

void showMenu()
{
    printf("0. Exit \n");
    printf("1. Show book \n");
    printf("2. Show list book \n");
    printf("3. Get recommend \n");
}

void showBook()
{
    int id;
    printf("Enter id :");
    scanf("%d", &id);
    search_book_by_id(id);
}

void showListBook()
{
    int beginingKey, nums;
    printf("Enter begining key:");
    scanf("%d", &beginingKey);
    printf("Enter nums:");
    scanf("%d", &nums);
}

void getRecommend()
{
}

int main()
{
    // Create a new book
    srand(time(NULL));

    int totalBooks = 100;
    int id;

    preparate_book();

    printf("Generate book :\n");
    int skipCode;
    do
    {
        printf("Should skip generate book ? (1/0) : ");
        scanf("%d", &skipCode);
        if (skipCode == 1)
        {
            break;
        }
        else if (skipCode == 0)
        {
            for (id = 1; id <= totalBooks; id++)
            {
                Book book = generate_book(id);
                add_book(&book);
            }
            break;
        }
        else
        {
            printf("Invalid input. Please enter 1 or 0.\n");
        }
    } while (skipCode != 1 || skipCode != 0);
    printf("Generate book successfully!\n");

    printf("Save data!\n");
    save_book_management();
    printf("Save data successfully!\n");

    printf("Load data!:\n");
    load_book_management();
    printf("Load data successfully!\n");

    int menu;
    do
    {
        showMenu();
        printf("Enter your choice: ");
        scanf("%d", &menu);
        switch (menu)
        {
        case 0:
            break;
        case 1:
            showBook();
            break;
        case 2:
            showListBook();
            break;
        default:
            break;
        }
    } while (menu != 0);
    return 0;
}