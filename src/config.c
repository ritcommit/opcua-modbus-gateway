/**
 * Part of opcua-modbus-gateway project subjected to terms of
 * MIT license agreement. A license file is distributed with
 * the project.
 * @Author: Ritesh Sharma
 * @Date: 31-10-2025
 * @Detail: json parser configuration
 */
/*********************INCLUDES**********************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <cjson/cJSON.h>
#include "config.h"

/*****************GLOBAL VARIABLES******************/
data_config_t *gwy_data = NULL;
extern const int mb_start_addr[MODBUS_DTYPE_MAX];

/****************GLOBAL FUNCTIONS*******************/
void load_configuration(const char *filename, cJSON **config_json)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
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
    if (config_json != NULL)
    {
        cJSON_Delete(config_json);
    }
}

parse_error_t parse_configuration(cJSON *config_json, gateway_config_t *cfg)
{
    parse_error_t error = PARSE_ERROR_NONE;
    if (config_json == NULL)
    {
        error = PARSE_ERROR_NULL_CONFIG;
        return error;
    }

    cJSON *opcua_config = cJSON_GetObjectItemCaseSensitive(config_json, "opcua");
    if (cJSON_IsObject(opcua_config))
    {
        cJSON *opcua_port = cJSON_GetObjectItemCaseSensitive(opcua_config, "port");
        cJSON *security_policy = cJSON_GetObjectItemCaseSensitive(opcua_config, "security_policy");
        if (cJSON_IsNumber(opcua_port))
        {
            cfg->opcua_port = opcua_port->valueint;
        }
        else
        {
            error = PARSE_ERROR_OPCUA_PORT;
        }
        if (cJSON_IsString(security_policy) && (security_policy->valuestring != NULL))
        {
            strncpy(cfg->opcua_securitypolicy, security_policy->valuestring, UA_SECURITY_POLICY_SIZE_MAX);
        }
        else
        {
            error = PARSE_ERROR_OPCUA_SECURITYPOLICY;
        }
    }
    else
    {
        error = PARSE_ERROR_OPCUA_CONFIG;
    }

    cJSON *modbus_config = cJSON_GetObjectItemCaseSensitive(config_json, "modbus");
    if (cJSON_IsObject(modbus_config))
    {
        cJSON *type = cJSON_GetObjectItemCaseSensitive(modbus_config, "type");
        cJSON *address = cJSON_GetObjectItemCaseSensitive(modbus_config, "address");
        cJSON *slave_id = cJSON_GetObjectItemCaseSensitive(modbus_config, "slave_id");

        if (cJSON_IsString(type) && (type->valuestring != NULL))
        {
            if (strcmp(type->valuestring, "tcp") == 0)
            {
                cfg->modbus_type = MODBUS_TYPE_TCP;
                cJSON *port = cJSON_GetObjectItemCaseSensitive(modbus_config, "port");
                if (cJSON_IsNumber(port))
                {
                    cfg->modbus_port = port->valueint;
                }
                else
                {
                    error = PARSE_ERROR_MODBUS_PORT;
                }
            }
            else if (strcmp(type->valuestring, "rtu") == 0)
            {
                cfg->modbus_type = MODBUS_TYPE_RTU;
                cJSON *baudrate = cJSON_GetObjectItemCaseSensitive(modbus_config, "baudrate");
                cJSON *parity = cJSON_GetObjectItemCaseSensitive(modbus_config, "parity");
                if (cJSON_IsNumber(baudrate))
                {
                    cfg->modbus_baudrate = baudrate->valueint;
                }
                else
                {
                    error = PARSE_ERROR_MODBUS_BAUDRATE;
                }
                if (cJSON_IsString(parity) && (parity->valuestring != NULL))
                {
                    if (strcmp(parity->valuestring, "odd") == 0)
                    {
                        cfg->modbus_parity = 'O';
                    }
                    else if (strcmp(parity->valuestring, "even") == 0)
                    {
                        cfg->modbus_parity = 'E';
                    }
                    else
                    {
                        cfg->modbus_parity = 'N';
                    }
                }
                else
                {
                    error = PARSE_ERROR_MODBUS_PARITY;
                }
            }
            else
            {
                cfg->modbus_type = MODBUS_TYPE_NONE;
                error = PARSE_ERROR_MODBUS_TYPE;
            }
        }
        else
        {
            error = PARSE_ERROR_MODBUS_TYPE;
        }
        if (cJSON_IsString(address) && (address->valuestring != NULL))
        {
            strncpy(cfg->modbus_address, address->valuestring, MB_ADDRESS_SIZE_MAX);
        }
        else
        {
            error = PARSE_ERROR_MODBUS_ADDRESS;
        }
        if (cJSON_IsNumber(slave_id))
        {
            cfg->modbus_slaveid = slave_id->valueint;
        }
        else
        {
            error = PARSE_ERROR_MODBUS_SLAVEID;
        }
    }
    else
    {
        error = PARSE_ERROR_MODBUS_CONFIG;
    }

    cJSON *poll_config = cJSON_GetObjectItemCaseSensitive(config_json, "poll_interval_ms");
    if (cJSON_IsNumber(poll_config))
    {
        cfg->polling_interval = poll_config->valueint;
    }
    else
    {
        error = PARSE_ERROR_MODBUS_POLLINT;
    }

    cJSON *data_configs = cJSON_GetObjectItemCaseSensitive(config_json, "data");
    if (cJSON_IsArray(data_configs))
    {
        cfg->data_cfg_size = cJSON_GetArraySize(data_configs);
        gwy_data = (data_config_t *)calloc(cfg->data_cfg_size, sizeof(data_config_t));
        if (gwy_data == NULL)
        {
            error = PARSE_ERROR_DATA_CALLOC;
            return error;
        }

        cJSON *data_config = NULL;
        int index = 0;
        cJSON_ArrayForEach(data_config, data_configs)
        {
            cJSON *mb_addr = cJSON_GetObjectItemCaseSensitive(data_config, "mb_register");
            cJSON *mb_dlen = cJSON_GetObjectItemCaseSensitive(data_config, "mb_dlen");
            cJSON *ua_nodeid = cJSON_GetObjectItemCaseSensitive(data_config, "ua_nodeid");
            cJSON *ua_description = cJSON_GetObjectItemCaseSensitive(data_config, "ua_description");
            cJSON *ua_dtype = cJSON_GetObjectItemCaseSensitive(data_config, "ua_dtype");

            if (cJSON_IsNumber(mb_addr))
            {
                gwy_data[index].modbus_datatype = detectMbDatatype(mb_addr->valueint);
                if (MODBUS_DTYPE_MAX == gwy_data[index].modbus_datatype)
                {
                    error = PARSE_ERROR_DATA_MB_DTYPE;
                    break;
                }
                gwy_data[index].modbus_reg = mb_addr->valueint - mb_start_addr[gwy_data[index].modbus_datatype];
            }
            else
            {
                error = PARSE_ERROR_DATA_MB_ADDR;
                break;
            }
            if (cJSON_IsNumber(mb_dlen))
            {
                gwy_data[index].modbus_datalen = mb_dlen->valueint;
            }
            else
            {
                error = PARSE_ERROR_DATA_MB_DLEN;
                break;
            }
            if (cJSON_IsString(ua_nodeid) && (ua_nodeid->valuestring != NULL))
            {
                gwy_data[index].opcua_nodeid = parseNodeId(ua_nodeid->valuestring);
            }
            else
            {
                error = PARSE_ERROR_DATA_UA_NODEID;
                break;
            }
            if (cJSON_IsString(ua_description) && (ua_description->valuestring != NULL))
            {
                strncpy(gwy_data[index].opcua_description, ua_description->valuestring, UA_DESCRIPTION_SIZE_MAX);
            }
            else
            {
                error = PARSE_ERROR_DATA_UA_DESCRIPTION;
                break;
            }
            if (cJSON_IsString(ua_dtype) && ua_dtype->valuestring != NULL)
            {
                gwy_data[index].opcua_datatype = parseUaType(ua_dtype->valuestring);
            }
            else
            {
                error = PARSE_ERROR_DATA_UA_DTYPE;
                break;
            }
            index++;
        }

        if (error != PARSE_ERROR_NONE)
        {
            free(gwy_data);
            gwy_data = NULL;
        }
        else
        {
            cfg->data_cfg = gwy_data;
        }
    }
    else
    {
        error = PARSE_ERROR_DATA_CONFIG;
    }
    return error;
}