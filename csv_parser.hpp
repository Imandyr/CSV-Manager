// Header with CSVParser class.

#ifndef CSV_MANAGER_CSV_PARSER
#define CSV_MANAGER_CSV_PARSER


#include <vector>
#include <string>


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
        // The delimiter can have a length of more than one character.
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

    vector_s operator*() {
        // Returns current row value.
        return row;
    }

    bool operator==(const CSVParser& right) {
        return ended == right.ended;
    }
    bool operator!=(const CSVParser& right) {
        return ended != right.ended;
    }


private:
    // The pointer to the parser current state.
    void (CSVParser::*current_state) (char) = &CSVParser::normal_state;

    // The field buffer.
    std::string buffer;

    //The count of sequential delimiter matches.
    size_type delim_count = 0;

    // Does text is currently enclosed?
    bool enclosed = false;

    // The previous processed character.
    char previous = 0;


    void next() {
        // next() iterates string lines from source_start, and parses characters from this lines one by one.
        // If at the end of the line, text is in enclosed state, the next line is considered as part of the same record.
        // If line ends and text is in enclosed state, then goes break of iteration.
        // This parser works by using different state member functions.

        // Reset all values.
        row = {};
        buffer = "";
        delim_count = 0;
        previous = 0;

        // While the iterator of lines is not ended.
        while (source_begin != source_end) {
            // Iteration of characters in a line and increment of the iterator of lines.
            for (char i : *source_begin++) {
                // Use of the current state function on this character.
                (this->*current_state)(i);
                // Update of the previous character.
                previous = i;
            }

            // If record has ended.
            if (!enclosed) {
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
            enclosed = true;
            return;
        }

        // Adds a character to the filed text buffer.
        buffer += c;

        // Goes into the delimiter state if this character is the first part of the delimiter, or delimiter right away if the delimiter is a one char.
        if (c == delimiter[0]) {
            ++delim_count;
            if (delim_count == delimiter_size)
                use_full_delimiter();
            else
                current_state = &CSVParser::delimiter_state;
        }
    }


    void delimiter_state(char c) {
        // State that judges if this is the full delimiter or just a part of it. If yes, ends this field and starts a new one.
        // If no, then increases or resets the delimiter counter.

        // Goes into the enclosed state if an enclosure has occurred.
        if (c == quote) {
            current_state = &CSVParser::enclosed_state;
            enclosed = true;
            return;
        }

        // Adds a character to the filed text buffer.
        buffer += c;

        // Increases delimiter count if this char is a part of the delimiter, else resets and goes into the normal state.
        if (c == delimiter[delim_count])
            ++delim_count;
        else {
            delim_count = 0;
            current_state = &CSVParser::normal_state;
        }

        // Splits if full and goes into the normal state.
        if (delim_count == delimiter_size) {
            use_full_delimiter();
            current_state = &CSVParser::normal_state;
        }
    }


    void enclosed_state(char c) {
        // State of character parsing when the text is currently enclosed.

        if (c == quote) {
            current_state = &CSVParser::normal_state;
            enclosed = false;
            return;
        }

        buffer += c;

    }


    // TO DO: An intermediate state between normal and comment, allowing to handle escaped quote inside comment.










};




#endif
