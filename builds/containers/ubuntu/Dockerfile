FROM ubuntu:16.04

RUN apt-get update -y
RUN apt-get upgrade -y
RUN apt-get dist-upgrade -y
RUN apt-get install build-essential software-properties-common -y
RUN add-apt-repository ppa:ubuntu-toolchain-r/test -y
RUN apt-get update -y
RUN apt-get install python gcc-7 g++-7 -y
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 60 --slave /usr/bin/g++ g++ /usr/bin/g++-7
RUN update-alternatives --config gcc

RUN apt-get -y install libtool libtool-bin e2fslibs-dev libblkid-dev make cmake \
	autoconf automake wget git openssl

# Install camke
RUN wget https://cmake.org/files/v3.12/cmake-3.12.3.tar.gz && \
        tar zxvf cmake-3.* && \
        cd cmake-3.* && \
        ./bootstrap --prefix=/usr/local && \
        make -j$(nproc) && \
        make install

# Install libcurl, since ubuntu 16 doesn't support install libcurl-dev
RUN wget https://github.com/bagder/curl/releases/download/curl-7_44_0/curl-7.44.0.tar.gz
RUN tar xvf curl-7.44.0.tar.gz
RUN (cd curl-7.44.0/ && ./configure && make && make install)

# fpm
RUN apt-get install -y ruby ruby-dev rubygems
RUN gem install --no-document fpm




