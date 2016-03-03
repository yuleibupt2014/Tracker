//
//###########################################################################
// Original Author: D.F.
//
// $TI Release: DSP280x V1.30 $
// $Release Date: February 10, 2006 $
//###########################################################################


#include "DSP280x_Device.h"     // DSP280x Headerfile Include File+
 

void EEPROM_InitI2CGpio()
{

    EALLOW;

    GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 1;   // Configure GPIO32 for SDAA operation
    GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 1;   // Configure GPIO33 for SCLA operation

    GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 3;  // Asynch input GPIO32 (SDAA)
    GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 3;  // Asynch input GPIO33 (SCLA)

    SysCtrlRegs.PCLKCR0.bit.I2CAENCLK = 1;  // Turn I2C module clock on

    EDIS;
}

void EEPROM_I2CA_Init(void)
{
    // Initialize I2C
    I2caRegs.I2CMDR.bit.IRS =0;
    I2caRegs.I2CSAR = 0x52;     // Slave address - EEPROM control code
    I2caRegs.I2CPSC.all = 9;            // Prescaler - need 7-12 Mhz on module clk
    I2caRegs.I2CCLKL = 3;           // NOTE: must be non zero
    I2caRegs.I2CCLKH = 3;           // NOTE: must be non zero
    I2caRegs.I2CIER.all = 0x00;     // Enable SCD & ARDY interrupts

    I2caRegs.I2CMDR.bit.IRS = 1;    // Take I2C out of reset
                                   // Stop I2C when suspended   
}


Uint16  EEPROM_I2C_xrdy()
{
    Uint16  t;
    t = I2caRegs.I2CSTR.bit.XRDY;
    return t;
}

Uint16  EEPROM_I2C_rrdy()
{
    Uint16  t;
    t = I2caRegs.I2CSTR.bit.RRDY;
    return t;
}

int32   gE2promWritenTime = 0;
int32   gE2promReadTime = 0;


Uint16 EEPROM_I2CA_WriteData(unsigned char *str,Uint16 RomAddress,Uint16 num)
{
    Uint16 i;
    Uint16 tmp;
	ServiceDog();
    if (I2caRegs.I2CSTR.bit.BB == 1)
    {
      return 1;
    }
    while(!EEPROM_I2C_xrdy());
    I2caRegs.I2CSAR = 0x52;
    I2caRegs.I2CCNT = num + 2;
    I2caRegs.I2CDXR = RomAddress>>8;
    I2caRegs.I2CMDR.all = 0x6E20;
    while(!EEPROM_I2C_xrdy());
    I2caRegs.I2CDXR = RomAddress%256;
    for (i=0; i<num; i++)
    {
      tmp = *str++;
      while(!EEPROM_I2C_xrdy());
      I2caRegs.I2CDXR = tmp & 0xFF;

      if (I2caRegs.I2CSTR.bit.NACK == 1)
              return    1;
    }    

    gE2promWritenTime++;
    return 0;   
}


Uint16 EEPROM_I2CA_ReadData(unsigned char *RamAddr, Uint16  RomAddress, Uint16 number)
{
    Uint16  i,Temp;
    ServiceDog();
    if (I2caRegs.I2CSTR.bit.BB == 1)
    {
       return 1;
    }
    while(!EEPROM_I2C_xrdy());
    I2caRegs.I2CSAR = 0x52;
    I2caRegs.I2CCNT = 2;
    I2caRegs.I2CDXR = RomAddress>>8;
    I2caRegs.I2CMDR.all = 0x6620; 
    while(!EEPROM_I2C_xrdy());
    I2caRegs.I2CDXR = RomAddress%256;
    if (I2caRegs.I2CSTR.bit.NACK == 1)
            return  1;
    //for(i=0;i<0xff;i++);      
    while(!EEPROM_I2C_xrdy());
    I2caRegs.I2CSAR = 0x52;
    I2caRegs.I2CCNT = number;    
    I2caRegs.I2CMDR.all = 0x6C20; 
    if (I2caRegs.I2CSTR.bit.NACK == 1)
            return  1;
    for(i=0;i<number;i++)
    {
      while(!EEPROM_I2C_rrdy());
          Temp = I2caRegs.I2CDRR;
      if (I2caRegs.I2CSTR.bit.NACK == 1)
              return    1;
          *RamAddr = Temp;
          RamAddr++;
    }

    gE2promReadTime++;
    return 0;
}


//===========================================================================
// No more.
//===========================================================================


//===========================================================================
// No more.
//===========================================================================
void eeprom_init()
{
    InitI2CGpio();
    I2CA_Init();
}

void eeprom_delay(unsigned long j)
{
    int i;
    for(i=0;i<j;i++);
}
 

//===========================================================================
//EEPROM操作说明
//板子上的EEPROM为32KBytes，地址从0到32767。
//EEPROM每64个字节为一个页面，多字节写操作时注意不能跨页写，
//即一次写操作不能跨越地址为64的整数倍的地址。
//每次读写操作之后需要延时，延时时间见eeprom_test函数
//===========================================================================
void eeprom_test(void)
{
    unsigned char temp_wr_buf[16]={0x93,0x79,0x95,0xAA,5,6,4,7,78,79,5};
    unsigned char temp_rd_buf[16]={0};
    unsigned int cc;
    for(cc=0;cc<16;cc++)
    {
        EEPROM_I2CA_WriteData(temp_wr_buf,64*15,16);
		ServiceDog();
        eeprom_delay(200000);
		ServiceDog();
        EEPROM_I2CA_ReadData(temp_rd_buf, 64*15 ,16);
		ServiceDog();
        eeprom_delay(200000);
		ServiceDog();
    }
    
    while(1);
}
