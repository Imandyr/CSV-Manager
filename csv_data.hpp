#ifndef CSV_MANAGER_CSV_DATA
#define CSV_MANAGER_CSV_DATA


#include <string>
#include <vector>
#include <unordered_map>


using vector_v_s = std::vector<std::vector<std::string>>;
using index_type = vector_v_s::size_type;
using u_map_s_st = std::unordered_map<std::string, index_type>;


class CSVData {
/* Container of CSV data.
 *
 */
public:

    // Helper classes.

    class BaseIterator {
    // Base iterator for all CSVData iteators.
    public:
        // Iterator tags.
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = std::vector<std::string>;
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

    // CSVData class definition.

    // Table with column names and thier indexes in vector.
    u_map_s_st column_index;
    // Vector with all data.
    vector_v_s values;

    // Initialization from column table and vector with values.
    CSVData(u_map_s_st c_i = {}, vector_v_s v = {}) {column_index = c_i; values = v;}

    ColumnIterator begin_col_iter(std::string c_name) {
        // Returns ColumnIterator on the beginning of a values vector.
        return ColumnIterator(values.begin(), column_index[c_name]);
    }

    ColumnIterator end_col_iter(std::string c_name) {
        // Returns ColumnIterator on the end of a values vector.
        return ColumnIterator(values.end(), column_index[c_name]);
    }

    std::vector<std::string> column_values(std::string c_name) {
        // Vector with copies of objects from given column.
        std::vector<std::string> output;
        auto index = column_index[c_name];
        for (auto &i : values) output.push_back(i[index]);
        return output;
    }


private:

};






#endif
