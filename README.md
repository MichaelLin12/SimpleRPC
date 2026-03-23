# SimpleRPC

![C++](https://img.shields.io/badge/C++-23-blue?logo=cplusplus&logoColor=white)
![C](https://img.shields.io/badge/C-A8B9CC?style=flat&logo=c&logoColor=white)
![Clang](https://img.shields.io/badge/Clang-F34B7D?style=flat&logo=llvm&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-3.20+-green?logo=cmake&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?logo=linux&logoColor=black)
![License](https://img.shields.io/badge/License-Apache%202.0-blue)
![Status](https://img.shields.io/badge/Status-In%20Progress-orange)

A high-performance RPC framework in modern C++ designed around a lock-free architecture and event-driven I/O multiplexing.
Built as a deep-dive into the systems-level mechanics of inter-process communication — serialization overhead, lock-free data structures, and non-blocking I/O — informed by production experience on a DDS-based distributed defense system.

# Architecture
The design separates I/O and execution into distinct stages:

- I/O layer: A single epoll-based event loop monitors all active client connections. When a connection has data ready, the request is read and packaged into a lightweight request struct. This keeps the I/O path fully non-blocking without per-connection threads.
- Handoff: The packaged request is pushed onto a lock-free SPSC queue, decoupling the I/O thread from the execution thread with minimal synchronization overhead.
- Execution layer: A dedicated worker thread consumes requests from the queue, deserializes the payload, dispatches to the appropriate handler, and writes the response back to the client.

---

TCP was chosen as the transport layer for its ordering and exactly-once delivery guarantees, which are necessary for correct RPC semantics. UDP would require reimplementing these guarantees to avoid duplicate or out-of-order execution.

## Components
- SPSC Queue — lock-free single-producer single-consumer queue. Benchmarks in progress.
Serialization — custom binary serialization/deserialization layer over TCP with focus on minimal overhead
- Transport — TCP socket layer with epoll-based multi-client support

# Build

`./CMakeBuilds.sh`

# Run

- `cd ./build`
- `cd ./sample`
- `./server`
- `./client`

# RoadMap
- MPMC Queue - Switch the SPSC Queue to be MPMC so that multiple consumers can consume the queue
- Concurrent hashmap - Use a concurrent hashmap to avoid race conditions while registering functions
- Flat hasmap - use a flat hashmap as it has better
- Error Handling - allow for the server to send back errors to the client
- Void handling - allow for the server to execute void functions and return back nothing
- Heart Beats - allow for multiple servers to be spun up allow for detection of heartbeating
- Data Sharing - enable multiple servers to gossip data back and forth about the different connections and functions registered
- **Logging - update logging to use fmt::println**
