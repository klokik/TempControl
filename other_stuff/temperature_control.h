
#ifndef TEMPERATURE_CONTROL_H_
#define TEMPERATURE_CONTROL_H_

#include "data_types.h"
#include "delay.h"

#include "stm32f10x.h"
#include "stm32f10x_conf.h"

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"

#define KEY_UP		= 10;
#define KEY_DOWN	= 11;
#define KEY_LEFT	= 12;
#define KEY_RIGHT	= 13;
#define KEY_SET		= 14;


void LoadSettings();
void InitializePeriphery();						//DONE
void InitializeGPIO();
void InitializeSensors();						//DONE
void InitializeDisplay();						//DONE
void InitializeTimers();
void InitializeHeater();
void InitializePWM();
void InitializeInterrupts();
void StartActionTimer();
void StartPWM();
SensorData ReadSensorsData();					//DONE
void AdjustHeaterPower(SensorData sensorData);
void DisplayData(SensorData sensorData);		//-
void ShowMessage(char *text);					//DONE

void ActionTimerHandler(size_t interval);		//DONE
void ButtonPressHandler(int keyCode);			//DONE

#endif