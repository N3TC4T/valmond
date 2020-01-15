# rippled-agent
> Rippled Monitoring Agent

## What
 The Rippled Monitoring Agent is a lightweight process that monitors system
 metrics and rippled validator and sends information back to monitoring backend.

## Dependencies
|Name|Debian Package|
|-|-|
|[CMake](https://cmake.org)|`cmake`|
|[Crypto++](https://www.cryptopp.com/)|`libcrypto++-dev`|
|[secp256k1](https://github.com/bitcoin-core/secp256k1)|`libsecp256k1-dev`|

## Build Instructions
```bash
# grab the dependencies if you haven't already
sudo apt install git \
                 build-essential \
                 cmake \
                 libcrypto++-dev \
                 libsecp256k1-dev

# download the source and build
git clone https://github.com/N3TC4T/rippled-agent.git
mkdir rippled-agent/build
cd $_
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .

```

## License
Released under the terms of the MIT license. See `LICENSE` for more
information.
