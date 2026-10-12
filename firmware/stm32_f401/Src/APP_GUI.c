/*
 * APP_GUI.c
 *
 *  Created on: Aug 12, 2023
 *      Author: [removed]
 */

#include"BIT_MATH.h"
#include"STD_TYPES.h"
#include <stdlib.h> // for itoa

#include "MCAL_RCC_Interface.h"
#include "MCAL_GPIO_Interface.h"
#include "UART_Interface.h"
#include "APP_GUI.h"


static u8 array[24];

static u8 GUI_u8AppendNumber(u8 *Copy_pu8Buffer, u8 Copy_u8Index, u32 Copy_u32Value)
{
	u8 Local_u8Digits[11];
	u8 Local_u8DigitIndex = 0;

	itoa(Copy_u32Value, (char *)Local_u8Digits, 10);

	while ((Local_u8Digits[Local_u8DigitIndex] != '\0') && (Copy_u8Index < 23))
	{
		Copy_pu8Buffer[Copy_u8Index++] = Local_u8Digits[Local_u8DigitIndex++];
	}

	return Copy_u8Index;
}

void GUI_voidInit(void)
{


	//TX->PA9  RX->PA10
	GPIO_voidSetPinMode(GPIO_PORTA, GPIO_PIN9, GPIO_MODE_AF_PP);
	GPIO_voidSetPinMode(GPIO_PORTA, GPIO_PIN10, GPIO_MODE_AF_PP);
	GPIO_voidSetPinALTF(GPIO_PORTA, GPIO_PIN9, GPIO_AF7_USART1_USART2_SPI1);
	GPIO_voidSetPinALTF(GPIO_PORTA, GPIO_PIN10,GPIO_AF7_USART1_USART2_SPI1);

	UART_voidInit();
}


void GUI_voidSendSpeedDistance(u32 Copy_u32Speed, u32 Copy_u32Distance)
{
	u8 Local_u8Index = 0;

	Local_u8Index = GUI_u8AppendNumber(array, Local_u8Index, Copy_u32Speed);
	array[Local_u8Index++] = ' ';
	Local_u8Index = GUI_u8AppendNumber(array, Local_u8Index, Copy_u32Distance);
	array[Local_u8Index++] = '\r';
	array[Local_u8Index++] = '\n';

	UART_voidTransmit(array, Local_u8Index);
}
