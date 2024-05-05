// Header with CSVData class.

#ifndef CSV_MANAGER_CSV_DATA
#define CSV_MANAGER_CSV_DATA


#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>




class CSVData {
/* Container of CSV data.
 *
 */
public:

    // Type aliases.
    using vector_s = std::vector<std::string>;
    using vector_v_s = std::vector<vector_s>;
    using index_type = vector_v_s::size_type;
    using u_map_s_st = std::unordered_map<std::string, index_type>;


    // Helper classes.


    class BaseIterator {
    // Base iterator for all CSVData iteators.
    public:
        // Iterator tags.
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = vector_s;
        using pointer = value_type*;
        using reference = value_type&;

        // Real iterator for vector values.
        vector_v_s::iterator iter;

        // Initialization from real iterator.
        BaseIterator(vector_v_s::iterator it) : iter{it} {}

        // Basic interface.
        reference operator*() {return *iter;}
        reference operator->() {return *iter;}
        BaseIterator& operator++() {iter++; return *this;}
        BaseIterator& operator++(int) {auto temp = this; ++(*this); return *temp;}
        bool operator==(BaseIterator& right) {return iter == right.iter;}
        bool operator!=(BaseIterator& right) {return iter != right.iter;}
    };


    class ColumnIterator : public BaseIterator {
    // Iterator for iteration of values of one column.
    public:
        // Returns one std::string.
        using value_type = std::string;
        using pointer = value_type*;
        using reference = value_type&;

        // What values and from which column should it iterate.
        index_type column_index = 0;
        ColumnIterator(vector_v_s::iterator it, index_type col) : BaseIterator(it), column_index{col} {}

        // Retrun value of only one column.
        reference operator*() {return (*iter)[column_index];}
        reference operator->() {return (*iter)[column_index];}
    };


    class Column {
    /* Column represents one column from data. It not stores column values, but provides .begin() and .end() methods,
     * which returns iterators for iteration of values from target column.
     */
    private:
        // Pointer to original data and index of target column.
        vector_v_s* target;
        index_type column_index = 0;

    public:
        // Initialization with pointer to target vector and index of target column.
        Column(vector_v_s* vec, index_type col) : target{vec}, column_index{col} {}

        ColumnIterator begin() {
            // Returns ColumnIterator on the beginning of a values vector.
            return ColumnIterator(target->begin(), column_index);
        }

        ColumnIterator end() {
            // Returns ColumnIterator on the end of a values vector.
            return ColumnIterator(target->end(), column_index);
        }

        ColumnIterator::value_type& operator[](index_type index) {
            // Returns a reference to the element on a given index from the column.
            return (*target)[index][column_index];
        }
    };


    // CSVData class attributes and methods.


    // Table with column names and thier indexes in vector.
    u_map_s_st column_index;
    // Vector with all data.
    vector_v_s values;


    CSVData(const u_map_s_st& c_i = {}, const vector_v_s& v = {}) {
        // Initialization from column table and vector with values.

        column_index = c_i;

        is_sequence_of_rows_valid(v);

        values = v;
    }


    Column column(std::string name) {
        // Creates Column object of column with target name.

        is_column_not_exist(name);

        return Column(&values, column_index[name]);
    }


    vector_v_s::iterator begin() {
        return values.begin();
    }
    vector_v_s::iterator end() {
        return values.end();
    }
    vector_s operator[](index_type index) {
        return values[index];
    }
    Column operator[](std::string name) {
        return column(name);
    }


    u_map_s_st::size_type columns_number() {
        return column_index.size();
    }
    index_type rows_number() {
        return values.size();
    }


    CSVData& delete_row(index_type index) {
        // Deletes row on given index. Raises std::invalid_argument if the row index is out of bounds.

        is_row_index_valid(index);

        values.erase(values.begin() + index);

        return *this;
    }

    CSVData& delete_row(index_type from, index_type number) {
        // Deletes number of rows after "from" index (including "from"). Raises std::invalid_argument if the row indexes is out of bounds.

        is_row_index_valid(from);

        if (from + number > values.size())
            throw std::invalid_argument("Invalid row deletion range between \"" + std::to_string(from) +
                                        "\" and \"" + std::to_string(from) + " + " + std::to_string(number) + "\".");

        auto start = values.begin() + from;
        values.erase(start, start + number);

        return *this;
    }


    CSVData& add_row(index_type number) {
        // Adds "number" blank rows at the end of the values vector.

        vector_s blank(column_index.size(), "");

        for (index_type i = 0; i < number; ++i)
            values.push_back(blank);

        return *this;
    }

    CSVData& add_row(const vector_s& row) {
        // Adds "row" at the end of the values vector.
        is_row_valid(row);

        values.push_back(row);

        return *this;
    }

    CSVData& add_row(const vector_s& row, index_type number) {
        // Adds "number" rows with value "row" at the end of the values vector.
        is_row_valid(row);

        for (index_type i = 0; i < number; ++i)
            values.push_back(row);

        return *this;
    }

    CSVData& add_row(const std::initializer_list<vector_s>& rows) {
        // Adds rows from list of rows to the end of the values vector.
        is_sequence_of_rows_valid(rows);

        for (auto& row : rows)
            values.push_back(row);

        return *this;
    }


    CSVData& insert_row(index_type to, index_type number = 1) {
        // Inserts number of blank rows on index before "to".

        is_row_index_valid(to);

        values.insert(values.begin() + to, number, vector_s(column_index.size(), ""));

        return *this;
    }

    CSVData& insert_row(index_type to, const vector_s& row, index_type number = 1) {
        // Inserts number of rows with given value on index before "to".

        is_row_index_valid(to);
        is_row_valid(row);

        values.insert(values.begin() + to, number, row);

        return *this;
    }

    CSVData& insert_row(index_type to, const std::initializer_list<vector_s>& rows) {
        // Inserts rows on index of "to".

        is_row_index_valid(to);
        is_sequence_of_rows_valid(rows);

        values.insert(values.begin() + to, rows);

        return *this;
    }


    CSVData& add_column(std::string name, std::string value = "") {
        // Adds a column at the end with the name "name" and appends the value to every row, so a number of columns and elements matches.
        // Raises std::invalid_argument if the column name is already exist.

        is_column_already_exist(name);

        _add_column(name, value);

        return *this;
    }


    CSVData& delete_column(std::string name) {
        // Deletes column and all its values. Raises std::invalid_argument if there is no such column.

        is_column_not_exist(name);

        auto index = column_index[name];
        column_index.erase(name);

        for (auto &i : values)
            i.erase(i.begin() + index);

        return *this;
    }


    CSVData& clear() {
        // Deletes all rows and columns. It may invalidate references, pointers, and iterators referring to deleted elements.

        values.clear();
        column_index.clear();

        return *this;
    }



private:

    // Some hidden versions of methods.

    void _add_column(std::string name, std::string value = "") {
        // Adds a new column and expands every row with the value.
        column_index[name] = column_index.size();
        for (auto &i : values)
            i.push_back(value);
    }

    // Methods for data validation.

     void is_row_valid(const vector_s& row) {
         // Tests if a given row is valid in the current context. Throws std::invalid_argument if invalid.
         auto size = row.size();
         if (size != column_index.size())
             throw std::invalid_argument("The row size \"" + std::to_string(size) + "\" is invalid.");
    }

    template <typename T> void is_sequence_of_rows_valid(const T& sequence) {
        // Checks if all rows in a sequence of rows are valid. Throws std::invalid_argument if not.
        for (auto& i : sequence) is_row_valid(i);
    }

    void is_column_already_exist(std::string name) {
        // Checks if this column name is already in use.
        if (column_index.find(name) != column_index.end())
            throw std::invalid_argument("A column with name \"" + name + "\" already exists.");
    }

    template <typename T> void is_any_column_already_exist(const T& sequence) {
        // Checks if any of column names are already in use.
        for (auto i : sequence) is_column_already_exist(i);
    }

    void is_column_not_exist(std::string name) {
        // Checks if this column name is not exist.
        if (column_index.find(name) == column_index.end())
            throw std::invalid_argument("A column with name \"" + name + "\" doesn't exists.");
    }

    void is_row_index_valid(index_type index) {
        // Checks if given row index valid.
        if (index >= values.size())
            throw std::invalid_argument("There is no row with index \"" + std::to_string(index) + "\".");

    }


};






#endif
