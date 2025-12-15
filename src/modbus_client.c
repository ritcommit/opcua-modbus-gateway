/**
 * Part of opcua-modbus-gateway project subjected to terms of
 * MIT license agreement. A license file is distributed with
 * the project. 
 * @Author: Ritesh Sharma
 * @Date: 31-10-2025
 * @Detail: modbus client
 */
/*********************INCLUDES**********************/
#include <stdio.h>
#include <errno.h>
#include "modbus_client.h"

/**********************MACROS***********************/
#define MODBUS_DATA_BITS 8
#define MODBUS_STOP_BITS 1

/*****************LOCAL VARIABLES ******************/
static modbus_t *ctx = NULL;

/****************GLOBAL FUNCTIONS*******************/
int init_modbus_client(gateway_config_t cfg)
{
    if (MODBUS_TYPE_RTU == cfg.modbus_type)
    {
        ctx = modbus_new_rtu(cfg.modbus_address, cfg.modbus_baudrate, cfg.modbus_parity, MODBUS_DATA_BITS, MODBUS_STOP_BITS);
    }
    else if (MODBUS_TYPE_TCP == cfg.modbus_type)
    {
        ctx = modbus_new_tcp(cfg.modbus_address, cfg.modbus_port);
    }
    else
    {
        fprintf(stderr, "Wrong modbus connection credentials\n");
        return -1;
    }

    if (ctx == NULL) {
        fprintf(stderr, "Unable to allocate libmodbus context\n");
        return -2;
    }

    int res = modbus_set_slave(ctx, cfg.modbus_slaveid);
    if (res == -1) {
        fprintf(stderr, "Invalid slave ID\n");
        modbus_free(ctx);
        return -3;
    }

    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -4;
    }

    return 0;
}

void close_modbus_client(void)
{
    if (ctx) {
        modbus_close(ctx);
        modbus_free(ctx);
    }
}