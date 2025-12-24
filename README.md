# OPC UA ↔ Modbus Gateway

[![License](https://img.shields.io/github/license/sanny32/OpenModSim)](LICENSE)

A lightweight, configurable gateway written in C that connects to Modbus RTU/TCP devices, reads their registers, and exposes them as OPC UA nodes using [open62541](https://open62541.org/).

## Features
* Modbus TCP and RTU support.
* Configurable mapping between Modbus registers and OPC UA nodes.
* Works cross-platform (Linux, Windows, embedded Linux).
* Lightweight and fast connection.

## Getting Started
### Prerequisites
* CMake >= 3.10
* GCC
* Install [libmodbus](https://libmodbus.org/)
* Install [open62541](https://open62541.org/)
* [cJSON](https://github.com/DaveGamble/cJSON)
* OPCUA Client for testing: [opcua-client-gui](https://github.com/FreeOpcUa/opcua-client-gui)
* Modbus Slave/Server simulator for testing: [OpenModsim](https://github.com/sanny32/OpenModSim)

### Build
```bash
mkdir build
cd build/
cmake ..
make
```

### Use
* Connect modbus slave device or start modbus slave simulator `openmodsim`.
* Change `config/config.json`.
* If using a simulator with RS485 bridge or actual modbus rtu device change `modbus->type` to `"rtu"`
* Change `modbus->address` to `"com port address"`.
* If using simulator in tcp mode change `modbus->type` to `"tcp"`.
* Change `modbus->address` to `"ip address of modbus server"`.
* Default modbus port is `502`. To change edit `modbus->port`.
* Default slave ID is `1`. To change edit `modbus->slave_id`.
* Default opcua port is `4841`. To change edit `opcua->port`.
* Leave security policy as `"none"`.
* To change modbus polling time edit `poll_interval_ms`. Time is in `ms`.
* To change data definition edit `data` section.
```json
"mb_register": 40001, # this is modbus register address
"mb_dlen": 2, # this is data length in terms of 16 bit registers
"ua_nodeid": "ns=1;s=Temperature", # opcua node id context
"ua_description": "Random Temperature", # data decription 
"ua_dtype": "float" # opcua datatype supported datatypes: "int" "float" "bool"
```
* Start application `./uamb_gateway`.
* Start `opcua-client`.
* Enter `opc.tcp://<opcua server IP>:4841` in address bar and click on `Connect`.
* Drag required nodes to `Subscription` window to observe data change.

## Contributor
* [Ritesh Sharma](https://www.linkedin.com/in/ritexarma/)
