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

/************TYPEDEFS, STRUCTS, ENUMS***************/
typedef enum{
    DTYPE_INT,
    DTYPE_FLOAT,
    DTYPE_MAX
}dtype_t;

typedef struct{
    int i;
    float f;
} default_t;

/*****************LOCAL VARIABLES*******************/
const char dtype_str[DTYPE_MAX][12]= {
    "int",
    "float",
};

default_t df = {.i=42, .f=42.24f};

/************LOCAL FUNCTION PROTOTYPES**************/
static UA_NodeId parseNodeId(char *nodeidStr);
static void get_default_value(void** init_val, UA_DataType** ua_dtype, const char* cfg_dtype);
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

void run_opcua_server(UA_Server *server, gateway_config_t* gwy_cfg)
{
    UA_Server_addRepeatedCallback(
        server,
        update_data,
        (void*)gwy_cfg,
        1000,      // 1 second
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
                               data_config_t data_cfg) {

    UA_NodeId nodeId = parseNodeId(data_cfg.opcua_nodeid);
    if(UA_NodeId_isNull(&nodeId))
        return UA_STATUSCODE_BADNODEIDINVALID;

    /* Add Variable Attributes */
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", nodeId.identifier.string.data);
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

    // /* Default value for each type */
    void* init_val = NULL;
    UA_DataType* ua_dtype = NULL;
    
    get_default_value(&init_val, &ua_dtype, data_cfg.opcua_datatype);
    
    UA_Variant_setScalar(&attr.value, init_val, ua_dtype);

    UA_NodeId newNodeId = UA_NODEID_STRING(nodeId.namespaceIndex, nodeId.identifier.string.data);
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableType = UA_NODEID_NULL;
    UA_QualifiedName browseName = UA_QUALIFIEDNAME(nodeId.namespaceIndex, nodeId.identifier.string.data);

    UA_StatusCode status = UA_Server_addVariableNode(
        server,
        newNodeId,                 /* requested NodeId */
        parentNodeId,              /* parent folder */
        parentReferenceNodeId,
        browseName,
        variableType,
        attr,
        NULL,                   /* no context */
        NULL                    /* outNodeId (optional) */
    );

    if(status == UA_STATUSCODE_GOOD)
    {
        printf("Node added: %s (type: %s)\n", data_cfg.opcua_nodeid, data_cfg.opcua_datatype);
    }

    return status;
}

/*****************LOCAL FUNCTIONS*******************/
static UA_NodeId parseNodeId(char *nodeidStr) {
    UA_NodeId nodeId;
    UA_StatusCode ret = UA_NodeId_parse(&nodeId, UA_STRING(nodeidStr));
    if(ret != UA_STATUSCODE_GOOD) {
        printf("Error parsing NodeId string %s\n", nodeidStr);
        return UA_NODEID_NULL;
    }
    return nodeId;
}

static void get_default_value(void** init_val, UA_DataType** ua_dtype, const char* cfg_dtype)
{
    dtype_t dtype = DTYPE_INT;
    while (dtype<DTYPE_MAX)
    {
        if (!strcmp(dtype_str[dtype], cfg_dtype))
        {
            break;
        }
        dtype += 1;
    }

    switch (dtype)
    {
        case DTYPE_INT:
            *init_val = &df.i;
            *ua_dtype = &UA_TYPES[UA_TYPES_INT32];
            break; 
        case DTYPE_FLOAT:
            *init_val = &df.f;
            *ua_dtype = &UA_TYPES[UA_TYPES_FLOAT];
            break;  
        case DTYPE_MAX:
        default:
            *init_val = &df.i;
            *ua_dtype = &UA_TYPES[UA_TYPES_INT32];
            break; 
    }
}

static void update_data(UA_Server *server, void *data)
{
    for(int i=0; i<((gateway_config_t*)data)->data_cfg_size; i++)
    {
        switch (((gateway_config_t*)data)->data_cfg[i].modbus_datatype)
        {
            case MODBUS_DTYPE_OC:
                break;
            case MODBUS_DTYPE_DI:
                break;
            case MODBUS_DTYPE_IR:
                break;
            case MODBUS_DTYPE_HR:
                printf("Reading Holding Register: %d\r\n", ((gateway_config_t*)data)->data_cfg[i].modbus_reg);
                break;
            case MODBUS_DTYPE_ERR:
            default:
                break;
        }
    }
}