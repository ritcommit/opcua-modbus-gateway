/**
 * Part of opcua-modbus-gateway project subjected to terms of
 * MIT license agreement. A license file is distributed with
 * the project. 
 * @Author: Ritesh Sharma
 * @Date: 31-10-2025
 * @Detail: opcua server implementation
 */
#include "config.h"
#include "opcua_server.h"

extern gateway_config_t config;

void init_opcua_server(UA_Server **server)
{
    /* Create a server listening on port 4840 (default) */
    *server = UA_Server_new();
    UA_ServerConfig *ua_server_config = UA_Server_getConfig(*server);
    UA_UInt16 port = (config.opcua_port != 0) ? config.opcua_port : 4840;
    UA_ServerConfig_setMinimal(ua_server_config, port, NULL); /* default port */
}

void run_opcua_server(UA_Server *server)
{
    /* Add a variable node to the server */

    /* Run the server (until ctrl-c interrupt) */
    UA_StatusCode status = UA_Server_runUntilInterrupt(server);
}

void cleanup_opcua_server(UA_Server *server)
{
    /* Clean up */
    UA_Server_delete(server);
}

void add_variable_node(UA_Server *server)
{
    /* 1) Define the variable attributes */
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "the answer");
    UA_Int32 myInteger = 42;
    UA_Variant_setScalar(&attr.value, &myInteger, &UA_TYPES[UA_TYPES_INT32]);

    /* 2) Define where the node shall be added with which browsename */
    UA_NodeId newNodeId = UA_NODEID_STRING(1, "the.answer");
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableType = UA_NODEID_NULL; /* take the default variable type */
    UA_QualifiedName browseName = UA_QUALIFIEDNAME(1, "the answer");

    /* 3) Add the node */
    UA_Server_addVariableNode(server, newNodeId, parentNodeId,
                              parentReferenceNodeId, browseName,
                              variableType, attr, NULL, NULL);
}