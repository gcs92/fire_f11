#ifndef __DEBUG_H
#define __DEBUG_H
#include "fm33le0xx_fl.h"
#ifndef MFANG
#include <stdio.h>
#endif

/* DEBUG level */
#define DBG_INFO           1
#define DBG_ERROR           2
#define DBG_LOG             3

extern int8_t uoc_loglevel;


typedef struct _UART_RX_BUF
{	
	uint16_t	rxLen:14;				//接收缓冲区有效数据长度	
	uint16_t	rxFlag:2;				//接收完成标志 0表示缓冲区未接收 1-表示缓冲区按协议处理接收完成 2-表示缓冲区超时处理接收完成 3-表示缓冲区满接收完成
	uint8_t 	rxData[1];			//接收数据
}UART_RX_BUF;

/*高字节为发送灯,低字节为接收灯（每字节其中的低3位为IO组，高5位为IO引脚号）*/
/*初始化值为2,2,2,3 分别表接收灯IO组，接收灯IO脚，发送灯IO组，发送灯IO脚*/			
typedef struct _UART_CTRL
{	
	GPIO_Type *	rxLedGP;
	uint16_t		rxLedIO;	
	GPIO_Type * 	txLedGP;	
	uint16_t 		txLedIO;
}UART_CTRL;


typedef struct _UART_X
{
	UART_Type * 	uart;
	UART_CTRL	trCtrl;				//收发指示灯		
	
	uint8_t*	rxBuf;				//接收缓冲区	其中包括多次接收包，每个包前2字节为数据长度，后面rxMaxLen长度为数据区
	uint8_t*	txBuf;				//发送缓冲区		
	
	uint8_t		rxBufIndex:4;			//接收缓冲区当前索引
	uint8_t		rxBufCount:4;			//接收缓冲区个数
	
	uint16_t	rxMaxLen;			//接收缓冲区数据最大长度,但总长度＝rxMaxLen*rxBufCount
	uint16_t	txMaxLen;			//发送缓冲区数据最大长度
	
	uint16_t 	rx_timeout;							//字节接收超时
	uint16_t 	tx_interval;						//帧发送间隔
	
	volatile uint16_t	txLen;				//发送缓冲区有效数据长度	
	volatile uint16_t	txPos;				//发送缓冲区当前数据位置	
		
	volatile uint32_t	rxTickCount;	//最后接收计数
	volatile uint32_t	txTickCount;	//最后发送计数
	
}UART_X;

#define UART0_RX_COUNT		1
#define UART0_RX_MAXLEN		1
#define UART0_TX_MAXLEN		1
#define UART0_RB_TIMEOUT	3
#define UART0_TB_TIMEOUT	3


#define UART1_RX_COUNT		1
#define UART1_RX_MAXLEN		512
#define UART1_TX_MAXLEN		512
#define UART1_RB_TIMEOUT	5
#define UART1_TB_TIMEOUT	5


#define UART2_RX_COUNT		1
#define UART2_RX_MAXLEN		1
#define UART2_TX_MAXLEN		1
#define UART2_RB_TIMEOUT	2
#define UART2_TB_TIMEOUT	5

#define UART3_RX_COUNT		3
#define UART3_RX_MAXLEN		2048
#define UART3_TX_MAXLEN		2048
#define UART3_RB_TIMEOUT	2
#define UART3_TB_TIMEOUT	5

#define UART4_RX_COUNT		1
#define UART4_RX_MAXLEN		1
#define UART4_TX_MAXLEN		1
#define UART4_RB_TIMEOUT	5
#define UART4_TB_TIMEOUT	5

#define UART5_RX_COUNT		1
#define UART5_RX_MAXLEN		1024
#define UART5_TX_MAXLEN		1024
#define UART5_RB_TIMEOUT	2
#define UART5_TB_TIMEOUT	2

#define UART6_RX_COUNT		3
#define UART6_RX_MAXLEN		2048
#define UART6_TX_MAXLEN		2048
#define UART6_RB_TIMEOUT	2
#define UART6_TB_TIMEOUT	5

#define UART0_USED				0
#define UART1_USED				1
#define UART2_USED				0
#define UART3_USED				0//no
#define UART4_USED				0
#define UART5_USED				1
#define UART6_USED				0//no

#define DEBUG_BUF_SIZE 256


#define UART_Rx_Handler(index) \
			uint16_t offset = g_Uart[index]->rxBufIndex*(sizeof(uint16_t)+g_Uart[index]->rxMaxLen);\
			uint8_t ch;\
			UART_RX_BUF* uart_rx_buf	= (UART_RX_BUF*)&g_Uart[index]->rxBuf[offset];\
			if(uart_rx_buf->rxFlag == 0)\
			{\
				do\
				{\
					ch = (FL_UART_ReadRXBuff(g_Uart[index]->uart));\
					if(uart_rx_buf->rxLen < g_Uart[index]->rxMaxLen)\
					{\
						uart_rx_buf->rxData[uart_rx_buf->rxLen] = ch;\
						uart_rx_buf->rxLen++;\
						g_Uart[index]->rxTickCount = GetSysTickCount();\
					}\
				}while(0);\
			}\
			if(uart_rx_buf->rxFlag > 0)\
			{\
				ch = (FL_UART_ReadRXBuff(g_Uart[index]->uart));\
			}


uint16_t UART_RxFlag(uint8_t index);
uint16_t UART_RxGet(uint8_t index,void* buf,uint16_t len);
void UART_Tx(uint8_t index,void* buf,uint16_t len);
uint16_t MemCmp(void *s1, const void *s2, uint16_t n);
void *MemCpy(void *s1, const void *s2, uint16_t n);
void *MemSet(void *s1, uint8_t c, uint16_t n);
void dbg_printf(const char *fmt, ...);
void Debug_Display(void* buf,uint16_t len);
void dbg_array_buffer(char* msg,void* buf,uint16_t len);


#define dbg_log(lvl, fmt, ...)                \
    do                                                      \
    {                                                       \
        dbg_printf("%s",lvl);                         \
        dbg_printf(fmt, ##__VA_ARGS__);                     \
    }                                                       \
    while (0)


#if 0
#if (DBG_LVL >= DBG_LOG)
#define debug_log(fmt, ...)      dbg_log("log=>", fmt, ##__VA_ARGS__)
#else
#define debug_log(...)
#endif


#if (DBG_LVL >= DBG_ERROR)
#define debug_err(fmt, ...)      dbg_log("err=>", fmt, ##__VA_ARGS__)
#else
#define debug_err(...)
#endif
#endif
#define debug_info(fmt, ...)    if(uoc_loglevel >= DBG_INFO)  dbg_log("info=>", fmt, ##__VA_ARGS__)
#define debug_err(fmt, ...)    if(uoc_loglevel >= DBG_ERROR)  dbg_log("err=>", fmt, ##__VA_ARGS__)
#define debug_log(fmt, ...)    if(uoc_loglevel >= DBG_LOG)  dbg_log("log=>", fmt, ##__VA_ARGS__)

#endif
