#!/bin/bash

# Compiles and runs all tests.

tests="test_csv_data test_csv_read"

echo "CSV Manager tests:"

for i in $tests; do
    printf "    ${i}: "
    dir="$(dirname "$0")"
    g++ "${dir}/${i}.cpp" -o "${dir}/${i}"
    (exec "${dir}/${i}") && echo "Good" || echo "Bad"
done
