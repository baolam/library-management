#include "books.h"

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

char management_file[MAX_FILE_NAME_LENGTH] = "book_management.bin";
char management_name_file[MAX_FILE_NAME_LENGTH] = "book_name_management.bin";
char content_file[MAX_FILE_NAME_LENGTH] = "book.bin";

Node *book_management = NULL;

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

void __showAddInfor(int id, int code, long offset, long length)
{
    printf("Id : %d, ", id);
    if (code == ADD_CONTENT_FAILED)
    {
        printf("Failed to add!\n");
    }
    else
    {
        printf("Offset : %ld, Length : %ld\n", offset, length);
    }
}

void save_book_management()
{
    saveTree(book_management, management_file);
}

void load_book_management()
{
    book_management = loadTree(management_file);
}

void add_book_stochastic(int total)
{
    int id;
    for (id = 1; id <= total; id++)
    {
        Book book = generate_book(id);
        book_management = add_content(book_management, id, content_file, &book, sizeof(Book), __showAddInfor);
    }
    save_book_management();
}

void on_search_book_by_id(FILE *f, long package_size)
{
    if (f == NULL)
    {
        printf("No data \n");
        return;
    }

    Book book;
    fread(&book, package_size, 1, f);

    show_book(book);
}

void search_book_by_id(int id)
{
    read_content(book_management, id, on_search_book_by_id);
}