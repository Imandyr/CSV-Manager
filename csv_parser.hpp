// Header with CSVParser class.

#ifndef CSV_MANAGER_CSV_PARSER
#define CSV_MANAGER_CSV_PARSER


#include <vector>
#include <string>


template <typename AnyIter> class CSVParser {
// A parser for the CSV format.

public:

    using vector_s = std::vector<std::string>;
    using size_type = vector_s::size_type;


    // Member classes.


    class Iterator {
    // Iterator which parser a one CSV row with on each iterator.

    public:
        // Start and end iterators for the string iteration.
        AnyIter source_start;
        AnyIter source_end;
        // Current row.
        vector_s row;
        // Did it end.
        bool ended = false;
        // Delimiter and its size.
        const std::string delimiter;
        const size_type delimiter_size = delimiter.size();

        Iterator(AnyIter start, AnyIter end, std::string sep) : source_start{start}, source_end{end}, delimiter{sep} {
            // A normal initialization.
        }
        Iterator(bool ended = true) : ended{ended} {
            // An iterator of the end.
        }

        Iterator& operator++() {
            // Makes one iteration and returns the reference to the self.
            next();
            return *this;
        }

        Iterator operator++(int) {
            // Makes one iteration and returns a copy of self from before the iteration.
            auto copy = *this;
            next();
            return copy;
        }

        vector_s operator*() {
            // Returns current row value.
            return row;
        }

        bool operator==(const Iterator& right) {
            return ended == right.ended;
        }
        bool operator!=(const Iterator& right) {
            return ended != right.ended;
        }

    private:

        // Pointer to the current state.
        void (Iterator::*current_state) (char) = &Iterator::normal_state;
        // Variables.
        std::string buffer;
        size_type delim_count = 0;
        char last = 0;
        bool no_comment = true;

        vector_s next() {
            // Makes one iteration of the source_start content parsing and returns returns current row.

            row = {};
            buffer = "";
            delim_count = 0;
            last = 0;

            while (source_start != source_end) {
                for (char i : *source_start++) {
                    (this->*current_state)(i);
                    last = i;
                }

                if (no_comment) {
                    row.push_back(buffer);
                    break;
                }
            }

            return row;
        }

        void normal_state(char c) {
            if (c == '\"') {
                current_state = &Iterator::comment_double_quote_state;
                no_comment = false;
                return;
            }

            if (c == '\'') {
                current_state = &Iterator::comment_single_quote_state;
                no_comment = false;
                return;
            }

            buffer += c;

            if (c == delimiter[delim_count])
                if (++delim_count == delimiter_size) {
                    std::string out;
                    for (auto begin = buffer.begin(), end = buffer.end() - delimiter_size; begin != end; ++begin)
                        out += *begin;
                    row.push_back(out);
                    buffer = "";
                    delim_count = 0;
                }
        }

        void comment_double_quote_state(char c) {
            if (c == '\"') {
                current_state = &Iterator::normal_state;
                no_comment = true;
                return;
            }
            buffer += c;

        }

        void comment_single_quote_state(char c) {
//             if (c == '\'') {
//                 current_state = &Iterator::normal_state;
//                 no_comment = true;
//                 return;
//             }
        }


    };



};




#endif
