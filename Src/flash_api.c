#include "main.h"
#include "flash_api.h"
//#include "fm33le0xx_fl_flash.h"
#include"debug.h"


int32_t flash_write(uint32_t id, uint8_t *buff, uint16_t len)
{
    // uint16_t pageNum, i, j;
    // uint8_t buffTmp[FLASH_PAGE_SIZE] = {0};
    // uint8_t cs = 0;
    uint32_t addr = NULL;
    switch (id)
    {
    case FLASH_PROGRAM_ID:
        addr = FLASH_PROG_ADDR;
        break;
    case FLASH_SYSPARAM_ID:
        addr = FLASH_SYSPARAM_ADDR;
        break;
    default:
        return false;
    }
    FL_FLASH_PageErase(FLASH, addr);
    FL_FLASH_Write_Dma(FLASH, addr, (uint32_t *)buff);
    return true;
}
int32_t flase_read(uint32_t id, uint8_t *buff, uint16_t len)
{
    // uint16_t pageNum, i;
    uint32_t addr = NULL;
    // uint8_t rxLen;
    // pageNum = (len + FLASH_PAGE_SIZE - 1) / FLASH_PAGE_SIZE;
    switch (id)
    {
    case FLASH_PROGRAM_ID:
        addr = FLASH_PROG_ADDR;
        break;
    case FLASH_SYSPARAM_ID:
        addr = FLASH_SYSPARAM_ADDR;
        break;
    default:
        return false;
    }
    FL_FLASH_Read_Dma(FLASH, addr, (uint32_t *)buff, FLASH_PAGE_SIZE >> 2);
    return true;
}

FL_ErrorStatus FL_FLASH_Program_String(FLASH_Type* FLASHx, uint32_t Address, uint32_t *data,uint32_t Length)
{
  uint32_t i;
	FL_ErrorStatus ret;
	for(i=0; i<Length; i++)
	{
		ret = FL_FLASH_Program_Word(FLASH, Address + i * 4, data[i]);
		if(ret == FL_FAIL)
			return FL_FAIL;
	}
  return FL_PASS;
}
#if 0
#include "flash.h"
#include <string.h>

#define FLASH_PROG_ADDR         0x00004000
#define FLASH_PAGE_SIZE         256

#define BUFFER_SIZE     128
uint32_t DataBuffer[BUFFER_SIZE];

void FlashRW(void)
{
    FL_FLASH_PageErase(FLASH, FLASH_PROG_ADDR);
    memset(DataBuffer, 0x55, BUFFER_SIZE * 4);
    FL_FLASH_Program_Word(FLASH, FLASH_PROG_ADDR, DataBuffer[0]);

    FL_FLASH_PageErase(FLASH, FLASH_PROG_ADDR);
    memset(DataBuffer, 0xAA, BUFFER_SIZE * 4);
    FL_FLASH_Program_Page(FLASH, FLASH_PROG_ADDR / FL_FLASH_PAGE_SIZE_BYTE, DataBuffer);

    FL_FLASH_SectorErase(FLASH, FLASH_PROG_ADDR);
    memset(DataBuffer, 0x00, BUFFER_SIZE * 4);
    FL_FLASH_Program_Sector(FLASH, FLASH_PROG_ADDR / FL_FLASH_SECTOR_SIZE_BYTE, DataBuffer);

    FL_FLASH_SectorErase(FLASH, FLASH_PROG_ADDR);
}

extern FL_ErrorStatus FL_FLASH_Write_Dma(FLASH_Type *FLASHx, uint32_t address, uint32_t *data);
extern FL_ErrorStatus FL_FLASH_Read_Dma(FLASH_Type *FLASHx, uint32_t address, uint32_t *data, uint16_t length);

void FlashRW_Dma(void)
{
    FL_FLASH_PageErase(FLASH, FLASH_PROG_ADDR);
    memset(DataBuffer, 0x00, BUFFER_SIZE * 4);
    FL_FLASH_Read_Dma(FLASH, FLASH_PROG_ADDR, DataBuffer, FLASH_PAGE_SIZE / 4);
    memset(DataBuffer, 0x55, BUFFER_SIZE * 4);
    FL_FLASH_Write_Dma(FLASH, FLASH_PROG_ADDR, DataBuffer);
    memset(DataBuffer, 0x00, BUFFER_SIZE * 4);
    FL_FLASH_Read_Dma(FLASH, FLASH_PROG_ADDR, DataBuffer, FLASH_PAGE_SIZE / 4);

    FL_FLASH_PageErase(FLASH, FLASH_PROG_ADDR);
    memset(DataBuffer, 0x00, BUFFER_SIZE * 4);
    FL_FLASH_Read_Dma(FLASH, FLASH_PROG_ADDR, DataBuffer, FLASH_PAGE_SIZE / 4);
    memset(DataBuffer, 0xAA, BUFFER_SIZE * 4);
    FL_FLASH_Write_Dma(FLASH, FLASH_PROG_ADDR, DataBuffer);
    memset(DataBuffer, 0x00, BUFFER_SIZE * 4);
    FL_FLASH_Read_Dma(FLASH, FLASH_PROG_ADDR, DataBuffer, FLASH_PAGE_SIZE / 4);

    FL_FLASH_PageErase(FLASH, FLASH_PROG_ADDR);
}

#endif
