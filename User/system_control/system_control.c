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
	{UOC_R00,	GPIOA,	FL_GPIO_PIN_8},
	{UOC_R01,	GPIOA,	FL_GPIO_PIN_7},
	{UOC_R02,	GPIOA,	FL_GPIO_PIN_6},
	{UOC_R03,	GPIOA,	FL_GPIO_PIN_5},
	{UOC_R04,	GPIOA,	FL_GPIO_PIN_4},
	{UOC_R05,	GPIOA,	FL_GPIO_PIN_3},
	{UOC_R06,	GPIOA,	FL_GPIO_PIN_2},
	{UOC_D00,	GPIOA,	FL_GPIO_PIN_0},
	{UOC_D01,	GPIOA,	FL_GPIO_PIN_15},
	{UOC_D02,	GPIOA,	FL_GPIO_PIN_14},
	{UOC_D03,	GPIOA, 	FL_GPIO_PIN_13},
	{UOC_D04,	GPIOD, 	FL_GPIO_PIN_6},
	{UOC_D05,	GPIOD, 	FL_GPIO_PIN_5},
	{UOC_D06,	GPIOD, 	FL_GPIO_PIN_4},
	{UOC_D07,	GPIOD, 	FL_GPIO_PIN_3},
	{UOC_D08,	GPIOD, 	FL_GPIO_PIN_2}
};

sDInputEvent_TypeDef uoc_DI_Funtion[UOC_DIMAX] =
{
	{UOC_DI0,0,0, UOC_DI0_FUNC},
	{UOC_DI1,0,0, UOC_DI1_FUNC},
	{UOC_DI2,0,0, UOC_DI2_FUNC},
	{UOC_DI3,0,0, UOC_DI3_FUNC},
	{UOC_DI4,0,0, UOC_DI4_FUNC},
	{UOC_DI5,0,0, UOC_DI5_FUNC},
	{UOC_DI6,0,0, UOC_DI6_FUNC},
	{UOC_DI7,0,0, UOC_DI7_FUNC},
	{UOC_DI8,0,0, UOC_DI8_FUNC},
	{UOC_DI9,0,0, UOC_DI9_FUNC},
	{UOC_DI10,0,0, UOC_DI10_FUNC},
	{UOC_DI11,0,0, UOC_DI11_FUNC},
	{UOC_DI12,0,0, UOC_DI12_FUNC},
	{UOC_DI13,0,0, UOC_DI13_FUNC},
	{UOC_DI14,0,0, UOC_DI14_FUNC}
};
gUOCControl_StateDef UOC_Input_State={0};


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

void Control_R03R04R05R06_Function(unsigned id,unsigned char flag)
{
	if(g_uoc_param.R03OutputDef == id)
	{
		if(flag == OPEN)
		{
			Output_Control(UOC_R03,CONTORL_LOW);
		}
		else if(flag == CLOSE)
		{
			Output_Control(UOC_R03,CONTORL_HIGH);
		}
	}
	if(g_uoc_param.R04OutputDef == id)
	{
		if(flag == OPEN)
		{
			Output_Control(UOC_R04,CONTORL_LOW);
		}
		else if(flag == CLOSE)
		{
			Output_Control(UOC_R04,CONTORL_HIGH);
		}
	}
	if(g_uoc_param.R05OutputDef == id)
	{
		if(flag == OPEN)
		{
			Output_Control(UOC_R05,CONTORL_LOW);
		}
		else if(flag == CLOSE)
		{
			Output_Control(UOC_R05,CONTORL_HIGH);
		}
	}
	if(g_uoc_param.R06OutputDef == id)
	{
		if(flag == OPEN)
		{
			Output_Control(UOC_R06,CONTORL_LOW);
		}
		else if(flag == CLOSE)
		{
			Output_Control(UOC_R06,CONTORL_HIGH);
		}
	}
	return;
}




void Control_High_Speed_Fan(unsigned char flag)
{
	if(flag == OPEN)
	{
		Control_R03R04R05R06_Function(UOC_SMOKE_EXHAUST_VALVE_CLOSE,CLOSE);//启动风机 则关闭此输出
		if((UOC_Input_State.Fire_Damper_flag == 1) || (g_uoc_param.FanStopState == 0))
		{
			if(UOC_Input_State.Smoke_Exhaust_Valve_Start_flag == 1){
				UOC_Input_State.fan_flag = 2;
				UOC_Input_State.Smoke_Exhaust_Valve_Start_check_flag = 1;
				UOC_Input_State.Smoke_timeCount = 0;
				Control_R03R04R05R06_Function(UOC_SMOKE_EXHAUST_VALVE_OPEN,OPEN);
			}
			else
			{
				UOC_Input_State.fan_flag = 3;
				UOC_Input_State.fan_timeCount = 0;
				Output_Control(UOC_R00,CONTORL_LOW);//高速风机开启
				Output_Control(UOC_R01,CONTORL_LOW);//高速风机开启
			}
			Control_R03R04R05R06_Function(UOC_HIGH_SPEED_RUN,OPEN);//反馈
			Output_Control(UOC_D07,CONTORL_LOW);//高速风机指示灯开启
			Output_Control(UOC_D08,CONTORL_HIGH);//普通风机指示灯关闭
			if(UOC_Input_State.run_flag == 1){
				Output_Control(UOC_D03,CONTORL_LOW);//设备运行打开
				Control_R03R04R05R06_Function(UOC_CONTROLLED_RUN,OPEN);
			}
			else{
				Output_Control(UOC_D03,CONTORL_HIGH);//设备运行关闭
				Control_R03R04R05R06_Function(UOC_CONTROLLED_RUN,CLOSE);
			}
		}
		else if(UOC_Input_State.Fire_Damper_flag == 0)
		{
			UOC_Input_State.ready_high = 0;
			uoc_ERR_Flag_clean();
			UOCPara.new_state = F_ERR;
			UOC_ERR_PARA[E08].flag = 1;
		}
			
	}
	else if(flag == CLOSE)
	{
		Output_Control(UOC_R00,CONTORL_HIGH);//高速风机关闭
		Output_Control(UOC_R01,CONTORL_HIGH);//高速风机关闭
		Control_R03R04R05R06_Function(UOC_HIGH_SPEED_RUN,CLOSE);//反馈
		Output_Control(UOC_D03,CONTORL_HIGH);//设备运行关闭
		Control_R03R04R05R06_Function(UOC_CONTROLLED_RUN,CLOSE);
		Output_Control(UOC_D07,CONTORL_HIGH);//高速风机指示灯关闭
		if(UOC_Input_State.Smoke_Exhaust_Valve_Stop_flag == 1){
			Control_R03R04R05R06_Function(UOC_SMOKE_EXHAUST_VALVE_CLOSE,OPEN);
			UOC_Input_State.Smoke_Exhaust_Valve_Stop_check_flag = 1;
			UOC_Input_State.Smoke_timeCount = 0;
		}
	}
	return;
}

void Control_Ordinary_Fan(unsigned char flag)
{
	if(flag == OPEN)
	{
		if((UOC_Input_State.Fire_Damper_flag == 1) || (g_uoc_param.FanStopState == 0))
		{
			if(UOC_Input_State.Smoke_Exhaust_Valve_Start_flag == 1){
				UOC_Input_State.fan_flag = 2;
				UOC_Input_State.Smoke_Exhaust_Valve_Start_check_flag = 1;
				UOC_Input_State.Smoke_timeCount = 0;
				Control_R03R04R05R06_Function(UOC_SMOKE_EXHAUST_VALVE_OPEN,OPEN);
			}
			else
			{
				UOC_Input_State.fan_flag = 3;
				UOC_Input_State.fan_timeCount = 0;
				Output_Control(UOC_R02,CONTORL_LOW);//普通风机开启
			}
			Control_R03R04R05R06_Function(UOC_LOW_SPEED_RUN,OPEN);//反馈

			Output_Control(UOC_D08,CONTORL_LOW);//普通风机指示灯开启
			Output_Control(UOC_D07,CONTORL_HIGH);//高速风机指示灯关闭
			if(UOC_Input_State.run_flag == 1){
				Output_Control(UOC_D03,CONTORL_LOW);//设备运行打开
				Control_R03R04R05R06_Function(UOC_CONTROLLED_RUN,OPEN);
			}
			else{
				Output_Control(UOC_D03,CONTORL_HIGH);//设备运行关闭
				Control_R03R04R05R06_Function(UOC_CONTROLLED_RUN,CLOSE);
			}

			UOC_Input_State.Smoke_Exhaust_Valve_Stop_check_flag = 0;
			Control_R03R04R05R06_Function(UOC_SMOKE_EXHAUST_VALVE_CLOSE,CLOSE);
		}
		else if(UOC_Input_State.Fire_Damper_flag == 0)
		{
			UOC_Input_State.ready_Ordinary = 0;
			uoc_ERR_Flag_clean();
			UOCPara.new_state = F_ERR;
			UOC_ERR_PARA[E08].flag = 1;
		}
	}
	else if(flag == CLOSE)
	{
		Output_Control(UOC_R02,CONTORL_HIGH);//普通风机关闭
		Control_R03R04R05R06_Function(UOC_LOW_SPEED_RUN,CLOSE);//反馈

		Output_Control(UOC_D03,CONTORL_HIGH);//设备运行关闭
		Control_R03R04R05R06_Function(UOC_CONTROLLED_RUN,CLOSE);

		Output_Control(UOC_D08,CONTORL_HIGH);//普通风机指示灯关闭
		UOC_Input_State.Smoke_Exhaust_Valve_Start_check_flag = 0;
		Control_R03R04R05R06_Function(UOC_SMOKE_EXHAUST_VALVE_OPEN,CLOSE);
		if(UOC_Input_State.Smoke_Exhaust_Valve_Stop_flag == 1){
			UOC_Input_State.Smoke_Exhaust_Valve_Stop_check_flag = 1;
			UOC_Input_State.Smoke_timeCount = 0;
			Control_R03R04R05R06_Function(UOC_SMOKE_EXHAUST_VALVE_CLOSE,OPEN);
		}
	}
	return;
}

void UOC_Trip_Fan(void)//跳闸 关闭高速 低速风机
{
	if(UOC_Input_State.high_speed_fan ==1)
	{
		UOC_Input_State.high_speed_fan = 0;
		Control_High_Speed_Fan(CLOSE);
		Output_Control(UOC_D03,CONTORL_HIGH);//设备运行反馈关闭
		Control_R03R04R05R06_Function(UOC_CONTROLLED_RUN,CLOSE);
		UOC_Input_State.Auto_trip = 1;
	}
	else if(UOC_Input_State.Ordinary_fan == 1)
	{
		UOC_Input_State.Ordinary_fan = 0;//高速风机可以关闭普通风机
		Control_Ordinary_Fan(CLOSE);
		Output_Control(UOC_D03,CONTORL_HIGH);//设备运行反馈关闭
		Control_R03R04R05R06_Function(UOC_CONTROLLED_RUN,CLOSE);
		UOC_Input_State.Auto_trip = 1;
	}				
}

void Control_DI7_DI14_Function(unsigned id,unsigned char flag)
{
	static unsigned char Control_flag_buf[DI7_DI14MAX]={0};
	if(flag == OPEN)
	{
		switch (id)
		{
			case External_Control_Stop://无论什么状态 直接关闭所有风机
				if(UOC_Input_State.mode_flag== 0)
					return;
				UOC_Input_State.static_clean();

				Control_High_Speed_Fan(CLOSE);
				Control_Ordinary_Fan(CLOSE);

				Output_Control(UOC_D03,CONTORL_HIGH);//高速风机反馈关闭
				Control_R03R04R05R06_Function(UOC_CONTROLLED_RUN,CLOSE);
				break;
			case External_Control_Start:
				if(UOC_Input_State.mode_flag== 0 || (UOC_Input_State.Auto_trip == 1))
					return;
				if(UOC_Input_State.high_speed_fan == 0 && UOC_Input_State.ready_Ordinary == 0 && UOC_Input_State.ready_high == 0)//高速风机启动
				{
					if(UOC_Input_State.Ordinary_fan == 1)//低速风机开启状态中
					{
						UOC_Input_State.ready_high = 1;
						UOC_Input_State.Ordinary_fan = 0;
						Control_Ordinary_Fan(CLOSE);
						UOC_Input_State.fan_flag = 1;//低速风机转高速风机标识
					}
					else//如果低速风机未开启则直接启动
					{
						UOC_Input_State.ready_high = 1;//准备开启高速风机
					}
				}
				break;
			case Emergency_Start:
				if(UOC_Input_State.mode_flag== 0 ||(UOC_Input_State.Auto_trip == 1))
					return;
				Control_flag_buf[Emergency_Start] = OPEN;
				if(UOC_Input_State.high_speed_fan == 0 && UOC_Input_State.ready_Ordinary == 0 && UOC_Input_State.ready_high == 0)//高速风机启动
				{
					if(UOC_Input_State.Ordinary_fan == 1)//低速风机开启状态中
					{
						UOC_Input_State.ready_high = 1;
						UOC_Input_State.Ordinary_fan = 0;
						Control_Ordinary_Fan(CLOSE);
						UOC_Input_State.fan_flag = 1;//低速风机转高速风机标识
					}
					else//如果低速风机未开启则直接启动
					{
						UOC_Input_State.ready_high = 1;//准备开启高速风机
					}
				}
				break;
			case Dynamic_Control:
				Control_flag_buf[Dynamic_Control] = OPEN;
				if(UOC_Input_State.mode_flag && (UOC_Input_State.Auto_trip == 0)){
					if(UOC_Input_State.high_speed_fan == 0 && UOC_Input_State.ready_Ordinary == 0 && UOC_Input_State.ready_high == 0)//高速风机启动
					{
						if(UOC_Input_State.Ordinary_fan == 1)//低速风机开启状态中
						{
							UOC_Input_State.ready_high = 1;
							UOC_Input_State.Ordinary_fan = 0;
							Control_Ordinary_Fan(CLOSE);
							UOC_Input_State.fan_flag = 1;//低速风机转高速风机标识
						}
						else//如果低速风机未开启则直接启动
						{
							UOC_Input_State.ready_high = 1;//准备开启高速风机
						}
					}
				}
				Output_Control(UOC_D02,CONTORL_LOW);//联动控制打开
				break;
			case Low_Speed_Fan:
				if(UOC_Input_State.mode_flag== 0 ||(UOC_Input_State.Auto_trip == 1))
					return;
				if(UOC_Input_State.high_speed_fan == 0 && UOC_Input_State.Ordinary_fan == 0 && UOC_Input_State.ready_high == 0 && UOC_Input_State.ready_Ordinary == 0)//低速风机启动
				{
					if(g_uoc_param.FanType != 1)
						UOC_Input_State.ready_Ordinary = 1;//准备开启普通风机
				}
				break;
			case High_Speed_Fan:
				if(UOC_Input_State.mode_flag== 0 ||(UOC_Input_State.Auto_trip == 1))
					return;
				Control_flag_buf[High_Speed_Fan] = OPEN;
				if(UOC_Input_State.high_speed_fan == 0 && UOC_Input_State.ready_Ordinary == 0 && UOC_Input_State.ready_high == 0)//高速风机启动
				{
					if(UOC_Input_State.Ordinary_fan == 1)//低速风机开启状态中
					{
						UOC_Input_State.ready_high = 1;
						UOC_Input_State.Ordinary_fan = 0;
						Control_Ordinary_Fan(CLOSE);
						UOC_Input_State.fan_flag = 1;//低速风机转高速风机标识
					}
					else//如果低速风机未开启则直接启动
					{
						UOC_Input_State.ready_high = 1;//准备开启高速风机
					}
				}
				break;
			case Controlled_Feedback:
				if((UOC_Input_State.Ordinary_fan != 0) || (UOC_Input_State.high_speed_fan != 0) || (UOC_Input_State.ready_high != 0) || (UOC_Input_State.ready_Ordinary != 0)){ 
					Control_R03R04R05R06_Function(UOC_CONTROLLED_RUN,OPEN);
					Output_Control(UOC_D03,CONTORL_LOW);//设备运行打开
				}
				UOC_Input_State.run_flag = 1;
				break;
			case Fire_Damper:
				UOC_Input_State.Fire_Damper_flag = 1;
				break;
			case High_Speed_Fan_Dynamic_Start:
				if(UOC_Input_State.mode_flag== 0 ||(UOC_Input_State.Auto_trip == 1))
					return;
				if(UOC_Input_State.high_speed_fan == 0 && UOC_Input_State.ready_Ordinary == 0 && UOC_Input_State.ready_high == 0)//高速风机启动
				{
					if(UOC_Input_State.Ordinary_fan == 1)//低速风机开启状态中
					{
						UOC_Input_State.ready_high = 1;
						UOC_Input_State.Ordinary_fan = 0;
						Control_Ordinary_Fan(CLOSE);
						UOC_Input_State.fan_flag = 1;//低速风机转高速风机标识
					}
					else//如果低速风机未开启则直接启动
					{
						UOC_Input_State.ready_high = 1;//准备开启高速风机
					}
				}
				Output_Control(UOC_D02,CONTORL_LOW);//联动控制打开
				break;
			case High_Speed_Fan_Dynamic_Stop://无论什么状态直接停止
				UOC_Input_State.static_clean();
				Control_High_Speed_Fan(CLOSE);
				Control_Ordinary_Fan(CLOSE);

				Output_Control(UOC_D03,CONTORL_HIGH);//高速风机反馈关闭
				Control_R03R04R05R06_Function(UOC_CONTROLLED_RUN,CLOSE);
				debug_log("%s:%d: External_Control_Stop!\n",__func__,__LINE__);
				break;
			case Low_Speed_Fan_Dynamic_Start:
				if(UOC_Input_State.mode_flag== 0 ||(UOC_Input_State.Auto_trip == 1))
					return;
				if(UOC_Input_State.high_speed_fan == 0 && UOC_Input_State.Ordinary_fan == 0 && UOC_Input_State.ready_high == 0 && UOC_Input_State.ready_Ordinary == 0)//低速风机启动
				{
					if(g_uoc_param.FanType != 1)
						UOC_Input_State.ready_Ordinary = 1;//准备开启普通风机
				}
				//Output_Control(UOC_D02,CONTORL_LOW);//联动控制打开
				break;
			case Smoke_Exhaust_Valve_Stop:
				UOC_Input_State.Smoke_Exhaust_Valve_Stop_flag = 0;
				break;
			case Smoke_Exhaust_Valve_Start:
				UOC_Input_State.Smoke_Exhaust_Valve_Start_flag  = 0;
				break;
			default:
			break;
		}
	}
	else if(flag == CLOSE)
	{
		switch (id)
		{
			case External_Control_Stop:
				
				break;
			case External_Control_Start:
				
				break;
			case Emergency_Start:
				if(UOC_Input_State.mode_flag== 0 ||(UOC_Input_State.Auto_trip == 1))
					return;
				Control_flag_buf[Emergency_Start] = CLOSE;
				if((Control_flag_buf[Emergency_Start] == CLOSE) && (Control_flag_buf[Dynamic_Control] == CLOSE) && (Control_flag_buf[High_Speed_Fan] == CLOSE))
				{
					UOC_Input_State.static_clean();
					Control_High_Speed_Fan(CLOSE);
					Control_Ordinary_Fan(CLOSE);

					Output_Control(UOC_D03,CONTORL_HIGH);//高速风机反馈关闭
					Control_R03R04R05R06_Function(UOC_CONTROLLED_RUN,CLOSE);
					debug_log("%s:%d: Emergency_Start close!\n",__func__,__LINE__);
				}
				break;
			case Dynamic_Control:
				Control_flag_buf[Dynamic_Control] = CLOSE;
				if(UOC_Input_State.mode_flag && (UOC_Input_State.Auto_trip == 0)){
					if((Control_flag_buf[Emergency_Start] == CLOSE) && (Control_flag_buf[Dynamic_Control] == CLOSE) && (Control_flag_buf[High_Speed_Fan] == CLOSE))
					{
						UOC_Input_State.static_clean();
						Control_High_Speed_Fan(CLOSE);
						Control_Ordinary_Fan(CLOSE);

						Output_Control(UOC_D03,CONTORL_HIGH);//高速风机反馈关闭
						Control_R03R04R05R06_Function(UOC_CONTROLLED_RUN,CLOSE);
						debug_log("%s:%d: Dynamic_Control close!\n",__func__,__LINE__);
					}
				}
				Output_Control(UOC_D02,CONTORL_HIGH);//联动控制关闭
				break;
			case Low_Speed_Fan:
				if(UOC_Input_State.mode_flag== 0 ||(UOC_Input_State.Auto_trip == 1))
					return;
				if((UOC_Input_State.Ordinary_fan == 1) ||(UOC_Input_State.ready_Ordinary == 1))//低速风机停止
				{
					UOC_Input_State.static_clean();
					Control_Ordinary_Fan(CLOSE);
					debug_log("%s:%d: Low_Speed_Fan close\n",__func__,__LINE__);
					Output_Control(UOC_D03,CONTORL_HIGH);//高速风机反馈关闭
					Control_R03R04R05R06_Function(UOC_CONTROLLED_RUN,CLOSE);
				}
				break;
			case High_Speed_Fan:
				Control_flag_buf[High_Speed_Fan] = CLOSE;
				if((Control_flag_buf[Emergency_Start] == CLOSE) && (Control_flag_buf[Dynamic_Control] == CLOSE) && (Control_flag_buf[High_Speed_Fan] == CLOSE))
				{
					UOC_Input_State.static_clean();
					Control_High_Speed_Fan(CLOSE);
					Control_Ordinary_Fan(CLOSE);

					Output_Control(UOC_D03,CONTORL_HIGH);//设备运行关闭
					Control_R03R04R05R06_Function(UOC_CONTROLLED_RUN,CLOSE);
					debug_log("%s:%d: High_Speed_Fan close!\n",__func__,__LINE__);
				}
				break;
			case Controlled_Feedback:
				UOC_Input_State.run_flag = 0;
				Control_R03R04R05R06_Function(UOC_CONTROLLED_RUN,CLOSE);
				Output_Control(UOC_D03,CONTORL_HIGH);//设备运行关闭
				break;
			case Fire_Damper:
				UOC_Input_State.Fire_Damper_flag = 0;
				if((UOC_Input_State.high_speed_fan ==1 || UOC_Input_State.Ordinary_fan == 1) && ((UOC_ERR_PARA[E08].time_out +4000) < GetSysTickCount()))
				{
					uoc_ERR_Flag_clean();
					UOCPara.new_state = F_ERR;
					UOC_ERR_PARA[E08].flag = 1;
				}
				break;
			case High_Speed_Fan_Dynamic_Start:
				break;
			case High_Speed_Fan_Dynamic_Stop:
				Output_Control(UOC_D02,CONTORL_HIGH);//联动控制关闭
				break;
			case Low_Speed_Fan_Dynamic_Start:
				break;
			case Smoke_Exhaust_Valve_Stop:
				UOC_Input_State.Smoke_Exhaust_Valve_Stop_flag = 1;
				break;
			case Smoke_Exhaust_Valve_Start:
				UOC_Input_State.Smoke_Exhaust_Valve_Start_flag = 1;
				//只是关闭风机，为了报警
				Output_Control(UOC_R01,CONTORL_HIGH);//高速风机关闭 //先上再延时1ms
				Output_Control(UOC_R00,CONTORL_HIGH);//高速风机关闭
				

				Output_Control(UOC_R02,CONTORL_HIGH);//普通风机关闭
				if(UOC_Input_State.high_speed_fan == 1)
				{
					UOC_Input_State.high_speed_fan = 0;
					UOC_Input_State.ready_high = 1;
					Control_High_Speed_Fan(OPEN);
				}
				else if(UOC_Input_State.Ordinary_fan == 1)
				{
					UOC_Input_State.Ordinary_fan = 0;
					UOC_Input_State.ready_Ordinary = 1;
					Control_Ordinary_Fan(OPEN);
				}
				break;
			default:
				break;
		}
	}
	return;
}


void Input_Function_Cycle(void)
{
	if(UOC_Input_State.mode_flag == 0)//手动
	{
		Control_R03R04R05R06_Function(UOC_AUTOMATIC_RUN,CLOSE);
		Control_R03R04R05R06_Function(UOC_MANUAL_RUN,OPEN);
		//debug_log("%s:%d: 手动\n",__func__,__LINE__);

		Output_Control(UOC_D05,CONTORL_HIGH);//自动指示灯关闭
	}
	else if(UOC_Input_State.mode_flag == 1)//自动
	{
		Control_R03R04R05R06_Function(UOC_AUTOMATIC_RUN,OPEN);
		Control_R03R04R05R06_Function(UOC_MANUAL_RUN,CLOSE);
		//debug_log("%s:%d: 自动\n",__func__,__LINE__);

		Output_Control(UOC_D05,CONTORL_LOW);//自动指示灯开启
	}
	return;
}

void UOC_DI_timemout_deal(void)
{
	#if 0
	unsigned char i=0;
	for(i=4;i<UOC_DIMAX;i++)
	{
		if(uoc_DI_Funtion[i].timeCount > keep_time+150)
		{
			uoc_DI_Funtion[i].timeflag = 0;
			uoc_DI_Funtion[i].timeCount = 0;
		}
	}
	#endif
	if((UOC_Check_Para.A_Cur <= 0.5) && (UOC_Check_Para.B_Cur <= 0.5) && (UOC_Check_Para.C_Cur <= 0.5))
	{
		if((UOC_Input_State.fan_flag ==1) && (UOC_Input_State.fan_timeCount > 1000))//低速转高速
		{
			UOC_Input_State.fan_timeCount = 0;
			UOC_Input_State.fan_flag = 0;
			if(UOC_Input_State.ready_high ==1)
			{
				debug_log("%s,%d: high fan start\n",__func__,__LINE__);
				Control_High_Speed_Fan(OPEN);
			}
		}
		if(UOC_Input_State.fan_flag == 0){
			if(UOC_Input_State.ready_high ==1)
			{
				debug_log("%s,%d: high fan start\n",__func__,__LINE__);
				Control_High_Speed_Fan(OPEN);
			}
			else if(UOC_Input_State.ready_Ordinary ==1)
			{
				debug_log("%s,%d: ordinary fan start\n",__func__,__LINE__);
				Control_Ordinary_Fan(OPEN);
			}
		}
	}
	if(UOC_Input_State.fan_flag == 3)
	{
		float stand_cur=0;
		if(UOC_Input_State.ready_Ordinary == 1)
		{
			stand_cur = (float)g_uoc_param.RatedCurrent2/10.0;
		}
		else if(UOC_Input_State.ready_high == 1)
		{
			stand_cur = (float)g_uoc_param.RatedCurrent1/10.0;
		}
		if((UOC_Input_State.fan_timeCount > 2000) ||((UOC_Check_Para.A_Cur > 0.5*stand_cur) && (UOC_Check_Para.B_Cur > 0.5*stand_cur) && (UOC_Check_Para.C_Cur > 0.5*stand_cur)))
		{
			UOC_Input_State.fan_timeCount = 0;
			UOC_Input_State.fan_flag = 0;
			if(UOC_Input_State.ready_high ==1)
			{
				UOC_Input_State.ready_high = 0;
				UOC_Input_State.high_speed_fan =1;
				debug_log("%s:%d: high_speed_fan\n",__func__,__LINE__);
			}
			if(UOC_Input_State.ready_Ordinary ==1)
			{
				UOC_Input_State.ready_Ordinary = 0;
				UOC_Input_State.Ordinary_fan =1;
				debug_log("%s:%d: Ordinary_fan\n",__func__,__LINE__);
			}
		}
	}
	return;
	
}

void UOC_DI0_FUNC(void)
{
	if(UOC_Input_State.mode_flag)
		return;
	if((DI0() == STATE_HIGH) && ((UOC_Input_State.Ordinary_fan == 1) ||(UOC_Input_State.ready_Ordinary == 1)))//低速风机停止
	{
		uoc_DI_Funtion[UOC_DI0].timeflag = 1;
		if(uoc_DI_Funtion[UOC_DI0].timeCount > (keep_time +(g_uoc_param.ButtonInputDelay*100)))
		{
			uoc_DI_Funtion[UOC_DI0].timeflag = 0;
			uoc_DI_Funtion[UOC_DI0].timeCount = 0;
			if(DI0() == STATE_HIGH)
			{
				UOC_Input_State.fan_timeCount = 0;//停止启动前所有预备动作
				UOC_Input_State.fan_flag = 0;
				UOC_Input_State.ready_Ordinary = 0;
				
				UOC_Input_State.Ordinary_fan = 0;
				Control_Ordinary_Fan(CLOSE);
				debug_log("%s:%d: UOC_DI0:0\n",__func__,__LINE__);
				Output_Control(UOC_D03,CONTORL_HIGH);//设备运行反馈关闭
				Output_Control(UOC_D02,CONTORL_HIGH);//联动灯关闭
				Control_R03R04R05R06_Function(UOC_CONTROLLED_RUN,CLOSE);
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
	if(UOC_Input_State.mode_flag)
		return;
	if((DI1() == STATE_HIGH) && (UOC_Input_State.high_speed_fan == 1 || UOC_Input_State.Ordinary_fan == 1 || UOC_Input_State.ready_Ordinary == 1 || UOC_Input_State.ready_high == 1))//高速风机停止
	{
		uoc_DI_Funtion[UOC_DI1].timeflag = 1;
		if(uoc_DI_Funtion[UOC_DI1].timeCount > (keep_time +(g_uoc_param.ButtonInputDelay*100)))
		{
			uoc_DI_Funtion[UOC_DI1].timeflag = 0;
			uoc_DI_Funtion[UOC_DI1].timeCount = 0;
			if(DI1() == STATE_HIGH)
			{
				UOC_Input_State.fan_timeCount = 0;//停止启动前所有预备动作
				UOC_Input_State.fan_flag = 0;
				UOC_Input_State.ready_Ordinary = 0;
				UOC_Input_State.ready_high = 0;
			
				UOC_Input_State.high_speed_fan = 0;
				Control_High_Speed_Fan(CLOSE);

				UOC_Input_State.Ordinary_fan = 0;//高速风机可以关闭普通风机
				Control_Ordinary_Fan(CLOSE);

				Output_Control(UOC_D03,CONTORL_HIGH);//设备运行反馈关闭
				Output_Control(UOC_D02,CONTORL_HIGH);//联动灯关闭
				Control_R03R04R05R06_Function(UOC_CONTROLLED_RUN,CLOSE);
				debug_log("%s:%d: UOC_DI0:0\n",__func__,__LINE__);
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
	if(UOC_Input_State.mode_flag || (UOC_Input_State.Auto_trip == 1))
		return;
	if(UOCPara.new_state == F_ERR || g_uoc_param.FanType == 1){
		return;
	}
	if(DI2() == STATE_LOW && UOC_Input_State.high_speed_fan == 0 && UOC_Input_State.Ordinary_fan == 0 && UOC_Input_State.ready_high == 0 && UOC_Input_State.ready_Ordinary == 0)//低速风机启动
	{
		uoc_DI_Funtion[UOC_DI2].timeflag = 1;
		if(uoc_DI_Funtion[UOC_DI2].timeCount > (keep_time +(g_uoc_param.ButtonInputDelay*100)))
		{
			uoc_DI_Funtion[UOC_DI2].timeflag = 0;
			uoc_DI_Funtion[UOC_DI2].timeCount = 0;
			if(DI2() == STATE_LOW)
			{
				debug_log("%s:%d: **Ordinary start!!!!!*******\n",__func__,__LINE__);
				UOC_Input_State.ready_Ordinary = 1;//准备开启普通风机
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
	if(UOC_Input_State.mode_flag || (UOC_Input_State.Auto_trip == 1))
		return;
	if(UOCPara.new_state == F_ERR)
		return;
	if(DI3() == STATE_LOW && UOC_Input_State.high_speed_fan == 0 && UOC_Input_State.ready_Ordinary == 0 && UOC_Input_State.ready_high == 0)//高速风机启动
	{
		uoc_DI_Funtion[UOC_DI3].timeflag = 1;
		if(uoc_DI_Funtion[UOC_DI3].timeCount > (keep_time +(g_uoc_param.ButtonInputDelay*100)))
		{
			uoc_DI_Funtion[UOC_DI3].timeflag = 0;
			uoc_DI_Funtion[UOC_DI3].timeCount = 0;
			if(DI3() == STATE_LOW)
			{
				if(UOC_Input_State.Ordinary_fan == 1)//低速风机开启状态中
				{
					UOC_Input_State.ready_high = 1;
					UOC_Input_State.Ordinary_fan = 0;
					Control_Ordinary_Fan(CLOSE);
					//需要延时
					debug_log("%s:%d: low fan ->high fan :0\n",__func__,__LINE__);
					UOC_Input_State.fan_flag = 1;//低速风机转高速风机标识
				}
				else//如果低速风机未开启则直接启动
				{
					debug_log("%s:%d: high fan start\n",__func__,__LINE__);
					UOC_Input_State.ready_high = 1;//准备开启高速风机
				}
				
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
	//debug_log("%s:%d: DI4:NO  flag%d\n",__func__,__LINE__,UOC_Input_State.mode_flag);
	if(DI4() == STATE_HIGH && UOC_Input_State.mode_flag == 1)//开启手动
	{
		debug_log("%s:%d: DI4:1 %d\n",__func__,__LINE__,uoc_DI_Funtion[UOC_DI4].timeCount);
		uoc_DI_Funtion[UOC_DI4].timeflag = 1;
		if(uoc_DI_Funtion[UOC_DI4].timeCount > keep_time)
		{
			uoc_DI_Funtion[UOC_DI4].timeflag = 0;
			uoc_DI_Funtion[UOC_DI4].timeCount = 0;
			if(DI4() == STATE_HIGH)
			{
				UOC_Input_State.mode_flag = 0;
				Control_R03R04R05R06_Function(UOC_AUTOMATIC_RUN,CLOSE);
				Control_R03R04R05R06_Function(UOC_MANUAL_RUN,OPEN);
				debug_log("%s:%d: UOC_DI0:0\n",__func__,__LINE__);

				Output_Control(UOC_D05,CONTORL_HIGH);//自动指示灯关闭
			}
		}
	}
	else if(DI4() == STATE_LOW && UOC_Input_State.mode_flag == 0)//开启自动
	{
		debug_log("%s:%d: DI4:0 %d\n",__func__,__LINE__,uoc_DI_Funtion[UOC_DI4].timeCount);
		uoc_DI_Funtion[UOC_DI4].timeflag = 1;
		if(uoc_DI_Funtion[UOC_DI4].timeCount > keep_time)
		{
			uoc_DI_Funtion[UOC_DI4].timeflag = 0;
			uoc_DI_Funtion[UOC_DI4].timeCount = 0;
			if(DI4() == STATE_LOW)
			{
				UOC_Input_State.mode_flag = 1;
				Control_R03R04R05R06_Function(UOC_AUTOMATIC_RUN,OPEN);
				Control_R03R04R05R06_Function(UOC_MANUAL_RUN,CLOSE);
				debug_log("%s:%d: UOC_DI0:1\n",__func__,__LINE__);

				Output_Control(UOC_D05,CONTORL_LOW);//自动指示灯开启
			}
		}
	}
}
void UOC_DI5_FUNC(void)
{
	if(DI5() == STATE_LOW && UOC_Input_State.high_speed_fan == 1)
	{
		uoc_DI_Funtion[UOC_DI5].timeflag = 1;
		if(uoc_DI_Funtion[UOC_DI5].timeCount > keep_time+140)
		{
			uoc_DI_Funtion[UOC_DI5].timeflag = 0;
			uoc_DI_Funtion[UOC_DI5].timeCount = 0;
			if(DI5() == STATE_LOW)
			{
				//高速正常
			}
		}
	}
	else if(DI5() == STATE_HIGH && UOC_Input_State.high_speed_fan == 1)
	{
		uoc_DI_Funtion[UOC_DI5].timeflag = 1;
		if(uoc_DI_Funtion[UOC_DI5].timeCount > keep_time+140)
		{
			uoc_DI_Funtion[UOC_DI5].timeflag = 0;
			uoc_DI_Funtion[UOC_DI5].timeCount = 0;
			if(DI5() == STATE_HIGH)
			{
				//高速异常
				debug_log("%s,%d: higt_fan err \n",__func__,__LINE__);
				uoc_ERR_Flag_clean();
				UOCPara.new_state = F_ERR;
				UOC_ERR_PARA[E07].flag = 1;
			}
		}
	}
	return;
}
void UOC_DI6_FUNC(void)
{
	if(DI6() == STATE_LOW && UOC_Input_State.Ordinary_fan == 1)
	{
		uoc_DI_Funtion[UOC_DI6].timeflag = 1;
		if(uoc_DI_Funtion[UOC_DI6].timeCount > keep_time+140)
		{
			uoc_DI_Funtion[UOC_DI6].timeflag = 0;
			uoc_DI_Funtion[UOC_DI6].timeCount = 0;
			if(DI6() == STATE_LOW)
			{
				//低速正常
			}
		}
	}
	else if(DI6() == STATE_HIGH && UOC_Input_State.Ordinary_fan == 1)
	{
		uoc_DI_Funtion[UOC_DI6].timeflag = 1;
		if(uoc_DI_Funtion[UOC_DI6].timeCount > keep_time+140)
		{
			uoc_DI_Funtion[UOC_DI6].timeflag = 0;
			uoc_DI_Funtion[UOC_DI6].timeCount = 0;
			if(DI6() == STATE_HIGH)
			{
				//低速异常
				uoc_ERR_Flag_clean();
				UOCPara.new_state = F_ERR;
				UOC_ERR_PARA[E07].flag = 1;
			}
		}
	}
	return;
}

void UOC_DI7_FUNC(void)
{
	static unsigned char button_flag=1;//按一下启动一下
	static unsigned char last_flag = 0;
	if(DI7() == STATE_LOW && (button_flag == 1 
		|| g_uoc_param.DI7InputSet == Controlled_Feedback
		|| g_uoc_param.DI7InputSet == Fire_Damper))
	{
		uoc_DI_Funtion[UOC_DI7].timeflag = 1;
		last_flag = 1;
		if(uoc_DI_Funtion[UOC_DI7].timeCount > (keep_time+ (g_uoc_param.DI7InputDelay*100)))
		{
			uoc_DI_Funtion[UOC_DI7].timeflag = 0;
			uoc_DI_Funtion[UOC_DI7].timeCount = 0;
			if(DI7() == STATE_LOW)
			{
				//正常
				button_flag = 0;
				Control_DI7_DI14_Function(g_uoc_param.DI7InputSet,OPEN);
			}
		}
		
	}
	else if((DI7() == STATE_HIGH) && last_flag == 1)
	{
		last_flag = 0;
		uoc_DI_Funtion[UOC_DI7].timeflag = 0;
		uoc_DI_Funtion[UOC_DI7].timeCount = 0;
	}
	if(DI7() == STATE_HIGH && (button_flag == 0 
		|| g_uoc_param.DI7InputSet == Controlled_Feedback
		|| g_uoc_param.DI7InputSet ==Fire_Damper))
	{
		uoc_DI_Funtion[UOC_DI7].timeflag = 1;
		last_flag = 2;
		if(uoc_DI_Funtion[UOC_DI7].timeCount > (keep_time+ (g_uoc_param.DI7InputDelay*100)))
		{
			uoc_DI_Funtion[UOC_DI7].timeflag = 0;
			uoc_DI_Funtion[UOC_DI7].timeCount = 0;
			if(DI7() == STATE_HIGH)
			{
				//异常
				button_flag = 1;
				Control_DI7_DI14_Function(g_uoc_param.DI7InputSet,CLOSE);
			}
		}
	}
	else if((DI7() == STATE_LOW) && last_flag == 2)
	{
		last_flag = 0;
		uoc_DI_Funtion[UOC_DI7].timeflag = 0;
		uoc_DI_Funtion[UOC_DI7].timeCount = 0;
	}
}
void UOC_DI8_FUNC(void)
{
	static unsigned char button_flag=1;
	static unsigned char last_flag = 0;
	if(DI8() == STATE_LOW && (button_flag == 1
		|| g_uoc_param.DI8InputSet == Controlled_Feedback
		|| g_uoc_param.DI8InputSet ==Fire_Damper))
	{
		uoc_DI_Funtion[UOC_DI8].timeflag = 1;
		last_flag = 1;
		if(uoc_DI_Funtion[UOC_DI8].timeCount > (keep_time+ (g_uoc_param.DI8InputDelay*100)))
		{
			uoc_DI_Funtion[UOC_DI8].timeflag = 0;
			uoc_DI_Funtion[UOC_DI8].timeCount = 0;
			if(DI8() == STATE_LOW)
			{
				//正常
				button_flag = 0;
				Control_DI7_DI14_Function(g_uoc_param.DI8InputSet,OPEN);
			}
		}
	}
	else if((DI8() == STATE_HIGH) && last_flag == 1)
	{
		last_flag = 0;
		uoc_DI_Funtion[UOC_DI8].timeflag = 0;
		uoc_DI_Funtion[UOC_DI8].timeCount = 0;
	}
	if(DI8() == STATE_HIGH && (button_flag == 0 
		|| g_uoc_param.DI8InputSet == Controlled_Feedback
		|| g_uoc_param.DI8InputSet ==Fire_Damper))
	{
		uoc_DI_Funtion[UOC_DI8].timeflag = 1;
		last_flag = 2;
		if(uoc_DI_Funtion[UOC_DI8].timeCount > (keep_time+ (g_uoc_param.DI8InputDelay*100)))
		{
			uoc_DI_Funtion[UOC_DI8].timeflag = 0;
			uoc_DI_Funtion[UOC_DI8].timeCount = 0;
			if(DI8() == STATE_HIGH)
			{
				//异常
				button_flag = 1;
				Control_DI7_DI14_Function(g_uoc_param.DI8InputSet,CLOSE);
			}
		}
	}
	else if((DI8() == STATE_LOW) && last_flag == 2)
	{
		last_flag = 0;
		uoc_DI_Funtion[UOC_DI8].timeflag = 0;
		uoc_DI_Funtion[UOC_DI8].timeCount = 0;
	}
}
void UOC_DI9_FUNC(void)
{
	static unsigned char button_flag=1;
	static unsigned char last_flag = 0;
	if(DI9() == STATE_LOW && (button_flag == 1 
		|| g_uoc_param.DI9InputSet == Controlled_Feedback
		|| g_uoc_param.DI9InputSet ==Fire_Damper))
	{
		uoc_DI_Funtion[UOC_DI9].timeflag = 1;
		last_flag = 1;
		if(uoc_DI_Funtion[UOC_DI9].timeCount > (keep_time+ (g_uoc_param.DI9InputDelay*100)))
		{
			uoc_DI_Funtion[UOC_DI9].timeflag = 0;
			uoc_DI_Funtion[UOC_DI9].timeCount = 0;
			if(DI9() == STATE_LOW)
			{
				//正常
				button_flag=0;
				Control_DI7_DI14_Function(g_uoc_param.DI9InputSet,OPEN);
			}
		}
	}
	else if((DI9() == STATE_HIGH) && last_flag == 1)
	{
		last_flag = 0;
		uoc_DI_Funtion[UOC_DI9].timeflag = 0;
		uoc_DI_Funtion[UOC_DI9].timeCount = 0;
	}
	if(DI9() == STATE_HIGH && (button_flag == 0 
		|| g_uoc_param.DI9InputSet == Controlled_Feedback
		|| g_uoc_param.DI9InputSet ==Fire_Damper))
	{
		uoc_DI_Funtion[UOC_DI9].timeflag = 1;
		last_flag = 2;
		if(uoc_DI_Funtion[UOC_DI9].timeCount > (keep_time+ (g_uoc_param.DI9InputDelay*100)))
		{
			uoc_DI_Funtion[UOC_DI9].timeflag = 0;
			uoc_DI_Funtion[UOC_DI9].timeCount = 0;
			if(DI9() == STATE_HIGH)
			{
				//异常
				button_flag=1;
				Control_DI7_DI14_Function(g_uoc_param.DI9InputSet,CLOSE);
			}
		}
	}
	else if((DI9() == STATE_LOW) && last_flag == 2)
	{
		last_flag = 0;
		uoc_DI_Funtion[UOC_DI9].timeflag = 0;
		uoc_DI_Funtion[UOC_DI9].timeCount = 0;
	}
}
void UOC_DI10_FUNC(void)
{
	static unsigned char button_flag=1;
	static unsigned char last_flag = 0;
	if(DI10() == STATE_LOW && (button_flag == 1 
		|| g_uoc_param.DI10InputSet == Controlled_Feedback
		|| g_uoc_param.DI10InputSet ==Fire_Damper))
	{
		uoc_DI_Funtion[UOC_DI10].timeflag = 1;
		 last_flag = 1;
		if(uoc_DI_Funtion[UOC_DI10].timeCount > (keep_time+ (g_uoc_param.DI10InputDelay*100)))
		{
			uoc_DI_Funtion[UOC_DI10].timeflag = 0;
			uoc_DI_Funtion[UOC_DI10].timeCount = 0;
			if(DI10() == STATE_LOW)
			{
				//正常
				button_flag = 0;
				Control_DI7_DI14_Function(g_uoc_param.DI10InputSet,OPEN);
			}
		}
	}
	else if((DI10() == STATE_HIGH) && last_flag == 1)
	{
		last_flag = 0;
		uoc_DI_Funtion[UOC_DI10].timeflag = 0;
		uoc_DI_Funtion[UOC_DI10].timeCount = 0;
	}
	if(DI10() == STATE_HIGH && (button_flag == 0 
		|| g_uoc_param.DI10InputSet == Controlled_Feedback
		|| g_uoc_param.DI10InputSet ==Fire_Damper))
	{
		uoc_DI_Funtion[UOC_DI10].timeflag = 1;
		 last_flag = 2;
		if(uoc_DI_Funtion[UOC_DI10].timeCount > (keep_time+ (g_uoc_param.DI10InputDelay*100)))
		{
			uoc_DI_Funtion[UOC_DI10].timeflag = 0;
			uoc_DI_Funtion[UOC_DI10].timeCount = 0;
			if(DI10() == STATE_HIGH)
			{
				//异常
				button_flag =1;
				Control_DI7_DI14_Function(g_uoc_param.DI10InputSet,CLOSE);
			}
		}
	}
	else if((DI10() == STATE_LOW) && last_flag == 2)
	{
		last_flag = 0;
		uoc_DI_Funtion[UOC_DI10].timeflag = 0;
		uoc_DI_Funtion[UOC_DI10].timeCount = 0;
	}
}
void UOC_DI11_FUNC(void)
{
	static unsigned char button_flag=1;
	static unsigned char last_flag = 0;
	if(DI11() == STATE_LOW && (button_flag == 1 
		|| g_uoc_param.DI11InputSet == Controlled_Feedback
		|| g_uoc_param.DI11InputSet ==Fire_Damper))
	{
		uoc_DI_Funtion[UOC_DI11].timeflag = 1;
		last_flag = 1;
		if(uoc_DI_Funtion[UOC_DI11].timeCount > (keep_time+ (g_uoc_param.DI11InputDelay*100)))
		{
			uoc_DI_Funtion[UOC_DI11].timeflag = 0;
			uoc_DI_Funtion[UOC_DI11].timeCount = 0;
			if(DI11() == STATE_LOW)
			{
				//正常
				button_flag =0;
				Control_DI7_DI14_Function(g_uoc_param.DI11InputSet,OPEN);
			}
		}
	}
	else if((DI11() == STATE_HIGH) && last_flag == 1)
	{
		last_flag = 0;
		uoc_DI_Funtion[UOC_DI11].timeflag = 0;
		uoc_DI_Funtion[UOC_DI11].timeCount = 0;
	}
	if(DI11() == STATE_HIGH && (button_flag == 0 
		|| g_uoc_param.DI11InputSet == Controlled_Feedback
		|| g_uoc_param.DI11InputSet ==Fire_Damper))
	{
		uoc_DI_Funtion[UOC_DI11].timeflag = 1;
		last_flag = 2;
		if(uoc_DI_Funtion[UOC_DI11].timeCount > (keep_time+ (g_uoc_param.DI11InputDelay*100)))
		{
			uoc_DI_Funtion[UOC_DI11].timeflag = 0;
			uoc_DI_Funtion[UOC_DI11].timeCount = 0;
			if(DI11() == STATE_HIGH)
			{
				//异常
				button_flag =1;
				Control_DI7_DI14_Function(g_uoc_param.DI11InputSet,CLOSE);
			}
		}
	}
	else if((DI11() == STATE_LOW) && last_flag == 2)
	{
		last_flag = 0;
		uoc_DI_Funtion[UOC_DI11].timeflag = 0;
		uoc_DI_Funtion[UOC_DI11].timeCount = 0;
	}
}
void UOC_DI12_FUNC(void)
{
	static unsigned char button_flag=1;
	static unsigned char last_flag = 0;
	if(DI12() == STATE_LOW && (button_flag == 1 
		|| g_uoc_param.DI12InputSet == Controlled_Feedback
		|| g_uoc_param.DI12InputSet ==Fire_Damper))
	{
		uoc_DI_Funtion[UOC_DI12].timeflag = 1;
		last_flag = 1;
		if(uoc_DI_Funtion[UOC_DI12].timeCount > (keep_time+ (g_uoc_param.DI12InputDelay*100)))
		{
			uoc_DI_Funtion[UOC_DI12].timeflag = 0;
			uoc_DI_Funtion[UOC_DI12].timeCount = 0;
			if(DI12() == STATE_LOW)
			{
				//正常
				button_flag = 0;
				Control_DI7_DI14_Function(g_uoc_param.DI12InputSet,OPEN);
			}
		}
	}
	else if((DI12() == STATE_HIGH) && last_flag == 1)
	{
		last_flag = 0;
		uoc_DI_Funtion[UOC_DI12].timeflag = 0;
		uoc_DI_Funtion[UOC_DI12].timeCount = 0;
	}
	if(DI12() == STATE_HIGH && (button_flag == 0 
		|| g_uoc_param.DI12InputSet == Controlled_Feedback
		|| g_uoc_param.DI12InputSet ==Fire_Damper))
	{
		uoc_DI_Funtion[UOC_DI12].timeflag = 1;
		last_flag = 2;
		if(uoc_DI_Funtion[UOC_DI12].timeCount > (keep_time+ (g_uoc_param.DI12InputDelay*100)))
		{
			uoc_DI_Funtion[UOC_DI12].timeflag = 0;
			uoc_DI_Funtion[UOC_DI12].timeCount = 0;
			if(DI12() == STATE_HIGH)
			{
				//异常
				button_flag =1;
				Control_DI7_DI14_Function(g_uoc_param.DI12InputSet,CLOSE);
			}
		}
	}
	else if((DI12() == STATE_LOW) && last_flag == 2)
	{
		last_flag = 0;
		uoc_DI_Funtion[UOC_DI12].timeflag = 0;
		uoc_DI_Funtion[UOC_DI12].timeCount = 0;
	}
}
void UOC_DI13_FUNC(void)
{
	static unsigned char button_flag=1;
	static unsigned char last_flag = 0;
	if(DI13() == STATE_LOW && (button_flag == 1 
		|| g_uoc_param.DI13InputSet == Controlled_Feedback
		|| g_uoc_param.DI13InputSet == Fire_Damper))
	{
		uoc_DI_Funtion[UOC_DI13].timeflag = 1;
		 last_flag = 1;
		if(uoc_DI_Funtion[UOC_DI13].timeCount > (keep_time+ (g_uoc_param.DI13InputDelay*100)))
		{
			uoc_DI_Funtion[UOC_DI13].timeflag = 0;
			uoc_DI_Funtion[UOC_DI13].timeCount = 0;
			if(DI13() == STATE_LOW)
			{
				//正常
				button_flag = 0;
				Control_DI7_DI14_Function(g_uoc_param.DI13InputSet,OPEN);
			}
			
		}
	}
	else if((DI13() == STATE_HIGH) && last_flag == 1)
	{
		last_flag = 0;
		uoc_DI_Funtion[UOC_DI13].timeflag = 0;
		uoc_DI_Funtion[UOC_DI13].timeCount = 0;
	}
	if(DI13() == STATE_HIGH && (button_flag == 0 
		|| g_uoc_param.DI13InputSet == Controlled_Feedback
		|| g_uoc_param.DI13InputSet == Fire_Damper))
	{
		uoc_DI_Funtion[UOC_DI13].timeflag = 1;
		 last_flag = 2;
		if(uoc_DI_Funtion[UOC_DI13].timeCount > (keep_time+ (g_uoc_param.DI13InputDelay*100)))
		{
			uoc_DI_Funtion[UOC_DI13].timeflag = 0;
			uoc_DI_Funtion[UOC_DI13].timeCount = 0;
			if(DI13() == STATE_HIGH)
			{
				//异常
				button_flag =1;
				Control_DI7_DI14_Function(g_uoc_param.DI13InputSet,CLOSE);
			}
		}
	}
	else if((DI13() == STATE_LOW) && last_flag == 2)
	{
		last_flag = 0;
		uoc_DI_Funtion[UOC_DI13].timeflag = 0;
		uoc_DI_Funtion[UOC_DI13].timeCount = 0;
	}
}
void UOC_DI14_FUNC(void)
{
	static unsigned char button_flag=0;
	static unsigned char last_flag = 0;
	if(DI14() == STATE_LOW && (button_flag == 1 || button_flag == 0
		|| g_uoc_param.DI14InputSet == Controlled_Feedback
		|| g_uoc_param.DI14InputSet == Fire_Damper))
	{
		uoc_DI_Funtion[UOC_DI14].timeflag = 1;
		last_flag = 1;
		if(uoc_DI_Funtion[UOC_DI14].timeCount > (keep_time+ (g_uoc_param.DI14InputDelay*100)))
		{
			uoc_DI_Funtion[UOC_DI14].timeflag = 0;
			uoc_DI_Funtion[UOC_DI14].timeCount = 0;
			if(DI14() == STATE_LOW)
			{
				//正常
				button_flag = 2;
				Control_DI7_DI14_Function(g_uoc_param.DI14InputSet,OPEN);
			}
		}
	}
	else if((DI14() == STATE_HIGH) && last_flag == 1)
	{
		last_flag = 0;
		uoc_DI_Funtion[UOC_DI14].timeflag = 0;
		uoc_DI_Funtion[UOC_DI14].timeCount = 0;
	}
	if(DI14() == STATE_HIGH && (button_flag == 2 || button_flag == 0
		|| g_uoc_param.DI14InputSet == Controlled_Feedback
		|| g_uoc_param.DI14InputSet == Fire_Damper))
	{
		uoc_DI_Funtion[UOC_DI14].timeflag = 1;
		last_flag = 2;
		if(uoc_DI_Funtion[UOC_DI14].timeCount > (keep_time+ (g_uoc_param.DI14InputDelay*100)))
		{
			uoc_DI_Funtion[UOC_DI14].timeflag = 0;
			uoc_DI_Funtion[UOC_DI14].timeCount = 0;
			if(DI14() == STATE_HIGH)
			{
				//异常
				button_flag = 1;
				Control_DI7_DI14_Function(g_uoc_param.DI14InputSet,CLOSE);
			}
		}
	}
	else if((DI14() == STATE_LOW) && last_flag == 2)
	{
		last_flag = 0;
		uoc_DI_Funtion[UOC_DI14].timeflag = 0;
		uoc_DI_Funtion[UOC_DI14].timeCount = 0;
	}
}

void Smoke_Check_Function_Cycle()
{
	if(UOC_Input_State.Smoke_Exhaust_Valve_Start_check_flag == 1){
		if(UOC_Input_State.Smoke_Exhaust_Valve_Start_flag == 0)
		{
			UOC_Input_State.Smoke_Exhaust_Valve_Start_check_flag = 0;
			UOC_Input_State.Smoke_timeCount = 0;

			Control_R03R04R05R06_Function(UOC_SMOKE_EXHAUST_VALVE_OPEN,CLOSE);
			UOC_Input_State.fan_flag = 3;
			UOC_Input_State.fan_timeCount = 0;
			if(UOC_Input_State.ready_high == 1)
			{
				Output_Control(UOC_R00,CONTORL_LOW);//高速风机开启
				Output_Control(UOC_R01,CONTORL_LOW);//高速风机开启
				
			}
			else if(UOC_Input_State.ready_Ordinary == 1)
			{
				Output_Control(UOC_R02,CONTORL_LOW);//普通风机开启
			}
		}
		else if(UOC_Input_State.Smoke_timeCount >(g_uoc_param.DischargeSmokeFeedbackTime * 1000)){
			UOC_Input_State.Smoke_Exhaust_Valve_Start_check_flag = 0;
			UOC_Input_State.Smoke_timeCount = 0;
			Control_R03R04R05R06_Function(UOC_SMOKE_EXHAUST_VALVE_OPEN,CLOSE);
			uoc_ERR_Flag_clean();
			UOCPara.new_state = F_ERR;
			UOC_ERR_PARA[E11].flag = 1;

			UOC_Input_State.static_clean();
			Control_R03R04R05R06_Function(UOC_HIGH_SPEED_RUN,CLOSE);//反馈
			Output_Control(UOC_D03,CONTORL_HIGH);//设备运行关闭
			Control_R03R04R05R06_Function(UOC_CONTROLLED_RUN,CLOSE);
			Output_Control(UOC_D07,CONTORL_HIGH);//高速风机指示灯关闭
			Output_Control(UOC_D08,CONTORL_HIGH);//普通风机指示灯关闭
		}
	}
	else if(UOC_Input_State.Smoke_Exhaust_Valve_Stop_check_flag == 1){
		if(UOC_Input_State.Smoke_Exhaust_Valve_Stop_flag == 0)
		{
			Control_R03R04R05R06_Function(UOC_SMOKE_EXHAUST_VALVE_CLOSE,CLOSE);
			UOC_Input_State.Smoke_Exhaust_Valve_Stop_check_flag = 1;
			UOC_Input_State.Smoke_timeCount = 0;
		}
		else if(UOC_Input_State.Smoke_timeCount >(g_uoc_param.DischargeSmokeOutputTime * 1000)){
			UOC_Input_State.Smoke_Exhaust_Valve_Stop_check_flag = 1;
			UOC_Input_State.Smoke_timeCount = 0;
			Control_R03R04R05R06_Function(UOC_SMOKE_EXHAUST_VALVE_CLOSE,CLOSE);
		}
	}
}
void UOC_Status_Output(void)
{
	if(UOCPara.new_state == F_ERR){
		Control_R03R04R05R06_Function(UOC_WORK_NORMALLY,CLOSE);
		Control_R03R04R05R06_Function(UOC_FAULT_FEEDBACK,OPEN);
	}
	else{
		Control_R03R04R05R06_Function(UOC_WORK_NORMALLY,OPEN);
		Control_R03R04R05R06_Function(UOC_FAULT_FEEDBACK,CLOSE);
	}
	if(UOC_Input_State.Ordinary_fan == 0){
		Control_R03R04R05R06_Function(UOC_LOW_SPEED_STOP,OPEN);
	}
	else{
		Control_R03R04R05R06_Function(UOC_LOW_SPEED_STOP,CLOSE);
	}
	if(UOC_Input_State.high_speed_fan == 0){
		Control_R03R04R05R06_Function(UOC_HIGH_SPEED_STOP,OPEN);
	}
	else{
		Control_R03R04R05R06_Function(UOC_HIGH_SPEED_STOP,CLOSE);
	}
	return;
}

void Input_Detection(void)
{
	unsigned char i = 0;
	for(i=0;i<UOC_DIMAX;i++)
	{
		uoc_DI_Funtion[i].cb();
	}	
	Input_Function_Cycle();
	UOC_DI_timemout_deal();
	Smoke_Check_Function_Cycle();
	UOC_Status_Output();
	return;
}
