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
uint8_t UOC_VER[4] = {2,3,3,9};
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

void sys_control_init(void)
{

	
}
void UOC_Sys_Param_init(void)
{
	// uint8_t dataTmp[FLASH_PAGE_SIZE] = {0};
	// g_uoc_param = g_uoc_init_param;
    // if (true == flase_read(FLASH_SYSPARAM_ID, dataTmp, sizeof(gUOCParameter_TypeDef)))
    // {
    // 	debug_log("%s:%d:read flash success!!!!\n",__func__,__LINE__);
    // 	if(dataTmp[0] != 0xFF){
    //     	memcpy((uint8_t *)&g_uoc_param, dataTmp, sizeof(gUOCParameter_TypeDef));
	// 		debug_log("%s:%d:set g_uoc_param success!!!!\n",__func__,__LINE__);
    // 	}
		
    // }
}
void UOC_sys_data_init(void)
{
	//sys_control_init(); 
	Debug_dispaly_init();	
	//UOC_Sys_Param_init();
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
		if(UART_RxFlag(5))//打印口
		{					
		  		uint16_t len;
				len = UART_RxGet(5,g_tmpRxBuf,sizeof(g_tmpRxBuf));
					
				Debug_Display(g_tmpRxBuf,len);
		}
		System_Funtion();
		UOC_spi_read_data_cycle();
		Input_Detection();
    }
}


