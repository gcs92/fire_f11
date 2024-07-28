#ifndef __BUTTON_FUNCTION_H__
#define __BUTTON_FUNCTION_H__
#include "stdint.h"

#define key_down 0
#define key_up 1
#define delay_time 10
#define key_max 4
typedef enum
{
    UOC_KEY_FREE = 0,
	UOC_KEY_1,
	UOC_KEY_2,
	UOC_KEY_3,
	UOC_KEY_4,
    UOC_KEY_MAX,
} eUOCKEYId_TypeDef;

typedef struct
{
    unsigned char name;
	unsigned char flag:1;
	unsigned char param_seat:7;
    void (*cb)(void);
} sButtonEvent_TypeDef;


typedef struct
{
    
	uint8_t		new_state;//当前状态
	uint8_t		para_state;//是否进入设置模式
	uint8_t		last_state;//上一次状态
	uint32_t	pare_num;
	uint32_t	Display_timeout;
	uint32_t	Wait_timeout;
	uint8_t		offset:5;
	uint8_t		displayflag:1;
	uint8_t		ledflag:1;
	uint8_t		spi_read_flag:1;//spi读取数值是否锁定 0：不锁定 1：锁定
	uint8_t		para_fail:4;//参数是否设置成功 1：不成功 0；成功
	uint8_t		para_fail_count:4;//参数设置不成功时闪动次数
} sParaFlag_TypeDef;

typedef struct
{
    uint16_t		flag:1;
	uint16_t		timeflag:1;
	uint16_t		timeout:14;
} sKeyFlag_TypeDef;





#define KEY1()	FL_GPIO_GetInputPin(GPIOC, FL_GPIO_PIN_6)
#define KEY2()	FL_GPIO_GetInputPin(GPIOC, FL_GPIO_PIN_7)
#define KEY3()	FL_GPIO_GetInputPin(GPIOC, FL_GPIO_PIN_8)
#define KEY4()	FL_GPIO_GetInputPin(GPIOC, FL_GPIO_PIN_9)

void UOC_paremeter_F00_FUNC(void);
void UOC_paremeter_F01_FUNC(void);
void UOC_paremeter_F02_FUNC(void);
void UOC_paremeter_F03_FUNC(void);
void UOC_paremeter_F04_FUNC(void);
void UOC_paremeter_F05_FUNC(void);
void UOC_paremeter_F06_FUNC(void);
void UOC_paremeter_F07_FUNC(void);
void UOC_paremeter_F08_FUNC(void);
void UOC_paremeter_F09_FUNC(void);

void UOC_paremeter_F10_FUNC(void);
void UOC_paremeter_F11_FUNC(void);
void UOC_paremeter_F12_FUNC(void);
void UOC_paremeter_F13_FUNC(void);
void UOC_paremeter_F14_FUNC(void);
void UOC_paremeter_F15_FUNC(void);
void UOC_paremeter_F16_FUNC(void);
void UOC_paremeter_F17_FUNC(void);
void UOC_paremeter_F18_FUNC(void);
void UOC_paremeter_F19_FUNC(void);

void UOC_paremeter_F20_FUNC(void);
void UOC_paremeter_F21_FUNC(void);
void UOC_paremeter_F22_FUNC(void);
void UOC_paremeter_F23_FUNC(void);
void UOC_paremeter_F24_FUNC(void);
void UOC_paremeter_F25_FUNC(void);
void UOC_paremeter_F26_FUNC(void);
void UOC_paremeter_F27_FUNC(void);
void UOC_paremeter_F28_FUNC(void);
void UOC_paremeter_F29_FUNC(void);

void UOC_paremeter_F30_FUNC(void);
void UOC_paremeter_F31_FUNC(void);
void UOC_paremeter_F32_FUNC(void);
void UOC_paremeter_F33_FUNC(void);
void UOC_paremeter_F34_FUNC(void);
void UOC_paremeter_F35_FUNC(void);
void UOC_paremeter_F36_FUNC(void);
void UOC_paremeter_F37_FUNC(void);
void UOC_paremeter_F38_FUNC(void);
void UOC_paremeter_F39_FUNC(void);

void UOC_paremeter_F40_FUNC(void);
void UOC_paremeter_F41_FUNC(void);
void UOC_paremeter_F42_FUNC(void);
void UOC_paremeter_F43_FUNC(void);
void UOC_paremeter_F44_FUNC(void);
void UOC_paremeter_F45_FUNC(void);
void UOC_paremeter_F46_FUNC(void);
void UOC_paremeter_F47_FUNC(void);
void UOC_paremeter_F48_FUNC(void);
void UOC_paremeter_F49_FUNC(void);

void UOC_paremeter_F50_FUNC(void);
void UOC_paremeter_F51_FUNC(void);
void UOC_paremeter_F52_FUNC(void);
void UOC_paremeter_F53_FUNC(void);
void UOC_paremeter_F54_FUNC(void);
void UOC_paremeter_F55_FUNC(void);
void UOC_paremeter_F56_FUNC(void);
void UOC_paremeter_F57_FUNC(void);
void UOC_paremeter_F58_FUNC(void);
void UOC_paremeter_F59_FUNC(void);

void UOC_paremeter_F60_FUNC(void);
void UOC_paremeter_F61_FUNC(void);
void UOC_paremeter_F62_FUNC(void);
void UOC_paremeter_F63_FUNC(void);
void UOC_paremeter_F64_FUNC(void);
void UOC_paremeter_F65_FUNC(void);
void UOC_paremeter_F66_FUNC(void);
void UOC_paremeter_F67_FUNC(void);
void UOC_paremeter_F68_FUNC(void);
void UOC_paremeter_F69_FUNC(void);

void UOC_paremeter_FREE_FUNC(void);
void UOC_paremeter_ERR_FUNC(void);
void uoc_ERR_Flag_clean(void);

uint8_t protocol_deal(void *buf,uint8_t len);



void System_Funtion(void);
#endif

