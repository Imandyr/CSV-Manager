#ifndef CSV_MANAGER_CSV_DATA
#define CSV_MANAGER_CSV_DATA


#include <string>
#include <vector>
#include <unordered_map>


using vector_v_s = std::vector<std::vector<std::string>>;
using u_map_s_ui = std::unordered_map<std::string, uint>;


class CSVData {
/* Container of CSV data.
 *
 */
public:

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

//     class ColumnIterator : public BaseIterator {
//         // Iterator for iteration of values of one column
//     public:
//         using BaseIterator::BaseIterator;
//         ColumnIterator(vector_v_s::iterator it) : iter{it} {}
//     };


    u_map_s_ui column_index;
    vector_v_s values;

    CSVData(u_map_s_ui c_i = {}, vector_v_s v = {}) {column_index = c_i; values = v;}

    // TO DO: Remake it into pointer iterator to original objects in values.
    std::vector<std::string> column_values(std::string c_name) {
        std::vector<std::string> output;
        auto index = column_index[c_name];
        for (auto &i : values) output.push_back(i[index]);
        return output;
    }

private:

};






#endif
