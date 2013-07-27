#ifndef DATA_TYPES_H_
#define DATA_TYPES_H_

typedef struct
{
	size_t pwmInterval;
	size_t actionInterval;

	struct
	{
		float maxTemperatureLimit;
		float minTemperatureLimit;

		float proportionalGain;
		float integralGain;
		float derivativeGain;
	} heater;
} DeviceSettings;

typedef struct
{
	float temperature01;
	float temperature02;
	
	float humidity;
} SensorData;

#endif