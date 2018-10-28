FROM gcc:7.3.0

RUN apt-get -y update
RUN apt-get -y upgrade
RUN apt-get -y install cmake

RUN apt-get -y install libboost-all-dev=1.62.0.1
RUN apt-get -y install build-essential libtcmalloc-minimal4 && ln -s /usr/lib/libtcmalloc_minimal.so.4 /usr/lib/libtcmalloc_minimal.so
