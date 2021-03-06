/**
 *********************************************************************
 **标题：tc6014.h
 **作者：fyf
 **日期：2014.10.28
 **版本：v1.0
 **说明：tc6014 driver
 *********************************************************************
**/

#ifndef		_TC6014_H
#define		_TC6014_H

#define		BASE_ADDR	0x60000000  	//FSMC基地址
#define		OFFSET		0X00020000		//偏移地址

//---------------------------------------------- 寄存器列表 ---------------------------------------
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

//---------------------------------------- 轴列表 -----------------------------------------
typedef enum
{
	AXIS_NONE = 0X0000,
	AXIS_X = 0X0100,		//X轴
	AXIS_Y = 0X0200,		//Y轴
	AXIS_X_Y = 0X0300,      //XY轴
	AXIS_Z = 0X0400,		//Z轴
	AXIS_X_Z = 0X0500,		//XZ轴
	AXIS_Y_Z = 0X0600,		//YZ轴
	AXIS_X_Y_Z = 0X0700,	//XYZ轴
	AXIS_U = 0X0800,		//U轴	
	AXIS_X_U = 0X0900,		//XU轴
	AXIS_Y_U = 0X0A00,		//YU轴
	AXIS_X_Y_U = 0X0B00,	//XYU轴
	AXIS_Z_U = 0X0C00,		//ZU轴
	AXIS_X_Z_U = 0X0D00,	//XZU轴	
	AXIS_Y_Z_U = 0X0E00,	//YZU轴
	AXIS_ALL = 0X0F00,		//XYZU轴
} AXIS_NUM;

//---------------------------------------------- 命令列表 -----------------------------------------
typedef enum
{
	//-------------------------Write Commands----------------------------
	WR,					//00h  	范围设置
	WK,					//01 	加速度增加率设置
	WA, 				//02  	加速度设置
	WD, 				//03  	减速度设置
	WSV,  				//04  	启动速度设置
	WV, 				//05  	驱动速度设置
	WP, 				//06  	输出脉冲数设置或者终点设置
	WDP,				//07  	减速点设置
	WC, 				//08  	圆心设置
	WLP,				//09  	逻辑位置计数器初值设置
	WEP,				//0A  	编码器计数器初值设置
	WCP,				//0B  	正向比较寄存器数值设置
	WCM,				//0C  	负向比较寄存器设置
	WAO,				//0D  	加速度寄存器偏移设置
	WL, 				//0E  	减速度减少率设置
	WEM = 0x60,			//60  	扩展模式设置（位数据）
	WHV,				//61  	原点搜索速度
	WSM	= 0x64,			//64  	同步模式设置（位数据）
	 
	//------------------------Data Reading Commands--------------------------
	RLP = 0x10, 		//10h  	读取当前逻辑位置计数器数值
	REP,  				//11  	读取编码器数值
	RV,  				//12  	读取当前驱动速度
	RA,  				//13  	读加减速度数值
	RSB,  				//14  	读同步缓冲寄存器
	
	//------------------------Driving Commands-------------------------------- 
	PFD	= 0x20,			//20h  	正方向固定脉冲驱动
	MFD,				//21  	负方向固定脉冲驱动
	PCD,				//22  	正方向连续驱动
	MCD,				//23  	负方向连续驱动
	DSH,				//24  	驱动启动保持
	DSHR,				//25 	驱动启动保持释放
	DECS,				//26  	减速停止
	SUDS,				//27  	立即停止
	ALLSTOP,    //28    所有的轴立即停止
	ALLDECSTOP, //29    所有的轴减速停止
	
	//------------------------Interpolation Commands------------------------- 
	I2L	= 0x30,			//30h  	2轴直线插补
	I3L,				//31  	3轴直线插补
	ICW,				//32  	顺时针方向圆弧插补
	ICCW,				//33  	逆时针方向圆弧插补
	I2B,				//34  	2轴位插补
	I3B,				//35  	3轴位插补
	BPRWEN,				//36  	位模式寄存器写
	BPRWDIS,			//37  	位模式寄存器写失能
	BPDS,				//38    位模式插补寄存器压栈
	BPDC,				//39  	位模式插补数据寄存器清除
	ISS,				//3A  	单步插补
	DECV,				//3B  	插补减速有效
	DECINV,				//3C  	插补减速无效
	IIC,				//3D  	插补中断清除
	
	//-----------------------Other commands---------------------------------
	AHOME = 0x62,		//62  	自动原点搜索
	DCCOUT,				//63  	偏移计数器清零输出
	SA = 0x65,			//65  	启动同步运行
	AS = 0x0f			//0F  	轴切换
} CMD_NUM;

typedef enum
{
	Disable,			//失能
	Enable				//使能
} STATE;

//写寄存器
void Reg_Write(REG reg, short dat);
//写命令
void CMD_Write(CMD_NUM cmd);

void AXIS_Switch(void);
//写型数据
void Data_Write(CMD_NUM cmd, int dat);
//读寄存器
unsigned short Reg_Read(REG reg);
//读型数据
int Data_Read(CMD_NUM cmd);
//软件复位

//轴设置
void Set_Axis(AXIS_NUM axis);
void Software_Reset(void);
//范围设置
void Range_Set(AXIS_NUM axis,int data);

//加速度增加率设置
void INC_Rate(AXIS_NUM axis,int data);

//减速度减少率设置
void DEC_Rate(AXIS_NUM axis,int data);

//加速度设置
void INC_Set(AXIS_NUM axis,int data);

//减速度设置
void DEC_Set(AXIS_NUM axis,int data);

//启动速度设置
void Start_Speed(AXIS_NUM axis,int data);

//驱动速度设置
void Drive_Speed(AXIS_NUM axis,int data);

//输出脉冲数设置或者终点设置
void Pulse_Out(AXIS_NUM axis,int data);

//减速点设置
void DECP_Set(AXIS_NUM axis,int data);

//圆心设置
void Center_Set(AXIS_NUM axis,int data);

//逻辑位置计数器初值设置
void LP_Set(AXIS_NUM axis,int data);

//编码器计数器初值设置
void EP_Set(AXIS_NUM axis,int data);

//正向比较寄存器数值设置
void CompP_Set(AXIS_NUM axis,int data);

//负向比较寄存器设置
void CompM_Set(AXIS_NUM axis,int data);

//加速度寄存器偏移设置
void ACC_Offset(AXIS_NUM axis,int data);

//扩展模式设置（位数据）
void EXP_Mode(AXIS_NUM axis,int data);

//原点搜索速度
void Home_SearchSpeed(AXIS_NUM axis,int data);

//同步模式设置（位数据�
void SYN_Mode(AXIS_NUM axis,int data);

//读实际位置 读取编码器数值
int Read_REP(AXIS_NUM axis);

//读逻辑位置 读取当前逻辑位置计数器数值
int Read_RLP(AXIS_NUM axis);

//读取当前驱动速度
int Read_RV(AXIS_NUM axis);

//读加减速度数值
int Read_RA(AXIS_NUM axis);

//读同步缓冲寄存器
int Read_RSB(AXIS_NUM axis);

void AXIS_Speed_Set(unsigned char *temp);

// 等待轴驱动停止
void Wait(AXIS_NUM axis);

// Next data setting of waiting for continuous interpolation
//等待连续插补下一个数据的设置 RR0/D9
void Next_Wait(void);

// Next data setting of waiting for BP interpolation
//等待位模式插补下一个数据设置 RR0/D13-D14  SC
void BP_Wait(void);

typedef enum
{
	IT_PULSE = 0x0100,	//脉冲上升沿中断
	IT_LMC = 0x0200,	//逻辑/编码器计数器大于负方向比较寄存器数值
	IT_SMC = 0x0400,	//逻辑/编码器计数器大于负方向比较寄存器数值
	IT_SPC = 0x0800,	//逻辑/编码器计数器大于正方向比较寄存器数值
	IT_LPC = 0x1000,	//逻辑/编码器计数器大于正方向比较寄存器数值
	IT_CEND = 0x2000,	//加减速驱动时，定速域结束脉冲输出
	IT_CSTA = 0x4000,	//加减速驱动时，定速域开始脉冲输出
	IT_DEND = 0x8000,	//驱动结束
	IT_ALL = 0xff00		//所有
} IT_NUM;

//中断配置
void It_Config(IT_NUM it, STATE state);

typedef enum
{
	IT_PULSE_MASK = 0x0001,		//脉冲上升沿
	IT_LMC_MASK = 0x0002,		//逻辑/编码器计数器大于负方向比较寄存器数值
	IT_SMC_MASK = 0x0004,		//逻辑/编码器计数器大于负方向比较寄存器数值
	IT_SPC_MASK = 0x0008,		//逻辑/编码器计数器大于正方向比较寄存器数值
	IT_LPC_MASK = 0x0010,		//逻辑/编码器计数器大于正方向比较寄存器数值
	IT_CEND_MASK = 0x0020,		//加减速驱动是，定速域结束脉冲输出
	IT_CSTA_MASK = 0x0040,		//加减速驱动是，定速域开始脉冲输出
	IT_DEND_MASK = 0x0080,		//驱动结束
	IT_HOME_MASK = 0x0100,		//中断原点搜索
	IT_SYNC_MASK = 0x0200,		//同步激活
	IT_ALL_MASK =0x03ff,			//所有
	IT_ERR_MASK = 0
} IT_MASK;

//
IT_MASK IT_Axis_Is(void);

typedef enum
{
	ERR_LPC_MASK = 0x0001,		//逻辑/编码器计数器大于正方向比较寄存器数值
	ERR_SMC_MASK = 0x0002,		//逻辑/编码器计数器小于负方向比较寄存器数值
	ERR_PL_MASK = 0x0004,		//硬件正限位
	ERR_ML_MASK = 0x0008,		//硬件负限位
	ERR_ALARM_MASK = 0x0010,	//伺服报警
	ERR_EMG_MASK = 0x0020,		//急停
	ERR_ALL_MASK = 0x003f,		//所有
	ERR_ERR_MASK = 0
} ERR_MASK;

//
ERR_MASK Error_Is(void);

//----------------------------------------- 脉冲模式 ----------------------------------------
typedef enum
{
	DOUBLE_PULSE,		//双脉冲模式
	DIR_PULSE			//方向-脉冲模式
} PULSE_MODE;
//------------------------------------------ 电平列表 ---------------------------------------
typedef enum
{
	LOW,				//低电平
	HIGH				//高电平
} LEVEL;

//倍频
typedef enum
{
	MUL_1 = 0x0000,		//不倍频
	MUL_2 = 0x0400,		//2倍频
	MUL_4 = 0x0800		//4倍频
} ENCODER_MUL;

typedef struct
{
	PULSE_MODE Pulse_Mode;		//输入脉冲模式
	ENCODER_MUL Pulse_Mul;		//倍频数
} Encoder_InitTypeDef;

void Encoder_Init(Encoder_InitTypeDef Encoder_InitStruct);
void Encoder_SetCounter(int Counter_Value);
int Encoder_GetCounter(void);

//比较寄存器的比较对象
typedef enum
{
	LG_CNT = 0x0000,		//逻辑计数器
	EC_CNT = 0x0020	       //编码器计数器
} OBJ;
//比较寄存器设置
typedef struct
{
	STATE PComp_State;			//正方向比较状态
	STATE MComp_State;			//负方向比较状态
	OBJ Comp_Obj;				//比较对象
	int PComp_Value;			//正方向比较寄存器初值
	int MComp_Value;			//负方向比较寄存器初值
} Comp_InitTypeDef;
//初始化比较器设置
void Comp_Init(Comp_InitTypeDef Comp_InitStruct);
void Comp_Config(unsigned char *temp);
//停止模式
typedef enum
{
	SUD_STOP,		//立即停止
	DEC_STOP		//减速停止
} STOP_MODE;
//轴驱动初始化
typedef struct
{
	STOP_MODE Stop_Mode;		//限位停止模式
	LEVEL PLimit_Level;			//正向限位有效电平
	LEVEL MLimit_Level;			//负向限位有效电平
	PULSE_MODE Pulse_Mode;		//输出脉冲模式
	LEVEL Logic_Level;			//输出脉冲逻辑电平
	LEVEL Dir_Level;			//输出脉冲方向电平
} AXIS_InitTypeDef;
//轴驱动初始化
void AXIS_Init(AXIS_InitTypeDef AXIS_InitStuct);

//驱动参数
typedef struct
{
	unsigned int 	Range;			//倍率,			16000-8000000,	4bytes
	unsigned short 	Acce;			//加速度,		1-8000,			2bytes
	unsigned short 	Dece;			//减速度,		1-8000,			2bytes
	unsigned short 	Acce_Rate;		//加速度变化率,	1-65535,		2bytes
	unsigned short 	Dece_Rate;		//加速度变化率,	1-65535,		2bytes
	short			Acce_Offset;	//加速度误差，	-32768-+32767	2bytes
	unsigned short 	Init_Speed;		//初始速度,		1-8000,			2bytes
	unsigned short 	Speed;			//驱动速度,		1-8000,			2bytes
	unsigned short 	Man_Dece_Pos;	//手动减速点,	0-4294967295,	4bytes
} Drive_InitTypeDef;
//驱动初始化
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
//连续驱动
void Axis_Continuous_Drive(unsigned char *temp);
//外部信号驱动
void External_Signals_Drive(unsigned char *temp);
void Clear_External_Drive(void);
//停止模式
void AXIS_Stop(unsigned char *temp);
//软件限位
void Software_Limit(unsigned char *temp);
//外部信号清除实际位置
void EXT_CLR_RLP(void);


//驱动完成中断状态
void It_Finish(STATE state);
unsigned char ITP_Axis(unsigned char Data);
//插补函数
void ITP_2AxisLine(unsigned char *temp);
void ITP_3AxisLine(unsigned char *temp);
void ITP_CW(unsigned char *temp);
void ITP_CCW(unsigned char *temp);
unsigned char Itp_Stauts(void);

void ITP_Single_Step(unsigned char *temp);
void ITP_Step_External(void);
void ITP_Step_Command(void);

//IO读
char IO_ReadBit(unsigned int in);

//IO写
void IO_WriteBit(unsigned char out, unsigned short act);
void Iput_Filters_Config(void);
void Auto_Home_Search(unsigned char *temp);
void SYN_Action1(unsigned char *temp);
void SYN_Action2(unsigned char *temp);
void SYN_Action(unsigned char *temp);
#endif
