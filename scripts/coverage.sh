#!/bin/bash

TMP_NAME=$(mktemp -d)

cleanup() {
    # Shell quoting protects spaces or new‑lines in the path
    rm -rf "$TMP_NAME"
    cd ${OLDPWD}
}
trap cleanup EXIT INT TERM

cd ${TMP_NAME} \
    && cmake \
        -DCMAKE_BUILD_TYPE:STRING=Debug \
        -DFETCH_3RD_PARTY_REPOS:BOOL=TRUE \
        -DCI_MODE="COVERAGE" \
        ${OLDPWD} \
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
