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
    };


    // CSVData class definition.


    // Table with column names and thier indexes in vector.
    u_map_s_st column_index;
    // Vector with all data.
    vector_v_s values;

    CSVData(u_map_s_st c_i = {}, vector_v_s v = {}) {
        // Initialization from column table and vector with values.
        column_index = c_i;
        values = v;
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


    Column column(std::string name) {
        // Creates Column object of column with target name.
        return Column(&values, column_index[name]);
    }


    void delete_row(index_type index) {
        // Deletes row on given index.
        values.erase(values.begin() + index);
    }
    void delete_row(index_type from, index_type number) {
        // Deletes number of rows after "from" index (including "from").
        auto start = values.begin() + from;
        values.erase(start, start + number);
    }


    void add_row(index_type number) {
        // Adds "number" blank rows at the end of the values vector.
        vector_s blank(column_index.size(), "");
        for (index_type i = 0; i < number; ++i)
            values.push_back(blank);
    }
    void add_row(index_type number, vector_s row) {
        // Adds "number" rows with value "row" at the end of the values vector.
        for (index_type i = 0; i < number; ++i)
            values.push_back(row);
    }


    void insert_row(index_type to) {
        // Inserts blank row on index before "to".
        values.insert(values.begin() + to, vector_s(column_index.size(), ""));
    }
    void insert_row(index_type to, index_type number) {
        // Inserts number of blank rows on index before "to".
        values.insert(values.begin() + to, number, vector_s(column_index.size(), ""));
    }
    void insert_row(index_type to, vector_s row) {
        // Inserts row with given value on index before "to".
        values.insert(values.begin() + to, row);
    }
    void insert_row(index_type to, vector_s row, index_type number) {
        // Inserts number of rows with given value on index before "to".
        values.insert(values.begin() + to, number, row);
    }
    void insert_row(index_type to, std::initializer_list<vector_s> rows) {
        // Inserts rows on index of "to".
        values.insert(values.begin() + to, rows);
    }


    void add_column(std::string name, std::string value = "") {
        // Adds a column at the end with the name "name" and appends the value to every row, so a number of columns and elements matches.
        // Raises std::invalid_argument if the column name is already exist.

        if (column_index.find(name) != column_index.end())
            throw std::invalid_argument("A column with name \"" + name + "\" already exists.");

        column_index[name] = column_index.size();
        for (auto &i : values)
            i.push_back(value);
    }


    void delete_column(std::string name) {
        // Deletes column and all its values. Raises std::invalid_argument if there is no such column.

        if (column_index.find(name) == column_index.end())
            throw std::invalid_argument("A column with name \"" + name + "\" doesn't exists.");

        auto index = column_index[name];
        column_index.erase(name);

        for (auto &i : values)
            i.erase(i.begin() + index);
    }



private:

};






#endif
