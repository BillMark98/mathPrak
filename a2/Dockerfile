FROM ubuntu:trusty

RUN apt-get update
RUN apt-get install -y software-properties-common python-software-properties
RUN add-apt-repository ppa:ubuntu-toolchain-r/test
RUN apt-get update

RUN apt-get install gcc-4.9 g++-4.9 cmake gdb gdbserver -y  && \
    apt-get clean autoclean && \
    apt-get autoremove -y && \
    rm -rf /var/lib/apt/lists/*
# for gdbserver
EXPOSE 2000

ADD . /code
WORKDIR /code