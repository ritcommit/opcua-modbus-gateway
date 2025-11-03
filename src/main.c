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

gateway_config_t config;

int main(void)
{
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
    init_opcua_server(&server);

    /* Add a variable node */
    add_variable_node(server);

    /* Run the OPC UA server */
    run_opcua_server(server);

    /* Cleanup the OPC UA server */
    cleanup_opcua_server(server);

    free_configuration(config_json);
	return 0;
}
