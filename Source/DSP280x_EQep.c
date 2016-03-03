// TI File $Revision: /main/3 $
// Checkin $Date: August 2, 2006   13:51:11 $
//###########################################################################
//
// FILE:   DSP280x_EQep.c
//
// TITLE:  DSP280x eQEP Initialization & Support Functions.
//
//###########################################################################
// $TI Release: DSP280x Header Files V1.60 $
// $Release Date: December 3, 2007 $
//###########################################################################

#include "DSP280x_Device.h"     // DSP280x Headerfile Include File
#include "DC_Servo.h"
//---------------------------------------------------------------------------
// InitEQep: 
//---------------------------------------------------------------------------
// This function initializes the eQEP(s) to a known state.
//
void InitEQep(void)
{
	// Initialize eQEP1

	EQep1Regs.QUPRD=1000000;			// Unit Timer for 100Hz at 100 MHz SYSCLKOUT
	InitEQepGpio();

	EQep1Regs.QDECCTL.bit.QSRC=0;		// 正交脉冲计数模式
	
	EQep1Regs.QEPCTL.bit.FREE_SOFT=2;
	EQep1Regs.QEPCTL.bit.PCRM=01;		// 位置计数器在最大位置复位
	EQep1Regs.QEPCTL.bit.QCLM=1; 		//  当定时器的值等于QUPRD时锁存
	EQep1Regs.QEPCTL.bit.SWI = 1;		// 初始化位置计数器
	EQep1Regs.QEPCTL.bit.UTE=1; 		// 使能超时功能
	EQep1Regs.QEPCTL.bit.QPEN=1; 		// 使能位置计数器

	EQep1Regs.QPOSMAX= 680036; //680036=664*256*4+100+1

	EQep1Regs.QPOSCTL.bit.PCSHDW = 1;	// 位置比较控制，使能影子寄存器
	EQep1Regs.QPOSCTL.bit.PCLOAD = 1;   //当位置匹配时装载位置比较寄存器
	EQep1Regs.QPOSCTL.bit.PCE = 1;		// 使能位置比较电路

	EQep1Regs.QCAPCTL.bit.UPPS=0;   	// 位置计数器1分频
	EQep1Regs.QCAPCTL.bit.CCPS=7;		// CAP时钟对系统时钟128分频
	EQep1Regs.QCAPCTL.bit.CEN=1; 		// QEP Capture Enable

	EQep1Regs.QEINT.all = 0x0000;
	EQep1Regs.QEINT.bit.UTO = 1;		//使能超时中断

	EQep1Regs.QCLR.all = 0xFFFF;		// 清所有中断	
}

//---------------------------------------------------------------------------
// Example: InitEQepGpio: 
//---------------------------------------------------------------------------
// This function initializes GPIO pins to function as eQEP pins
//
// Each GPIO pin can be configured as a GPIO pin or up to 3 different
// peripheral functional pins. By default all pins come up as GPIO
// inputs after reset.  
// 
// Caution: 
// For each eQEP peripheral
// Only one GPIO pin should be enabled for EQEPxA operation. 
// Only one GPIO pin should be enabled for EQEPxB operation. 
// Only one GPIO pin should be enabled for EQEPxS operation. 
// Only one GPIO pin should be enabled for EQEPxI operation.
// Comment out other unwanted lines.

void InitEQepGpio()
{
	#if DSP28_EQEP1
   		InitEQep1Gpio();
	#endif  // endif DSP28_EQEP1
//#if DSP28_EQEP2     
//   InitEQep2Gpio();
//#endif // endif DSP28_EQEP2   
}

#if DSP28_EQEP1
void InitEQep1Gpio(void)
{
   EALLOW;
	
/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user. 
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAPUD.bit.GPIO20 = 0;   // Enable pull-up on GPIO20 (EQEP1A)
    GpioCtrlRegs.GPAPUD.bit.GPIO21 = 0;   // Enable pull-up on GPIO21 (EQEP1B)
    GpioCtrlRegs.GPAPUD.bit.GPIO22 = 0;   // Enable pull-up on GPIO22 (EQEP1S)
    GpioCtrlRegs.GPAPUD.bit.GPIO23 = 0;   // Enable pull-up on GPIO23 (EQEP1I)


// Inputs are synchronized to SYSCLKOUT by default.  
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAQSEL2.bit.GPIO20 = 0;   // Sync to SYSCLKOUT GPIO20 (EQEP1A)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO21 = 0;   // Sync to SYSCLKOUT GPIO21 (EQEP1B)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO22 = 0;   // Sync to SYSCLKOUT GPIO22 (EQEP1S)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO23 = 0;   // Sync to SYSCLKOUT GPIO23 (EQEP1I)

/* Configure eQEP-1 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be eQEP1 functional pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 1;   // Configure GPIO20 as EQEP1A
    GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 1;   // Configure GPIO21 as EQEP1B
    GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 1;   // Configure GPIO22 as EQEP1S
    GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 1;   // Configure GPIO23 as EQEP1I


    EDIS;
}
#endif // if DSP28_EQEP1



#if DSP28_EQEP2
void InitEQep2Gpio(void)
{
   EALLOW;
	
/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user. 
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAPUD.bit.GPIO24 = 0;    // Enable pull-up on GPIO24 (EQEP2A)
    GpioCtrlRegs.GPAPUD.bit.GPIO25 = 0;    // Enable pull-up on GPIO25 (EQEP2B)
    GpioCtrlRegs.GPAPUD.bit.GPIO26 = 0;    // Enable pull-up on GPIO26 (EQEP2I)
    GpioCtrlRegs.GPAPUD.bit.GPIO27 = 0;    // Enable pull-up on GPIO27 (EQEP2S)

// Inputs are synchronized to SYSCLKOUT by default.  
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAQSEL2.bit.GPIO24 = 0;  // Sync to SYSCLKOUT GPIO24 (EQEP2A)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO25 = 0;  // Sync to SYSCLKOUT GPIO25 (EQEP2B)               
    GpioCtrlRegs.GPAQSEL2.bit.GPIO26 = 0;  // Sync to SYSCLKOUT GPIO26 (EQEP2I)               
    GpioCtrlRegs.GPAQSEL2.bit.GPIO27 = 0;  // Sync to SYSCLKOUT GPIO27 (EQEP2S)               
                              
/* Configure eQEP-2 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be eQEP2 functional pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 2;   // Configure GPIO24 as EQEP2A
    GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 2;   // Configure GPIO25 as EQEP2B          
    GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 2;   // Configure GPIO26 as EQEP2I          
    GpioCtrlRegs.GPAMUX2.bit.GPIO27 = 2;   // Configure GPIO27 as EQEP2S          


    EDIS;
}
#endif // endif DSP28_EQEP2   




//===========================================================================
// End of file.
//===========================================================================
