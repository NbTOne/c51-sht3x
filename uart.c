#include"head.h"

/**
 * UART初始化
 * 波特率：4800
*/
void UART_init(void)
{

	
 SCON = 0x50;        // 10位uart，允许串行接受

    TMOD = 0x20;        // 定时器1工作在方式2（自动重装）
    TH1 = 0xFD;
    TL1 = 0xFD;

    TR1 = 1;
}

/**
 * UART发送一字节
*/
void UART_Send_Byte(uint8 dat)
{
	TI=0;
	SBUF = dat;
	while(!TI);
	TI = 0;

}

/*
 * UART 发送字符串
*/
void Uart_Send_String(uint8 *string)
{
	while(*string)
		{
			UART_Send_Byte(*string++);
		}
}
