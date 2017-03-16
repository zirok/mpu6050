#pragma once

#include "stm32f4xx_hal.h"

#define I2C_TRAILS							3

class DataI2CTransfer
{
	I2C_HandleTypeDef* _i2c;
	uint8_t _timeOut;

public:	
	DataI2CTransfer(I2C_HandleTypeDef * i2c, uint8_t timeOut);

	HAL_StatusTypeDef IsDeviceReady(uint16_t deviceAddress);
	
	HAL_StatusTypeDef ReadByte(uint16_t deviceAddress, uint8_t registerAddress, uint8_t* data);

	HAL_StatusTypeDef ReadBytes(uint16_t deviceAddress, uint8_t registerAddress, uint16_t size, uint8_t* data);

	HAL_StatusTypeDef WriteByte(uint16_t deviceAddress, uint8_t registerAddress, uint8_t data);
};

