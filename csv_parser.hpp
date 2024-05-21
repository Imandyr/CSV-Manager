// Header with CSVParser class.

#ifndef CSV_MANAGER_CSV_PARSER
#define CSV_MANAGER_CSV_PARSER


#include <vector>
#include <string>
#include <stdexcept>


namespace csvm {


template <typename IterStr> class CSVParser {
/* CSVParser returns Iterator from the .begin() and .end(), which uses internal Parser object, which parses lines from the IterStr iterator,
 * and yield one parsed CSV row content as std::vector<std::string> on each iteration.
 *
 * It resets the internal parser object at every .begin() call.
 *
 * This parser doesn't recognize the header and doesn't add or delete any fields. It simply converts one record from plain text into the vector of strings.
 * IterStr must be some type of iterator that yields std::string objects, which represent one line of a CSV file.
 *
 * On construction takes IterStr iterators of the begin and end. The begin will be iterated until begin == end.
 *
 * Attributes shouldn't be change by outer actors after the object creation.
 */

public:

    // Type aliases.
    using vector_s = std::vector<std::string>;
    using size_type = vector_s::size_type;


    // Member classes.


    class Parser {
    // The CSV parser.

    public:
        // Delimiter and its size.
        const std::string delimiter;
        const size_type delimiter_size = delimiter.size();

        // Quote character.
        const char quote = '\"';

        // Did it end.
        bool ended = false;

        // Current row.
        vector_s row;

        Parser(const IterStr &begin, const IterStr &end, std::string del = ",", char quote = '\"') : delimiter{del}, quote{quote},
              source_begin{begin}, source_end{end} {
            // The normal parser construction.
        }


        void reset(IterStr begin) {
            // Resets parser with a new iterator.
            source_begin = begin;
            ended = false;
        }


        void next() {
            // next() iterates string lines from source_start, and parses characters from this lines one by one.
            // If at the end of the line, text is in enclosed state, the next line is considered as part of the same record.
            // If line ends and text is in enclosed state, then goes break of iteration.
            // This parser works by using different state member functions.

            // Reset all values.
            current_state = &Parser::start_state;
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

                // Adds "\r\n" to the buffer, if the end of a line is inside quotes.
                buffer += "\r\n";
            }

            // If source_begin == source_end, then it is the end.
            row.push_back(buffer);
            ended = true;
        }


    private:
        // Begin and end iterators for the line iteration.
        IterStr source_begin;
        IterStr source_end;

        // The pointer to the current state of the parser.
        void (Parser::*current_state) (char) = &Parser::start_state;

        // The pointer to the enclosed state.
        void (Parser::*enclosed_state_pointer) (char) = &Parser::enclosed_state;

        // The field buffer.
        std::string buffer;

        //The count of sequential delimiter matches.
        size_type delim_count = 0;


        void use_full_delimiter() {
            // Ends the current field and starts a new one. Should be used when the delimiter is full.
            std::string out;

            for (auto begin = buffer.begin(), end = buffer.end() - delimiter_size; begin != end; ++begin)
                out += *begin;
            row.push_back(out);

            buffer = "";
            delim_count = 0;
        }


        void first_delimiter_part() {
            // What happens when the first part of the delimiter has appeared.
            // Delimit if the delimiter is already full, and goes into the delimiter state if not yet.
            ++delim_count;

            if (delim_count == delimiter_size) {
                use_full_delimiter();
                current_state = &Parser::start_state;
            }
            else
                current_state = &Parser::delimiter_state;
        }


        void start_state(char c) {
            // State of a character parsing at the start of every new field.

            // Goes into the enclosed state if an enclosure has occurred.
            if (c == quote) {
                current_state = &Parser::enclosed_state;
                return;
            }

            // Adds a character to the filed text buffer.
            buffer += c;

            // Goes into the delimiter state if the char is a part of the delimiter, or delimit right away.
            if (c == delimiter[0])
                first_delimiter_part();

            // Goes into the normal state if nothing happened.
            current_state = &Parser::normal_state;

        }


        void normal_state(char c) {
            // State of a character parsing when there is nothing happening.

            // Adds a character to the filed text buffer.
            buffer += c;

            // Goes into the delimiter state if the char is a part of the delimiter, or delimit right away.
            if (c == delimiter[0])
                first_delimiter_part();
        }


        void delimiter_state(char c) {
            // State that judges if this is the full delimiter or just a part of it. If yes, ends this field and starts a new one.
            // If no, then increases or resets the delimiter counter.

            // Adds a character to the filed text buffer.
            buffer += c;

            // Increases delimiter count if this char is a part of the delimiter.
            if (c == delimiter[delim_count])
                first_delimiter_part();

            // else resets and goes into the normal state.
            else {
                delim_count = 0;
                current_state = &Parser::normal_state;
            }
        }


        void enclosed_state(char c) {
            // State of character parsing when the text is currently enclosed.
            // Goes into the quote escape state if the char is a quote, else appends it to the field buffer.

            if (c == quote) {
                current_state = &Parser::quote_escape_state;
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
                current_state = &Parser::enclosed_state;

            else if (c == delimiter[0])
                first_delimiter_part();

            else
                current_state = &Parser::normal_state;
        }
    };


    class Iterator {
    // The Iterator for the Parser.

    public:
        Iterator(Parser& parser) : parser{&parser}{
            // Takes Parser object which it will iterate. Also unends it so it would make at least one iteration.
            ++*this;
            parser.ended = false;
        }
        Iterator() {
            // This works as an end iterator.
        };

        Iterator& operator++() {
            // Makes one iteration and returns the reference to the self.
            parser->next();
            row = parser->row;
            return *this;
        }

        Iterator operator++(int) {
            // Makes one iteration and returns a copy of self from before the iteration.
            auto copy = *this;
            parser->next();
            row = parser->row;
            return copy;
        }

        vector_s operator*() const {
            // Returns current row value.
            return row;
        }

        operator bool() const {
            return parser->ended;
        }

        bool operator==(const Iterator& right) const {
            return parser->ended != static_cast<bool>(right.parser);
        }
        bool operator!=(const Iterator& right) const {
            return parser->ended == static_cast<bool>(right.parser);
        }

    private:
        // The pointer to the parser.
        Parser *parser = nullptr;
        // The last parsed row.
        vector_s row;
    };


    // CSVParser attributes and methods.


    // Begin and end iterators for the parser.
    IterStr source_begin;
    IterStr source_end;

    // The parser object.
    Parser parser;


    CSVParser(const IterStr &begin, const IterStr &end, std::string del = ",", char quote = '\"') : source_begin{begin}, source_end{end},
             parser{begin, end, del, quote} {
        // The CSVparser constructor. Takes begin and end std::string iterators, the field delimiter, and a character that will be used as a quote.
        // The delimiter can have a length of more than one character. The quote character can't be used into the delimiter.
        // Creates the parser.

        if (del.find(quote) != std::string::npos)
            throw std::invalid_argument("The quote can't be part of the delimiter.");

    }


    Iterator begin() {
        // Resets the parser and returns an iterator to it.
        parser.reset(source_begin);
        return Iterator(parser);
    }


    Iterator end() {
        // Returns the end iterator.
        return Iterator();
    }


private:


};


}


#endif
