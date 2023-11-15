#ifndef __PARAMETER_H__
#define __PARAMETER_H__
#include "fm33le0xx_fl.h"


typedef struct
{
	uint32_t A_Vol;//A相电压
	uint32_t B_Vol;//B相电压
	uint32_t C_Vol;//C相电压
	float A_Cur;//A相电流
	float B_Cur;//B相电流
	float C_Cur;//C相电流
	uint32_t A_Phase;//A相相位
	uint32_t B_Phase;//B相相位
	uint32_t C_Phase;//C相相位
	uint32_t Vol_err;//报警电压
	float Cur_err;//报警电流
}gUOCParameter_Check_TypeDef;



typedef struct
{
    unsigned char	FanType;//F00:2
	unsigned short	RatedCurrent1;//F01:999.9
	unsigned short	RatedCurrent2;//F02:999.9
	unsigned short	CurTransRatedCurrent;//F03:Current transformer rated current 999
	unsigned char	CurTransPierceTimes1;//F04:9
	unsigned char	CurTransPierceTimes2;//F05:9
	unsigned char	Restart:2;//F06:1
	unsigned char	RestoreFactorySettings:2;//F07:1
	unsigned char	VoltageFaultState:2;//F08:1
	unsigned char	FanStopState:2;//F09:1
	unsigned short	CurCorParam;//F10:Current correction parameter 9999
	unsigned char	CurOverShortDelayFlag;//F11:1
	unsigned char	CurOverShortDelayTime;//F12:99
	unsigned char	CurOverLongDelayFlag;//F13:1
	unsigned short	CurOverLongDelayTime;//F14:999
	unsigned char	MotorShortCircuitFlag;//F15:1
	unsigned char	ShortCircuitMax;//F16:9
	unsigned char	ShortCircuitDelay;//F17:50
	unsigned short	DischargeSmokeOutputTime;//F18:？
	unsigned short	DischargeSmokeFeedbackTime;//F19:？
	unsigned short	VolCorParam;//F20:Voltage correction parameter 9999
	unsigned char	VolOverFlag;//F21:1
	unsigned short	VolOverMax;//F22:999
	unsigned short	VolOverTime;//F23:9999
	unsigned char	VolUnderFlag;//F24:1
	unsigned short	VolUnderMax;//F25:999
	unsigned short	VolUnderTime;//F26:9999
	unsigned short   Phase_num;//F27 相角度
	unsigned char	WrongPhaseFlag:4;//F27:1
	unsigned char	LackPhaseFlag:4;//F28:1
	unsigned short	WrongPhaseTime;//F29:9999
	unsigned short	LackPhaseTime;//F30:9999
	unsigned char	CurEquipFlag;//F31:1
	unsigned char	CurEquipTime;//F32:60
	unsigned char	FaultTrippingFlag;//F33:1
	unsigned char	DI9InputDelay;//F34:0.5  max 20
	unsigned char	DI10InputDelay;//F35:0.5  max 20
	unsigned char	DI11InputDelay;//F36:0.5  max 20
	unsigned char	DI12InputDelay;//F37:0.5  max 20
	unsigned char	DI13InputDelay;//F38:0.5  max 20
	unsigned char	DI14InputDelay;//F39:0.5  max 20
	unsigned char	DI7InputDelay;//F45:0.5  max 20
	unsigned char	DI8InputDelay;//F46:0.5  max 20
	unsigned char	ButtonInputDelay;//F47:0.5  max 20
	unsigned char	R03OutputDef;//F50:6
	unsigned char	R04OutputDef;//F51:6
	unsigned char	R05OutputDef;//F52:6
	unsigned char	R06OutputDef;//F53:6
	unsigned char	DI7InputSet;//F60:7
	unsigned char	DI8InputSet;//F61:7
	unsigned char	DI9InputSet;//F62:7
	unsigned char	DI10InputSet;//F63:7
	unsigned char	DI11InputSet;//F64:7
	unsigned char	DI12InputSet;//F65:7
	unsigned char	DI13InputSet;//F66:7
	unsigned char	DI14InputSet;//F67:7
} gUOCParameter_TypeDef;

typedef enum
{ 
	F_00H = 0, 
	F_01H,
	F_02H,
	F_03H,
	F_04H,
	F_05H,
	F_06H,
	F_07H,
	F_08H,
	F_09H, 
	F_10H, 
	F_11H,
	F_12H,
	F_13H,
	F_14H,
	F_15H,
	F_16H,
	F_17H,
	F_18H,
	F_19H,
	F_20H, 
	F_21H,
	F_22H,
	F_23H,
	F_24H,
	F_25H,
	F_26H,
	F_27H,
	F_28H,
	F_29H,
	F_30H, 
	F_31H,
	F_32H,
	F_33H,
	F_34H,
	F_35H,
	F_36H,
	F_37H,
	F_38H,
	F_39H,
	F_40H, 
	F_41H,
	F_42H,
	F_43H,
	F_44H,
	F_45H,
	F_46H,
	F_47H,
	F_48H,
	F_49H,
	F_50H, 
	F_51H,
	F_52H,
	F_53H,
	F_54H,
	F_55H,
	F_56H,
	F_57H,
	F_58H,
	F_59H,
	F_FREE,
	F_ERR,
	F_MAX,
} ENUM_F;



#endif
