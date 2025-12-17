/**
 * Part of opcua-modbus-gateway project subjected to terms of
 * MIT license agreement. A license file is distributed with
 * the project. 
 * @Author: Ritesh Sharma
 * @Date: 31-10-2025
 * @Detail: modbus client header file
 */

#ifndef GWY_MODBUS_CLIENT_H
#define GWY_MODBUS_CLIENT_H

/*********************INCLUDES**********************/
#include <modbus/modbus.h>
#include <stdint.h>
#include "config.h"

/************GLOBAL FUNCTION PROTOTYPES*************/
int init_modbus_client(gateway_config_t cfg);
void close_modbus_client(void);
int mbclient_read_output_coils(int addr, int nb, uint8_t* dh);
int mbclient_read_discrete_inputs(int addr, int nb, uint8_t* dh);
int mbclient_read_input_registers(int addr, int nb, uint16_t* dh);
int mbclient_read_holding_registers(int addr, int nb, uint16_t* dh);

#endif /*GWY_MODBUS_CLIENT_H*/