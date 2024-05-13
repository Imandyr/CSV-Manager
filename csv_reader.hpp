// Header with CSVReader class.

#ifndef CSV_MANAGER_CSV_READER
#define CSV_MANAGER_CSV_READER


#include <fstream>

#include "./csv_data.hpp"
#include "./csv_parser.hpp"


class CSVReader {
// Reads a CSV file and extracts its data into the CSVData object.
//
public:

    using vector_s = std::vector<std::string>;
    using vector_v_s = std::vector<vector_s>;
    using size_type = vector_v_s::size_type;


    // Member types.


    class LineIterator {
    // Reads the input file stream line by line.
    // Doesn't close it, so you should do it by yourself after.
    public:
        friend LineIterator;

        LineIterator(std::ifstream& input) : input{input} {}
        LineIterator() {}

        std::string operator*() {
            // Returns current line.
            return line;
        }

        LineIterator& operator++() {
            // Gets a new line and returns self.
            getline(input, line);
            return *this;
        }

        LineIterator operator++(int) {
            // Gets a new line and returns previous self.
            auto copy = *this;
            getline(input, line);
            return copy;
        }

        bool operator==(LineIterator other) {
            return input == other.input;
        }

        bool operator!=(LineIterator other) {
            return input != other.input;
        }

    private:
        // The input stream.
        std::ifstream& input;
        // Current line of the file.
        std::string line;
    };



    // Pointer to the output CSVData object.
    CSVData* output;


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


private:
    //

    // Path to the file and the separator.
    const std::string path, sep;
    // Vector for manually entered columns names.
    vector_s columns;
    // Should columns be extracted from the file?
    bool auto_columns = true;


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





};






#endif
