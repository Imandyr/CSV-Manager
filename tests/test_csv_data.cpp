// Tests for csv_data.hpp.


#include <iostream>
#include <string>
#include <stdexcept>

#include "../csv_data.hpp"



template <typename T> void print_1d(T seq) {
    // Prints content of one dimensional sequence.
    for (auto &i : seq) std::cout << i << ", "; std::cout << "\n";
}
template <typename T> void print_2d(T seq) {
    // Prints content of two dimensional sequence.
    for (auto &i : seq) {
        print_1d(i);
    }
    std::cout << "\n";
}


void test_of_init() {
    // Does CSVData initializing correctly?

    CSVData data({{"message", 0}}, {{"text"}});

    if ((data.values[0][0] != "text") || (data.column_index["message"] != 0))
        throw std::logic_error("Value was modified.");
}


void test_column() {
    // Does .column() return references to the original objects from given column?

    CSVData data({{"message", 0}, {"thing", 1}}, {{"text1", "text2"}, {"text3", "text4"}});

    for (auto &i : data.column("thing")) {
        i = "Changed text.";
    }
    // print_1d(data.values[0]); print_1d(data.values[1]);

    if (data.values[0][1] != "Changed text." || data.values[1][1] != "Changed text.") {
        throw std::logic_error("Iterator doesn't provide direct references.");
    }
}


void test_delete_row_1() {
    // Does .delete_row(1, 2) delete elements correctly?
    CSVData data(
        {{"message", 0}, {"thing", 1}, {"third", 2}},
        {{"text1", "text2", "what"}, {"text3", "text4", "where"},
         {"text5", "text6", "when"}, {"who", "how", "why"}}
    );
    // print_2d(data.values);
    data.delete_row(1, 2);
    // print_2d(data.values);
    if (data.values[0][0] != "text1" || data.values[1][0] != "who") {
        throw std::logic_error(".delete_row(1, 2) doesn't work as inteded.");
    }
}


void test_delete_row_2() {
    // Does .delete_row(1) delete element correctly?

    CSVData data({{"message", 0}}, {{"text3"}, {"text5"}, {"who"}});

    //print_2d(data.values);
    data.delete_row(1);
    //print_2d(data.values);

    if (data.values[0][0] != "text3" || data.values[1][0] != "who")
        throw std::logic_error(".delete_row(1) doesn't work as inteded.");

    data.delete_row(1);
}


void test_delete_row_3() {
    // What happens if you try to delete a non-existing row?
}


void test_add_row_1() {
    // Test of addition of n rows using .add_row(3).

    CSVData data({{"message", 0}, {"number", 1}, {"thing", 2}, {"yeah", 3}}, {{"1", "2", "3", "4"}});
    std::vector<std::string> blank(4, "");

    data.add_row(3);

    if (data.values.size() != 4)
        throw std::logic_error(".add_row(3) hasn't added 3 rows.");
    if (data.values[0][0] != "1")
        throw std::logic_error(".add_row(3) changed first rows.");

    // print_2d(data.values);
    for (auto begin = data.begin() + 1, end = data.end(); begin != end; ++begin)
        if (*begin != blank)
            throw std::logic_error(".add_row(3) added invalid rows.");
}


void test_add_row_2() {
    // Test of addition of n rows using .add_row(number, row).

    CSVData data({{"message", 0}, {"number", 1}, {"thing", 2}, {"yeah", 3}}, {{"1", "2", "3", "4"}});
    std::vector<std::string> row = {"4", "3", "2", "1"};

    data.add_row(3, {"4", "3", "2", "1"});

    if (data.values.size() != 4)
        throw std::logic_error(".add_row(number, row) hasn't added 3 rows.");
    if (data.values[0][0] != "1")
        throw std::logic_error(".add_row(number, row) changed first rows.");

    // print_2d(data.values);
    for (auto begin = data.begin() + 1, end = data.end(); begin != end; ++begin)
        if (*begin != row)
            throw std::logic_error(".add_row(number, row) added invalid rows.");
}


void test_insert_row() {
    // Test of all variants of .insert_row().

    CSVData::vector_s first = {"1", "2", "3", "4"}, last = {"56", "23", "34", "54"};
    std::vector<std::string> row = {"4", "3", "2", "1"};
    std::vector<std::string> blank(4, "");

    CSVData data({{"message", 0}, {"number", 1}, {"thing", 2}, {"yeah", 3}}, {first, last});

    data.insert_row(1); // 2 blank on 1 index.
    data.insert_row(2, 2); // 2 blanks on 2 index.
    data.insert_row(4, row); // Row on 4 index.
    data.insert_row(5, row, 2); // 2 rows on 4 index.
    data.insert_row(7, {{"4", "21", "53", "24"}, row}); // {"4", "21", "53", "24"} and row on 7 index.
    //print_2d(data.values);

    if (
        data.values[0] != first || data.values[9] != last ||
        data.values[1] != blank || data.values[2] != blank || data.values[3] != blank ||
        data.values[4] != row || data.values[5] != row || data.values[6] != row ||
        data.values[7] != CSVData::vector_s{"4", "21", "53", "24"} || data.values[8] != row
    ) throw std::logic_error(".insert_row() isn't working right.");
}


void test_add_column() {
    // Test of .add_column() method.

    CSVData data({{"message", 0}}, {{"4"}});

    data.add_column("who", "123");

    if (data.column_index["who"] != 1 || data.values[0] != CSVData::vector_s{"4", "123"})
        throw std::logic_error(".add_column() isn't working right.");

    bool bad = true;
    try {data.add_column("who");}
    catch (std::invalid_argument) {bad = false;}

    if (bad)
        throw std::logic_error(".add_column() isn't detecting rewrite of already existing column.");
}


void test_delete_column() {
    // Test of column deletion.

    CSVData::vector_s first = {"1", "2", "3", "4"}, last = {"56", "23", "34", "54"};
    CSVData data({{"message", 0}, {"number", 1}, {"thing", 2}, {"yeah", 3}}, {first, last});

    data.delete_column("thing");
    // print_2d(data);

    if (
        data.column_index.find("thing") != data.column_index.end() ||
        data[0][1] != "2" || data[0][2] != "4" || data[1][1] != "23" || data[1][2] != "54"
    ) throw std::logic_error(".delete_column() Doesn't delete things correctly.");

    bool bad = true;
    try {data.delete_column("thing");}
    catch (std::invalid_argument) {bad = false;}

    if (bad)
        throw std::logic_error(".delete_column() deletes non-existing column.");

}


int main() {
    // Runs all tests. If terminates without exceptions - everything good.

    test_of_init();

    test_column();

    test_delete_row_1();
    test_delete_row_2();
    test_delete_row_3();

    test_add_row_1();
    test_add_row_2();

    test_insert_row();

    test_add_column();

    test_delete_column();

    return 0;
}
