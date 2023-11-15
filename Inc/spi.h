#ifndef __SPI_H__
#define __SPI_H__

#include "fm33le0xx_fl.h"

typedef enum
{
    UOC_SPI_DEV_ID = 0X00000000,
    UOC_SPI_PHASE_A_VOLTAGE = 0X0D000000,
    UOC_SPI_PHASE_B_VOLTAGE = 0X0E000000,
    UOC_SPI_PHASE_C_VOLTAGE = 0X0F000000,
	UOC_SPI_PHASE_A_CURRENTS = 0X10000000,
	UOC_SPI_PHASE_B_CURRENTS = 0X11000000,
	UOC_SPI_PHASE_C_CURRENTS = 0X12000000,
	UOC_SPI_PHASE_A_PHASE = 0X26000000,
	UOC_SPI_PHASE_B_PHASE = 0X27000000,
	UOC_SPI_PHASE_C_PHASE = 0X28000000,
    UOC_SPI_MAX,
} eUOCSPIId_TypeDef;

typedef enum
{
    UOC_SPI_CHECK_PARA = 0,
	UOC_SPI_READ_PARA,
	UOC_SPI_DISPLAY_PARA,
	UOC_SPI_ERROR_PARA,
    UOC_SPI_MAX_PARA,
} eUOCSPICYC_TypeDef;


typedef struct
{
	unsigned char id;
	uint32_t start_time;
    uint32_t time_out;
    void (*cb)(void);
} sSPIData_TypeDef;

typedef struct
{
	unsigned char id;
	unsigned char flag;
    void (*cb)(void);
} sSPIError_TypeDef;



#define	UOC_SPI_READ_CHECK_EN		0xC600005A
#define	UOC_SPI_READ_CHECK_DIS	0xC6000055
#define	UOC_SPI_WRITE_CHECK_EN	0xC900005A
#define	UOC_SPI_WRITE_CHECK_DIS	0xC9000055
#define	UOC_SPI_SET_RESET				0XD3000000
#define	UOC_SPI_SET_MODE				0X8100B97E
#define	UOC_SPI_SET_EMU_EN				0X8300F804
#define	UOC_SPI_SET_BOR_EN				0Xb1003427


#define	UOC_SPI_SET_TONGBU_DATA				0XC5000002
#define	UOC_SPI_SET_DATA_START				0Xc000cccB

//#define UOC_Currents_Offsets	18.47
#define UOC_Currents_Offsets	12.03
#define UOC_Voltage_Offsets	1.55




uint32_t SPI_ReadWrite(uint32_t data);
void SPI_Write(uint8_t *data, uint16_t length);
void SPI_Read(uint8_t *data, uint16_t length);
void SPI_Test(void);
uint32_t SPI_Read_data(uint32_t cmd);
void SPI_Mode_init(void);
int SPI_Check(void);

void UOC_Spi_Check_Para(void);
void UOC_Spi_Read_Para(void);
void UOC_Spi_Display_Para(void);
void UOC_spi_read_data_cycle(void);
void UOC_Spi_Error_Para(void);
void UOC_Spi_Error_E00(void);
void UOC_Spi_Error_E01(void);
void UOC_Spi_Error_E02(void);
void UOC_Spi_Error_E03(void);
void UOC_Spi_Error_E04(void);
void UOC_Spi_Error_E05(void);
void UOC_Spi_Error_E06(void);
void UOC_Spi_Error_E07(void);
void UOC_Spi_Error_E08(void);
void UOC_Spi_Error_E09(void);
void UOC_Spi_Error_E10(void);
void UOC_Spi_Error_E11(void);
void UOC_Spi_Error_E12(void);

#endif
