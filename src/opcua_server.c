/**
 * Part of opcua-modbus-gateway project subjected to terms of
 * MIT license agreement. A license file is distributed with
 * the project.
 * @Author: Ritesh Sharma
 * @Date: 31-10-2025
 * @Detail: opcua server implementation
 */
/*********************INCLUDES**********************/
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "opcua_server.h"
#include "modbus_client.h"

/************TYPEDEFS, STRUCTS, ENUMS***************/

/*****************LOCAL VARIABLES*******************/

/************LOCAL FUNCTION PROTOTYPES**************/
static void update_data(UA_Server *server, void *data);

/****************GLOBAL FUNCTIONS*******************/
void init_opcua_server(UA_Server **server, gateway_config_t gwy_cfg)
{
    /* Create a server listening on given port or 4840 (default) */
    *server = UA_Server_new();
    UA_ServerConfig *ua_server_config = UA_Server_getConfig(*server);
    UA_UInt16 port = (gwy_cfg.opcua_port != 0) ? gwy_cfg.opcua_port : 4840;
    UA_ServerConfig_setMinimal(ua_server_config, port, NULL); /* default port */
}

void run_opcua_server(UA_Server *server, gateway_config_t *gwy_cfg)
{
    UA_Server_addRepeatedCallback(
        server,
        update_data,
        (void *)gwy_cfg,
        1000, // 1 second
        NULL);
    /* Run the server (until ctrl-c interrupt) */
    UA_StatusCode status = UA_Server_runUntilInterrupt(server);
}

void cleanup_opcua_server(UA_Server *server)
{
    /* Clean up */
    UA_Server_delete(server);
}

/* Function to add a node */
UA_StatusCode add_variable_node(UA_Server *server,
                                data_config_t data_cfg)
{

    /* Add Variable Attributes */
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", data_cfg.opcua_nodeid.identifier.string.data);
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

    UA_Variant_setScalar(&attr.value, NULL, &data_cfg.opcua_datatype);

    UA_NodeId newNodeId = UA_NODEID_STRING(data_cfg.opcua_nodeid.namespaceIndex, data_cfg.opcua_nodeid.identifier.string.data);
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableType = UA_NODEID_NULL;
    UA_QualifiedName browseName = UA_QUALIFIEDNAME(data_cfg.opcua_nodeid.namespaceIndex, data_cfg.opcua_nodeid.identifier.string.data);

    UA_StatusCode status = UA_Server_addVariableNode(
        server,
        data_cfg.opcua_nodeid, /* requested NodeId */
        parentNodeId,          /* parent folder */
        parentReferenceNodeId,
        browseName,
        variableType,
        attr,
        NULL, /* no context */
        NULL  /* outNodeId (optional) */
    );

    if (status == UA_STATUSCODE_GOOD)
    {
        printf("Node added: [%s]<-->[%d]\n", data_cfg.opcua_nodeid.identifier.string.data, data_cfg.modbus_reg);
    }

    return status;
}

UA_NodeId parseNodeId(char *nodeidStr)
{
    UA_NodeId nodeId;
    UA_StatusCode ret = UA_NodeId_parse(&nodeId, UA_STRING(nodeidStr));
    if (ret != UA_STATUSCODE_GOOD)
    {
        printf("Error parsing NodeId string %s\n", nodeidStr);
        return UA_NODEID_NULL;
    }
    return nodeId;
}

UA_DataType parseUaType(const char *dtype)
{
    if (strcmp(dtype, "int16") == 0)
        return UA_TYPES[UA_TYPES_INT16];
    if (strcmp(dtype, "int32") == 0)
        return UA_TYPES[UA_TYPES_INT32];
    if (strcmp(dtype, "float") == 0)
        return UA_TYPES[UA_TYPES_FLOAT];
    if (strcmp(dtype, "bool") == 0)
        return UA_TYPES[UA_TYPES_BOOLEAN];
    return UA_TYPES[UA_TYPES_INT16];
}

/*****************LOCAL FUNCTIONS*******************/
static void update_data(UA_Server *server, void *data)
{
    for (int i = 0; i < ((gateway_config_t *)data)->data_cfg_size; i++)
    {
        switch (((gateway_config_t *)data)->data_cfg[i].modbus_datatype)
        {
        case MODBUS_DTYPE_OC:
            mbclient_read_coils();
            break;
        case MODBUS_DTYPE_DI:
            mbclient_read_discrete_inputs();
            break;
        case MODBUS_DTYPE_IR:
            mbclient_read_input_registers();
            break;
        case MODBUS_DTYPE_HR:
            printf("Reading Holding Register: %d\r\n", ((gateway_config_t *)data)->data_cfg[i].modbus_reg);
            mbclient_read_holding_registers();
            break;
        default:
            break;
        }
    }
}