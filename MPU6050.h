#pragma once

#include "stm32f4xx_hal.h"
#include "DataI2CTransfer.h"

/* Колличество попыток */
#define MPU6050_I2C_TRAILS							3
/* TimeOut на выполнение операции*/
#define MPU6050_I2C_TIMEOUT							10

/* Default I2C address */
#define MPU6050_DEFAULT_ADDRESS						0xD0

/* Who I am значение регистра */
#define MPU6050_I_AM_VALUE							0x68


/*Регистры */

/*	Self Test	*/
#define MPU6050_SELF_TEST_X							0x0D
#define MPU6050_SELF_TEST_XA_BIT					7
#define MPU6050_SELF_TEST_XA_LEN					3
#define MPU6050_SELF_TEST_XG_BIT					4
#define MPU6050_SELF_TEST_XG_LEN					5

#define MPU6050_SELF_TEST_Y							0x0E
#define MPU6050_SELF_TEST_YA_BIT					7
#define MPU6050_SELF_TEST_YA_LEN					3
#define MPU6050_SELF_TEST_YG_BIT					4
#define MPU6050_SELF_TEST_YG_LEN					5

#define MPU6050_SELF_TEST_Z							0x0F
#define MPU6050_SELF_TEST_ZA_BIT					7
#define MPU6050_SELF_TEST_ZA_LEN					3
#define MPU6050_SELF_TEST_ZG_BIT					4
#define MPU6050_SELF_TEST_ZG_LEN					5

#define MPU6050_SELF_TEST_A							0x10
#define MPU6050_SELF_TEST_AX_BIT					5
#define MPU6050_SELF_TEST_AX_LEN					2
#define MPU6050_SELF_TEST_AY_BIT					3
#define MPU6050_SELF_TEST_AY_LEN					2
#define MPU6050_SELF_TEST_AZ_BIT					1
#define MPU6050_SELF_TEST_AZ_LEN					2


#define MPU6050_SMPRT_DIV_A							0x19
#define MPU6050_SMPRT_DIV_A_BIT						7
#define MPU6050_SMPRT_DIV_A_LEN						8

#define MPU6050_CONFIG								0x1A
#define MPU6050_CONFIG_EXT_SYNC_SET_BIT				5
#define MPU6050_CONFIG_EXT_SYNC_SET_LEN				3
#define MPU6050_CONFIG_EXT_DLPF_CFG_BIT				2
#define MPU6050_CONFIG_EXT_DLPF_CFG_LEN				3


#define MPU6050_GYRO_CONFIG							0x1B
#define MPU6050_GYRO_CONFIG_G_ST_BIT				7
#define MPU6050_GYRO_CONFIG_G_ST_LEN				3
#define MPU6050_GYRO_CONFIG_FS_SEL_BIT				4
#define MPU6050_GYRO_CONFIG_FS_SEL_LEN				2

#define MPU6050_ACCEL_CONFIG						0x1C
#define MPU6050_ACCEL_CONFIG_A_ST_BIT				7
#define MPU6050_ACCEL_CONFIG_A_ST_LEN				3
#define MPU6050_ACCEL_CONFIG_AFA_SEL_BIT			4
#define MPU6050_ACCEL_CONFIG_AFS_SEL_LEN			2

#define MPU6050_SIGNAL_PATH_RESET					0x68
#define MPU6050_SIGNAL_PATH_RESET_BIT				2
#define MPU6050_SIGNAL_PATH_RESET_LEN				3


#define MPU6050_FIFO_EN								0x23
#define MPU6050_FIFO_BIT							7
#define MPU6050_FIFO_LEN							8


#define MPU6050_INT_ENABLE							0x38
#define MPU6050_INT_ENABLE_BIT						7
#define MPU6050_INT_ENABLE_LEN						8

#define MPU6050_INT_STATUS							0x3A
#define MPU6050_INT_STATUS_BIT						7
#define MPU6050_INT_STATUS_LEN						8


#define MPU6050_FIFO_COUNT_H						0x72
#define MPU6050_FIFO_R_W							0x74

#define MPU6050_PWR_MGMT_1							0x6B
#define MPU6050_PWR_MGMT_1_DEVICE_RESET_BIT			7
#define MPU6050_PWR_MGMT_1_DEVICE_RESET_LEN			1
#define MPU6050_PWR_MGMT_1_SLEEP_BIT				6
#define MPU6050_PWR_MGMT_1_SLEEP_LEN				1
#define MPU6050_PWR_MGMT_1_CYCLE_BIT				5
#define MPU6050_PWR_MGMT_1_CYCLE_LEN				1
#define MPU6050_PWR_MGMT_1_TEMP_DIS_BIT				3
#define MPU6050_PWR_MGMT_1_TEMP_DIS_LEN				1
#define MPU6050_PWR_MGMT_1_CLKSEL_BIT				2
#define MPU6050_PWR_MGMT_1_CLKSEL_LEN				3

#define MPU6050_USER_CTRL							0x6A
#define MPU6050_USER_CTRL_FIFO_EN_BIT				6
#define MPU6050_USER_CTRL_FIFO_EN_LEN				1
#define MPU6050_USER_CTRL_I2C_MST_EN_BIT			5
#define MPU6050_USER_CTRL_I2C_MST_EN_LEN			1
#define MPU6050_USER_CTRL_FIFO_RESET_BIT			2
#define MPU6050_USER_CTRL_FIFO_RESET_LEN			1
#define MPU6050_USER_CTRL_I2C_MST_RESET_BIT			1
#define MPU6050_USER_CTRL_I2C_MST_RESET_LEN			1
#define MPU6050_USER_CTRL_SIG_COND_RESET_BIT		0
#define MPU6050_USER_CTRL_SIG_COND_RESET_LEN		1



#define MPU6050_PWR_MGMT_2							0x6C
#define MPU6050_PWR_MGMT_2_LP_WAKE_CTRL_BIT			7
#define MPU6050_PWR_MGMT_2_LP_WAKE_CTRL_LEN			2
#define MPU6050_PWR_MGMT_2_STBY_BIT					5
#define MPU6050_PWR_MGMT_2_STBY_LEN					6

#define MPU6050_ACCEL_OUT							0x3B
#define MPU6050_TEMP_OUT							0x41
#define MPU6050_GYRO_OUT							0x43

#define MPU6050_WHO_AM_I							0x75

#define MPU6050_BIT_SET								0x01
#define MPU6050_BIT_RESET							0x00
#define MPU6050_CONVERT(value, bitStart, length) ((value & (((1 << length) - 1) << (bitStart - length + 1))) >> (bitStart - length + 1))

#define MPU6050_SET_VALUE(original, value, bitStart, length) (value != 0 ? ((original & ((0xFF << (bitStart + 1)) | (0xFF >> (7 - bitStart + length)))) | (value << (bitStart - length + 1))) : (original & ((0xFF << (bitStart + 1)) | (0xFF >> (7 - bitStart + length)))))

#define MPU6050_CONVERT_TEMP(temp) (temp/340 + 36.53)
#define MPU6050_GYRO_FT(value) (value != 0 ? (3275* pow(1.046,(value-1))) : 0)
#define MPU6050_ACCSEL_FT(value) (value !=0 ? (1392.64*pow(0.92/0.34,((value-1)/30))) : 0)

typedef enum {
	MPU6050_Device_0 = 0,   /*!< AD0 pin is set to low */
	MPU6050_Device_1 = 0x02 /*!< AD0 pin is set to high */
} MPU6050_DeviceNumber;

typedef enum {
	MPU6050_Ok                      = 0,        
	MPU6050_ErrorDeviceNotConnected, 
	MPU6050_ErrorDeviceInvalid,      
	MPU6050_ErrorFailure
} MPU6050_ErrorCode;


	/**
	 * @brief  Parameters for accelerometer range
	 */
typedef enum {
	MPU6050_AccelerometerScaleRange2G  = 0x00, /*!< Range is +- 2G */
	MPU6050_AccelerometerScaleRange4G  = 0x01, /*!< Range is +- 4G */
	MPU6050_AccelerometerScaleRanger8G = 0x02, /*!< Range is +- 8G */
	MPU6050_AccelerometerScaleRange16G = 0x03 /*!< Range is +- 16G */
} MPU6050_AccelerometerFullScaleRange;

	/**
	 * @brief  Parameters for gyroscope range
	 */
typedef enum {
	MPU6050_GyroscopeScaleRange250s  = 0x00,  /*!< Range is +- 250 degrees/s */
	MPU6050_GyroscopeScaleRange500s  = 0x01,  /*!< Range is +- 500 degrees/s */
	MPU6050_GyroscopeScaleRange1000s = 0x02, /*!< Range is +- 1000 degrees/s */
	MPU6050_GyroscopeScaleRange2000s = 0x03  /*!< Range is +- 2000 degrees/s */
} MPU6050_GyroscopeFullScaleRange;

typedef enum {
	MPU6050_WakeUp = 0, // Пробуждает
	MPU6050_Sleep  = 1, // Засыпает
	MPU6050_Cycle  = 2 // Выполняет циклическое пробуждение, равное частоте MPU6050_WakeUpFrequency.
} MPU6050_SleepMode;

/**
 * \brief Режим сохращения потребления питания, отключая ось измерения.
 */
typedef enum
{
	MPU6050_AccelerometerXa = 0x20,
	MPU6050_AccelerometerYa = 0x10,
	MPU6050_AccelerometerZa = 0x08,
	MPU6050_GyroscopeXa     = 0x04,
	MPU6050_GyroscopeYa     = 0x02,
	MPU6050_GyroscopeZa     = 0x01,
	MPU6050_StandbyOff      = 0x00
} MPU6050_StandbyMode;

typedef enum 
{
	MPU6050_Internal8MHz             = 0x00, //Internal 8MHz oscillator
	MPU6050_PLLWithXAxisGyroscope    = 0x01, //PLL with X axis gyroscope reference
	MPU6050_PLLWithYAxisGyroscope    = 0x02, //PLL with Y axis gyroscope reference
	MPU6050_PLLWithZAxisGyroscope    = 0x03, //PLL with Z axis gyroscope reference
	MPU6050_PLLWithExternal32p768kHz = 0x04, //PLL with external 32.768kHz reference
	MPU6050_PLLWithExternal19p2MHz   = 0x05, //PLL with external 19.2MHz reference
	MPU6050_TimingGeneratorReset     = 0x07, //Stops the clock and keeps the timing generator in reset
} MPU6050_ClockSourceDevice;

/**
 * \brief Частота циклического пробуждения.
 */
typedef enum
{
	MPU6050_1p25Hz = 0x00,
	MPU6050_5Hz    = 0x01,
	MPU6050_20Hz   = 0x02,
	MPU6050_40Hz   = 0x03
} MPU6050_WakeUpFrequency;

typedef struct
{
	MPU6050_WakeUpFrequency WakeUpFrequency;
	MPU6050_StandbyMode StandbyMode;
} MPU6050_PowerMode;

typedef struct
{
	uint8_t A;
	uint8_t G;	
} MPU6050_SelfTestXYZDef;

typedef struct
{
	uint8_t Xa;
	uint8_t Ya;	
	uint8_t Za;	
} MPU6050_SelfTestADef;

typedef struct
{
	MPU6050_SelfTestXYZDef X;	

	MPU6050_SelfTestXYZDef Y;	

	MPU6050_SelfTestXYZDef Z;	

	MPU6050_SelfTestADef A;	
	
} MPU6050_SelfTestTypeDef;

typedef enum
{
	MPU6050_GyroscopeSelfTestXa  = 0x04,
	MPU6050_GyroscopeSelfTestYa  = 0x02,
	MPU6050_GyroscopeSelfTestZa  = 0x01,
	MPU6050_GyroscopeSelfTestOff = 0x00
} MPU6050_GyroscopeSeftTestAxis;


typedef enum 
{
	MPU6050_AccelerometerSelfTestXa  = 0x04,
	MPU6050_AccelerometerSelfTestYa  = 0x02,
	MPU6050_AccelerometerSelfTestZa  = 0x01,
	MPU6050_AccelerometerSelfTestOff = 0x00
} MPU6050_AccelerometerSeftTestAxis;

typedef enum
{
	MPU6050_InputDisabled = 0,
	MPU6050_TempOutL      = 1,
	MPU6050_GyroXOutL     = 2,
	MPU6050_GyroYOutL     = 3,
	MPU6050_GyroZOutL     = 4,
	MPU6050_AccelXOutL    = 5,
	MPU6050_AccelYOutL    = 6,
	MPU6050_AccelZOutL    = 7,
} MPU6050_ExternalFrameSynchronization;


typedef enum
{
	MPU6050_DLPFConfig0 = 0,	
	MPU6050_DLPFConfig1 = 1,
	MPU6050_DLPFConfig2 = 2,
	MPU6050_DLPFConfig3 = 3,
	MPU6050_DLPFConfig4 = 4,
	MPU6050_DLPFConfig5 = 5,
	MPU6050_DLPFConfig6 = 6,
	MPU6050_DLPFConfig7 = 7,
} MPU6050_DigitalLowPassFilterConfig;

typedef struct
{
	MPU6050_DigitalLowPassFilterConfig Config;
	uint16_t GyroscopeBandwidthHz;
	uint16_t GyroscopeDelay; // Значене в микросекундах.
	uint16_t GyroscopeOutputRateHz;
	uint16_t AccelerometerBandwidth;
	uint16_t AccelerometerDelay; // Значене в микросекундах.

} MPU6050_DigitalLowPassFilter;

typedef struct
{
	MPU6050_ExternalFrameSynchronization ExternalFrameSynchronization;
	MPU6050_DigitalLowPassFilter DigitalLowPassFilter;	
} MPU6050_Config;


typedef struct 
{
	float SampleRateHz;
	uint8_t SampleRateDivisor;
} MPU6050_SampleRate;

typedef struct
{
	int16_t X = 0;
	int16_t Y = 0;
	int16_t Z = 0;
} MPU6050_AccelerometerData;

typedef struct
{
	int16_t X = 0;
	int16_t Y = 0;
	int16_t Z = 0;
} MPU6050_GyroscopeData;


typedef enum
{
	MPU6050_FifoTemp          = 0x80,
	MPU6050_FifoXg            = 0x40,
	MPU6050_FifoYg            = 0x20,
	MPU6050_FifoZg            = 0x10,
	MPU6050_FifoAccelerometer = 0x08,
	MPU6050_FifoSLV2          = 0x04,
	MPU6050_FifoSLV1          = 0x02,
	MPU6050_FifoLSV0          = 0x01,
	MPU6050_FifoOff           = 0x00
} MPU6050_FIFOConfig;

typedef enum {
	MPU6050_FifoOverflow            = 0x10,
	MPU6050_MasterIinterruptSources = 0x08,
	MPU6050_DataReady               = 0x01,
	MPU6050_InterruptOff            = 0x00
}MPU6050_Interrupt;

class MPU6050
{
	DataI2CTransfer* _transfer;	
	MPU6050_DeviceNumber _deviceNumber;
	uint8_t _deviceAddress;
	MPU6050_ErrorCode _lastResult = MPU6050_Ok;
public:
	MPU6050(DataI2CTransfer* transfer, MPU6050_DeviceNumber deviceNumber);
	~MPU6050();

	MPU6050_ErrorCode GetLastError() const;

	bool IsDeviceMPU6050();

	MPU6050_ErrorCode DeviceReset();

	MPU6050_SleepMode GetSleepMode();

	MPU6050_ErrorCode GetPowerMode(MPU6050_PowerMode* powerMode);
	
	MPU6050_ErrorCode SetPowerMode(const MPU6050_PowerMode* powerMode);

	MPU6050_ErrorCode SetPowerEnabledTemperatureSensor(bool value);

	bool GetPowerEnabledTemperatureSensor();

	MPU6050_ClockSourceDevice GetClockSourceDevice();

	MPU6050_ErrorCode SetClockSourceDevice(MPU6050_ClockSourceDevice value);

	/**
	 * \brief 
	 * \param mode Установить режим ожидания. Если выбран MPU6050_Cycle, то частота равна MPU6050_WakeUpFrequency.	  
	 */
	MPU6050_ErrorCode SetSleepMode(MPU6050_SleepMode mode); 

	MPU6050_ErrorCode SelfTest(MPU6050_SelfTestTypeDef* data);

	MPU6050_ErrorCode GetConfig(MPU6050_Config* config);

	MPU6050_ErrorCode SetConfig(MPU6050_ExternalFrameSynchronization externalFrameSynchronization, MPU6050_DigitalLowPassFilterConfig digitalLowPassFilterConfig);

	MPU6050_ErrorCode GetSampleRate(MPU6050_SampleRate* sampleRate);

	MPU6050_ErrorCode SetSampleRate(uint8_t sampleRateDivisor);

	MPU6050_GyroscopeFullScaleRange GetGyroscopeFullScaleRange();

	MPU6050_ErrorCode SetGyroscopeFullScaleRange(MPU6050_GyroscopeFullScaleRange fullScaleRange);

	MPU6050_GyroscopeSeftTestAxis GetGyroscopeSeftTestAxis();

	MPU6050_ErrorCode SetGyroscopeSeftTestAxis(MPU6050_GyroscopeSeftTestAxis seftTestAxis);

	MPU6050_AccelerometerFullScaleRange GetAccelerometerFullScaleRange();

	MPU6050_ErrorCode SetAccelerometerFullScaleRange(MPU6050_AccelerometerFullScaleRange fullScaleRange);

	MPU6050_AccelerometerSeftTestAxis GetAccelerometerSeftTestAxis();

	MPU6050_ErrorCode SetAccelerometerSeftTestAxis(MPU6050_AccelerometerSeftTestAxis seftTestAxis);
	
	MPU6050_ErrorCode ReadAccelerometer(MPU6050_AccelerometerData* data);
	
	MPU6050_ErrorCode ReadGyroscope(MPU6050_GyroscopeData* data);
	
	float ReadTemperature();

	MPU6050_FIFOConfig GetFIFOConfig();

	MPU6050_ErrorCode SetFIFOConfig(MPU6050_FIFOConfig fifoEnabled);

	MPU6050_Interrupt GetInterruptEnabled();

	MPU6050_ErrorCode SetInterruptEnabled(MPU6050_Interrupt interruptEnabled);

	MPU6050_Interrupt GetInterruptStatus();

	uint16_t GetFIFOCount();

	MPU6050_ErrorCode ReadFIFOByte(uint8_t* pointer);

	MPU6050_ErrorCode SetFIFOEnabled(bool status);

	bool GetFIFOEnabled();

	MPU6050_ErrorCode FIFOReset();

	MPU6050_ErrorCode ResetsSignal();
};