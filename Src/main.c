#include "main.h"
#include "fm33le0xx_fl.h"
#include "spi.h"
#include "led.h"
#include "timer.h"
#include "button_function.h"
#include "debug.h"
#include "stdio.h"
#include "i2c.h"
#include "parameter.h"
#include "system_control.h"
#include "flash_api.h"
#include "production_test.h"

extern gUOCLed_TypeDef UOCLed[UOC_LED_MAX];
uint8_t  g_tmpRxBuf[256];
volatile uint32_t g_u32SysTickCount = 0;
extern gUOCParameter_TypeDef g_uoc_param;
extern gUOCParameter_TypeDef g_uoc_init_param;
extern gUOCControl_StateDef UOC_Input_State;
uint8_t UOC_VER[4] = {2,3,3,3};
void uoc_wdt_init(void)
{
    FL_IWDT_InitTypeDef IWDT_InitStruct;

    IWDT_InitStruct.overflowPeriod = FL_IWDT_PERIOD_4000MS;
    IWDT_InitStruct.iwdtWindows = 0;

    FL_IWDT_Init(IWDT, &IWDT_InitStruct);
    //uoc_wdtNvic_config();
}
void uoc_wdt_feed(void)
{
    FL_IWDT_ReloadCounter(IWDT);
}

void Debug_dispaly_init(void)
{
	unsigned char testbuf[8]={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
	dbg_array_buffer("test",testbuf,8);
	debug_log("%s:%d: \n",__func__,__LINE__);
	debug_err("%s:%d: \n",__func__,__LINE__);
	return;
}
void Spi_data_init(void)
{
	do
	{
		SPI_Mode_init();
	}
	while (SPI_Check());	
	debug_log("%s:%d: \n",__func__,__LINE__);
	return;
}
void Display_init(void)
{
	I2C_Display(DISPLAYCMD,DISPLAY_POINT,3);
	I2C_Display(DISPLAY1,DISPLAY_NO_POINT,UOC_VER[0]);
	I2C_Display(DISPLAY2,DISPLAY_NO_POINT,UOC_VER[1]);
	I2C_Display(DISPLAY3,DISPLAY_NO_POINT,UOC_VER[2]);
	I2C_Display(DISPLAY4,DISPLAY_NO_POINT,UOC_VER[3]);
	return;
}

void UOC_Fan_Static_Clean(void)
{
	UOC_Input_State.ready_Ordinary = 0;
	UOC_Input_State.Ordinary_fan = 0;
	UOC_Input_State.high_speed_fan = 0;
	UOC_Input_State.ready_high = 0;
	UOC_Input_State.fan_timeCount = 0;//停止启动前所有预备动作
	UOC_Input_State.fan_flag = 0;
}

void UOC_Input_Res(unsigned char i)
{
	/*
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
	*/
	if(i == High_Speed_Fan_Dynamic_Stop){
		UOC_Input_State.Smoke_Exhaust_Valve_Stop_flag = 0;
	}
	else if(i == High_Speed_Fan_Dynamic_Start){
		UOC_Input_State.Smoke_Exhaust_Valve_Start_flag = 0;
	}
	else if(i == Fire_Damper){
		UOC_Input_State.Fire_Damper_flag = 1;
	}else if(i == Controlled_Feedback){
		UOC_Input_State.run_flag = 1;
	}
	return;
}

void sys_control_init(void)
{
	unsigned char i = 0;
	Output_Control(UOC_D04,CONTORL_LOW);//柜门电源指示灯开
	Output_Control(UOC_D06,CONTORL_HIGH);//故障报警关

	Output_Control(UOC_D03,CONTORL_HIGH);//反馈指示灯关闭

	Output_Control(UOC_D07,CONTORL_HIGH);//高速风机指示灯开启
	Output_Control(UOC_D08,CONTORL_HIGH);//普通风机指示灯关闭
	Output_Control(UOC_D02,CONTORL_HIGH);

	Control_High_Speed_Fan(CLOSE);
	Control_Ordinary_Fan(CLOSE);

	for(i=0;i<UOC_R03R04R05R06_STATE_MAX;i++)
		Control_R03R04R05R06_Function(i,CLOSE);

	UOC_Input_State.Smoke_Exhaust_Valve_Stop_flag = 0;
	UOC_Input_State.Smoke_Exhaust_Valve_Start_flag = 0;
	UOC_Input_State.Fire_Damper_flag = 1;
	UOC_Input_State.run_flag = 1;
	UOC_Input_State.static_clean = UOC_Fan_Static_Clean;
	UOC_Input_State.static_res = UOC_Input_Res;
	
}
void UOC_Sys_Param_init(void)
{
	uint8_t dataTmp[FLASH_PAGE_SIZE] = {0};
	g_uoc_param = g_uoc_init_param;
    if (true == flase_read(FLASH_SYSPARAM_ID, dataTmp, sizeof(gUOCParameter_TypeDef)))
    {
    	debug_log("%s:%d:read flash success!!!!\n",__func__,__LINE__);
    	if(dataTmp[0] != 0xFF){
        	memcpy((uint8_t *)&g_uoc_param, dataTmp, sizeof(gUOCParameter_TypeDef));
			debug_log("%s:%d:set g_uoc_param success!!!!\n",__func__,__LINE__);
    	}
		Show_Param_info();
    }
}
void UOC_sys_data_init(void)
{
	sys_control_init(); 
	Spi_data_init();
	Display_init();
	Debug_dispaly_init();	
	UOC_Sys_Param_init();
	return;
}

int main(void)
{   
	
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    /* SHOULD BE KEPT!!! */
    MF_Clock_Init();
    
    /* Configure the system clock */
    /* SHOULD BE KEPT!!! */
    
 	MF_SystemClock_Config();
    /* Initialize FL Driver Library */
    /* SHOULD BE KEPT!!! */
    FL_Init();

    /* Initialize all configured peripherals */
    /* SHOULD BE KEPT!!! */
    MF_Config_Init();
	//uoc_wdt_init();
	 
	UOC_sys_data_init();

	dbg_printf("Main start\n");
	while(GetSysTickCount() < int_time)
	{
		production_test_main();
	}

	sys_control_init(); 

    while(1)
    {     
    	//uoc_wdt_feed();
    	if(UART_RxFlag(1))//打印口
		{					
		  		uint16_t len;
				len = UART_RxGet(1,g_tmpRxBuf,sizeof(g_tmpRxBuf));	
				Debug_Display(g_tmpRxBuf,len);
		}
		System_Funtion();
		UOC_spi_read_data_cycle();
		Input_Detection();
    }
}


