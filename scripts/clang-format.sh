#!/bin/bash

DIRS="test include"
for dir in $DIRS; 
do 
    echo ${dir}
    find ./${dir} -not -type d \( -name "*.cpp" -or -name "*.h" -or -name "*.hpp" \) | xargs -r -n1 clang-format-14 --fallback-style=Mozilla -i
done
