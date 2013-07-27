#include "temperature_control.h"

int main(void)
{
	LoadSettings();
	InitializePeriphery();

	Run();
}

int InitializePeriphery(void)
{
	InitializeGPIO();
	InitializePorts();
	InitializeSensors();
	InitializeDisplay();
	InitializeTimers();
	InitializePWM();
	InitializeInterrupts();
}

void Run(void)
{
	StartActionTimer();
	StartPWM();

	bool done = false;
	while(!done)
	{
		//	Nothing	
	}
}

void ActionTimerHandler(size_t interval)
{
	var sensorData = ReadSensorsData();

	AdjustHeaterPower(sensorData);

	DisplayData(sensorData);
}

void ButtonPressHandler(int keyCode)
{
	switch(keyCode)
	{
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	default:
		ShowMessage("Unknown key");
		break;
	}
}