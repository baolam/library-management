#include "fake.h"
#include "books.h"

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
        }
        else
        {
            printf("Invalid input. Please enter 1 or 0.\n");
        }
        printf("Generate book successfully!\n");

        printf("Save data!\n");
        save_book_management();
        printf("Save data successfully!\n");

        printf("Load data!:\n");
        load_book_management();
        printf("Load data successfully!\n");
        return 0;
    }