#ifndef __SYSTEM_CONTROL_H__
#define __SYSTEM_CONTROL_H__
#include "fm33le0xx_fl.h"

// #define DI0()	FL_GPIO_GetInputPin(GPIOD, FL_GPIO_PIN_1)
// #define DI1()	FL_GPIO_GetInputPin(GPIOD, FL_GPIO_PIN_0)
// #define DI2()	FL_GPIO_GetInputPin(GPIOD, FL_GPIO_PIN_11)
// #define DI3()	FL_GPIO_GetInputPin(GPIOD, FL_GPIO_PIN_10)
// #define DI4()	FL_GPIO_GetInputPin(GPIOD, FL_GPIO_PIN_9)
#define DI5()	FL_GPIO_GetInputPin(GPIOB, FL_GPIO_PIN_0)
#define DI6()	FL_GPIO_GetInputPin(GPIOB, FL_GPIO_PIN_1)
#define DI7()	FL_GPIO_GetInputPin(GPIOB, FL_GPIO_PIN_2)
#define DI8()	FL_GPIO_GetInputPin(GPIOB, FL_GPIO_PIN_3)
#define DI9()	FL_GPIO_GetInputPin(GPIOB, FL_GPIO_PIN_4)
#define DI10()	FL_GPIO_GetInputPin(GPIOB, FL_GPIO_PIN_5)
#define DI11()	FL_GPIO_GetInputPin(GPIOB, FL_GPIO_PIN_6)
#define DI12()	FL_GPIO_GetInputPin(GPIOB, FL_GPIO_PIN_7)
#define DI13()	FL_GPIO_GetInputPin(GPIOB, FL_GPIO_PIN_12)
#define DI14()	FL_GPIO_GetInputPin(GPIOC, FL_GPIO_PIN_1)

#define CONTORL_HIGH	1
#define CONTORL_LOW		0

#define STATE_LOW 0
#define STATE_HIGH 1

#define OPEN		1
#define CLOSE		0


#define keep_time 5


typedef enum
{
    UOC_R00 = 0,
    UOC_R01,
    UOC_R02,
    UOC_R03,
    UOC_R04,
    UOC_R05,
    UOC_R06,
    UOC_D00,
    UOC_D01,
    UOC_D02,
    UOC_D03,
    UOC_D04,
    UOC_D05,
	UOC_D06,
	UOC_D07,
	UOC_D08,
    UOC_CONTROL_MAX,
} eUOCControlId_TypeDef;

typedef struct
{
    unsigned char ControlName;
	GPIO_Type * ControlGpio;
	uint32_t	ControlPin;
} gUOCControl_TypeDef;

typedef enum
{
    UOC_HIGH_SPEED_RUN= 0,	//高速运行
    UOC_LOW_SPEED_RUN,		//低速运行
    UOC_AUTOMATIC_RUN,		//自动运行
    UOC_MANUAL_RUN,			//手动运行
    UOC_CONTROLLED_RUN,		//受控反馈
    UOC_FAULT_FEEDBACK,		//故障反馈
    UOC_WORK_NORMALLY,		//正常工作
    UOC_FAN_OVERLOAD,		//风机过载
	UOC_HIGH_SPEED_STOP,	//高速停止
	UOC_LOW_SPEED_STOP,		//低速停止
	UOC_SMOKE_EXHAUST_VALVE_CLOSE,//排烟阀关
	UOC_SMOKE_EXHAUST_VALVE_OPEN,//排烟阀开
	UOC_HIGH_SPEED_OVERCURRENT,	//高速过流
	UOC_LOW_SPEED_OVERCURRENT,	//低速过流
	UOC_POWER_NORMAL,		//电源正常
    UOC_R03R04R05R06_STATE_MAX,
} eUOCR03R04R05Id_Def;


typedef struct
{
    unsigned char mode_flag:1;		//0手动 1自动
    unsigned char ready_high:1;	//高速风机/消防风机准备启动 0关闭，1启动
    unsigned char high_speed_fan:1;	//高速风机/消防风机 0关闭，1启动 实际真正开启高速风机通过检测电流  用于报警
    unsigned char ready_Ordinary:1;	//普通风机准备启动 0关闭 1开启   实际真正开启普通风机通过检测电流用于报警
    unsigned char Ordinary_fan:1;	//普通风机 0关闭，1启动
    unsigned char fan_flag:2;	//1:低速风机转高速风机时启动计时标志 2:启动风扇后延时检测
    unsigned char Fire_Damper_flag:1;//防火阀是否开启 1开启 0关闭
    unsigned char run_flag:1;	//设备运行反馈 1开启，0关闭
    unsigned char Smoke_Exhaust_Valve_Stop_flag:1; //排烟阀关闭是否到位 0到位，1未到位，默认为0 外部输入控制
    unsigned char Smoke_Exhaust_Valve_Start_flag:1;//排烟阀开启是否到位 0到位，1未到位，默认为0 外部输入控制
    unsigned char Smoke_Exhaust_Valve_Stop_check_flag:1; //排烟阀关闭是否检测 1检测，0不需要检测
    unsigned char Smoke_Exhaust_Valve_Start_check_flag:1;//排烟阀开启是否检测 1检测，0不需要检测
	unsigned char Auto_trip:1;//是否自动跳闸，1:是 所以按键启动失效
	unsigned char Vol_Normal_flag:1;//电压是否正常 1:正常，0:异常
	unsigned char res:1;
    unsigned int Smoke_timeCount;//排烟阀开启or关闭计时器
	unsigned int fan_timeCount;//风机开启计时器 与fan_flag一起共用
	void (*static_clean)(void);
	void (*static_res)(unsigned char i);
} gUOCControl_StateDef;


typedef enum
{ 
	UOC_DI0 = 0, 
	UOC_DI1,
	UOC_DI2,
	UOC_DI3,
	UOC_DI4,
	UOC_DI5,
	UOC_DI6,
	UOC_DI7,
	UOC_DI8,
	UOC_DI9,
	UOC_DI10,
	UOC_DI11,
	UOC_DI12,
	UOC_DI13,
	UOC_DI14,
	UOC_DIMAX,
} ENUM_DI;


typedef struct
{
    unsigned char name;
	unsigned short timeflag:1;
	unsigned short timeCount:15;
    void (*cb)(void);
} sDInputEvent_TypeDef;

typedef enum
{ 
	External_Control_Stop = 0, 	//外控停止
	External_Control_Start,		//外控启动
	Emergency_Start,			//应急启动
	Dynamic_Control,			//联动控制
	Low_Speed_Fan,				//低速风机
	High_Speed_Fan,				//高速风机
	Controlled_Feedback,		//受控反馈
	Fire_Damper,				//防火阀
	High_Speed_Fan_Dynamic_Start,//消防联动手动开启
	High_Speed_Fan_Dynamic_Stop,//消防联动手动关闭
	Low_Speed_Fan_Dynamic_Start,//低速联动手动开启
	Smoke_Exhaust_Valve_Stop,	//排烟阀关
	Smoke_Exhaust_Valve_Start,	//排烟阀开
	DI7_DI14MAX,				//DI7-DI14功能最大值
} ENUM_DI7_DI14;

typedef struct 
{
    uint8_t head;
    uint8_t data;
    uint8_t end;
}F12_PROTOCOL;

void Input_Detection(void);
void Output_Control(unsigned char name,unsigned char flag);
void Control_High_Speed_Fan(unsigned char flag);
void Control_Ordinary_Fan(unsigned char flag);
void UOC_Trip_Fan(void);
void Control_R03R04R05R06_Function(unsigned id,unsigned char flag);

void UOC_DI0_FUNC(void);
void UOC_DI1_FUNC(void);
void UOC_DI2_FUNC(void);
void UOC_DI3_FUNC(void);
void UOC_DI4_FUNC(void);
void UOC_DI5_FUNC(void);
void UOC_DI6_FUNC(void);
void UOC_DI7_FUNC(void);
void UOC_DI8_FUNC(void);
void UOC_DI9_FUNC(void);
void UOC_DI10_FUNC(void);
void UOC_DI11_FUNC(void);
void UOC_DI12_FUNC(void);
void UOC_DI13_FUNC(void);
void UOC_DI14_FUNC(void);


#endif
