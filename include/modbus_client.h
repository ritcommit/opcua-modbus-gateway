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
#include "config.h"

int init_modbus_client(gateway_config_t cfg);
void close_modbus_client(void);

#endif /*GWY_MODBUS_CLIENT_H*/