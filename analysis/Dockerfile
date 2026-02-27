FROM ubuntu:latest

WORKDIR /app
RUN apt-get update && apt-get install -y build-essential \
                      clang-18 clang-tools-18 clangd-18 \
                      linux-tools-common linux-tools-generic

ENV CXX=clang++-18
ENV CC=clang-18
CMD ["/bin/bash"]