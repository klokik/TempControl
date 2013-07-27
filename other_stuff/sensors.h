
#include "temperature_control.h"
#include "OneWire.h"

owPort port;

void InitializeSensors()
{
	InitTempSensor();
	InitHumiditySensor();
}

SensorsData ReadSensorsData()
{
	RequestTemp();

	SensorsData value;
	value.temperature01 = PullTemp();
	value.temperature02 = 0.0f;

	value.humidity = GetHumidity();

	return value;
}

void InitHumiditySensor(void)
{
	//TODO: init some gpio ports
}

float GetHumidity(void)
{
	//TODO: get data from DHT11 Sensor
	return 0;
}

void InitTempSensor(void)
{
	port.pin=GPIO_Pin_0;
	port.port=GPIOC;
	port.debpin=GPIO_Pin_8;
	port.debport=GPIOB;

	owInitPort(&port);
	owDebugInit(&port);
}

float ConvertTemperature(uint8_t buf[9])
{
	int result=((buf[1]&0x07)<<8)|buf[0];
	if(buf[1]&0x80) result=-((~result)&0x7FF);
	return result/16.0f;
}

float PullTemp(void)
{
	uint8_t buf[9]={0};

	owReset(&port);
	owSetWrite(&port);
	owWriteByte(&port,OW_SKIP_ROM);
	owWriteByte(&port,OW_DS18B20_READ_SCRATCHPAD);
	owReadData(&port,buf,9);

	float temp=ConvertTemperature(buf);
	owDebugMsg(&port,1);
	return temp;
}

void RequestTemp(void)
{
	owReset(&port);
	owSetWrite(&port);
	owWriteByte(&port,OW_SKIP_ROM);
	owWriteByte(&port,0x44); //CONVERT_T
}