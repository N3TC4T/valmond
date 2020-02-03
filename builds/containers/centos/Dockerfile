FROM centos:7

RUN yum -y upgrade
RUN yum -y update
RUN yum -y install git wget epel-release centos-release-scl scl-utils \
        libblkid-devel e2fsprogs-devel rpm-build rubygems ruby-devel \
        curl curl-devel gcc-c++ openssl pkgconfig libtool autoconf make which gnugp

RUN yum -y install devtoolset-8 devtoolset-8-gdb devtoolset-8-binutils devtoolset-8-libstdc++-devel

# install FPM
RUN gem install --no-document fpm

# install camke
RUN wget https://cmake.org/files/v3.12/cmake-3.12.3.tar.gz && \
        tar zxvf cmake-3.* && \
        cd cmake-3.* && \
        ./bootstrap --prefix=/usr/local && \
        make -j$(nproc) && \
        make install

# Enable rh-python scl binary
COPY entrypoint.sh /usr/bin/entrypoint.sh
RUN chmod +x /usr/bin/entrypoint.sh
ENTRYPOINT [ "/usr/bin/entrypoint.sh" ]
