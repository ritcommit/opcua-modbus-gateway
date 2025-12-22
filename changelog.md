# Changelog #

## [Unreleased](#) ##

**Added** 

* Initial code with modbus client and opcua servers defined
* Json parser for parsing connection and data configuration
* Dynamic opcua node object creation using `config/config.json`
* Callback for reading modbus data and updating opc-ua node added
* Modbus read methods using libmodbus api added
* Keeping a dummy variable in holding register method for testing
* Auto detection of modbus data type based on register address and exit if address out of range
* Tested with modbus slave simulator [OpenModSim](https://github.com/sanny32/OpenModSim)

**Changed** 
* Config data structure changed for better parsing
* Removed mb datatype field from config
* Removed scaling factor from config
* Not setting ua node value during initialisation