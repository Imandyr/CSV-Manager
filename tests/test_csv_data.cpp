#include <iostream>
#include <string>
#include <stdexcept>

#include "../csv_data.hpp"


template <typename T> void print_1d(T seq) {for (auto &i : seq) std::cout << i << ", "; std::cout << "\n";}


void test_of_init() {
    CSVData data({{"message", 0}}, {{"text"}});
    if ((data.values[0][0] != "text") || (data.column_index["message"] != 0)) throw std::logic_error("Value was modified.");
}


void test_of_column_values() {
    CSVData data({{"message", 0}, {"thing", 1}}, {{"text1", "text2"}, {"text3", "text4"}});
    auto out = data.column_values("thing");
    data.values[0][1] = "new text";
    std::vector<std::string> example = {"new text", "text4"};
    print_1d(example); print_1d(out); print_1d(data.values[0]);
    if (out != example) throw std::logic_error("Something wrong with .column_values().");
}


int main() {

    test_of_init();
    test_of_column_values();

    return 0;
}
