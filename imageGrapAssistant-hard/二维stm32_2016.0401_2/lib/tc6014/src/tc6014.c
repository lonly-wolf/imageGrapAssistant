/**
 *********************************************************************
 **标题：tc6014.c
 **作者：fyf
 **日期：2014.10.28
 **版本：v1.0
 **说明：tc6014 driver
 *********************************************************************
**/

#include "tc6014.h"

//寄存器映射
unsigned short wr_map[8] = {0x0000,0x0000,0x0000,0x00000,0x00000,0x0000,0x0000,0x0000};
unsigned short rr_map[8] = {0x0000,0x0000,0x0000,0x00000,0x00000,0x0000,0x0000,0x0000};

unsigned short Itp_Axis(AXIS_NUM axis);

/**********************************************************************************************
* 参数：reg可取枚举REG中定义的值，用于选择指定的寄存器
		dat为要写入reg的数据
* 返回值：无
* 说明：该函数用于将指定的数据写入指定的寄存器
**********************************************************************************************/
void Reg_Write(REG reg, short dat)
{
	*(unsigned short*)(BASE_ADDR + OFFSET * reg) = dat;
}

/**********************************************************************************************
* 参数：cmd可取枚举CMD_NUM中定义的命令代码，用于执行对应的命令
* 返回值：无
* 说明：写指定的命令到命令寄存器，以实现对应的功能
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
* 参数：cmd可取枚举CMD_NUM中定义的命令代码，用于执行对应的命令
		dat为命令对应的数据
* 返回值：无
* 说明：执行带有数据的命令
**********************************************************************************************/
void Data_Write(CMD_NUM cmd, int dat)
{
	switch(cmd)
	{
		case WR:		//00h  	范围设置
		case WP: 		//06  	输出脉冲数设置或者终点设置
		case WDP:		//07  	速度减速点设置
		case WC: 		//08  	圆心设置
		case WLP:		//09  	逻辑位置计数器初值设置
		case WEP:		//0A  	编码器计数器初值设置
		case WCP:		//0B  	正向比较寄存器数值设置
		case WCM:		//0C  	负向比较寄存器设置
		case WEM:		//60  	扩展模式设置（位数据）
		case WSM:		//64  	同步模式设置（位数据)
			Reg_Write(R6, dat);
			Reg_Write(R7, (dat >> 16));
			CMD_Write(cmd);
			break;
		case WK:		//01 	加速度增加率设置
		case WA: 		//02  	加速度设置
		case WD: 		//03  	减速度设置
		case WSV:  		//04  	启动速度设置
		case WV: 		//05  	驱动速度设置
		case WAO:		//0D  	加速度寄存器偏移设置
		case WL: 		//0E  	减速度减少率设置
		case WHV:		//61  	原点搜索速度
			Reg_Write(R6, dat);
			CMD_Write(cmd);
			break;
		default:
			break;
	}
}

/**********************************************************************************************
* 参数：reg为REG类型枚举
* 返回值：读取到的数据
* 说明：读取reg寄存器对应的数据
**********************************************************************************************/
unsigned short Reg_Read(REG reg)
{
	return *(unsigned short*)(BASE_ADDR + OFFSET * reg);
}

/**********************************************************************************************
* 参数：cmd为CMD_NUM类型枚举
* 返回值：cmd命令对应的数值
* 说明：读取cmd命令对应的short（16位）型数据
**********************************************************************************************/
int Data_Read(CMD_NUM cmd)
{
	int temp;
	
	CMD_Write(cmd);
	switch(cmd)
	{
		case RLP: 		        //10h  	读取当前逻辑位置计数器数值
		case REP:  				//11  	读取编码器数值
		case RSB:  				//14  	读同步缓冲寄存器
			temp = Reg_Read(R7);//      读取R7中的数据 为高16位
			temp <<= 16;
			temp |= Reg_Read(R6);//     读取R6中的数据 为低16位
		break;
		case RV:  				//12  	读取当前驱动速度
		case RA:  				//13  	读加减速度数值
			temp = Reg_Read(R6);//      读取R6中的数据 为低16位 高16位为0
			break;
		default:
			break;
	}
	
	return temp;
}

/**********************************************************************************************
* 参数：无
* 返回值：无
* 说明：软件复位
**********************************************************************************************/
void Software_Reset(void)
{
	wr_map[R0] &= 0x0000;//0x0f7f
	wr_map[R0] |= 0x8000;
	Reg_Write(R0, wr_map[0]);
}
/**********************************************************************************************
* 参数：axis为AXIS_NUM类型枚举
* 返回值：无
* 说明：指定轴
**********************************************************************************************/
void Set_Axis(AXIS_NUM axis)
{
	wr_map[R0] &= 0x007f;
	wr_map[R0] |= axis;
	Reg_Write(R0, wr_map[R0]);
}

//范围设置
void Range_Set(AXIS_NUM axis,int data)
{
	Data_Write(WR,data);
}
//加速度增加率设置
void INC_Rate(AXIS_NUM axis,int data)
{
	Data_Write(WK,data);
}
//减速度减少率设置
void DEC_Rate(AXIS_NUM axis,int data)
{
	Data_Write(WL,data);
}
//加速度设置
void INC_Set(AXIS_NUM axis,int data)
{
	Data_Write(WA,data);
}
//减速度设置
void DEC_Set(AXIS_NUM axis,int data)
{
	Data_Write(WD,data);
}
//启动速度设置
void Start_Speed(AXIS_NUM axis,int data)
{
	Data_Write(WSV,data);
}
//驱动速度设置
void Drive_Speed(AXIS_NUM axis,int data)
{
	Data_Write(WV,data);
}
//输出脉冲数设置或者终点设置
void Pulse_Out(AXIS_NUM axis,int data)
{
	Set_Axis(axis);
	Data_Write(WP,data);
}
//减速点设置
void DECP_Set(AXIS_NUM axis,int data)
{
	Set_Axis(axis);
	Data_Write(WDP,data);
}
//圆心设置
void Center_Set(AXIS_NUM axis,int data)
{
	Set_Axis(axis);
	Data_Write(WC,data);
}
//逻辑位置计数器初值设置
void LP_Set(AXIS_NUM axis,int data)
{
	Set_Axis(axis);
	Data_Write(WLP,data);
}
//编码器计数器初值设置
void EP_Set(AXIS_NUM axis,int data)
{
	Set_Axis(axis);
	Data_Write(WEP,data);
}
//正向比较寄存器数值设置
void CompP_Set(AXIS_NUM axis,int data)
{
	Set_Axis(axis);
	Data_Write(WCP,data);
}
//负向比较寄存器设置
void CompM_Set(AXIS_NUM axis,int data)
{
	Set_Axis(axis);
	Data_Write(WCM,data);
}
//加速偏移设置
void ACC_Offset(AXIS_NUM axis,int data)
{
	Set_Axis(axis);
	Data_Write(WAO,data);
}

//扩展模式设置（位数据）
void EXP_Mode(AXIS_NUM axis,int data)
{
	Set_Axis(axis);
	Data_Write(WEM,data);
}
//原点搜索速度
void Home_SearchSpeed(AXIS_NUM axis,int data)
{
	Set_Axis(axis);
	Data_Write(WHV,data);
}
//同步模式设置（位数据�
void SYN_Mode(AXIS_NUM axis,int data)
{
	Set_Axis(axis);
	Data_Write(WSM,data);
}


//Data_Read();

//读实际位置 读取编码器数值
int Read_REP(AXIS_NUM axis)
{
	int Data;
	Set_Axis(axis);
	Data = Data_Read(REP);
	return Data;
}
//读逻辑位置 读取当前逻辑位置计数器数值
int Read_RLP(AXIS_NUM axis)
{
	int Data;
	Set_Axis(axis);
	Data = Data_Read(RLP);
	return Data;
}
//读取当前驱动速度
int Read_RV(AXIS_NUM axis)
{
	int Data;
	Set_Axis(axis);
	Data = Data_Read(RV);
	return Data;
}
//读加减速度数值
int Read_RA(AXIS_NUM axis)
{
	int Data;
	Set_Axis(axis);
	Data = Data_Read(RA);
	return Data;
}
//读同步缓冲寄存器
int Read_RSB(AXIS_NUM axis)
{
	int Data;
	Set_Axis(axis);
	Data = Data_Read(RSB);
	return Data;
}


// 等待轴驱动停止
void Wait_Stop(AXIS_NUM axis)
{
	while(Reg_Read(R0) & axis);
}

// Next data setting of waiting for continuous interpolation
//等待连续插补下一个数据的设置 RR0/D9
void Next_ITP_Wait(void)
{
	while((Reg_Read(R0) & 0x0200) == 0x0);
}

// Next data setting of waiting for BP interpolation
//等待位模式插补下一个数据设置 RR0/D13-D14  SC
void BP_ITP_Wait(void)
{
	while((Reg_Read(R0) & 0x6000) == 0x6000);
}
	

/**********************************************************************************************
* 参数：无
* 返回值：停止的轴代号
* 说明：返回停止轴代号 
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
* 参数：无
* 返回值：无
* 说明：返回出错轴代号 
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
* 参数：it:为IT_NUM类型枚举 STATE:使能/失能 WR1--D8~D15
* 返回值：无
* 说明：轴中断配置
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
* 参数：失能/使能 WR5--D14
* 返回值：无
* 说明：连续插补中断配置
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
* 参数：位模式插补中断 失能/使能 WR5--D15
* 返回值：无
* 说明：位模式插补中断配置
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
* 参数：回原点中断 使能ENB   使能DIS
* 返回值：
* 说明：回原点中断使能/失能
**********************************************************************************************/
//回原点中断 在外部模式中60h设置   WR6/D5(HMINT)   读中断RR3/D8(HMEND)  
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
* 参数：同步运动中断 使能ENB   使能DIS
* 返回值：
* 说明：同步运动中断使能/失能
**********************************************************************************************/
//同步运动中断 同步模式配置64h   WR7/D15 (INT)    读中断RR3/D9(SYNC)
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
* 参数：无
* 返回值：1中断发生 0中断没发生
* 说明：获取中断�
**********************************************************************************************/
unsigned char IT_ITP_Status(void)
{
	unsigned short temp;
	temp = ((Reg_Read(R0)>>9)&0x0001);
	return (unsigned char) temp;
}

/**********************************************************************************************
* 参数：
* 返回值：中断状态
		  1   中断产生
		  2/3 中断没产生
* 说明：位模式插补中断状态 SC由2 变为1 
**********************************************************************************************/
//位模式插补中断状态 RR0/D14-D13  //(SC) is changed from 2to 1, and the stack is available for next BP command writing...
unsigned char IT_BP_Status(void)
{
	unsigned short temp;
	temp = ((Reg_Read(R0)>>13)&0x0003);
	return (unsigned char) temp;
}

/**********************************************************************************************
* 参数：
* 返回值：中断状态
		  0 中断没产生
		  1 中断产生
* 说明：回原点中断状态 回原点结束中断
**********************************************************************************************/
//回原点中断状态 RR0/D14-D13  回原点结束是否有中断产生
unsigned char IT_Home_Status(void)
{
	unsigned short temp;
	temp = ((Reg_Read(R3)>>8)&0x0001);
	return (unsigned char) temp;
}

/**********************************************************************************************
* 参数：
* 返回值：中断状态
		  0 中断没产生
		  1 中断产生
* 说明：获取同步运动中断状态�
**********************************************************************************************/
//同步运动中断状态 RR0/D14-D13 
unsigned char IT_SYN_Status(void)
{
	unsigned short temp;
	temp = ((Reg_Read(R3)>>9)&0x0001);
	return (unsigned char) temp;
}
/**********************************************************************************************
* 参数：it为IT_NUM类型枚举
* 返回值：无
* 说明：获取中断�
**********************************************************************************************/
IT_MASK IT_Axis_Status(void)
{
	IT_MASK temp;
	
	temp = (IT_MASK)(Reg_Read(R3) & IT_ALL_MASK);

	switch(temp)
	{
		case IT_PULSE_MASK:		//0x0001,脉冲上升沿
			temp = IT_PULSE_MASK;
		break;
		case IT_LMC_MASK:		//0x0002,逻辑/编码器计数器大于负方向比较寄存器数值
			temp = IT_LMC_MASK;
		break;
		case IT_SMC_MASK:		//0x0004,逻辑/编码器计数器小于负方向比较寄存器数值
			temp = IT_SMC_MASK;
		break;
		case IT_SPC_MASK:		//0x0008,逻辑/编码器计数器小于正方向比较寄存器数值
			temp = IT_SPC_MASK;
		break;
		case IT_LPC_MASK:		//0x0010,逻辑/编码器计数器大于正方向比较寄存器数值
			temp = IT_LPC_MASK;
		break;
		case IT_CEND_MASK:		//0x0020,加减速驱动时，定速域结束脉冲输出
			temp = IT_CEND_MASK;
		break;
		case IT_CSTA_MASK:		//0x0040,加减速驱动时，定速域开始脉冲输出
			temp = IT_CSTA_MASK;
		break;
		case IT_DEND_MASK:		//0x0080,驱动结束
			temp = IT_DEND_MASK;
		break;
		case IT_HOME_MASK:		//0x0100,原点搜索
			temp = IT_HOME_MASK;
		break;
		case IT_SYNC_MASK:		//0x0200,同步激活
			temp = IT_SYNC_MASK;
		break;
		default:
			temp = IT_ERR_MASK;
		break;
	}
	
	return temp;
}
/**********************************************************************************************
* 参数：无
* 返回值：出错的状态源
* 说明：获取错误源
**********************************************************************************************/
//获取错误源
ERR_MASK Error_Is(void)
{
	ERR_MASK temp;
	
	temp = (ERR_MASK)((Reg_Read(R2) & ERR_ALL_MASK));

	switch(temp)
	{
		case ERR_LPC_MASK:		//0x0001,逻辑/编码器计数器大于正方向比较寄存器数值
			temp = ERR_LPC_MASK;
		break;
		case ERR_SMC_MASK:		//0x0002,逻辑/编码器计数器大于负方向比较寄存器数值
			temp = ERR_SMC_MASK;
		break;
		case ERR_PL_MASK:		//0x0004,硬件正限位
			temp = ERR_PL_MASK;
		break;
		case ERR_ML_MASK:		//0x0008,硬件负限位
			temp = ERR_ML_MASK;
		break;
		case ERR_ALARM_MASK:	//0x0010,伺服报警
			temp = ERR_ALARM_MASK;
		break;
		case ERR_EMG_MASK:		//急停
			temp = ERR_EMG_MASK;
		break;
		default:
			temp = ERR_ERR_MASK;
		break;
	}

	return temp;
}


/**********************************************************************************************
* 参数：Encoder_InitStruct为ENCODER_InitTypeDef类型枚举
* 返回值：无
* 说明：用Encoder_InitStruct中设置的参数初始化编码器
**********************************************************************************************/
void Encoder_Init(Encoder_InitTypeDef Encoder_InitStruct)
{
	wr_map[R2] &= 0xf1ff;
	wr_map[R2] |= (Encoder_InitStruct.Pulse_Mode << 9)
					|(Encoder_InitStruct.Pulse_Mul);
	Reg_Write(R2, wr_map[R2]);
}

/**********************************************************************************************
* 参数：Counter_Value为编码器寄存器设置值
* 返回值：无
* 说明：设置编码器计数器初值
**********************************************************************************************/
void Encoder_SetCounter(int Counter_Value)
{
	Data_Write(WEP, Counter_Value);
}

/**********************************************************************************************
* 名称：int Encoder_GetCounter()
* 参数：无
* 返回值：编码器计数值
* 说明：获取编码器计数器初值
**********************************************************************************************/
int Encoder_GetCounter()
{
	return Data_Read(REP);
}

/**********************************************************************************************
* 参数：Comp_InitStruct为Comp_InitTypeDef类型枚举，
		其中定义比较寄存器信息
* 返回值：无
* 说明：用Comp_InitStruct中设置的参数初始化比较器
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
* 参数：数组指针
		Data0      命令码	
		Data1      选轴 01-X轴 02-Y轴 04-Z轴  08-U轴
		Data2      正向比较寄存器状态 STATE：0使能 1失能
		Data3      负向比较寄存器状态 STATE：0使能 1失能
		Data4      0x00:比较对象LG_CNT = 0x0000逻辑计数器   0x20:EC_CNT = 0x0020编码器计数器	  	
		Data5~8    正向比较寄存器初值
		Data9~12   负向比较寄存器初值	
* 返回值：无
* 说明   比较寄存器配置 1轴 2正向比较寄存器状态 3负向比较寄存器状态 4比较对象 5~8正向比较寄存器初值 9~12负向比较寄存器初值	
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
	Set_Axis((AXIS_NUM)(Data[1]<<8));//选轴
//Data 0命令码 1轴 2正向比较寄存器状态 3负向比较寄存器状态 4比较对象 5~8正向比较寄存器初值 9~12负向比较寄存器初值	
	Comp_InitStruct.PComp_State = (STATE)Data[2];
	Comp_InitStruct.MComp_State = (STATE)Data[3];
	Comp_InitStruct.Comp_Obj = (OBJ)Data[4];
	Comp_InitStruct.PComp_Value = (Data[5]<<24) | (Data[6]<<16) | (Data[7]<<8) | (Data[8]);
	Comp_InitStruct.MComp_Value = (Data[9]<<24) | (Data[10]<<16) | (Data[11]<<8) | (Data[12]);
	Comp_Init(Comp_InitStruct);	
}
/**********************************************************************************************
* 参数：AXIS_InitStruct为AXIS_InitTypeDef类型枚举，
		其中定义硬件限位、脉冲模式信息
* 返回值：无
* 说明：用AXIS_InitStuct中设置的参数初始化驱动轴
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
* 参数：数组指针
		Data0   命令码	
		Data1   选轴 01-X轴 02-Y轴 04-Z轴  08-U轴
		Data2   停止模式  0立即停止 1减速停止 
		Data3   正限位逻辑电平 0低  1高
		Data4   负限位逻辑电平 0低  1高
		Data5   脉冲模式  0双脉冲模式 1方向-脉冲模式
		Data6   输出脉冲逻辑电平 0低  1高
		Data7   输出脉冲方向电平 0低  1高	
* 返回值：无
* 说明  轴模式设置 1轴 2停止模式 3正限位逻辑电平 4负限位逻辑电平 5脉冲模式  6输出脉冲逻辑电平 7输出脉冲方向电平
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
	Set_Axis((AXIS_NUM)(Data[1]<<8));//选轴
//Data 0命令码 1轴 2停止模式 3正限位逻辑电平 4负限位逻辑电平 5脉冲模式  6输出脉冲逻辑电平 7输出脉冲方向电平
	AXIS_InitStruct.Stop_Mode = (STOP_MODE)Data[2];
	AXIS_InitStruct.PLimit_Level = (LEVEL)Data[3];
	AXIS_InitStruct.MLimit_Level = (LEVEL)Data[4];
	AXIS_InitStruct.Pulse_Mode = (PULSE_MODE)Data[5];
	AXIS_InitStruct.Logic_Level = (LEVEL)Data[6];
	AXIS_InitStruct.Dir_Level = (LEVEL)Data[7];
	AXIS_Init(AXIS_InitStruct);
}

/**********************************************************************************************
* 参数：Drive_InitStruct为Drive_InitTypeDef类型结构体
* 返回值：无
* 说明：用结构体Drive_InitTypeDef中定义的值初始化指定的轴
**********************************************************************************************/
void Drive_Init(Drive_InitTypeDef Drive_InitStruct)
{
	//速度变化率设置
	if(Drive_InitStruct.Range)
		Data_Write(WR, Drive_InitStruct.Range);
	
	//加速度设置
	if(Drive_InitStruct.Acce)
		Data_Write(WA, Drive_InitStruct.Acce);
	
	//减速度设置
	if(Drive_InitStruct.Dece)
	{
		wr_map[R3] &= 0x0f9d;
		wr_map[R3] |= 0x0002;
		Reg_Write(R3, wr_map[R3]);
		Data_Write(WD, Drive_InitStruct.Dece);
	}
	//减速度使用加速度值
	else
	{
		wr_map[R3] &= 0x0f9d;
		Reg_Write(R3, wr_map[R3]);
	}
	
	//S曲线加减速变化率设置
	if(Drive_InitStruct.Acce_Rate || Drive_InitStruct.Dece_Rate)
	{
		wr_map[R3] &= 0x0f9b;
		wr_map[R3] |= 0x0004;
		Reg_Write(R3, wr_map[R3]);
		Data_Write(WK, Drive_InitStruct.Acce_Rate);
		Data_Write(WL, Drive_InitStruct.Dece_Rate);
	}
	//直线加减速设置
	else
	{
		wr_map[R3] &= 0x0f9b;
		Reg_Write(R3, wr_map[R3]);
	}
	
	//设置加速偏移值，默认为8，在低启动速度的固定脉冲驱动时该值应设为0
	Data_Write(WAO, Drive_InitStruct.Acce_Offset);
	
	//初始速度设置
	if(Drive_InitStruct.Init_Speed)
		Data_Write(WSV, Drive_InitStruct.Init_Speed);
	
	//驱动速度设置
	if(Drive_InitStruct.Speed)
		Data_Write(WV, Drive_InitStruct.Speed);
	
	//手动减速点设置，Man_Dece_Pos为0表示无手动减速点
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
* 参数：数组指针
		Data0       命令码	
		Data1       选轴 01-X轴 02-Y轴 04-Z轴  08-U轴
		Data2~5      R     倍率 Multiple=8000000/R
		Data6~7      A     加速度ACC=A*25*Multiple
		Data8~9      D     减速度DEC=D*125*Multiple
		Data10~11    K     加速度率Jerk=62.5*10^6/K*Multiple
		Data12~13    K     减速度率Jerk=62.5*10^6/K*Multiple
		Data14~15    SV    初始速度Init_Speed=SV*Multiple 
		Data16~17    V     驱动速度Drive_Speed=V*Multiple 
		Data18~19	加速度偏移
		Data20~23	手动减速点		
* 返回值：无
* 说明：Axis 倍率 初始/驱动速度  加/减速度 加/减速度率 加速度偏移 手动减速点参数设置 
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
	Set_Axis((AXIS_NUM)(Data[1]<<8));//选轴
	//  Data 0命令码 1轴 2~5倍率 6~7加速度 8~9减速度 10~11加速度率 12~13减速度率 14~15初始速度 16~17驱动速度 18~19加速度偏移 20~23手动减速点
//	Drive_InitStruct.Range = ((Data[2]<<24) | (Data[3]<<16) | (Data[4]<<8) | (Data[5]));   //R   范围R  16000-8000000  4bytes  倍率 Multiple=8000000/R
//	Drive_InitStruct.Acce = (Data[6]<<8) | Data[7];              //A     加速度A     1-8000       2bytes  ACC=A*25*Multiple
//	Drive_InitStruct.Dece = (Data[8]<<8) | Data[9];              //D     减速度D     1-8000       2bytes  DEC=D*125*Multiple
//	Drive_InitStruct.Acce_Rate = (Data[10]<<8) | Data[11];       //K     加速度率K   1-65535      2bytes  Jerk=62.5*10^6/K*Multiple
//	Drive_InitStruct.Dece_Rate = (Data[12]<<8) | Data[13];       //K     减速度率K   1-65535      2bytes  Jerk=62.5*10^6/K*Multiple
//	Drive_InitStruct.Init_Speed = (Data[14]<<8)| Data[15];       //SV    初始速度SV  1-8000       2bytes  Init_Speed=SV*Multiple 
//	Drive_InitStruct.Speed = (Data[16]<<8) | Data[17];           //V     驱动速度V   1-8000       2bytes  Drive_Speed=V*Multiple 
//	Drive_InitStruct.Acce_Offset = (Data[18]<<8) | Data[19];     //加速度偏移-32768-+32768        2bytes
//	Drive_InitStruct.Man_Dece_Pos = (Data[20]<<24) | (Data[21]<<16) | (Data[22]<<8) | (Data[23]);
															  // 手动减速点 0-4294967295 4bytes
	Drive_InitStruct.Range = 8000000;   //R   范围R  16000-8000000  4bytes  倍率 Multiple=8000000/R
	Drive_InitStruct.Acce = 1;              //A     加速度A     1-8000       2bytes  ACC=A*25*Multiple
	Drive_InitStruct.Dece = 1;              //D     减速度D     1-8000       2bytes  DEC=D*125*Multiple
	Drive_InitStruct.Acce_Rate = 10;       //K     加速度率K   1-65535      2bytes  Jerk=62.5*10^6/K*Multiple
	Drive_InitStruct.Dece_Rate = 10;       //K     减速度率K   1-65535      2bytes  Jerk=62.5*10^6/K*Multiple
	Drive_InitStruct.Init_Speed = 0;       //SV    初始速度SV  1-8000       2bytes  Init_Speed=SV*Multiple 
	Drive_InitStruct.Speed = (Data[2]<<8) | Data[3];           //V     驱动速度V   1-8000       2bytes  Drive_Speed=V*Multiple 
	Drive_InitStruct.Acce_Offset = 0;     //加速度偏移-32768-+32768        2bytes
	Drive_InitStruct.Man_Dece_Pos =0;
															  // 手动减速点 0-4294967295 4bytes

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
* 参数  数组指针�
 		DData0  命令码	
		Data1  选轴驱动的轴 01-X轴 02-Y轴 04-Z轴  08-U轴		
		Data2  8 固定速度 
			   0 对称线性加减速度率,   自动减速 （梯形） 
			   1 对称线性加减速度率，  手动减速 （梯形） 
			   2 非对称线性加减速度率，自动减速 （梯形） 
			   3 非对称线性加减速度率，手动减速 （梯形） 
			   4 对称S形加减速度率，   自动减速 （S形） 
			   5 对称S形加减速度率，   手动减速 （S形） 
			   6 非对称S形加减速度率， 自动减速 （S形）
			   7 非对称S形加减速度率， 手动减速 （S形）
		Data3  PFD(0x20)或MFD(0x21)，PFD正向固定脉冲驱动，MFD反向固定脉冲驱动
		Data4~7 为脉冲数 
* 返回值：无
* 说明：让指定轴运动mode方向,驱动模式，pulse数量的脉冲
**********************************************************************************************/
void Axis_Fixed_Drive(unsigned char *temp)
{
	unsigned char i;
	unsigned char Data[8];
	for(i=0;i<8;i++)
	{
		Data[i] = *temp++;
	}
	Set_Axis((AXIS_NUM)(Data[1]<<8));//Data[1]选轴
//Data 0命令码 1轴 2驱动模式 3驱动类型 4~7脉冲数
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
		CMD_Write((CMD_NUM)Data[3]);//固定脉冲输出命令		
	}
}

/**********************************************************************************************
* 参数：数组指针
		Data0  命令码	
		Data1  选轴 01-X轴 02-Y轴 04-Z轴  08-U轴
		Data2  可取PCD(0x22)或MCD(0x23)，PCD正向连续驱动，MCD反向连续驱动
* 返回值：无
* 说明：启动指定轴指定方向连续驱动
**********************************************************************************************/
void Axis_Continuous_Drive(unsigned char *temp)
{
	unsigned char i;
	unsigned char Data[3];
	for(i=0;i<3;i++)
	{
		Data[i] = *temp++;
	}
	Set_Axis((AXIS_NUM)(Data[1]<<8)); //Data[1]选轴
	CMD_Write((CMD_NUM)Data[2]);     //写连续驱动命令
}
/**********************************************************************************************
* 参数：数组指针
		Data0   命令码	
		Data1   选轴 01-X轴 02-Y轴 04-Z轴  08-U轴
		Data2~5 诺为定长运动则为定长运动输出的脉冲数，如连续输出则可以不管
		Data6   00  external signals disabled
				01  continuous driving mode
				02  fixed pulse driving mode
				03  MPG mode
* 返回值：无
* 说明：指定轴外部信号驱动
**********************************************************************************************/
void External_Signals_Drive(unsigned char *temp)
{
	unsigned char i;
	unsigned char Data[7];
	for(i=0;i<7;i++)
	{
		Data[i] = *temp++;
	}
	Set_Axis((AXIS_NUM)(Data[1]<<8)); //Data[1]选轴
	Data_Write(WEM,0x00005f00);//外部信号滤波
	Data_Write(WP, (Data[2]<<24) | (Data[3]<<16) | (Data[4]<<8) | (Data[5]));
	wr_map[R3] &= 0x0f87;
	wr_map[R3] |= (Data[6]<<3);
	Reg_Write(R3,wr_map[R3]);	
}
//失能外部信号驱动
void Clear_External_Drive(void)
{
	wr_map[R3] &= 0x0f87;
	Reg_Write(R3,wr_map[R3]);
}	

/**********************************************************************************************
* 参数: 数组指针
		Data0  命令码	
		Data1  选轴  01-X轴 02-Y轴 04-Z轴  08-U轴    
        Data2  停止模式
				0x26 DECS减速停止�
				0x27 SUDS立即停止
* 返回值：无
* 说明：停止指定的轴运动
**********************************************************************************************/
void AXIS_Stop(unsigned char *temp)
{
	unsigned char i;
	unsigned char Data[3];
	for(i=0;i<3;i++)
	{
		Data[i] = *temp++;
	}
	Set_Axis((AXIS_NUM)(Data[1]<<8)); //Data[1]选轴
	CMD_Write((CMD_NUM)Data[2]);
}
/**********************************************************************************************
* 参数: 数组指针
		Data0  命令码	
		Data1  选轴 01-X轴 02-Y轴 04-Z轴  08-U轴     
        Data2  伺服模式  D0 nALARM 0:Low/1:Hi  
						 D1 nALARM 0:Disable/1:Enable 
						 D3 nINPOS 0:Low/1:Hi
						 D4 nINPOS 0:Disable/1:Enable 
* 返回值：无
* 说明：伺服模式配置
**********************************************************************************************/
void Servo_Mode_Config(unsigned char *temp)
{
	unsigned char i;
	unsigned char Data[3];
	for(i=0;i<3;i++)
	{
		Data[i] = *temp++;
	}
	Set_Axis((AXIS_NUM)(Data[1]<<8)); //Data[1]选轴
	wr_map[R2] &= 0x0fff;
	wr_map[R2] |= (Data[2]<<12);
	Reg_Write(R2, wr_map[R2]);
}
/**********************************************************************************************
* 参数: 数组指针
		Data0  命令码	
		Data1  选轴 01-X轴 02-Y轴 04-Z轴  08-U轴     
        Data2  1使能正方向软件限位   2使能负方向软件限位
		Data3  0与逻辑位置计数器比较 1与实际位置计数器比较
* 返回值：无
* 说明：软限位使能
**********************************************************************************************/
void Software_Limit(unsigned char *temp)
{	
	unsigned char i;
	unsigned char Data[8];
	for(i=0;i<8;i++)
	{
		Data[i] = *temp++;
	}
	Set_Axis((AXIS_NUM)(Data[1]<<8)); //Data[1]选轴
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
* 参数： 无
* 返回值：无
* 说明：外部信号清除实际位置  回原点搜索 Z-phase search  IN2
**********************************************************************************************/
//扩展模式设置中  外部信号清除实际位置  回原点搜索 Z-phase search  IN2
void EXT_CLR_RLP(void)
{
	wr_map[R1] &= 0xffcf;
	wr_map[R1] |= 0x0030;// 使能IN2 WR1/D5    active level WR1/D4 IN2为Hi 
	Reg_Write(R1,wr_map[R1]);
	
	wr_map[R6] &= 0xfffe;
	wr_map[R6] |= 0x0001;
	Reg_Write(R6,wr_map[R6]);
	Data_Write(WEM,0x5f00);
}


/**********************************************************************************************
* 参数：数组值
* 返回值：
* 说明：处理插补的各轴轴号（轴转换）
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
* 参数：数组指针
		Data0    命令码
		Data1    轴1 01-X轴 02-Y轴 04-Z轴  08-U轴
		Data2    轴2 01-X轴 02-Y轴 04-Z轴  08-U轴
		Data3~6  轴1脉冲数
		Data7~10 轴2脉冲数
		Data11   3B-插补减速有效 3C-插补减速无效
* 返回值：无
* 说明：两轴直线插补
**********************************************************************************************/
void ITP_2AxisLine(unsigned char *temp)
{
	unsigned char i;
	unsigned char Data[12];
	for(i=0;i<12;i++)
	{
		Data[i] = *temp++;
	}
	//Data 0命令码  1轴1  2轴2  3~6轴1脉冲数 7~10轴2脉冲数 11插补减速有效否
	wr_map[R5] &= 0xdb00;
	wr_map[R5] |= ITP_Axis(Data[1])
					|(ITP_Axis(Data[2]) << 2);					
	Reg_Write(R5, wr_map[R5]);
	
	Set_Axis((AXIS_NUM)(Data[1]<<8)); //Data[1]选轴1
	Data_Write(WP, (Data[3]<<24) | (Data[4]<<16) | (Data[5]<<8) | (Data[6]));
	
	Set_Axis((AXIS_NUM)(Data[2]<<8)); //Data[2]选轴2
	Data_Write(WP, (Data[7]<<24) | (Data[8]<<16) | (Data[9]<<8) | (Data[10]));
	
	CMD_Write((CMD_NUM)Data[11]);
	CMD_Write(I2L);
}



/**********************************************************************************************
* 参数：数组指针
		Data0     命令码
		Data1     轴1 01-X轴 02-Y轴 04-Z轴  08-U轴
		Data2     轴2 01-X轴 02-Y轴 04-Z轴  08-U轴
		Data3     轴3 01-X轴 02-Y轴 04-Z轴  08-U轴
		Data4~7   轴1脉冲数
		Data8~11  轴2脉冲数
		Data12~15 轴2脉冲数
		Data16   3B-插补减速有效 3C-插补减速无效
* 返回值：无
* 说明：三轴直线插补
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
	
	Set_Axis((AXIS_NUM)(Data[1]<<8)); //Data[1]选轴1
	Data_Write(WP, (Data[4]<<24) | (Data[5]<<16) | (Data[6]<<8) | (Data[7]));
	
	Set_Axis((AXIS_NUM)(Data[2]<<8)); //Data[2]选轴2
	Data_Write(WP, (Data[8]<<24) | (Data[9]<<16) | (Data[10]<<8) | (Data[11]));
	
	Set_Axis((AXIS_NUM)(Data[3]<<8)); //Data[3]选轴3
	Data_Write(WP, (Data[12]<<24) | (Data[13]<<16) | (Data[14]<<8) | (Data[15]));
	
	CMD_Write((CMD_NUM)Data[16]);
	CMD_Write(I3L);
}

/**********************************************************************************************
* 参数：数组指针
		Data0      命令码
		Data1      轴1 01-X轴 02-Y轴 04-Z轴  08-U轴
		Data2      轴2 01-X轴 02-Y轴 04-Z轴  08-U轴
		Data3~6    轴1圆心
		Data7~10   轴2圆心
		Data11~14  轴1脉冲数
		Data15~18  轴2脉冲数
		Data19     3B-插补减速有效 3C-插补减速无效
* 返回值：无
* 说明：两轴顺时针方向圆弧插补
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
	
	Set_Axis((AXIS_NUM)(Data[1]<<8)); //Data[1]选轴1
	Data_Write(WC, (Data[3]<<24) | (Data[4]<<16) | (Data[5]<<8) | (Data[6]));
	Data_Write(WP, (Data[11]<<24) | (Data[12]<<16) | (Data[13]<<8) | (Data[14]));
	
	Set_Axis((AXIS_NUM)(Data[1]<<8)); //Data[1]选轴2
	Data_Write(WC, (Data[7]<<24) | (Data[8]<<16) | (Data[9]<<8) | (Data[10]));
	Data_Write(WP, (Data[15]<<24) | (Data[16]<<16) | (Data[17]<<8) | (Data[18]));
	
	CMD_Write((CMD_NUM)Data[19]);
	CMD_Write(ICW);
}


/**********************************************************************************************
* 参数：数组指针
		Data0      命令码
		Data1      轴1 01-X轴 02-Y轴 04-Z轴  08-U轴
		Data2      轴2 01-X轴 02-Y轴 04-Z轴  08-U轴
		Data3~6    轴1圆心
		Data7~10   轴2圆心
		Data11~14  轴2脉冲数
		Data15~18  轴2脉冲数
		Data19     3B-插补减速有效 3C-插补减速无效
* 返回值：无
* 说明：两轴逆时针方向圆弧插补
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
	
	Set_Axis((AXIS_NUM)(Data[1]<<8)); //Data[1]选轴1
	Data_Write(WC, (Data[3]<<24) | (Data[4]<<16) | (Data[5]<<8) | (Data[6]));
	Data_Write(WP, (Data[11]<<24) | (Data[12]<<16) | (Data[13]<<8) | (Data[14]));
	
	Set_Axis((AXIS_NUM)(Data[2]<<8)); //Data[2]选轴2
	Data_Write(WC, (Data[7]<<24) | (Data[8]<<16) | (Data[9]<<8) | (Data[10]));
	Data_Write(WP, (Data[15]<<24) | (Data[16]<<16) | (Data[17]<<8) | (Data[18]));
	
	CMD_Write((CMD_NUM)Data[19]);
	CMD_Write(ICCW);
}
#ifdef IB
/**********************************************************************************************
* 参数：axisn可取枚举AXIS_NUM中定义的值
		pulsen为轴axisn对应的相对脉冲数
* 返回值：无
* 说明：两轴位模式插补
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
* 参数：
* 返回值：无
* 说明：2轴位模式插补
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
	
	Set_Axis((AXIS_NUM)(Data[1]<<8)); //Data[1]选轴1
	Set_Axis((AXIS_NUM)(Data[2]<<8)); //Data[2]选轴2
	CMD_Write(BPDC);//清除BP寄存器数据
	CMD_Write(BPRWEN);//使能写BP寄存器
	
						//命令数据0~15
	Reg_Write(R2,Data[3]);//axis1 +direction
	Reg_Write(R3,Data[4]);//axis1 -direction
	Reg_Write(R4,Data[5]);//axis2 +direction
	Reg_Write(R5,Data[6]);//axis2 -direction
	CMD_Write(BPDS);   //BP数据压栈 SC=1
	
						//命令数据16~31
	Reg_Write(R2,Data[7]);//axis1 +direction
	Reg_Write(R3,Data[8]);//axis1 -direction
	Reg_Write(R4,Data[9]);//axis2 +direction
	Reg_Write(R5,Data[10]);//axis2 -direction
	CMD_Write(BPDS);   //BP数据压栈 SC=2
	
						//命令数据32~47
	Reg_Write(R2,Data[11]);//axis1 +direction
	Reg_Write(R3,Data[12]);//axis1 -direction
	Reg_Write(R4,Data[13]);//axis2 +direction
	Reg_Write(R5,Data[14]);//axis3 -direction
	CMD_Write(BPDS);   //BP数据压栈 SC=3
	
	CMD_Write((CMD_NUM)Data[15]);
	CMD_Write(I2B);	
	
	RData = ((Reg_Read(R0)>>13)&0x0003);
	if(RData==0x02||0x01)
	{						
							//命令数据48~62
		Reg_Write(R2,Data[16]);//axis1 +direction
		Reg_Write(R3,Data[17]);//axis1 -direction
		Reg_Write(R4,Data[18]);//axis2 +direction
		Reg_Write(R5,Data[19]);//axis3 -direction
		CMD_Write(BPDS);//BP数据压栈 SC=3
	}
	RData = ((Reg_Read(R0)>>13)&0x0003);
	if(RData==0x02||0x01)
	{						
							//命令数据48~62
		Reg_Write(R2,Data[20]);//axis1 +direction
		Reg_Write(R3,Data[21]);//axis1 -direction
		Reg_Write(R4,Data[22]);//axis2 +direction
		Reg_Write(R5,Data[23]);//axis3 -direction
		CMD_Write(BPDS);//BP数据压栈 SC=3
	}
	
}

/**********************************************************************************************
* 参数：axisn可取枚举AXIS_NUM中定义的值
		pulsen为轴axisn对应的相对脉冲数
* 返回值：无
* 说明：三轴位模式插补
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
* 参数：
* 返回值：无
* 说明：单步Command插补模式  
**********************************************************************************************/
void ITP_Step_Command(void)
{	
	Reg_Write(R5,0x1104);
	CMD_Write(ISS);		
	//插补参数和插补命令Set the initial and drive speeds,finish point, center point.......
		
}
/**********************************************************************************************
* 参数：数组指针
		Data0      命令码
		Data1      0 外部信号执行单步插补 
				   1 命令(0x3A)执行单步插补	
* 返回值：无
* 说明：外部信号或者命令执行单步插补
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
* 参数：
* 返回值：无
* 说明：单步外部信号插补模式  
**********************************************************************************************/
void ITP_Step_External(void)
{
	
	Reg_Write(R5,0x0904);
	Data_Write(WEM,0x5f00);
	CMD_Write(ISS);		
	//设置插补参数 命令  滤波器 等待外部信号为低 	
}
/**********************************************************************************************
* 参数：state可取DECV    3B  插补减速有效 enable the automatic deceleration or manual deceleration function
	             DECINV  3C	 插补减速无效 disable the automatic deceleration or manual deceleration function
* 返回值：无
* 说明：插补停止模式//In continuous interpolation, same as in circular and bit pattern interpolations, only manual deceleration in the trapezoidal driving
					//is available; The automatic deceleration in S-curve driving is not available.
					//In circular interpolation and bit pattern interpolation, only manual deceleration in trapezoidal driving is available; the automatic
					//deceleration in S-curve driving is not available.
**********************************************************************************************/
void ITP_Dece_State(CMD_NUM state)
{
	CMD_Write(state);
}
/**********************************************************************************************
* 参数：state可取DECS或者SUDS，DECS减速停止，SUDS立即停止
* 返回值：无
* 说明：插补停止模式
**********************************************************************************************/
void ITP_Dece_Mode(CMD_NUM state)
{
	CMD_Write(state);
}

/**********************************************************************************************
* 参数：无
* 返回值：插补状态
		  0 插补完成结束
		  1 插补在进行
* 说明：查询插补状态
**********************************************************************************************/
unsigned char ITP_Stauts(void)
{
	unsigned short temp;
	temp = ((Reg_Read(R0)>>8)&0x0001);
	return (unsigned char)temp;
}
/**********************************************************************************************
* 参数：无
* 返回值：连续插补状态
		  0 不能进行连续插补
		  1 能进行下一个插补
* 说明：查询是否可以进行连续插补状态 //连续插补 当前插补命令已经执行且 RR0/D9 为1 可以进行连续插补(检查状态之前 先要检查错误（插补的轴RRO/D4~D7） 没错误才可以执行下个插补)�
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
* 说明：IO读
* 参数：IO口编号
* 返回值：IO状态
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
* 说明：IO写
* 参数：IO口编号，写IO口状态
* 返回值：无
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
* 参数：数组指针
		Data0         命令码	
		Data1         选轴 01-X轴 02-Y轴 04-Z轴  08-U轴
		Data2~5       输入信号滤波使能 对应为1就是使能对其信号滤波功能
* 返回值：无
* 说明�
**********************************************************************************************/
void Iput_Filters_Config(void)
{
	
	Data_Write(WEM,0x5f00);
}

/**********************************************************************************************
* 参数：数组指针
		Data0       命令码	
		Data1       选轴
		Data2       输入信号电平
		Data3~6     外部扩展模式和信号滤波参数设置（WR6滤波 WR7扩展）
		Data7~8     原点搜索速度
		Data9~12    偏移脉冲数
* 返回值：无
* 说明：近零点nIN0、零点nIN2、Z相信号IN2作为回原点信号,nIN0与nIN1作为回原点信号限位信号作为回原点信号，
**********************************************************************************************/
void Auto_Home_Search(unsigned char *temp)
{
	unsigned char Data[13];
	int i;	
	for(i=0;i<13;i++)
	{
		Data[i]=*temp++;
	}	
	Set_Axis((AXIS_NUM)(Data[1]<<8));//选轴
	wr_map[R1] &= 0xff00;
	wr_map[R1] |= Data[2];
	Reg_Write(R1, wr_map[R1]);  //输入信号逻辑设置D6,4,2,0 INm-L 高低电平   D7,5,3,1 驱动停止输入信号 INm 0: disable, 1: enable  
	//Data[0]..... 0命令1字节   1轴1字节   2输入信号逻辑1字节  3~6扩展模式参数4字节  7原点搜索速度2字节  9~12输出偏移脉冲数 
	//0x00,0x01,0x00,0x49,0x5f,0x5f,0x00,0x00,0x50,0x00,0x00,0x0f,0xff
	Data_Write(WEM,(Data[3]<<24) | (Data[4]<<16) | (Data[5]<<8) | Data[6]);
								//扩展模式设置
								//WR6输入信号滤波器模式：   5f00
								//D15~D13  010  滤波器延时：512us
								//D8        1   nLMTM,nIN0,nIN1信号： ENABLE(使能滤波器)
								//D9        0   nIN2信号      DISABLE(失能滤波器)�
	
								//WR7自动回零模式设置：a near home, home, or a Z-phase signal-0x495f ，IN0 and IN1-0x014f,a limit signal-0x054f
								//D15~D13  010 误差计数器清零脉冲宽度100us
								//D12       0  误差计数器清零脉冲输出逻辑电平：高
 								//D11       1  误差计数器清零输出：      ENABLE
								//D10		0  使用限位信号作为回零信号：DISABLE
								//D9        0  Z相AND回零信号：          DISABLE
								//D8        1  逻辑/实际位置计数器：     ENABLE  
	                            //D7        0  Step4驱动方向：           +正方向
								//D6        0  Step4：                   ENABLE
								//D5        0  Step3驱动方向：           -负方向
								//D4        1  Step3：                   ENABLE
								//D3        1  Step2驱动方向：           -负方向
								//D2        1  Step2：                   ENABLE
								//D1        1  Step1驱动方向：           -负方向
								//D0        1  Step1：                   ENABLE
								
								
	Data_Write(WHV,(Data[7]<<8) | (Data[8]));//Step 2原点搜索速度500
	Data_Write(WP, (Data[9]<<24) | (Data[10]<<16) | (Data[11]<<8) | Data[12]);//偏移脉冲
	CMD_Write(AHOME);//启动自动回原点搜索
}
//清除回原点限位
void Clear_LimtMod(void)
{
	wr_map[R1] &= 0xff33;
	Reg_Write(R1, wr_map[R1]);    //失能限位信号
	//Clear_Axis();
}
/**********************************************************************************************
* 参数：数组指针
		Data0       命令码	
		Data1       轴1主轴 
		Data2       轴2
		Data3       轴3
		Data4       轴4
		Data5~8     轴1正向比较器计数值
		Data9~12    轴1输出脉冲数
		Data13~16   轴2输出脉冲数
		Data17~20   轴3输出脉冲数
		Data21~24   轴4输出脉冲数
		Data25~28   轴1同步模式参数
		Data29~32   轴2同步模式参数
		Data33~36   轴3同步模式参数
		Data37~40   轴4同步模式参数
		Data41      轴1初始驱动状态
		Data42		轴2初始驱动状态
		Data43		轴3初始驱动状态
		Data44		轴4初始驱动状态
* 返回值：无
* 说明：
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
	Reg_Write(R2, wr_map[R2]);//与逻辑计数器比较WR2-D5=0
	Set_Axis((AXIS_NUM)(Data[1]<<8));//选轴1
	Set_Axis((AXIS_NUM)(Data[2]<<8));//选轴2
	Set_Axis((AXIS_NUM)(Data[3]<<8));//选轴3
	Set_Axis((AXIS_NUM)(Data[4]<<8));//选轴4
//0x00,/0x01,0x02,0x04,0x08,/0x00,0x00,0xff,0xff,/0x00,0x02,0xff,0xff,/0x00,0x02,0xff,0xff,/0x00,0x02,0xff,0xff,/0x00,0x02,0xff,0xff,/0x00,0x10,0xe0,0x01,/0x00,0x01,0x00,0x00,/0x00,0x04,0x00,0x00/,0x00,0x08,0x00,0x00/0x20,0x25,0x25,0x25	
	CompP_Set((AXIS_NUM)(Data[1]<<8),(Data[5]<<24) | (Data[6]<<16) | (Data[7]<<8) | Data[8]);  //axis1轴比较器设置数pulse
	LP_Set((AXIS_NUM)(Data[1]<<8),0);//清除axis1逻辑位置寄存器
	LP_Set((AXIS_NUM)(Data[2]<<8),0);//清除axis2逻辑位置寄存器
	LP_Set((AXIS_NUM)(Data[3]<<8),0);//清除axis3逻辑位置寄存器
	LP_Set((AXIS_NUM)(Data[4]<<8),0);//清除axis4逻辑位置寄存器
	
	Pulse_Out((AXIS_NUM)(Data[1]<<8),(Data[9]<<24) | (Data[10]<<16) | (Data[11]<<8) | Data[12]);  //axis1输出脉冲数pulse	
	Pulse_Out((AXIS_NUM)(Data[2]<<8),(Data[13]<<24) | (Data[14]<<16) | (Data[15]<<8) | Data[16]); //axis2输出脉冲数pulse
	Pulse_Out((AXIS_NUM)(Data[3]<<8),(Data[17]<<24) | (Data[18]<<16) | (Data[19]<<8) | Data[20]); //axis3输出脉冲数pulse	
	Pulse_Out((AXIS_NUM)(Data[4]<<8),(Data[21]<<24) | (Data[22]<<16) | (Data[23]<<8) | Data[24]); //axis4输出脉冲数pulse	
	
	
	SYN_Mode((AXIS_NUM)(Data[1]<<8),(Data[25]<<24) | (Data[26]<<16) | (Data[27]<<8) | Data[28]);        //axis1同步模式设置
	SYN_Mode((AXIS_NUM)(Data[2]<<8),(Data[29]<<24) | (Data[30]<<16) | (Data[31]<<8) | Data[32]);        //axis2同步模式设置
	SYN_Mode((AXIS_NUM)(Data[3]<<8),(Data[33]<<24) | (Data[34]<<16) | (Data[35]<<8) | Data[36]);        //axis3同步模式设置
	SYN_Mode((AXIS_NUM)(Data[4]<<8),(Data[37]<<24) | (Data[38]<<16) | (Data[39]<<8) | Data[40]);        //axis4同步模式设置
	
	Reg_Write(R0,(AXIS_NUM)(Data[1]<<8)|Data[41]);
	Reg_Write(R0,(AXIS_NUM)(Data[2]<<8)|Data[42]);
	Reg_Write(R0,(AXIS_NUM)(Data[3]<<8)|Data[43]);
	Reg_Write(R0,(AXIS_NUM)(Data[4]<<8)|Data[44]);	
	
	//输入信号nIN3 fell  nIN3中断发生 保存轴axis1，axis2，axis3位置数据
//读取XRR3寄存器 检查D9=1.中断发生 XIN3 fell�	
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
//清除同步模式
void Clear_Synch(void)
{
	Set_Axis(AXIS_ALL);
	Data_Write(WSM,0);
	AXIS_Switch();
}

