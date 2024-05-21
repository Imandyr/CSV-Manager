# CSV Manager

A bunch of headers with classes to read, parse, contain, encode, and write CSV formatted files.
They can be accesses through the "csvm" namespace.


## Content

### "csv_data.hpp"
csvm::CSVData class, which is used to store content of the CSV file.
Based on std::vector, so has finite maximum element size.

### "csv_parser.hpp"
csvm::CSVParser class, which is used to parse an iterator with std::string elements, which should be lines of a CSV formatted file.

### "csv_encoder.hpp"
csvm::CSVEncoder, which takes an iterator of iterators of std::strings inside, and encodes content of every iterator into the CSV formatted std::string.

### "csv_reader.hpp"
csvm::CSVReader reads, encodes, and places content of the CSV formatted file into the csvm::CSVData object.

### "csv_writer.hpp"
csvm::CSVWriter encodes and writes content from the CSVData object into the file.

### "tests"
Directory that contains tests for all of these classes. If every test exits without errors, then everything working as intended.
Bash script "tests/run_all.sh" compiles and runs all tests using GCC.


## Requirements

The code is working ok on the GCC (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0.
No third-party packages are used.
