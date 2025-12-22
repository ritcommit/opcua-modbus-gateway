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

/*****************GLOBAL VARIABLES******************/
const int mb_start_addr[MODBUS_DTYPE_MAX] = {1, 10001, 30001, 40001};

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

    printf("SUCCESS: Modbus %s created\r\n", (MODBUS_TYPE_RTU == cfg.modbus_type)?"slave":"client");

    return 0;
}

void close_modbus_client(void)
{
    if (ctx) {
        modbus_close(ctx);
        modbus_free(ctx);
    }
}

modbus_dtype_t detectMbDatatype(int regaddr)
{
    if ((regaddr >= 1) && (regaddr <= 9999)) return MODBUS_DTYPE_OC;
    if ((regaddr >= 10001) && (regaddr <= 19999)) return MODBUS_DTYPE_DI;
    if ((regaddr >= 30001) && (regaddr <= 39999)) return MODBUS_DTYPE_IR;
    if ((regaddr >= 40001) && (regaddr <= 49999)) return MODBUS_DTYPE_HR;
    return MODBUS_DTYPE_MAX;
}

int mbclient_read_output_coils(int addr, int nb, uint8_t* dh)
{
    if (ctx != NULL)
    {
       return modbus_read_bits(ctx, addr, nb, dh);
    }
}

int mbclient_read_discrete_inputs(int addr, int nb, uint8_t* dh)
{
    if (ctx != NULL)
    {
       return modbus_read_input_bits(ctx, addr, nb, dh);
    }
}

int mbclient_read_input_registers(int addr, int nb, uint16_t* dh)
{
    if (ctx != NULL)
    {
       return modbus_read_input_registers(ctx, addr, nb, dh);
    }
}

int mbclient_read_holding_registers(int addr, int nb, uint16_t* dh)
{
    if (ctx != NULL)
    {
        return modbus_read_registers(ctx, addr, nb, dh);
    }
    else
    {
        static int counter = 0;
        if (counter > 0xFFFFFFFF)
        {
            counter = 0;
        }
        *dh = counter++;
        return nb;
    }
}