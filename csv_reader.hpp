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


    class FileIterator {
    // Reads and iterates the input file stream line by line.
    // Doesn't close it at the end, so you should do it yourself after.
    public:

        FileIterator(std::ifstream& input) : input{&input} {
            // Creates a valid FileIterator object, that can be used to read lines.
            ++*this;
        }
        FileIterator() {
            // Creates an invalid FileIterator object, that only can be used on the right side of comparison with a valid FileIterator object.
        }

        std::string operator*() {
            // Returns current line.
            return line;
        }

        FileIterator& operator++() {
            // Gets a new line and returns self.
            getline(*input, line);
            return *this;
        }

        FileIterator operator++(int) {
            // Gets a new line and returns a copy of self from before it.
            auto copy = *this;
            getline(*input, line);
            return copy;
        }

        bool operator==(FileIterator other) {
            return static_cast<bool>(*input) == static_cast<bool>(other.input);
        }

        bool operator!=(FileIterator other) {
            return static_cast<bool>(*input) != static_cast<bool>(other.input);
        }

        operator bool() {
            return static_cast<bool>(*input);
        }

    private:
        // The pointer to the input stream.
        std::ifstream* input = nullptr;
        // Current line of the file.
        std::string line;
    };


    // Pointer to the output CSVData object.
    CSVData* output;


    CSVReader(CSVData& output, std::string path, std::string sep = ",", vector_s columns = {}) :
        output{&output}, path{path}, sep{sep}, columns{columns} {
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
        return {""};
    }





};






#endif
