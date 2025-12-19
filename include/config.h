/**
 * Part of opcua-modbus-gateway project subjected to terms of
 * MIT license agreement. A license file is distributed with
 * the project.
 * @Author: Ritesh Sharma
 * @Date: 31-10-2025
 * @Detail: json parser configuration header file
 */

#ifndef GWY_CONFIG_H
#define GWY_CONFIG_H

/*********************INCLUDES**********************/
#include <cjson/cJSON.h>
#include <open62541/server.h>

/*********************MACROS************************/
#define UA_DESCRIPTION_SIZE_MAX         (128)
#define UA_SECURITY_POLICY_SIZE_MAX     (64)
#define MB_ADDRESS_SIZE_MAX             (128)
/************TYPEDEFS, STRUCTS, ENUMS***************/
typedef enum {
    MODBUS_TYPE_NONE=0,
    MODBUS_TYPE_RTU=1,
    MODBUS_TYPE_TCP=2,
} modbus_type_t;

typedef enum{
    MODBUS_DTYPE_OC,
    MODBUS_DTYPE_DI,
    MODBUS_DTYPE_IR,
    MODBUS_DTYPE_HR,
    MODBUS_DTYPE_MAX
} modbus_dtype_t;

typedef enum
{
    PARSE_ERROR_NONE,                 // 0
    PARSE_ERROR_OPCUA_CONFIG,         // 1
    PARSE_ERROR_NULL_CONFIG,          // 2
    PARSE_ERROR_OPCUA_PORT,           // 3
    PARSE_ERROR_OPCUA_SECURITYPOLICY, // 4
    PARSE_ERROR_MODBUS_CONFIG,        // 5
    PARSE_ERROR_MODBUS_TYPE,          // 6
    PARSE_ERROR_MODBUS_BAUDRATE,      // 7
    PARSE_ERROR_MODBUS_PARITY,        // 8
    PARSE_ERROR_MODBUS_ADDRESS,       // 9
    PARSE_ERROR_MODBUS_PORT,          // 10
    PARSE_ERROR_MODBUS_SLAVEID,       // 11
    PARSE_ERROR_MODBUS_POLLINT,       // 12
    PARSE_ERROR_DATA_CONFIG,          // 13
    PARSE_ERROR_DATA_CALLOC,          // 14
    PARSE_ERROR_DATA_MB_ADDR,         // 15
    PARSE_ERROR_DATA_MB_DTYPE,        // 16
    PARSE_ERROR_DATA_MB_DLEN,         // 17
    PARSE_ERROR_DATA_UA_NODEID,       // 18
    PARSE_ERROR_DATA_UA_DESCRIPTION,  // 19
    PARSE_ERROR_DATA_UA_DTYPE,        // 20
} parse_error_t;

typedef struct
{
    int modbus_reg;
    modbus_dtype_t modbus_datatype;
    int modbus_datalen;
    UA_NodeId opcua_nodeid;
    UA_DataType opcua_datatype;
    char opcua_description[UA_DESCRIPTION_SIZE_MAX];
} data_config_t;

typedef struct
{
    int polling_interval;
    int opcua_port;
    char opcua_securitypolicy[UA_SECURITY_POLICY_SIZE_MAX];
    modbus_type_t modbus_type;
    char modbus_address[MB_ADDRESS_SIZE_MAX];
    int modbus_port;
    int modbus_baudrate;
    char modbus_parity;
    int modbus_slaveid;
    int data_cfg_size;
    data_config_t *data_cfg;
} gateway_config_t;

/************GLOBAL FUNCTION PROTOTYPES*************/
void load_configuration(const char *filename, cJSON **config_json);
parse_error_t parse_configuration(cJSON *config_json, gateway_config_t *config);
void free_configuration(cJSON *config_json);
modbus_dtype_t detectMbDatatype(int regaddr);
UA_NodeId parseNodeId(char *nodeidStr);
UA_DataType parseUaType(const char *dtype);

#endif /*GWY_CONFIG_H*/