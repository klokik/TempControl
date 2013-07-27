#include "include/delay.h"

#include "stm32f10x_rcc.h"

volatile uint32_t nCount;
RCC_ClocksTypeDef RCC_Clocks;
uint32_t thms;
uint32_t thus;

//-------
void Delay_Init(void)
{
	RCC_GetClocksFreq (&RCC_Clocks);
	thms=RCC_Clocks.HCLK_Frequency/10000;
}

//-------
void Delay_ms(uint32_t ms)
{
	nCount=thms*ms;
	for (; nCount!=0; nCount--);
}

//-------
void Delay_us(uint32_t us)
{
	nCount=thms*us/1500;
	for (; nCount!=0; nCount--);
}

//-------
void Delay_loop(uint32_t count)
{
	uint32_t q;
	for(q=0;q<count;q++);
}
