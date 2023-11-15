#include"button_function.h"
#include"timer.h"
#include "debug.h"
#include"led.h"
#include"i2c.h"
#include"spi.h"
#include "parameter.h"
#include "system_control.h"
#include "mf_config.h"
#include "flash_api.h"

#define Wait_time 20000

extern eUOCDisplayErr_TypeDef UOC_ERR_PARA[EMAX];
extern gUOCParameter_TypeDef g_uoc_param;
extern gUOCParameter_TypeDef g_uoc_init_param;
extern gUOCParameter_Check_TypeDef UOC_Check_Para;
extern gUOCControl_StateDef UOC_Input_State;
sKeyFlag_TypeDef uoc_key[key_max]={0};
unsigned char key_state = 0;

sParaFlag_TypeDef UOCPara={F_FREE,0,F_FREE,0,0,0,0,0,0,0};

static sButtonEvent_TypeDef Button_deal[F_MAX] =
{
    {F_00H,1,1, UOC_paremeter_F00_FUNC},
    {F_01H,1,4, UOC_paremeter_F01_FUNC},
    {F_02H,1,4, UOC_paremeter_F02_FUNC},
    {F_03H,1,4, UOC_paremeter_F03_FUNC},
    {F_04H,1,1, UOC_paremeter_F04_FUNC},
    {F_05H,0,1, UOC_paremeter_F05_FUNC},
	{F_06H,1,1, UOC_paremeter_F06_FUNC},
	{F_07H,1,1, UOC_paremeter_F07_FUNC},
	{F_08H,1,1, UOC_paremeter_F08_FUNC},
	{F_09H,1,2, UOC_paremeter_F09_FUNC},
	{F_10H,1,2, UOC_paremeter_F10_FUNC},
    {F_11H,1,1, UOC_paremeter_F11_FUNC},
    {F_12H,1,1, UOC_paremeter_F12_FUNC},
    {F_13H,1,1, UOC_paremeter_F13_FUNC},
    {F_14H,1,1, UOC_paremeter_F14_FUNC},
    {F_15H,1,4, UOC_paremeter_F15_FUNC},
    {F_16H,1,1, UOC_paremeter_F16_FUNC},
    {F_17H,1,3, UOC_paremeter_F17_FUNC},
    {F_18H,1,1, UOC_paremeter_F18_FUNC},
    {F_19H,1,3, UOC_paremeter_F19_FUNC},
    {F_20H,1,1, UOC_paremeter_F20_FUNC},
    {F_21H,1,1, UOC_paremeter_F21_FUNC},
    {F_22H,1,2, UOC_paremeter_F22_FUNC},
    {F_23H,1,3, UOC_paremeter_F23_FUNC},
    {F_24H,1,3, UOC_paremeter_F24_FUNC},
    {F_25H,1,4, UOC_paremeter_F25_FUNC},
    {F_26H,1,1, UOC_paremeter_F26_FUNC},
    {F_27H,1,3, UOC_paremeter_F27_FUNC},
    {F_28H,1,4, UOC_paremeter_F28_FUNC},
    {F_29H,1,1, UOC_paremeter_F29_FUNC},
    {F_30H,1,3, UOC_paremeter_F30_FUNC},
    {F_31H,1,4, UOC_paremeter_F31_FUNC},
    {F_32H,1,1, UOC_paremeter_F32_FUNC},
    {F_33H,1,4, UOC_paremeter_F33_FUNC},
	{F_34H,1,4, UOC_paremeter_F34_FUNC},
	{F_35H,1,2, UOC_paremeter_F35_FUNC},
	{F_36H,1,2, UOC_paremeter_F36_FUNC},
	{F_37H,1,2, UOC_paremeter_F37_FUNC},
	{F_38H,1,2, UOC_paremeter_F38_FUNC},
	{F_39H,1,2, UOC_paremeter_F39_FUNC},
	{F_40H,1,2, UOC_paremeter_F40_FUNC},
	{F_41H,1,2, UOC_paremeter_F41_FUNC},
    {F_42H,1,2, UOC_paremeter_F42_FUNC},
    {F_43H,1,2, UOC_paremeter_F43_FUNC},
    {F_44H,1,2, UOC_paremeter_F44_FUNC},
    {F_45H,1,2, UOC_paremeter_F45_FUNC},
    {F_46H,1,2, UOC_paremeter_F46_FUNC},
	{F_47H,0,4, UOC_paremeter_F47_FUNC},
	{F_48H,0,4, UOC_paremeter_F48_FUNC},
    {F_49H,0,4, UOC_paremeter_F49_FUNC},
	{F_50H,1,2, UOC_paremeter_F50_FUNC},
	{F_51H,1,2, UOC_paremeter_F51_FUNC},
	{F_52H,1,2, UOC_paremeter_F52_FUNC},
	{F_53H,1,2, UOC_paremeter_F53_FUNC},
	{F_54H,1,2, UOC_paremeter_F54_FUNC},
	{F_55H,1,2, UOC_paremeter_F55_FUNC},
	{F_56H,1,2, UOC_paremeter_F56_FUNC},
	{F_57H,1,2, UOC_paremeter_F57_FUNC},
	{F_58H,0,4, UOC_paremeter_F58_FUNC},
    {F_59H,0,4, UOC_paremeter_F59_FUNC},
	{F_FREE,1,4, UOC_paremeter_FREE_FUNC},
	{F_ERR,1,4, UOC_paremeter_ERR_FUNC},
};


void Key_timemout_deal(void)
{
	unsigned char i=0;
	for(i=0;i<key_max;i++)
	{
		if(uoc_key[i].timeout > delay_time+5)
		{
			uoc_key[i].timeflag = 0;
			uoc_key[i].timeout = 0;
		}
	}
}
void Key_press(void)
{	
	if(KEY1() == key_down && uoc_key[0].flag ==0)
	{
		uoc_key[0].timeflag = 1;
		if(uoc_key[0].timeout > delay_time)
		{
			uoc_key[0].timeflag = 0;
			uoc_key[0].timeout = 0;
			if(KEY1() == key_down)
			{
				uoc_key[0].flag = 1;
				key_state = UOC_KEY_1;
			}
		}
		
	}
	else if(KEY2() == key_down && uoc_key[1].flag ==0)
	{
		uoc_key[1].timeflag = 1;
		if(uoc_key[1].timeout > delay_time)
		{
			uoc_key[1].timeflag = 0;
			uoc_key[1].timeout = 0;
			if(KEY2() == key_down)
			{
				uoc_key[1].flag = 1;
				key_state = UOC_KEY_2;
			}
		}
	}
	else if(KEY3() == key_down && uoc_key[2].flag ==0)
	{
		uoc_key[2].timeflag = 1;
		if(uoc_key[2].timeout > delay_time)
		{
			uoc_key[2].timeflag = 0;
			uoc_key[2].timeout = 0;
			if(KEY3() == key_down)
			{
				uoc_key[2].flag = 1;
				key_state = UOC_KEY_3;
			}
		}
	}
	else if(KEY4() == key_down && uoc_key[3].flag ==0)
	{
		uoc_key[3].timeflag = 1;
		if(uoc_key[3].timeout > delay_time)
		{
			uoc_key[3].timeflag = 0;
			uoc_key[3].timeout = 0;
			if(KEY4() == key_down)
			{
				uoc_key[3].flag = 1;
				key_state = UOC_KEY_4;
			}
		}
	}
}
void Key_release(void)
{
	if(KEY1() == key_up && uoc_key[0].flag ==1)
	{
		uoc_key[0].timeflag = 1;
		if(uoc_key[0].timeout > delay_time)
		{
			uoc_key[0].timeflag = 0;
			uoc_key[0].timeout = 0;
			if(KEY1() == key_up)
			{
				uoc_key[0].flag = 0;
			}
		}
	}
	else if(KEY2() == key_up && uoc_key[1].flag ==1)
	{
		uoc_key[1].timeflag = 1;
		if(uoc_key[1].timeout > delay_time)
		{
			uoc_key[1].timeflag = 0;
			uoc_key[1].timeout = 0;
			if(KEY2() == key_up)
			{
				uoc_key[1].flag = 0;
			}
		}
	}
	else if(KEY3() == key_up && uoc_key[2].flag ==1)
	{
		uoc_key[2].timeflag = 1;
		if(uoc_key[2].timeout > delay_time)
		{
			uoc_key[2].timeflag = 0;
			uoc_key[2].timeout = 0;
			if(KEY3() == key_up)
			{
				uoc_key[2].flag = 0;
			}
		}
	}
	else if(KEY4() == key_up && uoc_key[3].flag ==1)
	{
		uoc_key[3].timeflag = 1;
		if(uoc_key[3].timeout > delay_time)
		{
			uoc_key[3].timeflag = 0;
			uoc_key[3].timeout = 0;
			if(KEY4() == key_up)
			{
				uoc_key[3].flag = 0;
			}
		}
	}
}
void UOC_paremeter_Display(uint8_t data1,uint8_t data2,uint8_t data3,uint8_t data4)
{
	I2C_Display(DISPLAY1,DISPLAY_NO_POINT,data1);
	I2C_Display(DISPLAY2,DISPLAY_NO_POINT,data2);
	if(((F_01H == UOCPara.new_state) || (F_02H ==UOCPara.new_state) || 
		(F_22H ==UOCPara.new_state) || (F_33H ==UOCPara.new_state) || (F_34H ==UOCPara.new_state) ||
		((F_36H<=UOCPara.new_state) && (UOCPara.new_state<= F_44H))) && UOCPara.para_state == 1)
		if(data3 == DISPLAY_OFF)
			I2C_Display(DISPLAY3,DISPLAY_NO_POINT,data3);
		else
			I2C_Display(DISPLAY3,DISPLAY_POINT,data3);
	else
		I2C_Display(DISPLAY3,DISPLAY_NO_POINT,data3);
	I2C_Display(DISPLAY4,DISPLAY_NO_POINT,data4);
	return;
}

void UOC_paremeter_F00_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.FanType;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				g_uoc_param.FanType = UOCPara.pare_num;
				UOCPara.last_state = F_MAX;//成功
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num++;
				if(UOCPara.pare_num > 2)
					UOCPara.pare_num = 1;
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F01_FUNC(void)
{	
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.RatedCurrent1;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				if(UOCPara.pare_num >= 10){
					g_uoc_param.RatedCurrent1 = UOCPara.pare_num;
					UOCPara.last_state = F_MAX;//成功
				}
				else{//失败
					UOCPara.para_state = 1;
					UOCPara.para_fail = 1;
				}
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F02_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.RatedCurrent2;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				if(UOCPara.pare_num >= 10){
					g_uoc_param.RatedCurrent2 = UOCPara.pare_num;
					UOCPara.last_state = F_MAX;//成功
				}
				else{//失败
					UOCPara.para_state = 1;
					UOCPara.para_fail = 1;
				}
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F03_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.CurTransRatedCurrent;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				if((UOCPara.pare_num >= 100) && (UOCPara.pare_num <= 3000)){
					g_uoc_param.CurTransRatedCurrent = UOCPara.pare_num;
					UOCPara.last_state = F_MAX;//成功
				}
				else{//失败
					UOCPara.para_state = 1;
					UOCPara.para_fail = 1;
				}
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(UOCPara.offset == 3){
					if(((UOCPara.pare_num/data)%10)>=4){
						UOCPara.pare_num = UOCPara.pare_num%data;
					}
				}
				else if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
			
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F04_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.CurTransPierceTimes1;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				if(UOCPara.pare_num > 0){
					g_uoc_param.CurTransPierceTimes1 = UOCPara.pare_num;//原f04
					g_uoc_param.CurTransPierceTimes2 = UOCPara.pare_num;//原f05 合在一起
					UOCPara.last_state = F_MAX;//成功
				}else{//失败
					UOCPara.para_state = 1;
					UOCPara.para_fail = 1;
				}
				
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F05_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.CurTransPierceTimes2;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				g_uoc_param.CurTransPierceTimes2 = UOCPara.pare_num;
				UOCPara.last_state = F_MAX;//成功
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F06_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.WrongPhaseFlag;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				if(UOCPara.pare_num == 0){
					g_uoc_param.WrongPhaseFlag = UOCPara.pare_num;
				}
				else if(UOCPara.pare_num == 1)
				{
					g_uoc_param.WrongPhaseFlag =1;
					g_uoc_param.Phase_num = 480;
				}
				else if(UOCPara.pare_num == 2)
				{
					g_uoc_param.WrongPhaseFlag =2;
					g_uoc_param.Phase_num = 600;
				}
				UOCPara.last_state = F_MAX;//成功
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num++;
				if(UOCPara.pare_num > 2)
					UOCPara.pare_num = 0;
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F07_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.CurEquipFlag;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				g_uoc_param.CurEquipFlag = UOCPara.pare_num;
				UOCPara.last_state = F_MAX;//成功
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num++;
				if(UOCPara.pare_num > 1)
					UOCPara.pare_num = 0;
			}
		break;;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F08_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.FaultTrippingFlag;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				g_uoc_param.FaultTrippingFlag = UOCPara.pare_num;
				UOCPara.last_state = F_MAX;//成功
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num++;
				if(UOCPara.pare_num > 1)
					UOCPara.pare_num = 0;
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F09_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.R04OutputDef;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				if(UOCPara.pare_num < 15){
					g_uoc_param.R04OutputDef = UOCPara.pare_num;
					UOCPara.last_state = F_MAX;//成功
				}else{//失败
					UOCPara.para_state = 1;
					UOCPara.para_fail = 1;
				}
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F10_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.R06OutputDef;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				if(UOCPara.pare_num < 15){
					g_uoc_param.R06OutputDef = UOCPara.pare_num;
					UOCPara.last_state = F_MAX;//成功
				}else{//失败
					UOCPara.para_state = 1;
					UOCPara.para_fail = 1;
				}
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
		break;
		case UOC_KEY_4:
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F11_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.Restart;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				if(UOCPara.pare_num == 1){
					//重启
					flash_write(FLASH_SYSPARAM_ID, (uint8_t *)&g_uoc_param, sizeof(gUOCParameter_TypeDef));
					MF_SysTemRest();
				}
				UOCPara.last_state = F_MAX;//成功
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num++;
				if(UOCPara.pare_num > 1)
					UOCPara.pare_num = 0;
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F12_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.RestoreFactorySettings;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				UOCPara.last_state = F_MAX;//成功
				if(UOCPara.pare_num == 1){
					g_uoc_param =g_uoc_init_param;
				}
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num++;
				if(UOCPara.pare_num > 1)
					UOCPara.pare_num = 0;
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F13_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.VoltageFaultState;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				g_uoc_param.VoltageFaultState = UOCPara.pare_num;
				UOCPara.last_state = F_MAX;//成功
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num++;
				if(UOCPara.pare_num > 1)
					UOCPara.pare_num = 0;
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F14_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.FanStopState;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				g_uoc_param.FanStopState = UOCPara.pare_num;
				UOCPara.last_state = F_MAX;//成功
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num++;
				if(UOCPara.pare_num > 1)
					UOCPara.pare_num = 0;
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F15_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.CurCorParam;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				g_uoc_param.CurCorParam = UOCPara.pare_num;
				UOCPara.last_state = F_MAX;//成功
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F16_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.CurOverShortDelayFlag;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				g_uoc_param.CurOverShortDelayFlag = UOCPara.pare_num;
				UOCPara.last_state = F_MAX;//成功
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num++;
				if(UOCPara.pare_num > 1) 
					UOCPara.pare_num = 0;
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F17_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.CurOverShortDelayTime;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				g_uoc_param.CurOverShortDelayTime = UOCPara.pare_num;
				UOCPara.last_state = F_MAX;//成功
			}	
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F18_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.CurOverLongDelayFlag;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				g_uoc_param.CurOverLongDelayFlag = UOCPara.pare_num;
				UOCPara.last_state = F_MAX;//成功
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num++;
				if(UOCPara.pare_num > 1)
					UOCPara.pare_num = 0;
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F19_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.CurOverLongDelayTime;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				g_uoc_param.CurOverLongDelayTime = UOCPara.pare_num;
				UOCPara.last_state = F_MAX;//成功
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F20_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.MotorShortCircuitFlag;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				g_uoc_param.MotorShortCircuitFlag = UOCPara.pare_num;
				UOCPara.last_state = F_MAX;//成功
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num++;
				if(UOCPara.pare_num > 1)
					UOCPara.pare_num = 0;
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F21_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.ShortCircuitMax;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				if(UOCPara.pare_num >= 5){
					g_uoc_param.ShortCircuitMax = UOCPara.pare_num;
					UOCPara.last_state = F_MAX;//成功
				}
				else{//失败
					UOCPara.para_state = 1;
					UOCPara.para_fail = 1;
				}
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num++;
				if(UOCPara.pare_num > 9)
					UOCPara.pare_num = 5;
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F22_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.ShortCircuitDelay;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				if((UOCPara.pare_num <= 50)&&(3<UOCPara.pare_num)) {
					g_uoc_param.ShortCircuitDelay = UOCPara.pare_num;
					UOCPara.last_state = F_MAX;//成功
				}else{//失败
					UOCPara.para_state = 1;
					UOCPara.para_fail = 1;
				}
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F23_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.DischargeSmokeOutputTime;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				g_uoc_param.DischargeSmokeOutputTime = UOCPara.pare_num;
				UOCPara.last_state = F_MAX;//成功
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F24_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.DischargeSmokeFeedbackTime;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				g_uoc_param.DischargeSmokeFeedbackTime = UOCPara.pare_num;
				UOCPara.last_state = F_MAX;//成功
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F25_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.VolCorParam;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				g_uoc_param.VolCorParam = UOCPara.pare_num;
				UOCPara.last_state = F_MAX;//成功
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F26_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.VolOverFlag;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				g_uoc_param.VolOverFlag = UOCPara.pare_num;
				UOCPara.last_state = F_MAX;//成功
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num++;
				if(UOCPara.pare_num > 1)
					UOCPara.pare_num = 0;
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}

void UOC_paremeter_F27_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.VolOverMax;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				g_uoc_param.VolOverMax = UOCPara.pare_num;
				UOCPara.last_state = F_MAX;//成功
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F28_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.VolOverTime;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				g_uoc_param.VolOverTime = UOCPara.pare_num;
				UOCPara.last_state = F_MAX;//成功
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F29_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.VolUnderFlag;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				g_uoc_param.VolUnderFlag = UOCPara.pare_num;
				UOCPara.last_state = F_MAX;//成功
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num++;
				if(UOCPara.pare_num > 1)
					UOCPara.pare_num = 0;
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F30_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.VolUnderMax;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				g_uoc_param.VolUnderMax = UOCPara.pare_num;
				UOCPara.last_state = F_MAX;//成功
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F31_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.VolUnderTime;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				g_uoc_param.VolUnderTime = UOCPara.pare_num;
				UOCPara.last_state = F_MAX;//成功
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}

void UOC_paremeter_F32_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.LackPhaseFlag;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				g_uoc_param.LackPhaseFlag = UOCPara.pare_num;
				UOCPara.last_state = F_MAX;//成功
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num++;
				if(UOCPara.pare_num > 1)
					UOCPara.pare_num = 0;
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F33_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.WrongPhaseTime;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				g_uoc_param.WrongPhaseTime = UOCPara.pare_num;
				UOCPara.last_state = F_MAX;//成功
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F34_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.LackPhaseTime;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				g_uoc_param.LackPhaseTime = UOCPara.pare_num;
				UOCPara.last_state = F_MAX;//成功
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}

void UOC_paremeter_F35_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.CurEquipTime;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				if((UOCPara.pare_num >= 2) && (UOCPara.pare_num <= 60)){
					g_uoc_param.CurEquipTime = UOCPara.pare_num;
					UOCPara.last_state = F_MAX;//成功
				}else{//失败
					UOCPara.para_state = 1;
					UOCPara.para_fail = 1;
				}
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}

void UOC_paremeter_F36_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.DI9InputDelay;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				if((UOCPara.pare_num >= 3) && (UOCPara.pare_num <= 20)){
					g_uoc_param.DI9InputDelay = UOCPara.pare_num;
					UOCPara.last_state = F_MAX;//成功
				}else{//失败
					UOCPara.para_state = 1;
					UOCPara.para_fail = 1;
				}
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F37_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.DI10InputDelay;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				if((UOCPara.pare_num >= 3) && (UOCPara.pare_num <= 20)){
					g_uoc_param.DI10InputDelay = UOCPara.pare_num;
					UOCPara.last_state = F_MAX;//成功
				}else{//失败
					UOCPara.para_state = 1;
					UOCPara.para_fail = 1;
				}
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F38_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.DI11InputDelay;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				if((UOCPara.pare_num >= 3) && (UOCPara.pare_num <= 20)){
					g_uoc_param.DI11InputDelay = UOCPara.pare_num;
					UOCPara.last_state = F_MAX;//成功
				}else{//失败
					UOCPara.para_state = 1;
					UOCPara.para_fail = 1;
				}
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F39_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.DI12InputDelay;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				if((UOCPara.pare_num >= 3) && (UOCPara.pare_num <= 20)){
					g_uoc_param.DI12InputDelay = UOCPara.pare_num;
					UOCPara.last_state = F_MAX;//成功
				}else{//失败
					UOCPara.para_state = 1;
					UOCPara.para_fail = 1;
				}
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F40_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.DI13InputDelay;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				if((UOCPara.pare_num >= 3) && (UOCPara.pare_num <= 20)){
					g_uoc_param.DI13InputDelay = UOCPara.pare_num;
					UOCPara.last_state = F_MAX;//成功
				}else{//失败
					UOCPara.para_state = 1;
					UOCPara.para_fail = 1;
				}
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F41_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.DI14InputDelay;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				if((UOCPara.pare_num >= 3) && (UOCPara.pare_num <= 20)){
					g_uoc_param.DI14InputDelay = UOCPara.pare_num;
					UOCPara.last_state = F_MAX;//成功
				}else{//失败
					UOCPara.para_state = 1;
					UOCPara.para_fail = 1;
				}
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F42_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.DI7InputDelay;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				if((UOCPara.pare_num >= 3) && (UOCPara.pare_num <= 20)){
					g_uoc_param.DI7InputDelay = UOCPara.pare_num;
					UOCPara.last_state = F_MAX;//成功
				}else{//失败
					UOCPara.para_state = 1;
					UOCPara.para_fail = 1;
				}
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F43_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.DI8InputDelay;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				if((UOCPara.pare_num >= 3) && (UOCPara.pare_num <= 20)){
					g_uoc_param.DI8InputDelay = UOCPara.pare_num;
					UOCPara.last_state = F_MAX;//成功
				}else{//失败
					UOCPara.para_state = 1;
					UOCPara.para_fail = 1;
				}
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F44_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.ButtonInputDelay;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				if((UOCPara.pare_num >= 3) && (UOCPara.pare_num <= 20)){
					g_uoc_param.ButtonInputDelay = UOCPara.pare_num;
					UOCPara.last_state = F_MAX;//成功
				}else{//失败
					UOCPara.para_state = 1;
					UOCPara.para_fail = 1;
				}
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F45_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.R03OutputDef;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				if(UOCPara.pare_num < 15){
					g_uoc_param.R03OutputDef = UOCPara.pare_num;
					UOCPara.last_state = F_MAX;//成功
				}else{//失败
					UOCPara.para_state = 1;
					UOCPara.para_fail = 1;
				}
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}

void UOC_paremeter_F46_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.R05OutputDef;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				if(UOCPara.pare_num < 15){
					g_uoc_param.R05OutputDef = UOCPara.pare_num;
					UOCPara.last_state = F_MAX;//成功
				}else{//失败
					UOCPara.para_state = 1;
					UOCPara.para_fail = 1;
				}
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F47_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			
		break;
		case UOC_KEY_2:
		break;
		case UOC_KEY_3:
		break;
		case UOC_KEY_4:
			
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F48_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			
		break;
		case UOC_KEY_2:
		break;
		case UOC_KEY_3:
		break;
		case UOC_KEY_4:
			
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F49_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			
		break;
		case UOC_KEY_2:
		break;
		case UOC_KEY_3:
		break;
		case UOC_KEY_4:
			
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F50_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.DI7InputSet;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				if(UOCPara.pare_num < 13){
					UOC_Input_State.static_res(g_uoc_param.DI7InputSet);
					g_uoc_param.DI7InputSet = UOCPara.pare_num;
					UOCPara.last_state = F_MAX;//成功
				}else{//失败
					UOCPara.para_state = 1;
					UOCPara.para_fail = 1;
				}
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F51_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.DI8InputSet;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				if(UOCPara.pare_num < 13){
					UOC_Input_State.static_res(g_uoc_param.DI8InputSet);
					g_uoc_param.DI8InputSet = UOCPara.pare_num;
					UOCPara.last_state = F_MAX;//成功
				}else{//失败
					UOCPara.para_state = 1;
					UOCPara.para_fail = 1;
				}
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F52_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.DI9InputSet;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				if(UOCPara.pare_num < 13){
					UOC_Input_State.static_res(g_uoc_param.DI9InputSet);
					g_uoc_param.DI9InputSet = UOCPara.pare_num;
					UOCPara.last_state = F_MAX;//成功
				}else{//失败
					UOCPara.para_state = 1;
					UOCPara.para_fail = 1;
				}
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F53_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.DI10InputSet;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				if(UOCPara.pare_num < 13){
					UOC_Input_State.static_res(g_uoc_param.DI10InputSet);
					g_uoc_param.DI10InputSet = UOCPara.pare_num;
					UOCPara.last_state = F_MAX;//成功
				}else{//失败
					UOCPara.para_state = 1;
					UOCPara.para_fail = 1;
				}
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F54_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.DI11InputSet;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				if(UOCPara.pare_num < 13){
					UOC_Input_State.static_res(g_uoc_param.DI11InputSet);
					g_uoc_param.DI11InputSet = UOCPara.pare_num;
					UOCPara.last_state = F_MAX;//成功
				}else{//失败
					UOCPara.para_state = 1;
					UOCPara.para_fail = 1;
				}
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F55_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.DI12InputSet;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				if(UOCPara.pare_num < 13){
					UOC_Input_State.static_res(g_uoc_param.DI12InputSet);
					g_uoc_param.DI12InputSet = UOCPara.pare_num;
					UOCPara.last_state = F_MAX;//成功
				}else{//失败
					UOCPara.para_state = 1;
					UOCPara.para_fail = 1;
				}
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F56_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.DI13InputSet;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				if(UOCPara.pare_num < 13){
					UOC_Input_State.static_res(g_uoc_param.DI13InputSet);
					g_uoc_param.DI13InputSet = UOCPara.pare_num;
					UOCPara.last_state = F_MAX;//成功
				}else{//失败
					UOCPara.para_state = 1;
					UOCPara.para_fail = 1;
				}
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F57_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			if(UOCPara.para_state == 1)
			{
				UOCPara.pare_num = g_uoc_param.DI14InputSet;
				UOCPara.offset = 0;
			}
			else if(UOCPara.para_state == 0)
			{
				if(UOCPara.pare_num < 13){
					UOC_Input_State.static_res(g_uoc_param.DI14InputSet);
					g_uoc_param.DI14InputSet = UOCPara.pare_num;
					UOCPara.last_state = F_MAX;//成功
				}else{//失败
					UOCPara.para_state = 1;
					UOCPara.para_fail = 1;
				}
			}
		break;
		case UOC_KEY_3:
			if(UOCPara.para_state == 1)
			{
				uint32_t data = 1;
				uint8_t i=0;
				for(i=0;i<UOCPara.offset;i++)
					data = data *10;
				UOCPara.pare_num = UOCPara.pare_num + data;
				if(((UOCPara.pare_num/data)%10)==0)
					UOCPara.pare_num = UOCPara.pare_num - (data*10);
			}
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F58_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
				
		break;
		case UOC_KEY_2:
		break;
		case UOC_KEY_3:
		break;
		case UOC_KEY_4:
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_F59_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
			
		break;
		case UOC_KEY_2:
		break;
		case UOC_KEY_3:
		break;
		case UOC_KEY_4:
			
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void UOC_paremeter_FREE_FUNC(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
				UOCPara.new_state = F_00H;
				UOCPara.Wait_timeout = GetSysTickCount() + Wait_time;
		break;
		case UOC_KEY_2:
		break;
		case UOC_KEY_3:
			if(!UOCPara.spi_read_flag)
				UOCPara.spi_read_flag = 1;
			else
				UOCPara.spi_read_flag = 0;
		break;
		case UOC_KEY_4:
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}
void uoc_ERR_Flag_clean(void)
{
	unsigned char i=0;
	for(i=0;i<EMAX;i++){
		UOC_ERR_PARA[i].flag = 0;
	}
	return;
}
void UOC_paremeter_ERR_FUNC(void)
{
	unsigned char i =0;
	for(i=0;i<EMAX;i++)
	{
		if(UOC_ERR_PARA[i].flag == 1)
		{
			if(i == E06){
				I2C_Display_Err(UOC_ERR_PARA[i].id,UOC_ERR_PARA[i].data);
			}
			else if(i > E03){
				I2C_Display_Err(UOC_ERR_PARA[i].id,0);
			}
			break;
		}
	}
	Output_Control(UOC_D06,CONTORL_LOW);
	if((i == E01) || (i == E00))
	{
		if((UOC_Input_State.Ordinary_fan == 1) || (UOC_Input_State.high_speed_fan == 1)){
			UOC_Trip_Fan();
		}
		if((g_uoc_param.VoltageFaultState == 0)&& (UOC_Input_State.Vol_Normal_flag == 1))
		{
			UOC_Input_State.Vol_Normal_flag = 0;
			key_state = UOC_KEY_4;//电压自恢复
		}
	}	
	if (i == E03 || i == E02){
		//if(i == E03){
			if(UOC_Input_State.Ordinary_fan == 1){
				Control_R03R04R05R06_Function(UOC_LOW_SPEED_OVERCURRENT,OPEN);
			}
			if(UOC_Input_State.high_speed_fan == 1){
				Control_R03R04R05R06_Function(UOC_HIGH_SPEED_OVERCURRENT,OPEN);
			}
		//}
		if((g_uoc_param.FaultTrippingFlag == 0) || (UOC_Input_State.Ordinary_fan == 1))
		{
			UOC_Trip_Fan();
		}
	}
	else if(i == E08){
		if(g_uoc_param.FanStopState)
		{
			UOC_Trip_Fan();
		}
	}
	else
	{
		UOC_Trip_Fan();
	}
	switch (key_state)
	{
		case UOC_KEY_1:
		break;
		case UOC_KEY_2:
		break;
		case UOC_KEY_3:
		break;
		case UOC_KEY_4:
			UOC_Input_State.Auto_trip = 0;
			UOC_Input_State.Vol_Normal_flag = 0;
			if (i == E03 || i == E02){
				if(UOC_Input_State.high_speed_fan ==1)
					break;
			}
			if (i == E08){
				if((UOC_Input_State.high_speed_fan ==1) || (UOC_Input_State.Ordinary_fan ==1)){
					UOCPara.new_state = F_FREE;
					UOC_ERR_PARA[i].flag = 0;
					
					for(i=0;i<EMAX;i++){
						UOC_ERR_PARA[i].fisrt_flag = 0;
						UOC_ERR_PARA[i].time_out = GetSysTickCount();
					}
					led_display(UOC_LED_RUN,LED_ON);//电源灯亮
					Output_Control(UOC_D06,CONTORL_HIGH);//警报灯灭
					return;
					}
			}
			UOCPara.new_state = F_FREE;
			UOC_ERR_PARA[i].flag = 0;
			
			for(i=0;i<EMAX;i++){
				UOC_ERR_PARA[i].fisrt_flag = 0;
				UOC_ERR_PARA[i].time_out = GetSysTickCount();
			}
			led_display(UOC_LED_RUN,LED_ON);//电源灯亮
			Output_Control(UOC_D06,CONTORL_HIGH);//警报灯灭
			UOC_Input_State.static_clean();//清除风机状态
			Control_R03R04R05R06_Function(UOC_HIGH_SPEED_OVERCURRENT,CLOSE);
			Control_R03R04R05R06_Function(UOC_LOW_SPEED_OVERCURRENT,CLOSE);
			Control_R03R04R05R06_Function(UOC_POWER_NORMAL,OPEN);
		break;
		default:
		break;
	}
	if(key_state != UOC_KEY_FREE)
		key_state = UOC_KEY_FREE;
	return;
}




void UOC_FN_Menu(void)
{
	uint8_t dataTmp[FLASH_PAGE_SIZE] = {0};
	switch (key_state)
	{
		case UOC_KEY_1:
				if(Button_deal[UOCPara.new_state].flag == 1){
					UOCPara.para_state = 1;
					UOCPara.displayflag =1;//修复第一次卡顿显示
				}
		break;
		case UOC_KEY_2:
				UOCPara.new_state = (UOCPara.new_state+10)%(F_FREE);
				key_state = UOC_KEY_FREE;
		break;
		case UOC_KEY_3:
				UOCPara.new_state++;
				if(UOCPara.new_state%10 == 0)
					UOCPara.new_state = UOCPara.new_state - 10;
				UOCPara.new_state = (UOCPara.new_state)%(F_FREE);
				key_state = UOC_KEY_FREE;
		break;
		case UOC_KEY_4:
				UOCPara.new_state = F_FREE;
				key_state = UOC_KEY_FREE;
			    if (true == flase_read(FLASH_SYSPARAM_ID, dataTmp, sizeof(gUOCParameter_TypeDef)))
			    {//退出设置FN，当参数不一致时，存入flash
			    	if(memcmp((uint8_t *)&g_uoc_param,dataTmp,sizeof(gUOCParameter_TypeDef)) != 0)
			    	{
						flash_write(FLASH_SYSPARAM_ID, (uint8_t *)&g_uoc_param, sizeof(gUOCParameter_TypeDef));
						Show_Param_info();
			    	}
			    }
		break;
		default:
		break;
	}	
	return;
}
void UOC_FN_Exit_Menu(void)
{
	switch (key_state)
	{
		case UOC_KEY_1:
				UOCPara.para_state = 0;
		break;
		case UOC_KEY_2:
			UOCPara.offset ++;
			if(UOCPara.offset >= Button_deal[UOCPara.new_state].param_seat)
				UOCPara.offset = 0;
			break;
		case UOC_KEY_3:
			break;
		case UOC_KEY_4:
				led_display(UOC_LED_RUN,LED_ON);
				UOCPara.para_state = 0;
				UOCPara.last_state = F_MAX;
				key_state = UOC_KEY_FREE;
		break;
		default:
		break;
	}	
	return;
}
void UOC_Parameter_Offset_Display(unsigned char offset)
{
	switch (offset)
	{
		case 0:
			I2C_Display(DISPLAY4,DISPLAY_NO_POINT,DISPLAY_OFF);
		break;
		case 1:
			I2C_Display(DISPLAY3,DISPLAY_NO_POINT,DISPLAY_OFF);
		break;
		case 2:
			I2C_Display(DISPLAY2,DISPLAY_NO_POINT,DISPLAY_OFF);
		break;
		case 3:
			I2C_Display(DISPLAY1,DISPLAY_NO_POINT,DISPLAY_OFF);
		break;
		default:
		break;
	}
	return;
}

void UOC_Parameter_func(void)
{
	unsigned char i = 0;
	static unsigned err_flag = 0;
	if(UOCPara.new_state < F_FREE)
	{
		if(UOCPara.Wait_timeout < GetSysTickCount())
		{
			uint8_t dataTmp[FLASH_PAGE_SIZE] = {0};
			dbg_printf("wait_timeout start %d hehe %d\n",UOCPara.Wait_timeout,GetSysTickCount());
			//UOCPara.Wait_timeout = GetSysTickCount() + 5000;
			UOCPara.para_state = 0;
			UOCPara.new_state = F_FREE;
			key_state = UOC_KEY_FREE;
		    if (true == flase_read(FLASH_SYSPARAM_ID, dataTmp, sizeof(gUOCParameter_TypeDef)))
		    {//退出设置FN，当参数不一致时，存入flash
		    	if(memcmp((uint8_t *)&g_uoc_param,dataTmp,sizeof(gUOCParameter_TypeDef)) != 0)
		    	{
					flash_write(FLASH_SYSPARAM_ID, (uint8_t *)&g_uoc_param, sizeof(gUOCParameter_TypeDef));
					Show_Param_info();
		    	}
		    }
		}
	}
	for(i=0;i<F_MAX;i++)
	{
		if(UOCPara.new_state == Button_deal[i].name){
			Button_deal[i].cb();
			break;
			}
	}
	if(UOCPara.new_state >= F_FREE)
	{
		if(UOCPara.new_state == F_ERR)
		{
			for(i=0;i<EMAX;i++)
			{
				if(UOC_ERR_PARA[i].flag == 1)
				{
					break;
				}
			}
			if(UOCPara.Display_timeout < GetSysTickCount())
			{
				if(UOCPara.ledflag)//之前需要led闪烁 现在不需要了
				{
					UOCPara.ledflag = 0;
					led_display(UOC_LED_RUN,LED_ON);
					led_display(UOC_LED_NV,LED_OFF);
					led_display(UOC_LED_NA,LED_OFF);
					if(i < E02){
						led_display(UOC_LED_NV,LED_ON);
						switch (err_flag)
						{
							case 0:
								err_flag = 2;
								debug_log("V error i:%d\n",i);
								I2C_Display_Err(UOC_ERR_PARA[i].id,0);
							break;
							case 1:
								err_flag++;
								I2C_Display(DISPLAY1,DISPLAY_NO_POINT,DISPLAY_A_PHASE);
								I2C_Display(DISPLAY2,DISPLAY_NO_POINT,(UOC_Check_Para.A_Vol%1000)/100);
								I2C_Display(DISPLAY3,DISPLAY_NO_POINT,(UOC_Check_Para.A_Vol%100)/10);
								I2C_Display(DISPLAY4,DISPLAY_NO_POINT,UOC_Check_Para.A_Vol%10);
							break;
							case 2:
								err_flag = 0;
								I2C_Display(DISPLAY1,DISPLAY_NO_POINT,DISPLAY_B_PHASE);
								I2C_Display(DISPLAY2,DISPLAY_NO_POINT,(UOC_Check_Para.Vol_err%1000)/100);
								I2C_Display(DISPLAY3,DISPLAY_NO_POINT,(UOC_Check_Para.Vol_err%100)/10);
								I2C_Display(DISPLAY4,DISPLAY_NO_POINT,UOC_Check_Para.Vol_err%10);
							break;
							case 3:
								err_flag = 0;
								I2C_Display(DISPLAY1,DISPLAY_NO_POINT,DISPLAY_C_PHASE);
								I2C_Display(DISPLAY2,DISPLAY_NO_POINT,(UOC_Check_Para.C_Vol%1000)/100);
								I2C_Display(DISPLAY3,DISPLAY_NO_POINT,(UOC_Check_Para.C_Vol%100)/10);
								I2C_Display(DISPLAY4,DISPLAY_NO_POINT,UOC_Check_Para.C_Vol%10);
							break;
						}
					}
					else if(i < E04){
						led_display(UOC_LED_NA,LED_ON);
						switch (err_flag)
						{
							case 0:
								err_flag = 3;
								debug_log("C error i:%d\n",i);
								I2C_Display_Err(UOC_ERR_PARA[i].id,0);
							break;
							case 1:
								err_flag++;
								I2C_Display(DISPLAY1,DISPLAY_NO_POINT,DISPLAY_A_PHASE);
								I2C_Display(DISPLAY2,DISPLAY_NO_POINT,(((int)(UOC_Check_Para.A_Cur*10))%1000)/100);
								I2C_Display(DISPLAY3,DISPLAY_POINT,(((int)(UOC_Check_Para.A_Cur*10))%100)/10);
								I2C_Display(DISPLAY4,DISPLAY_NO_POINT,((int)(UOC_Check_Para.A_Cur*10))%10);
							break;
							case 2:
								err_flag++;
								I2C_Display(DISPLAY1,DISPLAY_NO_POINT,DISPLAY_B_PHASE);
								I2C_Display(DISPLAY2,DISPLAY_NO_POINT,(((int)(UOC_Check_Para.B_Cur*10))%1000)/100);
								I2C_Display(DISPLAY3,DISPLAY_POINT,(((int)(UOC_Check_Para.B_Cur*10))%100)/10);
								I2C_Display(DISPLAY4,DISPLAY_NO_POINT,((int)(UOC_Check_Para.B_Cur*10))%10);
							break;
							case 3:
								err_flag = 0;
								I2C_Display(DISPLAY1,DISPLAY_NO_POINT,DISPLAY_C_PHASE);
								I2C_Display(DISPLAY2,DISPLAY_NO_POINT,(((int)(UOC_Check_Para.Cur_err*10))%1000)/100);
								I2C_Display(DISPLAY3,DISPLAY_POINT,(((int)(UOC_Check_Para.Cur_err*10))%100)/10);
								I2C_Display(DISPLAY4,DISPLAY_NO_POINT,((int)(UOC_Check_Para.Cur_err*10))%10);
							break;
						}
					}
				}
				else
				{
					UOCPara.ledflag = 1;
					led_display(UOC_LED_RUN,LED_OFF);
				}
				UOCPara.Display_timeout = GetSysTickCount() + 500;
			}
		}
		return;
	}
	if(UOCPara.Display_timeout < GetSysTickCount())
	{
		if(UOCPara.para_state == 1)
		{
			if(UOCPara.displayflag)
			{
				UOCPara.displayflag = 0;
				switch (Button_deal[i].param_seat)
					{
					case 1:
						UOC_paremeter_Display(DISPLAY_OFF,DISPLAY_OFF,DISPLAY_OFF,UOCPara.pare_num%10);
						break;
					case 2:
						UOC_paremeter_Display(DISPLAY_OFF,DISPLAY_OFF,(UOCPara.pare_num%100)/10,UOCPara.pare_num%10);
						break;
					case 3:
						UOC_paremeter_Display(DISPLAY_OFF,(UOCPara.pare_num%1000)/100,(UOCPara.pare_num%100)/10,UOCPara.pare_num%10);
						break;
					case 4:
						UOC_paremeter_Display(UOCPara.pare_num/1000,(UOCPara.pare_num%1000)/100,(UOCPara.pare_num%100)/10,UOCPara.pare_num%10);
						break;
					}
				
			}
			else
			{
				UOCPara.displayflag = 1;
				if(UOCPara.para_fail){
					UOC_paremeter_Display(DISPLAY_OFF,DISPLAY_OFF,DISPLAY_OFF,DISPLAY_OFF);
				}
				else{
					UOC_Parameter_Offset_Display(UOCPara.offset);
				}
			}
		}
		if(UOCPara.para_fail){
			if(UOCPara.para_fail_count >= 2){
				UOCPara.para_fail_count = 0;
				UOCPara.para_fail =0;
			}
			else{
				UOCPara.para_fail_count++;
			}
			UOCPara.Display_timeout = GetSysTickCount() + 240;
		}
		else{
			UOCPara.Display_timeout = GetSysTickCount() + 500;
		}
	}
	return;
}
void UOC_Parameter_State(void)
{
	if(UOCPara.new_state != UOCPara.last_state)
	{
		UOCPara.last_state = UOCPara.new_state;
		if(UOCPara.new_state < F_FREE)
			UOC_paremeter_Display(DISPLAY_OFF,DISPLAY_FN,UOCPara.new_state/10,UOCPara.new_state%10);
	}
	if(UOCPara.new_state < F_FREE)
	{
		if(key_state != UOC_KEY_FREE)
		{
			UOCPara.Wait_timeout = GetSysTickCount() + Wait_time;
		}
		if(UOCPara.para_state == 0)
		{
			if(UOCPara.para_fail == 0){
				UOC_FN_Menu();
				}
		}
		else if(UOCPara.para_state == 1)
		{
			if(UOCPara.para_fail == 0){
				UOC_FN_Exit_Menu();
				}
		}
	}
	UOC_Parameter_func();
	return;
}

void UOC_FanType_control(void)
{
	Button_deal[F_02H].flag = g_uoc_param.FanType-1;
	//Button_deal[F_05H].flag = g_uoc_param.FanType-1;
	return;
}
void System_Funtion(void)
{
	UOC_FanType_control();
	Key_press();
	Key_release();
	Key_timemout_deal();
	UOC_Parameter_State();
}

