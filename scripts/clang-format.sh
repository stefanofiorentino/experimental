#!/bin/bash

DIRS="test include"
for dir in $DIRS; 
do 
    find ./${DIR} -not -type d -name "*.cpp" -name "*.h" -name "*.hpp" | xargs -r -n1 clang-format-14 --fallback-style=Mozilla -i
done
