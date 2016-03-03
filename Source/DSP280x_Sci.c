// TI File $Revision: /main/2 $
// Checkin $Date: December 2, 2004   11:59:53 $
//###########################################################################
//
// FILE:    DSP280x_Sci.c
//
// TITLE:   DSP280x SCI Initialization & Support Functions.
//
//###########################################################################
// $TI Release: DSP280x V1.30 $
// $Release Date: February 10, 2006 $
//###########################################################################

#include "DSP280x_Device.h"     // DSP280x Headerfile Include File
#include "DSP280x_Examples.h"   // DSP280x Examples Include File

void Scia_init();
void Init_SCIB();
void DirFor485(void);
//---------------------------------------------------------------------------
// InitSci: 
//---------------------------------------------------------------------------
// This function initializes the SCI(s) to a known state.
//
void InitSci(void)
{
    InitSciaGpio();
    Scia_init();
}   


void InitSciGpio()
{
   InitSciaGpio();
#if DSP28_2808 || DSP28_2806   
   InitScibGpio();
#endif // if DSP28_2808 || DSP28_2806  
}
void SCIB_Init_For485()
{
	InitScibGpio();  //初始化IO
	Init_SCIB();    //初始化SCIB
	DirFor485();   //485选择接收使能
}
void DirFor485(void)
{
   EALLOW;
   GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 0;    // 485?????? 
   GpioCtrlRegs.GPADIR.bit.GPIO19 = 1;      //
   GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;      // 
   GpioDataRegs.GPADAT.bit.GPIO19 = 0;      //   0:选择接受使能   1:发送使能
   EDIS;
}

void InitSciaGpio()
{
   EALLOW;


    GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;    // Enable pull-up for GPIO28 (SCIRXDA)
    GpioCtrlRegs.GPAPUD.bit.GPIO29 = 0;    // Enable pull-up for GPIO29 (SCITXDA)


    GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;  // Asynch input GPIO28 (SCIRXDA)


    GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 1;   // Configure GPIO28 for SCIRXDA operation
    GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 1;   // Configure GPIO29 for SCITXDA operation
    
    EDIS;
}

void InitScibGpio()
{
   EALLOW;

//  GpioCtrlRegs.GPAPUD.bit.GPIO9 = 0;     // Enable pull-up for GPIO9  (SCITXDB)
//  GpioCtrlRegs.GPAPUD.bit.GPIO14 = 0;    // Enable pull-up for GPIO14 (SCITXDB)
//  GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;    // Enable pull-up for GPIO18 (SCITXDB)
    GpioCtrlRegs.GPAPUD.bit.GPIO22 = 0;    // Enable pull-up for GPIO22 (SCITXDB)

    
//  GpioCtrlRegs.GPAPUD.bit.GPIO11 = 0;    // Enable pull-up for GPIO11 (SCIRXDB)
//  GpioCtrlRegs.GPAPUD.bit.GPIO15 = 0;    // Enable pull-up for GPIO15 (SCIRXDB)
//  GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;    // Enable pull-up for GPIO19 (SCIRXDB)
    GpioCtrlRegs.GPAPUD.bit.GPIO23 = 0;    // Enable pull-up for GPIO23 (SCIRXDB)


//  GpioCtrlRegs.GPAQSEL1.bit.GPIO11 = 3;  // Asynch input GPIO11 (SCIRXDB)
//  GpioCtrlRegs.GPAQSEL1.bit.GPIO15 = 3;  // Asynch input GPIO15 (SCIRXDB)
//  GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3;  // Asynch input GPIO19 (SCIRXDB)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO23 = 3;  // Asynch input GPIO23 (SCIRXDB)


//  GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 2;    // Configure GPIO9 for SCITXDB operation
//  GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 2;   // Configure GPIO14 for SCITXDB operation
//  GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 2;   // Configure GPIO18 for SCITXDB operation
    GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 3;   // Configure GPIO22 for SCITXDB operation
    
//  GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 2;   // Configure GPIO11 for SCIRXDB operation
//  GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 2;   // Configure GPIO15 for SCIRXDB operation
//  GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 2;   // Configure GPIO19 for SCIRXDB operation
    GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 3;   // Configure GPIO23 for SCIRXDB operation
    
    EDIS;
}


void Scia_init()   //波特率约为9600，实际为9566.327
{    
    // Note: Clocks were turned on to the SCIA peripheral
    // in the InitSysCtrl() function
    EALLOW;
    
    SciaRegs.SCICCR.all =0x0007;   // 1 stop bit,  No loopback 
                                   // No parity,8 char bits,
                                   // async mode, idle-line protocol
    SciaRegs.SCICTL1.all =0x0003;  // enable TX, RX, internal SCICLK, 
                                   // Disable RX ERR, SLEEP, TXWAKE
    SciaRegs.SCICTL2.all =0x0003; 
    SciaRegs.SCIHBAUD    =0x0000;
    SciaRegs.SCILBAUD    =0x0061;   //原来是0x0061
    SciaRegs.SCICTL1.all =0x0023;     // Relinquish SCI from Reset 
    EDIS;
}
void Init_SCIB(void)
{
	EALLOW;
	//????SCI,???
    ScibRegs.SCICTL1.bit.SWRESET =0;
	//????????????????
	ScibRegs.SCICCR.bit.SCICHAR=7; // ????:8 bits
	ScibRegs.SCICCR.bit.PARITYENA=0;//????
	ScibRegs.SCICCR.bit.STOPBITS=0;//????1?
	ScibRegs.SCICCR.bit.ADDRIDLE_MODE=0;//???????

	//?????
   // ???:9600(LSPCLK = 7.5MHz);

    SciaRegs.SCIHBAUD    =0x0000;
    SciaRegs.SCILBAUD    =0x0061;
	//??SCI???????
	ScibRegs.SCICTL1.bit.RXENA=1;// ?????? 
	ScibRegs.SCICTL1.bit.TXENA=1;// ??????

	//????(??????PIE??CPU?),??????		
	ScibRegs.SCICTL2.bit.RXBKINTENA =1; // ??SCI????    
    //??????
   	ScibRegs.SCICTL1.bit.SWRESET =1; 
	EDIS;

	PieCtrlRegs.PIEIER9.bit.INTx3 = 1; //??PIE?? SCIB_RX_INT??     

}
int SciaTx_Ready(void)
{
    unsigned int i;
    if(SciaRegs.SCICTL2.bit.TXRDY == 1)
    {
        i = 1;
    }
    else
    {
        i = 0;
    }
    return(i);
}

/********************************************************************************
    name:   int SciaRx_Ready(void)
    input:  none
    output: i   1:  new data
            0:  none
*********************************************************************************/

int SciaRx_Ready(void)
{
    unsigned int i;
    if(SciaRegs.SCIRXST.bit.RXRDY == 1)
    {
        i = 1;
    }
    else
    {
        i = 0;
    }
    return(i);
}
    
//===========================================================================
// End of file.
//===========================================================================
