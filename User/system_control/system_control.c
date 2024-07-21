#include "system_control.h"
#include "timer.h"
#include"debug.h"

F12_PROTOCOL send_buf={0xf0,0x00,0x55};

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
};

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
void protocol_deal(void *buf,uint8_t len)
{
	uint8_t bit = 0;
	uint8_t i = 0;
	F12_PROTOCOL send_data = {0xf1,0x00,0x55};
	F12_PROTOCOL *rev_data = (F12_PROTOCOL*)buf;
	if(rev_data->head == 0xf0 && rev_data->end == 0x55)
	{
		for(i=1;i<8;i++){
			bit = (rev_data->data >> i) & 0x01;
			Output_Control(i-1,bit);
		}
		send_data.data = 0x01;
		UART_Tx(5,&send_data,sizeof(F12_PROTOCOL));
		return;
	}
	UART_Tx(5,&send_data,sizeof(F12_PROTOCOL));
	return;
}



void UOC_DI0_FUNC(void)
{
	static unsigned char flag =0;
	if(DI0() == STATE_LOW && flag == 0)
	{
		uoc_DI_Funtion[UOC_DI0].timeflag = 1;
		if(uoc_DI_Funtion[UOC_DI0].timeCount > (keep_time))// +(g_uoc_param.ButtonInputDelay*100)))
		{
			uoc_DI_Funtion[UOC_DI0].timeflag = 0;
			uoc_DI_Funtion[UOC_DI0].timeCount = 0;
			if(DI0() == STATE_LOW && flag == 0)
			{
				flag =1;
				debug_log("%s:%d: UOC_DI0:0\n",__func__,__LINE__);
				send_buf.data |= 0x01;
				UART_Tx(5,&send_buf,sizeof(F12_PROTOCOL));
			}
		}
	}
	else if(DI0() == STATE_HIGH)
	{
		flag =0;
		uoc_DI_Funtion[UOC_DI0].timeflag = 0;
		uoc_DI_Funtion[UOC_DI0].timeCount = 0;
	}
	send_buf.data &= 0xfe;
	return;
}
void UOC_DI1_FUNC(void)
{
	static unsigned char flag =0;
	if(DI1() == STATE_LOW && flag == 0)
	{
		uoc_DI_Funtion[UOC_DI1].timeflag = 1;
		if(uoc_DI_Funtion[UOC_DI1].timeCount > (keep_time))// +(g_uoc_param.ButtonInputDelay*100)))
		{
			uoc_DI_Funtion[UOC_DI1].timeflag = 0;
			uoc_DI_Funtion[UOC_DI1].timeCount = 0;
			if(DI1() == STATE_LOW && flag == 0)
			{
				flag =1;
				debug_log("%s:%d: UOC_DI1:0\n",__func__,__LINE__);
				send_buf.data |= 0x01 << 1;
				UART_Tx(5,&send_buf,sizeof(F12_PROTOCOL));
			}
		}
	}
	else if(DI1() == STATE_HIGH)
	{
		flag =0;
		uoc_DI_Funtion[UOC_DI1].timeflag = 0;
		uoc_DI_Funtion[UOC_DI1].timeCount = 0;
	}
	send_buf.data &= 0xFD;
	return;
}
void UOC_DI2_FUNC(void)
{
	static unsigned char flag =0;
	if(DI2() == STATE_LOW)
	{
		uoc_DI_Funtion[UOC_DI2].timeflag = 1;
		if(uoc_DI_Funtion[UOC_DI2].timeCount > (keep_time))// +(g_uoc_param.ButtonInputDelay*100)))
		{
			uoc_DI_Funtion[UOC_DI2].timeflag = 0;
			uoc_DI_Funtion[UOC_DI2].timeCount = 0;
			if(DI2() == STATE_LOW && flag == 0)
			{
				flag =1;
				debug_log("%s:%d: UOC_DI2:0\n",__func__,__LINE__);
				send_buf.data |= 0x01 << 2;
				UART_Tx(5,&send_buf,sizeof(F12_PROTOCOL));
			}
		}
	}
	else if(DI2() == STATE_HIGH)
	{
		flag =0;
		uoc_DI_Funtion[UOC_DI2].timeflag = 0;
		uoc_DI_Funtion[UOC_DI2].timeCount = 0;
	}
	send_buf.data &= 0xFB;
	return;
}
void UOC_DI3_FUNC(void)
{
	static unsigned char flag =0;
	if(DI3() == STATE_LOW && flag == 0)
	{
		uoc_DI_Funtion[UOC_DI3].timeflag = 1;
		if(uoc_DI_Funtion[UOC_DI3].timeCount > (keep_time))// +(g_uoc_param.ButtonInputDelay*100)))
		{
			uoc_DI_Funtion[UOC_DI3].timeflag = 0;
			uoc_DI_Funtion[UOC_DI3].timeCount = 0;
			if(DI3() == STATE_LOW && flag == 0)
			{
				flag =1;
				debug_log("%s:%d: UOC_DI3:0\n",__func__,__LINE__);
				send_buf.data |= 0x01 << 3;
				UART_Tx(5,&send_buf,sizeof(F12_PROTOCOL));
			}
		}
	}
	else if(DI3() == STATE_HIGH)
	{
		flag =0;
		uoc_DI_Funtion[UOC_DI3].timeflag = 0;
		uoc_DI_Funtion[UOC_DI3].timeCount = 0;
	}
	send_buf.data &= 0xF7;
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
				send_buf.data |= 0x01 << 4;
				UART_Tx(5,&send_buf,sizeof(F12_PROTOCOL));

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
				send_buf.data &= 0x00;
				UART_Tx(5,&send_buf,sizeof(F12_PROTOCOL));
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
