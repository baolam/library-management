#include "reader.h"
#include "utils/fake.h"

int main()
{
    srand(time(NULL));
    preparate_reader();

    // int totals = 250;
    // int id;
    // for (id = 1; id <= totals; id++)
    // {
    //     Readers reader = generate_reader(id);
    //     add_reader(&reader);
    // }
    // save_reader_management();

    load_reader_management();

    // search_reader_by_id(1);
    // search_reader_by_id(15);
    // search_reader_by_id(251);

    // printf("\n");
    char prefix[100] = "Bui";
    search_reader_by_name(prefix, 5);

    return 0;
}