#include "system_control.h"
#include "timer.h"
#include"debug.h"
#include "parameter.h"
#include "I2C.h"
#include"button_function.h"

extern gUOCParameter_TypeDef g_uoc_param;
extern eUOCDisplayErr_TypeDef UOC_ERR_PARA[EMAX];
extern sParaFlag_TypeDef UOCPara;
extern gUOCParameter_Check_TypeDef UOC_Check_Para;

gUOCControl_TypeDef uoc_control[UOC_CONTROL_MAX] =
{
	{UOC_D02,	GPIOC,	FL_GPIO_PIN_8},
	{UOC_D03,	GPIOC, 	FL_GPIO_PIN_9},
	{UOC_D04,	GPIOC, 	FL_GPIO_PIN_10},
	{UOC_D05,	GPIOB, 	FL_GPIO_PIN_8},
	{UOC_D06,	GPIOB, 	FL_GPIO_PIN_9},
	{UOC_D07,	GPIOB, 	FL_GPIO_PIN_10},
	{UOC_D08,	GPIOB, 	FL_GPIO_PIN_11}
};

sDInputEvent_TypeDef uoc_DI_Funtion[UOC_DIMAX] =
{
	{UOC_DI0,0,0, UOC_DI0_FUNC},
	{UOC_DI1,0,0, UOC_DI1_FUNC},
	{UOC_DI2,0,0, UOC_DI2_FUNC},
	{UOC_DI3,0,0, UOC_DI3_FUNC},
	{UOC_DI4,0,0, UOC_DI4_FUNC},
};s


void Output_Control(unsigned char name,unsigned char flag)
{
	if(flag == CONTORL_LOW)
	{
		FL_GPIO_ResetOutputPin(uoc_control[name].ControlGpio,uoc_control[name].ControlPin);
	}
	else if(flag == CONTORL_HIGH)
	{
		FL_GPIO_SetOutputPin(uoc_control[name].ControlGpio,uoc_control[name].ControlPin);
	}
}

void UOC_DI0_FUNC(void)
{
	if(DI0() == STATE_HIGH)
	{
		uoc_DI_Funtion[UOC_DI0].timeflag = 1;
		if(uoc_DI_Funtion[UOC_DI0].timeCount > (keep_time +(g_uoc_param.ButtonInputDelay*100)))
		{
			uoc_DI_Funtion[UOC_DI0].timeflag = 0;
			uoc_DI_Funtion[UOC_DI0].timeCount = 0;
			if(DI0() == STATE_HIGH)
			{
				debug_log("%s:%d: UOC_DI0:0\n",__func__,__LINE__);
			}
		}
	}
	else if(DI0() == STATE_LOW)
	{
		uoc_DI_Funtion[UOC_DI0].timeflag = 0;
		uoc_DI_Funtion[UOC_DI0].timeCount = 0;
	}
	return;
}
void UOC_DI1_FUNC(void)
{
	if(DI1() == STATE_HIGH)
	{
		uoc_DI_Funtion[UOC_DI1].timeflag = 1;
		if(uoc_DI_Funtion[UOC_DI1].timeCount > (keep_time +(g_uoc_param.ButtonInputDelay*100)))
		{
			uoc_DI_Funtion[UOC_DI1].timeflag = 0;
			uoc_DI_Funtion[UOC_DI1].timeCount = 0;
			if(DI1() == STATE_HIGH)
			{

				debug_log("%s:%d: UOC_DI1:0\n",__func__,__LINE__);
			}
		}
	}
	else if(DI1() == STATE_LOW)
	{
		uoc_DI_Funtion[UOC_DI1].timeflag = 0;
		uoc_DI_Funtion[UOC_DI1].timeCount = 0;
	}
	return;
}
void UOC_DI2_FUNC(void)
{
	if(DI2() == STATE_LOW)
	{
		uoc_DI_Funtion[UOC_DI2].timeflag = 1;
		if(uoc_DI_Funtion[UOC_DI2].timeCount > (keep_time +(g_uoc_param.ButtonInputDelay*100)))
		{
			uoc_DI_Funtion[UOC_DI2].timeflag = 0;
			uoc_DI_Funtion[UOC_DI2].timeCount = 0;
			if(DI2() == STATE_LOW)
			{
				debug_log("%s:%d: UOC_DI2:0\n",__func__,__LINE__);
			}
		}
	}
	else if(DI2() == STATE_HIGH)
	{
		uoc_DI_Funtion[UOC_DI2].timeflag = 0;
		uoc_DI_Funtion[UOC_DI2].timeCount = 0;
	}
	return;
}
void UOC_DI3_FUNC(void)
{


	if(DI3() == STATE_LOW)
	{
		uoc_DI_Funtion[UOC_DI3].timeflag = 1;
		if(uoc_DI_Funtion[UOC_DI3].timeCount > (keep_time +(g_uoc_param.ButtonInputDelay*100)))
		{
			uoc_DI_Funtion[UOC_DI3].timeflag = 0;
			uoc_DI_Funtion[UOC_DI3].timeCount = 0;
			if(DI3() == STATE_LOW)
			{
				debug_log("%s:%d: UOC_DI3:0\n",__func__,__LINE__);
			}
		}
	}
	else if(DI3() == STATE_HIGH)
	{
		uoc_DI_Funtion[UOC_DI3].timeflag = 0;
		uoc_DI_Funtion[UOC_DI3].timeCount = 0;
	}
	return;
}
void UOC_DI4_FUNC(void)
{
	static unsigned mode_flag = 0; 
	if(DI4() == STATE_HIGH && mode_flag == 1)//开启手动
	{
		debug_log("%s:%d: DI4:1 %d\n",__func__,__LINE__,uoc_DI_Funtion[UOC_DI4].timeCount);
		uoc_DI_Funtion[UOC_DI4].timeflag = 1;
		if(uoc_DI_Funtion[UOC_DI4].timeCount > keep_time)
		{
			uoc_DI_Funtion[UOC_DI4].timeflag = 0;
			uoc_DI_Funtion[UOC_DI4].timeCount = 0;
			if(DI4() == STATE_HIGH)
			{
				mode_flag = 0;
				debug_log("%s:%d: UOC_DI0:0\n",__func__,__LINE__);

			}
		}
	}
	else if(DI4() == STATE_LOW && mode_flag == 0)//开启自动
	{
		debug_log("%s:%d: DI4:0 %d\n",__func__,__LINE__,uoc_DI_Funtion[UOC_DI4].timeCount);
		uoc_DI_Funtion[UOC_DI4].timeflag = 1;
		if(uoc_DI_Funtion[UOC_DI4].timeCount > keep_time)
		{
			uoc_DI_Funtion[UOC_DI4].timeflag = 0;
			uoc_DI_Funtion[UOC_DI4].timeCount = 0;
			if(DI4() == STATE_LOW)
			{
				mode_flag = 1;
				debug_log("%s:%d: UOC_DI0:1\n",__func__,__LINE__);
			}
		}
	}
}



void Input_Detection(void)
{
	unsigned char i = 0;
	for(i=0;i<UOC_DIMAX;i++)
	{
		uoc_DI_Funtion[i].cb();
	}	

	return;
}
