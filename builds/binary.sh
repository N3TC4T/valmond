#!/usr/bin/env bash

# Let's exit on errors
set -e

GREEN='\033[1;32m'
NC='\033[0m'

HERE=$(cd $(dirname $0) && pwd)
ROOT=$(dirname "$HERE")
BUILD_DIR="${HERE}/build"


echo -e "\n${GREEN}[-] Updating deps... ${NC}"

git submodule update --init --recursive

echo -e "\n${GREEN}[-] Building binary... ${NC}"

mkdir -p $BUILD_DIR
cd $BUILD_DIR

# cmake .. -DCMAKE_BUILD_TYPE=Release
cmake $ROOT "$@"
cmake --build .

echo -e "\n${GREEN}[-] Build success!${NC}"
echo -e "${GREEN}[-] Output >> bin/valmond ${NC}"
