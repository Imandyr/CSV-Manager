// Tests for csv_parser.hpp.
// Some of them are taken from: https://github.com/max-mapper/csv-spectrum


#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

#include "../csv_parser.hpp"


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


void check_correctness(CSVParser<vector_s::iterator> &parser, const vector_v_s &correct,
                       const std::string &message = "The parser doesn't work right.") {
    // Method for comparison of the parser output and the correct one.
    vector_v_s output;

    while(!++parser)
        output.push_back(*parser);

    if (output != correct) {

        std::cout << "\nIncorrect value:\n\nOutput:\n";
        print_2d(output);
        std::cout << "Correct:\n";
        print_2d(correct);
        std::cout << "Sizes: " << output.size() << " " << correct.size() << "\n\n";

        throw std::logic_error(message);
    }
}


void test_init_1() {
    // Test of an incorrect initialization.

    vector_s input;
    bool bad = true;

    try {CSVParser<vector_s::iterator> parser(input.begin(), input.end(), "|\"'", '\"');}
    catch (std::invalid_argument) {bad = false;}

    if (bad)
        throw std::logic_error("Parser mustn't be able to have the quote character as a part of the delimiter.");


}


void test_parse_1() {
    // Test with some parsing cases.

    vector_s input = {"first<|>line\"", "second<|>line\"", "third<|>line<|>some\"\"\"<|>\"text"};
    std::vector<vector_s> target = {{"first", "linesecond<|>line"}, {"third", "line", "some\"<|>text"}};
    std::vector<vector_s> out;

    CSVParser<vector_s::iterator> iter(input.begin(), input.end(), "<|>", '"');

    check_correctness(iter, target);
}


void test_comma_in_quotes() {
    // Does it work properly with enclosed commas?.

    vector_s input = {"first,last,address,city,zip",
                      "John,Doe,120 any st.,\"Anytown, WW\",08123,\"what is,,,,the problem?\""};
    vector_v_s correct = {{"first", "last", "address", "city", "zip"},
                          {"John", "Doe", "120 any st.", "Anytown, WW", "08123", "what is,,,,the problem?"}};

    CSVParser<vector_s::iterator> parser(input.begin(), input.end());

    check_correctness(parser, correct, "The parser mustn't delimit text in quotes.");
}


void test_empty() {
    // Does it work properly with empty fields?.

    vector_s input = {"a,b,c",
                      "1,\"\",\"\"",
                      "2,3,4"};
    vector_v_s correct = {{"a", "b", "c"},
                          {"1", "", ""},
                          {"2", "3", "4"}};

    CSVParser<vector_s::iterator> parser(input.begin(), input.end());

    check_correctness(parser, correct, "The parser doesn't work right with empty fields.");
}


void test_escaped_quotes() {
    // Are quotes escaped correctly?

    vector_s input = {"a,b",
                      "1,'ha ''ha'' ha'",
                      "3,4"};
    vector_v_s correct = {{"a", "b"},
                          {"1", "ha 'ha' ha"},
                          {"3", "4"}};

    CSVParser<vector_s::iterator> parser(input.begin(), input.end(), ",", '\'');

    check_correctness(parser, correct, "Quotes aren't escaped correctly.");
}


void test_json() {

    vector_s input = {"key,val",
                      "1,\"{\"\"type\"\": \"\"Point\"\", \"\"coordinates\"\": [102.0, 0.5]}\""};
    vector_v_s correct = {{"key", "val"},
                          {"1", "{\"type\": \"Point\", \"coordinates\": [102.0, 0.5]}"}};

    CSVParser<vector_s::iterator> parser(input.begin(), input.end());

    check_correctness(parser, correct);
}


void test_newlines() {

    vector_s input = {"a,,b,,c",
                      "1,,2,,3,",
                      "\"Once upon ",
                      "a time\",,5,,,6",
                      "7,,8,,9"};
    vector_v_s correct = {{"a", "b", "c"},
                          {"1", "2", "3,"},
                          {"Once upon a time", "5", ",6"},
                          {"7", "8", "9"}};

    CSVParser<vector_s::iterator> parser(input.begin(), input.end(), ",,");

    check_correctness(parser, correct);
}


void test_quotes_and_newlines() {

    vector_s input = {"a,b",
                      "1,|ha ",
                      "||ha|| ",
                      "ha|",
                      "3,4"};
    vector_v_s correct = {{"a", "b"},
                          {"1", "ha |ha| ha"},
                          {"3", "4"}};

    CSVParser<vector_s::iterator> parser(input.begin(), input.end(), ",", '|');

    check_correctness(parser, correct);
}


void test_simple() {

    vector_s input = {"ajfirtbjfirtc",
                      "1jfirt2jfirt3"};
    vector_v_s correct = {{"a", "b", "c"},
                          {"1", "2", "3"}};

    CSVParser<vector_s::iterator> parser(input.begin(), input.end(), "jfirt");

    check_correctness(parser, correct);
}


void test_utf_8() {

    vector_s input = {"a,b,c",
                      "1,2,3",
                      "4,5,ʤ"};
    vector_v_s correct = {{"a", "b", "c"},
                          {"1", "2", "3"},
                          {"4", "5", "ʤ"}};

    CSVParser<vector_s::iterator> parser(input.begin(), input.end());

    check_correctness(parser, correct);
}


int main() {

    test_init_1();

    test_parse_1();

    test_comma_in_quotes();

    test_empty();

    test_escaped_quotes();

    test_json();

    test_newlines();

    test_quotes_and_newlines();

    test_simple();

    test_utf_8();


    return 0;
}
