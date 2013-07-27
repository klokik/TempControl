#include "temperature_control.h"

const uint8_t HD_CLEAR = 0x01;
const uint8_t HD_DDRAM = 0x02;
const uint8_t HD_INCCR = 0x06;	// increment cursor, not frame
const uint8_t HD_ONCOF = 0x12;	// display on, cursor off
const uint8_t HD_DMODE = 0x28;	// 4 - lane, 2 - rows, 5x8

const unsigned PIN_RS = GPIO_ODR_ODR1;
const unsigned PIN_RW = GPIO_ODR_ODR1;
const unsigned PIN_E  = GPIO_ODR_ODR1;

const unsigned PIN_DB4 = GPIO_ODR_ODR1;
const unsigned PIN_DB5 = GPIO_ODR_ODR1;
const unsigned PIN_DB6 = GPIO_ODR_ODR1;
const unsigned PIN_DB7 = GPIO_ODR_ODR1;

#define PORT_RS GPIOA
#define PORT_RW GPIOA
#define PORT_E 	GPIOA

#define PORT_DB4 GPIOA
#define PORT_DB5 GPIOA
#define PORT_DB6 GPIOA
#define PORT_DB7 GPIOA


void hd44780_init(void);
void hd44780_setPosition(int col,int row);
void hd44780_showString(char *text, size_t length);
void hd44780_writeData(uint8_t byte);
void hd44780_writeCommand(uint8_t byte);
void hd44780_writeByte(uint8_t byte);
void hd44780_clear(void);
void hd44780_waitReady(void);

void InitializeDisplay()
{
	hd44780_init();
	hd44780_setPosition(0,0);
}

void DisplayData(SensorData sensorData)
{
	hd44780_clear();
	hd44780_setPosition(0,0);
	hd44780_showString("Hello, world!",13);
}

void ShowMessage(char *text)
{
	hd44780_clear();
	hd44780_showString(text,strlen(text));
}

void hd44780_init(void)
{
	//TODO: init pins
	GPIO_InitStructure initStructure;

	initStructure.GPIO_Pin = (GPIO_Pin_0);
	initStructure.GPIO_Mode = GPIO_Mode_PP;
	initStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&initStructure);

	hd44780_writeCommand(HD_DMODE);
	hd44780_writeCommand(HD_ONCOF);
	hd44780_writeCommand(HD_DDRAM);
}

void hd44780_clear(void)
{
	hd44780_writeCommand(HD_CLEAR);
}

void hd44780_setPosition(int col,int row)
{
	//TODO:
}

void hd44780_showString(char *text, size_t length)
{
	size_t q = 0;
	for(q=0; q<length; q++)
	{
		hd44780_writeData(text[q]);
	}
}

void hd44780_writeCommand(uint8_t byte)
{
	//set command pin
	GPIO_WriteBit(PORT_RW,PIN_RS,0);
	hd44780_writeByte(byte);
}

void hd44780_writeData(uint8_t byte)
{
	//unset command pin
	GPIO_WriteBit(PORT_RW,PIN_RS,1);
	hd44780_writeByte(byte);
}

void hd44780_writeByte(uint8_t byte)
{
	hd44780_waitReady();
	GPIO_WriteBit(PORT_RW,PIN_RW,0);

	GPIO_WriteBit(PORT_E,PIN_E,0);
	//set hight 4 bits
	GPIO_WriteBit(PORT_DB7,PIN_DB7,byte&0x80);
	GPIO_WriteBit(PORT_DB6,PIN_DB6,byte&0x40);
	GPIO_WriteBit(PORT_DB5,PIN_DB5,byte&0x20);
	GPIO_WriteBit(PORT_DB4,PIN_DB4,byte&0x10);

	delay_ms(1);
	GPIO_WriteBit(PORT_E,PIN_E,0);

	hd44780_waitReady();

	GPIO_WriteBit(PORT_E,PIN_E,1);
	//set low 4 bits
	GPIO_WriteBit(PORT_DB7,PIN_DB7,byte&0x08);
	GPIO_WriteBit(PORT_DB6,PIN_DB6,byte&0x04);
	GPIO_WriteBit(PORT_DB5,PIN_DB5,byte&0x02);
	GPIO_WriteBit(PORT_DB4,PIN_DB4,byte&0x01);

	delay_ms(1)
	GPIO_WriteBit(PORT_E,PIN_E,0);
}

void hd44780_waitReady(void)
{
	// TODO: implement waiting on pin ready
	delay_ms(5);
}