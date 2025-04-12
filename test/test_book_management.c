#include "utils/fake.h"
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

    int actualBooks = 0;
    Book *books = retrieve_bucket_books(beginingKey, nums, &actualBooks);

    for (int i = 0; i < actualBooks; i++)
    {
        show_book(books[i]);
    }
}

void getRecommend()
{
    int maxNumbers;
    char prefix[100] = "The";
    // printf("Enter prefix: \n");
    // fgets(prefix, sizeof(prefix), stdin);
    // prefix[strcspn(prefix, "\n")] = 0;
    printf("Enter max numbers:");
    scanf("%d", &maxNumbers);
    printf("Prefix search : %s \n", prefix);
    search_book_by_title(prefix, maxNumbers);
    printf("\n");
}

int main()
{
    // Create a new book
    srand(time(NULL));

    int totalBooks = 200;
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
    if (skipCode == 0)
    {
        save_book_management();
    }
    printf("Save data successfully!\n");

    printf("Load data!\n");
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
        case 3:
            getRecommend();
            break;
        default:
            break;
        }
    } while (menu != 0);

    printf("Check exist word!\n");
    bool exist1 = existPrefix(book_trie, "the");
    printf("Exist word: %d\n", exist1);
    return 0;
}