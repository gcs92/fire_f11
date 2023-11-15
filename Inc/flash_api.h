#ifndef FLASH_H
#define FLASH_H

#include <stdint.h>

#define FLASH_PROG_ADDR (0x00004000)
#define FLASH_SYSPARAM_ADDR (0xE000) //系统参数
#define FLASH_PAGE_SIZE (256)

typedef enum
{
    FLASH_PROGRAM_ID = 0,
    FLASH_SYSPARAM_ID,
    FLASH_ID_MAX,
} eFlash_ID;

int32_t flase_read(uint32_t id, uint8_t *buff, uint16_t len);
int32_t flash_write(uint32_t id, uint8_t *buff, uint16_t len);
#if 0
uint8_t Flash_Erase_Sector( uint16_t SectorNum );
uint8_t Flash_Write_String(uint32_t prog_addr, uint32_t *prog_data, uint16_t Len);
#endif
#endif
