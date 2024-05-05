// Header with CSVReader class.

#ifndef CSV_MANAGER_CSV_READER
#define CSV_MANAGER_CSV_READER


#include <fstream>

#include "./csv_data.hpp"


class CSVReader {
// Reads a CSV file and extracts its data into the CSVData object.
public:


    using vector_s = std::vector<std::string>;


    // CSVReader class attributes and methods.


    // Pointer to the output CSVData object.
    CSVData* output = nullptr;
    // Path to the file and the separator.
    std::string path, sep;
    // Vector for manually entered columns names.
    vector_s columns;
    // Should columns be extracted from the file?
    bool auto_columns = true;


    CSVReader(CSVData& output, std::string path, std::string sep = ",", vector_s columns = {}) :
        output{&output}, path{path}, sep{sep}, columns{columns}
    {
        /* CSVReader constructor.
         * Arguments:
         *     output: The CSVData object in which data will be loaded. It'll be wiped out before reading started.
         *     path: Path to a CSV file from which the reader will read.
         *     sep: The separator used in this CSV file.
         *     columns: Vector with columns of this CSV file. If empty, column information will be extracted from the first row.
         *              If specified, the first row will be read as common row.
         */
        if (columns.size() == 0)
            auto_columns = false;
    }


    CSVData& read_all() {
        // Reads all data from the file and inserts it into the output. Returns reference to the output object.
        std::string line_buffer;
        std::ifstream file(path);

        if (auto_columns) {
            getline(file, line_buffer);
            columns = extract_header(line_buffer);
        }

        output->clear();
        for (auto i : columns) output->add_column(i);

        while (getline(file, line_buffer))
            output->add_row(parse_line(line_buffer));

        return *output;
    }


    vector_s parse_line(std::string line) {
        // Parses one raw CSV data line into a vector and returns it.
        return {""};
    }


    // TO DO: Make it extract columns.
    vector_s extract_header(std::string line) {
        // Parses the CSV file header into the vector with column names and returns it.
        vector_s out_vec;
        std::string str_buf;
        bool quotes = false;

        // TO DO: Make parsing using state design pattern.
        for (auto i : line) {

        }


        return out_vec;
    }


private:






};






#endif
