#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <cassert>

#include "../csv_encoder.hpp"


using vector_s = std::vector<std::string>;
using vector_v_s = std::vector<vector_s>;



template <typename T> void print_1d(T seq) {
    // Prints the content of a one dimensional sequence.
    for (auto &i : seq) std::cout << i << ", "; std::cout << " \\n\n";
}


template <typename T> void print_2d(T seq) {
    // Prints the content of a two dimensional sequence.
    for (auto &i : seq) {
        print_1d(i);
    }
    std::cout << "\n";
}


void check_correctness(CSVEncoder<vector_v_s::iterator> &encoder, const vector_s &correct,
                       const std::string &message = "The encoder doesn't work right.") {
    // Method for comparison of the encoder output and the correct one.
    vector_s output;

    for (auto i : encoder) {
        output.push_back(i);
    }

    if (output != correct) {

        std::cout << "\nIncorrect value:\n\nOutput:\n";
        print_1d(output);
        std::cout << "Correct:\n";
        print_1d(correct);
        std::cout << "Sizes: " << output.size() << " " << correct.size() << "\n\n";

        throw std::logic_error(message);
    }
}


void test_simple_1() {
    // Simple test number one.

    vector_v_s source = {{"val_1<|>", "\"val_2", "val\n3"}, {"v\ral_4", "val_5", "val_6"}};
    vector_s correct = {"\"val_1<|>\"<|>\"\"\"val_2\"<|>\"val\n3\"", "\"v\ral_4\"<|>val_5<|>val_6"};

   CSVEncoder<vector_v_s::iterator> encoder(source.begin(), source.end(), "<|>");

   check_correctness(encoder, correct);
}


void test_iter_postfix() {
    // Does it hold the previous value?
    vector_v_s source = {{"First"}, {"Second"}};

    CSVEncoder<vector_v_s::iterator> encoder(source.begin(), source.end());

    auto begin = encoder.begin();
    auto begin_prev = begin++;

    if (*begin != "Second")
        throw std::logic_error("The original iterator hasn't been incremented.");
    if (*begin_prev != "First")
        throw std::logic_error("The value of a previous iterator isn't previous.");
}


void test_empty_iterable() {
    //

}


void test_empty_row() {
    //

}


void test_empty_field() {
    //

}



int main() {

    test_simple_1();

    test_iter_postfix();

    test_empty_iterable();

    test_empty_row();

    test_empty_field();

    return 0;
}
