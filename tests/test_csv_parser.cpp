// Tests for csv_parser.hpp.


#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

#include "../csv_parser.hpp"


using vector_s = std::vector<std::string>;


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


void test_parse_1() {
    // Test with some parsing cases.

    vector_s input = {"first<|>line\"", "second<|>line\"", "third<|>line<|>some\"\"\"<|>\"text"};
    std::vector<vector_s> target = {{"first", "linesecond<|>line"}, {"third", "line", "some\"<|>text"}};
    std::vector<vector_s> out;

    CSVParser<vector_s::iterator> iter(input.begin(), input.end(), "<|>");
    CSVParser<vector_s::iterator> end;

    while(iter != end)
        out.push_back(*iter++);

    print_2d(out);
    print_2d(target);

    if (out != target)
        throw std::logic_error("The parser doesn't work right.");
}


int main() {

    test_parse_1();

    return 0;
}
