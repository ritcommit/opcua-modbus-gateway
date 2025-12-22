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
- For testing OPCUA Client: [UaExpert](https://www.unified-automation.com/products/development-tools/uaexpert.html)
- For testing Modbus Slave Simulator: [OpenModsim](https://github.com/sanny32/OpenModSim)

### Build
```bash
mkdir build
cd build/
cmake ..
make
./uamb_gateway
```

### Use
* Connect modbus slave device or modbus slave simulator
* Change `config/config.json` as per your utility
* Start application `./uamb_gateway`
* Start `UaExpert` and click on `+ ` to add server
* Using `opc.tcp://<your IP or machine name or localhost>:4841` and Endpointurl connect to the server

## Contributor
* [Ritesh Sharma](myprofile.ritesh@gmail.com)
