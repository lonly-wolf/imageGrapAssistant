/**
 *********************************************************************
 **���⣺tc6014.h
 **���ߣ�fyf
 **���ڣ�2014.10.28
 **�汾��v1.0
 **˵����tc6014 driver
 *********************************************************************
**/

#ifndef		_TC6014_H
#define		_TC6014_H

#define		BASE_ADDR	0x60000000  	//FSMC����ַ
#define		OFFSET		0X00020000		//ƫ�Ƶ�ַ

//---------------------------------------------- �Ĵ����б� ---------------------------------------
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

//---------------------------------------- ���б� -----------------------------------------
typedef enum
{
	AXIS_NONE = 0X0000,
	AXIS_X = 0X0100,		//X��
	AXIS_Y = 0X0200,		//Y��
	AXIS_X_Y = 0X0300,      //XY��
	AXIS_Z = 0X0400,		//Z��
	AXIS_X_Z = 0X0500,		//XZ��
	AXIS_Y_Z = 0X0600,		//YZ��
	AXIS_X_Y_Z = 0X0700,	//XYZ��
	AXIS_U = 0X0800,		//U��	
	AXIS_X_U = 0X0900,		//XU��
	AXIS_Y_U = 0X0A00,		//YU��
	AXIS_X_Y_U = 0X0B00,	//XYU��
	AXIS_Z_U = 0X0C00,		//ZU��
	AXIS_X_Z_U = 0X0D00,	//XZU��	
	AXIS_Y_Z_U = 0X0E00,	//YZU��
	AXIS_ALL = 0X0F00,		//XYZU��
} AXIS_NUM;

//---------------------------------------------- �����б� -----------------------------------------
typedef enum
{
	//-------------------------Write Commands----------------------------
	WR,					//00h  	��Χ����
	WK,					//01 	���ٶ�����������
	WA, 				//02  	���ٶ�����
	WD, 				//03  	���ٶ�����
	WSV,  				//04  	�����ٶ�����
	WV, 				//05  	�����ٶ�����
	WP, 				//06  	������������û����յ�����
	WDP,				//07  	���ٵ�����
	WC, 				//08  	Բ������
	WLP,				//09  	�߼�λ�ü�������ֵ����
	WEP,				//0A  	��������������ֵ����
	WCP,				//0B  	����ȽϼĴ�����ֵ����
	WCM,				//0C  	����ȽϼĴ�������
	WAO,				//0D  	���ٶȼĴ���ƫ������
	WL, 				//0E  	���ٶȼ���������
	WEM = 0x60,			//60  	��չģʽ���ã�λ���ݣ�
	WHV,				//61  	ԭ�������ٶ�
	WSM	= 0x64,			//64  	ͬ��ģʽ���ã�λ���ݣ�
	 
	//------------------------Data Reading Commands--------------------------
	RLP = 0x10, 		//10h  	��ȡ��ǰ�߼�λ�ü�������ֵ
	REP,  				//11  	��ȡ��������ֵ
	RV,  				//12  	��ȡ��ǰ�����ٶ�
	RA,  				//13  	���Ӽ��ٶ���ֵ
	RSB,  				//14  	��ͬ������Ĵ���
	
	//------------------------Driving Commands-------------------------------- 
	PFD	= 0x20,			//20h  	������̶���������
	MFD,				//21  	������̶���������
	PCD,				//22  	��������������
	MCD,				//23  	��������������
	DSH,				//24  	������������
	DSHR,				//25 	�������������ͷ�
	DECS,				//26  	����ֹͣ
	SUDS,				//27  	����ֹͣ
	ALLSTOP,    //28    ���е�������ֹͣ
	ALLDECSTOP, //29    ���е������ֹͣ
	
	//------------------------Interpolation Commands------------------------- 
	I2L	= 0x30,			//30h  	2��ֱ�߲岹
	I3L,				//31  	3��ֱ�߲岹
	ICW,				//32  	˳ʱ�뷽��Բ���岹
	ICCW,				//33  	��ʱ�뷽��Բ���岹
	I2B,				//34  	2��λ�岹
	I3B,				//35  	3��λ�岹
	BPRWEN,				//36  	λģʽ�Ĵ���д
	BPRWDIS,			//37  	λģʽ�Ĵ���дʧ��
	BPDS,				//38    λģʽ�岹�Ĵ���ѹջ
	BPDC,				//39  	λģʽ�岹���ݼĴ������
	ISS,				//3A  	�����岹
	DECV,				//3B  	�岹������Ч
	DECINV,				//3C  	�岹������Ч
	IIC,				//3D  	�岹�ж����
	
	//-----------------------Other commands---------------------------------
	AHOME = 0x62,		//62  	�Զ�ԭ������
	DCCOUT,				//63  	ƫ�Ƽ������������
	SA = 0x65,			//65  	����ͬ������
	AS = 0x0f			//0F  	���л�
} CMD_NUM;

typedef enum
{
	Disable,			//ʧ��
	Enable				//ʹ��
} STATE;

//д�Ĵ���
void Reg_Write(REG reg, short dat);
//д����
void CMD_Write(CMD_NUM cmd);

void AXIS_Switch(void);
//д������
void Data_Write(CMD_NUM cmd, int dat);
//���Ĵ���
unsigned short Reg_Read(REG reg);
//��������
int Data_Read(CMD_NUM cmd);
//�����λ

//������
void Set_Axis(AXIS_NUM axis);
void Software_Reset(void);
//��Χ����
void Range_Set(AXIS_NUM axis,int data);

//���ٶ�����������
void INC_Rate(AXIS_NUM axis,int data);

//���ٶȼ���������
void DEC_Rate(AXIS_NUM axis,int data);

//���ٶ�����
void INC_Set(AXIS_NUM axis,int data);

//���ٶ�����
void DEC_Set(AXIS_NUM axis,int data);

//�����ٶ�����
void Start_Speed(AXIS_NUM axis,int data);

//�����ٶ�����
void Drive_Speed(AXIS_NUM axis,int data);

//������������û����յ�����
void Pulse_Out(AXIS_NUM axis,int data);

//���ٵ�����
void DECP_Set(AXIS_NUM axis,int data);

//Բ������
void Center_Set(AXIS_NUM axis,int data);

//�߼�λ�ü�������ֵ����
void LP_Set(AXIS_NUM axis,int data);

//��������������ֵ����
void EP_Set(AXIS_NUM axis,int data);

//����ȽϼĴ�����ֵ����
void CompP_Set(AXIS_NUM axis,int data);

//����ȽϼĴ�������
void CompM_Set(AXIS_NUM axis,int data);

//���ٶȼĴ���ƫ������
void ACC_Offset(AXIS_NUM axis,int data);

//��չģʽ���ã�λ���ݣ�
void EXP_Mode(AXIS_NUM axis,int data);

//ԭ�������ٶ�
void Home_SearchSpeed(AXIS_NUM axis,int data);

//ͬ��ģʽ���ã�λ���ݣ
void SYN_Mode(AXIS_NUM axis,int data);

//��ʵ��λ�� ��ȡ��������ֵ
int Read_REP(AXIS_NUM axis);

//���߼�λ�� ��ȡ��ǰ�߼�λ�ü�������ֵ
int Read_RLP(AXIS_NUM axis);

//��ȡ��ǰ�����ٶ�
int Read_RV(AXIS_NUM axis);

//���Ӽ��ٶ���ֵ
int Read_RA(AXIS_NUM axis);

//��ͬ������Ĵ���
int Read_RSB(AXIS_NUM axis);

void AXIS_Speed_Set(unsigned char *temp);

// �ȴ�������ֹͣ
void Wait(AXIS_NUM axis);

// Next data setting of waiting for continuous interpolation
//�ȴ������岹��һ�����ݵ����� RR0/D9
void Next_Wait(void);

// Next data setting of waiting for BP interpolation
//�ȴ�λģʽ�岹��һ���������� RR0/D13-D14  SC
void BP_Wait(void);

typedef enum
{
	IT_PULSE = 0x0100,	//�����������ж�
	IT_LMC = 0x0200,	//�߼�/���������������ڸ�����ȽϼĴ�����ֵ
	IT_SMC = 0x0400,	//�߼�/���������������ڸ�����ȽϼĴ�����ֵ
	IT_SPC = 0x0800,	//�߼�/����������������������ȽϼĴ�����ֵ
	IT_LPC = 0x1000,	//�߼�/����������������������ȽϼĴ�����ֵ
	IT_CEND = 0x2000,	//�Ӽ�������ʱ������������������
	IT_CSTA = 0x4000,	//�Ӽ�������ʱ��������ʼ�������
	IT_DEND = 0x8000,	//��������
	IT_ALL = 0xff00		//����
} IT_NUM;

//�ж�����
void It_Config(IT_NUM it, STATE state);

typedef enum
{
	IT_PULSE_MASK = 0x0001,		//����������
	IT_LMC_MASK = 0x0002,		//�߼�/���������������ڸ�����ȽϼĴ�����ֵ
	IT_SMC_MASK = 0x0004,		//�߼�/���������������ڸ�����ȽϼĴ�����ֵ
	IT_SPC_MASK = 0x0008,		//�߼�/����������������������ȽϼĴ�����ֵ
	IT_LPC_MASK = 0x0010,		//�߼�/����������������������ȽϼĴ�����ֵ
	IT_CEND_MASK = 0x0020,		//�Ӽ��������ǣ�����������������
	IT_CSTA_MASK = 0x0040,		//�Ӽ��������ǣ�������ʼ�������
	IT_DEND_MASK = 0x0080,		//��������
	IT_HOME_MASK = 0x0100,		//�ж�ԭ������
	IT_SYNC_MASK = 0x0200,		//ͬ������
	IT_ALL_MASK =0x03ff,			//����
	IT_ERR_MASK = 0
} IT_MASK;

//
IT_MASK IT_Axis_Is(void);

typedef enum
{
	ERR_LPC_MASK = 0x0001,		//�߼�/����������������������ȽϼĴ�����ֵ
	ERR_SMC_MASK = 0x0002,		//�߼�/������������С�ڸ�����ȽϼĴ�����ֵ
	ERR_PL_MASK = 0x0004,		//Ӳ������λ
	ERR_ML_MASK = 0x0008,		//Ӳ������λ
	ERR_ALARM_MASK = 0x0010,	//�ŷ�����
	ERR_EMG_MASK = 0x0020,		//��ͣ
	ERR_ALL_MASK = 0x003f,		//����
	ERR_ERR_MASK = 0
} ERR_MASK;

//
ERR_MASK Error_Is(void);

//----------------------------------------- ����ģʽ ----------------------------------------
typedef enum
{
	DOUBLE_PULSE,		//˫����ģʽ
	DIR_PULSE			//����-����ģʽ
} PULSE_MODE;
//------------------------------------------ ��ƽ�б� ---------------------------------------
typedef enum
{
	LOW,				//�͵�ƽ
	HIGH				//�ߵ�ƽ
} LEVEL;

//��Ƶ
typedef enum
{
	MUL_1 = 0x0000,		//����Ƶ
	MUL_2 = 0x0400,		//2��Ƶ
	MUL_4 = 0x0800		//4��Ƶ
} ENCODER_MUL;

typedef struct
{
	PULSE_MODE Pulse_Mode;		//��������ģʽ
	ENCODER_MUL Pulse_Mul;		//��Ƶ��
} Encoder_InitTypeDef;

void Encoder_Init(Encoder_InitTypeDef Encoder_InitStruct);
void Encoder_SetCounter(int Counter_Value);
int Encoder_GetCounter(void);

//�ȽϼĴ����ıȽ϶���
typedef enum
{
	LG_CNT = 0x0000,		//�߼�������
	EC_CNT = 0x0020	       //������������
} OBJ;
//�ȽϼĴ�������
typedef struct
{
	STATE PComp_State;			//������Ƚ�״̬
	STATE MComp_State;			//������Ƚ�״̬
	OBJ Comp_Obj;				//�Ƚ϶���
	int PComp_Value;			//������ȽϼĴ�����ֵ
	int MComp_Value;			//������ȽϼĴ�����ֵ
} Comp_InitTypeDef;
//��ʼ���Ƚ�������
void Comp_Init(Comp_InitTypeDef Comp_InitStruct);
void Comp_Config(unsigned char *temp);
//ֹͣģʽ
typedef enum
{
	SUD_STOP,		//����ֹͣ
	DEC_STOP		//����ֹͣ
} STOP_MODE;
//��������ʼ��
typedef struct
{
	STOP_MODE Stop_Mode;		//��λֹͣģʽ
	LEVEL PLimit_Level;			//������λ��Ч��ƽ
	LEVEL MLimit_Level;			//������λ��Ч��ƽ
	PULSE_MODE Pulse_Mode;		//�������ģʽ
	LEVEL Logic_Level;			//��������߼���ƽ
	LEVEL Dir_Level;			//������巽���ƽ
} AXIS_InitTypeDef;
//��������ʼ��
void AXIS_Init(AXIS_InitTypeDef AXIS_InitStuct);

//��������
typedef struct
{
	unsigned int 	Range;			//����,			16000-8000000,	4bytes
	unsigned short 	Acce;			//���ٶ�,		1-8000,			2bytes
	unsigned short 	Dece;			//���ٶ�,		1-8000,			2bytes
	unsigned short 	Acce_Rate;		//���ٶȱ仯��,	1-65535,		2bytes
	unsigned short 	Dece_Rate;		//���ٶȱ仯��,	1-65535,		2bytes
	short			Acce_Offset;	//���ٶ���	-32768-+32767	2bytes
	unsigned short 	Init_Speed;		//��ʼ�ٶ�,		1-8000,			2bytes
	unsigned short 	Speed;			//�����ٶ�,		1-8000,			2bytes
	unsigned short 	Man_Dece_Pos;	//�ֶ����ٵ�,	0-4294967295,	4bytes
} Drive_InitTypeDef;
//������ʼ��
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
//��������
void Axis_Continuous_Drive(unsigned char *temp);
//�ⲿ�ź�����
void External_Signals_Drive(unsigned char *temp);
void Clear_External_Drive(void);
//ֹͣģʽ
void AXIS_Stop(unsigned char *temp);
//�����λ
void Software_Limit(unsigned char *temp);
//�ⲿ�ź����ʵ��λ��
void EXT_CLR_RLP(void);


//��������ж�״̬
void It_Finish(STATE state);
unsigned char ITP_Axis(unsigned char Data);
//�岹����
void ITP_2AxisLine(unsigned char *temp);
void ITP_3AxisLine(unsigned char *temp);
void ITP_CW(unsigned char *temp);
void ITP_CCW(unsigned char *temp);
unsigned char Itp_Stauts(void);

void ITP_Single_Step(unsigned char *temp);
void ITP_Step_External(void);
void ITP_Step_Command(void);

//IO��
char IO_ReadBit(unsigned int in);

//IOд
void IO_WriteBit(unsigned char out, unsigned short act);
void Iput_Filters_Config(void);
void Auto_Home_Search(unsigned char *temp);
void SYN_Action1(unsigned char *temp);
void SYN_Action2(unsigned char *temp);
void SYN_Action(unsigned char *temp);
#endif
