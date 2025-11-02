/**
 * Part of opcua-modbus-gateway project subjected to terms of
 * MIT license agreement. A license file is distributed with
 * the project. 
 * @Author: Ritesh Sharma
 * @Date: 31-10-2025
 * @Detail: json parser configuration
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <cjson/cJSON.h>
#include "config.h"

void load_configuration(const char *filename, cJSON **config_json)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        *config_json = NULL;
        return;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = (char *)malloc(length + 1);
    fread(data, 1, length, file);
    data[length] = '\0';
    fclose(file);

    *config_json = cJSON_Parse(data);
    free(data);
}

void free_configuration(cJSON *config_json)
{
    if (config_json != NULL) {
        cJSON_Delete(config_json);
    }
}

parse_error_t parse_configuration(cJSON *config_json, gateway_config_t *cfg)
{
    parse_error_t error = PARSE_ERROR_NONE;
    if (config_json == NULL) {
        error = PARSE_ERROR_NULL_CONFIG;
        return error;
    }

    cJSON *opcua_config = cJSON_GetObjectItemCaseSensitive(config_json, "opcua");
    if (cJSON_IsObject(opcua_config)) {
        cJSON *opcua_port = cJSON_GetObjectItemCaseSensitive(opcua_config, "port");
        cJSON *security_policy = cJSON_GetObjectItemCaseSensitive(opcua_config, "security_policy");
        if (cJSON_IsNumber(opcua_port)) {
            cfg->opcua_port = opcua_port->valueint;
        } else {
            error = PARSE_ERROR_OPCUA_PORT;
        }
        if (cJSON_IsString(security_policy) && (security_policy->valuestring != NULL)) {
            strcpy(cfg->opcua_securitypolicy, security_policy->valuestring);
        } else {
            error = PARSE_ERROR_OPCUA_SECURITYPOLICY;
        }
    } else {
        error = PARSE_ERROR_OPCUA_CONFIG;
    }

    cJSON *modbus_config = cJSON_GetObjectItemCaseSensitive(config_json, "modbus");
    if (cJSON_IsObject(modbus_config)) {
        cJSON *type = cJSON_GetObjectItemCaseSensitive(modbus_config, "type");
        cJSON *address = cJSON_GetObjectItemCaseSensitive(modbus_config, "address");
        cJSON *slave_id = cJSON_GetObjectItemCaseSensitive(modbus_config, "slave_id");

        if (cJSON_IsString(type) && (type->valuestring != NULL)) {
            strcpy(cfg->modbus_type, type->valuestring);
            if (strcmp(type->valuestring, "tcp") == 0) {
                cJSON *port = cJSON_GetObjectItemCaseSensitive(modbus_config, "port");
                if (cJSON_IsNumber(port)) {
                    cfg->modbus_port = port->valueint;
                } else {
                    error = PARSE_ERROR_MODBUS_PORT;
                }
            }
            else if (strcmp(type->valuestring, "rtu") == 0) {
                cJSON *baudrate = cJSON_GetObjectItemCaseSensitive(modbus_config, "baudrate");
                cJSON *parity = cJSON_GetObjectItemCaseSensitive(modbus_config, "parity");
                if (cJSON_IsNumber(baudrate)) {
                    cfg->modbus_baudrate = baudrate->valueint;
                } else {
                    error = PARSE_ERROR_MODBUS_BAUDRATE;
                }
                if (cJSON_IsString(parity) && (parity->valuestring != NULL)) {
                    strcpy(cfg->modbus_parity, parity->valuestring);
                } else {
                    error = PARSE_ERROR_MODBUS_PARITY;
                }
            }
            else {
                error = PARSE_ERROR_MODBUS_TYPE;
            }
        } else {
            error = PARSE_ERROR_MODBUS_TYPE;
        }
        if (cJSON_IsString(address) && (address->valuestring != NULL)) {
            strcpy(cfg->modbus_address, address->valuestring);  
        } else {
            error = PARSE_ERROR_MODBUS_ADDRESS;
        }
        if (cJSON_IsNumber(slave_id)) {
            cfg->modbus_slaveid = slave_id->valueint;
        } else {
            error = PARSE_ERROR_MODBUS_SLAVEID;
        }
    } else {
        error = PARSE_ERROR_MODBUS_CONFIG;
    }
    return error;
}