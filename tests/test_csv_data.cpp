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


void test_of_init_1() {
    // Does CSVData initializing correctly?

    CSVData data({{"message", 0}}, {{"text"}});

    if ((data[0][0] != "text") || (data.get_column_index()["message"] != 0))
        throw std::logic_error("Value was modified.");
}


void test_of_init_2() {
    // Does invalid initialization check works?

    bool bad = false;
    try {CSVData data({{"message", 0}}, {{"text", "2"}}); bad = true;} catch (std::invalid_argument) {}
    try {CSVData data({{"message", 0}, {"2", 1}}, {{"text"}}); bad = true;} catch (std::invalid_argument) {}

    if (bad)
        throw std::logic_error(".delete_row() doesn't prohibit deletion of a non-existing row.");
}


void test_column_1() {
    // Does .column() return references to the original objects from given column?

    CSVData data({{"message", 0}, {"thing", 1}}, {{"text1", "text2"}, {"text3", "text4"}});

    for (auto &i : data.column("thing")) {
        i = "Changed text.";
    }
    // print_1d(data[0]); print_1d(data[1]);

    if (data[0][1] != "Changed text." || data[1][1] != "Changed text.") {
        throw std::logic_error("Iterator doesn't provide direct references.");
    }
}


void test_column_2() {
    // Second part of tests of .column().

    CSVData data({{"message", 0}, {"thing", 1}}, {{"text1", "text2"}, {"text3", "text4"}});

    bool bad = false;
    try {data.column("asdasdadwe"); bad = true;} catch (std::invalid_argument) {}

    if (bad)
        throw std::logic_error(".column() takes a non-existing column name.");

    if (data.column("message")[1] != "text3")
        throw std::logic_error(".column()[] doesn't return the right value.");

}


void test_delete_row_1() {
    // Does .delete_row(1, 2) delete elements correctly?
    CSVData data(
        {{"message", 0}, {"thing", 1}, {"third", 2}},
        {{"text1", "text2", "what"}, {"text3", "text4", "where"},
         {"text5", "text6", "when"}, {"who", "how", "why"}}
    );
    // print_2d(data);
    data.delete_row(1, 2);
    // print_2d(data);
    if (data[0][0] != "text1" || data[1][0] != "who") {
        throw std::logic_error(".delete_row(1, 2) doesn't work as inteded.");
    }
}


void test_delete_row_2() {
    // Does .delete_row(1) delete element correctly?

    CSVData data({{"message", 0}}, {{"text3"}, {"text5"}, {"who"}});

    //print_2d(data);
    data.delete_row(1);
    //print_2d(data);

    if (data[0][0] != "text3" || data[1][0] != "who")
        throw std::logic_error(".delete_row(1) doesn't work as inteded.");

    data.delete_row(1);
}


void test_delete_row_3() {
    // What happens if you try to delete a non-existing row?

    CSVData data({{"message", 0}, {"number", 1}}, {{"text1", "number1"}, {"text2", "number2"}});

    bool bad = false;
    try {data.delete_row(2); bad = true;} catch (std::invalid_argument) {}
    try {data.delete_row(-1); bad = true;} catch (std::invalid_argument) {}
    try {data.delete_row(0, 3); bad = true;} catch (std::invalid_argument) {}
    try {data.delete_row(-1, 3); bad = true;} catch (std::invalid_argument) {}
    //print_2d(data);

    if (bad)
        throw std::logic_error(".delete_row() doesn't prohibit deletion of a non-existing row.");
}


void test_add_row_1() {
    // Test of addition of n rows using .add_row(3).

    CSVData data({{"message", 0}, {"number", 1}, {"thing", 2}, {"yeah", 3}}, {{"1", "2", "3", "4"}});
    std::vector<std::string> blank(4, "");

    data.add_row(3);

    if (data.rows_number() != 4)
        throw std::logic_error(".add_row(3) hasn't added 3 rows.");
    if (data[0][0] != "1")
        throw std::logic_error(".add_row(3) changed first rows.");

    // print_2d(data);
    for (auto begin = data.begin() + 1, end = data.end(); begin != end; ++begin)
        if (*begin != blank)
            throw std::logic_error(".add_row(3) added invalid rows.");
}


void test_add_row_2() {
    // Test of addition of n rows using .add_row(number, row).

    CSVData data({{"message", 0}, {"number", 1}, {"thing", 2}, {"yeah", 3}}, {{"1", "2", "3", "4"}});
    std::vector<std::string> row = {"4", "3", "2", "1"};

    data.add_row({"4", "3", "2", "1"}, 3);

    if (data.rows_number() != 4)
        throw std::logic_error(".add_row(number, row) hasn't added 3 rows.");
    if (data[0][0] != "1")
        throw std::logic_error(".add_row(number, row) changed first rows.");

    // print_2d(data);
    for (auto begin = data.begin() + 1, end = data.end(); begin != end; ++begin)
        if (*begin != row)
            throw std::logic_error(".add_row(number, row) added invalid rows.");
}


void test_add_row_3() {
    // Test of inavlid uses of .add_row().

    CSVData data({{"message", 0}, {"number", 1}, {"3", 2}}, {{"text1", "number1", "1"}, {"text2", "number2", "2"}});

    bool bad = false;
    try {data.add_row({"4", "2"}); bad = true;} catch (std::invalid_argument) {}
    try {data.add_row({"4", "2", "1", "1", "1"}, 3); bad = true;} catch (std::invalid_argument) {}

    if (bad)
        throw std::logic_error(".add_row() doesn't prohibit addition of invalid row.");
}


void test_insert_row_1() {
    // Test of all valid variants of .insert_row() use.

    CSVData::vector_s first = {"1", "2", "3", "4"}, last = {"56", "23", "34", "54"};
    std::vector<std::string> row = {"4", "3", "2", "1"};
    std::vector<std::string> blank(4, "");

    CSVData data({{"message", 0}, {"number", 1}, {"thing", 2}, {"yeah", 3}}, {first, last});

    data.insert_row(1); // 2 blank on 1 index.
    data.insert_row(2, 2); // 2 blanks on 2 index.
    data.insert_row(4, row); // Row on 4 index.
    data.insert_row(5, row, 2); // 2 rows on 4 index.
    data.insert_row(7, {{"4", "21", "53", "24"}, row}); // {"4", "21", "53", "24"} and row on 7 index.
    //print_2d(data);

    if (
        data[0] != first || data[9] != last ||
        data[1] != blank || data[2] != blank || data[3] != blank ||
        data[4] != row || data[5] != row || data[6] != row ||
        data[7] != CSVData::vector_s{"4", "21", "53", "24"} || data[8] != row
    ) throw std::logic_error(".insert_row() isn't working right.");
}


void test_insert_row_2() {
    // Test of inavlid use of .insert_row().

    CSVData data({{"message", 0}, {"number", 1}, {"3", 2}}, {{"text1", "number1", "1"}, {"text2", "number2", "2"}});

    bool bad = false;
    try {data.insert_row(2); bad = true;} catch (std::invalid_argument) {}
    try {data.insert_row(2, 3); bad = true;} catch (std::invalid_argument) {}
    try {data.insert_row(1, {"invalid", "number"}); bad = true;} catch (std::invalid_argument) {}
    try {data.insert_row(1, {"bigger", "and", "invalid", "number"}, 2); bad = true;} catch (std::invalid_argument) {}
    try {data.insert_row(1, {{"valid", "elements", "number"}, {"invalid", "number"}}); bad = true;} catch (std::invalid_argument) {}

    if (bad)
        throw std::logic_error(".insert_row() doesn't prohibit insertion to a non-existing index and insertion of invalid row.");
}


void test_add_column_1() {
    // Test of .add_column() method.

    CSVData data({{"message", 0}}, {{"4"}});

    data.add_column("who", "123");

    if (data.get_column_index()["who"] != 1 || data[0] != CSVData::vector_s{"4", "123"})
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
        data.get_column_index().find("thing") != data.get_column_index().end() ||
        data[0][1] != "2" || data[0][2] != "4" || data[1][1] != "23" || data[1][2] != "54"
    ) throw std::logic_error(".delete_column() Doesn't delete things correctly.");

    bool bad = true;
    try {data.delete_column("thing");}
    catch (std::invalid_argument) {bad = false;}

    if (bad)
        throw std::logic_error(".delete_column() deletes non-existing column.");

}


void test_clear() {
    // Does .clear() clears everything?

    CSVData data;

    data.add_column("Name").add_column("Age").add_column("Gender");
    data.add_row({{"Grag", "32", "m"}, {"Jane", "21", "f"}});

    if (data.rows_number() != 2 || data.columns_number() != 3)
        throw std::logic_error("Something wrong with other elements.");

    data.clear();

    if (data.rows_number() != 0 || data.columns_number() != 0)
        throw std::logic_error(".clear() doesn't remove everything.");




}


int main() {
    // Runs all tests. If terminates without exceptions - everything good.

    test_of_init_1();
    test_of_init_2();

    test_column_1();
    test_column_2();

    test_delete_row_1();
    test_delete_row_2();
    test_delete_row_3();

    test_add_row_1();
    test_add_row_2();
    test_add_row_3();

    test_insert_row_1();
    test_insert_row_2();

    test_add_column_1();

    test_delete_column();

    test_clear();

    return 0;
}
