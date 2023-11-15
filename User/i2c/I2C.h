#ifndef __I2CTEST_H__
#define __I2CTEST_H__

#include "fm33le0xx_fl.h"

#define  I2C_TIMEOUT   0x1FFFFU



typedef enum
{
    DISPLAY_NO_POINT = 0,
    DISPLAY_POINT,
} eUOCDisplayStatus_TypeDef;

typedef enum
{
	DISPLAY_FN	=15,
    DISPLAY_A_PHASE = 16,
    DISPLAY_B_PHASE,
    DISPLAY_C_PHASE,
    DISPLAY_OFF,
    DISPLAY_MAX,
} eUOCDisplayFlagStatus_TypeDef;


typedef enum
{
	DISPLAYCMD = 0,
    DISPLAY1,
    DISPLAY2,
    DISPLAY3,
    DISPLAY4,
} eUOCDisplayId_TypeDef;

typedef enum
{
	E00 = 0,
    E01,
    E02,
    E03,
    E04,
    E05,
    E06,
    E07,
    E08,
    E09,
    E10,
    E11,
    E12,
    EMAX,
} eUOCDisplayErrId_TypeDef;

typedef struct
{
	unsigned char id;
	unsigned char flag:2;
	unsigned char fisrt_flag:2;
	unsigned char data:4;
	uint32_t time_out;
} eUOCDisplayErr_TypeDef;




uint8_t I2C_Eeprom(void);
uint8_t I2C_Display(uint8_t flag,uint8_t status,uint8_t num);
uint8_t I2C_Display_Err(uint8_t flag,uint8_t id);


#endif
