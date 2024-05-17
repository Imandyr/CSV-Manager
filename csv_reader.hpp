// Header with CSVReader class.

#ifndef CSV_MANAGER_CSV_READER
#define CSV_MANAGER_CSV_READER


#include <fstream>
#include <stdexcept>

#include "./csv_data.hpp"
#include "./csv_parser.hpp"


class CSVReader {
// Reads a CSV file and extracts its data into the CSVData object.
// It reads file only once, so you must create a new instance to read it again.
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
            if (input)
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


    // Attributes and methods.


    // reference to the output CSVData object.
    CSVData& output;


    CSVReader(CSVData& output, std::string path, std::string sep = ",", char quote = '"', vector_s columns = {}) :
        output{output}, path{path}, sep{sep}, quote{quote}, columns{columns}
        {
        /* CSVReader constructor.
         * Arguments:
         *     output: The CSVData object in which data will be loaded. It'll be wiped out before reading started.
         *     path: Path to a CSV file from which the reader will read.
         *     sep: The separator used in this CSV file.
         *     quote: The quote character.
         *     columns: Vector with columns of this CSV file. If empty, column information will be extracted from the first row.
         *              If specified, the first row will be read as common row.
         */
        output.clear();
        extract_header();
    }


    CSVReader& read_all() {
        // Reads all the data from the file and inserts it into the output. Closes the file stream at the end.
        if (file.is_open()) {
            while (parser_iter != parser_iter_end)
                read_line();
            close();
        }
        else
            throw std::runtime_error("The file stream is closed.");

        return *this;
    }


    CSVReader& read_line() {
        // Parses one raw CSV data line into a vector and adds it to the CSVData.
        auto row = *parser_iter++;

        row.resize(column_number, "");
        output.add_row(row);

        return *this;
    }


    CSVReader& close() {
        // Closes input file stream.
        file.close();
        return *this;
    }


private:

    // Path to the file, the separator and the quote.
    std::string path, sep;
    char quote;

    // Vector with columns names.
    vector_s columns;
    vector_s::size_type column_number;

    // input file stream object.
    std::ifstream file{path};

    // input file stream iterator.
    FileIterator file_iter{file}, file_iter_end;

    // The parser object.
    CSVParser<FileIterator> parser{file_iter, file_iter_end, sep, quote};

    // Parser's iterators.
    CSVParser<FileIterator>::Iterator parser_iter = parser.begin(), parser_iter_end = parser.end();


    void extract_header() {
        // Parses the CSV file header and adds its fields as output columns.
        if (columns.size() == 0)
            columns = *parser_iter++;

        for (std::string i : columns)
            output.add_column(i);

        column_number = columns.size();
    }


};





#endif
