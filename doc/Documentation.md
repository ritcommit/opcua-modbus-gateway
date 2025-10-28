# Documentation for OPC-UA MODBUS GATEWAY Project

**Technology:** C, open62541, libmodbus, raspberry pi zero 2w

## 1. Introduction

### 1.1 Purpose
This project aims to develop a lightweight cross-platform OPC-UA Modbus Gateway application using open source libraries. This will enable legacy industrial equipment to be monitored and controlled via modern OPC UA clients and dashboards. Primary objectives are:
* To provide a open source ready to use opc ua modbus gateway for personal and commercial use
* To provide an interface to build and scale the application using simple configuration file
* To serve as a reference implementation for development in similar domains

### 1.2 Scope
* **Southbound Interface:** Communicate Modbus RTU master with  Modbus RTU slaves
* **Northbound Interface:** Expose Modbus registers as OPC UA variables via an embedded OPC UA server
* **Mapping Engine:** Map Modbus coils/registers to OPC UA nodes dynamically based on config file
* **Extensibility:** Allow users to add new Modbus devices without modifying source code

### 1.3 Definitions
* **OPC-UA** Open Platform Communication Unified Architecture a machine to machine communication standard for industrial automation
* **open62541** an open source implementation of the OPC UA standard written in C
* **Modbus** An legacy industrial communication protocol created originally for PLC communication and extended to toher domains over the time.

## 2. Functional Requirements

## 3. Non Functional Requirements

## 4. High Level Architecture
```
         _______________
        │ Config Loader │  <- Reads config.json
        |_______________|
                |
       _________|____________
      │ Data Mapper Layer    │  <- Links Modbus addresses to OPC UA node IDs
      |______________________|
                │
       _________|____________
      │ Modbus Client Module │ <- Reads/writes Modbus registers via TCP or RTU
      |______________________|
                │
       _________|____________
      │ OPC UA Server Module │ <- Exposes mapped registers as OPC UA nodes
      |______________________|
```

## 5. Directory Structure
```
opcua-modbus-gateway/
├── CMakeLists.txt
├── README.md
├── include/
|   |── open62541.h            # open6541 2 file SDK
│   ├── gateway_config.h       # Config parsing structures
│   ├── opcua_server.h         # OPC UA server logic
│   ├── modbus_client.h        # Modbus RTU/TCP handling
│   ├── data_mapper.h          # Mapping Modbus <-> OPC UA nodes
│   └── utils.h                # Logging, helpers
├── src/
│   ├── main.c                 # Entry point
|   ├── open62541.c
│   ├── gateway_config.c
│   ├── opcua_server.c
│   ├── modbus_client.c
│   ├── data_mapper.c
│   └── utils.c
├── config/
│   └── config.json           # Example Modbus <-> OPC UA mapping 
├── build/                     # Built executables
└── tests/
    └── unit test cases

```

## 6. Tool and Dependencies

## 7. Future Enhancements