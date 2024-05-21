#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <cassert>

#include "../csv_writer.hpp"
#include "../csv_reader.hpp"
#include "../csv_encoder.hpp"
#include "../csv_parser.hpp"


using vector_s = std::vector<std::string>;
using vector_v_s = std::vector<vector_s>;


std::string file_path = __FILE__;
std::string current_dir = file_path.substr(0, file_path.find_last_of("/\\"));


void test_write_all_1() {
    // It should write all the data from the CSVData into the file.
    std::string path = current_dir + "/assets/written_1.csv";

    csvm::CSVData data, output;
    data.add_column("Thing_1").add_column("Thing_2");
    data.add_row({{"Field_1", "Field_2"}, {"Field_3", "Field_4"}});

    csvm::CSVWriter writer(data, path, "|", '"');
    writer.write_all();

    csvm::CSVReader reader(output, path, "|", '"');
    reader.read_all();

    if (output != data)
        throw std::logic_error("The written data isn't the same as the input data.");
}


int main() {

    test_write_all_1();

    return 0;
}
