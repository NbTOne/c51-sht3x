#include"head.h"

/**
 * UART��ʼ��
 * �����ʣ�4800
*/
void UART_init(void)
{

	
 SCON = 0x50;        // 10λuart�������н���

    TMOD = 0x20;        // ��ʱ��1�����ڷ�ʽ2���Զ���װ��
    TH1 = 0xFD;
    TL1 = 0xFD;

    TR1 = 1;
}

/**
 * UART����һ�ֽ�
*/
void UART_Send_Byte(uint8 dat)
{
	TI=0;
	SBUF = dat;
	while(!TI);
	TI = 0;

}

/*
 * UART �����ַ���
*/
void Uart_Send_String(uint8 *string)
{
	while(*string)
		{
			UART_Send_Byte(*string++);
		}
}
