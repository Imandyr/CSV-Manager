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

    for (csvm::CSVReader::FileIterator begin(file), end; begin != end; ++begin)
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

    csvm::CSVReader::FileIterator begin(file), end;
    for (auto i : csvm::CSVParser<csvm::CSVReader::FileIterator>(begin, end, " "))
        output.push_back(i);

    file.close();

    if (target != output)
        throw std::logic_error("CSVReader::FileIterator doesn't work with CSVParser.");
}


void test_extract_header() {
    // Does header get extracted correctly?
    csvm::CSVData data;
    csvm::CSVReader reader(data, current_dir + "/assets/file_3.csv", "|");
    reader.close();

    auto index = data.get_column_index();
    if (index["first_name"] != 0 || index["middle_name"] != 1 || index["last_name"] != 2 || index.size() != 3)
        throw std::logic_error("CSVReader doesn't read the header from a file properly.");
}


void test_read_all() {
    // Test of the whole file read.
    csvm::CSVData target;
    target.add_column("first_name").add_column("middle_name").add_column("last_name");
    target.add_row({{"Roxie", "Marcellus", "Marroguin"},
                    {"Faith", "Bernard", "Millson"},
                    {"Nina", "Christa", "Haddan"},
                    {"Remedios", "Claretha", "Haddan"}});

    csvm::CSVData output;
    csvm::CSVReader reader(output, current_dir + "/assets/file_3.csv", "|");
    reader.read_all();
    // Reader doesn't need to be closed after the .read_all() call.

    if (target != output)
        throw std::logic_error("CSVReader doesn't read the file content properly.");
}


void test_read_line() {
    // Test of the one line read.
    csvm::CSVData target;
    target.add_column("first_name").add_column("middle_name").add_column("last_name");
    target.add_row({{"Roxie", "Marcellus", "Marroguin"},
                    {"Faith", "Bernard", "Millson"}});

    csvm::CSVData output;
    csvm::CSVReader reader(output, current_dir + "/assets/file_3.csv", "|");
    reader.read_line().read_line().close();

    if (target != output)
        throw std::logic_error("CSVReader doesn't read lines from a file properly.");
}


void test_invalid_field_number() {
    // It should work with rows that have less or more fields than in the header.
    csvm::CSVData target;
    target.add_column("first_name").add_column("middle_name").add_column("last_name");
    target.add_row({{"Roxie", "Marcellus", ""},
                    {"Faith", "", ""},
                    {"Nina", "Christa", "Haddan"},
                    {"Remedios", "Claretha", "Haddan"}});

    csvm::CSVData output;
    csvm::CSVReader reader(output, current_dir + "/assets/file_4.csv", "|");
    reader.read_all();

    if (target != output)
        throw std::logic_error("CSVReader should add blank strings, if a row has fewer fields than the header, and cut excessive fields, if a row has more fields than in the header.");
}


void test_repeat_read_all() {
    // .read_all() may be called only once.
    csvm::CSVData output;
    csvm::CSVReader reader(output, current_dir + "/assets/blank_1.csv");
    reader.read_all();

    try {
        reader.read_all();
        throw std::logic_error("The second call to .read_all() must throw an exception.");
    }

    catch (std::runtime_error) {}
}


void test_blank_file() {
    // It should work fine with a blank file.
    csvm::CSVData output;
    output.add_column("text").add_row({"something"});

    csvm::CSVReader reader(output, current_dir + "/assets/blank_1.csv");
    reader.read_all();

    if (csvm::CSVData() != output)
        throw std::logic_error("A blank file must make blank CSVData.");
}


void test_fixed_columns() {
    // CSVReader's columns can be specified manually.
    csvm::CSVData output, target;
    target.add_column("first").add_column("second");
    target.add_row({{"first_name", "middle_name"},
                    {"Roxie", "Marcellus"},
                    {"Faith", ""},
                    {"Nina", "Christa"},
                    {"Remedios", "Claretha"}});

    csvm::CSVReader reader(output, current_dir + "/assets/file_4.csv", "|", '"', {"first", "second"});
    reader.read_all();

    if (target != output)
        throw std::logic_error("CSVReader's should work with manually specified columns.");
}


int main() {

    test_extract_header();

    test_file_iterator();

    test_file_iterator_with_parser();

    test_read_all();

    test_read_line();

    test_invalid_field_number();

    test_repeat_read_all();

    test_blank_file();

    test_fixed_columns();

    return 0;
}
