#include "utils/fake.h"

char *prefixes[] = {
    "The", "A", "Beyond", "Inside", "Under", "Whispers of", "Echoes from"};

char *realWords[] = {
    "Shadow", "Dream", "Empire", "Chronicle", "Legend", "Flame", "Whisper", "Storm"};

char *nonsenseWords[] = {
    "Zarthul", "Morgix", "Veblor", "Tharnok", "Yuvana", "Quellor", "Droven", "Xandoria", "Vaccum", "Dead", "Live"};

char *authors[] = {
    "John Smith", "Alice Johnson", "Robert Brown", "Emily Davis", "William Wilson",
    "Sophia Martinez", "James Anderson", "Olivia Taylor", "Michael Thomas",
    "Elizabeth White", "David Harris", "Emma Clark", "Daniel Lewis", "Lucas Walker"};

char *genres[TOTAL_GENRE] = {
    "Fiction", "Mystery", "Science", "Adventure", "History", "Fantasy",
    "Programming", "Self-help", "Psychology", "Cooking", "Strategy", "Education"};

char *firstnames[] = {"Duc", "Thanh", "Nam", "Quan", "Anh", "Hoang", "Tien", "Dung", "Nhan", "Hoai",
                      "Nhi", "Thuong", "Sau", "Nga", "Thang", "Khanh", "Linh", "My", "Hanh", "Dat",
                      "Han", "Van", "Oanh", "Huy", "Thinh", "Kiet", "Trinh", "Uyen"};
char *lastnames[] = {"Nguyen", "Tran", "Le", "Pham", "Hoang", "Huynh", "Phan", "Vu", "Vo", "Dang",
                     "Bui", "Do", "Ho", "Ngo", "Duong", "Ly"};
char *middlenames[] = {"Van", "Huu", "Duc", "Xuan", "Ngoc", "Quang", "Cong", "Khanh", "Minh", "Tri",
                       "Tuan", "Duc", "Phuc", "Thanh", "Thinh", "Thi", "Thuy", "Lan", "Hong", "Mai",
                       "Nhung", "Ngoc", "Quynh", "Trang", "Vy", "Anh", "Chi", "Hoa", "Loan"};

char *addresses[] = {
    "District 1, Ho Chi Minh", "District 2, Ho Chi Minh", "District 3, Ho Chi Minh",
    "District 7, Ho Chi Minh", "Thu Duc City", "Binh Thanh District",
    "Go Vap District", "Tan Binh District", "Nha Be District", "Cu Chi District",
    "Can Tho City", "Da Nang City", "Nha Trang City", "Hue City", "Ha Noi City",
    "Hai Phong City", "Vung Tau City", "Bien Hoa City", "Long An Province",
    "Binh Duong Province", "Dong Nai Province"};

char *phone_prefix[] = {"09", "08", "07", "03", "05"};

void generateRandomBookTitle(char *output, int maxLen)
{
    const char *prefix = prefixes[rand() % SIZE(prefixes)];
    const char *word1 = rand() % 2 ? realWords[rand() % SIZE(realWords)] : nonsenseWords[rand() % SIZE(nonsenseWords)];
    const char *word2 = rand() % 2 ? realWords[rand() % SIZE(realWords)] : nonsenseWords[rand() % SIZE(nonsenseWords)];

    snprintf(output, maxLen, "%s %s of %s", prefix, word1, word2);
}

Book generate_book(int id)
{
    int publicationYear = rand() % 35 + 1990; // Random year from 1990 to 2024
    int stock = rand() % 50 + 1;              // Random stock from 1 to 50

    Book book;

    book.bookId = id;
    generateRandomBookTitle(book.title, MAX_TITLE);
    strcpy(book.author, authors[rand() % 14]);
    strcpy(book.genre, genres[rand() % 12]);
    book.publicationYear = publicationYear;
    book.stock = stock;

    return book;
}

Readers generate_reader(int id)
{
    Readers reader;
    reader.readerId = id;

    sprintf(reader.fullName, "%s %s %s", lastnames[rand() % 16], middlenames[rand() % 28], firstnames[rand() % 28]);
    sprintf(reader.phoneNumber, "%s%07d", phone_prefix[rand() % 5], rand() % 10000000);
    strcpy(reader.address, addresses[rand() % 21]);

    return reader;
}

BorrowReturn generate_borrow_return(int readerId)
{
    BorrowReturn borrowReturn;
    borrowReturn.readerId = readerId;

    borrowReturn.totalBooks = 1 + rand() % 5;

    int i;
    for (i = 0; i < borrowReturn.totalBooks; i++)
    {
        BookBorrowInfor infor;
        infor.bookId = 1 + rand() % 50;
        infor.quantity = 1 + rand() % 5;
        infor.status = ON_BORROWING;
        infor.onTime = false;
        infor.date = 1 + rand() % 365;
        infor.current_year = 2023 + rand() % 3;

        borrowReturn.infors[i] = infor;
    }

    return borrowReturn;
}

BorrowReturn default_borrow_return(int readerId)
{
    BorrowReturn borrowReturn;

    borrowReturn.readerId = readerId;
    borrowReturn.totalBooks = 0;

    return borrowReturn;
}