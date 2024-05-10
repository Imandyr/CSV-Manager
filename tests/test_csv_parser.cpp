// Tests for csv_parser.hpp.


#include <iostream>
#include <vector>
#include <string>

#include "../csv_parser.hpp"


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

    std::vector<std::string> input = {"first<|>line\"", "second<|>line\"", "third<|>line<|>some\"\"\"<|>\"text"};

    CSVParser<std::vector<std::string>::iterator>::Iterator iter(input.begin(), input.end(), "<|>");

    std::vector<std::vector<std::string>> out;
    for (uint i = 0; i < 3; ++i)
        out.push_back(*iter++);

    print_2d(out);



}


int main() {

    test_parse_1();

    return 0;
}
