
//
//###########################################################################
// Original Author: D.F.
//
// $TI Release: DSP280x V1.30 $
// $Release Date: February 10, 2006 $
//###########################################################################


#include "DSP280x_Device.h"     // DSP280x Headerfile Include File+
 
extern MagFlag;
void InitI2CGpio()
{

    EALLOW;

    GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 1;   // Configure GPIO32 for SDAA operation
    GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 1;   // Configure GPIO33 for SCLA operation

    GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 3;  // Asynch input GPIO32 (SDAA)
    GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 3;  // Asynch input GPIO33 (SCLA)

    SysCtrlRegs.PCLKCR0.bit.I2CAENCLK = 1;  // Turn I2C module clock on

    EDIS;
}

void I2CA_Init(void)
{
    EALLOW;
    // Initialize I2C
    I2caRegs.I2CMDR.bit.IRS =0;
    I2caRegs.I2CSAR = MagFlag?0x53:0x53;        // Slave address - EEPROM control code
    I2caRegs.I2CPSC.all = 9;            // Prescaler - need 7-12 Mhz on module clk
    I2caRegs.I2CCLKL = 3;           // NOTE: must be non zero
    I2caRegs.I2CCLKH = 3;           // NOTE: must be non zero
    I2caRegs.I2CIER.all = 0x00;     // Enable SCD & ARDY interrupts

    I2caRegs.I2CMDR.bit.IRS = 1;    // Take I2C out of reset
                                   // Stop I2C when suspended   
    EDIS;
}


Uint16  I2C_xrdy()
{
    Uint16  t;
    t = I2caRegs.I2CSTR.bit.XRDY;
    return t;
}

Uint16  I2C_rrdy()
{
    Uint16  t;
    t = I2caRegs.I2CSTR.bit.RRDY;
    return t;
}

Uint16 I2CA_WriteData(Uint16 num,Uint16 RomAddress,unsigned char *str)
{
    Uint16 i;
	ServiceDog();
    if (I2caRegs.I2CSTR.bit.BB == 1)
    {
      return 1;
    }
    while(!I2C_xrdy());

    I2caRegs.I2CSAR = MagFlag?0x53:0x53;
    I2caRegs.I2CCNT = num + 1;
    I2caRegs.I2CDXR = RomAddress;
    I2caRegs.I2CMDR.all = 0x6E20;
    for (i=0; i<num; i++)
    {
      while(!I2C_xrdy());
      I2caRegs.I2CDXR = *str;
      str++;
      if (I2caRegs.I2CSTR.bit.NACK == 1)
              return    1;
    }       
    return 0;   
}

Uint16 I2CA_ReadData(unsigned char *RamAddr, Uint16 RomAddress, Uint16 number)
{
    Uint16  i,Temp;
	ServiceDog();
    if (I2caRegs.I2CSTR.bit.BB == 1)
    {
       return 1;
    }
    while(!I2C_xrdy());
    I2caRegs.I2CSAR = MagFlag?0x53:0x53;
    I2caRegs.I2CCNT = 1;
    I2caRegs.I2CDXR = RomAddress;
    I2caRegs.I2CMDR.all = 0x6620; 
    while(!I2C_xrdy());
//  I2caRegs.I2CDXR = RomAddress%256;
    if (I2caRegs.I2CSTR.bit.NACK == 1)
            return  1;
    //for(i=0;i<0xff;i++);      
    while(!I2C_xrdy());
    I2caRegs.I2CSAR = MagFlag?0x53:0x53;
    I2caRegs.I2CCNT = number;    
    I2caRegs.I2CMDR.all = 0x6C20; 
    if (I2caRegs.I2CSTR.bit.NACK == 1)
            return  1;
    for(i=0;i<number;i++)
    {
      while(!I2C_rrdy());
          Temp = I2caRegs.I2CDRR;
      if (I2caRegs.I2CSTR.bit.NACK == 1)
              return    1;
          *RamAddr = Temp;
          RamAddr++;
    }
    return 0;
}


//===========================================================================
// No more.
//===========================================================================


//===========================================================================
// No more.
//===========================================================================
void board_i2c_init()
{
    InitI2CGpio();
    I2CA_Init();
}
