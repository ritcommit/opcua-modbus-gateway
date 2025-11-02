/**
 * Part of opcua-modbus-gateway project subjected to terms of
 * MIT license agreement. A license file is distributed with
 * the project. 
 * @Author: Ritesh Sharma
 * @Date: 31-10-2025
 * @Detail: opcua server header file
*/
#ifndef GWY_OPCUA_SERVER_H
#define GWY_OPCUA_SERVER_H

#include <open62541/server.h>
#include <open62541/server_config_default.h>

void init_opcua_server(UA_Server **server);
void run_opcua_server(UA_Server *server);
void cleanup_opcua_server(UA_Server *server);

#endif /*GWY_OPCUA_SERVER_H*/