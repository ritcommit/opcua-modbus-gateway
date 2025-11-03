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

#include <cjson/cJSON.h>

typedef enum {
    MODBUS_TYPE_NONE=0,
    MODBUS_TYPE_RTU=1,
    MODBUS_TYPE_TCP=2,
} modbus_type_t;

typedef enum{
    MODBUS_DTYPE_OC=0,
    MODBUS_DTYPE_DI,
    MODBUS_DTYPE_IR,
    MODBUS_DTYPE_HR,
} modbus_dtype_t;

typedef struct{
    int modbus_reg;
    modbus_dtype_t modbus_datatype;
    int modbus_datalen;
    char ua_nodeid[128];
    char ua_datatype[32];
    float scalingfactor;
} data_config_t;

typedef struct {
    int opcua_port;
    char opcua_securitypolicy[64];
    modbus_type_t modbus_type;
    char modbus_address[128];
    int modbus_port;
    int modbus_baudrate;
    char modbus_parity;
    int modbus_slaveid;
    int data_cfg_size;
    data_config_t* data_cfg;
} gateway_config_t;

typedef enum {
    PARSE_ERROR_NONE = 0,
    PARSE_ERROR_OPCUA_CONFIG = 1,
    PARSE_ERROR_NULL_CONFIG = 2,
    PARSE_ERROR_OPCUA_PORT = 3,
    PARSE_ERROR_OPCUA_SECURITYPOLICY = 4,
    PARSE_ERROR_MODBUS_CONFIG = 5,
    PARSE_ERROR_MODBUS_TYPE = 6,
    PARSE_ERROR_MODBUS_BAUDRATE = 7,
    PARSE_ERROR_MODBUS_PARITY = 8,
    PARSE_ERROR_MODBUS_ADDRESS = 9,
    PARSE_ERROR_MODBUS_PORT = 10,
    PARSE_ERROR_MODBUS_SLAVEID = 11,
    PARSE_ERROR_DATA_CONFIG = 12,
} parse_error_t;

void load_configuration(const char *filename, cJSON **config_json);
parse_error_t parse_configuration(cJSON *config_json, gateway_config_t *config);
void free_configuration(cJSON *config_json);

#endif /*GWY_CONFIG_H*/