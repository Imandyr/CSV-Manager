#!/bin/bash

# Compiles and runs all tests.

tests="test_csv_data test_csv_reader test_csv_parser test_csv_encoder"

dir="$(dirname "$0")"

echo "CSV Manager tests:"

for i in $tests; do
    printf "    ${i}: "
    path="${dir}/${i}"
    g++ "${path}.cpp" -o "${path}" -Og && (exec "${path}") && echo "Ok" || echo "Bad"
done
