#!/bin/bash

git ls-files *.[ch] | xargs -r -n1 clang-format-14 --fallback-style=Mozilla -i
git ls-files *.[ch]pp | xargs -r -n1 clang-format-14 --fallback-style=Mozilla -i
