#include "DataI2CTransfer.h"



DataI2CTransfer::DataI2CTransfer(I2C_HandleTypeDef* i2c, uint8_t timeOut)
	: _i2c(i2c)
	, _timeOut(timeOut)
{
}

HAL_StatusTypeDef DataI2CTransfer::IsDeviceReady(uint16_t deviceAddress)
{
	return HAL_I2C_IsDeviceReady(_i2c, deviceAddress, I2C_TRAILS, _timeOut);
}

HAL_StatusTypeDef DataI2CTransfer::ReadByte(uint16_t deviceAddress, uint8_t registerAddress, uint8_t* data)
{
	auto status = HAL_OK;
	
	while (HAL_I2C_GetState(_i2c) != HAL_I2C_STATE_READY)
		;
	status = HAL_I2C_Master_Transmit(_i2c, deviceAddress, &registerAddress, 1, _timeOut);

	if (status != HAL_OK) 
		return status;	

	while (HAL_I2C_GetState(_i2c) != HAL_I2C_STATE_READY)
		;

	status = HAL_I2C_Master_Receive(_i2c, deviceAddress, data, 1, _timeOut);

	if (status != HAL_OK) 
		return status;		

	return status;
}

HAL_StatusTypeDef DataI2CTransfer::ReadBytes(uint16_t deviceAddress, uint8_t registerAddress, uint16_t size, uint8_t* data)
{
	auto status = HAL_OK;
	
	while (HAL_I2C_GetState(_i2c) != HAL_I2C_STATE_READY)
		;

	uint8_t startRegisterAddress = registerAddress;

	for (uint16_t i = 0; i < size; i++)
	{			
		status = HAL_I2C_Master_Transmit(_i2c, deviceAddress, &startRegisterAddress, 1, _timeOut);

		if (status != HAL_OK) 
			return status;	

		while (HAL_I2C_GetState(_i2c) != HAL_I2C_STATE_READY)
			;

		uint8_t *nextAddress = data + static_cast<uint8_t>(i);

		status = HAL_I2C_Master_Receive(_i2c, deviceAddress, nextAddress, 1, _timeOut);

		if (status != HAL_OK) 
			return status;		
		
		startRegisterAddress++;
	}

	return status;
}

HAL_StatusTypeDef DataI2CTransfer::WriteByte(uint16_t deviceAddress, uint8_t registerAddress, uint8_t data)
{
	uint8_t buffer[] = { registerAddress, data };
	
	while (HAL_I2C_GetState(_i2c) != HAL_I2C_STATE_READY)
		;

	auto status = HAL_I2C_Master_Transmit(_i2c, deviceAddress, buffer, 2, _timeOut);

	if (status != HAL_OK) 
		return status;
	
	return HAL_OK;
}