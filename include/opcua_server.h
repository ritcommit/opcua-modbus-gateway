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
#include "config.h"

void init_opcua_server(UA_Server **server, gateway_config_t gwy_cfg);
void run_opcua_server(UA_Server *server, gateway_config_t* gwy_cfg);
void cleanup_opcua_server(UA_Server *server);
UA_StatusCode add_variable_node(UA_Server *server,  data_config_t data_cfg);

#endif /*GWY_OPCUA_SERVER_H*/