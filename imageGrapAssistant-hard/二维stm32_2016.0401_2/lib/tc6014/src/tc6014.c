/**
 *********************************************************************
 **±êÌâ£ºtc6014.c
 **×÷Õß£ºfyf
 **ÈÕÆÚ£º2014.10.28
 **°æ±¾£ºv1.0
 **ËµÃ÷£ºtc6014 driver
 *********************************************************************
**/

#include "tc6014.h"

//¼Ä´æÆ÷Ó³Éä
unsigned short wr_map[8] = {0x0000,0x0000,0x0000,0x00000,0x00000,0x0000,0x0000,0x0000};
unsigned short rr_map[8] = {0x0000,0x0000,0x0000,0x00000,0x00000,0x0000,0x0000,0x0000};

unsigned short Itp_Axis(AXIS_NUM axis);

/**********************************************************************************************
* ²ÎÊı£ºreg¿ÉÈ¡Ã¶¾ÙREGÖĞ¶¨ÒåµÄÖµ£¬ÓÃÓÚÑ¡ÔñÖ¸¶¨µÄ¼Ä´æÆ÷
		datÎªÒªĞ´ÈëregµÄÊı¾İ
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷£º¸Ãº¯ÊıÓÃÓÚ½«Ö¸¶¨µÄÊı¾İĞ´ÈëÖ¸¶¨µÄ¼Ä´æÆ÷
**********************************************************************************************/
void Reg_Write(REG reg, short dat)
{
	*(unsigned short*)(BASE_ADDR + OFFSET * reg) = dat;
}

/**********************************************************************************************
* ²ÎÊı£ºcmd¿ÉÈ¡Ã¶¾ÙCMD_NUMÖĞ¶¨ÒåµÄÃüÁî´úÂë£¬ÓÃÓÚÖ´ĞĞ¶ÔÓ¦µÄÃüÁî
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷£ºĞ´Ö¸¶¨µÄÃüÁîµ½ÃüÁî¼Ä´æÆ÷£¬ÒÔÊµÏÖ¶ÔÓ¦µÄ¹¦ÄÜ
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
* ²ÎÊı£ºcmd¿ÉÈ¡Ã¶¾ÙCMD_NUMÖĞ¶¨ÒåµÄÃüÁî´úÂë£¬ÓÃÓÚÖ´ĞĞ¶ÔÓ¦µÄÃüÁî
		datÎªÃüÁî¶ÔÓ¦µÄÊı¾İ
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷£ºÖ´ĞĞ´øÓĞÊı¾İµÄÃüÁî
**********************************************************************************************/
void Data_Write(CMD_NUM cmd, int dat)
{
	switch(cmd)
	{
		case WR:		//00h  	·¶Î§ÉèÖÃ
		case WP: 		//06  	Êä³öÂö³åÊıÉèÖÃ»òÕßÖÕµãÉèÖÃ
		case WDP:		//07  	ËÙ¶È¼õËÙµãÉèÖÃ
		case WC: 		//08  	Ô²ĞÄÉèÖÃ
		case WLP:		//09  	Âß¼­Î»ÖÃ¼ÆÊıÆ÷³õÖµÉèÖÃ
		case WEP:		//0A  	±àÂëÆ÷¼ÆÊıÆ÷³õÖµÉèÖÃ
		case WCP:		//0B  	ÕıÏò±È½Ï¼Ä´æÆ÷ÊıÖµÉèÖÃ
		case WCM:		//0C  	¸ºÏò±È½Ï¼Ä´æÆ÷ÉèÖÃ
		case WEM:		//60  	À©Õ¹Ä£Ê½ÉèÖÃ£¨Î»Êı¾İ£©
		case WSM:		//64  	Í¬²½Ä£Ê½ÉèÖÃ£¨Î»Êı¾İ)
			Reg_Write(R6, dat);
			Reg_Write(R7, (dat >> 16));
			CMD_Write(cmd);
			break;
		case WK:		//01 	¼ÓËÙ¶ÈÔö¼ÓÂÊÉèÖÃ
		case WA: 		//02  	¼ÓËÙ¶ÈÉèÖÃ
		case WD: 		//03  	¼õËÙ¶ÈÉèÖÃ
		case WSV:  		//04  	Æô¶¯ËÙ¶ÈÉèÖÃ
		case WV: 		//05  	Çı¶¯ËÙ¶ÈÉèÖÃ
		case WAO:		//0D  	¼ÓËÙ¶È¼Ä´æÆ÷Æ«ÒÆÉèÖÃ
		case WL: 		//0E  	¼õËÙ¶È¼õÉÙÂÊÉèÖÃ
		case WHV:		//61  	Ô­µãËÑË÷ËÙ¶È
			Reg_Write(R6, dat);
			CMD_Write(cmd);
			break;
		default:
			break;
	}
}

/**********************************************************************************************
* ²ÎÊı£ºregÎªREGÀàĞÍÃ¶¾Ù
* ·µ»ØÖµ£º¶ÁÈ¡µ½µÄÊı¾İ
* ËµÃ÷£º¶ÁÈ¡reg¼Ä´æÆ÷¶ÔÓ¦µÄÊı¾İ
**********************************************************************************************/
unsigned short Reg_Read(REG reg)
{
	return *(unsigned short*)(BASE_ADDR + OFFSET * reg);
}

/**********************************************************************************************
* ²ÎÊı£ºcmdÎªCMD_NUMÀàĞÍÃ¶¾Ù
* ·µ»ØÖµ£ºcmdÃüÁî¶ÔÓ¦µÄÊıÖµ
* ËµÃ÷£º¶ÁÈ¡cmdÃüÁî¶ÔÓ¦µÄshort£¨16Î»£©ĞÍÊı¾İ
**********************************************************************************************/
int Data_Read(CMD_NUM cmd)
{
	int temp;
	
	CMD_Write(cmd);
	switch(cmd)
	{
		case RLP: 		        //10h  	¶ÁÈ¡µ±Ç°Âß¼­Î»ÖÃ¼ÆÊıÆ÷ÊıÖµ
		case REP:  				//11  	¶ÁÈ¡±àÂëÆ÷ÊıÖµ
		case RSB:  				//14  	¶ÁÍ¬²½»º³å¼Ä´æÆ÷
			temp = Reg_Read(R7);//      ¶ÁÈ¡R7ÖĞµÄÊı¾İ Îª¸ß16Î»
			temp <<= 16;
			temp |= Reg_Read(R6);//     ¶ÁÈ¡R6ÖĞµÄÊı¾İ ÎªµÍ16Î»
		break;
		case RV:  				//12  	¶ÁÈ¡µ±Ç°Çı¶¯ËÙ¶È
		case RA:  				//13  	¶Á¼Ó¼õËÙ¶ÈÊıÖµ
			temp = Reg_Read(R6);//      ¶ÁÈ¡R6ÖĞµÄÊı¾İ ÎªµÍ16Î» ¸ß16Î»Îª0
			break;
		default:
			break;
	}
	
	return temp;
}

/**********************************************************************************************
* ²ÎÊı£ºÎŞ
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷£ºÈí¼ş¸´Î»
**********************************************************************************************/
void Software_Reset(void)
{
	wr_map[R0] &= 0x0000;//0x0f7f
	wr_map[R0] |= 0x8000;
	Reg_Write(R0, wr_map[0]);
}
/**********************************************************************************************
* ²ÎÊı£ºaxisÎªAXIS_NUMÀàĞÍÃ¶¾Ù
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷£ºÖ¸¶¨Öá
**********************************************************************************************/
void Set_Axis(AXIS_NUM axis)
{
	wr_map[R0] &= 0x007f;
	wr_map[R0] |= axis;
	Reg_Write(R0, wr_map[R0]);
}

//·¶Î§ÉèÖÃ
void Range_Set(AXIS_NUM axis,int data)
{
	Data_Write(WR,data);
}
//¼ÓËÙ¶ÈÔö¼ÓÂÊÉèÖÃ
void INC_Rate(AXIS_NUM axis,int data)
{
	Data_Write(WK,data);
}
//¼õËÙ¶È¼õÉÙÂÊÉèÖÃ
void DEC_Rate(AXIS_NUM axis,int data)
{
	Data_Write(WL,data);
}
//¼ÓËÙ¶ÈÉèÖÃ
void INC_Set(AXIS_NUM axis,int data)
{
	Data_Write(WA,data);
}
//¼õËÙ¶ÈÉèÖÃ
void DEC_Set(AXIS_NUM axis,int data)
{
	Data_Write(WD,data);
}
//Æô¶¯ËÙ¶ÈÉèÖÃ
void Start_Speed(AXIS_NUM axis,int data)
{
	Data_Write(WSV,data);
}
//Çı¶¯ËÙ¶ÈÉèÖÃ
void Drive_Speed(AXIS_NUM axis,int data)
{
	Data_Write(WV,data);
}
//Êä³öÂö³åÊıÉèÖÃ»òÕßÖÕµãÉèÖÃ
void Pulse_Out(AXIS_NUM axis,int data)
{
	Set_Axis(axis);
	Data_Write(WP,data);
}
//¼õËÙµãÉèÖÃ
void DECP_Set(AXIS_NUM axis,int data)
{
	Set_Axis(axis);
	Data_Write(WDP,data);
}
//Ô²ĞÄÉèÖÃ
void Center_Set(AXIS_NUM axis,int data)
{
	Set_Axis(axis);
	Data_Write(WC,data);
}
//Âß¼­Î»ÖÃ¼ÆÊıÆ÷³õÖµÉèÖÃ
void LP_Set(AXIS_NUM axis,int data)
{
	Set_Axis(axis);
	Data_Write(WLP,data);
}
//±àÂëÆ÷¼ÆÊıÆ÷³õÖµÉèÖÃ
void EP_Set(AXIS_NUM axis,int data)
{
	Set_Axis(axis);
	Data_Write(WEP,data);
}
//ÕıÏò±È½Ï¼Ä´æÆ÷ÊıÖµÉèÖÃ
void CompP_Set(AXIS_NUM axis,int data)
{
	Set_Axis(axis);
	Data_Write(WCP,data);
}
//¸ºÏò±È½Ï¼Ä´æÆ÷ÉèÖÃ
void CompM_Set(AXIS_NUM axis,int data)
{
	Set_Axis(axis);
	Data_Write(WCM,data);
}
//¼ÓËÙÆ«ÒÆÉèÖÃ
void ACC_Offset(AXIS_NUM axis,int data)
{
	Set_Axis(axis);
	Data_Write(WAO,data);
}

//À©Õ¹Ä£Ê½ÉèÖÃ£¨Î»Êı¾İ£©
void EXP_Mode(AXIS_NUM axis,int data)
{
	Set_Axis(axis);
	Data_Write(WEM,data);
}
//Ô­µãËÑË÷ËÙ¶È
void Home_SearchSpeed(AXIS_NUM axis,int data)
{
	Set_Axis(axis);
	Data_Write(WHV,data);
}
//Í¬²½Ä£Ê½ÉèÖÃ£¨Î»Êı¾İ£
void SYN_Mode(AXIS_NUM axis,int data)
{
	Set_Axis(axis);
	Data_Write(WSM,data);
}


//Data_Read();

//¶ÁÊµ¼ÊÎ»ÖÃ ¶ÁÈ¡±àÂëÆ÷ÊıÖµ
int Read_REP(AXIS_NUM axis)
{
	int Data;
	Set_Axis(axis);
	Data = Data_Read(REP);
	return Data;
}
//¶ÁÂß¼­Î»ÖÃ ¶ÁÈ¡µ±Ç°Âß¼­Î»ÖÃ¼ÆÊıÆ÷ÊıÖµ
int Read_RLP(AXIS_NUM axis)
{
	int Data;
	Set_Axis(axis);
	Data = Data_Read(RLP);
	return Data;
}
//¶ÁÈ¡µ±Ç°Çı¶¯ËÙ¶È
int Read_RV(AXIS_NUM axis)
{
	int Data;
	Set_Axis(axis);
	Data = Data_Read(RV);
	return Data;
}
//¶Á¼Ó¼õËÙ¶ÈÊıÖµ
int Read_RA(AXIS_NUM axis)
{
	int Data;
	Set_Axis(axis);
	Data = Data_Read(RA);
	return Data;
}
//¶ÁÍ¬²½»º³å¼Ä´æÆ÷
int Read_RSB(AXIS_NUM axis)
{
	int Data;
	Set_Axis(axis);
	Data = Data_Read(RSB);
	return Data;
}


// µÈ´ıÖáÇı¶¯Í£Ö¹
void Wait_Stop(AXIS_NUM axis)
{
	while(Reg_Read(R0) & axis);
}

// Next data setting of waiting for continuous interpolation
//µÈ´ıÁ¬Ğø²å²¹ÏÂÒ»¸öÊı¾İµÄÉèÖÃ RR0/D9
void Next_ITP_Wait(void)
{
	while((Reg_Read(R0) & 0x0200) == 0x0);
}

// Next data setting of waiting for BP interpolation
//µÈ´ıÎ»Ä£Ê½²å²¹ÏÂÒ»¸öÊı¾İÉèÖÃ RR0/D13-D14  SC
void BP_ITP_Wait(void)
{
	while((Reg_Read(R0) & 0x6000) == 0x6000);
}
	

/**********************************************************************************************
* ²ÎÊı£ºÎŞ
* ·µ»ØÖµ£ºÍ£Ö¹µÄÖá´úºÅ
* ËµÃ÷£º·µ»ØÍ£Ö¹Öá´úºÅ 
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
* ²ÎÊı£ºÎŞ
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷£º·µ»Ø³ö´íÖá´úºÅ 
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
* ²ÎÊı£ºit:ÎªIT_NUMÀàĞÍÃ¶¾Ù STATE:Ê¹ÄÜ/Ê§ÄÜ WR1--D8~D15
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷£ºÖáÖĞ¶ÏÅäÖÃ
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
* ²ÎÊı£ºÊ§ÄÜ/Ê¹ÄÜ WR5--D14
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷£ºÁ¬Ğø²å²¹ÖĞ¶ÏÅäÖÃ
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
* ²ÎÊı£ºÎ»Ä£Ê½²å²¹ÖĞ¶Ï Ê§ÄÜ/Ê¹ÄÜ WR5--D15
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷£ºÎ»Ä£Ê½²å²¹ÖĞ¶ÏÅäÖÃ
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
* ²ÎÊı£º»ØÔ­µãÖĞ¶Ï Ê¹ÄÜENB   Ê¹ÄÜDIS
* ·µ»ØÖµ£º
* ËµÃ÷£º»ØÔ­µãÖĞ¶ÏÊ¹ÄÜ/Ê§ÄÜ
**********************************************************************************************/
//»ØÔ­µãÖĞ¶Ï ÔÚÍâ²¿Ä£Ê½ÖĞ60hÉèÖÃ   WR6/D5(HMINT)   ¶ÁÖĞ¶ÏRR3/D8(HMEND)  
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
* ²ÎÊı£ºÍ¬²½ÔË¶¯ÖĞ¶Ï Ê¹ÄÜENB   Ê¹ÄÜDIS
* ·µ»ØÖµ£º
* ËµÃ÷£ºÍ¬²½ÔË¶¯ÖĞ¶ÏÊ¹ÄÜ/Ê§ÄÜ
**********************************************************************************************/
//Í¬²½ÔË¶¯ÖĞ¶Ï Í¬²½Ä£Ê½ÅäÖÃ64h   WR7/D15 (INT)    ¶ÁÖĞ¶ÏRR3/D9(SYNC)
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
* ²ÎÊı£ºÎŞ
* ·µ»ØÖµ£º1ÖĞ¶Ï·¢Éú 0ÖĞ¶ÏÃ»·¢Éú
* ËµÃ÷£º»ñÈ¡ÖĞ¶ÏÔ
**********************************************************************************************/
unsigned char IT_ITP_Status(void)
{
	unsigned short temp;
	temp = ((Reg_Read(R0)>>9)&0x0001);
	return (unsigned char) temp;
}

/**********************************************************************************************
* ²ÎÊı£º
* ·µ»ØÖµ£ºÖĞ¶Ï×´Ì¬
		  1   ÖĞ¶Ï²úÉú
		  2/3 ÖĞ¶ÏÃ»²úÉú
* ËµÃ÷£ºÎ»Ä£Ê½²å²¹ÖĞ¶Ï×´Ì¬ SCÓÉ2 ±äÎª1 
**********************************************************************************************/
//Î»Ä£Ê½²å²¹ÖĞ¶Ï×´Ì¬ RR0/D14-D13  //(SC) is changed from 2to 1, and the stack is available for next BP command writing...
unsigned char IT_BP_Status(void)
{
	unsigned short temp;
	temp = ((Reg_Read(R0)>>13)&0x0003);
	return (unsigned char) temp;
}

/**********************************************************************************************
* ²ÎÊı£º
* ·µ»ØÖµ£ºÖĞ¶Ï×´Ì¬
		  0 ÖĞ¶ÏÃ»²úÉú
		  1 ÖĞ¶Ï²úÉú
* ËµÃ÷£º»ØÔ­µãÖĞ¶Ï×´Ì¬ »ØÔ­µã½áÊøÖĞ¶Ï
**********************************************************************************************/
//»ØÔ­µãÖĞ¶Ï×´Ì¬ RR0/D14-D13  »ØÔ­µã½áÊøÊÇ·ñÓĞÖĞ¶Ï²úÉú
unsigned char IT_Home_Status(void)
{
	unsigned short temp;
	temp = ((Reg_Read(R3)>>8)&0x0001);
	return (unsigned char) temp;
}

/**********************************************************************************************
* ²ÎÊı£º
* ·µ»ØÖµ£ºÖĞ¶Ï×´Ì¬
		  0 ÖĞ¶ÏÃ»²úÉú
		  1 ÖĞ¶Ï²úÉú
* ËµÃ÷£º»ñÈ¡Í¬²½ÔË¶¯ÖĞ¶Ï×´Ì¬Ô
**********************************************************************************************/
//Í¬²½ÔË¶¯ÖĞ¶Ï×´Ì¬ RR0/D14-D13 
unsigned char IT_SYN_Status(void)
{
	unsigned short temp;
	temp = ((Reg_Read(R3)>>9)&0x0001);
	return (unsigned char) temp;
}
/**********************************************************************************************
* ²ÎÊı£ºitÎªIT_NUMÀàĞÍÃ¶¾Ù
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷£º»ñÈ¡ÖĞ¶ÏÔ
**********************************************************************************************/
IT_MASK IT_Axis_Status(void)
{
	IT_MASK temp;
	
	temp = (IT_MASK)(Reg_Read(R3) & IT_ALL_MASK);

	switch(temp)
	{
		case IT_PULSE_MASK:		//0x0001,Âö³åÉÏÉıÑØ
			temp = IT_PULSE_MASK;
		break;
		case IT_LMC_MASK:		//0x0002,Âß¼­/±àÂëÆ÷¼ÆÊıÆ÷´óÓÚ¸º·½Ïò±È½Ï¼Ä´æÆ÷ÊıÖµ
			temp = IT_LMC_MASK;
		break;
		case IT_SMC_MASK:		//0x0004,Âß¼­/±àÂëÆ÷¼ÆÊıÆ÷Ğ¡ÓÚ¸º·½Ïò±È½Ï¼Ä´æÆ÷ÊıÖµ
			temp = IT_SMC_MASK;
		break;
		case IT_SPC_MASK:		//0x0008,Âß¼­/±àÂëÆ÷¼ÆÊıÆ÷Ğ¡ÓÚÕı·½Ïò±È½Ï¼Ä´æÆ÷ÊıÖµ
			temp = IT_SPC_MASK;
		break;
		case IT_LPC_MASK:		//0x0010,Âß¼­/±àÂëÆ÷¼ÆÊıÆ÷´óÓÚÕı·½Ïò±È½Ï¼Ä´æÆ÷ÊıÖµ
			temp = IT_LPC_MASK;
		break;
		case IT_CEND_MASK:		//0x0020,¼Ó¼õËÙÇı¶¯Ê±£¬¶¨ËÙÓò½áÊøÂö³åÊä³ö
			temp = IT_CEND_MASK;
		break;
		case IT_CSTA_MASK:		//0x0040,¼Ó¼õËÙÇı¶¯Ê±£¬¶¨ËÙÓò¿ªÊ¼Âö³åÊä³ö
			temp = IT_CSTA_MASK;
		break;
		case IT_DEND_MASK:		//0x0080,Çı¶¯½áÊø
			temp = IT_DEND_MASK;
		break;
		case IT_HOME_MASK:		//0x0100,Ô­µãËÑË÷
			temp = IT_HOME_MASK;
		break;
		case IT_SYNC_MASK:		//0x0200,Í¬²½¼¤»î
			temp = IT_SYNC_MASK;
		break;
		default:
			temp = IT_ERR_MASK;
		break;
	}
	
	return temp;
}
/**********************************************************************************************
* ²ÎÊı£ºÎŞ
* ·µ»ØÖµ£º³ö´íµÄ×´Ì¬Ô´
* ËµÃ÷£º»ñÈ¡´íÎóÔ´
**********************************************************************************************/
//»ñÈ¡´íÎóÔ´
ERR_MASK Error_Is(void)
{
	ERR_MASK temp;
	
	temp = (ERR_MASK)((Reg_Read(R2) & ERR_ALL_MASK));

	switch(temp)
	{
		case ERR_LPC_MASK:		//0x0001,Âß¼­/±àÂëÆ÷¼ÆÊıÆ÷´óÓÚÕı·½Ïò±È½Ï¼Ä´æÆ÷ÊıÖµ
			temp = ERR_LPC_MASK;
		break;
		case ERR_SMC_MASK:		//0x0002,Âß¼­/±àÂëÆ÷¼ÆÊıÆ÷´óÓÚ¸º·½Ïò±È½Ï¼Ä´æÆ÷ÊıÖµ
			temp = ERR_SMC_MASK;
		break;
		case ERR_PL_MASK:		//0x0004,Ó²¼şÕıÏŞÎ»
			temp = ERR_PL_MASK;
		break;
		case ERR_ML_MASK:		//0x0008,Ó²¼ş¸ºÏŞÎ»
			temp = ERR_ML_MASK;
		break;
		case ERR_ALARM_MASK:	//0x0010,ËÅ·ş±¨¾¯
			temp = ERR_ALARM_MASK;
		break;
		case ERR_EMG_MASK:		//¼±Í£
			temp = ERR_EMG_MASK;
		break;
		default:
			temp = ERR_ERR_MASK;
		break;
	}

	return temp;
}


/**********************************************************************************************
* ²ÎÊı£ºEncoder_InitStructÎªENCODER_InitTypeDefÀàĞÍÃ¶¾Ù
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷£ºÓÃEncoder_InitStructÖĞÉèÖÃµÄ²ÎÊı³õÊ¼»¯±àÂëÆ÷
**********************************************************************************************/
void Encoder_Init(Encoder_InitTypeDef Encoder_InitStruct)
{
	wr_map[R2] &= 0xf1ff;
	wr_map[R2] |= (Encoder_InitStruct.Pulse_Mode << 9)
					|(Encoder_InitStruct.Pulse_Mul);
	Reg_Write(R2, wr_map[R2]);
}

/**********************************************************************************************
* ²ÎÊı£ºCounter_ValueÎª±àÂëÆ÷¼Ä´æÆ÷ÉèÖÃÖµ
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷£ºÉèÖÃ±àÂëÆ÷¼ÆÊıÆ÷³õÖµ
**********************************************************************************************/
void Encoder_SetCounter(int Counter_Value)
{
	Data_Write(WEP, Counter_Value);
}

/**********************************************************************************************
* Ãû³Æ£ºint Encoder_GetCounter()
* ²ÎÊı£ºÎŞ
* ·µ»ØÖµ£º±àÂëÆ÷¼ÆÊıÖµ
* ËµÃ÷£º»ñÈ¡±àÂëÆ÷¼ÆÊıÆ÷³õÖµ
**********************************************************************************************/
int Encoder_GetCounter()
{
	return Data_Read(REP);
}

/**********************************************************************************************
* ²ÎÊı£ºComp_InitStructÎªComp_InitTypeDefÀàĞÍÃ¶¾Ù£¬
		ÆäÖĞ¶¨Òå±È½Ï¼Ä´æÆ÷ĞÅÏ¢
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷£ºÓÃComp_InitStructÖĞÉèÖÃµÄ²ÎÊı³õÊ¼»¯±È½ÏÆ÷
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
* ²ÎÊı£ºÊı×éÖ¸Õë
		Data0      ÃüÁîÂë	
		Data1      Ñ¡Öá 01-XÖá 02-YÖá 04-ZÖá  08-UÖá
		Data2      ÕıÏò±È½Ï¼Ä´æÆ÷×´Ì¬ STATE£º0Ê¹ÄÜ 1Ê§ÄÜ
		Data3      ¸ºÏò±È½Ï¼Ä´æÆ÷×´Ì¬ STATE£º0Ê¹ÄÜ 1Ê§ÄÜ
		Data4      0x00:±È½Ï¶ÔÏóLG_CNT = 0x0000Âß¼­¼ÆÊıÆ÷   0x20:EC_CNT = 0x0020±àÂëÆ÷¼ÆÊıÆ÷	  	
		Data5~8    ÕıÏò±È½Ï¼Ä´æÆ÷³õÖµ
		Data9~12   ¸ºÏò±È½Ï¼Ä´æÆ÷³õÖµ	
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷   ±È½Ï¼Ä´æÆ÷ÅäÖÃ 1Öá 2ÕıÏò±È½Ï¼Ä´æÆ÷×´Ì¬ 3¸ºÏò±È½Ï¼Ä´æÆ÷×´Ì¬ 4±È½Ï¶ÔÏó 5~8ÕıÏò±È½Ï¼Ä´æÆ÷³õÖµ 9~12¸ºÏò±È½Ï¼Ä´æÆ÷³õÖµ	
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
	Set_Axis((AXIS_NUM)(Data[1]<<8));//Ñ¡Öá
//Data 0ÃüÁîÂë 1Öá 2ÕıÏò±È½Ï¼Ä´æÆ÷×´Ì¬ 3¸ºÏò±È½Ï¼Ä´æÆ÷×´Ì¬ 4±È½Ï¶ÔÏó 5~8ÕıÏò±È½Ï¼Ä´æÆ÷³õÖµ 9~12¸ºÏò±È½Ï¼Ä´æÆ÷³õÖµ	
	Comp_InitStruct.PComp_State = (STATE)Data[2];
	Comp_InitStruct.MComp_State = (STATE)Data[3];
	Comp_InitStruct.Comp_Obj = (OBJ)Data[4];
	Comp_InitStruct.PComp_Value = (Data[5]<<24) | (Data[6]<<16) | (Data[7]<<8) | (Data[8]);
	Comp_InitStruct.MComp_Value = (Data[9]<<24) | (Data[10]<<16) | (Data[11]<<8) | (Data[12]);
	Comp_Init(Comp_InitStruct);	
}
/**********************************************************************************************
* ²ÎÊı£ºAXIS_InitStructÎªAXIS_InitTypeDefÀàĞÍÃ¶¾Ù£¬
		ÆäÖĞ¶¨ÒåÓ²¼şÏŞÎ»¡¢Âö³åÄ£Ê½ĞÅÏ¢
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷£ºÓÃAXIS_InitStuctÖĞÉèÖÃµÄ²ÎÊı³õÊ¼»¯Çı¶¯Öá
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
* ²ÎÊı£ºÊı×éÖ¸Õë
		Data0   ÃüÁîÂë	
		Data1   Ñ¡Öá 01-XÖá 02-YÖá 04-ZÖá  08-UÖá
		Data2   Í£Ö¹Ä£Ê½  0Á¢¼´Í£Ö¹ 1¼õËÙÍ£Ö¹ 
		Data3   ÕıÏŞÎ»Âß¼­µçÆ½ 0µÍ  1¸ß
		Data4   ¸ºÏŞÎ»Âß¼­µçÆ½ 0µÍ  1¸ß
		Data5   Âö³åÄ£Ê½  0Ë«Âö³åÄ£Ê½ 1·½Ïò-Âö³åÄ£Ê½
		Data6   Êä³öÂö³åÂß¼­µçÆ½ 0µÍ  1¸ß
		Data7   Êä³öÂö³å·½ÏòµçÆ½ 0µÍ  1¸ß	
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷  ÖáÄ£Ê½ÉèÖÃ 1Öá 2Í£Ö¹Ä£Ê½ 3ÕıÏŞÎ»Âß¼­µçÆ½ 4¸ºÏŞÎ»Âß¼­µçÆ½ 5Âö³åÄ£Ê½  6Êä³öÂö³åÂß¼­µçÆ½ 7Êä³öÂö³å·½ÏòµçÆ½
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
	Set_Axis((AXIS_NUM)(Data[1]<<8));//Ñ¡Öá
//Data 0ÃüÁîÂë 1Öá 2Í£Ö¹Ä£Ê½ 3ÕıÏŞÎ»Âß¼­µçÆ½ 4¸ºÏŞÎ»Âß¼­µçÆ½ 5Âö³åÄ£Ê½  6Êä³öÂö³åÂß¼­µçÆ½ 7Êä³öÂö³å·½ÏòµçÆ½
	AXIS_InitStruct.Stop_Mode = (STOP_MODE)Data[2];
	AXIS_InitStruct.PLimit_Level = (LEVEL)Data[3];
	AXIS_InitStruct.MLimit_Level = (LEVEL)Data[4];
	AXIS_InitStruct.Pulse_Mode = (PULSE_MODE)Data[5];
	AXIS_InitStruct.Logic_Level = (LEVEL)Data[6];
	AXIS_InitStruct.Dir_Level = (LEVEL)Data[7];
	AXIS_Init(AXIS_InitStruct);
}

/**********************************************************************************************
* ²ÎÊı£ºDrive_InitStructÎªDrive_InitTypeDefÀàĞÍ½á¹¹Ìå
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷£ºÓÃ½á¹¹ÌåDrive_InitTypeDefÖĞ¶¨ÒåµÄÖµ³õÊ¼»¯Ö¸¶¨µÄÖá
**********************************************************************************************/
void Drive_Init(Drive_InitTypeDef Drive_InitStruct)
{
	//ËÙ¶È±ä»¯ÂÊÉèÖÃ
	if(Drive_InitStruct.Range)
		Data_Write(WR, Drive_InitStruct.Range);
	
	//¼ÓËÙ¶ÈÉèÖÃ
	if(Drive_InitStruct.Acce)
		Data_Write(WA, Drive_InitStruct.Acce);
	
	//¼õËÙ¶ÈÉèÖÃ
	if(Drive_InitStruct.Dece)
	{
		wr_map[R3] &= 0x0f9d;
		wr_map[R3] |= 0x0002;
		Reg_Write(R3, wr_map[R3]);
		Data_Write(WD, Drive_InitStruct.Dece);
	}
	//¼õËÙ¶ÈÊ¹ÓÃ¼ÓËÙ¶ÈÖµ
	else
	{
		wr_map[R3] &= 0x0f9d;
		Reg_Write(R3, wr_map[R3]);
	}
	
	//SÇúÏß¼Ó¼õËÙ±ä»¯ÂÊÉèÖÃ
	if(Drive_InitStruct.Acce_Rate || Drive_InitStruct.Dece_Rate)
	{
		wr_map[R3] &= 0x0f9b;
		wr_map[R3] |= 0x0004;
		Reg_Write(R3, wr_map[R3]);
		Data_Write(WK, Drive_InitStruct.Acce_Rate);
		Data_Write(WL, Drive_InitStruct.Dece_Rate);
	}
	//Ö±Ïß¼Ó¼õËÙÉèÖÃ
	else
	{
		wr_map[R3] &= 0x0f9b;
		Reg_Write(R3, wr_map[R3]);
	}
	
	//ÉèÖÃ¼ÓËÙÆ«ÒÆÖµ£¬Ä¬ÈÏÎª8£¬ÔÚµÍÆô¶¯ËÙ¶ÈµÄ¹Ì¶¨Âö³åÇı¶¯Ê±¸ÃÖµÓ¦ÉèÎª0
	Data_Write(WAO, Drive_InitStruct.Acce_Offset);
	
	//³õÊ¼ËÙ¶ÈÉèÖÃ
	if(Drive_InitStruct.Init_Speed)
		Data_Write(WSV, Drive_InitStruct.Init_Speed);
	
	//Çı¶¯ËÙ¶ÈÉèÖÃ
	if(Drive_InitStruct.Speed)
		Data_Write(WV, Drive_InitStruct.Speed);
	
	//ÊÖ¶¯¼õËÙµãÉèÖÃ£¬Man_Dece_PosÎª0±íÊ¾ÎŞÊÖ¶¯¼õËÙµã
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
* ²ÎÊı£ºÊı×éÖ¸Õë
		Data0       ÃüÁîÂë	
		Data1       Ñ¡Öá 01-XÖá 02-YÖá 04-ZÖá  08-UÖá
		Data2~5      R     ±¶ÂÊ Multiple=8000000/R
		Data6~7      A     ¼ÓËÙ¶ÈACC=A*25*Multiple
		Data8~9      D     ¼õËÙ¶ÈDEC=D*125*Multiple
		Data10~11    K     ¼ÓËÙ¶ÈÂÊJerk=62.5*10^6/K*Multiple
		Data12~13    K     ¼õËÙ¶ÈÂÊJerk=62.5*10^6/K*Multiple
		Data14~15    SV    ³õÊ¼ËÙ¶ÈInit_Speed=SV*Multiple 
		Data16~17    V     Çı¶¯ËÙ¶ÈDrive_Speed=V*Multiple 
		Data18~19	¼ÓËÙ¶ÈÆ«ÒÆ
		Data20~23	ÊÖ¶¯¼õËÙµã		
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷£ºAxis ±¶ÂÊ ³õÊ¼/Çı¶¯ËÙ¶È  ¼Ó/¼õËÙ¶È ¼Ó/¼õËÙ¶ÈÂÊ ¼ÓËÙ¶ÈÆ«ÒÆ ÊÖ¶¯¼õËÙµã²ÎÊıÉèÖÃ 
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
	Set_Axis((AXIS_NUM)(Data[1]<<8));//Ñ¡Öá
	//  Data 0ÃüÁîÂë 1Öá 2~5±¶ÂÊ 6~7¼ÓËÙ¶È 8~9¼õËÙ¶È 10~11¼ÓËÙ¶ÈÂÊ 12~13¼õËÙ¶ÈÂÊ 14~15³õÊ¼ËÙ¶È 16~17Çı¶¯ËÙ¶È 18~19¼ÓËÙ¶ÈÆ«ÒÆ 20~23ÊÖ¶¯¼õËÙµã
//	Drive_InitStruct.Range = ((Data[2]<<24) | (Data[3]<<16) | (Data[4]<<8) | (Data[5]));   //R   ·¶Î§R  16000-8000000  4bytes  ±¶ÂÊ Multiple=8000000/R
//	Drive_InitStruct.Acce = (Data[6]<<8) | Data[7];              //A     ¼ÓËÙ¶ÈA     1-8000       2bytes  ACC=A*25*Multiple
//	Drive_InitStruct.Dece = (Data[8]<<8) | Data[9];              //D     ¼õËÙ¶ÈD     1-8000       2bytes  DEC=D*125*Multiple
//	Drive_InitStruct.Acce_Rate = (Data[10]<<8) | Data[11];       //K     ¼ÓËÙ¶ÈÂÊK   1-65535      2bytes  Jerk=62.5*10^6/K*Multiple
//	Drive_InitStruct.Dece_Rate = (Data[12]<<8) | Data[13];       //K     ¼õËÙ¶ÈÂÊK   1-65535      2bytes  Jerk=62.5*10^6/K*Multiple
//	Drive_InitStruct.Init_Speed = (Data[14]<<8)| Data[15];       //SV    ³õÊ¼ËÙ¶ÈSV  1-8000       2bytes  Init_Speed=SV*Multiple 
//	Drive_InitStruct.Speed = (Data[16]<<8) | Data[17];           //V     Çı¶¯ËÙ¶ÈV   1-8000       2bytes  Drive_Speed=V*Multiple 
//	Drive_InitStruct.Acce_Offset = (Data[18]<<8) | Data[19];     //¼ÓËÙ¶ÈÆ«ÒÆ-32768-+32768        2bytes
//	Drive_InitStruct.Man_Dece_Pos = (Data[20]<<24) | (Data[21]<<16) | (Data[22]<<8) | (Data[23]);
															  // ÊÖ¶¯¼õËÙµã 0-4294967295 4bytes
	Drive_InitStruct.Range = 8000000;   //R   ·¶Î§R  16000-8000000  4bytes  ±¶ÂÊ Multiple=8000000/R
	Drive_InitStruct.Acce = 1;              //A     ¼ÓËÙ¶ÈA     1-8000       2bytes  ACC=A*25*Multiple
	Drive_InitStruct.Dece = 1;              //D     ¼õËÙ¶ÈD     1-8000       2bytes  DEC=D*125*Multiple
	Drive_InitStruct.Acce_Rate = 10;       //K     ¼ÓËÙ¶ÈÂÊK   1-65535      2bytes  Jerk=62.5*10^6/K*Multiple
	Drive_InitStruct.Dece_Rate = 10;       //K     ¼õËÙ¶ÈÂÊK   1-65535      2bytes  Jerk=62.5*10^6/K*Multiple
	Drive_InitStruct.Init_Speed = 0;       //SV    ³õÊ¼ËÙ¶ÈSV  1-8000       2bytes  Init_Speed=SV*Multiple 
	Drive_InitStruct.Speed = (Data[2]<<8) | Data[3];           //V     Çı¶¯ËÙ¶ÈV   1-8000       2bytes  Drive_Speed=V*Multiple 
	Drive_InitStruct.Acce_Offset = 0;     //¼ÓËÙ¶ÈÆ«ÒÆ-32768-+32768        2bytes
	Drive_InitStruct.Man_Dece_Pos =0;
															  // ÊÖ¶¯¼õËÙµã 0-4294967295 4bytes

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
* ²ÎÊı  Êı×éÖ¸Õë£
 		DData0  ÃüÁîÂë	
		Data1  Ñ¡ÖáÇı¶¯µÄÖá 01-XÖá 02-YÖá 04-ZÖá  08-UÖá		
		Data2  8 ¹Ì¶¨ËÙ¶È 
			   0 ¶Ô³ÆÏßĞÔ¼Ó¼õËÙ¶ÈÂÊ,   ×Ô¶¯¼õËÙ £¨ÌİĞÎ£© 
			   1 ¶Ô³ÆÏßĞÔ¼Ó¼õËÙ¶ÈÂÊ£¬  ÊÖ¶¯¼õËÙ £¨ÌİĞÎ£© 
			   2 ·Ç¶Ô³ÆÏßĞÔ¼Ó¼õËÙ¶ÈÂÊ£¬×Ô¶¯¼õËÙ £¨ÌİĞÎ£© 
			   3 ·Ç¶Ô³ÆÏßĞÔ¼Ó¼õËÙ¶ÈÂÊ£¬ÊÖ¶¯¼õËÙ £¨ÌİĞÎ£© 
			   4 ¶Ô³ÆSĞÎ¼Ó¼õËÙ¶ÈÂÊ£¬   ×Ô¶¯¼õËÙ £¨SĞÎ£© 
			   5 ¶Ô³ÆSĞÎ¼Ó¼õËÙ¶ÈÂÊ£¬   ÊÖ¶¯¼õËÙ £¨SĞÎ£© 
			   6 ·Ç¶Ô³ÆSĞÎ¼Ó¼õËÙ¶ÈÂÊ£¬ ×Ô¶¯¼õËÙ £¨SĞÎ£©
			   7 ·Ç¶Ô³ÆSĞÎ¼Ó¼õËÙ¶ÈÂÊ£¬ ÊÖ¶¯¼õËÙ £¨SĞÎ£©
		Data3  PFD(0x20)»òMFD(0x21)£¬PFDÕıÏò¹Ì¶¨Âö³åÇı¶¯£¬MFD·´Ïò¹Ì¶¨Âö³åÇı¶¯
		Data4~7 ÎªÂö³åÊı 
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷£ºÈÃÖ¸¶¨ÖáÔË¶¯mode·½Ïò,Çı¶¯Ä£Ê½£¬pulseÊıÁ¿µÄÂö³å
**********************************************************************************************/
void Axis_Fixed_Drive(unsigned char *temp)
{
	unsigned char i;
	unsigned char Data[8];
	for(i=0;i<8;i++)
	{
		Data[i] = *temp++;
	}
	Set_Axis((AXIS_NUM)(Data[1]<<8));//Data[1]Ñ¡Öá
//Data 0ÃüÁîÂë 1Öá 2Çı¶¯Ä£Ê½ 3Çı¶¯ÀàĞÍ 4~7Âö³åÊı
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
		CMD_Write((CMD_NUM)Data[3]);//¹Ì¶¨Âö³åÊä³öÃüÁî		
	}
}

/**********************************************************************************************
* ²ÎÊı£ºÊı×éÖ¸Õë
		Data0  ÃüÁîÂë	
		Data1  Ñ¡Öá 01-XÖá 02-YÖá 04-ZÖá  08-UÖá
		Data2  ¿ÉÈ¡PCD(0x22)»òMCD(0x23)£¬PCDÕıÏòÁ¬ĞøÇı¶¯£¬MCD·´ÏòÁ¬ĞøÇı¶¯
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷£ºÆô¶¯Ö¸¶¨ÖáÖ¸¶¨·½ÏòÁ¬ĞøÇı¶¯
**********************************************************************************************/
void Axis_Continuous_Drive(unsigned char *temp)
{
	unsigned char i;
	unsigned char Data[3];
	for(i=0;i<3;i++)
	{
		Data[i] = *temp++;
	}
	Set_Axis((AXIS_NUM)(Data[1]<<8)); //Data[1]Ñ¡Öá
	CMD_Write((CMD_NUM)Data[2]);     //Ğ´Á¬ĞøÇı¶¯ÃüÁî
}
/**********************************************************************************************
* ²ÎÊı£ºÊı×éÖ¸Õë
		Data0   ÃüÁîÂë	
		Data1   Ñ¡Öá 01-XÖá 02-YÖá 04-ZÖá  08-UÖá
		Data2~5 ÅµÎª¶¨³¤ÔË¶¯ÔòÎª¶¨³¤ÔË¶¯Êä³öµÄÂö³åÊı£¬ÈçÁ¬ĞøÊä³öÔò¿ÉÒÔ²»¹Ü
		Data6   00  external signals disabled
				01  continuous driving mode
				02  fixed pulse driving mode
				03  MPG mode
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷£ºÖ¸¶¨ÖáÍâ²¿ĞÅºÅÇı¶¯
**********************************************************************************************/
void External_Signals_Drive(unsigned char *temp)
{
	unsigned char i;
	unsigned char Data[7];
	for(i=0;i<7;i++)
	{
		Data[i] = *temp++;
	}
	Set_Axis((AXIS_NUM)(Data[1]<<8)); //Data[1]Ñ¡Öá
	Data_Write(WEM,0x00005f00);//Íâ²¿ĞÅºÅÂË²¨
	Data_Write(WP, (Data[2]<<24) | (Data[3]<<16) | (Data[4]<<8) | (Data[5]));
	wr_map[R3] &= 0x0f87;
	wr_map[R3] |= (Data[6]<<3);
	Reg_Write(R3,wr_map[R3]);	
}
//Ê§ÄÜÍâ²¿ĞÅºÅÇı¶¯
void Clear_External_Drive(void)
{
	wr_map[R3] &= 0x0f87;
	Reg_Write(R3,wr_map[R3]);
}	

/**********************************************************************************************
* ²ÎÊı: Êı×éÖ¸Õë
		Data0  ÃüÁîÂë	
		Data1  Ñ¡Öá  01-XÖá 02-YÖá 04-ZÖá  08-UÖá    
        Data2  Í£Ö¹Ä£Ê½
				0x26 DECS¼õËÙÍ£Ö¹£
				0x27 SUDSÁ¢¼´Í£Ö¹
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷£ºÍ£Ö¹Ö¸¶¨µÄÖáÔË¶¯
**********************************************************************************************/
void AXIS_Stop(unsigned char *temp)
{
	unsigned char i;
	unsigned char Data[3];
	for(i=0;i<3;i++)
	{
		Data[i] = *temp++;
	}
	Set_Axis((AXIS_NUM)(Data[1]<<8)); //Data[1]Ñ¡Öá
	CMD_Write((CMD_NUM)Data[2]);
}
/**********************************************************************************************
* ²ÎÊı: Êı×éÖ¸Õë
		Data0  ÃüÁîÂë	
		Data1  Ñ¡Öá 01-XÖá 02-YÖá 04-ZÖá  08-UÖá     
        Data2  ËÅ·şÄ£Ê½  D0 nALARM 0:Low/1:Hi  
						 D1 nALARM 0:Disable/1:Enable 
						 D3 nINPOS 0:Low/1:Hi
						 D4 nINPOS 0:Disable/1:Enable 
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷£ºËÅ·şÄ£Ê½ÅäÖÃ
**********************************************************************************************/
void Servo_Mode_Config(unsigned char *temp)
{
	unsigned char i;
	unsigned char Data[3];
	for(i=0;i<3;i++)
	{
		Data[i] = *temp++;
	}
	Set_Axis((AXIS_NUM)(Data[1]<<8)); //Data[1]Ñ¡Öá
	wr_map[R2] &= 0x0fff;
	wr_map[R2] |= (Data[2]<<12);
	Reg_Write(R2, wr_map[R2]);
}
/**********************************************************************************************
* ²ÎÊı: Êı×éÖ¸Õë
		Data0  ÃüÁîÂë	
		Data1  Ñ¡Öá 01-XÖá 02-YÖá 04-ZÖá  08-UÖá     
        Data2  1Ê¹ÄÜÕı·½ÏòÈí¼şÏŞÎ»   2Ê¹ÄÜ¸º·½ÏòÈí¼şÏŞÎ»
		Data3  0ÓëÂß¼­Î»ÖÃ¼ÆÊıÆ÷±È½Ï 1ÓëÊµ¼ÊÎ»ÖÃ¼ÆÊıÆ÷±È½Ï
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷£ºÈíÏŞÎ»Ê¹ÄÜ
**********************************************************************************************/
void Software_Limit(unsigned char *temp)
{	
	unsigned char i;
	unsigned char Data[8];
	for(i=0;i<8;i++)
	{
		Data[i] = *temp++;
	}
	Set_Axis((AXIS_NUM)(Data[1]<<8)); //Data[1]Ñ¡Öá
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
* ²ÎÊı£º ÎŞ
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷£ºÍâ²¿ĞÅºÅÇå³ıÊµ¼ÊÎ»ÖÃ  »ØÔ­µãËÑË÷ Z-phase search  IN2
**********************************************************************************************/
//À©Õ¹Ä£Ê½ÉèÖÃÖĞ  Íâ²¿ĞÅºÅÇå³ıÊµ¼ÊÎ»ÖÃ  »ØÔ­µãËÑË÷ Z-phase search  IN2
void EXT_CLR_RLP(void)
{
	wr_map[R1] &= 0xffcf;
	wr_map[R1] |= 0x0030;// Ê¹ÄÜIN2 WR1/D5    active level WR1/D4 IN2ÎªHi 
	Reg_Write(R1,wr_map[R1]);
	
	wr_map[R6] &= 0xfffe;
	wr_map[R6] |= 0x0001;
	Reg_Write(R6,wr_map[R6]);
	Data_Write(WEM,0x5f00);
}


/**********************************************************************************************
* ²ÎÊı£ºÊı×éÖµ
* ·µ»ØÖµ£º
* ËµÃ÷£º´¦Àí²å²¹µÄ¸÷ÖáÖáºÅ£¨Öá×ª»»£©
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
* ²ÎÊı£ºÊı×éÖ¸Õë
		Data0    ÃüÁîÂë
		Data1    Öá1 01-XÖá 02-YÖá 04-ZÖá  08-UÖá
		Data2    Öá2 01-XÖá 02-YÖá 04-ZÖá  08-UÖá
		Data3~6  Öá1Âö³åÊı
		Data7~10 Öá2Âö³åÊı
		Data11   3B-²å²¹¼õËÙÓĞĞ§ 3C-²å²¹¼õËÙÎŞĞ§
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷£ºÁ½ÖáÖ±Ïß²å²¹
**********************************************************************************************/
void ITP_2AxisLine(unsigned char *temp)
{
	unsigned char i;
	unsigned char Data[12];
	for(i=0;i<12;i++)
	{
		Data[i] = *temp++;
	}
	//Data 0ÃüÁîÂë  1Öá1  2Öá2  3~6Öá1Âö³åÊı 7~10Öá2Âö³åÊı 11²å²¹¼õËÙÓĞĞ§·ñ
	wr_map[R5] &= 0xdb00;
	wr_map[R5] |= ITP_Axis(Data[1])
					|(ITP_Axis(Data[2]) << 2);					
	Reg_Write(R5, wr_map[R5]);
	
	Set_Axis((AXIS_NUM)(Data[1]<<8)); //Data[1]Ñ¡Öá1
	Data_Write(WP, (Data[3]<<24) | (Data[4]<<16) | (Data[5]<<8) | (Data[6]));
	
	Set_Axis((AXIS_NUM)(Data[2]<<8)); //Data[2]Ñ¡Öá2
	Data_Write(WP, (Data[7]<<24) | (Data[8]<<16) | (Data[9]<<8) | (Data[10]));
	
	CMD_Write((CMD_NUM)Data[11]);
	CMD_Write(I2L);
}



/**********************************************************************************************
* ²ÎÊı£ºÊı×éÖ¸Õë
		Data0     ÃüÁîÂë
		Data1     Öá1 01-XÖá 02-YÖá 04-ZÖá  08-UÖá
		Data2     Öá2 01-XÖá 02-YÖá 04-ZÖá  08-UÖá
		Data3     Öá3 01-XÖá 02-YÖá 04-ZÖá  08-UÖá
		Data4~7   Öá1Âö³åÊı
		Data8~11  Öá2Âö³åÊı
		Data12~15 Öá2Âö³åÊı
		Data16   3B-²å²¹¼õËÙÓĞĞ§ 3C-²å²¹¼õËÙÎŞĞ§
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷£ºÈıÖáÖ±Ïß²å²¹
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
	
	Set_Axis((AXIS_NUM)(Data[1]<<8)); //Data[1]Ñ¡Öá1
	Data_Write(WP, (Data[4]<<24) | (Data[5]<<16) | (Data[6]<<8) | (Data[7]));
	
	Set_Axis((AXIS_NUM)(Data[2]<<8)); //Data[2]Ñ¡Öá2
	Data_Write(WP, (Data[8]<<24) | (Data[9]<<16) | (Data[10]<<8) | (Data[11]));
	
	Set_Axis((AXIS_NUM)(Data[3]<<8)); //Data[3]Ñ¡Öá3
	Data_Write(WP, (Data[12]<<24) | (Data[13]<<16) | (Data[14]<<8) | (Data[15]));
	
	CMD_Write((CMD_NUM)Data[16]);
	CMD_Write(I3L);
}

/**********************************************************************************************
* ²ÎÊı£ºÊı×éÖ¸Õë
		Data0      ÃüÁîÂë
		Data1      Öá1 01-XÖá 02-YÖá 04-ZÖá  08-UÖá
		Data2      Öá2 01-XÖá 02-YÖá 04-ZÖá  08-UÖá
		Data3~6    Öá1Ô²ĞÄ
		Data7~10   Öá2Ô²ĞÄ
		Data11~14  Öá1Âö³åÊı
		Data15~18  Öá2Âö³åÊı
		Data19     3B-²å²¹¼õËÙÓĞĞ§ 3C-²å²¹¼õËÙÎŞĞ§
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷£ºÁ½ÖáË³Ê±Õë·½ÏòÔ²»¡²å²¹
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
	
	Set_Axis((AXIS_NUM)(Data[1]<<8)); //Data[1]Ñ¡Öá1
	Data_Write(WC, (Data[3]<<24) | (Data[4]<<16) | (Data[5]<<8) | (Data[6]));
	Data_Write(WP, (Data[11]<<24) | (Data[12]<<16) | (Data[13]<<8) | (Data[14]));
	
	Set_Axis((AXIS_NUM)(Data[1]<<8)); //Data[1]Ñ¡Öá2
	Data_Write(WC, (Data[7]<<24) | (Data[8]<<16) | (Data[9]<<8) | (Data[10]));
	Data_Write(WP, (Data[15]<<24) | (Data[16]<<16) | (Data[17]<<8) | (Data[18]));
	
	CMD_Write((CMD_NUM)Data[19]);
	CMD_Write(ICW);
}


/**********************************************************************************************
* ²ÎÊı£ºÊı×éÖ¸Õë
		Data0      ÃüÁîÂë
		Data1      Öá1 01-XÖá 02-YÖá 04-ZÖá  08-UÖá
		Data2      Öá2 01-XÖá 02-YÖá 04-ZÖá  08-UÖá
		Data3~6    Öá1Ô²ĞÄ
		Data7~10   Öá2Ô²ĞÄ
		Data11~14  Öá2Âö³åÊı
		Data15~18  Öá2Âö³åÊı
		Data19     3B-²å²¹¼õËÙÓĞĞ§ 3C-²å²¹¼õËÙÎŞĞ§
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷£ºÁ½ÖáÄæÊ±Õë·½ÏòÔ²»¡²å²¹
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
	
	Set_Axis((AXIS_NUM)(Data[1]<<8)); //Data[1]Ñ¡Öá1
	Data_Write(WC, (Data[3]<<24) | (Data[4]<<16) | (Data[5]<<8) | (Data[6]));
	Data_Write(WP, (Data[11]<<24) | (Data[12]<<16) | (Data[13]<<8) | (Data[14]));
	
	Set_Axis((AXIS_NUM)(Data[2]<<8)); //Data[2]Ñ¡Öá2
	Data_Write(WC, (Data[7]<<24) | (Data[8]<<16) | (Data[9]<<8) | (Data[10]));
	Data_Write(WP, (Data[15]<<24) | (Data[16]<<16) | (Data[17]<<8) | (Data[18]));
	
	CMD_Write((CMD_NUM)Data[19]);
	CMD_Write(ICCW);
}
#ifdef IB
/**********************************************************************************************
* ²ÎÊı£ºaxisn¿ÉÈ¡Ã¶¾ÙAXIS_NUMÖĞ¶¨ÒåµÄÖµ
		pulsenÎªÖáaxisn¶ÔÓ¦µÄÏà¶ÔÂö³åÊı
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷£ºÁ½ÖáÎ»Ä£Ê½²å²¹
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
* ²ÎÊı£º
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷£º2ÖáÎ»Ä£Ê½²å²¹
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
	
	Set_Axis((AXIS_NUM)(Data[1]<<8)); //Data[1]Ñ¡Öá1
	Set_Axis((AXIS_NUM)(Data[2]<<8)); //Data[2]Ñ¡Öá2
	CMD_Write(BPDC);//Çå³ıBP¼Ä´æÆ÷Êı¾İ
	CMD_Write(BPRWEN);//Ê¹ÄÜĞ´BP¼Ä´æÆ÷
	
						//ÃüÁîÊı¾İ0~15
	Reg_Write(R2,Data[3]);//axis1 +direction
	Reg_Write(R3,Data[4]);//axis1 -direction
	Reg_Write(R4,Data[5]);//axis2 +direction
	Reg_Write(R5,Data[6]);//axis2 -direction
	CMD_Write(BPDS);   //BPÊı¾İÑ¹Õ» SC=1
	
						//ÃüÁîÊı¾İ16~31
	Reg_Write(R2,Data[7]);//axis1 +direction
	Reg_Write(R3,Data[8]);//axis1 -direction
	Reg_Write(R4,Data[9]);//axis2 +direction
	Reg_Write(R5,Data[10]);//axis2 -direction
	CMD_Write(BPDS);   //BPÊı¾İÑ¹Õ» SC=2
	
						//ÃüÁîÊı¾İ32~47
	Reg_Write(R2,Data[11]);//axis1 +direction
	Reg_Write(R3,Data[12]);//axis1 -direction
	Reg_Write(R4,Data[13]);//axis2 +direction
	Reg_Write(R5,Data[14]);//axis3 -direction
	CMD_Write(BPDS);   //BPÊı¾İÑ¹Õ» SC=3
	
	CMD_Write((CMD_NUM)Data[15]);
	CMD_Write(I2B);	
	
	RData = ((Reg_Read(R0)>>13)&0x0003);
	if(RData==0x02||0x01)
	{						
							//ÃüÁîÊı¾İ48~62
		Reg_Write(R2,Data[16]);//axis1 +direction
		Reg_Write(R3,Data[17]);//axis1 -direction
		Reg_Write(R4,Data[18]);//axis2 +direction
		Reg_Write(R5,Data[19]);//axis3 -direction
		CMD_Write(BPDS);//BPÊı¾İÑ¹Õ» SC=3
	}
	RData = ((Reg_Read(R0)>>13)&0x0003);
	if(RData==0x02||0x01)
	{						
							//ÃüÁîÊı¾İ48~62
		Reg_Write(R2,Data[20]);//axis1 +direction
		Reg_Write(R3,Data[21]);//axis1 -direction
		Reg_Write(R4,Data[22]);//axis2 +direction
		Reg_Write(R5,Data[23]);//axis3 -direction
		CMD_Write(BPDS);//BPÊı¾İÑ¹Õ» SC=3
	}
	
}

/**********************************************************************************************
* ²ÎÊı£ºaxisn¿ÉÈ¡Ã¶¾ÙAXIS_NUMÖĞ¶¨ÒåµÄÖµ
		pulsenÎªÖáaxisn¶ÔÓ¦µÄÏà¶ÔÂö³åÊı
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷£ºÈıÖáÎ»Ä£Ê½²å²¹
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
* ²ÎÊı£º
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷£ºµ¥²½Command²å²¹Ä£Ê½  
**********************************************************************************************/
void ITP_Step_Command(void)
{	
	Reg_Write(R5,0x1104);
	CMD_Write(ISS);		
	//²å²¹²ÎÊıºÍ²å²¹ÃüÁîSet the initial and drive speeds,finish point, center point.......
		
}
/**********************************************************************************************
* ²ÎÊı£ºÊı×éÖ¸Õë
		Data0      ÃüÁîÂë
		Data1      0 Íâ²¿ĞÅºÅÖ´ĞĞµ¥²½²å²¹ 
				   1 ÃüÁî(0x3A)Ö´ĞĞµ¥²½²å²¹	
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷£ºÍâ²¿ĞÅºÅ»òÕßÃüÁîÖ´ĞĞµ¥²½²å²¹
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
* ²ÎÊı£º
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷£ºµ¥²½Íâ²¿ĞÅºÅ²å²¹Ä£Ê½  
**********************************************************************************************/
void ITP_Step_External(void)
{
	
	Reg_Write(R5,0x0904);
	Data_Write(WEM,0x5f00);
	CMD_Write(ISS);		
	//ÉèÖÃ²å²¹²ÎÊı ÃüÁî  ÂË²¨Æ÷ µÈ´ıÍâ²¿ĞÅºÅÎªµÍ 	
}
/**********************************************************************************************
* ²ÎÊı£ºstate¿ÉÈ¡DECV    3B  ²å²¹¼õËÙÓĞĞ§ enable the automatic deceleration or manual deceleration function
	             DECINV  3C	 ²å²¹¼õËÙÎŞĞ§ disable the automatic deceleration or manual deceleration function
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷£º²å²¹Í£Ö¹Ä£Ê½//In continuous interpolation, same as in circular and bit pattern interpolations, only manual deceleration in the trapezoidal driving
					//is available; The automatic deceleration in S-curve driving is not available.
					//In circular interpolation and bit pattern interpolation, only manual deceleration in trapezoidal driving is available; the automatic
					//deceleration in S-curve driving is not available.
**********************************************************************************************/
void ITP_Dece_State(CMD_NUM state)
{
	CMD_Write(state);
}
/**********************************************************************************************
* ²ÎÊı£ºstate¿ÉÈ¡DECS»òÕßSUDS£¬DECS¼õËÙÍ£Ö¹£¬SUDSÁ¢¼´Í£Ö¹
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷£º²å²¹Í£Ö¹Ä£Ê½
**********************************************************************************************/
void ITP_Dece_Mode(CMD_NUM state)
{
	CMD_Write(state);
}

/**********************************************************************************************
* ²ÎÊı£ºÎŞ
* ·µ»ØÖµ£º²å²¹×´Ì¬
		  0 ²å²¹Íê³É½áÊø
		  1 ²å²¹ÔÚ½øĞĞ
* ËµÃ÷£º²éÑ¯²å²¹×´Ì¬
**********************************************************************************************/
unsigned char ITP_Stauts(void)
{
	unsigned short temp;
	temp = ((Reg_Read(R0)>>8)&0x0001);
	return (unsigned char)temp;
}
/**********************************************************************************************
* ²ÎÊı£ºÎŞ
* ·µ»ØÖµ£ºÁ¬Ğø²å²¹×´Ì¬
		  0 ²»ÄÜ½øĞĞÁ¬Ğø²å²¹
		  1 ÄÜ½øĞĞÏÂÒ»¸ö²å²¹
* ËµÃ÷£º²éÑ¯ÊÇ·ñ¿ÉÒÔ½øĞĞÁ¬Ğø²å²¹×´Ì¬ //Á¬Ğø²å²¹ µ±Ç°²å²¹ÃüÁîÒÑ¾­Ö´ĞĞÇÒ RR0/D9 Îª1 ¿ÉÒÔ½øĞĞÁ¬Ğø²å²¹(¼ì²é×´Ì¬Ö®Ç° ÏÈÒª¼ì²é´íÎó£¨²å²¹µÄÖáRRO/D4~D7£© Ã»´íÎó²Å¿ÉÒÔÖ´ĞĞÏÂ¸ö²å²¹)Ò
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
* ËµÃ÷£ºIO¶Á
* ²ÎÊı£ºIO¿Ú±àºÅ
* ·µ»ØÖµ£ºIO×´Ì¬
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
* ËµÃ÷£ºIOĞ´
* ²ÎÊı£ºIO¿Ú±àºÅ£¬Ğ´IO¿Ú×´Ì¬
* ·µ»ØÖµ£ºÎŞ
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
* ²ÎÊı£ºÊı×éÖ¸Õë
		Data0         ÃüÁîÂë	
		Data1         Ñ¡Öá 01-XÖá 02-YÖá 04-ZÖá  08-UÖá
		Data2~5       ÊäÈëĞÅºÅÂË²¨Ê¹ÄÜ ¶ÔÓ¦Îª1¾ÍÊÇÊ¹ÄÜ¶ÔÆäĞÅºÅÂË²¨¹¦ÄÜ
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷£
**********************************************************************************************/
void Iput_Filters_Config(void)
{
	
	Data_Write(WEM,0x5f00);
}

/**********************************************************************************************
* ²ÎÊı£ºÊı×éÖ¸Õë
		Data0       ÃüÁîÂë	
		Data1       Ñ¡Öá
		Data2       ÊäÈëĞÅºÅµçÆ½
		Data3~6     Íâ²¿À©Õ¹Ä£Ê½ºÍĞÅºÅÂË²¨²ÎÊıÉèÖÃ£¨WR6ÂË²¨ WR7À©Õ¹£©
		Data7~8     Ô­µãËÑË÷ËÙ¶È
		Data9~12    Æ«ÒÆÂö³åÊı
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷£º½üÁãµãnIN0¡¢ÁãµãnIN2¡¢ZÏàĞÅºÅIN2×÷Îª»ØÔ­µãĞÅºÅ,nIN0ÓënIN1×÷Îª»ØÔ­µãĞÅºÅÏŞÎ»ĞÅºÅ×÷Îª»ØÔ­µãĞÅºÅ£¬
**********************************************************************************************/
void Auto_Home_Search(unsigned char *temp)
{
	unsigned char Data[13];
	int i;	
	for(i=0;i<13;i++)
	{
		Data[i]=*temp++;
	}	
	Set_Axis((AXIS_NUM)(Data[1]<<8));//Ñ¡Öá
	wr_map[R1] &= 0xff00;
	wr_map[R1] |= Data[2];
	Reg_Write(R1, wr_map[R1]);  //ÊäÈëĞÅºÅÂß¼­ÉèÖÃD6,4,2,0 INm-L ¸ßµÍµçÆ½   D7,5,3,1 Çı¶¯Í£Ö¹ÊäÈëĞÅºÅ INm 0: disable, 1: enable  
	//Data[0]..... 0ÃüÁî1×Ö½Ú   1Öá1×Ö½Ú   2ÊäÈëĞÅºÅÂß¼­1×Ö½Ú  3~6À©Õ¹Ä£Ê½²ÎÊı4×Ö½Ú  7Ô­µãËÑË÷ËÙ¶È2×Ö½Ú  9~12Êä³öÆ«ÒÆÂö³åÊı 
	//0x00,0x01,0x00,0x49,0x5f,0x5f,0x00,0x00,0x50,0x00,0x00,0x0f,0xff
	Data_Write(WEM,(Data[3]<<24) | (Data[4]<<16) | (Data[5]<<8) | Data[6]);
								//À©Õ¹Ä£Ê½ÉèÖÃ
								//WR6ÊäÈëĞÅºÅÂË²¨Æ÷Ä£Ê½£º   5f00
								//D15~D13  010  ÂË²¨Æ÷ÑÓÊ±£º512us
								//D8        1   nLMTM,nIN0,nIN1ĞÅºÅ£º ENABLE(Ê¹ÄÜÂË²¨Æ÷)
								//D9        0   nIN2ĞÅºÅ£D      DISABLE(Ê§ÄÜÂË²¨Æ÷)º
	
								//WR7×Ô¶¯»ØÁãÄ£Ê½ÉèÖÃ£ºa near home, home, or a Z-phase signal-0x495f £¬IN0 and IN1-0x014f,a limit signal-0x054f
								//D15~D13  010 Îó²î¼ÆÊıÆ÷ÇåÁãÂö³å¿í¶È100us
								//D12       0  Îó²î¼ÆÊıÆ÷ÇåÁãÂö³åÊä³öÂß¼­µçÆ½£º¸ß
 								//D11       1  Îó²î¼ÆÊıÆ÷ÇåÁãÊä³ö£º      ENABLE
								//D10		0  Ê¹ÓÃÏŞÎ»ĞÅºÅ×÷Îª»ØÁãĞÅºÅ£ºDISABLE
								//D9        0  ZÏàAND»ØÁãĞÅºÅ£º          DISABLE
								//D8        1  Âß¼­/Êµ¼ÊÎ»ÖÃ¼ÆÊıÆ÷£º     ENABLE  
	                            //D7        0  Step4Çı¶¯·½Ïò£º           +Õı·½Ïò
								//D6        0  Step4£º                   ENABLE
								//D5        0  Step3Çı¶¯·½Ïò£º           -¸º·½Ïò
								//D4        1  Step3£º                   ENABLE
								//D3        1  Step2Çı¶¯·½Ïò£º           -¸º·½Ïò
								//D2        1  Step2£º                   ENABLE
								//D1        1  Step1Çı¶¯·½Ïò£º           -¸º·½Ïò
								//D0        1  Step1£º                   ENABLE
								
								
	Data_Write(WHV,(Data[7]<<8) | (Data[8]));//Step 2Ô­µãËÑË÷ËÙ¶È500
	Data_Write(WP, (Data[9]<<24) | (Data[10]<<16) | (Data[11]<<8) | Data[12]);//Æ«ÒÆÂö³å
	CMD_Write(AHOME);//Æô¶¯×Ô¶¯»ØÔ­µãËÑË÷
}
//Çå³ı»ØÔ­µãÏŞÎ»
void Clear_LimtMod(void)
{
	wr_map[R1] &= 0xff33;
	Reg_Write(R1, wr_map[R1]);    //Ê§ÄÜÏŞÎ»ĞÅºÅ
	//Clear_Axis();
}
/**********************************************************************************************
* ²ÎÊı£ºÊı×éÖ¸Õë
		Data0       ÃüÁîÂë	
		Data1       Öá1Ö÷Öá 
		Data2       Öá2
		Data3       Öá3
		Data4       Öá4
		Data5~8     Öá1ÕıÏò±È½ÏÆ÷¼ÆÊıÖµ
		Data9~12    Öá1Êä³öÂö³åÊı
		Data13~16   Öá2Êä³öÂö³åÊı
		Data17~20   Öá3Êä³öÂö³åÊı
		Data21~24   Öá4Êä³öÂö³åÊı
		Data25~28   Öá1Í¬²½Ä£Ê½²ÎÊı
		Data29~32   Öá2Í¬²½Ä£Ê½²ÎÊı
		Data33~36   Öá3Í¬²½Ä£Ê½²ÎÊı
		Data37~40   Öá4Í¬²½Ä£Ê½²ÎÊı
		Data41      Öá1³õÊ¼Çı¶¯×´Ì¬
		Data42		Öá2³õÊ¼Çı¶¯×´Ì¬
		Data43		Öá3³õÊ¼Çı¶¯×´Ì¬
		Data44		Öá4³õÊ¼Çı¶¯×´Ì¬
* ·µ»ØÖµ£ºÎŞ
* ËµÃ÷£º
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
	Reg_Write(R2, wr_map[R2]);//ÓëÂß¼­¼ÆÊıÆ÷±È½ÏWR2-D5=0
	Set_Axis((AXIS_NUM)(Data[1]<<8));//Ñ¡Öá1
	Set_Axis((AXIS_NUM)(Data[2]<<8));//Ñ¡Öá2
	Set_Axis((AXIS_NUM)(Data[3]<<8));//Ñ¡Öá3
	Set_Axis((AXIS_NUM)(Data[4]<<8));//Ñ¡Öá4
//0x00,/0x01,0x02,0x04,0x08,/0x00,0x00,0xff,0xff,/0x00,0x02,0xff,0xff,/0x00,0x02,0xff,0xff,/0x00,0x02,0xff,0xff,/0x00,0x02,0xff,0xff,/0x00,0x10,0xe0,0x01,/0x00,0x01,0x00,0x00,/0x00,0x04,0x00,0x00/,0x00,0x08,0x00,0x00/0x20,0x25,0x25,0x25	
	CompP_Set((AXIS_NUM)(Data[1]<<8),(Data[5]<<24) | (Data[6]<<16) | (Data[7]<<8) | Data[8]);  //axis1Öá±È½ÏÆ÷ÉèÖÃÊıpulse
	LP_Set((AXIS_NUM)(Data[1]<<8),0);//Çå³ıaxis1Âß¼­Î»ÖÃ¼Ä´æÆ÷
	LP_Set((AXIS_NUM)(Data[2]<<8),0);//Çå³ıaxis2Âß¼­Î»ÖÃ¼Ä´æÆ÷
	LP_Set((AXIS_NUM)(Data[3]<<8),0);//Çå³ıaxis3Âß¼­Î»ÖÃ¼Ä´æÆ÷
	LP_Set((AXIS_NUM)(Data[4]<<8),0);//Çå³ıaxis4Âß¼­Î»ÖÃ¼Ä´æÆ÷
	
	Pulse_Out((AXIS_NUM)(Data[1]<<8),(Data[9]<<24) | (Data[10]<<16) | (Data[11]<<8) | Data[12]);  //axis1Êä³öÂö³åÊıpulse	
	Pulse_Out((AXIS_NUM)(Data[2]<<8),(Data[13]<<24) | (Data[14]<<16) | (Data[15]<<8) | Data[16]); //axis2Êä³öÂö³åÊıpulse
	Pulse_Out((AXIS_NUM)(Data[3]<<8),(Data[17]<<24) | (Data[18]<<16) | (Data[19]<<8) | Data[20]); //axis3Êä³öÂö³åÊıpulse	
	Pulse_Out((AXIS_NUM)(Data[4]<<8),(Data[21]<<24) | (Data[22]<<16) | (Data[23]<<8) | Data[24]); //axis4Êä³öÂö³åÊıpulse	
	
	
	SYN_Mode((AXIS_NUM)(Data[1]<<8),(Data[25]<<24) | (Data[26]<<16) | (Data[27]<<8) | Data[28]);        //axis1Í¬²½Ä£Ê½ÉèÖÃ
	SYN_Mode((AXIS_NUM)(Data[2]<<8),(Data[29]<<24) | (Data[30]<<16) | (Data[31]<<8) | Data[32]);        //axis2Í¬²½Ä£Ê½ÉèÖÃ
	SYN_Mode((AXIS_NUM)(Data[3]<<8),(Data[33]<<24) | (Data[34]<<16) | (Data[35]<<8) | Data[36]);        //axis3Í¬²½Ä£Ê½ÉèÖÃ
	SYN_Mode((AXIS_NUM)(Data[4]<<8),(Data[37]<<24) | (Data[38]<<16) | (Data[39]<<8) | Data[40]);        //axis4Í¬²½Ä£Ê½ÉèÖÃ
	
	Reg_Write(R0,(AXIS_NUM)(Data[1]<<8)|Data[41]);
	Reg_Write(R0,(AXIS_NUM)(Data[2]<<8)|Data[42]);
	Reg_Write(R0,(AXIS_NUM)(Data[3]<<8)|Data[43]);
	Reg_Write(R0,(AXIS_NUM)(Data[4]<<8)|Data[44]);	
	
	//ÊäÈëĞÅºÅnIN3 fell  nIN3ÖĞ¶Ï·¢Éú ±£´æÖáaxis1£¬axis2£¬axis3Î»ÖÃÊı¾İ
//¶ÁÈ¡XRR3¼Ä´æÆ÷ ¼ì²éD9=1.ÖĞ¶Ï·¢Éú XIN3 fellË	
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
//Çå³ıÍ¬²½Ä£Ê½
void Clear_Synch(void)
{
	Set_Axis(AXIS_ALL);
	Data_Write(WSM,0);
	AXIS_Switch();
}

