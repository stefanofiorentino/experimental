#!/bin/bash

TMP_NAME=$(mktemp -d)
RETURN_FOLDER=${PWD}
cleanup() {
    cd ${RETURN_FOLDER}
    # Shell quoting protects spaces or new‑lines in the path
    rm -rf "$TMP_NAME"
}
trap cleanup EXIT INT TERM

cd ${TMP_NAME} \
    && cmake \
        -DCMAKE_BUILD_TYPE:STRING=Debug \
        -DCI_MODE="COVERAGE" \
        ${RETURN_FOLDER} \
    && make \
        -j$(nproc) \
    && ctest \
        --timeout 30 \
        -T Test \
        -T Coverage \
        -C Debug \
        -j2 \
        --output-on-failure \
        2>&1
    
cleanup
