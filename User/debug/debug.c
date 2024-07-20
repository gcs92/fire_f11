#include"debug.h"
#include"timer.h"
#include <stdarg.h>
#include"spi.h"

char  dbg_buf[DEBUG_BUF_SIZE];
int8_t uoc_loglevel = 0;


#if  UART0_USED > 0
uint8_t g_Uart0RxBuf[(sizeof(uint16_t)+UART0_RX_MAXLEN)*UART0_RX_COUNT]={0};
uint8_t g_Uart0TxBuf[UART0_TX_MAXLEN];
UART_X  g_Uart0 = {UART0,{0,0,0,0},g_Uart0RxBuf,g_Uart0TxBuf,0,UART0_RX_COUNT,
										UART0_RX_MAXLEN,UART0_TX_MAXLEN,UART0_RB_TIMEOUT,UART0_TB_TIMEOUT,0,0,0,0};
#endif


#if UART1_USED > 0
uint8_t g_Uart1RxBuf[(sizeof(uint16_t)+UART1_RX_MAXLEN)*UART1_RX_COUNT]={0};
uint8_t g_Uart1TxBuf[UART1_TX_MAXLEN];
UART_X  g_Uart1 = {UART1,{0,0,0,0},g_Uart1RxBuf,g_Uart1TxBuf,0,UART1_RX_COUNT,
										UART1_RX_MAXLEN,UART1_TX_MAXLEN,UART1_RB_TIMEOUT,UART1_TB_TIMEOUT,0,0,0,0};
#endif

#if UART2_USED > 0
uint8_t g_Uart2RxBuf[(sizeof(uint16_t)+UART2_RX_MAXLEN)*UART2_RX_COUNT]={0};
uint8_t g_Uart2TxBuf[UART2_TX_MAXLEN];
UART_X  g_Uart2 = {UART2,{0,0,0,0},g_Uart2RxBuf,g_Uart2TxBuf,0,UART2_RX_COUNT,
										UART2_RX_MAXLEN,UART2_TX_MAXLEN,UART2_RB_TIMEOUT,UART2_TB_TIMEOUT,0,0,0,0};
#endif

/*{GPIOC,GPIO_Pin_2,GPIOC,GPIO_Pin_2}  串口指示灯格式*/

#if UART4_USED >= 0
uint8_t g_Uart4RxBuf[(sizeof(uint16_t)+UART4_RX_MAXLEN)*UART4_RX_COUNT]={0};
uint8_t g_Uart4TxBuf[UART4_TX_MAXLEN];
UART_X  g_Uart4 = {UART4,{0,0,0,0},g_Uart4RxBuf,g_Uart4TxBuf,0,UART4_RX_COUNT,
										UART4_RX_MAXLEN,UART4_TX_MAXLEN,UART4_RB_TIMEOUT,UART4_TB_TIMEOUT,0,0,0,0};
#endif

#if UART5_USED > 0
uint8_t g_Uart5RxBuf[(sizeof(uint16_t)+UART5_RX_MAXLEN)*UART5_RX_COUNT]={0};
uint8_t g_Uart5TxBuf[UART5_TX_MAXLEN];
UART_X  g_Uart5 = {UART5,{0,0,0,0},g_Uart5RxBuf,g_Uart5TxBuf,0,UART5_RX_COUNT,
										UART5_RX_MAXLEN,UART5_TX_MAXLEN,UART5_RB_TIMEOUT,UART5_TB_TIMEOUT,0,0,0,0};
#endif

UART_X* g_Uart[7] = {
#if UART0_USED > 0	
		&g_Uart0,
#else	
		NULL,
#endif

#if UART1_USED > 0	
	&g_Uart1,
#else	
	NULL,
#endif
#if UART2_USED > 0	
	&g_Uart2,
#else	
	NULL,
#endif
#if UART3_USED > 0	
	NULL,
#else	
	NULL,
#endif
#if UART4_USED > 0	
	&g_Uart4,
#else	
	NULL,
#endif
#if UART5_USED > 0	
	&g_Uart5,
#else	
	NULL,
#endif
#if UART6_USED > 0	
	NULL,
#else	
	NULL,
#endif

};
uint16_t UART_RxGet(uint8_t index,void* buf,uint16_t len)
{
		uint8_t rxBufIndex;	
		uint16_t offset = 0;
		UART_RX_BUF* uart_rx_buf;
		if(g_Uart[index]->rxBufIndex >= g_Uart[index]->rxBufCount) g_Uart[index]->rxBufIndex = 0;
		for(rxBufIndex=g_Uart[index]->rxBufIndex;rxBufIndex < g_Uart[index]->rxBufCount;rxBufIndex++)
		{				
				offset = rxBufIndex*(sizeof(uint16_t)+g_Uart[index]->rxMaxLen);								
				uart_rx_buf	= (UART_RX_BUF*)&g_Uart[index]->rxBuf[offset];					
				//DEBUG_P(E_DEBUG,1,printf("UART%d UART_RxGet rxBufIndex=%d offset=%d rxlen=%d tick=%d\n",index,rxBufIndex,offset,uart_rx_buf->rxLen,GetSysTickCount()));	
				if(uart_rx_buf->rxFlag > 0 && uart_rx_buf->rxLen > 0)
				{
					if(len > uart_rx_buf->rxLen)
					{		
							len = uart_rx_buf->rxLen;
					}					
					
					MemCpy(buf,uart_rx_buf->rxData,len);
					uart_rx_buf->rxLen -= len;
					if(uart_rx_buf->rxLen == 0) uart_rx_buf->rxFlag = 0;			
					return len;					
				}								
		}			
		for(rxBufIndex=0;rxBufIndex < g_Uart[index]->rxBufIndex;rxBufIndex++)
		{				
				offset = rxBufIndex*(sizeof(uint16_t)+g_Uart[index]->rxMaxLen);								
				uart_rx_buf	= (UART_RX_BUF*)&g_Uart[index]->rxBuf[offset];					
				//DEBUG_P(E_DEBUG,1,printf("UART%d UART_RxGet rxBufIndex=%d offset=%d rxlen=%d tick=%d\n",index,rxBufIndex,offset,uart_rx_buf->rxLen,GetSysTickCount()));	
				if(uart_rx_buf->rxFlag > 0 && uart_rx_buf->rxLen > 0)
				{
					if(len > uart_rx_buf->rxLen)
					{		
							len = uart_rx_buf->rxLen;
					}
					
					MemCpy(buf,uart_rx_buf->rxData,len);
					uart_rx_buf->rxLen -= len;
					if(uart_rx_buf->rxLen == 0) uart_rx_buf->rxFlag = 0;	
					return len;					
				}								
		}				
		//DEBUG_P(E_DEBUG,1,printf("UART%d UART_RxGet(NO Data)\n",index));		
		return 0;
}
uint16_t UART_RxFlag(uint8_t index)
{		
		uint8_t rxBufIndex;	
		uint16_t offset = g_Uart[index]->rxBufIndex*(sizeof(uint16_t)+g_Uart[index]->rxMaxLen);
		UART_RX_BUF* uart_rx_buf	= (UART_RX_BUF*)&g_Uart[index]->rxBuf[offset];
	
		if(uart_rx_buf->rxFlag == 0 && uart_rx_buf->rxLen > 0 && 
			(GetSysTickCount() > (g_Uart[index]->rxTickCount + g_Uart[index]->rx_timeout)))
		{							
				uart_rx_buf->rxFlag = 2;	
				g_Uart[index]->rxBufIndex = (g_Uart[index]->rxBufIndex+1)%g_Uart[index]->rxBufCount;	

				//return uart_rx_buf->rxLen;
		}
		else if(uart_rx_buf->rxFlag == 0 && uart_rx_buf->rxLen >= g_Uart[index]->rxMaxLen)
		{								
				uart_rx_buf->rxFlag = 3;	
				g_Uart[index]->rxBufIndex = (g_Uart[index]->rxBufIndex+1)%g_Uart[index]->rxBufCount;
				//return uart_rx_buf->rxLen;
		}		
		else if(uart_rx_buf->rxFlag > 0 && uart_rx_buf->rxLen == 0)
		{
				uart_rx_buf->rxFlag = 0;	
		}
		
		for(rxBufIndex=0;rxBufIndex < g_Uart[index]->rxBufCount;rxBufIndex++)
		{				
				offset = rxBufIndex*(sizeof(uint16_t)+g_Uart[index]->rxMaxLen);								
				uart_rx_buf	= (UART_RX_BUF*)&g_Uart[index]->rxBuf[offset];									
				if(uart_rx_buf->rxFlag > 0)
				{	 
					return uart_rx_buf->rxLen;					
				}								
		}				
		return 0;
}


void UART1_IRQHandler(void)
{
    //接收中断处理
    if((FL_ENABLE == FL_UART_IsEnabledIT_RXBuffFull(UART1))
            && (FL_SET == FL_UART_IsActiveFlag_RXBuffFull(UART1)))
    {
        //中断转发接收到的数据
        UART_Rx_Handler(1);
    }
}

void UART5_IRQHandler(void)
{
    //接收中断处理
    if((FL_ENABLE == FL_UART_IsEnabledIT_RXBuffFull(UART5))
            && (FL_SET == FL_UART_IsActiveFlag_RXBuffFull(UART5)))
    {
        //中断转发接收到的数据
        UART_Rx_Handler(5);
    }
}
//两个字符串相等，则返回0，不相等则返回1
uint16_t MemCmp(void *s1, const void *s2, uint16_t n)
{
		uint16_t i;
    uint8_t* us1=(uint8_t*)s1;
    uint8_t* us2=(uint8_t*)s2;        
    for(i=0;i<n;i++)	
    {
        if(us1[i] != us2[i]) return 1;	       
    }
    return 0;
}

void *MemCpy(void *s1, const void *s2, uint16_t n)
{
		uint16_t i;
    uint8_t* us1=(uint8_t*)s1;
    uint8_t* us2=(uint8_t*)s2;         
    for(i=0;i<n;i++)	
    {
        us1[i] = us2[i];	       
    }
    return s1;
}

void *MemSet(void *s1, uint8_t c, uint16_t n)
{
		uint16_t i;
    uint8_t* us1=(uint8_t*)s1;       
    for(i=0;i<n;i++)	
    {        
        us1[i] = c;	       
    }
    return s1;
}

void SendOneByte(UART_Type *WrokCom, uint8_t b)
{
	
    FL_UART_WriteTXBuff(WrokCom, b);
	
    while(FL_SET != FL_UART_IsActiveFlag_TXBuffEmpty(WrokCom));       //等待发送完成
	for(int i=0;i<10;i++)
	{
		;
	}
}
void SendBuf(UART_Type *WrokCom, uint8_t *buf , uint32_t size)
{
    int i;
    for( i = 0 ; i < size; i++)
    {
        uint8_t data;
        data =  *( buf + i);
        SendOneByte( WrokCom , data);
    }
}

void UART_Tx(uint8_t index,void* buf,uint16_t len)
{
	while(GetSysTickCount() < (g_Uart[index]->txTickCount + g_Uart[index]->tx_interval));
	if(buf != NULL && len > 0){
		g_Uart[index]->txLen = len;
		MemCpy(g_Uart[index]->rxBuf,buf,len);
		SendBuf(g_Uart[index]->uart, g_Uart[index]->txBuf, g_Uart[index]->txLen);
		g_Uart[index]->txTickCount = GetSysTickCount();
	}
}

void dbg_printf(const char *fmt, ...)
{
    va_list args;//biancan
    uint32_t length;

    va_start(args, fmt);

    length = _vsnprintf(dbg_buf, sizeof(dbg_buf) - 1, fmt, args);
    if (length > DEBUG_BUF_SIZE - 1)
        length = DEBUG_BUF_SIZE - 1;

    SendBuf(UART1, (uint8_t *)dbg_buf, length);
	MemSet(dbg_buf, 0, sizeof(dbg_buf));
    va_end(args);
}

void dbg_array_buffer(char* msg,void* buf,uint16_t len)
{
	uint16_t i;	
	uint8_t* pbuf=(uint8_t*)buf;

    dbg_printf("%s: Len=%d Data=", msg,len);	
    for(i=0;i<len;i++)	
    {
			dbg_printf("%02X ", pbuf[i]);			
    }
	dbg_printf("\n");	
	return;
}

void Debug_Display(void* buf,uint16_t len)
{
	unsigned char debugbuf[128];
	MemCpy(debugbuf,buf,len-1);
	dbg_printf("debugbuf:%s, 长度:%d \n",debugbuf,len-1);
	if(MemCmp(debugbuf,"help",4) == 0)
	{
		dbg_printf("\t\t\t          	 控制命令提示v1.0 \n");
		dbg_printf("\t\t\t help				:显示命令信息 \n");
		dbg_printf("\t\t\t show info			:显示参数信息 \n");
		dbg_printf("\t\t\t reboot				:重新启动 \n");
		
	}  
	else if(MemCmp(debugbuf,"test",4)==0)
	{
		unsigned char num = debugbuf[5]-0x30;
		uint32_t data_num;
		dbg_printf("\t\t\t  num;%d \n",num);
		dbg_printf("\t\t\t  num;%08x \n",data_num);
		
	}
	else if(MemCmp(debugbuf,"show info",9) == 0)
	{
		dbg_printf("显示参数信息\n");
		//Show_Param_info();
	}
	else if(MemCmp(debugbuf,"set debug",9) == 0)
	{
		dbg_printf("打开debug显示\n");
		uoc_loglevel = debugbuf[10]-0x30;
		dbg_printf("set debug lovel:%d\n",uoc_loglevel);
		//Debugcontrol = 1;
	}
	else if(MemCmp(debugbuf,"close debug",11) == 0)
	{
		dbg_printf("关闭debug显示\n");
		//Debugcontrol = 0;
	}
	else if(MemCmp(debugbuf,"reboot",6) == 0)
	{
		dbg_printf("正在重启\n");
		Delay1Ms(200);//模块重启不得小于200MS
		NVIC_SystemReset();	
	}
	else
	{
		;
	}
	MemSet(debugbuf,'\0',128);
}
