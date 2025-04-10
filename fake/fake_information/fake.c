#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TOTAL_BOOKS 100 // 1 triệu quyển sách

int main()
{
    FILE *fptr = fopen("books.txt", "w");
    if (fptr == NULL)
    {
        printf("Cannot open file!\n");
        return 1;
    }

    srand(time(NULL));

    char *titles[] = {
        "The Great Adventure", "Mystery of the Lost Island", "Science and Future",
        "A Journey to the Stars", "The Secret of the Old Castle", "Beyond the Horizon",
        "Programming in C", "Data Structures & Algorithms", "History of Ancient Civilizations",
        "The Art of War", "Mastering Chess", "Cooking for Beginners", "Psychology and Mind",
        "The Road to Success", "Artificial Intelligence Basics"};

    char *authors[] = {
        "John Smith", "Alice Johnson", "Robert Brown", "Emily Davis", "William Wilson",
        "Sophia Martinez", "James Anderson", "Olivia Taylor", "Michael Thomas",
        "Elizabeth White", "David Harris", "Emma Clark", "Daniel Lewis", "Lucas Walker"};

    char *genres[] = {
        "Fiction", "Mystery", "Science", "Adventure", "History", "Fantasy",
        "Programming", "Self-help", "Psychology", "Cooking", "Strategy", "Education"};

    for (int i = 1; i <= TOTAL_BOOKS; i++)
    {
        char bookId[20];
        char title[100];
        char author[100];
        char genre[50];
        int publicationYear = rand() % 35 + 1990; // Random year from 1990 to 2024
        int stock = rand() % 50 + 1;              // Random stock from 1 to 50

        sprintf(bookId, "%06d", i); // Format: 000001 - 999999
        strcpy(title, titles[rand() % 15]);
        strcpy(author, authors[rand() % 14]);
        strcpy(genre, genres[rand() % 12]);

        fprintf(fptr, "%s| %s| %s| %s| %d| %d\n", bookId, title, author, genre, publicationYear, stock);

        // Print progress every 1%
        if (i % (TOTAL_BOOKS / 100) == 0)
        {
            printf("Generated: %d/%d books\n", i, TOTAL_BOOKS);
        }
    }

    fclose(fptr);
    printf("Completed! Data saved in 'books.txt'.\n");

    return 0;
}
