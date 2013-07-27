#include "stm32f10x.h"
#include "stm32f10x_conf.h"

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
//#include "stm32f10x_tim.h"
//#include "misc.h"

#include "xhw_types.h"
#include "xcore.h"
#include "xsysctl.h"
#include "xgpio.h"
#include "hw_hd44780.h"
#include "hd44780.h"

#include "include/delay.h"
#include "include/OneWire.h"

float ConvertTemperature(uint8_t buf[9]);
void RCC_Configuration(void);
void InitTempSensor(void);
float GetTemp(void);
void InitHeater(void);
void SetHeaterPower(float power);

void HD44780Test(void);

uint8_t buf[9];
owPort port;
ErrorStatus HSEStartUpStatus;

int dt = 100;

int main(void)
{
//	RCC_Configuration();

	HD44780Test();
	InitTempSensor();
	InitHeater();

	float target=27.0f;

	while(1)
	{
		float temp=GetTemp();
		if(temp<=target)
		{
			GPIO_WriteBit(GPIOC,GPIO_Pin_8,Bit_SET);
			SetHeaterPower((target-temp)/2.0f);
		}
		else
		{
			SetHeaterPower(0.0f);
			GPIO_WriteBit(GPIOC,GPIO_Pin_8,Bit_RESET);
		}
	}

	return 0;
}

void InitTempSensor(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7|GPIO_Pin_8; //7 - vcc; 6 - data 8 - LED
	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_WriteBit(GPIOC,GPIO_Pin_7,Bit_SET);

	port.pin=GPIO_Pin_6;//6
	port.port=GPIOC;
	port.debpin=GPIO_Pin_9;
	port.debport=GPIOC;

	owInitPort(&port);
	owDebugInit(&port);
}

float GetTemp(void)
{
	owReset(&port);
	owSetWrite(&port);
	owWriteByte(&port,OW_SKIP_ROM);
	owWriteByte(&port,0x44); //CONVERT_T
	Delay_ms(900);

	owReset(&port);
	owSetWrite(&port);
	owWriteByte(&port,OW_SKIP_ROM);
	owWriteByte(&port,OW_DS18B20_READ_SCRATCHPAD);
	owReadData(&port,buf,9);

	float temp=ConvertTemperature(buf);
	owDebugMsg(&port,1);
	return temp;
}

float ConvertTemperature(uint8_t buf[9])
{
	int result=((buf[1]&0x07)<<8)|buf[0];
	if(buf[1]&0x80) result=-((~result)&0x7FF);
	return result/16.0f;
}

void InitHeater(void)
{
	GPIO_InitTypeDef port;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

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

void SetHeaterPower(float power)
{
	int val=power*5;
	TIM2->CCR1=dt*val/5;
}
//------------
void RCC_Configuration(void)
{
    /*RCC system reset(for debug purpose) */
    RCC_DeInit();

    /* Enable HSE */
    RCC_HSEConfig(RCC_HSE_ON);

    /* Wait till HSE is ready */
    HSEStartUpStatus = RCC_WaitForHSEStartUp();

    if (HSEStartUpStatus == SUCCESS)
    {
        /* HCLK = SYSCLK */
        RCC_HCLKConfig(RCC_SYSCLK_Div1);

        /* PCLK2 = HCLK*/
        RCC_PCLK2Config(RCC_HCLK_Div1);

        /* PCLK1 = HCLK*/
        RCC_PCLK1Config(RCC_HCLK_Div1);

        //ADC CLK
        RCC_ADCCLKConfig(RCC_PCLK2_Div2);

        /* PLLCLK = 8MHz * 3 = 24 MHz */
        //RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_3);



        /* Enable PLL */
        RCC_PLLCmd(ENABLE);

        /* Wait till PLL is ready */
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {}

        /* Select PLL as system clock source */
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

        /* Wait till PLL is used as system clock source */
        while (RCC_GetSYSCLKSource() != 0x08) {}
    }

    /*Then need to enable peripheral clocks ----------------------------------------------*/
}

void HD44780Test(void)
{
	int i , j;

	HD44780Init();

	//
	// Location Set & Display string, number Test
	//
	HD44780LocationSet(0, 0);
	HD44780DisplayString("Hello HD44780!");
	HD44780LocationSet(0, 1);
	HD44780DisplayString("Number:");
	HD44780DisplayN(10086);
	xSysCtlDelay(xSysCtlClockGet() / 4);
	HD44780LocationSet(0,1);
	HD44780DisplayString("Display off/on");
	xSysCtlDelay(xSysCtlClockGet() / 4);
	HD44780DisplayOff();
	xSysCtlDelay(xSysCtlClockGet() / 4 );
	HD44780DisplayOn();
	xSysCtlDelay(xSysCtlClockGet() / 4);
	//
	// Cursor style
	//
	HD44780DisplayClear();
	HD44780DisplayString("Cursor Test");
	HD44780CursorOn(xfalse);
	xSysCtlDelay(xSysCtlClockGet() / 4);
	HD44780CursorOff();
	xSysCtlDelay(xSysCtlClockGet() / 4);

	//
	// Cursor will blink at the end of line
	//
	HD44780CursorOn(xtrue);
	HD44780CursorRightMove(1);
	xSysCtlDelay(xSysCtlClockGet() / 4);
	HD44780CursorRightMove(5);
	xSysCtlDelay(xSysCtlClockGet() / 4);
	HD44780CursorLeftMove(4);
	xSysCtlDelay(xSysCtlClockGet() / 4);

	for(i = 0; i < 10; i++)
	{
		HD44780CursorRightMove(1);
		xSysCtlDelay(xSysCtlClockGet() / 10);
	}

	for(i = 0; i < 10; i++)
	{
		HD44780CursorLeftMove(1);
		xSysCtlDelay(xSysCtlClockGet() / 10);
	}

	HD44780LocationSet(0, 1);
	HD44780DisplayString("Cursor Home");
	xSysCtlDelay(xSysCtlClockGet() / 4);

	HD44780Home();

	//
	// Display Right shift
	//
	HD44780DisplayClear();
	HD44780DisplayString("Right scrolling string");
	HD44780LocationSet(0, 1);
	HD44780DisplayString("        Line 2         ");
	for(i = 0; i < 20; i++)
	{
		HD44780DisplayRightShift(1);
		xSysCtlDelay(xSysCtlClockGet() / 10);
	}

	//
	// Display Left shift
	//
	HD44780DisplayClear();
	HD44780DisplayString("Left scrolling string");
	HD44780LocationSet(0, 1);
	HD44780DisplayString("        Line 2         ");
	for(i = 0; i < 20; i++)
	{
		HD44780DisplayLeftShift(1);
		xSysCtlDelay(xSysCtlClockGet() / 10);
	}

	//
	// Display Right Left shift
	//
	HD44780DisplayClear();
	HD44780DisplayString("Left Right scrolling string");
	HD44780LocationSet(0, 1);
	HD44780DisplayString("        Line 2         ");
	for(j = 0; j < 2; j++)
	{
		for(i = 0; i < 10; i++)
		{
			HD44780DisplayRightShift(1);
			xSysCtlDelay(xSysCtlClockGet() / 10);
		}
		for(i = 0; i < 10; i++)
		{
			HD44780DisplayLeftShift(1);
			xSysCtlDelay(xSysCtlClockGet() / 10);
		}
	}
	HD44780Home();
	xSysCtlDelay(xSysCtlClockGet() / 4);

	//
	// Cursor Move Direction
	//
	HD44780DisplayClear();
	HD44780LocationSet(10, 1);
	xSysCtlDelay(xSysCtlClockGet() / 4);
	HD44780EntryModeSet(HD44780_MOVE_DIRECTION_DEC, xfalse);
	HD44780DisplayString("DEC");

	HD44780LocationSet(10, 2);
	xSysCtlDelay(xSysCtlClockGet() / 4);
	HD44780EntryModeSet(HD44780_MOVE_DIRECTION_DEC, xtrue);
	HD44780DisplayString("DEC shift");

	HD44780DisplayClear();
	HD44780LocationSet(10, 1);
	xSysCtlDelay(xSysCtlClockGet() / 4);
	HD44780EntryModeSet(HD44780_MOVE_DIRECTION_INC, xfalse);
	HD44780DisplayString("INC");

	HD44780LocationSet(10, 2);
	xSysCtlDelay(xSysCtlClockGet() / 4);
	HD44780EntryModeSet(HD44780_MOVE_DIRECTION_INC, xtrue);
	HD44780DisplayString("INC shift");
	xSysCtlDelay(xSysCtlClockGet() / 4);

	HD44780DisplayClear();
	HD44780EntryModeSet(HD44780_MOVE_DIRECTION_INC, xfalse);
	HD44780DisplayString("Complete");
}
