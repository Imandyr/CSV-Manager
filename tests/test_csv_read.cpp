// Tests for csv_reader.hpp


#include <iostream>
#include <stdexcept>

#include "../csv_reader.hpp"


void test_init_1() {
    // Test of initialization.
    CSVData data;
    CSVReader reader(data, "assets/file_1.csv", "|");

    data.add_column("column 1").add_row({"row 1"});

    if ((*reader.output)[0][0] != "row 1")
        throw std::logic_error("\"output\" doesn't contain a pointer to the original CSVData object.");
}


void test_line_iterator() {
    // Test of the LineIterator work.

    std::ifstream file("assets/file_2.csv");
    CSVReader::LineIterator(file);



}


void test_extract_header() {
    // Test of the extract_header work.



}





int main() {

    test_init_1();

    test_extract_header();

    return 0;
}
