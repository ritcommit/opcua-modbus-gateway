# OPC UA ↔ Modbus Gateway

A lightweight, configurable gateway written in C that connects to Modbus RTU/TCP devices, reads their registers, and exposes them as OPC UA nodes using [open62541](https://open62541.org/).

## Features
- Modbus TCP and RTU support
- Configurable mapping between Modbus registers and OPC UA nodes
- Works cross-platform (Linux, Windows, embedded Linux)
- Lightweight and dependency-minimal

## Getting Started
### Prerequisites
- CMake >= 3.10
- GCC/Clang
- [libmodbus](https://libmodbus.org/) installed (or use third_party build)
- [open62541 two-file release](https://open62541.org/)

### Build
```bash
mkdir build && cd build
cmake ..
cmake --build .
