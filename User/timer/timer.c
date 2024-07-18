#include "timer.h"
#include "led.h"
#include"button_function.h"
#include "system_control.h"

extern sKeyFlag_TypeDef uoc_key[key_max];
extern sDInputEvent_TypeDef uoc_DI_Funtion[UOC_DIMAX];

void GPTIM_Start()
{
    //使能GPTIM
    FL_GPTIM_Enable(GPTIM0);
	FL_GPTIM_Enable(GPTIM1);
}


void GPTIM0_IRQHandler()
{
    if(FL_GPTIM_IsEnabledIT_Update(GPTIM0) && FL_GPTIM_IsActiveFlag_Update(GPTIM0))
    {
        FL_GPTIM_ClearFlag_Update(GPTIM0);

        g_u32SysTickCount++;
    }
}

void GPTIM1_IRQHandler()
{
	unsigned char i=0;
    if(FL_GPTIM_IsEnabledIT_Update(GPTIM1) && FL_GPTIM_IsActiveFlag_Update(GPTIM1))
    {
        FL_GPTIM_ClearFlag_Update(GPTIM1);

		for(i=0;i<key_max;i++)
		{
        	if(uoc_key[i].timeflag == 1)
        	{
        		uoc_key[i].timeout++;
        	}
		}
		for(i=0;i<UOC_DIMAX;i++)
		{
        	if(uoc_DI_Funtion[i].timeflag == 1)
        	{
        		uoc_DI_Funtion[i].timeCount++;
        	}
		}
			
    }
}


void Delay1Ms(uint32_t x)
{
    int32_t uDelayTickCount;
		uDelayTickCount = GetSysTickCount() + x;		
		while(GetSysTickCount() < uDelayTickCount);
}
