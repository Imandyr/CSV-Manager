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
    // Does header get extracted correctly?
    CSVData data;
    CSVReader reader(data, current_dir + "/assets/file_3.csv", "|");
    reader.close();

    auto index = data.get_column_index();
    if (index["first_name"] != 0 || index["middle_name"] != 1 || index["last_name"] != 2 || index.size() != 3)
        throw std::logic_error("CSVReader doesn't read the header from a file properly.");
}


void test_read_all() {
    // Test of the whole file read.
    CSVData target;
    target.add_column("first_name").add_column("middle_name").add_column("last_name");
    target.add_row({{"Roxie", "Marcellus", "Marroguin"},
                    {"Faith", "Bernard", "Millson"},
                    {"Nina", "Christa", "Haddan"},
                    {"Remedios", "Claretha", "Haddan"}});

    CSVData output;
    CSVReader reader(output, current_dir + "/assets/file_3.csv", "|");
    reader.read_all();
    // Reader doesn't need to be closed after the .read_all() call.

    if (target != output)
        throw std::logic_error("CSVReader doesn't read the file content properly.");
}


void test_read_line() {
    // Test of the one line read.
    CSVData target;
    target.add_column("first_name").add_column("middle_name").add_column("last_name");
    target.add_row({{"Roxie", "Marcellus", "Marroguin"},
                    {"Faith", "Bernard", "Millson"}});

    CSVData output;
    CSVReader reader(output, current_dir + "/assets/file_3.csv", "|");
    reader.read_line().read_line();
    reader.close();

    if (target != output)
        throw std::logic_error("CSVReader doesn't read lines from a file properly.");
}





int main() {

    test_extract_header();

    test_file_iterator();

    test_file_iterator_with_parser();

    test_read_all();

    test_read_line();

    return 0;
}
