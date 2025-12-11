# OPC UA ↔ Modbus Gateway

A lightweight, configurable gateway written in C that connects to Modbus RTU/TCP devices, reads their registers, and exposes them as OPC UA nodes using [open62541](https://open62541.org/).

## Features
- Modbus TCP and RTU support
- Configurable mapping between Modbus registers and OPC UA nodes
- Works cross-platform (Linux, Windows, embedded Linux)
- Lightweight and fast connection

## Getting Started
### Prerequisites
- CMake >= 3.10
- GCC
- [libmodbus](https://libmodbus.org/)
- [open62541](https://open62541.org/)
- [cJSON](https://github.com/DaveGamble/cJSON)
- [UaExpert](https://www.unified-automation.com/products/development-tools/uaexpert.html) or similar OPC-UA client application for testing and demonstration

### Build
```bash
mkdir build
cd build/
cmake ..
make
./uamb_gateway
```

### Use
* Start application `./uamb_gateway`
* Start `UaExpert` and click on `+ ` to add server
* Using `opc.tcp://<your IP or machine name or localhost>:4841` and Endpointurl connect to the server

## Contributor
* [Ritesh Sharma](myprofile.ritesh@gmail.com)
