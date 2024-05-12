// Header with CSVParser class.

#ifndef CSV_MANAGER_CSV_PARSER
#define CSV_MANAGER_CSV_PARSER


#include <vector>
#include <string>
#include <stdexcept>


template <typename IterStr> class CSVParser {
/* CSVParser is an iterator, parses lines from the IterStr iterator, and yields one parsed CSV row as std::vector<std::string> on each iteration.
 * This parser doesn't recognize the header and doesn't add or delete any fields. It simply converts one record from plain text into the vector of strings.
 * IterStr must be some type of iterator that yields std::string objects, which represent one line of a CSV file.
 * On construction takes IterStr iterators of the begin and end. The begin will be iterated until begin == end.
 * Attributes shouldn't be change by outer actors after the object creation.
 */

public:
    // Type aliases.
    using vector_s = std::vector<std::string>;
    using size_type = vector_s::size_type;

    // Begin and end iterators for the line iteration.
    IterStr source_begin;
    IterStr source_end;

    // Current row.
    vector_s row;

    // Delimiter and its size.
    const std::string delimiter;
    const size_type delimiter_size = delimiter.size();

    // Quote character.
    const char quote = '\"';

    // Did it end.
    bool ended = false;


    CSVParser(IterStr begin, IterStr end, std::string del = ",", char quote = '\"') : source_begin{begin}, source_end{end}, delimiter{del}, quote{quote} {
        // A normal initialization. Takes begin and end line iterators, the field delimiter, and a character that will be used as a quote.
        // The delimiter can have a length of more than one character. The quote character can't be used into the delimiter.
        if (del.find(quote) != std::string::npos)
            throw std::invalid_argument("The quote can't be part of the delimiter.");
    }

    CSVParser(bool ended = true) : ended{ended} {
        // Initialization for the end iterator. You shouldn't iterate it.
    }

    CSVParser& operator++() {
        // Makes one iteration and returns the reference to the self.
        next();
        return *this;
    }

    CSVParser operator++(int) {
        // Makes one iteration and returns a copy of self from before the iteration.
        auto copy = *this;
        next();
        return copy;
    }

    vector_s operator*() const {
        // Returns current row value.
        return row;
    }

    operator bool() const {
        return ended;
    }

    bool operator==(const CSVParser& right) const {
        return ended == right.ended;
    }
    bool operator!=(const CSVParser& right) const {
        return ended != right.ended;
    }


private:
    // The pointer to the current state of the parser.
    void (CSVParser::*current_state) (char) = &CSVParser::normal_state;

    // The pointer to the enclosed state.
    void (CSVParser::*enclosed_state_pointer) (char) = &CSVParser::enclosed_state;

    // The field buffer.
    std::string buffer;

    //The count of sequential delimiter matches.
    size_type delim_count = 0;


    void next() {
        // next() iterates string lines from source_start, and parses characters from this lines one by one.
        // If at the end of the line, text is in enclosed state, the next line is considered as part of the same record.
        // If line ends and text is in enclosed state, then goes break of iteration.
        // This parser works by using different state member functions.

        // Reset all values.
        current_state = &CSVParser::normal_state;
        delim_count = 0;
        buffer = "";
        row = {};

        // While the iterator of lines is not ended.
        while (source_begin != source_end) {
            // Iteration of characters in a line and increment of the iterator of lines.
            for (char i : *source_begin++) {
                // Use of the current state function on this character.
                (this->*current_state)(i);
            }

            // If record has ended.
            if (current_state != enclosed_state_pointer) {
                // Push the content of the last field into the row and stop the execution.
                row.push_back(buffer);
                return;
            }
        }

        // If source_begin == source_end, then it is the end.
        ended = true;
    }


    void use_full_delimiter() {
        // Ends the current field and starts a new one. Should be used when the delimiter is full.
        std::string out;

        for (auto begin = buffer.begin(), end = buffer.end() - delimiter_size; begin != end; ++begin)
            out += *begin;
        row.push_back(out);

        buffer = "";
        delim_count = 0;
    }


    void normal_state(char c) {
        // State of a character parsing when the text is not enclosed.

        // Goes into the enclosed state if an enclosure has occurred.
        if (c == quote) {
            current_state = &CSVParser::enclosed_state;
            return;
        }

        // Adds a character to the filed text buffer.
        buffer += c;

        // Goes into the delimiter state if the char is a part of the delimiter.
        if (c == delimiter[0]) {
            ++delim_count;
            current_state = &CSVParser::delimiter_state;
        }

    }


    void delimiter_state(char c) {
        // State that judges if this is the full delimiter or just a part of it. If yes, ends this field and starts a new one.
        // If no, then increases or resets the delimiter counter.

        // Splits the line if the delimiter count is full and goes into the normal state.
        if (delim_count == delimiter_size) {
            use_full_delimiter();
            current_state = &CSVParser::normal_state;
            normal_state(c);
            return;
        }

        else {
            // Adds a character to the filed text buffer.
            buffer += c;

            // Increases delimiter count if this char is a part of the delimiter.
            if (c == delimiter[delim_count])
                ++delim_count;

            // else resets and goes into the normal state.
            else {
                delim_count = 0;
                current_state = &CSVParser::normal_state;
            }
        }
    }


    void enclosed_state(char c) {
        // State of character parsing when the text is currently enclosed.
        // Goes into the quote escape state if the char is a quote, else appends it to the field buffer.

        if (c == quote) {
            current_state = &CSVParser::quote_escape_state;
            return;
        }
        else
            buffer += c;
    }


    void quote_escape_state(char c) {
        // State that arises when a quote occurs inside the enclosed state.
        // If this char is a quote too, then it's the quote escape. If not, then it's the end of an enclosure.

        buffer += c;

        if (c == quote)
            current_state = &CSVParser::enclosed_state;

        else if (c == delimiter[0]) {
            ++delim_count;
            current_state = &CSVParser::delimiter_state;
        }

        else
            current_state = &CSVParser::normal_state;
    }


};




#endif
