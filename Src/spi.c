#include "spi.h"
#include "debug.h"
#include "parameter.h"
#include "system_control.h"
#include "led.h"
#include "i2c.h"
#include "button_function.h"


extern gUOCParameter_TypeDef g_uoc_param;
extern gUOCControl_StateDef UOC_Input_State;
extern gUOCParameter_Check_TypeDef UOC_Check_Para;
extern sParaFlag_TypeDef UOCPara;
extern eUOCDisplayErr_TypeDef UOC_ERR_PARA[EMAX];

sSPIData_TypeDef uoc_cyc_para[UOC_SPI_MAX_PARA]={
	{UOC_SPI_CHECK_PARA,0,20000,UOC_Spi_Check_Para},
	{UOC_SPI_READ_PARA,0,100,UOC_Spi_Read_Para},
	{UOC_SPI_DISPLAY_PARA,0,1000,UOC_Spi_Display_Para},
	{UOC_SPI_ERROR_PARA,0,200,UOC_Spi_Error_Para}
};

sSPIError_TypeDef UOC_SPI_Error_FUN[EMAX]={
	{E00,0,UOC_Spi_Error_E00},
	{E01,0,UOC_Spi_Error_E01},
	{E02,0,UOC_Spi_Error_E02},
	{E03,0,UOC_Spi_Error_E03},
	{E04,0,UOC_Spi_Error_E04},
	{E05,0,UOC_Spi_Error_E05},
	{E06,0,UOC_Spi_Error_E06},
	{E07,0,UOC_Spi_Error_E07},
	{E08,0,UOC_Spi_Error_E08},
	{E09,0,UOC_Spi_Error_E09},
	{E10,0,UOC_Spi_Error_E10},
	{E11,0,UOC_Spi_Error_E11},
	{E12,0,UOC_Spi_Error_E12}
};



uint32_t SPI_ReadWrite(uint32_t data)
{
	FL_SPI_SetSSNPin(SPI1, FL_SPI_SSN_LOW);

	FL_SPI_WriteTXBuff(SPI1, data);

	while(!(FL_SPI_IsActiveFlag_TXBuffEmpty(SPI1)));

	while(!(FL_SPI_IsActiveFlag_RXBuffFull(SPI1)));
	for(int i = 0; i< 100;i++)
	{}
	
	data = FL_SPI_ReadRXBuff(SPI1);

	FL_SPI_SetSSNPin(SPI1, FL_SPI_SSN_HIGH);
	return data;
}

void SPI_Write(uint8_t *data, uint16_t length)
{
	FL_SPI_SetSSNPin(SPI1, FL_SPI_SSN_LOW);

	while(length--)
	{
		FL_SPI_WriteTXBuff(SPI1, *data);
		data++;

		while(!(FL_SPI_IsActiveFlag_RXBuffFull(SPI1)));

		// Dummy read
		FL_SPI_ReadRXBuff(SPI1);
	}

	FL_SPI_SetSSNPin(SPI1, FL_SPI_SSN_HIGH);
}

void SPI_Read(uint8_t *data, uint16_t length)
{
	FL_SPI_SetSSNPin(SPI1, FL_SPI_SSN_LOW);

	while(length--)
	{
		// Dummy write
		FL_SPI_WriteTXBuff(SPI1, 0x0D);

		while(!(FL_SPI_IsActiveFlag_RXBuffFull(SPI1)));

		*data = FL_SPI_ReadRXBuff(SPI1);
		data++;
	}

	FL_SPI_SetSSNPin(SPI1, FL_SPI_SSN_HIGH);
}

uint32_t SPI_Read_Cmd(uint32_t cmd)
{
	uint32_t data_num;
	FL_SPI_SetSSNPin(SPI1, FL_SPI_SSN_LOW);

	// Dummy write
	FL_SPI_WriteTXBuff(SPI1, cmd);

	while(!(FL_SPI_IsActiveFlag_RXBuffFull(SPI1)));

	
	data_num = FL_SPI_ReadRXBuff(SPI1);
	//dbg_printf("\t\t\t data_num	: %08x \n",data_num);

	FL_SPI_SetSSNPin(SPI1, FL_SPI_SSN_HIGH);
	return data_num;
}


void SPI_Test(void)
{
	//uint8_t wbuf[8] = {0x0D, 0x00, 0x00, 0x00, 0x55, 0x66, 0xAA, 0xBB};
	//uint8_t rbuf[4] = {0};
	uint32_t data_num = 0;

	/*A voltage data read*///data_num = data_num & 0x0007FFFF;
	
	

	// Read and write
	//rbuf[0] = SPI_ReadWrite(rbuf[0]);
	
	// Write buffer
	//SPI_Write(wbuf, 1);
	
	// Read to buffer
	//SPI_Read(rbuf,4);
	//dbg_array_buffer("spi before:",rbuf,4);
	//data_num = rbuf[0] + rbuf[1] << 8 + rbuf[2] << 16 + rbuf[3] << 24;
	data_num = SPI_Read_data(UOC_SPI_PHASE_A_VOLTAGE);
	debug_log("%s:%d: UOC_SPI_PHASE_A_VOLTAGE:%x\n",__func__,__LINE__,data_num);
	data_num = (int)(data_num * UOC_Voltage_Offsets);
	data_num = (int)((data_num * g_uoc_param.VolCorParam)/100);
	debug_log("%s:%d: Avoltage:%d\n",__func__,__LINE__,data_num);
	UOC_Check_Para.A_Vol = (int)((data_num%10000000)/10000);

}

uint32_t SPI_Read_data(uint32_t cmd)
{
	uint32_t read_data= 0;
	// Read and write
	//SPI_ReadWrite(cmd);

	// Read to buffer
	read_data =SPI_Read_Cmd(cmd);
	//dbg_array_buffer("spi",rbuf,8);
	return read_data;
}

void SPI_Mode_init(void)
{
		// Read and write
	SPI_ReadWrite(UOC_SPI_WRITE_CHECK_EN);//c9 00005a

	SPI_ReadWrite(UOC_SPI_SET_MODE);//0X81	B97e

	
	SPI_ReadWrite(UOC_SPI_SET_EMU_EN);//0X83 F804	   //start Energy counting

	
	SPI_ReadWrite(UOC_SPI_SET_BOR_EN);//0XB1 3427	//start Power supply detection
 
	SPI_ReadWrite(UOC_SPI_WRITE_CHECK_DIS);//c9 000055
}


int SPI_Check(void)
{
	uint32_t read_data= 0;

	SPI_ReadWrite(UOC_SPI_READ_CHECK_EN);//c6 0000005A
	read_data=SPI_Read_data(0X01000000);
	//debug_log("%s:%d:0x01 data:%08x\n",__func__,__LINE__,read_data);
	if(read_data != (UOC_SPI_SET_MODE &0x0000ffff))
		return -1;
	
	read_data=SPI_Read_data(0X03000000);
	//debug_log("%s:%d:0x03 data:%08x\n",__func__,__LINE__,read_data);
	if(read_data != (UOC_SPI_SET_EMU_EN &0x0000ffff))
		return -1;
	read_data=SPI_Read_data(0X31000000);
	//debug_log("%s:%d:0x31 data:%08x\n",__func__,__LINE__,read_data);
	if(read_data != (UOC_SPI_SET_BOR_EN &0x0000ffff))
		return -1;
	
	SPI_ReadWrite(UOC_SPI_READ_CHECK_DIS);//c6 00000055
	// Read to buffer
	SPI_ReadWrite(0X00);
	read_data =SPI_Read_Cmd(0x00000000);
	//dbg_array_buffer("spi",rbuf,8);
	return 0;
}

void UOC_Spi_Check_Para(void)
{
	if(SPI_Check())
			SPI_Mode_init();
}
void UOC_Spi_Read_Para(void)
{
	uint32_t data_num = 0;

	/*A voltage data read*///data_num = data_num & 0x0007FFFF;
	data_num = SPI_Read_data(UOC_SPI_PHASE_A_VOLTAGE);
	data_num = (int)(data_num * UOC_Voltage_Offsets);
	data_num = (int)((data_num * g_uoc_param.VolCorParam)/100);
	//debug_log("%s:%d: Avoltage:%d\n",__func__,__LINE__,data_num);
	UOC_Check_Para.A_Vol = (int)((data_num%10000000)/10000);

	/*B voltage data read*/
	data_num = SPI_Read_data(UOC_SPI_PHASE_B_VOLTAGE);
	data_num = (int)(data_num * UOC_Voltage_Offsets);
	data_num = (int)((data_num * g_uoc_param.VolCorParam)/100);
	//debug_log("%s:%d: Bvoltage:%d\n",__func__,__LINE__,data_num);
	UOC_Check_Para.B_Vol = (int)((data_num%10000000)/10000);

	/*C voltage data read*/
	data_num = SPI_Read_data(UOC_SPI_PHASE_C_VOLTAGE);
	data_num = (int)(data_num * UOC_Voltage_Offsets);
	data_num = (int)((data_num * g_uoc_param.VolCorParam)/100);
	//debug_log("%s:%d: Cvoltage:%d\n",__func__,__LINE__,data_num);
	UOC_Check_Para.C_Vol = (int)((data_num%10000000)/10000);

	/*A currents data read*/
	data_num = SPI_Read_data(UOC_SPI_PHASE_A_CURRENTS);
	data_num = (int)(data_num * UOC_Currents_Offsets * g_uoc_param.CurCorParam / 100.0 * g_uoc_param.CurTransRatedCurrent/1000.0);
	if(UOC_Input_State.high_speed_fan == 1)
		data_num = data_num / g_uoc_param.CurTransPierceTimes1;//高速
	else if(UOC_Input_State.Ordinary_fan == 1)
		data_num = data_num / g_uoc_param.CurTransPierceTimes2;//低速
	//debug_log("%s:%d: Acurrents:%d\n",__func__,__LINE__,data_num);
	UOC_Check_Para.A_Cur = (float)(((data_num%10000000)/10000)/10.0);

	/*B currents data read*/
	data_num = SPI_Read_data(UOC_SPI_PHASE_B_CURRENTS);	
	data_num = (int)(data_num * UOC_Currents_Offsets * g_uoc_param.CurCorParam / 100.0 * g_uoc_param.CurTransRatedCurrent/1000.0);
	if(UOC_Input_State.high_speed_fan == 1)
		data_num = data_num / g_uoc_param.CurTransPierceTimes1;//高速
	else if(UOC_Input_State.Ordinary_fan == 1)
		data_num = data_num / g_uoc_param.CurTransPierceTimes2;//低速
	//debug_log("%s:%d: Bcurrents:%d\n",__func__,__LINE__,data_num);
	UOC_Check_Para.B_Cur = (float)(((data_num%10000000)/10000)/10.0);

	/*C currents data read*/
	data_num = SPI_Read_data(UOC_SPI_PHASE_C_CURRENTS);
	//debug_log("%s:%d: Ccurrents:%08x\n",__func__,__LINE__,data_num);
	data_num = (int)(data_num * UOC_Currents_Offsets * g_uoc_param.CurCorParam / 100.0 * g_uoc_param.CurTransRatedCurrent/1000.0);
	if(UOC_Input_State.high_speed_fan == 1)
		data_num = data_num / g_uoc_param.CurTransPierceTimes1;//高速
	else if(UOC_Input_State.Ordinary_fan == 1)
		data_num = data_num / g_uoc_param.CurTransPierceTimes2;//低速
	//debug_log("%s:%d: Ccurrents:%d\n",__func__,__LINE__,data_num);
	UOC_Check_Para.C_Cur = (float)(((data_num%10000000)/10000)/10.0);

	/*A phase data read*/
	data_num = SPI_Read_data(UOC_SPI_PHASE_A_PHASE);
	
	data_num = data_num & 0x001FFFFF;
	data_num = (data_num * 180)/(1024*1024);
	//debug_log("%s:%d: a-bPHASE:%d\n",__func__,__LINE__,data_num);
	UOC_Check_Para.A_Phase = (data_num%1000);

	/*B phase data read*/
	data_num = SPI_Read_data(UOC_SPI_PHASE_B_PHASE);
	data_num = data_num & 0x001FFFFF;
	data_num = (data_num * 180)/(1024*1024);
	//debug_log("%s:%d: a-cPHASE:%d\n",__func__,__LINE__,data_num);
	UOC_Check_Para.B_Phase = (data_num%1000);

	/*C phase data read*/
	data_num = SPI_Read_data(UOC_SPI_PHASE_C_PHASE);
	data_num = data_num & 0x001FFFFF;
	data_num = (data_num * 180)/(1024*1024);
	//debug_log("%s:%d: B-cPHASE:%d\n",__func__,__LINE__,data_num);
	UOC_Check_Para.C_Phase = (data_num%1000);

	//debug_info("A:%d,\t,B:%d,\t,C:%d,\n",UOC_Check_Para.A_Phase,UOC_Check_Para.B_Phase,UOC_Check_Para.C_Phase);
	return;
	
}
void UOC_Spi_Display_Para(void)
{
#if 0
	static unsigned char read_flag=0;
	read_flag++;
	I2C_Display(DISPLAY1,DISPLAY_NO_POINT,DISPLAY_A_PHASE);
	I2C_Display(DISPLAY2,DISPLAY_NO_POINT,(read_flag%1000)/100);
	I2C_Display(DISPLAY3,DISPLAY_NO_POINT,(read_flag%100)/10);
	I2C_Display(DISPLAY4,DISPLAY_NO_POINT,read_flag%10);
#else
	static unsigned char read_flag=5;//使用最大值，让第一次从初始值开始计算
	if(!UOCPara.spi_read_flag){
		read_flag++;
		if(read_flag > 5)
			read_flag = 0;
	}
	switch (read_flag)
	{
		case 0:
			led_display(UOC_LED_NV,LED_ON);
			led_display(UOC_LED_NA,LED_OFF);

			if(UOCPara.spi_read_flag){
				I2C_Display(DISPLAY1,DISPLAY_POINT,DISPLAY_A_PHASE);
			}
			else{
				I2C_Display(DISPLAY1,DISPLAY_NO_POINT,DISPLAY_A_PHASE);
			}
			I2C_Display(DISPLAY2,DISPLAY_NO_POINT,(UOC_Check_Para.A_Vol%1000)/100);
			I2C_Display(DISPLAY3,DISPLAY_NO_POINT,(UOC_Check_Para.A_Vol%100)/10);
			I2C_Display(DISPLAY4,DISPLAY_NO_POINT,UOC_Check_Para.A_Vol%10);
		break;
		case 1:
			led_display(UOC_LED_NV,LED_ON);
			led_display(UOC_LED_NA,LED_OFF);

			if(UOCPara.spi_read_flag){
				I2C_Display(DISPLAY1,DISPLAY_POINT,DISPLAY_B_PHASE);
			}
			else{
				I2C_Display(DISPLAY1,DISPLAY_NO_POINT,DISPLAY_B_PHASE);
			}
			I2C_Display(DISPLAY2,DISPLAY_NO_POINT,(UOC_Check_Para.B_Vol%1000)/100);
			I2C_Display(DISPLAY3,DISPLAY_NO_POINT,(UOC_Check_Para.B_Vol%100)/10);
			I2C_Display(DISPLAY4,DISPLAY_NO_POINT,UOC_Check_Para.B_Vol%10);
		break;
		case 2:
			led_display(UOC_LED_NV,LED_ON);
			led_display(UOC_LED_NA,LED_OFF);
			
			if(UOCPara.spi_read_flag){
				I2C_Display(DISPLAY1,DISPLAY_POINT,DISPLAY_C_PHASE);
			}
			else{
				I2C_Display(DISPLAY1,DISPLAY_NO_POINT,DISPLAY_C_PHASE);
			}
			I2C_Display(DISPLAY2,DISPLAY_NO_POINT,(UOC_Check_Para.C_Vol%1000)/100);
			I2C_Display(DISPLAY3,DISPLAY_NO_POINT,(UOC_Check_Para.C_Vol%100)/10);
			I2C_Display(DISPLAY4,DISPLAY_NO_POINT,UOC_Check_Para.C_Vol%10);
		break;
		case 3:
			led_display(UOC_LED_NV,LED_OFF);
			led_display(UOC_LED_NA,LED_ON);
			
			if(UOCPara.spi_read_flag){
				I2C_Display(DISPLAY1,DISPLAY_POINT,DISPLAY_A_PHASE);
			}
			else{
				I2C_Display(DISPLAY1,DISPLAY_NO_POINT,DISPLAY_A_PHASE);
			}
			I2C_Display(DISPLAY2,DISPLAY_NO_POINT,(((int)(UOC_Check_Para.A_Cur*10))%1000)/100);
			I2C_Display(DISPLAY3,DISPLAY_POINT,(((int)(UOC_Check_Para.A_Cur*10))%100)/10);
			I2C_Display(DISPLAY4,DISPLAY_NO_POINT,((int)(UOC_Check_Para.A_Cur*10))%10);
		break;
		case 4:
			led_display(UOC_LED_NV,LED_OFF);
			led_display(UOC_LED_NA,LED_ON);
			
			if(UOCPara.spi_read_flag){
				I2C_Display(DISPLAY1,DISPLAY_POINT,DISPLAY_B_PHASE);
			}
			else{
				I2C_Display(DISPLAY1,DISPLAY_NO_POINT,DISPLAY_B_PHASE);
			}
			I2C_Display(DISPLAY2,DISPLAY_NO_POINT,(((int)(UOC_Check_Para.B_Cur*10))%1000)/100);
			I2C_Display(DISPLAY3,DISPLAY_POINT,(((int)(UOC_Check_Para.B_Cur*10))%100)/10);
			I2C_Display(DISPLAY4,DISPLAY_NO_POINT,((int)(UOC_Check_Para.B_Cur*10))%10);

		break;
		case 5:
			led_display(UOC_LED_NV,LED_OFF);
			led_display(UOC_LED_NA,LED_ON);
			
			if(UOCPara.spi_read_flag){
				I2C_Display(DISPLAY1,DISPLAY_POINT,DISPLAY_C_PHASE);
			}
			else{
				I2C_Display(DISPLAY1,DISPLAY_NO_POINT,DISPLAY_C_PHASE);
			}
			I2C_Display(DISPLAY2,DISPLAY_NO_POINT,(((int)(UOC_Check_Para.C_Cur*10))%1000)/100);
			I2C_Display(DISPLAY3,DISPLAY_POINT,(((int)(UOC_Check_Para.C_Cur*10))%100)/10);
			I2C_Display(DISPLAY4,DISPLAY_NO_POINT,((int)(UOC_Check_Para.C_Cur*10))%10);

		break;
		default:
			break;
	
	}
	return;
#endif
}

void UOC_Spi_Error_E00(void)
{
	if(UOC_ERR_PARA[E00].flag == 0){
		if((UOC_Check_Para.A_Vol > g_uoc_param.VolOverMax) && (UOC_Check_Para.B_Vol > g_uoc_param.VolOverMax) && (UOC_Check_Para.C_Vol > g_uoc_param.VolOverMax))
		{
			
			if(UOC_ERR_PARA[E00].fisrt_flag == 0)
			{
				UOC_ERR_PARA[E00].fisrt_flag = 1;
				UOC_ERR_PARA[E00].time_out = GetSysTickCount();
			}
			else if((UOC_ERR_PARA[E00].time_out + (g_uoc_param.VolOverTime *1000 ))< GetSysTickCount())
			{
				uoc_ERR_Flag_clean();
				UOC_Check_Para.Vol_err = UOC_Check_Para.B_Vol;
				UOCPara.new_state = F_ERR;
				UOC_ERR_PARA[E00].flag = 1;
				UOC_ERR_PARA[E00].fisrt_flag = 0;
				UOC_Input_State.Vol_Normal_flag = 0;
			}
		}
		else
		{
			UOC_ERR_PARA[E00].fisrt_flag = 0;
		}
	}
	else if(UOC_ERR_PARA[E00].flag == 1)
	{
		if((UOC_Check_Para.A_Vol < g_uoc_param.VolOverMax) && (UOC_Check_Para.B_Vol < g_uoc_param.VolOverMax) && (UOC_Check_Para.C_Vol < g_uoc_param.VolOverMax))
		{
			
			if(UOC_ERR_PARA[E00].fisrt_flag == 0)
			{
				UOC_ERR_PARA[E00].fisrt_flag = 1;
				UOC_ERR_PARA[E00].time_out = GetSysTickCount();
			}
			else if((UOC_ERR_PARA[E00].time_out + 3000 )< GetSysTickCount())
			{
				UOC_Input_State.Vol_Normal_flag = 1;
				UOC_ERR_PARA[E00].fisrt_flag = 0;
			}
		}
		else
		{
			UOC_ERR_PARA[E00].fisrt_flag = 0;
		}
	}
	return;
}
void UOC_Spi_Error_E01(void)
{
	if(UOC_ERR_PARA[E01].flag == 0){
		if((UOC_Check_Para.A_Vol < g_uoc_param.VolUnderMax) && (UOC_Check_Para.B_Vol < g_uoc_param.VolUnderMax) && (UOC_Check_Para.C_Vol < g_uoc_param.VolUnderMax))
		{
			if(UOC_ERR_PARA[E01].fisrt_flag == 0)
			{
				UOC_ERR_PARA[E01].fisrt_flag = 1;
				UOC_ERR_PARA[E01].time_out = GetSysTickCount();
			}
			else if((UOC_ERR_PARA[E01].time_out + (g_uoc_param.VolUnderTime *1000 ))< GetSysTickCount())
			{
				uoc_ERR_Flag_clean();
				UOC_Input_State.Vol_Normal_flag = 0;
				UOC_Check_Para.Vol_err = UOC_Check_Para.B_Vol;
				UOCPara.new_state = F_ERR;
				UOC_ERR_PARA[E01].flag = 1;
			}
		}
		else
		{
			UOC_ERR_PARA[E01].fisrt_flag = 0;
		}
	}
	else if(UOC_ERR_PARA[E01].flag == 1)
	{
		if((UOC_Check_Para.A_Vol > g_uoc_param.VolUnderMax) && (UOC_Check_Para.B_Vol > g_uoc_param.VolUnderMax) && (UOC_Check_Para.C_Vol > g_uoc_param.VolUnderMax))
		{
			if(UOC_ERR_PARA[E01].fisrt_flag == 0)
			{
				UOC_ERR_PARA[E01].fisrt_flag = 1;
				UOC_ERR_PARA[E01].time_out = GetSysTickCount();
			}
			else if((UOC_ERR_PARA[E01].time_out + 3000 )< GetSysTickCount())
			{
				UOC_Input_State.Vol_Normal_flag = 1;
				UOC_ERR_PARA[E01].fisrt_flag = 0;
			}
		}
		else
		{
			UOC_ERR_PARA[E01].fisrt_flag = 0;
		}
	}
	return;
}
void UOC_Spi_Error_E02(void)
{
	float stand_cur=0;
	if((UOC_Input_State.Ordinary_fan == 0) && (UOC_Input_State.high_speed_fan ==0)){
		Control_R03R04R05R06_Function(UOC_FAN_OVERLOAD,CLOSE);
		UOC_ERR_PARA[E02].fisrt_flag = 0;
		UOC_ERR_PARA[E02].time_out = GetSysTickCount();
		return;
	}
	if(UOC_Input_State.Ordinary_fan == 1)
	{
		stand_cur = (float)g_uoc_param.RatedCurrent2;
	}
	else if(UOC_Input_State.high_speed_fan == 1)
	{
		stand_cur = (float)g_uoc_param.RatedCurrent1;
	}

	if(UOC_ERR_PARA[E02].flag == 0)
	{
		if(((UOC_Check_Para.A_Cur * 1000) > ((stand_cur*115))) && ((UOC_Check_Para.B_Cur*1000) > ((stand_cur*115))) && ((UOC_Check_Para.C_Cur*1000) > ((stand_cur*115))))
		{
			if(UOC_ERR_PARA[E02].fisrt_flag == 0)
			{
				UOC_ERR_PARA[E02].fisrt_flag = 1;
				UOC_ERR_PARA[E02].time_out = GetSysTickCount();
				dbg_printf("e02 fisrt join\n");
			}
			else if((UOC_ERR_PARA[E02].time_out + (g_uoc_param.CurOverLongDelayTime *60000 ))< GetSysTickCount())
			{
				uoc_ERR_Flag_clean();
				UOCPara.new_state = F_ERR;
				UOC_Check_Para.Cur_err = UOC_Check_Para.C_Cur;
				UOC_ERR_PARA[E02].flag = 1;
				UOC_ERR_PARA[E03].fisrt_flag = 0;
				UOC_ERR_PARA[E02].fisrt_flag = 0;
				debug_log("%s:%d:close\n",__func__,__LINE__);
				dbg_printf("e02 error ok\n");
				Control_R03R04R05R06_Function(UOC_FAN_OVERLOAD,CLOSE);
				if(g_uoc_param.CurOverLongDelayFlag)//长延时使能
				{
					
				}
			}
			if((UOC_ERR_PARA[E02].fisrt_flag == 1) && ((UOC_ERR_PARA[E02].time_out +1000) < GetSysTickCount()))
			{
				debug_log("%s:%d:open\n",__func__,__LINE__);
				Control_R03R04R05R06_Function(UOC_FAN_OVERLOAD,OPEN);
			}
		}
		else
		{
			debug_log("%s:%d:close\n",__func__,__LINE__);
			Control_R03R04R05R06_Function(UOC_FAN_OVERLOAD,CLOSE);
			UOC_ERR_PARA[E02].fisrt_flag = 0;
			UOC_ERR_PARA[E02].time_out = GetSysTickCount();
		}
	}
	return;
}
void UOC_Spi_Error_E03(void)
{
	float stand_cur=0;
	if((UOC_Input_State.Ordinary_fan == 0) && (UOC_Input_State.high_speed_fan ==0)){
		Control_R03R04R05R06_Function(UOC_FAN_OVERLOAD,CLOSE);
		Output_Control(UOC_D00,CONTORL_HIGH);//测试
		UOC_ERR_PARA[E03].fisrt_flag = 0;
		UOC_ERR_PARA[E03].time_out = GetSysTickCount();
		return;
	}
	if(UOC_Input_State.Ordinary_fan == 1)
	{
		stand_cur = (float)g_uoc_param.RatedCurrent2;
		//stand_cur = (int)g_uoc_param.RatedCurrent2;
	}
	else if(UOC_Input_State.high_speed_fan == 1)
	{
		stand_cur = (float)g_uoc_param.RatedCurrent1;
		//stand_cur = (int)g_uoc_param.RatedCurrent1;
	}
	if(UOC_ERR_PARA[E03].flag == 0)
	{
		if(((UOC_Check_Para.A_Cur * 100) > ((stand_cur*15))) && 
			((UOC_Check_Para.B_Cur * 100) > ((stand_cur*15))) && 
				((UOC_Check_Para.C_Cur * 100) > ((stand_cur*15))))
		{
			Output_Control(UOC_D00,CONTORL_LOW);//测试
			if(UOC_ERR_PARA[E03].fisrt_flag == 0)
			{
				UOC_ERR_PARA[E03].fisrt_flag = 1;
				UOC_ERR_PARA[E03].time_out = GetSysTickCount();
			}
			else if((UOC_ERR_PARA[E03].time_out + (g_uoc_param.CurOverShortDelayTime *1000 ))< GetSysTickCount())
			{
				uoc_ERR_Flag_clean();
				UOCPara.new_state = F_ERR;
				UOC_Check_Para.Cur_err = UOC_Check_Para.C_Cur;
				UOC_ERR_PARA[E03].flag = 1;
				UOC_ERR_PARA[E02].fisrt_flag = 0;
				UOC_ERR_PARA[E03].fisrt_flag = 0;
				debug_log("%s:%d:close\n",__func__,__LINE__);
				Control_R03R04R05R06_Function(UOC_FAN_OVERLOAD,CLOSE);
				if(g_uoc_param.CurOverShortDelayFlag)//短延时使能
				{
					
				}
			}
			if((UOC_ERR_PARA[E03].fisrt_flag == 1) && ((UOC_ERR_PARA[E03].time_out +1000) < GetSysTickCount()))
			{
				debug_log("%s:%d:open\n",__func__,__LINE__);
				Control_R03R04R05R06_Function(UOC_FAN_OVERLOAD,OPEN);
			}
		}
		else
		{
			//if(UOC_ERR_PARA[E02].fisrt_flag != 1)
			//	Control_R03R04R05R06_Function(UOC_FAN_OVERLOAD,CLOSE);
			UOC_ERR_PARA[E03].fisrt_flag = 0;
			UOC_ERR_PARA[E03].time_out = GetSysTickCount();
		}
	}
	return;
}

void UOC_Spi_Error_E04(void)
{
	float stand_cur=0;
	if(UOC_Input_State.Ordinary_fan == 1)
	{
		stand_cur = (float)g_uoc_param.RatedCurrent2/10.0;
	}
	else if(UOC_Input_State.high_speed_fan == 1)
	{
		stand_cur = (float)g_uoc_param.RatedCurrent1/10.0;
	}
	if((UOC_Input_State.Ordinary_fan == 1) || (UOC_Input_State.high_speed_fan == 1))
	{
		if(UOC_ERR_PARA[E04].flag == 0)
		{
			if((UOC_Check_Para.A_Cur > (float)(stand_cur*g_uoc_param.ShortCircuitMax)) || (UOC_Check_Para.B_Cur > (float)(stand_cur*g_uoc_param.ShortCircuitMax)) || (UOC_Check_Para.C_Cur > (float)(stand_cur*g_uoc_param.ShortCircuitMax)))
			{
				stand_cur = (unsigned short)(stand_cur*g_uoc_param.ShortCircuitMax);
				if(UOC_ERR_PARA[E04].fisrt_flag == 0)
				{
					UOC_ERR_PARA[E04].fisrt_flag = 1;
					UOC_ERR_PARA[E04].time_out = GetSysTickCount();
				}
				else if(UOC_ERR_PARA[E04].time_out + (g_uoc_param.ShortCircuitDelay *100 )< GetSysTickCount())
				{
					uoc_ERR_Flag_clean();
					UOCPara.new_state = F_ERR;
					UOC_ERR_PARA[E04].flag = 1;
					Control_R03R04R05R06_Function(UOC_FAN_OVERLOAD,CLOSE);
					if(g_uoc_param.MotorShortCircuitFlag)//短延时使能
					{
						
					}
				}
			}
			else
			{
				UOC_ERR_PARA[E04].fisrt_flag = 0;
			}
		}
	}
	return;
}

void UOC_Spi_Error_E05(void)
{
	//return;//600 //480 120 240 120
	uint32_t check_max = 0;
	uint32_t check_min = 0;
	//uint32_t Phase_num =0;
	//Phase_num = UOC_Check_Para.A_Phase + UOC_Check_Para.B_Phase + UOC_Check_Para.C_Phase;

	if(UOC_Check_Para.A_Vol > UOC_Check_Para.B_Vol){
			check_max = UOC_Check_Para.A_Vol;
			check_min = UOC_Check_Para.B_Vol;
	}
	else{
		check_max = UOC_Check_Para.B_Vol;
		check_min = UOC_Check_Para.A_Vol;
	}
	if(check_max < UOC_Check_Para.C_Vol){
		check_max = UOC_Check_Para.C_Vol;
	}
	if(check_min > UOC_Check_Para.C_Vol){
		check_min = UOC_Check_Para.C_Vol;
	}
	if((check_max*10) > (check_min * 12))//判断条件
	{
		return;
	
}
	if(UOC_ERR_PARA[E05].flag == 0){
		if(((g_uoc_param.WrongPhaseFlag == 2) 
			&& ((UOC_Check_Para.A_Phase < 135) && (UOC_Check_Para.A_Phase > 105))
			&& ((UOC_Check_Para.B_Phase < 255) && (UOC_Check_Para.B_Phase > 225))
			&& ((UOC_Check_Para.C_Phase < 135) && (UOC_Check_Para.C_Phase > 105))) 
		||((g_uoc_param.WrongPhaseFlag == 1) 
			&& ((UOC_Check_Para.A_Phase < 255) && (UOC_Check_Para.A_Phase > 225))
			&& ((UOC_Check_Para.B_Phase < 135) && (UOC_Check_Para.B_Phase > 105))
			&& ((UOC_Check_Para.C_Phase < 255) && (UOC_Check_Para.C_Phase > 225))))
		//if(((g_uoc_param.Phase_num-10) > Phase_num) || ((g_uoc_param.Phase_num+10) < Phase_num))//判断条件
		{
			
			if(UOC_ERR_PARA[E05].fisrt_flag == 0)
			{
				UOC_ERR_PARA[E05].fisrt_flag = 1;
				UOC_ERR_PARA[E05].time_out = GetSysTickCount();
			}
			else if((UOC_ERR_PARA[E05].time_out + (g_uoc_param.WrongPhaseTime *100 ))< GetSysTickCount())
			{
				uoc_ERR_Flag_clean();
				UOCPara.new_state = F_ERR;
				dbg_printf("e05 set Phase_num:%d\n",g_uoc_param.Phase_num);
				dbg_printf("e05 A_Phase:%d\n",UOC_Check_Para.A_Phase);
				dbg_printf("e05 B_Phase:%d\n",UOC_Check_Para.B_Phase);
				dbg_printf("e05 C_Phase:%d\n",UOC_Check_Para.C_Phase);
				UOC_ERR_PARA[E05].flag = 1;
			}
		}
		else
		{
			UOC_ERR_PARA[E05].fisrt_flag = 0;
		}
	}
	return;
}
void UOC_Spi_Error_E06(void)
{
	uint32_t check_max = 0;
	uint32_t check_min = 0;
	if(UOC_ERR_PARA[E06].flag == 0){
		if(UOC_Check_Para.A_Vol > UOC_Check_Para.B_Vol){
			check_max = UOC_Check_Para.A_Vol;
			check_min = UOC_Check_Para.B_Vol;
		}
		else{
			check_max = UOC_Check_Para.B_Vol;
			check_min = UOC_Check_Para.A_Vol;
		}
			
		if(check_max < UOC_Check_Para.C_Vol){
			check_max = UOC_Check_Para.C_Vol;
		}
		if(check_min > UOC_Check_Para.C_Vol){
			check_min = UOC_Check_Para.C_Vol;
		}
			
		if((check_max*10) > (check_min * 15))//判断条件
		{
			if(UOC_ERR_PARA[E06].fisrt_flag == 0)
			{
				UOC_ERR_PARA[E06].fisrt_flag = 1;
				UOC_ERR_PARA[E06].time_out = GetSysTickCount();
			}
			else if((UOC_ERR_PARA[E06].time_out + (g_uoc_param.LackPhaseTime *100 ))< GetSysTickCount())
			{
				uoc_ERR_Flag_clean();
				UOCPara.new_state = F_ERR;
				UOC_ERR_PARA[E06].flag = 1;
				if(check_max == UOC_Check_Para.A_Vol){
					UOC_ERR_PARA[E06].data = 3;
				}
				else if(check_max == UOC_Check_Para.B_Vol){
					UOC_ERR_PARA[E06].data = 1;
				}
				else if(check_max == UOC_Check_Para.C_Vol){
					UOC_ERR_PARA[E06].data = 2;
				}
				else{
						UOC_ERR_PARA[E06].data = 0;
				}
			}
		}
		else
		{
			UOC_ERR_PARA[E06].fisrt_flag = 0;
		}
	}
}
void UOC_Spi_Error_E07(void)
{
	//反馈判断 不在此处
}
void UOC_Spi_Error_E08(void)
{
	
}
void UOC_Spi_Error_E09(void)
{
	float stand_cur=0;
	if(UOC_Input_State.Ordinary_fan == 1)
	{
		stand_cur = (float)g_uoc_param.RatedCurrent2/10.0;
	}
	else if(UOC_Input_State.high_speed_fan == 1)
	{
		stand_cur = (float)g_uoc_param.RatedCurrent1/10.0;
	}
	if((UOC_Input_State.Ordinary_fan == 1) || (UOC_Input_State.high_speed_fan == 1))
	{
		if(UOC_ERR_PARA[E09].flag == 0)
		{
			if((UOC_Check_Para.A_Cur < (float)(stand_cur*0.35)) || (UOC_Check_Para.B_Cur < (float)(stand_cur*0.35)) || (UOC_Check_Para.C_Cur < (float)(stand_cur*0.35)))
			{
				if(UOC_ERR_PARA[E09].fisrt_flag == 0)
				{
					UOC_ERR_PARA[E09].fisrt_flag = 1;
					UOC_ERR_PARA[E09].time_out = GetSysTickCount();
				}
				else if(UOC_ERR_PARA[E09].time_out + (g_uoc_param.CurEquipTime *1000 )< GetSysTickCount())
				{
					uoc_ERR_Flag_clean();
					UOCPara.new_state = F_ERR;
					UOC_ERR_PARA[E09].flag = 1;
				}
			}
			else
			{
				UOC_ERR_PARA[E09].fisrt_flag = 0;
			}
		}
	}
	return;
}

void UOC_Spi_Error_E10(void)
{
	float cur_max,cur_min;
	if(UOC_Check_Para.A_Cur >= UOC_Check_Para.B_Cur)
	{
		cur_max = UOC_Check_Para.A_Cur;
		cur_min = UOC_Check_Para.B_Cur;
	}
	else
	{
		cur_max = UOC_Check_Para.B_Cur;
		cur_min = UOC_Check_Para.A_Cur;
	}
	if(UOC_Check_Para.C_Cur >= cur_max)
	{
		cur_max = UOC_Check_Para.C_Cur;
	}
	if(cur_min >= UOC_Check_Para.C_Cur)
	{
		cur_min = UOC_Check_Para.C_Cur;
	}
	if(cur_max > cur_min*1.2)
	{
		uoc_ERR_Flag_clean();
		UOCPara.new_state = F_ERR;
		UOC_ERR_PARA[E10].flag = 1;
	}
}
void UOC_Spi_Error_E11(void)
{
	
}
void UOC_Spi_Error_E12(void)
{
	
}

void UOC_Vol_check(void)
{
	if(((UOC_Check_Para.A_Vol > g_uoc_param.VolOverMax) && (UOC_Check_Para.B_Vol > g_uoc_param.VolOverMax) && (UOC_Check_Para.C_Vol > g_uoc_param.VolOverMax))
		||((UOC_Check_Para.A_Vol < g_uoc_param.VolUnderMax) && (UOC_Check_Para.B_Vol < g_uoc_param.VolUnderMax) && (UOC_Check_Para.C_Vol < g_uoc_param.VolUnderMax))) //电源异常
	{
		Control_R03R04R05R06_Function(UOC_POWER_NORMAL,CLOSE);
	}
	else{
		Control_R03R04R05R06_Function(UOC_POWER_NORMAL,OPEN);
	}
}
void UOC_Spi_Error_Para(void)
{
	unsigned char i;
	UOC_SPI_Error_FUN[E00].flag = g_uoc_param.VolOverFlag;
	UOC_SPI_Error_FUN[E01].flag = g_uoc_param.VolUnderFlag;
	UOC_SPI_Error_FUN[E02].flag = g_uoc_param.CurOverLongDelayFlag;
	UOC_SPI_Error_FUN[E03].flag = g_uoc_param.CurOverShortDelayFlag;
	UOC_SPI_Error_FUN[E04].flag = g_uoc_param.MotorShortCircuitFlag;
	UOC_SPI_Error_FUN[E05].flag = g_uoc_param.WrongPhaseFlag;
	UOC_SPI_Error_FUN[E06].flag = g_uoc_param.LackPhaseFlag;
	UOC_SPI_Error_FUN[E09].flag = g_uoc_param.CurEquipFlag;
	for(i=0;i<EMAX;i++)
	{
		if(UOC_SPI_Error_FUN[i].flag){
			UOC_SPI_Error_FUN[i].cb();
		}
	}
	UOC_Vol_check();
	return;
}

void UOC_spi_read_data_cycle(void)
{
	unsigned char i =0;
	
	if(UOCPara.new_state < F_FREE)
		return;

	for(i=0;i<UOC_SPI_MAX_PARA;i++)
	{	
		if(UOCPara.new_state == F_ERR){
			uoc_cyc_para[UOC_SPI_DISPLAY_PARA].start_time = GetSysTickCount();
		}
		if(uoc_cyc_para[i].start_time + uoc_cyc_para[i].time_out  < GetSysTickCount()){
			uoc_cyc_para[i].start_time = GetSysTickCount();
			uoc_cyc_para[i].cb();
		}
	}
	return;
}


