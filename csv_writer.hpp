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
//
//
//
public:

    using vector_s = std::vector<std::string>;
    using vector_v_s = std::vector<vector_s>;


    CSVWriter(CSVData &input, std::string path, std::string delimiter = ",", char quote = '"') : input{input},
             path{path}, delimiter{delimiter}, quote{quote}, file{path} {
        // Initialization. Writes header.
        file << get_header_line() + "\n";
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

    CSVEncoder<CSVData::vector_v_s::iterator> encoder{input.begin(), input.end(), delimiter, quote};

    void write_line(std::string line) {
        // Writes line to file.
        file << line + "\n";
    }

    std::string get_header_line() {
        // Get the header line.
        std::vector<CSVData::ColumnNameContainer> h_iter_iter = {input.get_column_index()};
        CSVEncoder<std::vector<CSVData::ColumnNameContainer>::iterator>::Encoder h_encoder(h_iter_iter.begin(), delimiter, quote);
        return h_encoder.encode().string;
    }


};


#endif
