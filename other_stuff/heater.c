#include "temperature_control.h"

void InitializeHeater()
{
	GPIO_InitTypeDef port;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	//Init GPIO
	port.GPIO_Pin = (GPIO_Pin_0);
	port.GPIO_Mode = GPIO_Mode_AF_PP;
	port.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&port);

	//Init timer
	TIM2->CCER |= TIM_CCER_CC1E;
	TIM2->CCMR1 |= (TIM_CCMR1_OC1M_1|TIM_CCMR1_OC1M_2);
	TIM2->PSC = 24000-1;
	TIM2->ARR = dt;
	TIM2->DIER = TIM_DIER_UIE;
	TIM2->CR1 |= TIM_CR1_CEN;
}

void AdjustHeaterPower(SensorData sensorData)
{
	//PErform calculations to choose heater power
	float delta = targetTemperature - sensorData.temperature01;
	if (delta > 0)
		SetHeaterPower(delta);
	else
		SetHeaterPower(0.0f);
}

void SetHeaterPower(float value)
{
	int val=power*5;
	TIM2->CCR1=dt*val/5;
}