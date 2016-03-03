// TI File $Revision: /main/4 $
// Checkin $Date: August 2, 2006   13:51:13 $
//###########################################################################
//
// FILE:   DSP280x_EPwm.c
//
// TITLE:  DSP280x ePWM Initialization & Support Functions.
//
//###########################################################################
// $TI Release: DSP280x Header Files V1.60 $
// $Release Date: December 3, 2007 $
//###########################################################################

#include "DSP280x_Device.h"     // DSP280x Headerfile Include File
#include "DC_Servo.h"

//---------------------------------------------------------------------------
// InitEPwm: 
//---------------------------------------------------------------------------
// This function initializes the ePWM(s) to a known state.
//
void InitEPwm(void)
{
    InitEPwmGpio();

}

//---------------------------------------------------------------------------
// Example: InitEPwmGpio: 
//---------------------------------------------------------------------------
// This function initializes GPIO pins to function as ePWM pins
//
// Each GPIO pin can be configured as a GPIO pin or up to 3 different
// peripheral functional pins. By default all pins come up as GPIO
// inputs after reset.  
// 

void InitEPwmGpio(void)
{
   InitEPwm1Gpio();
}

void InitEPwm1Gpio(void)
{

       int i;
       EALLOW;
   
       GpioCtrlRegs.GPAQSEL1.all = 0x0000;    // GPIO0-GPIO15 Synch to SYSCLKOUT 
       GpioCtrlRegs.GPAQSEL2.all = 0x0000;    // GPIO16-GPIO31 Synch to SYSCLKOUT
       GpioCtrlRegs.GPBQSEL1.all = 0x0000;    // GPIO32-GPIO34 Synch to SYSCLKOUT 
   
       //HHHHHHHHHHHHHHHHHHHHHHHHH
       GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0; // Set GPIO16 -> Normal I/O Func
       GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0; // Set GPIO16 -> Normal I/O Func
       GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0; // Set GPIO16 -> Normal I/O Func
       GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 0; // Set GPIO16 -> Normal I/O Func
       GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 0; // Set GPIO16 -> Normal I/O Func
       //VVVVVVVVVVVVVVVVVVVVVVVV
       GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0; // Set GPIO16 -> Normal I/O Func
       GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 0; // Set GPIO16 -> Normal I/O Func
       GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 0; // Set GPIO16 -> Normal I/O Func
       GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 0; // Set GPIO16 -> Normal I/O Func
       GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 0; // Set GPIO16 -> Normal I/O Func
       //HHHHHHHHHHHHHHHHHHHHHHHHH
       GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;  // Set GPIO16 -> Input Interface
       GpioCtrlRegs.GPADIR.bit.GPIO1 = 1;  // Set GPIO16 -> Input Interface
       GpioCtrlRegs.GPADIR.bit.GPIO2 = 1;  // Set GPIO16 -> Input Interface
       GpioCtrlRegs.GPADIR.bit.GPIO7 = 1;  // Set GPIO16 -> Input Interface
       GpioCtrlRegs.GPADIR.bit.GPIO8 = 1;  // Set GPIO16 -> Input Interface
       //VVVVVVVVVVVVVVVVVVVVVVVV
       GpioCtrlRegs.GPADIR.bit.GPIO3 = 1;  // Set GPIO16 -> Input Interface
       GpioCtrlRegs.GPADIR.bit.GPIO4 = 1;  // Set GPIO16 -> Input Interface
       GpioCtrlRegs.GPADIR.bit.GPIO5 = 1;  // Set GPIO16 -> Input Interface
       GpioCtrlRegs.GPADIR.bit.GPIO10 = 1;  // Set GPIO16 -> Input Interface
       GpioCtrlRegs.GPADIR.bit.GPIO11 = 1;  // Set GPIO16 -> Input Interface
       //HHHHHHHHHHHHHHHHHHHHHHHHH
       GpioDataRegs.GPADAT.bit.GPIO0 = 1; // disable port  
       GpioDataRegs.GPADAT.bit.GPIO1 = 1; // disable port  
       GpioDataRegs.GPADAT.bit.GPIO2 = 1; // disable port   =1 
       GpioDataRegs.GPADAT.bit.GPIO7 = 0; // disable port  
       GpioDataRegs.GPADAT.bit.GPIO8 = 0; // disable port  
       //VVVVVVVVVVVVVVVVVVVVVVVV
       GpioDataRegs.GPADAT.bit.GPIO3 = 1; // disable port  
       GpioDataRegs.GPADAT.bit.GPIO4 = 1; // disable port  
       GpioDataRegs.GPADAT.bit.GPIO5 = 1; // disable port   =1
       GpioDataRegs.GPADAT.bit.GPIO10 = 0; // disable port  
       GpioDataRegs.GPADAT.bit.GPIO11 = 0; // disable port  
    
       EDIS;
       while(0)
       {
           GpioDataRegs.GPADAT.bit.GPIO2 = 0; // disable port 
           GpioDataRegs.GPADAT.bit.GPIO5 = 0; // disable port
           for(i=0;i<1;i++)
           {
            // GpioDataRegs.GPADAT.bit.GPIO2 =1; // disable port    
            GpioDataRegs.GPADAT.bit.GPIO1 = 0; // disable port 
               GpioDataRegs.GPADAT.bit.GPIO4 = 0; // disable port
            delay_001(); 
             GpioDataRegs.GPADAT.bit.GPIO1 = 1; // disable port
               GpioDataRegs.GPADAT.bit.GPIO4 = 1; // disable port 
            delay_001();   
                  // GpioDataRegs.GPADAT.bit.GPIO2 =0; // disable port  
            }
                   
       //  GpioDataRegs.GPADAT.bit.GPIO2 = 1; // disable port  
           //  delay_003();
       }            //  delay_002();
               




}





//===========================================================================
// End of file.
//===========================================================================
