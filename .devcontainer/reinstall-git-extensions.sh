#!/usr/bin/env bash
set -e

# Cleanup temporary directory and associated files when exiting the script.
cleanup() {
    EXIT_CODE=$?
    set +e
    if [[ -n "${TMP_DIR}" ]]; then
        echo "Executing cleanup of tmp files"
        rm -Rf "${TMP_DIR}"
    fi
    exit $EXIT_CODE
}
trap cleanup EXIT

echo "Installing git-extensions..."

BINARY_NAME="main.zip"
TMP_DIR=$(mktemp -d -t git-XXXXXXXXXX)

echo "${TMP_DIR}"
cd "${TMP_DIR}"

curl -sSL "https://github.com/stefanofiorentino/git-extensions/archive/refs/heads/${BINARY_NAME}" -O
unzip ${BINARY_NAME}

mkdir -p /home/vscode/.local/bin && chown -R vscode:vscode /home/vscode/.local/bin
mkdir -p /home/vscode/.local/share && chown -R vscode:vscode /home/vscode/.local/share
mv git-extensions-main/* /home/vscode/.local/bin/
find /home/vscode/.local/bin -name "git-*" | xargs -r -n1 chmod +x
