/**
 * Part of opcua-modbus-gateway project subjected to terms of
 * MIT license agreement. A license file is distributed with
 * the project. 
 * @Author: Ritesh Sharma
 * @Date: 30-10-2025
 * @Detail: entry point of the application
 */

#include <stdio.h>
#include "opcua_server.h"
#include "config.h"

int main(void)
{
    gateway_config_t config;
    UA_Server *server = NULL;   
    cJSON *config_json = NULL;
    load_configuration("../config/config.json", &config_json);
    if (config_json == NULL) {
        printf("Failed to load configuration.\n");
        return -1;
    }

    parse_error_t parse_error = parse_configuration(config_json, &config);
    if (PARSE_ERROR_NONE != parse_error) {
        printf("Failed to parse configuration. ERROR [%d]\n", (int)parse_error);
        free_configuration(config_json);
        return -1;
    }
    
    /* Initialize the OPC UA server */
    init_opcua_server(&server, config);

    for (int i=0; i<config.data_cfg_size; i++)
    {
        /* Add a variable node */
        UA_StatusCode add_node_error = add_variable_node(server, config.data_cfg[i]);
        if (UA_STATUSCODE_GOOD != add_node_error)
        {
            printf("Failed to add UA node. [%s] ERROR [%d]\n", config.data_cfg[i].opcua_nodeid, (int)add_node_error);
        }
    }

    /* Run the OPC UA server */
    run_opcua_server(server, &config);

    /* Cleanup the OPC UA server */
    cleanup_opcua_server(server);

    free_configuration(config_json);
	return 0;
}
