#include <stdio.h>
#include <signal.h>
#include "open62541.h"

#define OPCUA_ENDPOINT "opc.tcp://localhost:4840"

static volatile UA_Boolean running = true;

static void stop_handler(int sig)
{
	(void)sig;
	running = false;
}

void run_server(void)
{
	signal(SIGINT, stop_handler);
	signal(SIGTERM, stop_handler);

	UA_Server *server = UA_Server_new();
	UA_ServerConfig_setDefault(UA_Server_getConfig(server));

	printf("OPC UA server running at %s\n", OPCUA_ENDPOINT);
	UA_StatusCode status = UA_Server_run(server, &running);

	UA_Server_delete(server);
	if(status != UA_STATUSCODE_GOOD)
	{
		fprintf(stderr, "Server exited with status: %s\n", UA_StatusCode_name(status));
	}
}

void run_server_variable(void)
{
	/* Create a server listening on port 4840 (default) */
    UA_Server *server = UA_Server_new();

    /* Add a variable node to the server */

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

    /* Run the server (until ctrl-c interrupt) */
    UA_StatusCode status = UA_Server_runUntilInterrupt(server);

    /* Clean up */
    UA_Server_delete(server);
    //return status == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE;
}

int main(void)
{
	run_server_variable();
	return 0;
}
