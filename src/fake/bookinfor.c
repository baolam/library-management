#include "bookinfor.h"

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

Book generate_book(int id)
{
    Book _bookInfor;
    sprintf(_bookInfor.bookId, "%06d", id); // Format: 000001 - 999999
    strcpy(_bookInfor.title, titles[rand() % 15]);
    strcpy(_bookInfor.author, authors[rand() % 14]);
    strcpy(_bookInfor.genre, genres[rand() % 12]);
    _bookInfor.publicationYear = rand() % 35 + 1990;
    _bookInfor.stock = rand() % 50 + 1;

    return _bookInfor;
}

void show_book(Book book)
{
    printf("----------------------------\n");
    printf("Book ID: %s\n", book.bookId);
    printf("Book title: %s\n", book.title);
    printf("Book author: %s\n", book.author);
    printf("Book genere %s\n", book.genre);
    printf("Book publication year: %d\n", book.publicationYear);
    printf("Book stock: %d\n", book.stock);
}