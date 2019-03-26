#include"reg52.h"

typedef   unsigned char  uint8;
typedef   unsigned int   uint16;
 
#define uint unsigned int
#define uchar unsigned char
void UART_init(void);
void UART_Send_Byte(uint8 dat);
void Uart_Send_String(uint8 *string);