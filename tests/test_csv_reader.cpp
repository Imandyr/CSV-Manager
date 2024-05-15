// Tests for csv_reader.hpp


#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <string>

#include "../csv_reader.hpp"
#include "../csv_parser.hpp"


using vector_s = std::vector<std::string>;
using vector_v_s = std::vector<vector_s>;


std::string file_path = __FILE__;
std::string current_dir = file_path.substr(0, file_path.find_last_of("/\\"));


void test_init_1() {
    // Test of initialization.
    CSVData data;
    CSVReader reader(data, current_dir + "assets/file_1.csv", "|");

    data.add_column("column 1").add_row({"row 1"});

    if ((*reader.output)[0][0] != "row 1")
        throw std::logic_error("\"output\" doesn't contain a pointer to the original CSVData object.");
}


void test_file_iterator() {
    // Test of the FileIterator work.

    vector_s target = {"Line 1", "Line 2", "Line 3"};
    vector_s output;

    std::ifstream file(current_dir + "/assets/file_2.csv");

    for (CSVReader::FileIterator begin(file), end; begin != end; ++begin)
        output.push_back(*begin);

    file.close();

    if (target != output)
        throw std::logic_error("CSVReader::FileIterator doesn't read file correctly.");

}


void test_file_iterator_with_parser() {
    // The FileIterator and CSVParser together.

    vector_v_s target = {{"Line", "1"}, {"Line", "2"}, {"Line", "3"}};
    vector_v_s output;

    std::ifstream file(current_dir + "/assets/file_2.csv");

    CSVReader::FileIterator begin(file), end;
    for (auto i : CSVParser<CSVReader::FileIterator>(begin, end, " "))
        output.push_back(i);

    file.close();

    if (target != output)
        throw std::logic_error("CSVReader::FileIterator doesn't work with CSVParser.");

}


void test_extract_header() {
    // Test of the extract_header work.



}





int main() {

    test_init_1();

    test_extract_header();

    test_file_iterator();

    test_file_iterator_with_parser();

    return 0;
}
