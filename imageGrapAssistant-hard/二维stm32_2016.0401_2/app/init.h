/**
 *******************************************************************************************
 **标题：
 **作者：
 **日期：
 **版本：
 **说明：
 *******************************************************************************************
**/

#ifndef	_INIT_H
#define	_INIT_H

//
#define RS485_TR GPIO_Pin_8

void RCC_Config(void);
void IO_Config(void);
void UART1_Config(void);
void UART2_Config(void);
void FSMC_Config(void);
void NVIC_Config(void);
void TC6014_Config(void);
void USART_SendDataNUM(unsigned short *pucRegBuffer,unsigned char Number);
#endif

//----------------------------------------- end of file-------------------------------------------
