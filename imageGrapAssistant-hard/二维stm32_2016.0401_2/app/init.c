/**
 *********************************************************************
 **标题：
 **作者：
 **日期：
 **版本：
 **说明：
 *********************************************************************
***/

#include "stm32f10x.h"
#include "init.h"
#include "tc6014.h"

#define BAUDRATE	115200

void RCC_Config()
{
	/* DMA clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	//
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA			//
							| RCC_APB2Periph_GPIOB		//
							| RCC_APB2Periph_GPIOC		//
							| RCC_APB2Periph_GPIOD		//
							| RCC_APB2Periph_GPIOE,		//
							ENABLE);
	
	//
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	//
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	//
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		
}

/**********************************************************************************************
* 说明：IO口配置
* 参数：无
* 返回值：无
**********************************************************************************************/
void IO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//tc6014中断申请脚，连接PD2，配置其为外部中断管脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource2);
	
	//-------------------------------------------- IO -------------------------------------------
	//16路通用输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	//OUT1-OUT16
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//16路通用输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	//IN1-IN2,IN4-IN16
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//IN3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//----------------------------------------- 485使能管脚 --------------------------------------
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = RS485_TR;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//--------------------------------------------- uart1 -----------------------------------------
	//USART1 TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//USART1 RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//-------------------------------------------- uart2 -----------------------------------------
	//USART2 TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//USART2 RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//-------------------------------------------- fsmc ------------------------------------------
	/***************************************** NOR数据线 *******************************************
	+--------------------------------------------------------------------------------------------+
	| D0 	 D1   D2    D3   D4   D5   D6 	D7   D8 	D9    D10 	 D11   D12 	D13  D14   D15   |
	|  |      |     |    |    |    |    |    |     |     |      |     |     |     |   |      |   |
	| PD14  PD15  PD0  PD1  PE7  PE8  PE9  PE10  PE11  PE12  PE13  PE14  PE15  PD8  PD9   PD10   |
	+--------------------------------------------------------------------------------------------+
	***********************************************************************************************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 |
                                GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 |
                                GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	/* NOR地址线 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	/* 读（PD4）写（PD5）管脚配置 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	/* 片选配置 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	/* NOR忙或等待信号 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

/**********************************************************************************************
* 名称：
* 参数：
* 返回值：
* 说明：
**********************************************************************************************/
void UART1_Config()
{
	USART_InitTypeDef USART_InitStructure;
	
	USART_DeInit(USART1);
	USART_InitStructure.USART_BaudRate = BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		
	USART_Cmd(USART1,ENABLE);
}

/**********************************************************************************************
* 名称：
* 参数：
* 返回值：
* 说明：
**********************************************************************************************/
void UART2_Config()
{
	USART_InitTypeDef USART_InitStructure;
	
	USART_DeInit(USART2);
	USART_InitStructure.USART_BaudRate = BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2,&USART_InitStructure);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	USART_Cmd(USART2,ENABLE);
}


void USART_SendDataNUM(unsigned short *pucRegBuffer,unsigned char Number)
{
	unsigned char i=0;
	for(i=0;i<Number;i++)
	{
		USART_SendData(USART1, (unsigned char)(*pucRegBuffer & 0xff) ); //低八位
		
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);//等待串口发送完毕
		USART_SendData(USART1, (unsigned char)(*pucRegBuffer >> 8)); //高八位
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);//等待串口发送完毕
		*pucRegBuffer++;
	}
	*pucRegBuffer = 0;
}

/**********************************************************************************************
* 名称：
* 参数：
* 返回值：
* 说明：
**********************************************************************************************/
void FSMC_Config()
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  p;
	
	/*-- FSMC Configuration ----------------------------------------------------*/
	p.FSMC_AddressSetupTime = 0x00;
	p.FSMC_AddressHoldTime = 0x00;
	p.FSMC_DataSetupTime = 0x05;
	p.FSMC_BusTurnAroundDuration = 0x00;
	p.FSMC_CLKDivision = 0x00;
	p.FSMC_DataLatency = 0x00;
	p.FSMC_AccessMode = FSMC_AccessMode_B;
	
	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
	
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
	
	/*!< Enable FSMC Bank1_NOR Bank */
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}

void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Enable the USART1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable the USART2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable the tc6014 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TC6014_Config()
{
	AXIS_InitTypeDef AXIS_InitStruct;
	Drive_InitTypeDef Drive_InitStruct;
	//
	Set_Axis(AXIS_ALL);
	//
	AXIS_InitStruct.Dir_Level = LOW;
	AXIS_InitStruct.Logic_Level = LOW;
	AXIS_InitStruct.Pulse_Mode = DIR_PULSE;
	AXIS_InitStruct.MLimit_Level = LOW;
	AXIS_InitStruct.PLimit_Level = LOW;
	AXIS_InitStruct.Stop_Mode = SUD_STOP;
	
	AXIS_Init(AXIS_InitStruct);
	//
	Drive_InitStruct.Range = 8000000;
	Drive_InitStruct.Acce = 5;
	Drive_InitStruct.Dece = 5;
	Drive_InitStruct.Acce_Rate = 10;
	Drive_InitStruct.Dece_Rate = 10;
	Drive_InitStruct.Init_Speed = 0;
	Drive_InitStruct.Speed = 500;
	Drive_InitStruct.Acce_Offset = 0;
	Drive_InitStruct.Man_Dece_Pos = 0;
	
	Drive_Init(Drive_InitStruct);
}

//----------------------------------------- end of file-------------------------------------------
