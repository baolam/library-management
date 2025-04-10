#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TOTAL_READERS 1000000 // 1 triệu người đọc

int main()
{
    FILE *fptr = fopen("readers.txt", "w");
    if (fptr == NULL)
    {
        printf("Cannot open file!\n");
        return 1;
    }

    srand(time(NULL));

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

    char *phone_prefix[] = {"09", "08", "07", "03", "05"}; // Đầu số hợp lệ tại Việt Nam

    for (int i = 1; i <= TOTAL_READERS; i++)
    {
        char readerId[20];
        char fullName[100];
        char phoneNumber[15];
        char address[100];

        sprintf(readerId, "%06d", i); // Format: READER000001 - READER999999
        sprintf(fullName, "%s %s %s",
                lastnames[rand() % 16],
                middlenames[rand() % 28],
                firstnames[rand() % 28]);

        sprintf(phoneNumber, "%s%07d", phone_prefix[rand() % 5], rand() % 10000000);
        strcpy(address, addresses[rand() % 21]);

        fprintf(fptr, "%s, %s, %s, %s\n", readerId, fullName, phoneNumber, address);

        // Print progress every 1%
        if (i % (TOTAL_READERS / 100) == 0)
        {
            printf("Generated: %d/%d readers\n", i, TOTAL_READERS);
        }
    }

    fclose(fptr);
    printf("Completed! Data saved in 'readers.txt'.\n");

    return 0;
}
