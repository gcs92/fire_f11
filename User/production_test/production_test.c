#include "production_test.h"
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

#define push_time 500
#define test_key_time 10

extern sKeyFlag_TypeDef uoc_key[key_max];
extern unsigned char key_state;
extern sSPIData_TypeDef uoc_cyc_para[UOC_SPI_MAX_PARA];
extern sDInputEvent_TypeDef uoc_DI_Funtion[UOC_DIMAX];
static uint32_t g_Test_time_count = 0;
static unsigned char g_display_flag=0;
static unsigned char g_model_flag=0;

sPro_Test_TypeDef pro_test_func[UOC_KEY_MAX] ={
	{UOC_KEY_FREE,production_test_model},
	{UOC_KEY_1,production_test_model1},
	{UOC_KEY_2,production_test_model2},
	{UOC_KEY_3,production_test_model3},
	{UOC_KEY_4,production_test_model4}
};
sPro_Test_DI_TypeDef pro_test_DI_func[UOC_DIMAX] = {
	{UOC_DI0,0,TEST_UOC_DI0_FUNC},
	{UOC_DI1,0,TEST_UOC_DI1_FUNC},
	{UOC_DI2,0,TEST_UOC_DI2_FUNC},
	{UOC_DI3,0,TEST_UOC_DI3_FUNC},
	{UOC_DI4,0,TEST_UOC_DI4_FUNC},
	{UOC_DI5,0,TEST_UOC_DI5_FUNC},
	{UOC_DI6,0,TEST_UOC_DI6_FUNC},
	{UOC_DI7,0,TEST_UOC_DI7_FUNC},
	{UOC_DI8,0,TEST_UOC_DI8_FUNC},
	{UOC_DI9,0,TEST_UOC_DI9_FUNC},
	{UOC_DI10,0,TEST_UOC_DI10_FUNC},
	{UOC_DI11,0,TEST_UOC_DI11_FUNC},
	{UOC_DI12,0,TEST_UOC_DI12_FUNC},
	{UOC_DI13,0,TEST_UOC_DI13_FUNC},
	{UOC_DI14,0,TEST_UOC_DI14_FUNC}
};
void TEST_UOC_DI0_FUNC(void)
{
	if(DI0() == STATE_HIGH)
	{
		uoc_DI_Funtion[UOC_DI0].timeflag = 1;
		if(uoc_DI_Funtion[UOC_DI0].timeCount > test_key_time)
		{
			uoc_DI_Funtion[UOC_DI0].timeflag = 0;
			uoc_DI_Funtion[UOC_DI0].timeCount = 0;
			if(DI0() == STATE_HIGH)
			{
				pro_test_DI_func[UOC_DI0].flag =1;
			}
		}
	}
	else if(DI0() == STATE_LOW)
	{
		pro_test_DI_func[UOC_DI0].flag =0;
		uoc_DI_Funtion[UOC_DI0].timeflag = 0;
		uoc_DI_Funtion[UOC_DI0].timeCount = 0;
	}
}

void TEST_UOC_DI1_FUNC(void)
{
	if(DI1() == STATE_HIGH)
	{
		uoc_DI_Funtion[UOC_DI1].timeflag = 1;
		if(uoc_DI_Funtion[UOC_DI1].timeCount > test_key_time)
		{
			uoc_DI_Funtion[UOC_DI1].timeflag = 0;
			uoc_DI_Funtion[UOC_DI1].timeCount = 0;
			if(DI1() == STATE_HIGH)
			{
				pro_test_DI_func[UOC_DI1].flag =1;
			}
		}
	}
	else if(DI1() == STATE_LOW)
	{
		pro_test_DI_func[UOC_DI1].flag =0;	
		uoc_DI_Funtion[UOC_DI1].timeflag = 0;
		uoc_DI_Funtion[UOC_DI1].timeCount = 0;
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
				pro_test_DI_func[UOC_DI2].flag =1;
			}
		}
	}
	else if(DI2() == STATE_HIGH)
	{
		pro_test_DI_func[UOC_DI2].flag =0;
		uoc_DI_Funtion[UOC_DI2].timeflag = 0;
		uoc_DI_Funtion[UOC_DI2].timeCount = 0;
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
				pro_test_DI_func[UOC_DI3].flag =1;
			}
		}
	}
	else if(DI3() == STATE_HIGH)
	{
		pro_test_DI_func[UOC_DI3].flag =0;
		uoc_DI_Funtion[UOC_DI3].timeflag = 0;
		uoc_DI_Funtion[UOC_DI3].timeCount = 0;
	}
}

void TEST_UOC_DI4_FUNC(void)
{
	if(DI4() == STATE_LOW)
	{
		uoc_DI_Funtion[UOC_DI4].timeflag = 1;
		if(uoc_DI_Funtion[UOC_DI4].timeCount > test_key_time)
		{
			uoc_DI_Funtion[UOC_DI4].timeflag = 0;
			uoc_DI_Funtion[UOC_DI4].timeCount = 0;
			if(DI4() == STATE_LOW)
			{
				pro_test_DI_func[UOC_DI4].flag =1;
			}
		}
	}
	else if(DI4() == STATE_HIGH)
	{
		pro_test_DI_func[UOC_DI4].flag =0;
		uoc_DI_Funtion[UOC_DI4].timeflag = 0;
		uoc_DI_Funtion[UOC_DI4].timeCount = 0;
	}
}

void TEST_UOC_DI5_FUNC(void)
{
	if(DI5() == STATE_LOW)
	{
		uoc_DI_Funtion[UOC_DI5].timeflag = 1;
		if(uoc_DI_Funtion[UOC_DI5].timeCount > test_key_time)
		{
			uoc_DI_Funtion[UOC_DI5].timeflag = 0;
			uoc_DI_Funtion[UOC_DI5].timeCount = 0;
			if(DI5() == STATE_LOW)
			{
				pro_test_DI_func[UOC_DI5].flag =1;
			}
		}
	}
	else if(DI5() == STATE_HIGH)
	{
		pro_test_DI_func[UOC_DI5].flag =0;
		uoc_DI_Funtion[UOC_DI5].timeflag = 0;
		uoc_DI_Funtion[UOC_DI5].timeCount = 0;
	}
}

void TEST_UOC_DI6_FUNC(void)
{
	if(DI6() == STATE_LOW)
	{
		uoc_DI_Funtion[UOC_DI6].timeflag = 1;
		if(uoc_DI_Funtion[UOC_DI6].timeCount > test_key_time)
		{
			uoc_DI_Funtion[UOC_DI6].timeflag = 0;
			uoc_DI_Funtion[UOC_DI6].timeCount = 0;
			if(DI6() == STATE_LOW)
			{
				pro_test_DI_func[UOC_DI6].flag =1;
			}
		}
	}
	else if(DI6() == STATE_HIGH)
	{
		pro_test_DI_func[UOC_DI6].flag =0;
		uoc_DI_Funtion[UOC_DI6].timeflag = 0;
		uoc_DI_Funtion[UOC_DI6].timeCount = 0;
	}
}

void TEST_UOC_DI7_FUNC(void)
{
	if(DI7() == STATE_LOW)
	{
		uoc_DI_Funtion[UOC_DI7].timeflag = 1;
		if(uoc_DI_Funtion[UOC_DI7].timeCount > test_key_time)
		{
			uoc_DI_Funtion[UOC_DI7].timeflag = 0;
			uoc_DI_Funtion[UOC_DI7].timeCount = 0;
			if(DI7() == STATE_LOW)
			{
				pro_test_DI_func[UOC_DI7].flag =1;
			}
		}
	}
	else if(DI7() == STATE_HIGH)
	{
		pro_test_DI_func[UOC_DI7].flag =0;
		uoc_DI_Funtion[UOC_DI7].timeflag = 0;
		uoc_DI_Funtion[UOC_DI7].timeCount = 0;
	}
}

void TEST_UOC_DI8_FUNC(void)
{
	if(DI8() == STATE_LOW)
	{
		uoc_DI_Funtion[UOC_DI8].timeflag = 1;
		if(uoc_DI_Funtion[UOC_DI8].timeCount > test_key_time)
		{
			uoc_DI_Funtion[UOC_DI8].timeflag = 0;
			uoc_DI_Funtion[UOC_DI8].timeCount = 0;
			if(DI8() == STATE_LOW)
			{
				pro_test_DI_func[UOC_DI8].flag =1;
			}
		}
	}
	else if(DI8() == STATE_HIGH)
	{
		pro_test_DI_func[UOC_DI8].flag =0;
		uoc_DI_Funtion[UOC_DI8].timeflag = 0;
		uoc_DI_Funtion[UOC_DI8].timeCount = 0;
	}
}

void TEST_UOC_DI9_FUNC(void)
{
	if(DI9() == STATE_LOW)
	{
		uoc_DI_Funtion[UOC_DI9].timeflag = 1;
		if(uoc_DI_Funtion[UOC_DI9].timeCount > test_key_time)
		{
			uoc_DI_Funtion[UOC_DI9].timeflag = 0;
			uoc_DI_Funtion[UOC_DI9].timeCount = 0;
			if(DI9() == STATE_LOW)
			{
				pro_test_DI_func[UOC_DI9].flag =1;
			}
		}
	}
	else if(DI9() == STATE_HIGH)
	{
		pro_test_DI_func[UOC_DI9].flag =0;
		uoc_DI_Funtion[UOC_DI9].timeflag = 0;
		uoc_DI_Funtion[UOC_DI9].timeCount = 0;
	}
}

void TEST_UOC_DI10_FUNC(void)
{
	if(DI10() == STATE_LOW)
	{
		uoc_DI_Funtion[UOC_DI10].timeflag = 1;
		if(uoc_DI_Funtion[UOC_DI10].timeCount > test_key_time)
		{
			uoc_DI_Funtion[UOC_DI10].timeflag = 0;
			uoc_DI_Funtion[UOC_DI10].timeCount = 0;
			if(DI10() == STATE_LOW)
			{
				pro_test_DI_func[UOC_DI10].flag =1;
			}
		}
	}
	else if(DI10() == STATE_HIGH)
	{
		pro_test_DI_func[UOC_DI10].flag =0;
		uoc_DI_Funtion[UOC_DI10].timeflag = 0;
		uoc_DI_Funtion[UOC_DI10].timeCount = 0;
	}
}

void TEST_UOC_DI11_FUNC(void)
{
	if(DI11() == STATE_LOW)
	{
		uoc_DI_Funtion[UOC_DI11].timeflag = 1;
		if(uoc_DI_Funtion[UOC_DI11].timeCount > test_key_time)
		{
			uoc_DI_Funtion[UOC_DI11].timeflag = 0;
			uoc_DI_Funtion[UOC_DI11].timeCount = 0;
			if(DI11() == STATE_LOW)
			{
				pro_test_DI_func[UOC_DI11].flag =1;
			}
		}
	}
	else if(DI11() == STATE_HIGH)
	{
		pro_test_DI_func[UOC_DI11].flag =0;
		uoc_DI_Funtion[UOC_DI11].timeflag = 0;
		uoc_DI_Funtion[UOC_DI11].timeCount = 0;
	}
}

void TEST_UOC_DI12_FUNC(void)
{
	if(DI12() == STATE_LOW)
	{
		uoc_DI_Funtion[UOC_DI12].timeflag = 1;
		if(uoc_DI_Funtion[UOC_DI12].timeCount > test_key_time)
		{
			uoc_DI_Funtion[UOC_DI12].timeflag = 0;
			uoc_DI_Funtion[UOC_DI12].timeCount = 0;
			if(DI12() == STATE_LOW)
			{
				pro_test_DI_func[UOC_DI12].flag =1;
			}
		}
	}
	else if(DI12() == STATE_HIGH)
	{
		pro_test_DI_func[UOC_DI12].flag =0;
		uoc_DI_Funtion[UOC_DI12].timeflag = 0;
		uoc_DI_Funtion[UOC_DI12].timeCount = 0;
	}
}

void TEST_UOC_DI13_FUNC(void)
{
	if(DI13() == STATE_LOW)
	{
		uoc_DI_Funtion[UOC_DI13].timeflag = 1;
		if(uoc_DI_Funtion[UOC_DI13].timeCount > test_key_time)
		{
			uoc_DI_Funtion[UOC_DI13].timeflag = 0;
			uoc_DI_Funtion[UOC_DI13].timeCount = 0;
			if(DI13() == STATE_LOW)
			{
				pro_test_DI_func[UOC_DI13].flag =1;
			}
		}
	}
	else if(DI13() == STATE_HIGH)
	{
		pro_test_DI_func[UOC_DI13].flag =0;
		uoc_DI_Funtion[UOC_DI13].timeflag = 0;
		uoc_DI_Funtion[UOC_DI13].timeCount = 0;
	}
}

void TEST_UOC_DI14_FUNC(void)
{
	if(DI14() == STATE_LOW)
	{
		uoc_DI_Funtion[UOC_DI14].timeflag = 1;
		if(uoc_DI_Funtion[UOC_DI14].timeCount > test_key_time)
		{
			uoc_DI_Funtion[UOC_DI14].timeflag = 0;
			uoc_DI_Funtion[UOC_DI14].timeCount = 0;
			if(DI14() == STATE_LOW)
			{
				pro_test_DI_func[UOC_DI14].flag =1;
			}
		}
	}
	else if(DI14() == STATE_HIGH)
	{
		pro_test_DI_func[UOC_DI14].flag =0;
		uoc_DI_Funtion[UOC_DI14].timeflag = 0;
		uoc_DI_Funtion[UOC_DI14].timeCount = 0;
	}
}

unsigned char Join_Production_Test(void)
{	
	//通过判断按键进入生产自检模式
	/*if(KEY1() == key_down && KEY4() == key_down)
	{
		uoc_key[0].timeflag = 1;
		if(uoc_key[0].timeout > push_time)
		{
			uoc_key[0].timeflag = 0;
			uoc_key[0].timeout = 0;
			if(KEY1() == key_down && KEY4() == key_down)
			{
				uoc_key[0].flag =1;
				uoc_key[1].flag =1;
				uoc_key[2].flag =1;
				uoc_key[3].flag =1;
				return 1;
			}
		}
		
	}
	else
	{
		uoc_key[0].timeflag = 0;
		uoc_key[0].timeout = 0;
	}*/
	//通过检测io口进入生产自检模式
	if(FL_GPIO_GetInputPin(GPIOA, FL_GPIO_PIN_10) == 0)
	{
		uoc_key[0].timeflag = 1;
		if(uoc_key[0].timeout > push_time)
		{
			uoc_key[0].timeflag = 0;
			uoc_key[0].timeout = 0;
			if(FL_GPIO_GetInputPin(GPIOA, FL_GPIO_PIN_10) == 0)
			{
				uoc_key[0].flag =1;
				uoc_key[1].flag =1;
				uoc_key[2].flag =1;
				uoc_key[3].flag =1;
				return 1;
			}
		}
		
	}
	else
	{
		uoc_key[0].timeflag = 0;
		uoc_key[0].timeout = 0;
	}
	return 0;
}

void Production_Test_Key_release(void)
{
	if(KEY1() == key_up && uoc_key[0].flag ==1)
	{
		uoc_key[0].timeflag = 1;
		if(uoc_key[0].timeout > test_key_time)
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
		if(uoc_key[1].timeout > test_key_time)
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
		if(uoc_key[2].timeout > test_key_time)
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
		if(uoc_key[3].timeout > test_key_time)
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

void Production_Test_Key_press(void)
{	
	if(KEY1() == key_down && uoc_key[0].flag ==0)
	{
		uoc_key[0].timeflag = 1;
		if(uoc_key[0].timeout > test_key_time)
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
		if(uoc_key[1].timeout > test_key_time)
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
		if(uoc_key[2].timeout > test_key_time)
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
		if(uoc_key[3].timeout > test_key_time)
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

void Production_Test_Key_timemout_deal(void)
{
	unsigned char i=0;
	for(i=0;i<key_max;i++)
	{
		if(uoc_key[i].timeout > test_key_time+5)
		{
			uoc_key[i].timeflag = 0;
			uoc_key[i].timeout = 0;
		}
	}
}

void key_func_cyc(void)
{
	Production_Test_Key_press();
	Production_Test_Key_release();
	Production_Test_Key_timemout_deal();
	return;
}
void production_test_flag_close(void)
{
	I2C_Display(DISPLAY1,DISPLAY_NO_POINT,DISPLAY_OFF);
	I2C_Display(DISPLAY2,DISPLAY_NO_POINT,DISPLAY_OFF);
	I2C_Display(DISPLAY3,DISPLAY_NO_POINT,DISPLAY_OFF);
	I2C_Display(DISPLAY4,DISPLAY_NO_POINT,DISPLAY_OFF);
	led_display(UOC_LED_RUN,LED_OFF);
	led_display(UOC_LED_NV,LED_OFF);
	led_display(UOC_LED_NA,LED_OFF);

	Output_Control(UOC_R00,CONTORL_HIGH);
	Output_Control(UOC_R01,CONTORL_HIGH);
	Output_Control(UOC_R02,CONTORL_HIGH);
	Output_Control(UOC_R03,CONTORL_HIGH);
	Output_Control(UOC_R04,CONTORL_HIGH);
	Output_Control(UOC_R05,CONTORL_HIGH);
	Output_Control(UOC_R06,CONTORL_HIGH);

	Output_Control(UOC_D00,CONTORL_HIGH);
	Output_Control(UOC_D01,CONTORL_HIGH);
	Output_Control(UOC_D02,CONTORL_HIGH);
	Output_Control(UOC_D03,CONTORL_HIGH);
	Output_Control(UOC_D04,CONTORL_HIGH);
	Output_Control(UOC_D05,CONTORL_HIGH);
	Output_Control(UOC_D06,CONTORL_HIGH);
	Output_Control(UOC_D07,CONTORL_HIGH);
	Output_Control(UOC_D08,CONTORL_HIGH);
}
void production_test_model(void)
{
	
	if(g_Test_time_count < GetSysTickCount()){
		g_Test_time_count = GetSysTickCount() + 500;
		if(g_display_flag == 0)
		{
			g_display_flag = 1;
			I2C_Display(DISPLAY1,DISPLAY_POINT,8);
			I2C_Display(DISPLAY2,DISPLAY_POINT,8);
			I2C_Display(DISPLAY3,DISPLAY_POINT,8);
			I2C_Display(DISPLAY4,DISPLAY_POINT,8);
			led_display(UOC_LED_RUN,LED_ON);//电源灯亮
			led_display(UOC_LED_NV,LED_ON);//电源灯亮
			led_display(UOC_LED_NA,LED_ON);//电源灯亮
		}
		else
		{
			g_display_flag = 0;
			I2C_Display(DISPLAY1,DISPLAY_NO_POINT,DISPLAY_OFF);
			I2C_Display(DISPLAY2,DISPLAY_NO_POINT,DISPLAY_OFF);
			I2C_Display(DISPLAY3,DISPLAY_NO_POINT,DISPLAY_OFF);
			I2C_Display(DISPLAY4,DISPLAY_NO_POINT,DISPLAY_OFF);
			led_display(UOC_LED_RUN,LED_OFF);
			led_display(UOC_LED_NV,LED_OFF);
			led_display(UOC_LED_NA,LED_OFF);
		}
	}
}

void production_test_model1(void)
{
	unsigned char i=0;
	if(g_model_flag != 1)
	{
		g_model_flag = 1;
		production_test_flag_close();
	}
	
	for(i=0;i<UOC_SPI_MAX_PARA-1;i++)
	{	
		if(uoc_cyc_para[i].start_time + uoc_cyc_para[i].time_out  < GetSysTickCount()){
			uoc_cyc_para[i].start_time = GetSysTickCount();
			uoc_cyc_para[i].cb();
		}
	}
}
void production_test_model2(void)
{
	unsigned char i;
	static unsigned char test_di_count=0;
	if(g_model_flag != 2)
	{
		g_model_flag = 2;
		production_test_flag_close();
		g_Test_time_count = GetSysTickCount() + 1000;
	}
	for(i=0;i<UOC_DIMAX;i++){
		pro_test_DI_func[i].cb();
	}
	if(g_Test_time_count < GetSysTickCount()){
		g_Test_time_count = GetSysTickCount() + 1000;
		for(i=test_di_count+1;i<UOC_DIMAX;i++){
			if(pro_test_DI_func[i].flag == 1)
			{
				test_di_count = i;
				break;
			}
		}
		if(i >= UOC_DIMAX)
		{
			for(i=0;i<UOC_DIMAX;i++){
				if(pro_test_DI_func[i].flag == 1)
				{
					test_di_count = i;
					break;
				}
			}
		}
		if(i < UOC_DIMAX)
		{
			I2C_Display(DISPLAY1,DISPLAY_NO_POINT,DISPLAY_OFF);
			I2C_Display(DISPLAY2,DISPLAY_NO_POINT,DISPLAY_OFF);
			I2C_Display(DISPLAY3,DISPLAY_NO_POINT,(test_di_count/10));
			I2C_Display(DISPLAY4,DISPLAY_NO_POINT,(test_di_count%10));
		}
		else
		{
			I2C_Display(DISPLAY1,DISPLAY_NO_POINT,DISPLAY_OFF);
			I2C_Display(DISPLAY2,DISPLAY_NO_POINT,DISPLAY_OFF);
			I2C_Display(DISPLAY3,DISPLAY_NO_POINT,DISPLAY_OFF);
			I2C_Display(DISPLAY4,DISPLAY_POINT,DISPLAY_OFF);
		}

	}
}
void production_test_model3(void)
{
	if(g_model_flag != 3)
	{
		g_model_flag = 3;
		production_test_flag_close();
		g_Test_time_count = GetSysTickCount() + 1000;
	}
	if(g_Test_time_count < GetSysTickCount()){
		g_Test_time_count = GetSysTickCount() + 1000;
		if(FL_GPIO_GetInputPin(GPIOA, FL_GPIO_PIN_10) == 1)
		{
			MF_SysTemRest();
		}
		if((g_display_flag == 0))
		{
			g_display_flag = 1;
			Output_Control(UOC_R00,CONTORL_LOW);//开启高速风机
			Output_Control(UOC_R01,CONTORL_LOW);//开启高速风机
			//Output_Control(UOC_R02,CONTORL_LOW);
			Output_Control(UOC_R03,CONTORL_LOW);
			Output_Control(UOC_R04,CONTORL_LOW);
			Output_Control(UOC_R05,CONTORL_LOW);
			Output_Control(UOC_R06,CONTORL_LOW);

			Output_Control(UOC_D00,CONTORL_LOW);
			Output_Control(UOC_D01,CONTORL_LOW);
			Output_Control(UOC_D03,CONTORL_LOW);
			Output_Control(UOC_D04,CONTORL_LOW);
			Output_Control(UOC_D05,CONTORL_LOW);
			Output_Control(UOC_D07,CONTORL_LOW);
			Output_Control(UOC_D08,CONTORL_LOW);

			Output_Control(UOC_R02,CONTORL_HIGH);
			Output_Control(UOC_D06,CONTORL_HIGH);
			Output_Control(UOC_D02,CONTORL_HIGH);
		}
		else
		{
			g_display_flag = 0;
			Output_Control(UOC_R00,CONTORL_HIGH);
			Output_Control(UOC_R01,CONTORL_HIGH);
			//Output_Control(UOC_R02,CONTORL_HIGH);
			Output_Control(UOC_R03,CONTORL_HIGH);
			Output_Control(UOC_R04,CONTORL_HIGH);
			Output_Control(UOC_R05,CONTORL_HIGH);
			Output_Control(UOC_R06,CONTORL_HIGH);

			Output_Control(UOC_D00,CONTORL_HIGH);
			Output_Control(UOC_D01,CONTORL_HIGH);
			Output_Control(UOC_D03,CONTORL_HIGH);
			Output_Control(UOC_D04,CONTORL_HIGH);
			Output_Control(UOC_D05,CONTORL_HIGH);
			Output_Control(UOC_D07,CONTORL_HIGH);
			Output_Control(UOC_D08,CONTORL_HIGH);

			Output_Control(UOC_R02,CONTORL_LOW);
			Output_Control(UOC_D06,CONTORL_LOW);
			Output_Control(UOC_D02,CONTORL_LOW);
		}
	}
}
void production_test_model4(void)
{
	if(g_model_flag != 4)
	{
		g_model_flag = 4;
		production_test_flag_close();
		g_Test_time_count = GetSysTickCount() + 1000;
	}
	if(g_Test_time_count < GetSysTickCount()){
		g_Test_time_count = GetSysTickCount() + 3000;
		if(g_display_flag == 0)
		{
			g_display_flag = 1;
			
			Output_Control(UOC_D02,CONTORL_LOW);
			Output_Control(UOC_D06,CONTORL_HIGH);
			Output_Control(UOC_R02,CONTORL_LOW);
		}
		else
		{
			g_display_flag = 0;
			Output_Control(UOC_D02,CONTORL_HIGH);
			Output_Control(UOC_D06,CONTORL_LOW);
			Output_Control(UOC_R02,CONTORL_HIGH);

		}
	}
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
			key_func_cyc();
			pro_test_func[key_state].cb();
		}
	}
}
