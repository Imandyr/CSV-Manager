// Tests for csv_data.hpp.


#include <iostream>
#include <string>
#include <stdexcept>

#include "../csv_data.hpp"


// Prints content of one dimensional sequence.
template <typename T> void print_1d(T seq) {for (auto &i : seq) std::cout << i << ", "; std::cout << "\n";}


void test_of_init() {
    // Does it initializing correctly?
    CSVData data({{"message", 0}}, {{"text"}});
    if ((data.values[0][0] != "text") || (data.column_index["message"] != 0)) throw std::logic_error("Value was modified.");
}


void test_of_column_values() {
    // Does CSVData.column_values() returns values of given column name?
    CSVData data({{"message", 0}, {"thing", 1}}, {{"text1", "text2"}, {"text3", "text4"}});

    auto out = data.column_values("thing");

    std::vector<std::string> example = {"text2", "text4"};

    // print_1d(example); print_1d(out); print_1d(data.values[0]);

    if (out != example) throw std::logic_error("Something wrong with .column_values().");
}


void test_column_iterator() {
    // Does CSVData::ColumnIterator returns references to original objects from given column?
    CSVData data({{"message", 0}, {"thing", 1}}, {{"text1", "text2"}, {"text3", "text4"}});
    for (auto b = data.begin_col_iter("thing"), e = data.end_col_iter("thing"); b != e; ++b) {
        (*b) = "Changed text.";
    }
    // print_1d(data.values[0]); print_1d(data.values[1]);
    if (data.values[0][1] != "Changed text." || data.values[1][1] != "Changed text.") {
        throw std::logic_error("Iterator doesn't provide direct references.");
    }

}


int main() {
    // Runs all tests. If terminates without exceptions - everything good.

    test_of_init();
    test_of_column_values();
    test_column_iterator();

    return 0;
}
