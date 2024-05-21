#ifndef CSV_MANAGER_CSV_WRITER
#define CSV_MANAGER_CSV_WRITER


#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <array>

#include "./csv_encoder.hpp"
#include "./csv_data.hpp"


class CSVWriter {
// Writer which writes content of the CSVData to the file.
// Use .write_all() to write everything.
//
public:

    using vector_s = std::vector<std::string>;
    using vector_v_s = std::vector<vector_s>;


    CSVWriter(CSVData &input, std::string path, std::string delimiter = ",", char quote = '"') : input{input},
             path{path}, delimiter{delimiter}, quote{quote}, file{path} {
        // Initialization. Writes header.
        file << input.get_header_line(delimiter, quote) + "\n";
    }


    CSVWriter& write_all() {
        // Writes all the data from the input into the output file and closes it.
        if (file) {
            for (std::string i : encoder) {
                file << i + "\n";
            }
            close();
        }
        else
            throw std::runtime_error("The file stream is closed.");

        return *this;
    }


    CSVWriter& close() {
        // Closes file.
        file.close();
        return *this;
    }


private:
    // Attributes.
    CSVData &input;
    const std::string path, delimiter;
    const char quote;
    std::ofstream file;
    CSVEncoder<CSVData::vector_v_s::iterator> encoder = input.encode_content(delimiter, quote);
};


#endif
