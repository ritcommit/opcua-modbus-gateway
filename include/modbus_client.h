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

#include <modbus/modbus.h>

modbus_t* init_modbus_client(const char* device, int baud, \
    char parity, int data_bit, int stop_bit);
void close_modbus_client(modbus_t* ctx);

#endif /*GWY_MODBUS_CLIENT_H*/