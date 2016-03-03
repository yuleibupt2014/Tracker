// TI File $Revision: /main/1 $
// Checkin $Date: December 1, 2004   11:11:34 $
//###########################################################################
//
// FILE:	DSP280x_CpuTimers.c
//
// TITLE:	CPU 32-bit Timers Initialization & Support Functions.
//
// NOTES:   CpuTimer1 and CpuTimer2 are reserved for use with DSP BIOS and
//          other realtime operating systems.  
//
//          Do not use these two timers in your application if you ever plan
//          on integrating DSP-BIOS or another realtime OS. 
//
//          For this reason, the code to manipulate these two timers is
//          commented out and not used in these examples.
//           
//###########################################################################
// $TI Release: DSP280x Header Files V1.60 $
// $Release Date: December 3, 2007 $
//###########################################################################

#include "DSP280x_Device.h"     // Headerfile Include File
#include "DC_Servo.h"
struct CPUTIMER_VARS CpuTimer0;

// CpuTimer 1 and CpuTimer2 are reserved for DSP BIOS & other RTOS
struct CPUTIMER_VARS CpuTimer1;
//struct CPUTIMER_VARS CpuTimer2;

//---------------------------------------------------------------------------
// InitCpuTimers: 
//---------------------------------------------------------------------------
// This function initializes all three CPU timers to a known state.
//
void InitCpuTimers(void)
{
    // CPU Timer 0
	// Initialize address pointers to respective timer registers:
	CpuTimer0.RegsAddr = &CpuTimer0Regs;
	// Initialize timer period to maximum:	
	CpuTimer0Regs.PRD.all  = 0xFFFFFFFF;
	// Initialize pre-scale counter to divide by 1 (SYSCLKOUT):	
	CpuTimer0Regs.TPR.all  = 0;
	CpuTimer0Regs.TPRH.all = 0;
	// Make sure timer is stopped:
	CpuTimer0Regs.TCR.bit.TSS = 1;
	// Reload all counter register with period value:
	CpuTimer0Regs.TCR.bit.TRB = 1;
	// Reset interrupt counters:
	CpuTimer0.InterruptCount = 0;	             	
	

    // CPU Timer 1
	CpuTimer1.RegsAddr = &CpuTimer1Regs;
	CpuTimer1Regs.PRD.all  = 0xFFFFFFFF;
	CpuTimer1Regs.TPR.all  = 0;
	CpuTimer1Regs.TPRH.all = 0;
	CpuTimer1Regs.TCR.bit.TSS = 1;
	CpuTimer1Regs.TCR.bit.TRB = 1;
	CpuTimer1.InterruptCount = 0;	             
	
	CpuTimer1Regs.TCR.bit.TSS = 0; // 开始定时器1 中断
}	
	
//---------------------------------------------------------------------------
// ConfigCpuTimer: 
//---------------------------------------------------------------------------
// This function initializes the selected timer to the period specified
// by the "Freq" and "Period" parameters. The "Freq" is entered as "MHz"
// and the period in "uSeconds". The timer is held in the stopped state
// after configuration.
//
void ConfigCpuTimer(struct CPUTIMER_VARS *Timer, float Freq, float Period)
{
	Uint32 	temp;
	
	// Initialize timer period:	
	Timer->CPUFreqInMHz = Freq;
	Timer->PeriodInUSec = Period;
	temp = (long) (Freq * Period);
	Timer->RegsAddr->PRD.all = temp;

	// Set pre-scale counter to divide by 1 (SYSCLKOUT):	
	Timer->RegsAddr->TPR.all  = 0;
	Timer->RegsAddr->TPRH.all  = 0;
	
	// Initialize timer control register:
	Timer->RegsAddr->TCR.bit.TSS = 1;      // 1 = Stop timer, 0 = Start/Restart Timer 
	Timer->RegsAddr->TCR.bit.TRB = 1;      // 1 = reload timer
	Timer->RegsAddr->TCR.bit.SOFT = 1;
	Timer->RegsAddr->TCR.bit.FREE = 1;     // Timer Free Run
	Timer->RegsAddr->TCR.bit.TIE = 1;      // 0 = Disable/ 1 = Enable Timer Interrupt
	
	// Reset interrupt counter:
	Timer->InterruptCount = 0;
}

//===========================================================================
// End of file.
//===========================================================================
