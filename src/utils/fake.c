#include "fake.h"

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

char *genres[TOTAL_GENRE] = {
    "Fiction", "Mystery", "Science", "Adventure", "History", "Fantasy",
    "Programming", "Self-help", "Psychology", "Cooking", "Strategy", "Education"};

Book generate_book(int id)
{
    int publicationYear = rand() % 35 + 1990; // Random year from 1990 to 2024
    int stock = rand() % 50 + 1;              // Random stock from 1 to 50

    Book book;

    book.bookId = id;
    strcpy(book.title, titles[rand() % 15]);
    strcpy(book.author, authors[rand() % 14]);
    strcpy(book.genre, genres[rand() % 12]);
    book.publicationYear = publicationYear;
    book.stock = stock;

    return book;
}