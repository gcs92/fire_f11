#include "I2C.h"
#include <string.h>

#define I2CREAD     1   //I2C读操作
#define I2CWRITE    0   //I2C写操作

#define STARTBIT    0
#define RESTARTBIT  1
#define STOPBIT     2

#define DEVICE_EE256    0xA0    //FM24C256的器件地址
#define ADDRLEN_EE256   2       //FM24C256的数据地址长度为2字节

static unsigned char Nixie_Tube_Num[2][20]={
	{0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x01,0x40,0x08,0x00},
	{0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,0x87,0xff,0xef,0xf7,0xfc,0xb9,0xde,0xf9,0xf1,0x81,0xc0,0x88,0x80},
};

static unsigned char Nixie_Tube_Addr[5]={0x48,0x68,0x6a,0x6c,0x6e};

eUOCDisplayErr_TypeDef UOC_ERR_PARA[EMAX] = 
{
	{E00,0,0,0,0},
	{E01,0,0,0,0},
	{E02,0,0,0,0},
	{E03,0,0,0,0},
	{E04,0,0,0,0},
	{E05,0,0,0,0},
	{E06,0,0,0,0},
	{E07,0,0,0,0},
	{E08,0,0,0,0},
	{E09,0,0,0,0},
	{E10,0,0,0,0},
	{E11,0,0,0,0},
	{E12,0,0,0,0},
};

uint8_t I2C_Send_Bit(uint8_t BIT_def)
{
    uint32_t counter =0;
	  uint32_t stopstatus,startstatus;

    switch(BIT_def)
    {
        case STARTBIT:
            FL_I2C_Master_EnableI2CStart(I2C);
				    do
						{
						    startstatus = FL_I2C_Master_IsActiveFlag_Start(I2C);
							  counter++;
						}while((startstatus != 0x01U) && (counter != I2C_TIMEOUT));

            break;

        case RESTARTBIT:
            FL_I2C_Master_EnableI2CRestart(I2C);
						do
						{
						    startstatus = FL_I2C_Master_IsActiveFlag_Start(I2C);
							  counter++;
						}while((startstatus != 0x01U) && (counter != I2C_TIMEOUT));
            break;

        case STOPBIT:
            FL_I2C_Master_EnableI2CStop(I2C);
						do
						{
						    stopstatus = FL_I2C_Master_IsActiveFlag_Stop(I2C);
							  counter++;
						}while((stopstatus != 0x01U) && (counter != I2C_TIMEOUT));
            break;

        default:
            break;
    }

    return 0; //ok

}

uint8_t I2C_Send_Byte(uint8_t x_byte)
{
    FL_I2C_Master_WriteTXBuff(I2C, x_byte);

    while(!FL_I2C_Master_IsActiveFlag_TXComplete(I2C));

    FL_I2C_Master_ClearFlag_TXComplete(I2C);
    if(!FL_I2C_Master_IsActiveFlag_NACK(I2C))
    {
        return 0;
    }
    else
    {
        FL_I2C_Master_ClearFlag_NACK(I2C);
        return 1;
    }

}
uint8_t Wait_for_end(uint8_t Device)
{
		uint8_t result, Devi_Addr;
	  uint32_t counter =0;
    Devi_Addr = Device;
	  do
	  {
        I2C_Send_Bit(STARTBIT);      //发送起始位
        result = I2C_Send_Byte(Devi_Addr);   //发送器件地址
        I2C_Send_Bit(STOPBIT);   //发送停止位
			  counter++;
    }while((result == 0x01U) && (counter != I2C_TIMEOUT));
    
		if(result == 0)//设置地址成功, 写周期结束
     { 
			 return 0; 
		 } 
    else
		{			
       return 1; //设置地址失败,超时
		}
}

uint8_t Sendaddr_Display(uint8_t Device)
{
    uint8_t result, Devi_Addr;

    Devi_Addr = Device;

    /*-------------- start bit ---------------*/
    result = I2C_Send_Bit(STARTBIT);  //发送起始位

    if(result != 0) { return 1; }   //failure.

    /*-------------- disable read -------------*/
   // FL_I2C_Master_DisableRX(I2C);
    /*-------------- device addr -------------*/
    result = I2C_Send_Byte(Devi_Addr);   //发送器件地址

    if(result != 0) { return 2; }   //failure.

    return 0; //ok
}


uint8_t I2C_Write_Display(uint8_t Device, uint8_t *Buf, uint8_t Len)
{
    uint8_t k, n, status;

    if(Len > 128) { return 0xFF; }//一次最多操作128字节

    for(k = 0; k < 3; k++) //每遍最多写3次
    {
        status = 0;
        if(Sendaddr_Display(Device))
        {
            status = 1; //写入失败
        }
		else
        {
            for(n = 0; n < Len; n++)
            {
                //发送一个字节
                if(I2C_Send_Byte(Buf[n]))
                {	
                    status = 1;
                    break;
                } //写入失败
            }
        }
        //发送停止位
        if(I2C_Send_Bit(STOPBIT))
        { 
        	status = 1;
		}

        if(status == 0)
        {
            status = Wait_for_end(Device);
            break;
        } //写正确
    }

    return(status);
}


uint8_t I2C_Display(uint8_t flag,uint8_t status,uint8_t num)
{
	uint8_t Device = Nixie_Tube_Addr[flag];
	uint8_t Buf[2];
	uint8_t Len = 0;
	uint8_t Result = 0;

	Len = 1;
	if(flag == 0)
	{
		Buf[0] = 0x11;
	}
	else
	{
		Buf[0] = Nixie_Tube_Num[status][num];
	}
	Result = I2C_Write_Display(Device, Buf, Len);//写i2c

	return Result;
}

uint8_t I2C_Display_Err(uint8_t flag,uint8_t id)
{
	uint8_t Device = Nixie_Tube_Addr[2];
	uint8_t Buf[2];
	uint8_t Len = 0;
	uint8_t Result = 0;

	Len = 1;
	Buf[0] = 0x79;//e
	Result = I2C_Write_Display(Device, Buf, Len);//写i2c

	switch (id)
		{
		case 0:
			I2C_Display(DISPLAY1,DISPLAY_NO_POINT,DISPLAY_OFF);
			break;
		case 1:
			I2C_Display(DISPLAY1,DISPLAY_NO_POINT,DISPLAY_A_PHASE);
			break;
		case 2:
			I2C_Display(DISPLAY1,DISPLAY_NO_POINT,DISPLAY_B_PHASE);
			break;
		case 3:
			I2C_Display(DISPLAY1,DISPLAY_NO_POINT,DISPLAY_C_PHASE);
			break;
		default:
			I2C_Display(DISPLAY1,DISPLAY_NO_POINT,DISPLAY_OFF);
			break;
		}
	
	I2C_Display(DISPLAY3,DISPLAY_NO_POINT,(flag/10));
	I2C_Display(DISPLAY4,DISPLAY_NO_POINT,(flag%10));

	return Result;
}


