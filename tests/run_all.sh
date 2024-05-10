#!/bin/bash

# Compiles and runs all tests.

tests="test_csv_data test_csv_read test_csv_parser"

dir="$(dirname "$0")"

echo "CSV Manager tests:"

for i in $tests; do
    printf "    ${i}: "
    path="${dir}/${i}"
    g++ "${path}.cpp" -o "${path}"
    (exec "${path}") && echo "Good" || echo "Bad"
done
