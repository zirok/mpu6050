#include "MPU6050.h"
#include "stm32f4xx_hal_tim.h"


MPU6050::MPU6050(DataI2CTransfer* transfer, MPU6050_DeviceNumber deviceNumber)
	: _transfer(transfer)	
	, _deviceNumber(deviceNumber)
{
	_deviceAddress = MPU6050_DEFAULT_ADDRESS | (uint8_t)deviceNumber;		
}

MPU6050::~MPU6050()
{
}

MPU6050_ErrorCode MPU6050::GetLastError() const
{
	return _lastResult;
}

bool MPU6050::IsDeviceMPU6050()
{
	HAL_StatusTypeDef s = _transfer->IsDeviceReady(_deviceAddress);
		
	if (s !=  HAL_OK) {
		
		_lastResult = MPU6050_ErrorDeviceNotConnected;

		return false;		
	}
	
	uint8_t buffer;

	_transfer->ReadByte(_deviceAddress, MPU6050_WHO_AM_I, &buffer);	
		
	if (buffer != MPU6050_I_AM_VALUE) {
		
		_lastResult = MPU6050_ErrorDeviceInvalid;

		return false;
	}

	_lastResult = MPU6050_Ok;

	return true;
}

MPU6050_ErrorCode MPU6050::DeviceReset()
{
	uint8_t buffer;

	auto status =  _transfer->ReadByte(_deviceAddress, MPU6050_PWR_MGMT_1, &buffer);
	
	if (status != HAL_OK)
	{
		this->_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;

		return _lastResult;
	}

	buffer = MPU6050_SET_VALUE(buffer, MPU6050_BIT_SET, MPU6050_PWR_MGMT_1_DEVICE_RESET_BIT, MPU6050_PWR_MGMT_1_DEVICE_RESET_LEN);

	status = _transfer->WriteByte(_deviceAddress, MPU6050_PWR_MGMT_1, buffer);

	if (status != HAL_OK)	
		this->_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;		
	else
		this->_lastResult =  MPU6050_ErrorCode::MPU6050_Ok;	


	buffer = 0x07; //Reset All signals

	HAL_Delay(100);

	status = _transfer->WriteByte(_deviceAddress, MPU6050_SIGNAL_PATH_RESET, buffer);

	if (status != HAL_OK)	
		this->_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;		
	else
		this->_lastResult =  MPU6050_ErrorCode::MPU6050_Ok;	

	HAL_Delay(100);

	return _lastResult;	
}

MPU6050_SleepMode MPU6050::GetSleepMode()
{
	uint8_t buffer;
	
	auto status = _transfer->ReadByte(_deviceAddress, MPU6050_PWR_MGMT_1, &buffer);

	if (status != HAL_OK)
	{
		this->_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;

		return MPU6050_SleepMode::MPU6050_Sleep;
	}

	auto isSleep  = MPU6050_CONVERT(buffer, MPU6050_PWR_MGMT_1_SLEEP_BIT, MPU6050_PWR_MGMT_1_SLEEP_LEN);

	auto isCycle  = MPU6050_CONVERT(buffer, MPU6050_PWR_MGMT_1_CYCLE_BIT, MPU6050_PWR_MGMT_1_CYCLE_LEN);

	if (isSleep == MPU6050_BIT_RESET)
	{	
		return isCycle ? MPU6050_SleepMode::MPU6050_Cycle : MPU6050_SleepMode::MPU6050_WakeUp;
	}

	return MPU6050_SleepMode::MPU6050_Sleep;
}

MPU6050_ErrorCode MPU6050::GetPowerMode(MPU6050_PowerMode* powerMode)
{		
	uint8_t buffer;
	
	auto status = _transfer->ReadByte(_deviceAddress, MPU6050_PWR_MGMT_2, &buffer);

	if (status != HAL_OK)	
	{
		_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;
		
		return _lastResult;
	}

	auto wakeCtrl = MPU6050_CONVERT(buffer, MPU6050_PWR_MGMT_2_LP_WAKE_CTRL_BIT, MPU6050_PWR_MGMT_2_LP_WAKE_CTRL_LEN);

	powerMode->WakeUpFrequency = static_cast<MPU6050_WakeUpFrequency>(wakeCtrl);

	auto standbyMode = MPU6050_CONVERT(buffer, MPU6050_PWR_MGMT_2_STBY_BIT, MPU6050_PWR_MGMT_2_STBY_LEN);

	powerMode->StandbyMode = static_cast<MPU6050_StandbyMode>(standbyMode);

	_lastResult =  MPU6050_ErrorCode::MPU6050_Ok;

	return _lastResult;
	
}

MPU6050_ErrorCode MPU6050::SetPowerMode(const MPU6050_PowerMode* powerMode)
{
	uint8_t buffer;
	
	auto status = _transfer->ReadByte(_deviceAddress, MPU6050_PWR_MGMT_2, &buffer);

	if (status != HAL_OK)	
	{
		this->_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;
		
		return this->_lastResult;
	}

	buffer =  MPU6050_SET_VALUE(buffer, static_cast<uint8_t>(powerMode->WakeUpFrequency), MPU6050_PWR_MGMT_2_LP_WAKE_CTRL_BIT, MPU6050_PWR_MGMT_2_LP_WAKE_CTRL_LEN);

	buffer =  MPU6050_SET_VALUE(buffer, static_cast<uint8_t>(powerMode->StandbyMode), MPU6050_PWR_MGMT_2_STBY_BIT, MPU6050_PWR_MGMT_2_STBY_LEN);

	status = _transfer->WriteByte(_deviceAddress, MPU6050_PWR_MGMT_2, buffer);

	if (status != HAL_OK)	
	{
		this->_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;
		
		return this->_lastResult;
	}

	_lastResult =  MPU6050_ErrorCode::MPU6050_Ok;

	return _lastResult;
}

MPU6050_ErrorCode MPU6050::SetPowerEnabledTemperatureSensor(bool value)
{
	uint8_t buffer;
	
	auto status = _transfer->ReadByte(_deviceAddress, MPU6050_PWR_MGMT_1, &buffer);

	if (status != HAL_OK)	
	{
		this->_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;
		
		return this->_lastResult;
	}

	if (value)	
		buffer =  MPU6050_SET_VALUE(buffer, MPU6050_BIT_RESET, MPU6050_PWR_MGMT_1_TEMP_DIS_BIT, MPU6050_PWR_MGMT_1_TEMP_DIS_LEN);		
	else
		buffer =  MPU6050_SET_VALUE(buffer, MPU6050_BIT_SET, MPU6050_PWR_MGMT_1_TEMP_DIS_BIT, MPU6050_PWR_MGMT_1_TEMP_DIS_LEN);		
	
	status = _transfer->WriteByte(_deviceAddress, MPU6050_PWR_MGMT_1, buffer);

	if (status != HAL_OK)	
		this->_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;	
	else
		this->_lastResult =  MPU6050_ErrorCode::MPU6050_Ok;	

	return _lastResult;
}

bool MPU6050::GetPowerEnabledTemperatureSensor()
{
	uint8_t buffer = 0;
	
	auto status = _transfer->ReadByte(_deviceAddress, MPU6050_PWR_MGMT_1, &buffer);

	this->_lastResult =  MPU6050_ErrorCode::MPU6050_Ok;	

	if (status != HAL_OK)	
	{
		this->_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;
		
		return false;
	}
	
	auto disabled =  MPU6050_CONVERT(buffer, MPU6050_PWR_MGMT_1_TEMP_DIS_BIT, MPU6050_PWR_MGMT_1_TEMP_DIS_LEN);			

	if (status != HAL_OK)	
		this->_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;		
		
	return disabled == 0;
}

MPU6050_ClockSourceDevice MPU6050::GetClockSourceDevice()
{
	uint8_t buffer;
	
	auto status = _transfer->ReadByte(_deviceAddress, MPU6050_PWR_MGMT_1, &buffer);

	if (status != HAL_OK)	
	{
		_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;
		
		return MPU6050_ClockSourceDevice::MPU6050_Internal8MHz;
	}

	_lastResult =  MPU6050_ErrorCode::MPU6050_Ok;

	auto clkSel = MPU6050_CONVERT(buffer, MPU6050_PWR_MGMT_1_CLKSEL_BIT, MPU6050_PWR_MGMT_1_CLKSEL_LEN);

	return static_cast<MPU6050_ClockSourceDevice>(clkSel);
}

MPU6050_ErrorCode MPU6050::SetClockSourceDevice(MPU6050_ClockSourceDevice value)
{
	uint8_t buffer;
	
	auto status = _transfer->ReadByte(_deviceAddress, MPU6050_PWR_MGMT_1, &buffer);

	if (status != HAL_OK)	
	{
		_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;
		
		return _lastResult;
	}

	_lastResult =  MPU6050_ErrorCode::MPU6050_Ok;

	buffer = MPU6050_SET_VALUE(buffer, static_cast<uint8_t>(value), MPU6050_PWR_MGMT_1_CLKSEL_BIT, MPU6050_PWR_MGMT_1_CLKSEL_LEN);
	
	status = _transfer->WriteByte(_deviceAddress, MPU6050_PWR_MGMT_1, buffer);

	if (status != HAL_OK)	
	{
		_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;
	}

	return _lastResult;	
}


MPU6050_ErrorCode MPU6050::SetSleepMode(MPU6050_SleepMode mode)
{
	uint8_t buffer;
	
	auto status = _transfer->ReadByte(_deviceAddress, MPU6050_PWR_MGMT_1, &buffer);

	if (status != HAL_OK)	
	{
		this->_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;
		
		return this->_lastResult;
	}

	if (mode == MPU6050_SleepMode::MPU6050_Cycle)
	{
		buffer =  MPU6050_SET_VALUE(buffer, MPU6050_BIT_RESET, MPU6050_PWR_MGMT_1_SLEEP_BIT, MPU6050_PWR_MGMT_1_SLEEP_LEN);
		buffer =  MPU6050_SET_VALUE(buffer, MPU6050_BIT_SET, MPU6050_PWR_MGMT_1_CYCLE_BIT, MPU6050_PWR_MGMT_1_CYCLE_LEN);
	}
	else if (mode == MPU6050_SleepMode::MPU6050_WakeUp)
	{
		
		buffer =  MPU6050_SET_VALUE(buffer, MPU6050_BIT_RESET, MPU6050_PWR_MGMT_1_SLEEP_BIT, MPU6050_PWR_MGMT_1_SLEEP_LEN);
		buffer =  MPU6050_SET_VALUE(buffer, MPU6050_BIT_RESET, MPU6050_PWR_MGMT_1_CYCLE_BIT, MPU6050_PWR_MGMT_1_CYCLE_LEN);
	}
	else
	{
		buffer =  MPU6050_SET_VALUE(buffer, MPU6050_BIT_SET, MPU6050_PWR_MGMT_1_SLEEP_BIT, MPU6050_PWR_MGMT_1_SLEEP_LEN);
		buffer =  MPU6050_SET_VALUE(buffer, MPU6050_BIT_RESET, MPU6050_PWR_MGMT_1_CYCLE_BIT, MPU6050_PWR_MGMT_1_CYCLE_LEN);
	}
	
	status = _transfer->WriteByte(_deviceAddress, MPU6050_PWR_MGMT_1, buffer);

	if (status != HAL_OK)	
		this->_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;	
	else
		this->_lastResult =  MPU6050_ErrorCode::MPU6050_Ok;	

	return _lastResult;
}	

MPU6050_ErrorCode MPU6050::SelfTest(MPU6050_SelfTestTypeDef* data)
{
	uint8_t buffer[4];
	
	auto status = _transfer->ReadBytes(_deviceAddress, MPU6050_SELF_TEST_X, 4, buffer);

	if (status != HAL_OK)
		return MPU6050_ErrorCode::MPU6050_ErrorFailure;

	data->X.A  = MPU6050_CONVERT(buffer[0], MPU6050_SELF_TEST_XA_BIT, MPU6050_SELF_TEST_XA_LEN);
	data->X.G  = MPU6050_CONVERT(buffer[0], MPU6050_SELF_TEST_XG_BIT, MPU6050_SELF_TEST_XG_LEN);

	data->Y.A  = MPU6050_CONVERT(buffer[1], MPU6050_SELF_TEST_YA_BIT, MPU6050_SELF_TEST_YA_LEN);
	data->Y.G  = MPU6050_CONVERT(buffer[1], MPU6050_SELF_TEST_YG_BIT, MPU6050_SELF_TEST_YG_LEN);

	data->Z.A  = MPU6050_CONVERT(buffer[2], MPU6050_SELF_TEST_ZA_BIT, MPU6050_SELF_TEST_ZA_LEN);
	data->Z.G  = MPU6050_CONVERT(buffer[2], MPU6050_SELF_TEST_ZG_BIT, MPU6050_SELF_TEST_ZG_LEN);

	data->A.Xa = MPU6050_CONVERT(buffer[3], MPU6050_SELF_TEST_AX_BIT, MPU6050_SELF_TEST_AX_LEN);
	data->A.Ya = MPU6050_CONVERT(buffer[3], MPU6050_SELF_TEST_AY_BIT, MPU6050_SELF_TEST_AY_LEN);
	data->A.Za = MPU6050_CONVERT(buffer[3], MPU6050_SELF_TEST_AZ_BIT, MPU6050_SELF_TEST_AZ_LEN);

	return MPU6050_ErrorCode::MPU6050_Ok;
}

MPU6050_ErrorCode MPU6050::GetConfig(MPU6050_Config* config)
{
	uint8_t buffer;
	
	auto status = _transfer->ReadByte(_deviceAddress, MPU6050_CONFIG, &buffer);

	if (status != HAL_OK)	
	{
		this->_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;
		
		return this->_lastResult;
	}

	config->ExternalFrameSynchronization = static_cast<MPU6050_ExternalFrameSynchronization>(MPU6050_CONVERT(buffer, MPU6050_CONFIG_EXT_SYNC_SET_BIT, MPU6050_CONFIG_EXT_SYNC_SET_LEN));

	config->DigitalLowPassFilter.Config = static_cast<MPU6050_DigitalLowPassFilterConfig>(MPU6050_CONVERT(buffer, MPU6050_CONFIG_EXT_DLPF_CFG_BIT, MPU6050_CONFIG_EXT_DLPF_CFG_LEN));
	
	switch (config->DigitalLowPassFilter.Config)
	{
	case MPU6050_DigitalLowPassFilterConfig::MPU6050_DLPFConfig0 : {
		
			config->DigitalLowPassFilter.AccelerometerBandwidth = 260;
			config->DigitalLowPassFilter.AccelerometerDelay = 0;
			config->DigitalLowPassFilter.GyroscopeBandwidthHz = 256;
			config->DigitalLowPassFilter.GyroscopeDelay = 980;
			config->DigitalLowPassFilter.GyroscopeOutputRateHz = 8000;
			break;
		}
	case MPU6050_DigitalLowPassFilterConfig::MPU6050_DLPFConfig1 : {
		
			config->DigitalLowPassFilter.AccelerometerBandwidth = 184;
			config->DigitalLowPassFilter.AccelerometerDelay = 2000;
			config->DigitalLowPassFilter.GyroscopeBandwidthHz = 188;
			config->DigitalLowPassFilter.GyroscopeDelay = 1900;
			config->DigitalLowPassFilter.GyroscopeOutputRateHz = 1000;
			break;
		}
	case MPU6050_DigitalLowPassFilterConfig::MPU6050_DLPFConfig2 : {
		
			config->DigitalLowPassFilter.AccelerometerBandwidth = 94;
			config->DigitalLowPassFilter.AccelerometerDelay = 3000;
			config->DigitalLowPassFilter.GyroscopeBandwidthHz = 98;
			config->DigitalLowPassFilter.GyroscopeDelay = 2800;
			config->DigitalLowPassFilter.GyroscopeOutputRateHz = 1000;
			break;
		}
	case MPU6050_DigitalLowPassFilterConfig::MPU6050_DLPFConfig3 : {
		
			config->DigitalLowPassFilter.AccelerometerBandwidth = 44;
			config->DigitalLowPassFilter.AccelerometerDelay = 4900;
			config->DigitalLowPassFilter.GyroscopeBandwidthHz = 42;
			config->DigitalLowPassFilter.GyroscopeDelay = 4800;
			config->DigitalLowPassFilter.GyroscopeOutputRateHz = 1000;
			break;
		}
	case MPU6050_DigitalLowPassFilterConfig::MPU6050_DLPFConfig4 : {
		
			config->DigitalLowPassFilter.AccelerometerBandwidth = 21;
			config->DigitalLowPassFilter.AccelerometerDelay = 8500;
			config->DigitalLowPassFilter.GyroscopeBandwidthHz = 20;
			config->DigitalLowPassFilter.GyroscopeDelay = 8300;
			config->DigitalLowPassFilter.GyroscopeOutputRateHz = 1000;
			break;
		}
	case MPU6050_DigitalLowPassFilterConfig::MPU6050_DLPFConfig5 : {
		
			config->DigitalLowPassFilter.AccelerometerBandwidth = 10;
			config->DigitalLowPassFilter.AccelerometerDelay = 13800;
			config->DigitalLowPassFilter.GyroscopeBandwidthHz = 10;
			config->DigitalLowPassFilter.GyroscopeDelay = 13400;
			config->DigitalLowPassFilter.GyroscopeOutputRateHz = 1000;
			break;
		}
	case MPU6050_DigitalLowPassFilterConfig::MPU6050_DLPFConfig6 : {
		
			config->DigitalLowPassFilter.AccelerometerBandwidth = 5;
			config->DigitalLowPassFilter.AccelerometerDelay = 19000;
			config->DigitalLowPassFilter.GyroscopeBandwidthHz = 5;
			config->DigitalLowPassFilter.GyroscopeDelay = 18600;
			config->DigitalLowPassFilter.GyroscopeOutputRateHz = 1000;
			break;
		}
	case MPU6050_DigitalLowPassFilterConfig::MPU6050_DLPFConfig7 : {
		
			config->DigitalLowPassFilter.AccelerometerBandwidth = 0xFFFF;
			config->DigitalLowPassFilter.AccelerometerDelay = 0xFFFF;
			config->DigitalLowPassFilter.GyroscopeBandwidthHz = 0xFFFF;
			config->DigitalLowPassFilter.GyroscopeDelay = 0xFFFF;
			config->DigitalLowPassFilter.GyroscopeOutputRateHz = 8000;
			break;
		}
	default:
		{
			this->_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;
		}		
	}

	this->_lastResult =  MPU6050_ErrorCode::MPU6050_Ok;
		
	return this->_lastResult;
}

MPU6050_ErrorCode MPU6050::SetConfig(MPU6050_ExternalFrameSynchronization externalFrameSynchronization, MPU6050_DigitalLowPassFilterConfig digitalLowPassFilterConfig)
{
	uint8_t buffer = 0x00;

	buffer = MPU6050_SET_VALUE(buffer, static_cast<uint8_t>(externalFrameSynchronization), MPU6050_CONFIG_EXT_SYNC_SET_BIT, MPU6050_CONFIG_EXT_SYNC_SET_LEN);
	buffer = MPU6050_SET_VALUE(buffer, static_cast<uint8_t>(digitalLowPassFilterConfig), MPU6050_CONFIG_EXT_DLPF_CFG_BIT, MPU6050_CONFIG_EXT_DLPF_CFG_LEN);

	auto halStatus = _transfer->WriteByte(_deviceAddress, MPU6050_CONFIG, buffer);

	this->_lastResult =  MPU6050_ErrorCode::MPU6050_Ok;

	if (halStatus != HAL_OK)		
		this->_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;			
	
	return this->_lastResult;
}

MPU6050_ErrorCode MPU6050::SetSampleRate(uint8_t sampleRateDivisor)
{
	auto halStatus = _transfer->WriteByte(_deviceAddress, MPU6050_SMPRT_DIV_A, sampleRateDivisor);

	this->_lastResult =  MPU6050_ErrorCode::MPU6050_Ok;

	if (halStatus != HAL_OK)		
		this->_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;			
	
	return this->_lastResult;
}

MPU6050_GyroscopeFullScaleRange MPU6050::GetGyroscopeFullScaleRange()
{
	uint8_t buffer;
	
	auto status = _transfer->ReadByte(_deviceAddress, MPU6050_GYRO_CONFIG, &buffer);

	if (status != HAL_OK)	
	{
		_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;
		
		return MPU6050_GyroscopeFullScaleRange::MPU6050_GyroscopeScaleRange250s;
	}

	_lastResult =  MPU6050_ErrorCode::MPU6050_Ok;

	return static_cast<MPU6050_GyroscopeFullScaleRange>(MPU6050_CONVERT(buffer, MPU6050_GYRO_CONFIG_FS_SEL_BIT, MPU6050_GYRO_CONFIG_FS_SEL_LEN));	
}

MPU6050_ErrorCode MPU6050::SetGyroscopeFullScaleRange(MPU6050_GyroscopeFullScaleRange fullScaleRange)
{
	uint8_t buffer;
	
	auto status = _transfer->ReadByte(_deviceAddress, MPU6050_GYRO_CONFIG, &buffer);

	if (status != HAL_OK)	
	{
		_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;
		
		return _lastResult;
	}
	
	buffer = MPU6050_SET_VALUE(buffer, static_cast<uint8_t>(fullScaleRange), MPU6050_GYRO_CONFIG_FS_SEL_BIT, MPU6050_GYRO_CONFIG_FS_SEL_LEN);	


	auto halStatus = _transfer->WriteByte(_deviceAddress, MPU6050_GYRO_CONFIG, buffer);

	this->_lastResult =  MPU6050_ErrorCode::MPU6050_Ok;

	if (halStatus != HAL_OK)		
		this->_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;			
	
	return this->_lastResult;
}

MPU6050_GyroscopeSeftTestAxis MPU6050::GetGyroscopeSeftTestAxis()
{
	uint8_t buffer = 0x00;
	
	auto status = _transfer->ReadByte(_deviceAddress, MPU6050_GYRO_CONFIG, &buffer);

	if (status != HAL_OK)	
	{
		_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;
		
		return MPU6050_GyroscopeSeftTestAxis::MPU6050_GyroscopeSelfTestOff;
	}

	_lastResult =  MPU6050_ErrorCode::MPU6050_Ok;

	return static_cast<MPU6050_GyroscopeSeftTestAxis>(MPU6050_CONVERT(buffer, MPU6050_GYRO_CONFIG_G_ST_BIT, MPU6050_GYRO_CONFIG_G_ST_LEN));	
}

MPU6050_ErrorCode MPU6050::SetGyroscopeSeftTestAxis(MPU6050_GyroscopeSeftTestAxis seftTestAxis)
{
	auto code = this->SetGyroscopeFullScaleRange(MPU6050_GyroscopeFullScaleRange::MPU6050_GyroscopeScaleRange250s);

	if (code != MPU6050_ErrorCode::MPU6050_Ok)
		return code;

	uint8_t buffer;
	
	auto status = _transfer->ReadByte(_deviceAddress, MPU6050_GYRO_CONFIG, &buffer);

	if (status != HAL_OK)	
	{
		_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;
		
		return _lastResult;
	}
	
	buffer = MPU6050_SET_VALUE(buffer, static_cast<uint8_t>(seftTestAxis), MPU6050_GYRO_CONFIG_G_ST_BIT, MPU6050_GYRO_CONFIG_G_ST_LEN);	


	auto halStatus = _transfer->WriteByte(_deviceAddress, MPU6050_GYRO_CONFIG, buffer);

	this->_lastResult =  MPU6050_ErrorCode::MPU6050_Ok;

	if (halStatus != HAL_OK)		
		this->_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;			
	
	return this->_lastResult;
}

MPU6050_AccelerometerFullScaleRange MPU6050::GetAccelerometerFullScaleRange()
{
	uint8_t buffer;
	
	auto status = _transfer->ReadByte(_deviceAddress, MPU6050_ACCEL_CONFIG, &buffer);

	if (status != HAL_OK)	
	{
		_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;
		
		return MPU6050_AccelerometerFullScaleRange::MPU6050_AccelerometerScaleRange2G;
	}

	_lastResult =  MPU6050_ErrorCode::MPU6050_Ok;

	return static_cast<MPU6050_AccelerometerFullScaleRange>(MPU6050_CONVERT(buffer, MPU6050_ACCEL_CONFIG_AFA_SEL_BIT, MPU6050_ACCEL_CONFIG_AFS_SEL_LEN));	
}

MPU6050_ErrorCode MPU6050::SetAccelerometerFullScaleRange(MPU6050_AccelerometerFullScaleRange fullScaleRange)
{
	uint8_t buffer;
	
	auto status = _transfer->ReadByte(_deviceAddress, MPU6050_ACCEL_CONFIG, &buffer);

	if (status != HAL_OK)	
	{
		_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;
		
		return _lastResult;
	}
	
	buffer = MPU6050_SET_VALUE(buffer, static_cast<uint8_t>(fullScaleRange), MPU6050_ACCEL_CONFIG_AFA_SEL_BIT, MPU6050_ACCEL_CONFIG_AFS_SEL_LEN);	


	auto halStatus = _transfer->WriteByte(_deviceAddress, MPU6050_ACCEL_CONFIG, buffer);

	this->_lastResult =  MPU6050_ErrorCode::MPU6050_Ok;

	if (halStatus != HAL_OK)		
		this->_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;			
	
	return this->_lastResult;
}

MPU6050_AccelerometerSeftTestAxis MPU6050::GetAccelerometerSeftTestAxis()
{
	uint8_t buffer = 0x00;
	
	auto status = _transfer->ReadByte(_deviceAddress, MPU6050_ACCEL_CONFIG, &buffer);

	if (status != HAL_OK)	
	{
		_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;
		
		return MPU6050_AccelerometerSeftTestAxis::MPU6050_AccelerometerSelfTestOff;
	}

	_lastResult =  MPU6050_ErrorCode::MPU6050_Ok;

	return static_cast<MPU6050_AccelerometerSeftTestAxis>(MPU6050_CONVERT(buffer, MPU6050_ACCEL_CONFIG_A_ST_BIT, MPU6050_ACCEL_CONFIG_A_ST_LEN));	
}

MPU6050_ErrorCode MPU6050::SetAccelerometerSeftTestAxis(MPU6050_AccelerometerSeftTestAxis seftTestAxis)
{
	auto code = this->SetAccelerometerFullScaleRange(MPU6050_AccelerometerScaleRanger8G);

	if (code != MPU6050_ErrorCode::MPU6050_Ok)
		return code;

	uint8_t buffer;
	
	auto status = _transfer->ReadByte(_deviceAddress, MPU6050_ACCEL_CONFIG, &buffer);

	if (status != HAL_OK)	
	{
		_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;
		
		return _lastResult;
	}
	
	buffer = MPU6050_SET_VALUE(buffer, static_cast<uint8_t>(seftTestAxis), MPU6050_ACCEL_CONFIG_A_ST_BIT, MPU6050_ACCEL_CONFIG_A_ST_LEN);	


	auto halStatus = _transfer->WriteByte(_deviceAddress, MPU6050_ACCEL_CONFIG, buffer);

	this->_lastResult =  MPU6050_ErrorCode::MPU6050_Ok;

	if (halStatus != HAL_OK)		
		this->_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;			
	
	return this->_lastResult;
}

MPU6050_ErrorCode MPU6050::ReadAccelerometer(MPU6050_AccelerometerData* data)
{
	uint8_t buffer[6];
	
	
	auto status = _transfer->ReadBytes(_deviceAddress, MPU6050_ACCEL_OUT, 6, buffer);
	
	if (status != HAL_OK)	
	{
		_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;
		
		return _lastResult;
	}
	
	data->X = (int16_t)(buffer[0] << 8 | buffer[1]);	
	data->Y = (int16_t)(buffer[2] << 8 | buffer[3]);
	data->Z = (int16_t)(buffer[4] << 8 | buffer[5]);
	
	_lastResult =  MPU6050_ErrorCode::MPU6050_Ok;
		
	return _lastResult;
}

MPU6050_ErrorCode MPU6050::ReadGyroscope(MPU6050_GyroscopeData* data)
{
	uint8_t buffer[6];			
	
	auto status = _transfer->ReadBytes(_deviceAddress, MPU6050_GYRO_OUT, 6, buffer);

	if (status != HAL_OK)	
	{
		_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;
		
		return _lastResult;
	}
		
	data->X = (int16_t)(buffer[0] << 8 | buffer[1]);
	data->Y = (int16_t)(buffer[2] << 8 | buffer[3]);
	data->Z = (int16_t)(buffer[4] << 8 | buffer[5]);

	_lastResult =  MPU6050_ErrorCode::MPU6050_Ok;
		
	return _lastResult;
}

float MPU6050::ReadTemperature()
{
	uint8_t buffer[2] = { 0xFF, 0xFF };
	
	auto status = _transfer->ReadBytes(_deviceAddress, MPU6050_TEMP_OUT, 2, buffer);	 

	float temp = 0;

	if (status != HAL_OK)		
		_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;				
	else
	{
		
		int16_t registerTemp = (buffer[0] << 8 | buffer[1]);		
	
		if (status == HAL_OK)	
			temp = MPU6050_CONVERT_TEMP(registerTemp);
	
		_lastResult =  MPU6050_ErrorCode::MPU6050_Ok;
	}

	return temp ;
}

MPU6050_FIFOConfig MPU6050::GetFIFOConfig()
{
	uint8_t buffer = 0x00;
	
	auto status = _transfer->ReadByte(_deviceAddress, MPU6050_FIFO_EN, &buffer);

	if (status != HAL_OK)	
	{
		_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;
		
		return MPU6050_FIFOConfig::MPU6050_FifoOff;
	}

	_lastResult =  MPU6050_ErrorCode::MPU6050_Ok;

	return static_cast<MPU6050_FIFOConfig>(MPU6050_CONVERT(buffer, MPU6050_FIFO_BIT, MPU6050_FIFO_LEN));	
}

MPU6050_ErrorCode MPU6050::SetFIFOConfig(MPU6050_FIFOConfig fifoConfig)
{
	uint8_t buffer = 0x00;
			
	buffer = MPU6050_SET_VALUE(buffer, static_cast<uint8_t>(fifoConfig), MPU6050_FIFO_BIT, MPU6050_FIFO_LEN);	

	auto halStatus = _transfer->WriteByte(_deviceAddress, MPU6050_FIFO_EN, buffer);

	this->_lastResult =  MPU6050_ErrorCode::MPU6050_Ok;

	if (halStatus != HAL_OK)		
		this->_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;			
	
	return this->_lastResult;
}

MPU6050_Interrupt MPU6050::GetInterruptEnabled()
{
	uint8_t buffer = 0x00;
	
	auto status = _transfer->ReadByte(_deviceAddress, MPU6050_INT_ENABLE, &buffer);

	if (status != HAL_OK)	
	{
		_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;
		
		return MPU6050_Interrupt::MPU6050_InterruptOff;
	}

	_lastResult =  MPU6050_ErrorCode::MPU6050_Ok;

	return static_cast<MPU6050_Interrupt>(MPU6050_CONVERT(buffer, MPU6050_INT_ENABLE_BIT, MPU6050_INT_ENABLE_LEN));	
}

MPU6050_ErrorCode MPU6050::SetInterruptEnabled(MPU6050_Interrupt interruptEnabled)
{
	uint8_t buffer = 0x00;
			
	buffer = MPU6050_SET_VALUE(buffer, static_cast<uint8_t>(interruptEnabled), MPU6050_INT_ENABLE_BIT, MPU6050_INT_ENABLE_LEN);	

	auto halStatus = _transfer->WriteByte(_deviceAddress, MPU6050_INT_ENABLE, buffer);

	this->_lastResult =  MPU6050_ErrorCode::MPU6050_Ok;

	if (halStatus != HAL_OK)		
		this->_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;			
	
	return this->_lastResult;
}

MPU6050_Interrupt MPU6050::GetInterruptStatus()
{
	uint8_t buffer = 0x00;
	
	auto status = _transfer->ReadByte(_deviceAddress, MPU6050_INT_STATUS, &buffer);

	if (status != HAL_OK)	
	{
		_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;
		
		return MPU6050_Interrupt::MPU6050_InterruptOff;
	}

	_lastResult =  MPU6050_ErrorCode::MPU6050_Ok;

	return static_cast<MPU6050_Interrupt>(MPU6050_CONVERT(buffer, MPU6050_INT_STATUS_BIT, MPU6050_INT_STATUS_LEN));	
}

MPU6050_ErrorCode MPU6050::GetSampleRate(MPU6050_SampleRate* sampleRate)
{
	MPU6050_Config config;

	auto status = this->GetConfig(&config);

	if (status != MPU6050_Ok)
	{
		this->_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;

		return _lastResult;
	}

	uint8_t buffer;
	
	auto halStatus = _transfer->ReadByte(_deviceAddress, MPU6050_SMPRT_DIV_A, &buffer);

	if (halStatus != HAL_OK)	
	{
		this->_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;
		
		return this->_lastResult;
	}

	sampleRate->SampleRateDivisor = MPU6050_CONVERT(buffer, MPU6050_SMPRT_DIV_A_BIT, MPU6050_SMPRT_DIV_A_LEN);
	sampleRate->SampleRateHz = config.DigitalLowPassFilter.GyroscopeOutputRateHz / (1 + sampleRate->SampleRateDivisor);

	this->_lastResult =  MPU6050_ErrorCode::MPU6050_Ok;
		
	return this->_lastResult;
}

MPU6050_ErrorCode MPU6050::ReadFIFOByte(uint8_t* pointer)
{		
	//uint8_t buffer;

	auto status = _transfer->ReadByte(_deviceAddress, MPU6050_FIFO_R_W, pointer);	

	if (status != HAL_OK)	
	{
		_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;
		
		return _lastResult;
	}

	_lastResult =  MPU6050_ErrorCode::MPU6050_Ok;
	
	return _lastResult;		
}


uint16_t MPU6050::GetFIFOCount()
{
	uint8_t buffer[2];
	
	auto status = _transfer->ReadBytes(_deviceAddress, MPU6050_FIFO_COUNT_H, 2, buffer);

	if (status != HAL_OK)	
	{
		_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;
		
		return 0xFFFF;
	}

	_lastResult =  MPU6050_ErrorCode::MPU6050_Ok;

	uint16_t value = (int16_t)(buffer[0] << 8 | buffer[1]);

	return value;		
}

bool MPU6050::GetFIFOEnabled()
{
	uint8_t buffer = 0x00;
	
	auto status = _transfer->ReadByte(_deviceAddress, MPU6050_USER_CTRL, &buffer);

	if (status != HAL_OK)	
	{
		_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;
		
		return false;
	}

	_lastResult =  MPU6050_ErrorCode::MPU6050_Ok;

	return MPU6050_CONVERT(buffer, MPU6050_USER_CTRL_FIFO_EN_BIT, MPU6050_USER_CTRL_FIFO_EN_LEN) == MPU6050_BIT_SET;
}

MPU6050_ErrorCode MPU6050::SetFIFOEnabled(bool status)
{
	uint8_t buffer = 0x00;
			
	auto halStatus = _transfer->ReadByte(_deviceAddress, MPU6050_USER_CTRL, &buffer);

	if (halStatus != HAL_OK)	
	{
		_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;
		
		return _lastResult;
	}

	uint8_t value = status ? MPU6050_BIT_SET : MPU6050_BIT_RESET;


	buffer = MPU6050_SET_VALUE(buffer, value, MPU6050_USER_CTRL_FIFO_EN_BIT, MPU6050_USER_CTRL_FIFO_EN_LEN);

	halStatus = _transfer->WriteByte(_deviceAddress, MPU6050_USER_CTRL, buffer);

	this->_lastResult =  MPU6050_ErrorCode::MPU6050_Ok;

	if (halStatus != HAL_OK)		
		this->_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;			
	
	return this->_lastResult;
}

MPU6050_ErrorCode MPU6050::FIFOReset()
{
	uint8_t buffer = 0x00;
			
	auto halStatus = _transfer->ReadByte(_deviceAddress, MPU6050_USER_CTRL, &buffer);

	if (halStatus != HAL_OK)	
	{
		_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;
		
		return _lastResult;
	}

	buffer = MPU6050_SET_VALUE(buffer, MPU6050_BIT_SET, MPU6050_USER_CTRL_FIFO_RESET_BIT, MPU6050_USER_CTRL_FIFO_RESET_LEN);	

	halStatus = _transfer->WriteByte(_deviceAddress, MPU6050_USER_CTRL, buffer);

	this->_lastResult =  MPU6050_ErrorCode::MPU6050_Ok;

	if (halStatus != HAL_OK)		
		this->_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;			
	
	return this->_lastResult;
}

MPU6050_ErrorCode MPU6050::ResetsSignal()
{
	uint8_t buffer = 0x00;
			
	auto halStatus = _transfer->ReadByte(_deviceAddress, MPU6050_USER_CTRL, &buffer);

	if (halStatus != HAL_OK)	
	{
		_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;
		
		return _lastResult;
	}

	buffer = MPU6050_SET_VALUE(buffer, MPU6050_BIT_SET, MPU6050_USER_CTRL_SIG_COND_RESET_BIT, MPU6050_USER_CTRL_SIG_COND_RESET_LEN);	

	halStatus = _transfer->WriteByte(_deviceAddress, MPU6050_USER_CTRL, buffer);

	this->_lastResult =  MPU6050_ErrorCode::MPU6050_Ok;

	if (halStatus != HAL_OK)		
		this->_lastResult =  MPU6050_ErrorCode::MPU6050_ErrorFailure;			
	
	return this->_lastResult;
}
