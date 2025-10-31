/**
 * Part of opcua-modbus-gateway project subjected to terms of
 * MIT license agreement. A license file is distributed with
 * the project. 
 * @Author: Ritesh Sharma
 * @Date: 31-10-2025
 * @Detail: modbus client
 */
#include <stdio.h>
#include <errno.h>
#include "modbus_client.h"

modbus_t* init_modbus_client(const char* device, int baud, char parity, int data_bit, int stop_bit)
{
    modbus_t *ctx = modbus_new_rtu(device, baud, parity, data_bit, stop_bit);
    if (ctx == NULL) {
        fprintf(stderr, "Unable to allocate libmodbus context\n");
        return NULL;
    }

    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return NULL;
    }

    return ctx;
}

void close_modbus_client(modbus_t* ctx)
{
    if (ctx) {
        modbus_close(ctx);
        modbus_free(ctx);
    }
}