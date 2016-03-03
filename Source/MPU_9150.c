//###########################################################################
// FILE:    9150.c
//
// TITLE:   变量和外设初始化
//###########################################################################
#include "DSP280x_Device.h"     // DSP280x Headerfile Include File
//#include "Driver_I2C.h"

//****************************************
// 定义MPU6050内部地址
//****************************************
#define SMPLRT_DIV      0x19    //陀螺仪采样率，典型值：0x07(125Hz)
#define CONFIG          0x1A    //低通滤波频率，典型值：0x06(5Hz)
#define GYRO_CONFIG     0x1B    //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define ACCEL_CONFIG    0x1C    //加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define ACCEL_XOUT_H    0x3B
#define ACCEL_XOUT_L    0x3C
#define ACCEL_YOUT_H    0x3D
#define ACCEL_YOUT_L    0x3E
#define ACCEL_ZOUT_H    0x3F
#define ACCEL_ZOUT_L    0x40
#define TEMP_OUT_H      0x41
#define TEMP_OUT_L      0x42
#define GYRO_XOUT_H     0x43
#define GYRO_XOUT_L     0x44    
#define GYRO_YOUT_H     0x45
#define GYRO_YOUT_L     0x46
#define GYRO_ZOUT_H     0x47
#define GYRO_ZOUT_L     0x48
#define PWR_MGMT_1      0x6B    //电源管理，典型值：0x00(正常启用)
#define WHO_AM_I            0x75    //IIC地址寄存器(默认数值0x68，只读)
#define SlaveAddress    0xD0    //IIC写入时的地址字节数据，+1为读取

typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;

extern Uint16 I2CA_WriteData(Uint16 num,Uint16 RomAddress,unsigned char *str);
extern Uint16 I2CA_ReadData(unsigned char *RamAddr, Uint16  RomAddress, Uint16 number);
extern void board_i2c_init();
extern void eeprom_init();
unsigned char  MagFlag=0;

//**************************************
//向I2C设备写入一个字节数据
//**************************************
unsigned char  Single_WriteI2C(uchar REG_Address,uchar REG_data)
{
/*     I2C_Start();                  //起始信号            */
/*     I2C_SendByte(SlaveAddress);   //发送设备地址+写信号 */
/*     I2C_SendByte(REG_Address);    //内部寄存器地址，    */
/*     I2C_SendByte(REG_data);       //内部寄存器数据，    */
/*     I2C_Stop();                   //发送停止信号        */
    return I2CA_WriteData(1,REG_Address,&REG_data);

}

//**************************************
//从I2C设备读取一个字节数据
//**************************************
unsigned char Single_ReadI2C(uchar REG_Address,uchar *dat)
{
/*  uchar REG_data;                                              */
/*  I2C_Start();                   //起始信号                    */
/*  I2C_SendByte(SlaveAddress);    //发送设备地址+写信号         */
/*  I2C_SendByte(REG_Address);     //发送存储单元地址，从0开始   */
/*  I2C_Start();                   //起始信号                    */
/*  I2C_SendByte(SlaveAddress+1);  //发送设备地址+读信号         */
/*  REG_data=I2C_RecvByte();       //读出寄存器数据              */
/*  I2C_SendACK(1);                //接收应答信号                */
/*  I2C_Stop();                    //停止信号                    */
/*  return REG_data;                                             */
    return I2CA_ReadData(dat,REG_Address,1);
}

void MPU9150_Delay(unsigned int i)
{
    while(--i);
}
  
//**************************************
//初始化MPU6050
//**************************************
void InitMPU9150()
{
    Single_WriteI2C(0x31,0x0B); MPU9150_Delay(5000);   //测量范围,正负16g，13位模式
    Single_WriteI2C(0x2C,0x08);  MPU9150_Delay(5000);  //速率设定为12.5 参考pdf13页
    Single_WriteI2C(0x2D,0x08); MPU9150_Delay(5000);   //选择电源模式   参考pdf24页
    Single_WriteI2C(0x2E,0x80); MPU9150_Delay(5000);   //使能 DATA_READY 中断
    Single_WriteI2C(0x1E,0x00); MPU9150_Delay(5000);   //X 偏移量 根据测试传感器的状态写入pdf29页
    Single_WriteI2C(0x1F,0x00); MPU9150_Delay(5000);   //Y 偏移量 根据测试传感器的状态写入pdf29页
    Single_WriteI2C(0x20,0x05);  MPU9150_Delay(5000);  //Z 偏移量 根据测试传感器的状态写入pdf29页
}

//**************************************
//合成数据
//**************************************
unsigned char GetData(uchar REG_Address,short *dat)
{
    unsigned char  H=0,L=0;
    if(Single_ReadI2C(REG_Address+1,&H)!=0)
        return 1;
    MPU9150_Delay(50);
    if(Single_ReadI2C(REG_Address,&L)!=0)
        return 1;
    MPU9150_Delay(50);
    *dat =  (unsigned short)(H<<8)+L;   //合成数据
    return 0;
}
 
//**************************************
//初始化MPU6050
//**************************************
unsigned char MPU9150_GetAcc(unsigned char type,    short *val)
{
    unsigned char ret;
    switch(type)
    {
        case 0:
            ret =  GetData(0x32,val);
        break;
        case 1:
            ret =  GetData(0x34,val);
        break;
        case 2:
            ret =  GetData(0x36,val);
        break;
    }
    return ret;
}

//**************************************
//初始化MPU6050
//**************************************
unsigned char MPU9150_GetGyo(unsigned char type,    short *val)
{
    unsigned char ret;
    switch(type)
    {
        case 0:
            ret =  GetData(0x43,val);
        break;
        case 1:
            ret =  GetData(0x45,val);
        break;
        case 2:
            ret =  GetData(0x47,val);
        break;
    }
    return ret;
}

//**************************************
//初始化MPU6050
//**************************************
unsigned char MPU9150_GetMag(unsigned char type,    short *val)
{
    unsigned char ret,temp;

    MagFlag =0;
    Single_ReadI2C(0x37, &temp);
    MPU9150_Delay(50);
    Single_WriteI2C(0x37, temp | 0x02);
    MPU9150_Delay(500);
    MagFlag =1;
    Single_WriteI2C(0x0A, 0x01);
    MPU9150_Delay(50);
    temp = 0;
    while((temp&0x0001)!=0x0001)
    {
        Single_ReadI2C(0x02, &temp);
        MPU9150_Delay(50);
    }
    switch(type)
    {
        case 0:
            ret =  GetData(0x03,val);
        break;
        case 1:
            ret =  GetData(0x05,val);
        break;
        case 2:
            ret =  GetData(0x07,val);
        break;
    }
    MagFlag =0;
    *val = (*val) &0x8FFF;
    *val = ((((*val)>>8)&0x00FF) | (((*val)<<8)&0xFF00))&0x8FFF;
    MPU9150_Delay(50);
//  Single_ReadI2C(0x37, &temp);
//  MPU9150_Delay(50);
//  Single_WriteI2C(0x37, temp & 0xFD);
//  MPU9150_Delay(50);
    return ret;
}





#define BUFFSIZE   16
volatile short gaMPUData1_1;
short gaMPUData0[BUFFSIZE];
//short gaMPUData1[BUFFSIZE];
//short gaMPUData2[BUFFSIZE];

int   reading0 = 0;
int   reading1 = 0;
int   reading2 = 0;
int   max_reading = 4;

volatile int thr_h = 132;    ///!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//#define ControlInterval 64
#define CycleTimes  10

#define IntervalHex ((1 << CycleTimes) - 1)



volatile long sum=0;
volatile int li=0;
//**************************************
//初始化MPU6050
//**************************************
int MPU9150Test()
{
    short m=0,n=0;
   // long sum=0, sum1, sum2;
    short va1, va2, va3;
    int lj= 0; 
    int lk = 0;
    uchar a[3];
    int result = 0;
    int zvalue = 0;

    //Single_ReadI2C(0x00,a);
	li=0;
    sum = 0;
    
    for(;;)
    {
        if(MPU9150_GetAcc(0,&va1)==0)  
        {
     //       if((va1 >=0) && (va1 <= 265))    //排除奇异值
		//	{
            	sum += va1;
            	li++;
		//	}
        }
        if(li >= 2000)   //之前是1000，现在改成2000
        {
            sum=sum/2000;
           // gaMPUData0[reading0&(BUFFSIZE - 1)] = sum;
            //gaMPUData1_1 = gaMPUData0[reading0&(BUFFSIZE - 1)];
            gaMPUData1_1 = sum;
            sum = 0;
            li = 0;
			ServiceDog();
            return 0;
        }
		ServiceDog();
    }


}



