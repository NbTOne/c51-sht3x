#include "SHT3X.h"
#include "head.h"
int temp, humi;

void delay_ms(uint16 n)
{
		unsigned int i;

		unsigned char j;

		for(i=0;i<n;i++)
		{
			for(j=0;j<200;j++);

			for(j=0;j<102;j++);
		} 
	
}

void sendTempAndHumi(int temp,int humi){
	uint8 sendbuf[13],i;
	sendbuf[0]  = (temp / 10000) + '0';
	sendbuf[1]  = (temp % 10000 / 1000) + '0';
	sendbuf[2]  = (temp % 1000  / 100) + '0';
	sendbuf[3]  = (temp % 100   / 10) + '0';
	sendbuf[4]  = '.';
	sendbuf[5]  = (temp % 10) + '0';
	sendbuf[6]  = ':';
	sendbuf[7]  = humi/10000 + '0';
	sendbuf[8]  = humi%10000/1000 + '0';
	sendbuf[9]  = humi%1000/100+ '0';
	sendbuf[10]  = humi%100/10+ '0';
	sendbuf[11] = '.';
	sendbuf[12] = humi % 10 + '0';
	for(i=0;i<=12;i++){
		UART_Send_Byte(sendbuf[i]);
	}
//	UART_Send_Byte(humi%10);
	
}

void main(){
	SHT3XInit();
	UART_init();
//	Uart_Send_String("hkhkvv");
	while(1){
		SHT3X_XHGetTempAndHumi(&temp,&humi);
		sendTempAndHumi(temp,humi);
		delay_ms(2000);
	}
}
