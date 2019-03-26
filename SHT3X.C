
#include "SHT3X.H"

/*================================================================
【名 称】
【功 能】模拟IIC使用的引脚定义，注意要上拉电阻
【备 注】
【作 者】
【时 间】2010年8月18日11:24:09
================================================================*/
sbit SDA=P2^1;
sbit SCL=P2^0;

#define POLYNOMIAL  0x31 // P(x) = x^8 + x^5 + x^4 + 1 = 00110001

//==============================================================================
unsigned char SHT3X_CalcCrc(unsigned char *crcdata, unsigned char nbrOfBytes){
//==============================================================================
	unsigned char Bit;        // bit mask
  unsigned char crc = 0xFF; // calculated checksum
  unsigned char byteCtr;    // byte counter
  
  // calculates 8-Bit checksum with given polynomial
  for(byteCtr = 0; byteCtr < nbrOfBytes; byteCtr++)
  {
    crc ^= (crcdata[byteCtr]);
    for(Bit = 8; Bit > 0; --Bit)
    {
      if(crc & 0x80) crc = (crc << 1) ^ POLYNOMIAL;
      else           crc = (crc << 1);
    }
  }
	return crc;
}
///*================================================================
//【名 称】void Delay(unsigned int t)
//【功 能】延时长
//【备 注】
//【作 者】
//【时 间】2010年8月18日11:24:09
//================================================================*/
//void Delay(unsigned int t)
//{
//	unsigned char i;
//	while(t--)
//	{
//		for(i=0;i<200;i++);
//	}					   
//}
//void Delay1ms()		//@24.000MHz
//{
//	unsigned char i, j;

//	i = 24;
//	j = 85;
//	do
//	{
//		while (--j);
//	} while (--i);
//}

//***********************以下是模拟I2C函数****************************//
/*================================================================
【名 称】void I2CDelay (unsigned char t)
【功 能】模拟IIC用的短延时
【备 注】
【作 者】
【时 间】2010年8月18日11:24:09
================================================================*/
void I2CDelay (unsigned char t)
{
	while(t--);
}
/*================================================================
【名 称】void I2CInit(void)
【功 能】I2C初始化，空闲状态
【备 注】
【作 者】
【时 间】2010年8月18日11:24:09
================================================================*/
void I2CInit(void)
{
	SDA = 1; 
	SCL = 1;
}
/*================================================================
【名 称】void I2CStart(void)
【功 能】I2C起始信号
【备 注】SCL、SDA同为高，SDA跳变成低之后，SCL跳变成低
【作 者】
【时 间】2010年8月18日11:24:09
================================================================*/
void I2CStart(void)
{
  	SDA = 1;
  	SCL = 1;
  	I2CDelay(10);
  	SDA = 0;
  	I2CDelay(20);
  	SCL = 0;
  	I2CDelay(20);
}
/*================================================================
【名 称】void I2CStop(void)
【功 能】I2C停止信号
【备 注】SCL、SDA同为低，SCL跳变成高之后，SDA跳变成高
【作 者】
【时 间】2010年8月18日11:24:09
================================================================*/
void I2CStop(void)
{
  	SDA = 0;
  	SCL = 0;
	  I2CDelay(10);
  	SCL = 1;
  	I2CDelay(10);
  	SDA = 1;
  	I2CDelay(10);
}
/*================================================================
【名 称】unsigned char I2CWRByte(unsigned char WRByte)
【功 能】I2C写一个字节数据，返回ACK或者NACK
【备 注】从高到低，依次发送
【作 者】
【时 间】2010年8月18日11:24:09
================================================================*/
etError I2CWRByte(unsigned char WRByte)
{
	unsigned char i;
	SCL = 0;
	for(i=0;i<8;i++)
	{
		if(WRByte&0x80)
		{
			SDA = 1;	
		}
		else
		{
			SDA = 0;
		}
		I2CDelay(10);
		SCL=1;			//输出SDA稳定后，拉高SCL给出上升沿，从机检测到后进行数据采样
		I2CDelay(50);
		SCL=0;
		I2CDelay(10);
		WRByte <<= 1;
	} 
	SDA = 1;
	SCL = 1;
	I2CDelay(20);
	if(SDA==1)			//SDA为高，收到NACK
	{
		SCL=0;
		I2CDelay(50);
		return ACK_ERROR;	
	}
	else 				//SDA为低，收到ACK
	{
		SCL=0;
		I2CDelay(50);
		return NO_ERROR;
	}
}
/*================================================================
【名 称】unsigned char I2CRDByte(unsigned char AckValue)
【功 能】I2C读一个字节数据，入口参数用于控制应答状态，ACK或者NACK
【备 注】从高到低，依次接收
【作 者】
【时 间】2010年8月18日11:24:09
================================================================*/
unsigned char I2CRDByte(unsigned char AckValue)
{
	unsigned char i,RDByte=0;
	SCL = 0;
	SDA = 1;			//释放总线	
	for (i=0;i<8;i++) 
	{
		RDByte <<= 1;	//移位
		SCL = 1;		//给出上升沿
		I2CDelay(30);	//延时等待信号稳定
		if(SDA==1) 		//采样获取数据
		{
			RDByte |= 0x01;
		}
		else
		{
			RDByte &= 0xfe;
		}
		SCL = 0;   		//下降沿，从机给出下一位值
		I2CDelay(10);
	}   
	SDA =  AckValue;	//应答状态
	I2CDelay(10);
	SCL = 1;                         
  	I2CDelay(50);          
  	SCL = 0;
  	SDA = 1;                
  	I2CDelay(1);
	return RDByte;
}
etError SHT3X_WriteCommand(etCommands cmd){
//==============================================================================
  etError error; // error code
  // write the upper 8 bits of the command to the sensor
  error  = I2CWRByte(cmd >> 8);
  // write the lower 8 bits of the command to the sensor
  error |= I2CWRByte(cmd & 0xFF);
	
  return error;
}
/*================================================================
【名 称】void SHT3XInit(void)
【功 能】SHT3X初始化函数，主函数中调用
【备 注】从高到低，依次接收
【作 者】
【时 间】2010年8月18日11:24:09
================================================================*/
void SHT3XInit(void)
{

	I2CInit();//I2C初始化
	I2CStart();
  I2CWRByte(SHT3XWriteHeader);
	SHT3X_WriteCommand(CMD_CLEAR_STATUS); //
	I2CStop();
	
	
  I2CStart();
  I2CWRByte(SHT3XWriteHeader);
	SHT3X_WriteCommand(CMD_MEAS_PERI_1_H); //1秒高精度循环测量模式
	I2CStop();
	
	
}




//==============================================================================循环读取模式
etError SHT3X_XHGetTempAndHumi(int        *temp,
                             int         *humi){
//==============================================================================
  etError error;           // error code
  unsigned long int    rawValueTemp;    // temperature raw value from sensor
  unsigned long int    rawValueHumi;    // humidity raw value from sensor
	unsigned char Rdata[6]={0};
  unsigned char i;																			 
  I2CStart();
  error  = I2CWRByte(SHT3XWriteHeader);
  // if no error ...
  if(error == NO_ERROR)
  {
    // start measurement in polling mode
    // use depending on the required repeatability, the corresponding command
     error = SHT3X_WriteCommand(CMD_FETCH_DATA);
  }
  // if no error, wait until measurement ready
  if(error == NO_ERROR)
  {
	 	I2CStart();
     error = I2CWRByte(SHT3XReadHeader);
      // if measurement has finished -> exit loop
  }
	// if no error, read temperature and humidity raw values
  if(error == NO_ERROR)
  {
		for(i=0;i<5;i++)
		{
			Rdata[i] = I2CRDByte(ACK);
		}
    Rdata[i] = I2CRDByte(NACK);
		I2CStop();
		if(Rdata[2]!=SHT3X_CalcCrc(Rdata,2))     error = CHECKSUM_ERROR;
	  if(Rdata[5]!=SHT3X_CalcCrc(&Rdata[3],2)) error = CHECKSUM_ERROR;	
  }
  // if no error, calculate temperature in  and humidity in %RH
  if(error == NO_ERROR)
  {
		rawValueTemp =(Rdata[0] << 8) | Rdata[1];
		rawValueHumi =(Rdata[3] << 8) | Rdata[4];
    *temp =(int)( 1750 *rawValueTemp / 65535 - 450);
    *humi =(int)(1000 *rawValueHumi / 65535);
  }
  return error;
}




