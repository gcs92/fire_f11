#include "production_test.h"
#include"timer.h"
#include "debug.h"
#include"led.h"
#include"spi.h"
#include "system_control.h"
#include "mf_config.h"
#include "flash_api.h"

#define push_time 500
#define test_key_time 10

//extern unsigned char key_state;
//extern sSPIData_TypeDef uoc_cyc_para[UOC_SPI_MAX_PARA];
sKeyFlag_TypeDef uoc_key;
extern sDInputEvent_TypeDef uoc_DI_Funtion[UOC_DIMAX];
static uint32_t g_Test_time_count = 0;
// static unsigned char g_display_flag=0;
// static unsigned char g_model_flag=0;


sPro_Test_DI_TypeDef pro_test_DI_func[UOC_DIMAX] = {
	{UOC_DI0,0,TEST_UOC_DI0_FUNC},
	{UOC_DI1,0,TEST_UOC_DI1_FUNC},
	{UOC_DI2,0,TEST_UOC_DI2_FUNC},
	{UOC_DI3,0,TEST_UOC_DI3_FUNC},
	{UOC_DI4,0,NULL},
};
void TEST_UOC_DI0_FUNC(void)
{
	if(DI0() == STATE_LOW)
	{
		uoc_DI_Funtion[UOC_DI0].timeflag = 1;
		if(uoc_DI_Funtion[UOC_DI0].timeCount > test_key_time)
		{
			uoc_DI_Funtion[UOC_DI0].timeflag = 0;
			uoc_DI_Funtion[UOC_DI0].timeCount = 0;
			if(DI0() == STATE_LOW)
			{
				Output_Control(UOC_D08,CONTORL_HIGH);
			}
		}
	}
	else if(DI0() == STATE_HIGH)
	{
		pro_test_DI_func[UOC_DI0].flag =0;
		uoc_DI_Funtion[UOC_DI0].timeflag = 0;
		uoc_DI_Funtion[UOC_DI0].timeCount = 0;
		Output_Control(UOC_D08,CONTORL_LOW);
	}
}

void TEST_UOC_DI1_FUNC(void)
{
	if(DI1() == STATE_LOW)
	{
		uoc_DI_Funtion[UOC_DI1].timeflag = 1;
		if(uoc_DI_Funtion[UOC_DI1].timeCount > test_key_time)
		{
			uoc_DI_Funtion[UOC_DI1].timeflag = 0;
			uoc_DI_Funtion[UOC_DI1].timeCount = 0;
			if(DI1() == STATE_LOW)
			{
				Output_Control(UOC_D07,CONTORL_HIGH);
			}
		}
	}
	else if(DI1() == STATE_HIGH)
	{
		pro_test_DI_func[UOC_DI1].flag =0;	
		uoc_DI_Funtion[UOC_DI1].timeflag = 0;
		uoc_DI_Funtion[UOC_DI1].timeCount = 0;
		Output_Control(UOC_D07,CONTORL_LOW);
	}
}

void TEST_UOC_DI2_FUNC(void)
{
	if(DI2() == STATE_LOW)
	{
		uoc_DI_Funtion[UOC_DI2].timeflag = 1;
		if(uoc_DI_Funtion[UOC_DI2].timeCount > test_key_time)
		{
			uoc_DI_Funtion[UOC_DI2].timeflag = 0;
			uoc_DI_Funtion[UOC_DI2].timeCount = 0;
			if(DI2() == STATE_LOW)
			{
				Output_Control(UOC_D02,CONTORL_HIGH);
			}
		}
	}
	else if(DI2() == STATE_HIGH)
	{
		pro_test_DI_func[UOC_DI2].flag =0;
		uoc_DI_Funtion[UOC_DI2].timeflag = 0;
		uoc_DI_Funtion[UOC_DI2].timeCount = 0;
		Output_Control(UOC_D02,CONTORL_LOW);
	}
}

void TEST_UOC_DI3_FUNC(void)
{
	if(DI3() == STATE_LOW)
	{
		uoc_DI_Funtion[UOC_DI3].timeflag = 1;
		if(uoc_DI_Funtion[UOC_DI3].timeCount > test_key_time)
		{
			uoc_DI_Funtion[UOC_DI3].timeflag = 0;
			uoc_DI_Funtion[UOC_DI3].timeCount = 0;
			if(DI3() == STATE_LOW)
			{
				Output_Control(UOC_D03,CONTORL_HIGH);
			}
		}
	}
	else if(DI3() == STATE_HIGH)
	{
		pro_test_DI_func[UOC_DI3].flag =0;
		uoc_DI_Funtion[UOC_DI3].timeflag = 0;
		uoc_DI_Funtion[UOC_DI3].timeCount = 0;
		Output_Control(UOC_D03,CONTORL_LOW);
	}
}

unsigned char Join_Production_Test(void)
{	
	//通过检测io口进入生产自检模式
	if(FL_GPIO_GetInputPin(GPIOA, FL_GPIO_PIN_10) == 0)
	{
		uoc_key.timeflag = 1;
		if(uoc_key.timeout > push_time)
		{
			uoc_key.timeflag = 0;
			uoc_key.timeout = 0;
			if(FL_GPIO_GetInputPin(GPIOA, FL_GPIO_PIN_10) == 0)
			{
				return 1;
			}
		}
	}
	else
	{
		uoc_key.timeflag = 0;
		uoc_key.timeout = 0;
	}
	return 0;
}

void production_test_model(void)
{
	unsigned char i;
	static unsigned char join_flag = 0;
	if(DI4() == STATE_LOW)
	{
		uoc_DI_Funtion[UOC_DI4].timeflag = 1;
		if(uoc_DI_Funtion[UOC_DI4].timeCount > test_key_time)
		{
			uoc_DI_Funtion[UOC_DI4].timeflag = 0;
			uoc_DI_Funtion[UOC_DI4].timeCount = 0;
			if(DI4() == STATE_LOW)
			{	
				//关灯
				if(join_flag  == 0)
				{
					join_flag =1;
					Output_Control(UOC_D02,CONTORL_LOW);
					Output_Control(UOC_D03,CONTORL_LOW);
					Output_Control(UOC_D04,CONTORL_LOW);
					Output_Control(UOC_D05,CONTORL_LOW);
					Output_Control(UOC_D06,CONTORL_LOW);
					Output_Control(UOC_D07,CONTORL_LOW);
					Output_Control(UOC_D08,CONTORL_LOW);
					Output_Control(UOC_BUZZER,0);
				}
			}
		}
		for(i=0;i<UOC_DI4;i++)
			pro_test_DI_func[i].cb();
	}
	else if(DI4() == STATE_HIGH)
	{
		pro_test_DI_func[UOC_DI4].flag =0;
		uoc_DI_Funtion[UOC_DI4].timeflag = 0;
		uoc_DI_Funtion[UOC_DI4].timeCount = 0;
		join_flag = 0;
			
		Output_Control(UOC_D02,CONTORL_HIGH);
		Output_Control(UOC_D03,CONTORL_HIGH);
		Output_Control(UOC_D04,CONTORL_HIGH);
		Output_Control(UOC_D05,CONTORL_HIGH);
		Output_Control(UOC_D06,CONTORL_HIGH);
		Output_Control(UOC_D07,CONTORL_HIGH);
		Output_Control(UOC_D08,CONTORL_HIGH);
		if(GetSysTickCount() < g_Test_time_count+1000){
			Output_Control(UOC_BUZZER,0);
		}
		else{
			Output_Control(UOC_BUZZER,1);
			if(GetSysTickCount() > g_Test_time_count+2000)
				g_Test_time_count = GetSysTickCount();
		} 
		
	}
	return;
}

void production_test_main(void)
{
	unsigned char ret=0;
	ret = Join_Production_Test();
	if(ret == 1)
	{
		dbg_printf("join test module !!!!!!!\n");
		while (1)
		{
			production_test_model();
		}
	}
}
