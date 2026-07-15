#!/bin/bash

# Find all .cpp files and check if they contain the string "ASSERT_"
find . -name "*.cpp" | while read -r file; do
    if ! grep -q "ASSERT_" "$file"; then
        echo "$file"
    fi
done
