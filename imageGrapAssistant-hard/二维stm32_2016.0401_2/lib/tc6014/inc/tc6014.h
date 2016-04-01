/**
 *********************************************************************
 **±êÌâ£ºtc6014.h
 **×÷Õß£ºfyf
 **ÈÕÆÚ£º2014.10.28
 **°æ±¾£ºv1.0
 **ËµÃ÷£ºtc6014 driver
 *********************************************************************
**/

#ifndef		_TC6014_H
#define		_TC6014_H

#define		BASE_ADDR	0x60000000  	//FSMC»ùµØÖ·
#define		OFFSET		0X00020000		//Æ«ÒÆµØÖ·

//---------------------------------------------- ¼Ä´æÆ÷ÁĞ±í ---------------------------------------
typedef enum
{
	R0,
	R1,
	R2,
	R3,
	R4,
	R5,
	R6,
	R7
} REG;

//---------------------------------------- ÖáÁĞ±í -----------------------------------------
typedef enum
{
	AXIS_NONE = 0X0000,
	AXIS_X = 0X0100,		//XÖá
	AXIS_Y = 0X0200,		//YÖá
	AXIS_X_Y = 0X0300,      //XYÖá
	AXIS_Z = 0X0400,		//ZÖá
	AXIS_X_Z = 0X0500,		//XZÖá
	AXIS_Y_Z = 0X0600,		//YZÖá
	AXIS_X_Y_Z = 0X0700,	//XYZÖá
	AXIS_U = 0X0800,		//UÖá	
	AXIS_X_U = 0X0900,		//XUÖá
	AXIS_Y_U = 0X0A00,		//YUÖá
	AXIS_X_Y_U = 0X0B00,	//XYUÖá
	AXIS_Z_U = 0X0C00,		//ZUÖá
	AXIS_X_Z_U = 0X0D00,	//XZUÖá	
	AXIS_Y_Z_U = 0X0E00,	//YZUÖá
	AXIS_ALL = 0X0F00,		//XYZUÖá
} AXIS_NUM;

//---------------------------------------------- ÃüÁîÁĞ±í -----------------------------------------
typedef enum
{
	//-------------------------Write Commands----------------------------
	WR,					//00h  	·¶Î§ÉèÖÃ
	WK,					//01 	¼ÓËÙ¶ÈÔö¼ÓÂÊÉèÖÃ
	WA, 				//02  	¼ÓËÙ¶ÈÉèÖÃ
	WD, 				//03  	¼õËÙ¶ÈÉèÖÃ
	WSV,  				//04  	Æô¶¯ËÙ¶ÈÉèÖÃ
	WV, 				//05  	Çı¶¯ËÙ¶ÈÉèÖÃ
	WP, 				//06  	Êä³öÂö³åÊıÉèÖÃ»òÕßÖÕµãÉèÖÃ
	WDP,				//07  	¼õËÙµãÉèÖÃ
	WC, 				//08  	Ô²ĞÄÉèÖÃ
	WLP,				//09  	Âß¼­Î»ÖÃ¼ÆÊıÆ÷³õÖµÉèÖÃ
	WEP,				//0A  	±àÂëÆ÷¼ÆÊıÆ÷³õÖµÉèÖÃ
	WCP,				//0B  	ÕıÏò±È½Ï¼Ä´æÆ÷ÊıÖµÉèÖÃ
	WCM,				//0C  	¸ºÏò±È½Ï¼Ä´æÆ÷ÉèÖÃ
	WAO,				//0D  	¼ÓËÙ¶È¼Ä´æÆ÷Æ«ÒÆÉèÖÃ
	WL, 				//0E  	¼õËÙ¶È¼õÉÙÂÊÉèÖÃ
	WEM = 0x60,			//60  	À©Õ¹Ä£Ê½ÉèÖÃ£¨Î»Êı¾İ£©
	WHV,				//61  	Ô­µãËÑË÷ËÙ¶È
	WSM	= 0x64,			//64  	Í¬²½Ä£Ê½ÉèÖÃ£¨Î»Êı¾İ£©
	 
	//------------------------Data Reading Commands--------------------------
	RLP = 0x10, 		//10h  	¶ÁÈ¡µ±Ç°Âß¼­Î»ÖÃ¼ÆÊıÆ÷ÊıÖµ
	REP,  				//11  	¶ÁÈ¡±àÂëÆ÷ÊıÖµ
	RV,  				//12  	¶ÁÈ¡µ±Ç°Çı¶¯ËÙ¶È
	RA,  				//13  	¶Á¼Ó¼õËÙ¶ÈÊıÖµ
	RSB,  				//14  	¶ÁÍ¬²½»º³å¼Ä´æÆ÷
	
	//------------------------Driving Commands-------------------------------- 
	PFD	= 0x20,			//20h  	Õı·½Ïò¹Ì¶¨Âö³åÇı¶¯
	MFD,				//21  	¸º·½Ïò¹Ì¶¨Âö³åÇı¶¯
	PCD,				//22  	Õı·½ÏòÁ¬ĞøÇı¶¯
	MCD,				//23  	¸º·½ÏòÁ¬ĞøÇı¶¯
	DSH,				//24  	Çı¶¯Æô¶¯±£³Ö
	DSHR,				//25 	Çı¶¯Æô¶¯±£³ÖÊÍ·Å
	DECS,				//26  	¼õËÙÍ£Ö¹
	SUDS,				//27  	Á¢¼´Í£Ö¹
	ALLSTOP,    //28    ËùÓĞµÄÖáÁ¢¼´Í£Ö¹
	ALLDECSTOP, //29    ËùÓĞµÄÖá¼õËÙÍ£Ö¹
	
	//------------------------Interpolation Commands------------------------- 
	I2L	= 0x30,			//30h  	2ÖáÖ±Ïß²å²¹
	I3L,				//31  	3ÖáÖ±Ïß²å²¹
	ICW,				//32  	Ë³Ê±Õë·½ÏòÔ²»¡²å²¹
	ICCW,				//33  	ÄæÊ±Õë·½ÏòÔ²»¡²å²¹
	I2B,				//34  	2ÖáÎ»²å²¹
	I3B,				//35  	3ÖáÎ»²å²¹
	BPRWEN,				//36  	Î»Ä£Ê½¼Ä´æÆ÷Ğ´
	BPRWDIS,			//37  	Î»Ä£Ê½¼Ä´æÆ÷Ğ´Ê§ÄÜ
	BPDS,				//38    Î»Ä£Ê½²å²¹¼Ä´æÆ÷Ñ¹Õ»
	BPDC,				//39  	Î»Ä£Ê½²å²¹Êı¾İ¼Ä´æÆ÷Çå³ı
	ISS,				//3A  	µ¥²½²å²¹
	DECV,				//3B  	²å²¹¼õËÙÓĞĞ§
	DECINV,				//3C  	²å²¹¼õËÙÎŞĞ§
	IIC,				//3D  	²å²¹ÖĞ¶ÏÇå³ı
	
	//-----------------------Other commands---------------------------------
	AHOME = 0x62,		//62  	×Ô¶¯Ô­µãËÑË÷
	DCCOUT,				//63  	Æ«ÒÆ¼ÆÊıÆ÷ÇåÁãÊä³ö
	SA = 0x65,			//65  	Æô¶¯Í¬²½ÔËĞĞ
	AS = 0x0f			//0F  	ÖáÇĞ»»
} CMD_NUM;

typedef enum
{
	Disable,			//Ê§ÄÜ
	Enable				//Ê¹ÄÜ
} STATE;

//Ğ´¼Ä´æÆ÷
void Reg_Write(REG reg, short dat);
//Ğ´ÃüÁî
void CMD_Write(CMD_NUM cmd);

void AXIS_Switch(void);
//Ğ´ĞÍÊı¾İ
void Data_Write(CMD_NUM cmd, int dat);
//¶Á¼Ä´æÆ÷
unsigned short Reg_Read(REG reg);
//¶ÁĞÍÊı¾İ
int Data_Read(CMD_NUM cmd);
//Èí¼ş¸´Î»

//ÖáÉèÖÃ
void Set_Axis(AXIS_NUM axis);
void Software_Reset(void);
//·¶Î§ÉèÖÃ
void Range_Set(AXIS_NUM axis,int data);

//¼ÓËÙ¶ÈÔö¼ÓÂÊÉèÖÃ
void INC_Rate(AXIS_NUM axis,int data);

//¼õËÙ¶È¼õÉÙÂÊÉèÖÃ
void DEC_Rate(AXIS_NUM axis,int data);

//¼ÓËÙ¶ÈÉèÖÃ
void INC_Set(AXIS_NUM axis,int data);

//¼õËÙ¶ÈÉèÖÃ
void DEC_Set(AXIS_NUM axis,int data);

//Æô¶¯ËÙ¶ÈÉèÖÃ
void Start_Speed(AXIS_NUM axis,int data);

//Çı¶¯ËÙ¶ÈÉèÖÃ
void Drive_Speed(AXIS_NUM axis,int data);

//Êä³öÂö³åÊıÉèÖÃ»òÕßÖÕµãÉèÖÃ
void Pulse_Out(AXIS_NUM axis,int data);

//¼õËÙµãÉèÖÃ
void DECP_Set(AXIS_NUM axis,int data);

//Ô²ĞÄÉèÖÃ
void Center_Set(AXIS_NUM axis,int data);

//Âß¼­Î»ÖÃ¼ÆÊıÆ÷³õÖµÉèÖÃ
void LP_Set(AXIS_NUM axis,int data);

//±àÂëÆ÷¼ÆÊıÆ÷³õÖµÉèÖÃ
void EP_Set(AXIS_NUM axis,int data);

//ÕıÏò±È½Ï¼Ä´æÆ÷ÊıÖµÉèÖÃ
void CompP_Set(AXIS_NUM axis,int data);

//¸ºÏò±È½Ï¼Ä´æÆ÷ÉèÖÃ
void CompM_Set(AXIS_NUM axis,int data);

//¼ÓËÙ¶È¼Ä´æÆ÷Æ«ÒÆÉèÖÃ
void ACC_Offset(AXIS_NUM axis,int data);

//À©Õ¹Ä£Ê½ÉèÖÃ£¨Î»Êı¾İ£©
void EXP_Mode(AXIS_NUM axis,int data);

//Ô­µãËÑË÷ËÙ¶È
void Home_SearchSpeed(AXIS_NUM axis,int data);

//Í¬²½Ä£Ê½ÉèÖÃ£¨Î»Êı¾İ£
void SYN_Mode(AXIS_NUM axis,int data);

//¶ÁÊµ¼ÊÎ»ÖÃ ¶ÁÈ¡±àÂëÆ÷ÊıÖµ
int Read_REP(AXIS_NUM axis);

//¶ÁÂß¼­Î»ÖÃ ¶ÁÈ¡µ±Ç°Âß¼­Î»ÖÃ¼ÆÊıÆ÷ÊıÖµ
int Read_RLP(AXIS_NUM axis);

//¶ÁÈ¡µ±Ç°Çı¶¯ËÙ¶È
int Read_RV(AXIS_NUM axis);

//¶Á¼Ó¼õËÙ¶ÈÊıÖµ
int Read_RA(AXIS_NUM axis);

//¶ÁÍ¬²½»º³å¼Ä´æÆ÷
int Read_RSB(AXIS_NUM axis);

void AXIS_Speed_Set(unsigned char *temp);

// µÈ´ıÖáÇı¶¯Í£Ö¹
void Wait(AXIS_NUM axis);

// Next data setting of waiting for continuous interpolation
//µÈ´ıÁ¬Ğø²å²¹ÏÂÒ»¸öÊı¾İµÄÉèÖÃ RR0/D9
void Next_Wait(void);

// Next data setting of waiting for BP interpolation
//µÈ´ıÎ»Ä£Ê½²å²¹ÏÂÒ»¸öÊı¾İÉèÖÃ RR0/D13-D14  SC
void BP_Wait(void);

typedef enum
{
	IT_PULSE = 0x0100,	//Âö³åÉÏÉıÑØÖĞ¶Ï
	IT_LMC = 0x0200,	//Âß¼­/±àÂëÆ÷¼ÆÊıÆ÷´óÓÚ¸º·½Ïò±È½Ï¼Ä´æÆ÷ÊıÖµ
	IT_SMC = 0x0400,	//Âß¼­/±àÂëÆ÷¼ÆÊıÆ÷´óÓÚ¸º·½Ïò±È½Ï¼Ä´æÆ÷ÊıÖµ
	IT_SPC = 0x0800,	//Âß¼­/±àÂëÆ÷¼ÆÊıÆ÷´óÓÚÕı·½Ïò±È½Ï¼Ä´æÆ÷ÊıÖµ
	IT_LPC = 0x1000,	//Âß¼­/±àÂëÆ÷¼ÆÊıÆ÷´óÓÚÕı·½Ïò±È½Ï¼Ä´æÆ÷ÊıÖµ
	IT_CEND = 0x2000,	//¼Ó¼õËÙÇı¶¯Ê±£¬¶¨ËÙÓò½áÊøÂö³åÊä³ö
	IT_CSTA = 0x4000,	//¼Ó¼õËÙÇı¶¯Ê±£¬¶¨ËÙÓò¿ªÊ¼Âö³åÊä³ö
	IT_DEND = 0x8000,	//Çı¶¯½áÊø
	IT_ALL = 0xff00		//ËùÓĞ
} IT_NUM;

//ÖĞ¶ÏÅäÖÃ
void It_Config(IT_NUM it, STATE state);

typedef enum
{
	IT_PULSE_MASK = 0x0001,		//Âö³åÉÏÉıÑØ
	IT_LMC_MASK = 0x0002,		//Âß¼­/±àÂëÆ÷¼ÆÊıÆ÷´óÓÚ¸º·½Ïò±È½Ï¼Ä´æÆ÷ÊıÖµ
	IT_SMC_MASK = 0x0004,		//Âß¼­/±àÂëÆ÷¼ÆÊıÆ÷´óÓÚ¸º·½Ïò±È½Ï¼Ä´æÆ÷ÊıÖµ
	IT_SPC_MASK = 0x0008,		//Âß¼­/±àÂëÆ÷¼ÆÊıÆ÷´óÓÚÕı·½Ïò±È½Ï¼Ä´æÆ÷ÊıÖµ
	IT_LPC_MASK = 0x0010,		//Âß¼­/±àÂëÆ÷¼ÆÊıÆ÷´óÓÚÕı·½Ïò±È½Ï¼Ä´æÆ÷ÊıÖµ
	IT_CEND_MASK = 0x0020,		//¼Ó¼õËÙÇı¶¯ÊÇ£¬¶¨ËÙÓò½áÊøÂö³åÊä³ö
	IT_CSTA_MASK = 0x0040,		//¼Ó¼õËÙÇı¶¯ÊÇ£¬¶¨ËÙÓò¿ªÊ¼Âö³åÊä³ö
	IT_DEND_MASK = 0x0080,		//Çı¶¯½áÊø
	IT_HOME_MASK = 0x0100,		//ÖĞ¶ÏÔ­µãËÑË÷
	IT_SYNC_MASK = 0x0200,		//Í¬²½¼¤»î
	IT_ALL_MASK =0x03ff,			//ËùÓĞ
	IT_ERR_MASK = 0
} IT_MASK;

//
IT_MASK IT_Axis_Is(void);

typedef enum
{
	ERR_LPC_MASK = 0x0001,		//Âß¼­/±àÂëÆ÷¼ÆÊıÆ÷´óÓÚÕı·½Ïò±È½Ï¼Ä´æÆ÷ÊıÖµ
	ERR_SMC_MASK = 0x0002,		//Âß¼­/±àÂëÆ÷¼ÆÊıÆ÷Ğ¡ÓÚ¸º·½Ïò±È½Ï¼Ä´æÆ÷ÊıÖµ
	ERR_PL_MASK = 0x0004,		//Ó²¼şÕıÏŞÎ»
	ERR_ML_MASK = 0x0008,		//Ó²¼ş¸ºÏŞÎ»
	ERR_ALARM_MASK = 0x0010,	//ËÅ·ş±¨¾¯
	ERR_EMG_MASK = 0x0020,		//¼±Í£
	ERR_ALL_MASK = 0x003f,		//ËùÓĞ
	ERR_ERR_MASK = 0
} ERR_MASK;

//
ERR_MASK Error_Is(void);

//----------------------------------------- Âö³åÄ£Ê½ ----------------------------------------
typedef enum
{
	DOUBLE_PULSE,		//Ë«Âö³åÄ£Ê½
	DIR_PULSE			//·½Ïò-Âö³åÄ£Ê½
} PULSE_MODE;
//------------------------------------------ µçÆ½ÁĞ±í ---------------------------------------
typedef enum
{
	LOW,				//µÍµçÆ½
	HIGH				//¸ßµçÆ½
} LEVEL;

//±¶Æµ
typedef enum
{
	MUL_1 = 0x0000,		//²»±¶Æµ
	MUL_2 = 0x0400,		//2±¶Æµ
	MUL_4 = 0x0800		//4±¶Æµ
} ENCODER_MUL;

typedef struct
{
	PULSE_MODE Pulse_Mode;		//ÊäÈëÂö³åÄ£Ê½
	ENCODER_MUL Pulse_Mul;		//±¶ÆµÊı
} Encoder_InitTypeDef;

void Encoder_Init(Encoder_InitTypeDef Encoder_InitStruct);
void Encoder_SetCounter(int Counter_Value);
int Encoder_GetCounter(void);

//±È½Ï¼Ä´æÆ÷µÄ±È½Ï¶ÔÏó
typedef enum
{
	LG_CNT = 0x0000,		//Âß¼­¼ÆÊıÆ÷
	EC_CNT = 0x0020	       //±àÂëÆ÷¼ÆÊıÆ÷
} OBJ;
//±È½Ï¼Ä´æÆ÷ÉèÖÃ
typedef struct
{
	STATE PComp_State;			//Õı·½Ïò±È½Ï×´Ì¬
	STATE MComp_State;			//¸º·½Ïò±È½Ï×´Ì¬
	OBJ Comp_Obj;				//±È½Ï¶ÔÏó
	int PComp_Value;			//Õı·½Ïò±È½Ï¼Ä´æÆ÷³õÖµ
	int MComp_Value;			//¸º·½Ïò±È½Ï¼Ä´æÆ÷³õÖµ
} Comp_InitTypeDef;
//³õÊ¼»¯±È½ÏÆ÷ÉèÖÃ
void Comp_Init(Comp_InitTypeDef Comp_InitStruct);
void Comp_Config(unsigned char *temp);
//Í£Ö¹Ä£Ê½
typedef enum
{
	SUD_STOP,		//Á¢¼´Í£Ö¹
	DEC_STOP		//¼õËÙÍ£Ö¹
} STOP_MODE;
//ÖáÇı¶¯³õÊ¼»¯
typedef struct
{
	STOP_MODE Stop_Mode;		//ÏŞÎ»Í£Ö¹Ä£Ê½
	LEVEL PLimit_Level;			//ÕıÏòÏŞÎ»ÓĞĞ§µçÆ½
	LEVEL MLimit_Level;			//¸ºÏòÏŞÎ»ÓĞĞ§µçÆ½
	PULSE_MODE Pulse_Mode;		//Êä³öÂö³åÄ£Ê½
	LEVEL Logic_Level;			//Êä³öÂö³åÂß¼­µçÆ½
	LEVEL Dir_Level;			//Êä³öÂö³å·½ÏòµçÆ½
} AXIS_InitTypeDef;
//ÖáÇı¶¯³õÊ¼»¯
void AXIS_Init(AXIS_InitTypeDef AXIS_InitStuct);

//Çı¶¯²ÎÊı
typedef struct
{
	unsigned int 	Range;			//±¶ÂÊ,			16000-8000000,	4bytes
	unsigned short 	Acce;			//¼ÓËÙ¶È,		1-8000,			2bytes
	unsigned short 	Dece;			//¼õËÙ¶È,		1-8000,			2bytes
	unsigned short 	Acce_Rate;		//¼ÓËÙ¶È±ä»¯ÂÊ,	1-65535,		2bytes
	unsigned short 	Dece_Rate;		//¼ÓËÙ¶È±ä»¯ÂÊ,	1-65535,		2bytes
	short			Acce_Offset;	//¼ÓËÙ¶ÈÎó²î£¬	-32768-+32767	2bytes
	unsigned short 	Init_Speed;		//³õÊ¼ËÙ¶È,		1-8000,			2bytes
	unsigned short 	Speed;			//Çı¶¯ËÙ¶È,		1-8000,			2bytes
	unsigned short 	Man_Dece_Pos;	//ÊÖ¶¯¼õËÙµã,	0-4294967295,	4bytes
} Drive_InitTypeDef;
//Çı¶¯³õÊ¼»¯
void Drive_Init(Drive_InitTypeDef Drive_InitStruct);
void AXIS_Mode_Config(unsigned char *temp);
void Axis_Speed_Config(unsigned char *temp);
AXIS_NUM Stop_Axis(void);
void IT_Axis_Config(IT_NUM it, STATE state);
void IT_Itp_Config(STATE state);
void IT_BP_Config(STATE state);
void IT_Home_Config(STATE state);
void IT_SYN_Config(STATE state);
unsigned char IT_Itp_Is(void);
unsigned char IT_BP_Is(void);
unsigned char IT_Home_Is(void);
unsigned char IT_SYN_Is(void);
IT_MASK IT_Axis_Is(void);


void Axis_Fixed_Drive(unsigned char *temp);
//Á¬ĞøÇı¶¯
void Axis_Continuous_Drive(unsigned char *temp);
//Íâ²¿ĞÅºÅÇı¶¯
void External_Signals_Drive(unsigned char *temp);
void Clear_External_Drive(void);
//Í£Ö¹Ä£Ê½
void AXIS_Stop(unsigned char *temp);
//Èí¼şÏŞÎ»
void Software_Limit(unsigned char *temp);
//Íâ²¿ĞÅºÅÇå³ıÊµ¼ÊÎ»ÖÃ
void EXT_CLR_RLP(void);


//Çı¶¯Íê³ÉÖĞ¶Ï×´Ì¬
void It_Finish(STATE state);
unsigned char ITP_Axis(unsigned char Data);
//²å²¹º¯Êı
void ITP_2AxisLine(unsigned char *temp);
void ITP_3AxisLine(unsigned char *temp);
void ITP_CW(unsigned char *temp);
void ITP_CCW(unsigned char *temp);
unsigned char Itp_Stauts(void);

void ITP_Single_Step(unsigned char *temp);
void ITP_Step_External(void);
void ITP_Step_Command(void);

//IO¶Á
char IO_ReadBit(unsigned int in);

//IOĞ´
void IO_WriteBit(unsigned char out, unsigned short act);
void Iput_Filters_Config(void);
void Auto_Home_Search(unsigned char *temp);
void SYN_Action1(unsigned char *temp);
void SYN_Action2(unsigned char *temp);
void SYN_Action(unsigned char *temp);
#endif
