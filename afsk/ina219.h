/*
 * Modified from https://github.com/foglamp/foglamp-south-ina219/blob/develop/include/ina219.h
 *
 * FogLAMP south service plugin
 *
 * Copyright (c) 2018 Dianomic Systems
 *
 * Released under the Apache 2.0 Licence
 *
 * Author: Mark Riddoch
 */
#/**
 * Values for the gain setting
 */;
enum {
	INA219_CONFIG_GAIN_40MV  =       0x0000,  // 40mV Range
	INA219_CONFIG_GAIN_80MV  =       0x0800,  // 80mV Range
	INA219_CONFIG_GAIN_160MV =       0x1000,  // 160mV Range
	INA219_CONFIG_GAIN_320MV =       0x1800,  // 320mV Range
};

enum
{
	INA219_CONFIG_BVOLTAGERANGE_16V =        0x0000,  // 0-16V Range
	INA219_CONFIG_BVOLTAGERANGE_32V =        0x2000,  // 0-32V Range
};

#define INA219_CONFIG_BADCRES_MASK	0x0780 // ADC Resulotion Mask
enum
{
	INA219_CONFIG_BADCRES_9BIT  =    0x0000,  // 9-bit bus res = 0..511
	INA219_CONFIG_BADCRES_10BIT =    0x0080,  // 10-bit bus res = 0..1023
	INA219_CONFIG_BADCRES_11BIT =    0x0100,  // 11-bit bus res = 0..2047
	INA219_CONFIG_BADCRES_12BIT =    0x0180,  // 12-bit bus res = 0..4097
};

#define INA219_CONFIG_SADCRES_MASK	0x0078 // Mask for shunt ADC resolution bits
enum
{
	INA219_CONFIG_SADCRES_9BIT_1S_84US     = 0x00,  // 1 x 9-bit shunt sample
	INA219_CONFIG_SADCRES_10BIT_1S_148US   = 0x08,  // 1 x 10-bit shunt sample
	INA219_CONFIG_SADCRES_11BIT_1S_276US   = 0x10,  // 1 x 11-bit shunt sample
	INA219_CONFIG_SADCRES_12BIT_1S_532US   = 0x18,  // 1 x 12-bit shunt sample
	INA219_CONFIG_SADCRES_12BIT_2S_1060US  = 0x48,	// 2 x 12-bit shunt samples averaged together
	INA219_CONFIG_SADCRES_12BIT_4S_2130US  = 0x50,  // 4 x 12-bit shunt samples averaged together
	INA219_CONFIG_SADCRES_12BIT_8S_4260US  = 0x58,  // 8 x 12-bit shunt samples averaged together
	INA219_CONFIG_SADCRES_12BIT_16S_8510US = 0x60,  // 16 x 12-bit shunt samples averaged together
	INA219_CONFIG_SADCRES_12BIT_32S_17MS   = 0x68,  // 32 x 12-bit shunt samples averaged together
	INA219_CONFIG_SADCRES_12BIT_64S_34MS   = 0x70,  // 64 x 12-bit shunt samples averaged together
	INA219_CONFIG_SADCRES_12BIT_128S_69MS  = 0x78,  // 128 x 12-bit shunt samples averaged together
};

#define INA219_CONFIG_MODE_MASK		0x0007  // Operating Mode Mask
enum {   
	INA219_CONFIG_MODE_POWERDOWN            = 0x0000,
	INA219_CONFIG_MODE_SVOLT_TRIGGERED      = 0x0001,
	INA219_CONFIG_MODE_BVOLT_TRIGGERED      = 0x0002,
	INA219_CONFIG_MODE_SANDBVOLT_TRIGGERED  = 0x0003,
	INA219_CONFIG_MODE_ADCOFF               = 0x0004,
	INA219_CONFIG_MODE_SVOLT_CONTINUOUS     = 0x0005,
	INA219_CONFIG_MODE_BVOLT_CONTINUOUS     = 0x0006,
	INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS = 0x0007,	
};

typedef enum {
	CONF_16V_400MA,
	CONF_32V_1A,
	CONF_32V_2A
} INA219_CONFIGURATION;

/*
 * INA219 Registers
 */
#define INA219_REG_CONFIG 	                   0x00
#define INA219_REG_SHUNTVOLTAGE                0x01
#define INA219_REG_BUSVOLTAGE                  0x02
#define INA219_REG_POWER                       0x03
#define INA219_REG_CURRENT                     0x04
#define INA219_REG_CALIBRATION                 0x05
