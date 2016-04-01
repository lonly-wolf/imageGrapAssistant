#include "init.h"
#include "tc6014.h"
#include "stm32f10x.h"

#define READ_IO 	0x3e
#define WRITE_IO 	0x3f
#define Speed_Set	0x4a   
#define Read_Status	0x4b  
#define CLR_L	    0x4c
#define Fix_Drive	0x4d
#define Con_Drive	0x4e 
#define Read_Input	0x4f
#define Outside_Signals_Drive 0x50
#define Clear_mode 0x51
#define SYN 0X52
#define Clear_SYN 0X53
#define Home_Search 0x54
#define ITP_Single 0x55
#define Stop 0x56
#define Read_LP 0x57
#define READ_REG_2 	0x58
#define	read_speed  0x60


unsigned char uart1_rxd_buf[100];
unsigned char uart1_counter = 0;
unsigned char tc6014_it_flag;
unsigned char speed[4] = {0}; //  00 01 00 64

unsigned char reversal[8]= {0};;// 00 01 08 20 00 00 01 ff
unsigned char foreward[8]= {0};;// 00 01 08 21 00 00 01 ff
unsigned char Continuous_Drive[3] ={0};
unsigned char stop[3] =	{0};
unsigned char ReadData[10]={0};
unsigned char i;
int logic_position;
int logic_position_1;
unsigned short limit_state;
unsigned char t1=0,t2=0,t3=0,t4=0,t5=0,t6=0;
void Delay(int nCount) 
{   	
	for(; nCount!= 0;nCount--);  
}
int main(void)
{
	//**********************   0    1    2    3    4    5    6    7    8    9    10   11   12   13   14   15   16   17   18   19   20  21    22   23  
	//unsigned char Speed[24] = {0x00,0x0f,0x00,0x0c,0x35,0x00,0x00,0x28,0x00,0x28,0x00,0x0a,0x00,0x0a,0x00,0x64,0x07,0xd0,0x00,0x00,0x00,0x00,0x00,0x00};
//	unsigned char Speed[24] = {0x00,0x0f,0x00,0x0c,0x35,0x00,0x00,0x05,0x00,0x05,0x00,0x0a,0x00,0x0a,0x00,0x00,0x07,0xd0,0x00,0x00,0x00,0x00,0x00,0x00};
//	unsigned char Mode[8] = {0x00,0x0f,0x01,0x00,0x00,0x01,0x00,0x00};	
   int last_speed;
  // unsigned char speed_1[4] = {0}; //  00 01 00 64
	RCC_Config();
	IO_Config();
	UART1_Config();
	FSMC_Config();
	NVIC_Config();
	
	Software_Reset();//�����λ
	Delay(1000);	
	TC6014_Config();	
	//Axis_Speed_Config(Speed);      //Data 0������ 1��f������ 2~5����10 6~7���ٶ�40 8~9���ٶ�40 10~11���ٶ���10 12~13���ٶ���10  14~15��ʼ�ٶ�100 16~17�����ٶ�2000 18~19���ٶ�ƫ��0 20~23�ֶ����ٵ�0
	//AXIS_Mode_Config(Mode);       //Data 0������ 1��-f������ 2ֹͣģʽ-����ֹͣ 3����λ�߼���ƽ-�͵�ƽ 4����λ�߼���ƽ-�͵�ƽ  5����ģʽ-��������  6��������߼���ƽ-�͵�ƽ 7������巽���ƽ-�͵�ƽ

	while(1)				    
	{
		if(uart1_counter)
		{
			switch(uart1_rxd_buf[0])
			{
				case READ_IO://��IO 0x3e 05   ��RR4 ��5λ
					if(uart1_counter == 2)
					{
						uart1_counter = 0;											
						USART_SendData(USART1,IO_ReadBit(uart1_rxd_buf[1]));
						AXIS_Switch();
					}
					break;
				case WRITE_IO://дIO  0x3f 03 01  ��WR4��3λ�ø�
					if(uart1_counter == 3)
					{
						uart1_counter = 0;
						IO_WriteBit(uart1_rxd_buf[1],uart1_rxd_buf[2]);
						USART_SendData(USART1,WRITE_IO);
						AXIS_Switch();
					}
					break;
				case Speed_Set://�����ٶȲ��� 0x4a,0x0f,0x00,0x0c,0x35,0x00,0x00,0x28,0x00,0x28,0x00,0x0a,0x00,0x0a,0x00,0x64,0x07,0xd0,0x00,0x00,0x00,0x00,0x00,0x00
								//0f������ ��Χ0x0c3500  ���ٶ�0x0028 ���ٶ�0x0028 ���ٶ���0x000a ���ٶ���0x000a  ��ʼ�ٶ�0x0064 �����ٶ�0x07d0 ���ٶ�ƫ��0x0000 20~23�ֶ����ٵ�0x0000
				if(uart1_counter == 4)	 //0ָ�� 1�� 2��3�����ٶ�
					{
						uart1_counter = 0;	
						Axis_Speed_Config(uart1_rxd_buf);
						//TC6014_Config()

						USART_SendData(USART1,Speed_Set);
						AXIS_Switch();
					}
					break; 
				case CLR_L://����߼�λ�� 0x4c 03 ���XY���߼�λ��
					if(uart1_counter == 2)
					{
						uart1_counter = 0;	
						LP_Set((AXIS_NUM)(uart1_rxd_buf[1]<<8),0);
						USART_SendData(USART1,CLR_L);
						AXIS_Switch();
					}
					break;
				case Read_LP://���߼�λ��  0x57 01 ��X���߼�λ��
					if(uart1_counter == 2)
					{
						uart1_counter = 0;	
						logic_position=Read_RLP((AXIS_NUM)(uart1_rxd_buf[1]<<8));
					   	uart1_rxd_buf[1]&=0xfe;
						logic_position_1= Read_RLP((AXIS_NUM)(uart1_rxd_buf[1]<<8));   //joose

//						ReadData[0] = (unsigned char) logic_position;
//						ReadData[1] = (unsigned char)(logic_position>>8);
//						ReadData[2] = (unsigned char)logic_position>>16;
//						ReadData[3] = (unsigned char)(logic_position>>24);
					//	USART_SendData(USART1,0x58);
						ReadData[9] = (unsigned char) 	Read_LP;
						ReadData[8] = (unsigned char) 	logic_position_1;
						ReadData[7] = (unsigned char) 	(logic_position_1>>8);	
						ReadData[6] = (unsigned char)	(logic_position_1>>16);   
						ReadData[5] = (unsigned char)	(logic_position_1>>24);
						ReadData[4] = (unsigned char)  	logic_position;
						ReadData[3] = (unsigned char) 	(logic_position>>8);
						ReadData[2] = (unsigned char)	(logic_position>>16);
						ReadData[1] = (unsigned char)	(logic_position>>24);
						ReadData[0] = 0x09;

						for(i=0;i<10;i++)
						{
							 USART_SendData(USART1,ReadData[i]);
							 Delay(0xffff);
						}						 
						AXIS_Switch();
					}
					break;
				case Fix_Drive://�̶�������� 0x4d 03 08 20 00 00 ff ff  XY������̶�65535����������
					if(uart1_counter == 8)
					{
						uart1_counter = 0;	
						Axis_Fixed_Drive(uart1_rxd_buf);
						USART_SendData(USART1,Fix_Drive);
						AXIS_Switch();
					}
					break;
				case Con_Drive://����������� 0x4e 03 22 XY�Ḻ������������
					if(uart1_counter == 3)
					{
						uart1_counter = 0;	
						Axis_Continuous_Drive(uart1_rxd_buf);
						USART_SendData(USART1,Con_Drive);
						AXIS_Switch();
					}
					break;
				case Outside_Signals_Drive://�ⲿ�ź����� 0x50 01 ff ff ff ff 02 �ⲿ�ź�����X�����ffffffff������
					if(uart1_counter == 6)
					{
						uart1_counter = 0;	
						External_Signals_Drive(uart1_rxd_buf);
						USART_SendData(USART1,Outside_Signals_Drive);
						AXIS_Switch();
					}
					break;
				case Clear_mode://����ⲿ�����ź� 0x51 
					if(uart1_counter == 1)
					{
						uart1_counter = 0;	
						Clear_External_Drive();
						USART_SendData(USART1,Clear_mode);
						AXIS_Switch();
					}
					break;
				case SYN://ͬ���˶� 0x00,0x01,0x02,0x04,0x08,0x00,0x00,0xff,0xff,0x00,0x02,0xff,0xff,0x00,0x02,0xff,0xff,0x00,0x02,0xff,0xff,0x00,0x02,0xff,0xff,0x00,0x10,0xe0,0x01,0x00,0x08,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x04,0x00,0x00,0x22,0x25,0x25,0x25
					if(uart1_counter == 45)
					{
						uart1_counter = 0;	
						SYN_Action(uart1_rxd_buf);
						USART_SendData(USART1,SYN);
						AXIS_Switch();
					}
					break;
				case Home_Search://�Զ���ԭ�� 0x54,0x01,0x00,0x01,0x4f,0x5f,0x00,0x00,0x60,0x00,0x00,0x0f,0xff  IN0��IN1����һ����Ϊ��ԭ���ź�
								 //�Զ���ԭ�� 0x54,0x01,0x00,0x05,0x4f,0x5f,0x00,0x00,0x60,0x00,0x00,0x0f,0xff   limit signal��λ�ź���Ϊ��ԭ���ź�
				                 //�Զ���ԭ�� 0x54,0x01,0x00,0x49,0x5f,0x5f,0x00,0x00,0x60,0x00,0x00,0x0f,0xff  �����nIN0�����nIN2��Z���ź�IN2��Ϊ��ԭ���ź�
				if(uart1_counter == 2)
					{
						uart1_counter = 0;
						logic_position=0x0000;
						logic_position_1=0x0000;						
						
						USART_SendData(USART1,Home_Search);
						while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){};
						Continuous_Drive[0]	= 0x4e;
						Continuous_Drive[1]	= uart1_rxd_buf[1];
						Continuous_Drive[2]	=  0x23;
						t1 &= 0x00;
						while(!t1)
						{
							//USART_SendData(USART1,0x22);
							Set_Axis((AXIS_NUM)(uart1_rxd_buf[1]<<8));//ѡ��
							Axis_Continuous_Drive(Continuous_Drive);						
						 	limit_state = Reg_Read(R2);
							t1 =  limit_state;
							t1 &= 0x08;
						}  
						t1 &= 0x00;
						USART_SendData(USART1,0x22);
						while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){};
						speed[0] =0x00;
						speed[1] =uart1_rxd_buf[1];
						speed[2] =0x00;
						speed[3] =0x64;		 //��


					   	reversal[0]=0x00;
						reversal[1]=uart1_rxd_buf[1];
						reversal[2]=0x08;
						reversal[3]=0x20;
						reversal[4]=0x00;
						reversal[5]=0x00;
						reversal[6]=0x00;
						reversal[7]=0xff;

						foreward[0]=0x00;
						foreward[1]=uart1_rxd_buf[1];
						foreward[2]=0x08;
						foreward[3]=0x21;
						foreward[4]=0x00;
						foreward[5]=0x00;
						foreward[6]=0x00;
						foreward[7]=0xff;

						stop[0] = 0x56;					
						stop[1] = uart1_rxd_buf[1];
						stop[2] = 0x26;



					
						Axis_Speed_Config(speed); //  00 01 00 64
						Delay(1000000);
						logic_position=Read_RLP((AXIS_NUM)(uart1_rxd_buf[1]<<8));
						Axis_Fixed_Drive(reversal);
						USART_SendData(USART1,0x33);
						while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){};

						while(logic_position_1 !=logic_position + 0xff)
							{	
								USART_SendData(USART1,0xaa);															
								logic_position_1=Read_RLP((AXIS_NUM)(uart1_rxd_buf[1]<<8));
								Delay(3000000);							
							}

//						logic_position =0x0000;
//						logic_position =0xffff;
//						while(logic_position !=logic_position_1)
//						{
//							logic_position=Read_RLP((AXIS_NUM)(uart1_rxd_buf[1]<<8));
//							Delay(3000000);
//							logic_position_1=Read_RLP((AXIS_NUM)(uart1_rxd_buf[1]<<8));							
//						}
						USART_SendData(USART1,0x44);
						while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){};
						Delay(3000000);	
						Axis_Fixed_Drive(foreward);// 00 01 08 21 00 00 01 ff

						t1 &= 0x00;
						while(!t1)
						{	
							//USART_SendData(USART1,0x99);				
						 	limit_state = Reg_Read(R2);
							t1 =  limit_state;
							t1 &= 0x08;
						} 
						Delay(3000000);
						USART_SendData(USART1,0x55);
						while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){};
						Axis_Fixed_Drive(foreward);
						USART_SendData(USART1,0x66);
						while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){};
						t1 &= 0x00;
						while(!t1)
						{		
							//USART_SendData(USART1,0x77);
							while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){};			
						 	limit_state = Reg_Read(R2);
							t1 =  limit_state;
							t1 &= 0x08;
						} 
						USART_SendData(USART1,0x88);
						while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){};
					   LP_Set((AXIS_NUM)(uart1_rxd_buf[1]<<8),0);				
					   USART_SendData(USART1,0x4c);					 
					}
					break;
				case ITP_Single://�����岹0x55 0x00(�ⲿ�ź�)
					if(uart1_counter == 2)
					{
						uart1_counter = 0;	
						ITP_Single_Step(uart1_rxd_buf);
						USART_SendData(USART1,ITP_Single);
						AXIS_Switch();
					}
					break;
				case Stop://ֹͣ���� 0x56 03 26 ����ֹͣXY������
					if(uart1_counter == 3)
					{
						uart1_counter = 0;	
						AXIS_Stop(uart1_rxd_buf);
						USART_SendData(USART1,Stop);
						AXIS_Switch();
					}
					break;
				 case I2L:	 //2��ֱ�߲岹��30 01 02 00 00 ff ff 00 00 ff ff 3b
				 				//0������ 1�� 2�� 3~6��1������ 7~10��2������ 11 �����Ƿ���Ч
				 	if(uart1_counter == 12)
					{
						uart1_counter = 0;	
						ITP_2AxisLine(uart1_rxd_buf);
						USART_SendData(USART1,I2L);
						AXIS_Switch();
					}
					break;
				case READ_REG_2:	//58
					if(uart1_counter == 2)
					{
						uart1_counter = 0;

						USART_SendData(USART1,READ_REG_2);
						while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){};
						Set_Axis((AXIS_NUM)(uart1_rxd_buf[1]<<8)); //Data[1]ѡ��	
						limit_state = Reg_Read(R2);
						t1 =  limit_state;
						t2 =  limit_state>>8;
						USART_SendData(USART1,t1);
						while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){};
						USART_SendData(USART1,t2);
						AXIS_Switch();
					}
					break;
				case read_speed: // 0x60
					if(uart1_counter == 2)
					{
						uart1_counter = 0;
						USART_SendData(USART1,read_speed);
						Delay(0xffff);
						//while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){};
						last_speed=Read_RV((AXIS_NUM)(uart1_rxd_buf[1]<<8));
						t3 =  last_speed;
						t4 =  last_speed>>8;
						t5 =  last_speed>>16;
						t6 =  last_speed>>24;
						USART_SendData(USART1,t3);
						Delay(0xffff);
						//while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){};
						USART_SendData(USART1,t4);
						Delay(0xffff);
						USART_SendData(USART1,t5);
						Delay(0xffff);
						USART_SendData(USART1,t6);
						Delay(0xffff);
						//while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){};
						AXIS_Switch();
					}
					break;
				 		 		
				default:
					uart1_counter = 0;
					USART_SendData(USART1,0x55);
					break;
			}
			
		}
	}

}

































