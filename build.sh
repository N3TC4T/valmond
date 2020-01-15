
#!/usr/bin/env bash

# Let's exit on errors
set -e

CYAN='\033[1;36m'
RED='\033[1;31m'
GREEN='\033[1;32m'
LIGHTGRAY='\033[0;37m'
NC='\033[0m' # No Color

ROOT=$(pwd)

libcryptopp=`whereis libcryptopp | awk -F ":"  '{print $2}'`
if [[ $libcryptopp == "" ]]; then
	echo -e "\n${RED}[!] libcryptopp is not installed! ${NC}"
fi

libsecp256k1=`whereis libsecp256k1 | awk -F ":"  '{print $2}'`
if [[ $libsecp256k1 == "" ]]; then
	echo -e "\n${RED}[!] libsecp256k1 is not installed! ${NC}"
fi

echo -e "\n${GREEN}[-] Building binary... ${NC}"

mkdir -p build
cd build

# cmake .. -DCMAKE_BUILD_TYPE=Release
cmake .. "$@"
cmake --build .

echo -e "\n${GREEN}[-] Build success!${NC}"
echo -e "${GREEN}[-] Output >> bin/rippledagent ${NC}"
