cmake_minimum_required(VERSION 3.10)

project(opcua_modbus_gateway C)

# C Standara
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Directories
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_SOURCE_DIR}/include)

# include paths
include_directories(${INC_DIR})

# compiler options
# add_compile_options(-Wall -Wextra)

# sources
set(SRC_FILES ${SRC_DIR}/main.c ${SRC_DIR}/open62541.c)

# executable
add_executable(opcua_modbus_gateway ${SRC_FILES})

