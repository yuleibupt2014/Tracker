
#include "DSP280x_Device.h"     // DSP280x Headerfile Include File
#include "DSP280x_Examples.h"   // DSP280x Examples Include File

void    SendData(Uint16 num,Uint16  data);
extern void InitSci(void);

unsigned int Sci_VarRx[100];
unsigned int i,j;
unsigned int Send_Flag;




/* void maissn(void)                                  */
/* {                                                  */
/*                                                    */
/*    InitSysCtrl();                                  */
/*                                                    */
/*      InitGpio();                                   */
/*                                                    */
/*  SetCs(1);                                         */
/*  SetCs(0);                                         */
/*  InitSci();                                        */
/*     DINT;                                          */
/*                                                    */
/*      InitPieCtrl();                                */
/*                                                    */
/*      IER = 0x0000;                                 */
/*      IFR = 0x0000;                                 */
/*                                                    */
/*      InitPieVectTable();                           */
/*  SendData(0,0xFF);                                 */
/*  SendData(1,0xFF);                                 */
/*  Send_Flag = 0;                                    */
/*     i = 0;                                         */
/*     j = 0;                                         */
/*                                                    */
/*  for(;;)                                           */
/*  {                                                 */
/*      if((SciaTx_Ready() == 1) && (Send_Flag == 1)) */
/*      {                                             */
/*          SciaRegs.SCITXBUF = Sci_VarRx[i];         */
/*          Send_Flag = 0;                            */
/*          i++;                                      */
/*          if(i == j)                                */
/*          {                                         */
/*              i = 0;                                */
/*              j = 0;                                */
/*          }                                         */
/*      }                                             */
/*                                                    */
/*                                                    */
/*      if(SciaRx_Ready() == 1)                       */
/*      {                                             */
/*          Sci_VarRx[j] = SciaRegs.SCIRXBUF.all;     */
/*          Send_Flag = 1;                            */
/*          j++;                                      */
/*          if(j == 100)                              */
/*          {                                         */
/*              j = 0;                                */
/*          }                                         */
/*      }                                             */
/*  }                                                 */
/*                                                    */
/* }                                                  */



//===========================================================================
// No more.
//===========================================================================


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
 
void GPS_InitSciaGpio()
{
   EALLOW;


    GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;    // Enable pull-up for GPIO28 (SCIRXDA)
    GpioCtrlRegs.GPAPUD.bit.GPIO29 = 0;    // Enable pull-up for GPIO29 (SCITXDA)


    GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;  // Asynch input GPIO28 (SCIRXDA)


    GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 1;   // Configure GPIO28 for SCIRXDA operation
    GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 1;   // Configure GPIO29 for SCITXDA operation
    
    EDIS;
}

void GPS_InitScibGpio()
{
   EALLOW;

//  GpioCtrlRegs.GPAPUD.bit.GPIO9 = 0;     // Enable pull-up for GPIO9  (SCITXDB)
//  GpioCtrlRegs.GPAPUD.bit.GPIO14 = 0;    // Enable pull-up for GPIO14 (SCITXDB)
    GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;    // Enable pull-up for GPIO18 (SCITXDB)
//  GpioCtrlRegs.GPAPUD.bit.GPIO22 = 0;    // Enable pull-up for GPIO22 (SCITXDB)

    
//  GpioCtrlRegs.GPAPUD.bit.GPIO11 = 0;    // Enable pull-up for GPIO11 (SCIRXDB)
//  GpioCtrlRegs.GPAPUD.bit.GPIO15 = 0;    // Enable pull-up for GPIO15 (SCIRXDB)
    GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;    // Enable pull-up for GPIO19 (SCIRXDB)
//  GpioCtrlRegs.GPAPUD.bit.GPIO23 = 0;    // Enable pull-up for GPIO23 (SCIRXDB)


//  GpioCtrlRegs.GPAQSEL1.bit.GPIO11 = 3;  // Asynch input GPIO11 (SCIRXDB)
//  GpioCtrlRegs.GPAQSEL1.bit.GPIO15 = 3;  // Asynch input GPIO15 (SCIRXDB)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3;  // Asynch input GPIO19 (SCIRXDB)
//  GpioCtrlRegs.GPAQSEL2.bit.GPIO23 = 3;  // Asynch input GPIO23 (SCIRXDB)


//  GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 2;    // Configure GPIO9 for SCITXDB operation
//  GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 2;   // Configure GPIO14 for SCITXDB operation
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 2;   // Configure GPIO18 for SCITXDB operation
//  GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 3;   // Configure GPIO22 for SCITXDB operation
    
//  GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 2;   // Configure GPIO11 for SCIRXDB operation
//  GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 2;   // Configure GPIO15 for SCIRXDB operation
    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 2;   // Configure GPIO19 for SCIRXDB operation
//  GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 3;   // Configure GPIO23 for SCIRXDB operation
    
    EDIS;
}

void GPS_InitSciGpio()
{
   GPS_InitSciaGpio();
#if DSP28_2808 || DSP28_2806   
   GPS_InitScibGpio();
#endif // if DSP28_2808 || DSP28_2806  
}

void GPS_Scia_init()
{    
    // Note: Clocks were turned on to the SCIA peripheral
    // in the InitSysCtrl() function
    
    SciaRegs.SCICCR.all =0x0007;   // 1 stop bit,  No loopback 
                                   // No parity,8 char bits,
                                   // async mode, idle-line protocol
    SciaRegs.SCICTL1.all =0x0003;  // enable TX, RX, internal SCICLK, 
                                   // Disable RX ERR, SLEEP, TXWAKE
    SciaRegs.SCICTL2.all =0x0003; 
    SciaRegs.SCIHBAUD    =0x0000;
    SciaRegs.SCILBAUD    =0x00A1;
    SciaRegs.SCICTL1.all =0x0023;     // Relinquish SCI from Reset 
}

//---------------------------------------------------------------------------
// InitSci: 
//---------------------------------------------------------------------------
// This function initializes the SCI(s) to a known state.
//
void GPS_InitSci(void)
{
    InitSci();
}   


int SciaTx_Ready1(void)
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

int SciaRx_Ready1(void)
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


//===========================================================================
// GPS_GetInfo
//===========================================================================
extern char GPSBuffer[256];
extern int32  GPSBufNum;
extern char GPSBufferFlag;
int GPS_GetInfo()
{
    if(GPSBufferFlag==0)
    {
        PieCtrlRegs.PIEIER9.bit.INTx1 = 1;//SCI_RX  
        return 0;
    }
    else
    {
        GPSBufferFlag = 0;
        return  GPSBufNum;
    }       
} 


//===========================================================================
// GPS_GetInfo
//===========================================================================
int GPS_Send(unsigned char dat)
{
    SciaRegs.SCITXBUF = dat;
    if(SciaRegs.SCICTL2.bit.TXRDY == 0)
        return 1;
    SciaRegs.SCITXBUF = dat;    
    return 0;
} 


volatile int gGpsOK = 0;
int gGPSReadFlag = 0;

//===========================================================================
// GPS_GetInfo
//===========================================================================
int  GPS_Test()
{
    int result = 0;

    for(;;) 
    {
        ServiceDog();
        result = GPS_GetInfo();

        if ( (result!=0) && ((GPSBuffer[17] == 'A') || (GPSBuffer[18] == 'A')))
        {
            return 0;
        }

    }
} 






