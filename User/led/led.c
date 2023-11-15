#include "led.h"

gUOCLed_TypeDef UOCLed[UOC_LED_MAX] = 
{
	{UOC_LED_GS0,	Display_GPIO,	Display_PIN0},
	{UOC_LED_GS,	Display_GPIO,	Display_PIN1},
	{UOC_LED_DS,	Display_GPIO,	Display_PIN2},
	{UOC_LED_YX,	Display_GPIO,	Display_PIN3},
	{UOC_LED_SK,	Display_GPIO,	Display_PIN4},
	{UOC_LED_SD,	Display_GPIO,	Display_PIN5},
	{UOC_LED_GZ,	Display_GPIO,	Display_PIN6},
	{UOC_LED_DDC,	Display_GPIO,	Display_PIN7},
	{UOC_LED_RUN,	STATE_GPIO,		Display_PIN10},
	{UOC_LED_NV,	STATE_GPIO,		Display_PIN11},
	{UOC_LED_NA,	STATE_GPIO,		Display_PIN12},
};

void led_display(unsigned char ledname,unsigned char Control)
{
	if(Control == LED_ON)
	{
		FL_GPIO_ResetOutputPin(UOCLed[ledname].LedGpio,UOCLed[ledname].LedPin);
	}
	else if(Control == LED_OFF)
	{
		FL_GPIO_SetOutputPin(UOCLed[ledname].LedGpio,UOCLed[ledname].LedPin);
	}
}
