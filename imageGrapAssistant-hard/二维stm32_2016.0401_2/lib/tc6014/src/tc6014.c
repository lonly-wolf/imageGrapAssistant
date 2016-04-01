/**
 *********************************************************************
 **���⣺tc6014.c
 **���ߣ�fyf
 **���ڣ�2014.10.28
 **�汾��v1.0
 **˵����tc6014 driver
 *********************************************************************
**/

#include "tc6014.h"

//�Ĵ���ӳ��
unsigned short wr_map[8] = {0x0000,0x0000,0x0000,0x00000,0x00000,0x0000,0x0000,0x0000};
unsigned short rr_map[8] = {0x0000,0x0000,0x0000,0x00000,0x00000,0x0000,0x0000,0x0000};

unsigned short Itp_Axis(AXIS_NUM axis);

/**********************************************************************************************
* ������reg��ȡö��REG�ж����ֵ������ѡ��ָ���ļĴ���
		datΪҪд��reg������
* ����ֵ����
* ˵�����ú������ڽ�ָ��������д��ָ���ļĴ���
**********************************************************************************************/
void Reg_Write(REG reg, short dat)
{
	*(unsigned short*)(BASE_ADDR + OFFSET * reg) = dat;
}

/**********************************************************************************************
* ������cmd��ȡö��CMD_NUM�ж����������룬����ִ�ж�Ӧ������
* ����ֵ����
* ˵����дָ�����������Ĵ�������ʵ�ֶ�Ӧ�Ĺ���
**********************************************************************************************/
void CMD_Write(CMD_NUM cmd)
{
	wr_map[R0] &= 0x0f00;
	wr_map[R0] |= cmd;
	Reg_Write(R0, wr_map[0]);
}

void AXIS_Switch(void)
{
	wr_map[R0] &= 0x0000;
	wr_map[R0] |= 0x000f;
	Reg_Write(R0, wr_map[R0]);
}
/**********************************************************************************************
* ������cmd��ȡö��CMD_NUM�ж����������룬����ִ�ж�Ӧ������
		datΪ�����Ӧ������
* ����ֵ����
* ˵����ִ�д������ݵ�����
**********************************************************************************************/
void Data_Write(CMD_NUM cmd, int dat)
{
	switch(cmd)
	{
		case WR:		//00h  	��Χ����
		case WP: 		//06  	������������û����յ�����
		case WDP:		//07  	�ٶȼ��ٵ�����
		case WC: 		//08  	Բ������
		case WLP:		//09  	�߼�λ�ü�������ֵ����
		case WEP:		//0A  	��������������ֵ����
		case WCP:		//0B  	����ȽϼĴ�����ֵ����
		case WCM:		//0C  	����ȽϼĴ�������
		case WEM:		//60  	��չģʽ���ã�λ���ݣ�
		case WSM:		//64  	ͬ��ģʽ���ã�λ����)
			Reg_Write(R6, dat);
			Reg_Write(R7, (dat >> 16));
			CMD_Write(cmd);
			break;
		case WK:		//01 	���ٶ�����������
		case WA: 		//02  	���ٶ�����
		case WD: 		//03  	���ٶ�����
		case WSV:  		//04  	�����ٶ�����
		case WV: 		//05  	�����ٶ�����
		case WAO:		//0D  	���ٶȼĴ���ƫ������
		case WL: 		//0E  	���ٶȼ���������
		case WHV:		//61  	ԭ�������ٶ�
			Reg_Write(R6, dat);
			CMD_Write(cmd);
			break;
		default:
			break;
	}
}

/**********************************************************************************************
* ������regΪREG����ö��
* ����ֵ����ȡ��������
* ˵������ȡreg�Ĵ�����Ӧ������
**********************************************************************************************/
unsigned short Reg_Read(REG reg)
{
	return *(unsigned short*)(BASE_ADDR + OFFSET * reg);
}

/**********************************************************************************************
* ������cmdΪCMD_NUM����ö��
* ����ֵ��cmd�����Ӧ����ֵ
* ˵������ȡcmd�����Ӧ��short��16λ��������
**********************************************************************************************/
int Data_Read(CMD_NUM cmd)
{
	int temp;
	
	CMD_Write(cmd);
	switch(cmd)
	{
		case RLP: 		        //10h  	��ȡ��ǰ�߼�λ�ü�������ֵ
		case REP:  				//11  	��ȡ��������ֵ
		case RSB:  				//14  	��ͬ������Ĵ���
			temp = Reg_Read(R7);//      ��ȡR7�е����� Ϊ��16λ
			temp <<= 16;
			temp |= Reg_Read(R6);//     ��ȡR6�е����� Ϊ��16λ
		break;
		case RV:  				//12  	��ȡ��ǰ�����ٶ�
		case RA:  				//13  	���Ӽ��ٶ���ֵ
			temp = Reg_Read(R6);//      ��ȡR6�е����� Ϊ��16λ ��16λΪ0
			break;
		default:
			break;
	}
	
	return temp;
}

/**********************************************************************************************
* ��������
* ����ֵ����
* ˵���������λ
**********************************************************************************************/
void Software_Reset(void)
{
	wr_map[R0] &= 0x0000;//0x0f7f
	wr_map[R0] |= 0x8000;
	Reg_Write(R0, wr_map[0]);
}
/**********************************************************************************************
* ������axisΪAXIS_NUM����ö��
* ����ֵ����
* ˵����ָ����
**********************************************************************************************/
void Set_Axis(AXIS_NUM axis)
{
	wr_map[R0] &= 0x007f;
	wr_map[R0] |= axis;
	Reg_Write(R0, wr_map[R0]);
}

//��Χ����
void Range_Set(AXIS_NUM axis,int data)
{
	Data_Write(WR,data);
}
//���ٶ�����������
void INC_Rate(AXIS_NUM axis,int data)
{
	Data_Write(WK,data);
}
//���ٶȼ���������
void DEC_Rate(AXIS_NUM axis,int data)
{
	Data_Write(WL,data);
}
//���ٶ�����
void INC_Set(AXIS_NUM axis,int data)
{
	Data_Write(WA,data);
}
//���ٶ�����
void DEC_Set(AXIS_NUM axis,int data)
{
	Data_Write(WD,data);
}
//�����ٶ�����
void Start_Speed(AXIS_NUM axis,int data)
{
	Data_Write(WSV,data);
}
//�����ٶ�����
void Drive_Speed(AXIS_NUM axis,int data)
{
	Data_Write(WV,data);
}
//������������û����յ�����
void Pulse_Out(AXIS_NUM axis,int data)
{
	Set_Axis(axis);
	Data_Write(WP,data);
}
//���ٵ�����
void DECP_Set(AXIS_NUM axis,int data)
{
	Set_Axis(axis);
	Data_Write(WDP,data);
}
//Բ������
void Center_Set(AXIS_NUM axis,int data)
{
	Set_Axis(axis);
	Data_Write(WC,data);
}
//�߼�λ�ü�������ֵ����
void LP_Set(AXIS_NUM axis,int data)
{
	Set_Axis(axis);
	Data_Write(WLP,data);
}
//��������������ֵ����
void EP_Set(AXIS_NUM axis,int data)
{
	Set_Axis(axis);
	Data_Write(WEP,data);
}
//����ȽϼĴ�����ֵ����
void CompP_Set(AXIS_NUM axis,int data)
{
	Set_Axis(axis);
	Data_Write(WCP,data);
}
//����ȽϼĴ�������
void CompM_Set(AXIS_NUM axis,int data)
{
	Set_Axis(axis);
	Data_Write(WCM,data);
}
//����ƫ������
void ACC_Offset(AXIS_NUM axis,int data)
{
	Set_Axis(axis);
	Data_Write(WAO,data);
}

//��չģʽ���ã�λ���ݣ�
void EXP_Mode(AXIS_NUM axis,int data)
{
	Set_Axis(axis);
	Data_Write(WEM,data);
}
//ԭ�������ٶ�
void Home_SearchSpeed(AXIS_NUM axis,int data)
{
	Set_Axis(axis);
	Data_Write(WHV,data);
}
//ͬ��ģʽ���ã�λ���ݣ
void SYN_Mode(AXIS_NUM axis,int data)
{
	Set_Axis(axis);
	Data_Write(WSM,data);
}


//Data_Read();

//��ʵ��λ�� ��ȡ��������ֵ
int Read_REP(AXIS_NUM axis)
{
	int Data;
	Set_Axis(axis);
	Data = Data_Read(REP);
	return Data;
}
//���߼�λ�� ��ȡ��ǰ�߼�λ�ü�������ֵ
int Read_RLP(AXIS_NUM axis)
{
	int Data;
	Set_Axis(axis);
	Data = Data_Read(RLP);
	return Data;
}
//��ȡ��ǰ�����ٶ�
int Read_RV(AXIS_NUM axis)
{
	int Data;
	Set_Axis(axis);
	Data = Data_Read(RV);
	return Data;
}
//���Ӽ��ٶ���ֵ
int Read_RA(AXIS_NUM axis)
{
	int Data;
	Set_Axis(axis);
	Data = Data_Read(RA);
	return Data;
}
//��ͬ������Ĵ���
int Read_RSB(AXIS_NUM axis)
{
	int Data;
	Set_Axis(axis);
	Data = Data_Read(RSB);
	return Data;
}


// �ȴ�������ֹͣ
void Wait_Stop(AXIS_NUM axis)
{
	while(Reg_Read(R0) & axis);
}

// Next data setting of waiting for continuous interpolation
//�ȴ������岹��һ�����ݵ����� RR0/D9
void Next_ITP_Wait(void)
{
	while((Reg_Read(R0) & 0x0200) == 0x0);
}

// Next data setting of waiting for BP interpolation
//�ȴ�λģʽ�岹��һ���������� RR0/D13-D14  SC
void BP_ITP_Wait(void)
{
	while((Reg_Read(R0) & 0x6000) == 0x6000);
}
	

/**********************************************************************************************
* ��������
* ����ֵ��ֹͣ�������
* ˵��������ֹͣ����� 
**********************************************************************************************/
AXIS_NUM Stop_Axis(void)
{
	AXIS_NUM temp;
	temp = (AXIS_NUM)((~(Reg_Read(R0)<<8)) & AXIS_ALL);
	switch(temp)
	{
		case AXIS_NONE:		   //0x0000
			temp = AXIS_NONE;
		break;
		case AXIS_X:		  //0x0100
			temp = AXIS_X;
		break;
		case AXIS_Y:		  //0x0200
			temp = AXIS_Y;
		break;
		case AXIS_X_Y:		  //0x0300
			temp = AXIS_X_Y;
		break;
		case AXIS_Z:		  //0x0400
			temp = AXIS_Z;
		break;
        case AXIS_X_Z:		  //0x0500
			temp = AXIS_X_Z;
		break;
		case AXIS_Y_Z:		  //0x0600
			temp = AXIS_Y_Z;
		break;
		case AXIS_X_Y_Z:	  //0x0700
			temp = AXIS_X_Y_Z;
		break;
		case AXIS_U:		  //0x0800
			temp = AXIS_U;	
		break;		
        case AXIS_X_U:		  //0x0900
			temp = AXIS_X_U;
		break;
		case AXIS_Y_U:		  //0x0A00
			temp = AXIS_Y_U;
		break;
		case AXIS_X_Y_U:	  //0x0B00
			temp = AXIS_X_Y_U;
		break;
		case AXIS_Z_U:		  //0x0C00
			temp = AXIS_Z_U;
		break;		
        case AXIS_X_Z_U:	  //0x0D00
			temp = AXIS_X_Z_U;
		break;
		case AXIS_Y_Z_U:	  //0x0E00
			temp = AXIS_Y_Z_U;
		break;
		case AXIS_ALL:		 //0x0F00
			temp = AXIS_ALL;
		break;
					
	}
	return temp;
	
}

/**********************************************************************************************
* ��������
* ����ֵ����
* ˵�������س�������� 
**********************************************************************************************/
AXIS_NUM Error_Axis(void)
{
	AXIS_NUM temp; 
	temp = (AXIS_NUM)((Reg_Read(R0)<<4)&AXIS_ALL);
	switch(temp)
	{
		case AXIS_NONE:		   //0x0000
			temp = AXIS_NONE;
		break;
		case AXIS_X:		  //0x0100
			temp = AXIS_X;
		break;
		case AXIS_Y:		  //0x0200
			temp = AXIS_Y;
		break;
		case AXIS_X_Y:		  //0x0300
			temp = AXIS_X_Y;
		break;
		case AXIS_Z:		  //0x0400
			temp = AXIS_Z;
		break;
        case AXIS_X_Z:		  //0x0500
			temp = AXIS_X_Z;
		break;
		case AXIS_Y_Z:		  //0x0600
			temp = AXIS_Y_Z;
		break;
		case AXIS_X_Y_Z:	  //0x0700
			temp = AXIS_X_Y_Z;
		break;
		case AXIS_U:		  //0x0800
			temp = AXIS_U;	
		break;		
        case AXIS_X_U:		  //0x0900
			temp = AXIS_X_U;
		break;
		case AXIS_Y_U:		  //0x0A00
			temp = AXIS_Y_U;
		break;
		case AXIS_X_Y_U:	  //0x0B00
			temp = AXIS_X_Y_U;
		break;
		case AXIS_Z_U:		  //0x0C00
			temp = AXIS_Z_U;
		break;		
        case AXIS_X_Z_U:	  //0x0D00
			temp = AXIS_X_Z_U;
		break;
		case AXIS_Y_Z_U:	  //0x0E00
			temp = AXIS_Y_Z_U;
		break;
		case AXIS_ALL:		 //0x0F00
			temp = AXIS_ALL;
		break;		
	}
	return temp;
}

/**********************************************************************************************
* ������it:ΪIT_NUM����ö�� STATE:ʹ��/ʧ�� WR1--D8~D15
* ����ֵ����
* ˵�������ж�����
**********************************************************************************************/
void IT_Axis_Config(IT_NUM it, STATE state)
{
	if(state == Enable)
	{
		wr_map[R1] &= 0x00ff;
		wr_map[R1] |= it;
	}
	else
	{
		wr_map[R1] &= (~it);
	}
	
	Reg_Write(R1, wr_map[R1]);
}
/**********************************************************************************************
* ������ʧ��/ʹ�� WR5--D14
* ����ֵ����
* ˵���������岹�ж�����
**********************************************************************************************/
// interpolationdata of next node...*after next interpolation command is written, the interrupt will be cleared.
void IT_ITP_Config(STATE state)
{
	if(state == Enable)
	{
		wr_map[R5] &= 0xbfff;
		wr_map[R5] |= 0x4000;
	}
	else
	{
		wr_map[R5] &= 0xbfff;
	}
	Reg_Write(R5, wr_map[R5]);
}

/**********************************************************************************************
* ������λģʽ�岹�ж� ʧ��/ʹ�� WR5--D15
* ����ֵ����
* ˵����λģʽ�岹�ж�����
**********************************************************************************************/
//In bit pattern interpolation, when the value of stack counter (SC) is changed from 2
//to 1, and the stack is available for next BP command writing...
//*after a BP command for the stack is written, the interrupt will be cleared.
void IT_BP_Config(STATE state)
{
	if(state == Enable)
	{
		wr_map[R5] &= 0x7fff;
		wr_map[R5] |= 0x8000;
	}
	else
	{
		wr_map[R5] &= 0x7fff;
	}
	Reg_Write(R5, wr_map[R5]);
}

/**********************************************************************************************
* ��������ԭ���ж� ʹ��ENB   ʹ��DIS
* ����ֵ��
* ˵������ԭ���ж�ʹ��/ʧ��
**********************************************************************************************/
//��ԭ���ж� ���ⲿģʽ��60h����   WR6/D5(HMINT)   ���ж�RR3/D8(HMEND)  
//Automatic home search is terminated.
void IT_Home_Config(STATE state)
{
	if(state == Enable)
	{
		wr_map[R6] &= 0xffdf;
		wr_map[R6] |= 0x0020;
	}
	else
	{
		wr_map[R6] &= 0xffdf;
	}
	Reg_Write(R6, wr_map[R6]);
	CMD_Write(WEM);	
}
/**********************************************************************************************
* ������ͬ���˶��ж� ʹ��ENB   ʹ��DIS
* ����ֵ��
* ˵����ͬ���˶��ж�ʹ��/ʧ��
**********************************************************************************************/
//ͬ���˶��ж� ͬ��ģʽ����64h   WR7/D15 (INT)    ���ж�RR3/D9(SYNC)
//A synchronous action is activated due to the specifiedactivation factor.
void IT_SYN_Config(STATE state)
{
	if(state == Enable)
	{
		wr_map[R7] &= 0x0fff;
		wr_map[R7] |= 0x1000;
	}
	else
	{
		wr_map[R7] &= 0x0fff;
	}
	Reg_Write(R7, wr_map[R7]);
	CMD_Write(WSM);	
}


/**********************************************************************************************
* ��������
* ����ֵ��1�жϷ��� 0�ж�û����
* ˵������ȡ�ж��
**********************************************************************************************/
unsigned char IT_ITP_Status(void)
{
	unsigned short temp;
	temp = ((Reg_Read(R0)>>9)&0x0001);
	return (unsigned char) temp;
}

/**********************************************************************************************
* ������
* ����ֵ���ж�״̬
		  1   �жϲ���
		  2/3 �ж�û����
* ˵����λģʽ�岹�ж�״̬ SC��2 ��Ϊ1 
**********************************************************************************************/
//λģʽ�岹�ж�״̬ RR0/D14-D13  //(SC) is changed from 2to 1, and the stack is available for next BP command writing...
unsigned char IT_BP_Status(void)
{
	unsigned short temp;
	temp = ((Reg_Read(R0)>>13)&0x0003);
	return (unsigned char) temp;
}

/**********************************************************************************************
* ������
* ����ֵ���ж�״̬
		  0 �ж�û����
		  1 �жϲ���
* ˵������ԭ���ж�״̬ ��ԭ������ж�
**********************************************************************************************/
//��ԭ���ж�״̬ RR0/D14-D13  ��ԭ������Ƿ����жϲ���
unsigned char IT_Home_Status(void)
{
	unsigned short temp;
	temp = ((Reg_Read(R3)>>8)&0x0001);
	return (unsigned char) temp;
}

/**********************************************************************************************
* ������
* ����ֵ���ж�״̬
		  0 �ж�û����
		  1 �жϲ���
* ˵������ȡͬ���˶��ж�״̬�
**********************************************************************************************/
//ͬ���˶��ж�״̬ RR0/D14-D13 
unsigned char IT_SYN_Status(void)
{
	unsigned short temp;
	temp = ((Reg_Read(R3)>>9)&0x0001);
	return (unsigned char) temp;
}
/**********************************************************************************************
* ������itΪIT_NUM����ö��
* ����ֵ����
* ˵������ȡ�ж��
**********************************************************************************************/
IT_MASK IT_Axis_Status(void)
{
	IT_MASK temp;
	
	temp = (IT_MASK)(Reg_Read(R3) & IT_ALL_MASK);

	switch(temp)
	{
		case IT_PULSE_MASK:		//0x0001,����������
			temp = IT_PULSE_MASK;
		break;
		case IT_LMC_MASK:		//0x0002,�߼�/���������������ڸ�����ȽϼĴ�����ֵ
			temp = IT_LMC_MASK;
		break;
		case IT_SMC_MASK:		//0x0004,�߼�/������������С�ڸ�����ȽϼĴ�����ֵ
			temp = IT_SMC_MASK;
		break;
		case IT_SPC_MASK:		//0x0008,�߼�/������������С��������ȽϼĴ�����ֵ
			temp = IT_SPC_MASK;
		break;
		case IT_LPC_MASK:		//0x0010,�߼�/����������������������ȽϼĴ�����ֵ
			temp = IT_LPC_MASK;
		break;
		case IT_CEND_MASK:		//0x0020,�Ӽ�������ʱ������������������
			temp = IT_CEND_MASK;
		break;
		case IT_CSTA_MASK:		//0x0040,�Ӽ�������ʱ��������ʼ�������
			temp = IT_CSTA_MASK;
		break;
		case IT_DEND_MASK:		//0x0080,��������
			temp = IT_DEND_MASK;
		break;
		case IT_HOME_MASK:		//0x0100,ԭ������
			temp = IT_HOME_MASK;
		break;
		case IT_SYNC_MASK:		//0x0200,ͬ������
			temp = IT_SYNC_MASK;
		break;
		default:
			temp = IT_ERR_MASK;
		break;
	}
	
	return temp;
}
/**********************************************************************************************
* ��������
* ����ֵ�������״̬Դ
* ˵������ȡ����Դ
**********************************************************************************************/
//��ȡ����Դ
ERR_MASK Error_Is(void)
{
	ERR_MASK temp;
	
	temp = (ERR_MASK)((Reg_Read(R2) & ERR_ALL_MASK));

	switch(temp)
	{
		case ERR_LPC_MASK:		//0x0001,�߼�/����������������������ȽϼĴ�����ֵ
			temp = ERR_LPC_MASK;
		break;
		case ERR_SMC_MASK:		//0x0002,�߼�/���������������ڸ�����ȽϼĴ�����ֵ
			temp = ERR_SMC_MASK;
		break;
		case ERR_PL_MASK:		//0x0004,Ӳ������λ
			temp = ERR_PL_MASK;
		break;
		case ERR_ML_MASK:		//0x0008,Ӳ������λ
			temp = ERR_ML_MASK;
		break;
		case ERR_ALARM_MASK:	//0x0010,�ŷ�����
			temp = ERR_ALARM_MASK;
		break;
		case ERR_EMG_MASK:		//��ͣ
			temp = ERR_EMG_MASK;
		break;
		default:
			temp = ERR_ERR_MASK;
		break;
	}

	return temp;
}


/**********************************************************************************************
* ������Encoder_InitStructΪENCODER_InitTypeDef����ö��
* ����ֵ����
* ˵������Encoder_InitStruct�����õĲ�����ʼ��������
**********************************************************************************************/
void Encoder_Init(Encoder_InitTypeDef Encoder_InitStruct)
{
	wr_map[R2] &= 0xf1ff;
	wr_map[R2] |= (Encoder_InitStruct.Pulse_Mode << 9)
					|(Encoder_InitStruct.Pulse_Mul);
	Reg_Write(R2, wr_map[R2]);
}

/**********************************************************************************************
* ������Counter_ValueΪ�������Ĵ�������ֵ
* ����ֵ����
* ˵�������ñ�������������ֵ
**********************************************************************************************/
void Encoder_SetCounter(int Counter_Value)
{
	Data_Write(WEP, Counter_Value);
}

/**********************************************************************************************
* ���ƣ�int Encoder_GetCounter()
* ��������
* ����ֵ������������ֵ
* ˵������ȡ��������������ֵ
**********************************************************************************************/
int Encoder_GetCounter()
{
	return Data_Read(REP);
}

/**********************************************************************************************
* ������Comp_InitStructΪComp_InitTypeDef����ö�٣�
		���ж���ȽϼĴ�����Ϣ
* ����ֵ����
* ˵������Comp_InitStruct�����õĲ�����ʼ���Ƚ���
**********************************************************************************************/
void Comp_Init(Comp_InitTypeDef Comp_InitStruct)
{
	wr_map[R2] &= 0xffdc;
	
	wr_map[R2] |= Comp_InitStruct.PComp_State
				| (Comp_InitStruct.MComp_State << 1)
				| (Comp_InitStruct.Comp_Obj << 5);
	Reg_Write(R2, wr_map[R2]);
	
	Data_Write(WCP, Comp_InitStruct.PComp_Value);
	Data_Write(WCM, Comp_InitStruct.MComp_Value);
}

/**********************************************************************************************
* ����������ָ��
		Data0      ������	
		Data1      ѡ�� 01-X�� 02-Y�� 04-Z��  08-U��
		Data2      ����ȽϼĴ���״̬ STATE��0ʹ�� 1ʧ��
		Data3      ����ȽϼĴ���״̬ STATE��0ʹ�� 1ʧ��
		Data4      0x00:�Ƚ϶���LG_CNT = 0x0000�߼�������   0x20:EC_CNT = 0x0020������������	  	
		Data5~8    ����ȽϼĴ�����ֵ
		Data9~12   ����ȽϼĴ�����ֵ	
* ����ֵ����
* ˵��   �ȽϼĴ������� 1�� 2����ȽϼĴ���״̬ 3����ȽϼĴ���״̬ 4�Ƚ϶��� 5~8����ȽϼĴ�����ֵ 9~12����ȽϼĴ�����ֵ	
**********************************************************************************************/
void Comp_Config(unsigned char *temp)
{
	Comp_InitTypeDef Comp_InitStruct;
	unsigned char i;
	unsigned short Data[13];
	for(i=0;i<13;i++)
	{
		Data[i] = (Data[i])|(*temp++);
	}
	Set_Axis((AXIS_NUM)(Data[1]<<8));//ѡ��
//Data 0������ 1�� 2����ȽϼĴ���״̬ 3����ȽϼĴ���״̬ 4�Ƚ϶��� 5~8����ȽϼĴ�����ֵ 9~12����ȽϼĴ�����ֵ	
	Comp_InitStruct.PComp_State = (STATE)Data[2];
	Comp_InitStruct.MComp_State = (STATE)Data[3];
	Comp_InitStruct.Comp_Obj = (OBJ)Data[4];
	Comp_InitStruct.PComp_Value = (Data[5]<<24) | (Data[6]<<16) | (Data[7]<<8) | (Data[8]);
	Comp_InitStruct.MComp_Value = (Data[9]<<24) | (Data[10]<<16) | (Data[11]<<8) | (Data[12]);
	Comp_Init(Comp_InitStruct);	
}
/**********************************************************************************************
* ������AXIS_InitStructΪAXIS_InitTypeDef����ö�٣�
		���ж���Ӳ����λ������ģʽ��Ϣ
* ����ֵ����
* ˵������AXIS_InitStuct�����õĲ�����ʼ��������
**********************************************************************************************/
void AXIS_Init(AXIS_InitTypeDef AXIS_InitStruct)
{
	unsigned short temp = 0;
	
	temp = 		(AXIS_InitStruct.Stop_Mode <<2)
			| 	(AXIS_InitStruct.PLimit_Level << 3)
			| 	(AXIS_InitStruct.MLimit_Level << 4)
			| 	(AXIS_InitStruct.Pulse_Mode << 6)
			| 	(AXIS_InitStruct.Logic_Level <<7)
			| 	(AXIS_InitStruct.Dir_Level <<8);
	
	wr_map[R2] &= 0xfe23;
	wr_map[R2] |= temp;
	Reg_Write(R2, wr_map[R2]);
}

/**********************************************************************************************
* ����������ָ��
		Data0   ������	
		Data1   ѡ�� 01-X�� 02-Y�� 04-Z��  08-U��
		Data2   ֹͣģʽ  0����ֹͣ 1����ֹͣ 
		Data3   ����λ�߼���ƽ 0��  1��
		Data4   ����λ�߼���ƽ 0��  1��
		Data5   ����ģʽ  0˫����ģʽ 1����-����ģʽ
		Data6   ��������߼���ƽ 0��  1��
		Data7   ������巽���ƽ 0��  1��	
* ����ֵ����
* ˵��  ��ģʽ���� 1�� 2ֹͣģʽ 3����λ�߼���ƽ 4����λ�߼���ƽ 5����ģʽ  6��������߼���ƽ 7������巽���ƽ
**********************************************************************************************/
void AXIS_Mode_Config(unsigned char *temp)
{
	AXIS_InitTypeDef AXIS_InitStruct;
	unsigned char i;
	unsigned short Data[8];
	for(i=0;i<8;i++)
	{
		Data[i] = (Data[i])|(*temp++);
	}
	Set_Axis((AXIS_NUM)(Data[1]<<8));//ѡ��
//Data 0������ 1�� 2ֹͣģʽ 3����λ�߼���ƽ 4����λ�߼���ƽ 5����ģʽ  6��������߼���ƽ 7������巽���ƽ
	AXIS_InitStruct.Stop_Mode = (STOP_MODE)Data[2];
	AXIS_InitStruct.PLimit_Level = (LEVEL)Data[3];
	AXIS_InitStruct.MLimit_Level = (LEVEL)Data[4];
	AXIS_InitStruct.Pulse_Mode = (PULSE_MODE)Data[5];
	AXIS_InitStruct.Logic_Level = (LEVEL)Data[6];
	AXIS_InitStruct.Dir_Level = (LEVEL)Data[7];
	AXIS_Init(AXIS_InitStruct);
}

/**********************************************************************************************
* ������Drive_InitStructΪDrive_InitTypeDef���ͽṹ��
* ����ֵ����
* ˵�����ýṹ��Drive_InitTypeDef�ж����ֵ��ʼ��ָ������
**********************************************************************************************/
void Drive_Init(Drive_InitTypeDef Drive_InitStruct)
{
	//�ٶȱ仯������
	if(Drive_InitStruct.Range)
		Data_Write(WR, Drive_InitStruct.Range);
	
	//���ٶ�����
	if(Drive_InitStruct.Acce)
		Data_Write(WA, Drive_InitStruct.Acce);
	
	//���ٶ�����
	if(Drive_InitStruct.Dece)
	{
		wr_map[R3] &= 0x0f9d;
		wr_map[R3] |= 0x0002;
		Reg_Write(R3, wr_map[R3]);
		Data_Write(WD, Drive_InitStruct.Dece);
	}
	//���ٶ�ʹ�ü��ٶ�ֵ
	else
	{
		wr_map[R3] &= 0x0f9d;
		Reg_Write(R3, wr_map[R3]);
	}
	
	//S���߼Ӽ��ٱ仯������
	if(Drive_InitStruct.Acce_Rate || Drive_InitStruct.Dece_Rate)
	{
		wr_map[R3] &= 0x0f9b;
		wr_map[R3] |= 0x0004;
		Reg_Write(R3, wr_map[R3]);
		Data_Write(WK, Drive_InitStruct.Acce_Rate);
		Data_Write(WL, Drive_InitStruct.Dece_Rate);
	}
	//ֱ�߼Ӽ�������
	else
	{
		wr_map[R3] &= 0x0f9b;
		Reg_Write(R3, wr_map[R3]);
	}
	
	//���ü���ƫ��ֵ��Ĭ��Ϊ8���ڵ������ٶȵĹ̶���������ʱ��ֵӦ��Ϊ0
	Data_Write(WAO, Drive_InitStruct.Acce_Offset);
	
	//��ʼ�ٶ�����
	if(Drive_InitStruct.Init_Speed)
		Data_Write(WSV, Drive_InitStruct.Init_Speed);
	
	//�����ٶ�����
	if(Drive_InitStruct.Speed)
		Data_Write(WV, Drive_InitStruct.Speed);
	
	//�ֶ����ٵ����ã�Man_Dece_PosΪ0��ʾ���ֶ����ٵ�
	if(Drive_InitStruct.Man_Dece_Pos)
	{
		wr_map[R3] &= 0x0f9e;
		wr_map[R3] |= 0x0001;
		Reg_Write(R3, wr_map[R3]);
		Data_Write(WDP, Drive_InitStruct.Man_Dece_Pos);
	}
	else
	{
		wr_map[R3] &= 0x0f9e;
		Reg_Write(R3, wr_map[R3]);
	}
}

/**********************************************************************************************
* ����������ָ��
		Data0       ������	
		Data1       ѡ�� 01-X�� 02-Y�� 04-Z��  08-U��
		Data2~5      R     ���� Multiple=8000000/R
		Data6~7      A     ���ٶ�ACC=A*25*Multiple
		Data8~9      D     ���ٶ�DEC=D*125*Multiple
		Data10~11    K     ���ٶ���Jerk=62.5*10^6/K*Multiple
		Data12~13    K     ���ٶ���Jerk=62.5*10^6/K*Multiple
		Data14~15    SV    ��ʼ�ٶ�Init_Speed=SV*Multiple 
		Data16~17    V     �����ٶ�Drive_Speed=V*Multiple 
		Data18~19	���ٶ�ƫ��
		Data20~23	�ֶ����ٵ�		
* ����ֵ����
* ˵����Axis ���� ��ʼ/�����ٶ�  ��/���ٶ� ��/���ٶ��� ���ٶ�ƫ�� �ֶ����ٵ�������� 
**********************************************************************************************/
void Axis_Speed_Config(unsigned char *temp)
{
	Drive_InitTypeDef Drive_InitStruct;
	unsigned char i;
	unsigned char Data[4];
	for(i=0;i<4;i++)
	{
		Data[i] = *temp++;
	}
	Set_Axis((AXIS_NUM)(Data[1]<<8));//ѡ��
	//  Data 0������ 1�� 2~5���� 6~7���ٶ� 8~9���ٶ� 10~11���ٶ��� 12~13���ٶ��� 14~15��ʼ�ٶ� 16~17�����ٶ� 18~19���ٶ�ƫ�� 20~23�ֶ����ٵ�
//	Drive_InitStruct.Range = ((Data[2]<<24) | (Data[3]<<16) | (Data[4]<<8) | (Data[5]));   //R   ��ΧR  16000-8000000  4bytes  ���� Multiple=8000000/R
//	Drive_InitStruct.Acce = (Data[6]<<8) | Data[7];              //A     ���ٶ�A     1-8000       2bytes  ACC=A*25*Multiple
//	Drive_InitStruct.Dece = (Data[8]<<8) | Data[9];              //D     ���ٶ�D     1-8000       2bytes  DEC=D*125*Multiple
//	Drive_InitStruct.Acce_Rate = (Data[10]<<8) | Data[11];       //K     ���ٶ���K   1-65535      2bytes  Jerk=62.5*10^6/K*Multiple
//	Drive_InitStruct.Dece_Rate = (Data[12]<<8) | Data[13];       //K     ���ٶ���K   1-65535      2bytes  Jerk=62.5*10^6/K*Multiple
//	Drive_InitStruct.Init_Speed = (Data[14]<<8)| Data[15];       //SV    ��ʼ�ٶ�SV  1-8000       2bytes  Init_Speed=SV*Multiple 
//	Drive_InitStruct.Speed = (Data[16]<<8) | Data[17];           //V     �����ٶ�V   1-8000       2bytes  Drive_Speed=V*Multiple 
//	Drive_InitStruct.Acce_Offset = (Data[18]<<8) | Data[19];     //���ٶ�ƫ��-32768-+32768        2bytes
//	Drive_InitStruct.Man_Dece_Pos = (Data[20]<<24) | (Data[21]<<16) | (Data[22]<<8) | (Data[23]);
															  // �ֶ����ٵ� 0-4294967295 4bytes
	Drive_InitStruct.Range = 8000000;   //R   ��ΧR  16000-8000000  4bytes  ���� Multiple=8000000/R
	Drive_InitStruct.Acce = 1;              //A     ���ٶ�A     1-8000       2bytes  ACC=A*25*Multiple
	Drive_InitStruct.Dece = 1;              //D     ���ٶ�D     1-8000       2bytes  DEC=D*125*Multiple
	Drive_InitStruct.Acce_Rate = 10;       //K     ���ٶ���K   1-65535      2bytes  Jerk=62.5*10^6/K*Multiple
	Drive_InitStruct.Dece_Rate = 10;       //K     ���ٶ���K   1-65535      2bytes  Jerk=62.5*10^6/K*Multiple
	Drive_InitStruct.Init_Speed = 0;       //SV    ��ʼ�ٶ�SV  1-8000       2bytes  Init_Speed=SV*Multiple 
	Drive_InitStruct.Speed = (Data[2]<<8) | Data[3];           //V     �����ٶ�V   1-8000       2bytes  Drive_Speed=V*Multiple 
	Drive_InitStruct.Acce_Offset = 0;     //���ٶ�ƫ��-32768-+32768        2bytes
	Drive_InitStruct.Man_Dece_Pos =0;
															  // �ֶ����ٵ� 0-4294967295 4bytes

//	Drive_InitStruct.Range = 8000000;
//	Drive_InitStruct.Acce = 5;
//	Drive_InitStruct.Dece = 5;
//	Drive_InitStruct.Acce_Rate = 10;
//	Drive_InitStruct.Dece_Rate = 10;
//	Drive_InitStruct.Init_Speed = 0;
//	Drive_InitStruct.Speed = 1500;
//	Drive_InitStruct.Acce_Offset = 0;
//	Drive_InitStruct.Man_Dece_Pos = 0;

	Drive_Init(Drive_InitStruct);
}

/**********************************************************************************************
* ����  ����ָ��
 		DData0  ������	
		Data1  ѡ���������� 01-X�� 02-Y�� 04-Z��  08-U��		
		Data2  8 �̶��ٶ� 
			   0 �Գ����ԼӼ��ٶ���,   �Զ����� �����Σ� 
			   1 �Գ����ԼӼ��ٶ��ʣ�  �ֶ����� �����Σ� 
			   2 �ǶԳ����ԼӼ��ٶ��ʣ��Զ����� �����Σ� 
			   3 �ǶԳ����ԼӼ��ٶ��ʣ��ֶ����� �����Σ� 
			   4 �Գ�S�μӼ��ٶ��ʣ�   �Զ����� ��S�Σ� 
			   5 �Գ�S�μӼ��ٶ��ʣ�   �ֶ����� ��S�Σ� 
			   6 �ǶԳ�S�μӼ��ٶ��ʣ� �Զ����� ��S�Σ�
			   7 �ǶԳ�S�μӼ��ٶ��ʣ� �ֶ����� ��S�Σ�
		Data3  PFD(0x20)��MFD(0x21)��PFD����̶�����������MFD����̶���������
		Data4~7 Ϊ������ 
* ����ֵ����
* ˵������ָ�����˶�mode����,����ģʽ��pulse����������
**********************************************************************************************/
void Axis_Fixed_Drive(unsigned char *temp)
{
	unsigned char i;
	unsigned char Data[8];
	for(i=0;i<8;i++)
	{
		Data[i] = *temp++;
	}
	Set_Axis((AXIS_NUM)(Data[1]<<8));//Data[1]ѡ��
//Data 0������ 1�� 2����ģʽ 3�������� 4~7������
	if(Data[2]==0x08)
	{		
		Data_Write(WP, (Data[4]<<24) | (Data[5]<<16) | (Data[6]<<8) | (Data[7]));
		CMD_Write((CMD_NUM)Data[3]);
	}
	if(Data[2]<8)
	{
		wr_map[R3] &= 0xfff8;
		wr_map[R3] |= Data[2];
		Reg_Write(R3,wr_map[R3]);
		Data_Write(WP, (Data[4]<<24) | (Data[5]<<16) | (Data[6]<<8) | (Data[7]));
		CMD_Write((CMD_NUM)Data[3]);//�̶������������		
	}
}

/**********************************************************************************************
* ����������ָ��
		Data0  ������	
		Data1  ѡ�� 01-X�� 02-Y�� 04-Z��  08-U��
		Data2  ��ȡPCD(0x22)��MCD(0x23)��PCD��������������MCD������������
* ����ֵ����
* ˵��������ָ����ָ��������������
**********************************************************************************************/
void Axis_Continuous_Drive(unsigned char *temp)
{
	unsigned char i;
	unsigned char Data[3];
	for(i=0;i<3;i++)
	{
		Data[i] = *temp++;
	}
	Set_Axis((AXIS_NUM)(Data[1]<<8)); //Data[1]ѡ��
	CMD_Write((CMD_NUM)Data[2]);     //д������������
}
/**********************************************************************************************
* ����������ָ��
		Data0   ������	
		Data1   ѡ�� 01-X�� 02-Y�� 04-Z��  08-U��
		Data2~5 ŵΪ�����˶���Ϊ�����˶���������������������������Բ���
		Data6   00  external signals disabled
				01  continuous driving mode
				02  fixed pulse driving mode
				03  MPG mode
* ����ֵ����
* ˵����ָ�����ⲿ�ź�����
**********************************************************************************************/
void External_Signals_Drive(unsigned char *temp)
{
	unsigned char i;
	unsigned char Data[7];
	for(i=0;i<7;i++)
	{
		Data[i] = *temp++;
	}
	Set_Axis((AXIS_NUM)(Data[1]<<8)); //Data[1]ѡ��
	Data_Write(WEM,0x00005f00);//�ⲿ�ź��˲�
	Data_Write(WP, (Data[2]<<24) | (Data[3]<<16) | (Data[4]<<8) | (Data[5]));
	wr_map[R3] &= 0x0f87;
	wr_map[R3] |= (Data[6]<<3);
	Reg_Write(R3,wr_map[R3]);	
}
//ʧ���ⲿ�ź�����
void Clear_External_Drive(void)
{
	wr_map[R3] &= 0x0f87;
	Reg_Write(R3,wr_map[R3]);
}	

/**********************************************************************************************
* ����: ����ָ��
		Data0  ������	
		Data1  ѡ��  01-X�� 02-Y�� 04-Z��  08-U��    
        Data2  ֹͣģʽ
				0x26 DECS����ֹͣ�
				0x27 SUDS����ֹͣ
* ����ֵ����
* ˵����ָֹͣ�������˶�
**********************************************************************************************/
void AXIS_Stop(unsigned char *temp)
{
	unsigned char i;
	unsigned char Data[3];
	for(i=0;i<3;i++)
	{
		Data[i] = *temp++;
	}
	Set_Axis((AXIS_NUM)(Data[1]<<8)); //Data[1]ѡ��
	CMD_Write((CMD_NUM)Data[2]);
}
/**********************************************************************************************
* ����: ����ָ��
		Data0  ������	
		Data1  ѡ�� 01-X�� 02-Y�� 04-Z��  08-U��     
        Data2  �ŷ�ģʽ  D0 nALARM 0:Low/1:Hi  
						 D1 nALARM 0:Disable/1:Enable 
						 D3 nINPOS 0:Low/1:Hi
						 D4 nINPOS 0:Disable/1:Enable 
* ����ֵ����
* ˵�����ŷ�ģʽ����
**********************************************************************************************/
void Servo_Mode_Config(unsigned char *temp)
{
	unsigned char i;
	unsigned char Data[3];
	for(i=0;i<3;i++)
	{
		Data[i] = *temp++;
	}
	Set_Axis((AXIS_NUM)(Data[1]<<8)); //Data[1]ѡ��
	wr_map[R2] &= 0x0fff;
	wr_map[R2] |= (Data[2]<<12);
	Reg_Write(R2, wr_map[R2]);
}
/**********************************************************************************************
* ����: ����ָ��
		Data0  ������	
		Data1  ѡ�� 01-X�� 02-Y�� 04-Z��  08-U��     
        Data2  1ʹ�������������λ   2ʹ�ܸ����������λ
		Data3  0���߼�λ�ü������Ƚ� 1��ʵ��λ�ü������Ƚ�
* ����ֵ����
* ˵��������λʹ��
**********************************************************************************************/
void Software_Limit(unsigned char *temp)
{	
	unsigned char i;
	unsigned char Data[8];
	for(i=0;i<8;i++)
	{
		Data[i] = *temp++;
	}
	Set_Axis((AXIS_NUM)(Data[1]<<8)); //Data[1]ѡ��
	LP_Set((AXIS_NUM)(Data[1]<<8),0);
	wr_map[R2] &= 0xffdc;
	wr_map[R2] |= Data[2];
	wr_map[R2] |= (Data[3]<<5);
	Reg_Write(R2, wr_map[R2]);
	if(Data[2]==0x01)
	{
		Data_Write(WCP, (Data[4]<<24) | (Data[5]<<16) | (Data[6]<<8) | (Data[7]));
	}
	if(Data[2]==0x02)
	{
		Data_Write(WCM, (Data[4]<<24) | (Data[5]<<16) | (Data[6]<<8) | (Data[7]));
	}
}

/**********************************************************************************************
* ������ ��
* ����ֵ����
* ˵�����ⲿ�ź����ʵ��λ��  ��ԭ������ Z-phase search  IN2
**********************************************************************************************/
//��չģʽ������  �ⲿ�ź����ʵ��λ��  ��ԭ������ Z-phase search  IN2
void EXT_CLR_RLP(void)
{
	wr_map[R1] &= 0xffcf;
	wr_map[R1] |= 0x0030;// ʹ��IN2 WR1/D5    active level WR1/D4 IN2ΪHi 
	Reg_Write(R1,wr_map[R1]);
	
	wr_map[R6] &= 0xfffe;
	wr_map[R6] |= 0x0001;
	Reg_Write(R6,wr_map[R6]);
	Data_Write(WEM,0x5f00);
}


/**********************************************************************************************
* ����������ֵ
* ����ֵ��
* ˵��������岹�ĸ�����ţ���ת����
**********************************************************************************************/
unsigned char ITP_Axis(unsigned char Data)
{
	unsigned char temp;
	switch(Data)
	{
		case 0x01:
			temp = 0x00;
		break;
		case 0x02:
			temp = 0x01;
		break;
		case 0x04:
			temp = 0x02;
		break;
		case 0x08:
			temp = 0x03;
		break;
		default:
		break;
	}
	
	return temp;
}

/**********************************************************************************************
* ����������ָ��
		Data0    ������
		Data1    ��1 01-X�� 02-Y�� 04-Z��  08-U��
		Data2    ��2 01-X�� 02-Y�� 04-Z��  08-U��
		Data3~6  ��1������
		Data7~10 ��2������
		Data11   3B-�岹������Ч 3C-�岹������Ч
* ����ֵ����
* ˵��������ֱ�߲岹
**********************************************************************************************/
void ITP_2AxisLine(unsigned char *temp)
{
	unsigned char i;
	unsigned char Data[12];
	for(i=0;i<12;i++)
	{
		Data[i] = *temp++;
	}
	//Data 0������  1��1  2��2  3~6��1������ 7~10��2������ 11�岹������Ч��
	wr_map[R5] &= 0xdb00;
	wr_map[R5] |= ITP_Axis(Data[1])
					|(ITP_Axis(Data[2]) << 2);					
	Reg_Write(R5, wr_map[R5]);
	
	Set_Axis((AXIS_NUM)(Data[1]<<8)); //Data[1]ѡ��1
	Data_Write(WP, (Data[3]<<24) | (Data[4]<<16) | (Data[5]<<8) | (Data[6]));
	
	Set_Axis((AXIS_NUM)(Data[2]<<8)); //Data[2]ѡ��2
	Data_Write(WP, (Data[7]<<24) | (Data[8]<<16) | (Data[9]<<8) | (Data[10]));
	
	CMD_Write((CMD_NUM)Data[11]);
	CMD_Write(I2L);
}



/**********************************************************************************************
* ����������ָ��
		Data0     ������
		Data1     ��1 01-X�� 02-Y�� 04-Z��  08-U��
		Data2     ��2 01-X�� 02-Y�� 04-Z��  08-U��
		Data3     ��3 01-X�� 02-Y�� 04-Z��  08-U��
		Data4~7   ��1������
		Data8~11  ��2������
		Data12~15 ��2������
		Data16   3B-�岹������Ч 3C-�岹������Ч
* ����ֵ����
* ˵��������ֱ�߲岹
**********************************************************************************************/
void ITP_3AxisLine(unsigned char *temp)
{
	unsigned char i;
	unsigned char Data[17];
	for(i=0;i<17;i++)
	{
		Data[i] = *temp++;
	}
	wr_map[R5] &= 0xdb00;
	wr_map[R5] |= ITP_Axis(Data[1])
					|(ITP_Axis(Data[2]) << 2)
					|(ITP_Axis(Data[3]) << 4);								
	Reg_Write(R5, wr_map[R5]);
	
	Set_Axis((AXIS_NUM)(Data[1]<<8)); //Data[1]ѡ��1
	Data_Write(WP, (Data[4]<<24) | (Data[5]<<16) | (Data[6]<<8) | (Data[7]));
	
	Set_Axis((AXIS_NUM)(Data[2]<<8)); //Data[2]ѡ��2
	Data_Write(WP, (Data[8]<<24) | (Data[9]<<16) | (Data[10]<<8) | (Data[11]));
	
	Set_Axis((AXIS_NUM)(Data[3]<<8)); //Data[3]ѡ��3
	Data_Write(WP, (Data[12]<<24) | (Data[13]<<16) | (Data[14]<<8) | (Data[15]));
	
	CMD_Write((CMD_NUM)Data[16]);
	CMD_Write(I3L);
}

/**********************************************************************************************
* ����������ָ��
		Data0      ������
		Data1      ��1 01-X�� 02-Y�� 04-Z��  08-U��
		Data2      ��2 01-X�� 02-Y�� 04-Z��  08-U��
		Data3~6    ��1Բ��
		Data7~10   ��2Բ��
		Data11~14  ��1������
		Data15~18  ��2������
		Data19     3B-�岹������Ч 3C-�岹������Ч
* ����ֵ����
* ˵��������˳ʱ�뷽��Բ���岹
**********************************************************************************************/
void ITP_CW(unsigned char *temp)
{
	unsigned char i;
	unsigned char Data[20];
	for(i=0;i<20;i++)
	{
		Data[i] = *temp++;
	}
	wr_map[R5] &= 0xdb00;
	wr_map[R5] |= ITP_Axis(Data[1])
					|(ITP_Axis(Data[2]) << 2);
	Reg_Write(R5, wr_map[R5]);
	
	Set_Axis((AXIS_NUM)(Data[1]<<8)); //Data[1]ѡ��1
	Data_Write(WC, (Data[3]<<24) | (Data[4]<<16) | (Data[5]<<8) | (Data[6]));
	Data_Write(WP, (Data[11]<<24) | (Data[12]<<16) | (Data[13]<<8) | (Data[14]));
	
	Set_Axis((AXIS_NUM)(Data[1]<<8)); //Data[1]ѡ��2
	Data_Write(WC, (Data[7]<<24) | (Data[8]<<16) | (Data[9]<<8) | (Data[10]));
	Data_Write(WP, (Data[15]<<24) | (Data[16]<<16) | (Data[17]<<8) | (Data[18]));
	
	CMD_Write((CMD_NUM)Data[19]);
	CMD_Write(ICW);
}


/**********************************************************************************************
* ����������ָ��
		Data0      ������
		Data1      ��1 01-X�� 02-Y�� 04-Z��  08-U��
		Data2      ��2 01-X�� 02-Y�� 04-Z��  08-U��
		Data3~6    ��1Բ��
		Data7~10   ��2Բ��
		Data11~14  ��2������
		Data15~18  ��2������
		Data19     3B-�岹������Ч 3C-�岹������Ч
* ����ֵ����
* ˵����������ʱ�뷽��Բ���岹
**********************************************************************************************/
void ITP_CCW(unsigned char *temp)
{
	unsigned char i;
	unsigned char Data[20];
	for(i=0;i<20;i++)
	{
		Data[i] = *temp++;
	}
	wr_map[R5] &= 0xdb00;
	wr_map[R5] |= ITP_Axis(Data[1])
					|(ITP_Axis(Data[2]) << 2);
	Reg_Write(R5, wr_map[R5]);
	
	Set_Axis((AXIS_NUM)(Data[1]<<8)); //Data[1]ѡ��1
	Data_Write(WC, (Data[3]<<24) | (Data[4]<<16) | (Data[5]<<8) | (Data[6]));
	Data_Write(WP, (Data[11]<<24) | (Data[12]<<16) | (Data[13]<<8) | (Data[14]));
	
	Set_Axis((AXIS_NUM)(Data[2]<<8)); //Data[2]ѡ��2
	Data_Write(WC, (Data[7]<<24) | (Data[8]<<16) | (Data[9]<<8) | (Data[10]));
	Data_Write(WP, (Data[15]<<24) | (Data[16]<<16) | (Data[17]<<8) | (Data[18]));
	
	CMD_Write((CMD_NUM)Data[19]);
	CMD_Write(ICCW);
}
#ifdef IB
/**********************************************************************************************
* ������axisn��ȡö��AXIS_NUM�ж����ֵ
		pulsenΪ��axisn��Ӧ�����������
* ����ֵ����
* ˵��������λģʽ�岹
**********************************************************************************************/
void Itp_2AxisBit(AXIS_NUM axis1, AXIS_NUM axis2, int pulse1, int pulse2)
{
	wr_map[R5] &= 0xdb00;
	wr_map[R5] |= 	Itp_Axis(axis1)
					| (Itp_Axis(axis2) << 2);
	Reg_Write(R5, wr_map[R5]);
	
	Set_Axis(axis1);
	CMD_Write(BPRWEN);
	Data_Write(BPDS, pulse1);
	CMD_Write(BPRWDIS);
	
	Set_Axis(axis2);
	CMD_Write(BPRWEN);
	Data_Write(BPDS, pulse1);
	CMD_Write(BPRWDIS);
	
	CMD_Write(I2B);
}
/**********************************************************************************************
* ������
* ����ֵ����
* ˵����2��λģʽ�岹
**********************************************************************************************/
void ITP_2BP(unsigned short *temp)
{
	unsigned char i,RData;
	unsigned char Data[23];
	for(i=0;i<23;i++)
	{
		Data[i] = *temp++;
	}
	wr_map[R5] &= 0xdb00;
	wr_map[R5] |= ITP_Axis(Data[1])
					|(ITP_Axis(Data[2]) << 2);
	Reg_Write(R5, wr_map[R5]);
	
	Set_Axis((AXIS_NUM)(Data[1]<<8)); //Data[1]ѡ��1
	Set_Axis((AXIS_NUM)(Data[2]<<8)); //Data[2]ѡ��2
	CMD_Write(BPDC);//���BP�Ĵ�������
	CMD_Write(BPRWEN);//ʹ��дBP�Ĵ���
	
						//��������0~15
	Reg_Write(R2,Data[3]);//axis1 +direction
	Reg_Write(R3,Data[4]);//axis1 -direction
	Reg_Write(R4,Data[5]);//axis2 +direction
	Reg_Write(R5,Data[6]);//axis2 -direction
	CMD_Write(BPDS);   //BP����ѹջ SC=1
	
						//��������16~31
	Reg_Write(R2,Data[7]);//axis1 +direction
	Reg_Write(R3,Data[8]);//axis1 -direction
	Reg_Write(R4,Data[9]);//axis2 +direction
	Reg_Write(R5,Data[10]);//axis2 -direction
	CMD_Write(BPDS);   //BP����ѹջ SC=2
	
						//��������32~47
	Reg_Write(R2,Data[11]);//axis1 +direction
	Reg_Write(R3,Data[12]);//axis1 -direction
	Reg_Write(R4,Data[13]);//axis2 +direction
	Reg_Write(R5,Data[14]);//axis3 -direction
	CMD_Write(BPDS);   //BP����ѹջ SC=3
	
	CMD_Write((CMD_NUM)Data[15]);
	CMD_Write(I2B);	
	
	RData = ((Reg_Read(R0)>>13)&0x0003);
	if(RData==0x02||0x01)
	{						
							//��������48~62
		Reg_Write(R2,Data[16]);//axis1 +direction
		Reg_Write(R3,Data[17]);//axis1 -direction
		Reg_Write(R4,Data[18]);//axis2 +direction
		Reg_Write(R5,Data[19]);//axis3 -direction
		CMD_Write(BPDS);//BP����ѹջ SC=3
	}
	RData = ((Reg_Read(R0)>>13)&0x0003);
	if(RData==0x02||0x01)
	{						
							//��������48~62
		Reg_Write(R2,Data[20]);//axis1 +direction
		Reg_Write(R3,Data[21]);//axis1 -direction
		Reg_Write(R4,Data[22]);//axis2 +direction
		Reg_Write(R5,Data[23]);//axis3 -direction
		CMD_Write(BPDS);//BP����ѹջ SC=3
	}
	
}

/**********************************************************************************************
* ������axisn��ȡö��AXIS_NUM�ж����ֵ
		pulsenΪ��axisn��Ӧ�����������
* ����ֵ����
* ˵��������λģʽ�岹
**********************************************************************************************/
void Itp_3AxisBit(AXIS_NUM axis1, AXIS_NUM axis2, AXIS_NUM axis3, int pulse1, int pulse2, int pulse3)
{
	wr_map[R5] &= 0xdb00;
	wr_map[R5] |= 	Itp_Axis(axis1)
					| (Itp_Axis(axis2) << 2)
					| (Itp_Axis(axis2) << 4);
	Reg_Write(R5, wr_map[R5]);
	
	Set_Axis(axis1);
	CMD_Write(BPRWEN);
	Data_Write(BPDS, pulse1);
	CMD_Write(BPRWDIS);
	
	Set_Axis(axis2);
	CMD_Write(BPRWEN);
	Data_Write(BPDS, pulse1);
	CMD_Write(BPRWDIS);
	
	Set_Axis(axis3);
	CMD_Write(BPRWEN);
	Data_Write(BPDS, pulse3);
	CMD_Write(BPRWDIS);
	
	CMD_Write(I3B);
}

#endif
/**********************************************************************************************
* ������
* ����ֵ����
* ˵��������Command�岹ģʽ  
**********************************************************************************************/
void ITP_Step_Command(void)
{	
	Reg_Write(R5,0x1104);
	CMD_Write(ISS);		
	//�岹�����Ͳ岹����Set the initial and drive speeds,finish point, center point.......
		
}
/**********************************************************************************************
* ����������ָ��
		Data0      ������
		Data1      0 �ⲿ�ź�ִ�е����岹 
				   1 ����(0x3A)ִ�е����岹	
* ����ֵ����
* ˵�����ⲿ�źŻ�������ִ�е����岹
**********************************************************************************************/
void ITP_Single_Step(unsigned char *temp)
{
	unsigned char i;
	unsigned char Data[2];
	for(i=0;i<2;i++)
	{
		Data[i] = *temp++;
	}
	if(Data[1]==0)
	{
		Reg_Write(R5,0x0804);
		Data_Write(WEM,0x5f00);
	}
	if(Data[1]==1)
	{
		Reg_Write(R5,0x1104);
	}
	CMD_Write(ISS);
}

/**********************************************************************************************
* ������
* ����ֵ����
* ˵���������ⲿ�źŲ岹ģʽ  
**********************************************************************************************/
void ITP_Step_External(void)
{
	
	Reg_Write(R5,0x0904);
	Data_Write(WEM,0x5f00);
	CMD_Write(ISS);		
	//���ò岹���� ����  �˲��� �ȴ��ⲿ�ź�Ϊ�� 	
}
/**********************************************************************************************
* ������state��ȡDECV    3B  �岹������Ч enable the automatic deceleration or manual deceleration function
	             DECINV  3C	 �岹������Ч disable the automatic deceleration or manual deceleration function
* ����ֵ����
* ˵�����岹ֹͣģʽ//In continuous interpolation, same as in circular and bit pattern interpolations, only manual deceleration in the trapezoidal driving
					//is available; The automatic deceleration in S-curve driving is not available.
					//In circular interpolation and bit pattern interpolation, only manual deceleration in trapezoidal driving is available; the automatic
					//deceleration in S-curve driving is not available.
**********************************************************************************************/
void ITP_Dece_State(CMD_NUM state)
{
	CMD_Write(state);
}
/**********************************************************************************************
* ������state��ȡDECS����SUDS��DECS����ֹͣ��SUDS����ֹͣ
* ����ֵ����
* ˵�����岹ֹͣģʽ
**********************************************************************************************/
void ITP_Dece_Mode(CMD_NUM state)
{
	CMD_Write(state);
}

/**********************************************************************************************
* ��������
* ����ֵ���岹״̬
		  0 �岹��ɽ���
		  1 �岹�ڽ���
* ˵������ѯ�岹״̬
**********************************************************************************************/
unsigned char ITP_Stauts(void)
{
	unsigned short temp;
	temp = ((Reg_Read(R0)>>8)&0x0001);
	return (unsigned char)temp;
}
/**********************************************************************************************
* ��������
* ����ֵ�������岹״̬
		  0 ���ܽ��������岹
		  1 �ܽ�����һ���岹
* ˵������ѯ�Ƿ���Խ��������岹״̬ //�����岹 ��ǰ�岹�����Ѿ�ִ���� RR0/D9 Ϊ1 ���Խ��������岹(���״̬֮ǰ ��Ҫ�����󣨲岹����RRO/D4~D7�� û����ſ���ִ���¸��岹)�
					// first to write, and enable the interpolation data and command, then check if
					//D9 of RR0 is 1 or 0. And then, repeat writing commands and checking D9.
**********************************************************************************************/
unsigned char ITP_Continuous_Check(void) 
{
	unsigned short temp;
	temp = ((Reg_Read(R0)>>9)&0x0001);
	return (unsigned char)temp;
}

/**********************************************************************************************
* ˵����IO��
* ������IO�ڱ��
* ����ֵ��IO״̬
**********************************************************************************************/
char IO_ReadBit(unsigned int in)
{
	unsigned short result;
	
	if(in < 16)
	{
		result = Reg_Read(R4);
		result >>= in;
		result &= 1;
		return (char)(result == 1);
	}
	else
	{
		in -= 16;
		result = Reg_Read(R5);
		result >>= in;
		result &= 1;
		return (char)(result == 1);
	}
}

/**********************************************************************************************
* ˵����IOд
* ������IO�ڱ�ţ�дIO��״̬
* ����ֵ����
**********************************************************************************************/
void IO_WriteBit(unsigned char out, unsigned short act)
{
	unsigned short mask = 1;
	
	switch(out)
	{
		//X,R4 D0-D3
		case 0:
		case 1:
		case 2:
		case 3:
			act <<= out;
			mask <<= out;
			wr_map[R4] &= (~mask);
			wr_map[R4] |= act;
			Reg_Write(R4, wr_map[R4]);
		break;
		//X,R3 D8-D11
		case 4:
		case 5:
		case 6:
		case 7:
			out += 4;
			act <<= out;
			mask <<= out;
			wr_map[R3] &= (~mask);
			wr_map[R3] |= act;
			Reg_Write(R3, wr_map[R3]);
		break;
		//Y,R4 D4-D7
		case 8:
		case 9:
		case 10:
		case 11:
			out -= 4;
			act <<= out;
			mask <<= out;
			wr_map[R4] &= (~mask);
			wr_map[R4] |= act;
			Reg_Write(R4, wr_map[R4]);
		break;
		//Y,R3 D8-D11
		case 12:
		case 13:
		case 14:
		case 15:
			out -= 4;
			act <<= out;
			mask <<= out;
			wr_map[R3] &= (~mask);
			wr_map[R3] |= act;
			Reg_Write(R3, wr_map[R3]);
		break;
		//Z,R4 D8-D11
		case 16:
		case 17:
		case 18:
		case 19:
			out -= 8;
			act <<= out;
			mask <<= out;
			wr_map[R4] &= (~mask);
			wr_map[R4] |= act;
			Reg_Write(R4, wr_map[R4]);
		break;
		//Z,R3 D8-D11
		case 20:
		case 21:
		case 22:
		case 23:
			out -= 12;
			act <<= out;
			mask <<= out;
			wr_map[R3] &= (~mask);
			wr_map[R3] |= act;
			Reg_Write(R3, wr_map[R3]);
		break;
		//U,R4 D12-D15
		case 24:
		case 25:
		case 26:
		case 27:
			out -= 12;
			act <<= out;
			mask <<= out;
			wr_map[R4] &= (~mask);
			wr_map[R4] |= act;
			Reg_Write(R4, wr_map[R4]);
		break;
		//U,R3 D8-D11
		case 28:
		case 29:
		case 30:
		case 31:
			out -= 20;
			mask <<= out;
			wr_map[R3] &= (~mask);
			wr_map[R3] |= act;
			Reg_Write(R3, wr_map[R3]);
		break;
	}
}
/**********************************************************************************************
* ����������ָ��
		Data0         ������	
		Data1         ѡ�� 01-X�� 02-Y�� 04-Z��  08-U��
		Data2~5       �����ź��˲�ʹ�� ��ӦΪ1����ʹ�ܶ����ź��˲�����
* ����ֵ����
* ˵���
**********************************************************************************************/
void Iput_Filters_Config(void)
{
	
	Data_Write(WEM,0x5f00);
}

/**********************************************************************************************
* ����������ָ��
		Data0       ������	
		Data1       ѡ��
		Data2       �����źŵ�ƽ
		Data3~6     �ⲿ��չģʽ���ź��˲��������ã�WR6�˲� WR7��չ��
		Data7~8     ԭ�������ٶ�
		Data9~12    ƫ��������
* ����ֵ����
* ˵���������nIN0�����nIN2��Z���ź�IN2��Ϊ��ԭ���ź�,nIN0��nIN1��Ϊ��ԭ���ź���λ�ź���Ϊ��ԭ���źţ�
**********************************************************************************************/
void Auto_Home_Search(unsigned char *temp)
{
	unsigned char Data[13];
	int i;	
	for(i=0;i<13;i++)
	{
		Data[i]=*temp++;
	}	
	Set_Axis((AXIS_NUM)(Data[1]<<8));//ѡ��
	wr_map[R1] &= 0xff00;
	wr_map[R1] |= Data[2];
	Reg_Write(R1, wr_map[R1]);  //�����ź��߼�����D6,4,2,0 INm-L �ߵ͵�ƽ   D7,5,3,1 ����ֹͣ�����ź� INm 0: disable, 1: enable  
	//Data[0]..... 0����1�ֽ�   1��1�ֽ�   2�����ź��߼�1�ֽ�  3~6��չģʽ����4�ֽ�  7ԭ�������ٶ�2�ֽ�  9~12���ƫ�������� 
	//0x00,0x01,0x00,0x49,0x5f,0x5f,0x00,0x00,0x50,0x00,0x00,0x0f,0xff
	Data_Write(WEM,(Data[3]<<24) | (Data[4]<<16) | (Data[5]<<8) | Data[6]);
								//��չģʽ����
								//WR6�����ź��˲���ģʽ��   5f00
								//D15~D13  010  �˲�����ʱ��512us
								//D8        1   nLMTM,nIN0,nIN1�źţ� ENABLE(ʹ���˲���)
								//D9        0   nIN2�źţD      DISABLE(ʧ���˲���)�
	
								//WR7�Զ�����ģʽ���ã�a near home, home, or a Z-phase signal-0x495f ��IN0 and IN1-0x014f,a limit signal-0x054f
								//D15~D13  010 ������������������100us
								//D12       0  ��������������������߼���ƽ����
 								//D11       1  �����������������      ENABLE
								//D10		0  ʹ����λ�ź���Ϊ�����źţ�DISABLE
								//D9        0  Z��AND�����źţ�          DISABLE
								//D8        1  �߼�/ʵ��λ�ü�������     ENABLE  
	                            //D7        0  Step4��������           +������
								//D6        0  Step4��                   ENABLE
								//D5        0  Step3��������           -������
								//D4        1  Step3��                   ENABLE
								//D3        1  Step2��������           -������
								//D2        1  Step2��                   ENABLE
								//D1        1  Step1��������           -������
								//D0        1  Step1��                   ENABLE
								
								
	Data_Write(WHV,(Data[7]<<8) | (Data[8]));//Step 2ԭ�������ٶ�500
	Data_Write(WP, (Data[9]<<24) | (Data[10]<<16) | (Data[11]<<8) | Data[12]);//ƫ������
	CMD_Write(AHOME);//�����Զ���ԭ������
}
//�����ԭ����λ
void Clear_LimtMod(void)
{
	wr_map[R1] &= 0xff33;
	Reg_Write(R1, wr_map[R1]);    //ʧ����λ�ź�
	//Clear_Axis();
}
/**********************************************************************************************
* ����������ָ��
		Data0       ������	
		Data1       ��1���� 
		Data2       ��2
		Data3       ��3
		Data4       ��4
		Data5~8     ��1����Ƚ�������ֵ
		Data9~12    ��1���������
		Data13~16   ��2���������
		Data17~20   ��3���������
		Data21~24   ��4���������
		Data25~28   ��1ͬ��ģʽ����
		Data29~32   ��2ͬ��ģʽ����
		Data33~36   ��3ͬ��ģʽ����
		Data37~40   ��4ͬ��ģʽ����
		Data41      ��1��ʼ����״̬
		Data42		��2��ʼ����״̬
		Data43		��3��ʼ����״̬
		Data44		��4��ʼ����״̬
* ����ֵ����
* ˵����
**********************************************************************************************/
void SYN_Action(unsigned char *temp)
{
	unsigned char Data[45];
	unsigned char i;
//	unsigned short RR3;
	for(i=0;i<45;i++)
	{
		Data[i]=*temp++;
	}
	wr_map[R2] &= 0xffdf;
	Reg_Write(R2, wr_map[R2]);//���߼��������Ƚ�WR2-D5=0
	Set_Axis((AXIS_NUM)(Data[1]<<8));//ѡ��1
	Set_Axis((AXIS_NUM)(Data[2]<<8));//ѡ��2
	Set_Axis((AXIS_NUM)(Data[3]<<8));//ѡ��3
	Set_Axis((AXIS_NUM)(Data[4]<<8));//ѡ��4
//0x00,/0x01,0x02,0x04,0x08,/0x00,0x00,0xff,0xff,/0x00,0x02,0xff,0xff,/0x00,0x02,0xff,0xff,/0x00,0x02,0xff,0xff,/0x00,0x02,0xff,0xff,/0x00,0x10,0xe0,0x01,/0x00,0x01,0x00,0x00,/0x00,0x04,0x00,0x00/,0x00,0x08,0x00,0x00/0x20,0x25,0x25,0x25	
	CompP_Set((AXIS_NUM)(Data[1]<<8),(Data[5]<<24) | (Data[6]<<16) | (Data[7]<<8) | Data[8]);  //axis1��Ƚ���������pulse
	LP_Set((AXIS_NUM)(Data[1]<<8),0);//���axis1�߼�λ�üĴ���
	LP_Set((AXIS_NUM)(Data[2]<<8),0);//���axis2�߼�λ�üĴ���
	LP_Set((AXIS_NUM)(Data[3]<<8),0);//���axis3�߼�λ�üĴ���
	LP_Set((AXIS_NUM)(Data[4]<<8),0);//���axis4�߼�λ�üĴ���
	
	Pulse_Out((AXIS_NUM)(Data[1]<<8),(Data[9]<<24) | (Data[10]<<16) | (Data[11]<<8) | Data[12]);  //axis1���������pulse	
	Pulse_Out((AXIS_NUM)(Data[2]<<8),(Data[13]<<24) | (Data[14]<<16) | (Data[15]<<8) | Data[16]); //axis2���������pulse
	Pulse_Out((AXIS_NUM)(Data[3]<<8),(Data[17]<<24) | (Data[18]<<16) | (Data[19]<<8) | Data[20]); //axis3���������pulse	
	Pulse_Out((AXIS_NUM)(Data[4]<<8),(Data[21]<<24) | (Data[22]<<16) | (Data[23]<<8) | Data[24]); //axis4���������pulse	
	
	
	SYN_Mode((AXIS_NUM)(Data[1]<<8),(Data[25]<<24) | (Data[26]<<16) | (Data[27]<<8) | Data[28]);        //axis1ͬ��ģʽ����
	SYN_Mode((AXIS_NUM)(Data[2]<<8),(Data[29]<<24) | (Data[30]<<16) | (Data[31]<<8) | Data[32]);        //axis2ͬ��ģʽ����
	SYN_Mode((AXIS_NUM)(Data[3]<<8),(Data[33]<<24) | (Data[34]<<16) | (Data[35]<<8) | Data[36]);        //axis3ͬ��ģʽ����
	SYN_Mode((AXIS_NUM)(Data[4]<<8),(Data[37]<<24) | (Data[38]<<16) | (Data[39]<<8) | Data[40]);        //axis4ͬ��ģʽ����
	
	Reg_Write(R0,(AXIS_NUM)(Data[1]<<8)|Data[41]);
	Reg_Write(R0,(AXIS_NUM)(Data[2]<<8)|Data[42]);
	Reg_Write(R0,(AXIS_NUM)(Data[3]<<8)|Data[43]);
	Reg_Write(R0,(AXIS_NUM)(Data[4]<<8)|Data[44]);	
	
	//�����ź�nIN3 fell  nIN3�жϷ��� ������axis1��axis2��axis3λ������
//��ȡXRR3�Ĵ��� ���D9=1.�жϷ��� XIN3 fell�	
//	Reg_Write(R0,0x0722);
//	RR3 = Reg_Read(R3);
//	RR3 &= 0x0200;
//	if(RR3==0x0200)
//	{
//		Read_RSB(AXIS_X);
//		Read_RSB(AXIS_Y);
//		Read_RSB(AXIS_Z);
//	}
}
//���ͬ��ģʽ
void Clear_Synch(void)
{
	Set_Axis(AXIS_ALL);
	Data_Write(WSM,0);
	AXIS_Switch();
}

