#!/usr/bin/env bash

TMP_INC_FILE=/tmp/inc.txt
TMP_ERR_FILE=/tmp/err.xml
TMP_SUPPRESSIONS=/tmp/suppressions.txt

cat > ${TMP_SUPPRESSIONS} <<EOF
*:*folder-1-to-suppress*/*
*:*folder-2-to-suppress*/*
*:*build/_deps*/*
EOF
 
find . -type f \( -name "*.h" -o -name "*.hpp" \) \
    | xargs dirname \
    | sort \
    | uniq > ${TMP_INC_FILE}

cppcheck --suppress=missingInclude \
    --suppressions-list=${TMP_SUPPRESSIONS} \
    --includes-file=${TMP_INC_FILE} \
    --xml \
    --enable=all \
    --max-configs=100 \
    --inline-suppr ./test ./include 2> ${TMP_ERR_FILE}

cppcheck-htmlreport --file=${TMP_ERR_FILE} \
    --title=CppCheckReport \
    --report-dir=cppcheck_reports \
    --source-dir=$(pwd)

cd cppcheck_reports \
    && (killall python3 || true) \
    && /bin/bash -c 'python3 -m http.server &'

cd ${OLDPWD}
rm ${TMP_INC_FILE}
rm ${TMP_ERR_FILE}    
