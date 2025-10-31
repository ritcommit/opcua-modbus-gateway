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

int main(void)
{
    UA_Server *server = NULL;

    /* Initialize the OPC UA server */
    init_opcua_server(&server);

    /* Run the OPC UA server */
    run_opcua_server(server);

    /* Cleanup the OPC UA server */
    cleanup_opcua_server(server);

	return 0;
}
