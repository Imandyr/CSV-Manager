// Encoder to CSV format.

#ifndef CSV_MANAGER_CSV_ENCODER
#define CSV_MANAGER_CSV_ENCODER


template <typename IterIterStr> class CSVEncoder {
/* Encodes strings from the iterable of iterables into the CSV format strings.
 * On construction, takes begin and end streams, delimiter string and quote character.
 *
 * begin() creates and returns an Iterator from the start, end() returns an Iterator from the end.
 *
 * Encoder class encodes things, Iterator class iterates encoder.
 *
 */
public:

    // Type aliases.
    using vector_s = std::vector<std::string>;
    using size_type = vector_s::size_type;


    // Member classes.


    class Encoder {
    // The CSV encoder.
    public:
        // Delimiter
        const std::string delimiter;

        // Quote character.
        const char quote;

        // Source iterator.
        IterIterStr source_iter;

        // Current string.
        std::string string;

        Encoder(const IterIterStr &source_iter, std::string del = ",", char quote = '\"') : delimiter{del}, quote{quote},
              source_iter{source_iter} {}

        Encoder& reset(IterIterStr &_source_iter) {
            // Resets encoder with a new iterator.
            source_iter = _source_iter;
            return *this;
        }

        Encoder& next() {
            ++source_iter;
            return *this;
        }

        Encoder& encode() {
            // Encodes one iterable from the iterator and puts result into the string attribute.
            string = "";

            auto iterable = *source_iter;
            auto begin = iterable.begin(), end = iterable.end();

            // Encodes first iterable.
            if (begin != end)
                string += encode_field(*begin++);

            // Encodes the rest.
            while (begin != end)
                string = string + delimiter + encode_field(*begin++);

            return *this;
        }

    private:
        // Characters which should provoke the field enclosed.
        std::string enclose_pattern = static_cast<std::string>("\r\n") + quote;


        std::string encode_field(const std::string &field) {
            // Encodes one field. Encloses in quotes if necessary.

            std::string output = field;

            // Doubles all quotes inside.
            std::string::size_type position = field.find(quote);
            while (position != std::string::npos) {
                output.insert(position, 1, quote);
                position = field.find(quote, position + 1);
            }

            // Adds the quote enclose if necessary.
            if (field.find(delimiter) != std::string::npos || field.find_first_of(enclose_pattern) != std::string::npos)
                output = quote + output + quote;

            return output;
        }

    };


    class Iterator {
    // The Iterator for the Encoder.

    public:

        Iterator(Encoder& encoder) : encoder{encoder}{
            // Takes an Encoder object which it will iterate. Also unends it so it would make at least one iteration.
        }

        Iterator& operator++() {
            // Makes one iteration and returns the reference to the self.
            encoder.next();
            update = true;
            return *this;
        }

        Iterator operator++(int) {
            // Makes one iteration and returns a copy of self from before the iteration.
            // Because a copy of an iterator points to the same Encoder, if you increment it, it would yield a value from the +2 iteration.
            // So this copy is only useful as a temporary container to the previous value.
            if (update)
                string = encoder.encode().string;
            auto copy = *this;
            copy.update = false;
            encoder.next();
            update = true;
            return copy;
        }

        std::string operator*() {
            // Updates if necessary and returns the current string value.
            if (update) {
                string = encoder.encode().string;
                update = false;
            }
            return string;
        }

        bool operator==(const Iterator& right) const {
            return encoder.source_iter == right.encoder.source_iter;
        }
        bool operator!=(const Iterator& right) const {
            return encoder.source_iter != right.encoder.source_iter;
        }

    private:
        // Does the string need to be updated?
        bool update = true;
        // The encoder.
        Encoder &encoder;
        // The last parsed string.
        std::string string;
    };


    // CSVEncoder attributes and methods.


    CSVEncoder(const IterIterStr &begin, const IterIterStr &end, std::string del = ",", char quote = '\"') : source_begin{begin}, source_end{end},
             encoder{begin, del, quote}, encoder_end{end} {
        // The delimiter can have a length of more than one character. The quote character can't be used into the delimiter.

        if (del.find(quote) != std::string::npos)
            throw std::invalid_argument("The quote can't be part of the delimiter.");
    }


    Iterator begin() {
        // Resets the encoder and returns an iterator to it.
        encoder.reset(source_begin);
        return Iterator(encoder);
    }


    Iterator end() {
        // Returns the end iterator.
        return Iterator(encoder_end);
    }


private:
    // Begin and end iterators for the encoder.
    IterIterStr source_begin;
    IterIterStr source_end;

    // The encoder object.
    Encoder encoder;
    Encoder encoder_end;
};








#endif
