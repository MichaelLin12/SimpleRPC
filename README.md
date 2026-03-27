# SimpleRPC

![C++](https://img.shields.io/badge/C++-23-blue?logo=cplusplus&logoColor=white)
![C](https://img.shields.io/badge/C-A8B9CC?style=flat&logo=c&logoColor=white)
![Clang](https://img.shields.io/badge/Clang-F34B7D?style=flat&logo=llvm&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-3.20+-green?logo=cmake&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?logo=linux&logoColor=black)
![License](https://img.shields.io/badge/License-Apache%202.0-blue)
![Status](https://img.shields.io/badge/Status-In%20Progress-orange)

An RPC framework in modern C++ designed around a lock-free architecture and event-driven I/O multiplexing.
Built as a deep-dive into the systems-level mechanics of inter-process communication — serialization overhead, lock-free data structures, and non-blocking I/O — informed by production experience on a DDS-based distributed defense system.

## Table of Contents
- [SimpleRPC](#simplerpc)
- [Libraries](#libraries)
- [Build](#build)
- [Run](#run)
- [Developer Notes](#developer-notes)
- [Credits](#credits)


# Libraries
- Standard Template Library
- fmt

# Build

`./CMakeBuilds.sh`

# Run

- `cd ./build`
- `cd ./sample`
- `./server`
- `./client`

# Developer Notes

To learn more about the internals of this project and what things are being worked on, it is encouraged to look through the following documentation:

- [Roadmap](./docs/Roadmap.md) shares details of work either current or planned for this project. Any work that is bolded is currently being worked on at the moment.
- [Developer Notes](./docs/DeveloperNotes.md) has information about why certain design decisions were made in the making of this project
- [Protocol](./docs/Protocol.md) lists out all possible messages used in SimpleRPC
- [Message](./docs/Message.md) gives an overview of all the methods and members of a message object
- [MessageTypes](./docs/MessageTypes.md) details all the types of messages that can be passed around in SimpleRPC
- [Serialization](./docs/Serialization.md) talks about how serialization works
- [Deserialization](./docs/Deserialization.md) talks about how deserialization works

# Credits

[Michael Lin](https://www.linkedin.com/in/michaellin12white/)

