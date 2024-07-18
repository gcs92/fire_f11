/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : mf_config.c
  * @brief          : MCU FUNCTION CONFIG
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 FMSH.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by FMSH under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "mf_config.h"
#include "fm33le0xx_fl.h"
#define PLL_TIMEOUT      0xFFFFFFFFU

/* Private function prototypes -----------------------------------------------*/

void XTHFInit(void)
{
#if 0
    FL_GPIO_InitTypeDef gpioInitStruction;

    //
    gpioInitStruction.mode = FL_GPIO_MODE_ANALOG;
    gpioInitStruction.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    gpioInitStruction.pull = FL_DISABLE;

    gpioInitStruction.pin = FL_GPIO_PIN_2 | FL_GPIO_PIN_3;
    FL_GPIO_Init(GPIOC, &gpioInitStruction);

    FL_RCC_XTHF_WriteDriverStrength(7);
    FL_RCC_XTHF_Enable();
    //FL_DelayMs(2);
#endif

	FL_GPIO_InitTypeDef      gpioInitStruction;
//	  FL_NVIC_ConfigTypeDef    InterruptConfigStruct;
	
    //PC2.3配置成模拟功能，外接XTHF
    gpioInitStruction.mode       = FL_GPIO_MODE_ANALOG;
    gpioInitStruction.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    gpioInitStruction.pull       = FL_DISABLE;

    gpioInitStruction.pin = FL_GPIO_PIN_2 | FL_GPIO_PIN_3;
    FL_GPIO_Init(GPIOC, &gpioInitStruction);

    FL_RCC_XTHF_WriteDriverStrength(7);	
    FL_RCC_XTHF_Enable();
	//  FL_DelayMs(2);
	
	  FL_FDET_ClearFlag_XTHFFail();
	  FL_FDET_EnableIT_XTHFFail();
	
    //InterruptConfigStruct.preemptPriority = 2;
   // FL_NVIC_Init(&InterruptConfigStruct,HFDET_IRQn);
}

void RCC_PLL_ConfigDomain_SYS(uint32_t Source, uint32_t PLL_R, uint32_t PLL_DB, uint32_t PLL_O)
{
    MODIFY_REG(RCC->PLLCR, RCC_PLLCR_DB_Msk | RCC_PLLCR_REFPRSC_Msk | RCC_PLLCR_OSEL_Msk | RCC_PLLCR_INSEL_Msk,
               (PLL_DB << RCC_PLLCR_DB_Pos) | PLL_R | PLL_O | Source);
}

void SelXTHFToPLL(uint32_t clock)
{
#if 1
	uint32_t counter = 0;
    uint32_t readystatus = 0;
	
    if(clock > 64) { return; }

    XTHFInit();          //XTHF初始化使能配置

		FL_FLASH_SetReadWait(FLASH, FL_FLASH_READ_WAIT_2CYCLE);

    RCC_PLL_ConfigDomain_SYS(FL_RCC_PLL_CLK_SOURCE_XTHF, FL_RCC_PLL_PSC_DIV12, clock, FL_RCC_PLL_OUTPUT_X1);
				
    FL_RCC_PLL_Enable();             //使能PLL                             

		do
		{
	      readystatus = FL_RCC_IsActiveFlag_PLLReady();
			  counter++;
	  }
    while(( readystatus != 0x1U) && (counter != PLL_TIMEOUT));        //等待PLL建立稳定，才能将系统时钟源切到PLL，如没有建立，则进入超时
		
    FL_RCC_SetAHBPrescaler(FL_RCC_AHBCLK_PSC_DIV1);
    FL_RCC_SetAPB1Prescaler(FL_RCC_APB1CLK_PSC_DIV1);
    FL_RCC_SetAPB2Prescaler(FL_RCC_APB2CLK_PSC_DIV1);

    FL_RCC_SetSystemClockSource(FL_RCC_SYSTEM_CLK_SOURCE_PLL);
		
		if(clock <= 24)
    {
        FL_FLASH_SetReadWait(FLASH, FL_FLASH_READ_WAIT_0CYCLE);
    }
    else if((clock > 24) && (clock <= 48))
		{
				FL_FLASH_SetReadWait(FLASH, FL_FLASH_READ_WAIT_1CYCLE);
		}
		else
		{
				FL_FLASH_SetReadWait(FLASH, FL_FLASH_READ_WAIT_2CYCLE);
		}
		
		SystemCoreClockUpdate();

#else

		    if(clock > 64) { return; }

    XTHFInit();

    if(clock <= 24)
    {
        FL_FLASH_SetReadWait(FLASH, FL_FLASH_READ_WAIT_0CYCLE);
    }

    else
        if((clock > 24) && (clock <= 48))
        {
            FL_FLASH_SetReadWait(FLASH, FL_FLASH_READ_WAIT_1CYCLE);
        }

        else
        {
            FL_FLASH_SetReadWait(FLASH, FL_FLASH_READ_WAIT_2CYCLE);
        }

    RCC_PLL_ConfigDomain_SYS(FL_RCC_PLL_CLK_SOURCE_XTHF, FL_RCC_PLL_PSC_DIV12, clock, FL_RCC_PLL_OUTPUT_X1);
    FL_RCC_PLL_Enable();

    while(FL_RCC_IsActiveFlag_PLLReady() != FL_SET);

    FL_RCC_SetAHBPrescaler(FL_RCC_AHBCLK_PSC_DIV1);
    FL_RCC_SetAPB1Prescaler(FL_RCC_APB1CLK_PSC_DIV1);
    FL_RCC_SetAPB2Prescaler(FL_RCC_APB2CLK_PSC_DIV1);

    FL_RCC_SetSystemClockSource(FL_RCC_SYSTEM_CLK_SOURCE_PLL);
	#endif
}
/* Private function prototypes -----------------------------------------------*/
void MF_FOUT0_Init(void)
{

    FL_GPIO_InitTypeDef    GPIO_InitStruct;

    GPIO_InitStruct.pin = FL_GPIO_PIN_11;
    GPIO_InitStruct.mode = FL_GPIO_MODE_DIGITAL;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;

    FL_GPIO_Init( GPIOD, &GPIO_InitStruct );
   
}



/**
  * @brief  The application entry point.
  * @retval int
  */
void MF_Clock_Init(void)
{
  /* MCU Configuration--------------------------------------------------------*/

   FL_RCC_EnableGroup1BusClock(FL_RCC_GROUP1_BUSCLK_RTC);
   FL_RTC_WriteAdjustValue(RTC, 0);
   FL_RCC_DisableGroup1BusClock(FL_RCC_GROUP1_BUSCLK_RTC);
 
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

  /* System interrupt init*/

  /* Initialize all configured peripherals */
}





/**
  * @brief System Clock Configuration
  * @retval None
  */
void MF_SystemClock_Config(void)
{
	 SelXTHFToPLL(48-1);
}

/**
  * @brief  I2C Initialization function
  * @param  void
  * @retval None
  */
void MF_I2C_Init(void)
{

    FL_GPIO_InitTypeDef    GPIO_InitStruct;

    FL_I2C_MasterMode_InitTypeDef    defaultInitStruct;

    GPIO_InitStruct.pin = FL_GPIO_PIN_4;
    GPIO_InitStruct.mode = FL_GPIO_MODE_DIGITAL;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_ENABLE;

    FL_GPIO_Init( GPIOC, &GPIO_InitStruct );

    GPIO_InitStruct.pin = FL_GPIO_PIN_5;
    GPIO_InitStruct.mode = FL_GPIO_MODE_DIGITAL;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_ENABLE;

    FL_GPIO_Init( GPIOC, &GPIO_InitStruct );

    defaultInitStruct.clockSource = FL_RCC_I2C_CLK_SOURCE_RCHF;
    defaultInitStruct.baudRate = 40000;

    FL_I2C_MasterMode_Init(I2C,&defaultInitStruct );

    
}

void MF_Spi_Init(void)
{
	FL_GPIO_InitTypeDef gpio;
	FL_SPI_InitTypeDef spi;

	gpio.pin = FL_GPIO_PIN_8 | FL_GPIO_PIN_9 | FL_GPIO_PIN_10 | FL_GPIO_PIN_11;
	gpio.mode = FL_GPIO_MODE_DIGITAL;
	gpio.outputType = FL_GPIO_OUTPUT_PUSHPULL;
	gpio.pull = FL_ENABLE;
	gpio.remapPin = FL_DISABLE;
	FL_GPIO_Init(GPIOB, &gpio);

	spi.transferMode = FL_SPI_TRANSFER_MODE_FULL_DUPLEX;
	spi.mode = FL_SPI_WORK_MODE_MASTER;
	spi.dataWidth = FL_SPI_DATA_WIDTH_32B;
	spi.clockPolarity = FL_SPI_POLARITY_NORMAL;
	spi.clockPhase = FL_SPI_PHASE_EDGE2;
	spi.softControl = FL_ENABLE;
	//spi.baudRate = FL_SPI_BAUDRATE_DIV4;//in
	spi.baudRate = FL_SPI_BAUDRATE_DIV128;//out
	spi.bitOrder = FL_SPI_BIT_ORDER_MSB_FIRST;

	FL_SPI_Init(SPI1, &spi);
}

void MF_Led_Init(void)
{
	FL_GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.pin = FL_GPIO_PIN_3 | FL_GPIO_PIN_8 | FL_GPIO_PIN_9|
                        FL_GPIO_PIN_10| FL_GPIO_PIN_11;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.pin = FL_GPIO_PIN_8 | FL_GPIO_PIN_9 | FL_GPIO_PIN_10;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    FL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void MF_Button_Init(void)
{
	FL_GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.pin = FL_GPIO_PIN_8 | FL_GPIO_PIN_9 | FL_GPIO_PIN_10 
                        | FL_GPIO_PIN_13| FL_GPIO_PIN_14| FL_GPIO_PIN_15;
	GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
	GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.pull = FL_ENABLE;
	GPIO_InitStruct.remapPin = FL_DISABLE;
	FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void MF_Control_Init(void)
{
	// /*======================output init=========================================================*/
	// FL_GPIO_InitTypeDef GPIO_InitStruct;
	// // FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_0);
	// // FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_1);
	// // FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_2);
	// // FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_3);
	// // FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_4);
	// // FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_5);
	// // FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_6);
	// // FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_7);
	// // FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_8);
	// GPIO_InitStruct.pin =   FL_GPIO_PIN_0 | FL_GPIO_PIN_1 | FL_GPIO_PIN_2 |
	// 	                    FL_GPIO_PIN_3 | FL_GPIO_PIN_4 | FL_GPIO_PIN_5 |
	// 	                    FL_GPIO_PIN_6 | FL_GPIO_PIN_7 | FL_GPIO_PIN_8 | 
	// 	                    FL_GPIO_PIN_13| FL_GPIO_PIN_14| FL_GPIO_PIN_15;
	// GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
	// GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
	// GPIO_InitStruct.pull = FL_ENABLE;
	// GPIO_InitStruct.remapPin = FL_DISABLE;
	// FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	
	// GPIO_InitStruct.pin = FL_GPIO_PIN_2 | FL_GPIO_PIN_3 | FL_GPIO_PIN_4 | FL_GPIO_PIN_5 | FL_GPIO_PIN_6;
	// GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
	// GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
	// GPIO_InitStruct.pull = FL_DISABLE;
	// GPIO_InitStruct.remapPin = FL_DISABLE;
	// FL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	// /*======================intput init=========================================================*/	
	// GPIO_InitStruct.pin = FL_GPIO_PIN_0 | FL_GPIO_PIN_1 | FL_GPIO_PIN_2 | FL_GPIO_PIN_3 | FL_GPIO_PIN_4 |
	// 							FL_GPIO_PIN_5 | FL_GPIO_PIN_6 | FL_GPIO_PIN_7 | FL_GPIO_PIN_12;
	// GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
	// GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
	// GPIO_InitStruct.pull = FL_ENABLE;
	// GPIO_InitStruct.remapPin = FL_DISABLE;
	// FL_GPIO_Init(GPIOB, &GPIO_InitStruct);


	// GPIO_InitStruct.pin = FL_GPIO_PIN_1;
	// GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
	// GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
	// GPIO_InitStruct.pull = FL_ENABLE;
	// GPIO_InitStruct.remapPin = FL_DISABLE;
	// FL_GPIO_Init(GPIOC, &GPIO_InitStruct);


	// GPIO_InitStruct.pin = FL_GPIO_PIN_0 | FL_GPIO_PIN_1 | FL_GPIO_PIN_9 | FL_GPIO_PIN_10 | FL_GPIO_PIN_11;
	// GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
	// GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
	// GPIO_InitStruct.pull = FL_ENABLE;
	// GPIO_InitStruct.remapPin = FL_DISABLE;
	// FL_GPIO_Init(GPIOD, &GPIO_InitStruct);


}


void MF_GPTIM0_TimerBase_Init(void)
{

    /*IO CONFIG*/
    FL_GPTIM_InitTypeDef    TimerBaseInitStruct;

    TimerBaseInitStruct.prescaler = 48-1;
    TimerBaseInitStruct.counterMode = FL_GPTIM_COUNTER_DIR_UP;
    TimerBaseInitStruct.autoReload = 1000;
    TimerBaseInitStruct.autoReloadState = FL_DISABLE;
    TimerBaseInitStruct.clockDivision = FL_GPTIM_CLK_DIVISION_DIV1;
    FL_GPTIM_Init(GPTIM0,&TimerBaseInitStruct );

    FL_GPTIM_ClearFlag_Update(GPTIM0);
    FL_GPTIM_EnableIT_Update(GPTIM0 );
 
}

void MF_GPTIM1_TimerBase_Init(void)
{

    /*IO CONFIG*/
    FL_GPTIM_InitTypeDef    TimerBaseInitStruct;

    TimerBaseInitStruct.prescaler = 48-1;
    TimerBaseInitStruct.counterMode = FL_GPTIM_COUNTER_DIR_UP;
    TimerBaseInitStruct.autoReload = 999;
    TimerBaseInitStruct.autoReloadState = FL_DISABLE;
    TimerBaseInitStruct.clockDivision = FL_GPTIM_CLK_DIVISION_DIV1;
    FL_GPTIM_Init(GPTIM1,&TimerBaseInitStruct );

    FL_GPTIM_ClearFlag_Update(GPTIM1);
    FL_GPTIM_EnableIT_Update(GPTIM1);
 
}


void MF_NVIC_Init(void)
{
    FL_NVIC_ConfigTypeDef    InterruptConfigStruct;

    InterruptConfigStruct.preemptPriority = 0x00;
    FL_NVIC_Init(&InterruptConfigStruct,GPTIM0_IRQn );

	InterruptConfigStruct.preemptPriority = 0x00;
    FL_NVIC_Init(&InterruptConfigStruct,GPTIM1_IRQn );

	InterruptConfigStruct.preemptPriority = 2;
    FL_NVIC_Init(&InterruptConfigStruct,UART1_IRQn ); 

  InterruptConfigStruct.preemptPriority = 2;
    FL_NVIC_Init(&InterruptConfigStruct,UART5_IRQn ); 
}

void MF_UART1_Init(void)
{

    FL_GPIO_InitTypeDef    GPIO_InitStruct;

    FL_UART_InitTypeDef    defaultInitStruct;

	GPIO_InitStruct.pin = FL_GPIO_PIN_14;
	GPIO_InitStruct.mode = FL_GPIO_MODE_DIGITAL;
	GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.pull = FL_DISABLE;
	GPIO_InitStruct.remapPin = FL_DISABLE;

	FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.pin = FL_GPIO_PIN_13;
	GPIO_InitStruct.mode = FL_GPIO_MODE_DIGITAL;
	GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.pull = FL_DISABLE;
	GPIO_InitStruct.remapPin = FL_DISABLE;
	FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    defaultInitStruct.clockSrc          = FL_RCC_UART1_CLK_SOURCE_APB1CLK;
    defaultInitStruct.baudRate          = 115200;
    defaultInitStruct.dataWidth         = FL_UART_DATA_WIDTH_8B;
    defaultInitStruct.stopBits          = FL_UART_STOP_BIT_WIDTH_1B;
    defaultInitStruct.parity            = FL_UART_PARITY_EVEN;
    defaultInitStruct.transferDirection = FL_UART_DIRECTION_TX_RX;

    FL_UART_Init(UART1,&defaultInitStruct );
    
}

void MF_UART5_Init(void)
{

    FL_GPIO_InitTypeDef    GPIO_InitStruct;

    FL_UART_InitTypeDef    defaultInitStruct;

	// GPIO_InitStruct.pin = FL_GPIO_PIN_1;
	// GPIO_InitStruct.mode = FL_GPIO_MODE_DIGITAL;
	// GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
	// GPIO_InitStruct.pull = FL_DISABLE;
	// GPIO_InitStruct.remapPin = FL_DISABLE;

	// FL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	GPIO_InitStruct.pin = FL_GPIO_PIN_1 | FL_GPIO_PIN_0;
	GPIO_InitStruct.mode = FL_GPIO_MODE_DIGITAL;
	GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.pull = FL_DISABLE;
	GPIO_InitStruct.remapPin = FL_DISABLE;
	FL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    defaultInitStruct.clockSrc          = FL_RCC_UART1_CLK_SOURCE_APB1CLK;
    defaultInitStruct.baudRate          = 9600;
    defaultInitStruct.dataWidth         = FL_UART_DATA_WIDTH_8B;
    defaultInitStruct.stopBits          = FL_UART_STOP_BIT_WIDTH_1B;
    defaultInitStruct.parity            = FL_UART_PARITY_EVEN;
    defaultInitStruct.transferDirection = FL_UART_DIRECTION_TX_RX;

    FL_UART_Init(UART5,&defaultInitStruct );
    
}
void MF_SysTemRest(void)
{
    NVIC_SystemReset();
}


void MF_Debug_Init()
{
	MF_UART1_Init();
}
void MF_UART_Init()
{
	MF_UART5_Init();
}

void MF_Production_Init()
{
	FL_GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.pin = FL_GPIO_PIN_10;
	GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
	GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.pull = FL_ENABLE;
	GPIO_InitStruct.remapPin = FL_DISABLE;
	FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}


void MF_Config_Init(void)
{
	//MF_Control_Init();
	//MF_Spi_Init();
	MF_Led_Init();
	MF_Debug_Init();    //打印uart
  MF_UART_Init();     //数据uart
	MF_Production_Init();//生产自检模式
	MF_Button_Init();
	//MF_I2C_Init();
	MF_GPTIM0_TimerBase_Init();
	MF_GPTIM1_TimerBase_Init();
	MF_NVIC_Init();
	GPTIM_Start();


		FL_UART_ClearFlag_RXBuffFull(UART1);
    FL_UART_EnableIT_RXBuffFull(UART1);           //使能接收中断

    FL_UART_ClearFlag_RXBuffFull(UART5);
    FL_UART_EnableIT_RXBuffFull(UART5);           //使能接收中断
	//FL_UART_DisableIT_TXShiftBuffEmpty(UART1);
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN Assert_Failed */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END Assert_Failed */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT FMSH *****END OF FILE****/
