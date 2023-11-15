#ifndef __LED_H__
#define __LED_H__

#include "main.h"
#include "fm33le0xx_fl.h"
#ifndef MFANG
#include <stdio.h>
#endif

typedef enum
{
    UOC_LED_GS0 = 0,
    UOC_LED_GS,
    UOC_LED_DS,
    UOC_LED_YX,
    UOC_LED_SK,
    UOC_LED_SD,
    UOC_LED_GZ,
    UOC_LED_DDC,
    UOC_LED_RUN,
    UOC_LED_NV,
    UOC_LED_NA,
    UOC_LED_MAX,
} eUOCLedId_TypeDef;

typedef struct
{
    unsigned char LedFlag;
	GPIO_Type * LedGpio;
	uint32_t	LedPin;
} gUOCLed_TypeDef;



#define Display_GPIO   GPIOA
#define STATE_GPIO   GPIOC

#define Display_PIN0	FL_GPIO_PIN_0
#define Display_PIN1    FL_GPIO_PIN_1
#define Display_PIN2    FL_GPIO_PIN_2
#define Display_PIN3    FL_GPIO_PIN_3
#define Display_PIN4    FL_GPIO_PIN_4
#define Display_PIN5    FL_GPIO_PIN_5
#define Display_PIN6    FL_GPIO_PIN_6
#define Display_PIN7    FL_GPIO_PIN_7
#define Display_PIN8    FL_GPIO_PIN_8
#define Display_PIN9    FL_GPIO_PIN_9
#define Display_PIN10	FL_GPIO_PIN_10
#define Display_PIN11	FL_GPIO_PIN_11
#define Display_PIN12	FL_GPIO_PIN_12


#define LED_ON	0
#define LED_OFF 1

void led_display(unsigned char ledname,unsigned char Control);

#endif
