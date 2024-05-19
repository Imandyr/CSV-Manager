#!/bin/bash

# Compiles and runs all tests.


do_test() {
    path="$1"
    g++ "${path}.cpp" -o "${path}" -Og && (exec "${path}") && printf "Ok " || printf "Bad "
}


tests="test_csv_data test_csv_reader test_csv_parser test_csv_encoder"

dir="$(dirname "$0")"
TIMEFORMAT=%R


echo "CSV Manager tests:"

for i in $tests; do
    printf "    ${i}: "
    time do_test "${dir}/${i}"
done
