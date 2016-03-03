
#include "DSP280x_Device.h"     // DSP280x Headerfile Include File
#include "DC_Servo.h"
#include "IQmathLib.h"
#include "pid_reg.h"
#include "math.h"
#include "MotorCtrl.h"
interrupt void INT13_ISR(void)     // INT13 or CPU-Timer1
{

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1; 
//  CpuTimer1Regs.TCR.bit.TSS = 1; //stop定时器1中断
//  EPwm1Regs.TBCTL.bit.CTRMODE = 0;
//  EPwm1Regs.AQCSFRC.bit.CSFA = 0;

//  PieCtrlRegs.PIEACK.all = PIEACK_GROUP1; 
}

// Note CPU-Timer2 is reserved for TI use.
interrupt void INT14_ISR(void)     // CPU-Timer2
{
   // Insert ISR Code here
  
  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

interrupt void DATALOG_ISR(void)   // Datalogging interrupt
{
   // Insert ISR Code here
 
   // Next two lines for debug only to halt the processor here
   // Remove after inserting ISR Code
   asm ("      ESTOP0");
   for(;;);
}

interrupt void RTOSINT_ISR(void)   // RTOS interrupt
{
  // Insert ISR Code here

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

interrupt void EMUINT_ISR(void)    // Emulation interrupt
{
  // Insert ISR Code here

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

interrupt void NMI_ISR(void)       // Non-maskable interrupt
{
  // Insert ISR Code here

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

interrupt void ILLEGAL_ISR(void)   // Illegal operation TRAP
{
  // Insert ISR Code here

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm("          ESTOP0");
  for(;;);

}


interrupt void USER1_ISR(void)     // User Defined trap 1
{
  // Insert ISR Code here

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);

}

interrupt void USER2_ISR(void)     // User Defined trap 2
{
  // Insert ISR Code here

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);


}

interrupt void USER3_ISR(void)     // User Defined trap 3
{
  // Insert ISR Code here

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

interrupt void USER4_ISR(void)     // User Defined trap 4
{
  // Insert ISR Code here

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

interrupt void USER5_ISR(void)     // User Defined trap 5
{
  // Insert ISR Code here

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

interrupt void USER6_ISR(void)     // User Defined trap 6
{
  // Insert ISR Code here

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

interrupt void USER7_ISR(void)     // User Defined trap 7
{
  // Insert ISR Code here

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

interrupt void USER8_ISR(void)     // User Defined trap 8
{
  // Insert ISR Code here

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

interrupt void USER9_ISR(void)     // User Defined trap 9
{
  // Insert ISR Code here

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

interrupt void USER10_ISR(void)    // User Defined trap 10
{
  // Insert ISR Code here

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

interrupt void USER11_ISR(void)    // User Defined trap 11
{
  // Insert ISR Code here

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

interrupt void USER12_ISR(void)     // User Defined trap 12
{
 // Insert ISR Code here
 
  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);

}

// -----------------------------------------------------------
// PIE Group 1 - MUXed into CPU INT1
// -----------------------------------------------------------

// INT1.1 
interrupt void SEQ1INT_ISR(void)   //SEQ1 ADC
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code

  asm ("      ESTOP0");
  for(;;);

}     

// INT1.2 
interrupt void SEQ2INT_ISR(void)  //SEQ2 ADC
{

  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  
  asm("   ESTOP0");
  for(;;);
    
}
// INT1.3 - Reserved

// INT1.4
interrupt void  XINT1_ISR(void)
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);

}     

// INT1.5
interrupt void  XINT2_ISR(void)
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);

}

// INT1.6


int32 gawADCAvg[4];
int32 gAdcIsrTimes = 0;
int32 gReadTimes = 0;

int32 gawPsdV[128][4];

#define CycleTimes  8

#define IntervalHex ((1 << CycleTimes) - 1)


int32 GetSensorValue(volatile Uint16* pReg)
{
    Uint16   pResultReg;

    pResultReg = *pReg;

    return (int32)(pResultReg >> 4);
}

void findmaxmin(int32*    pwADCData, int len, int32* p_min, int32* p_max)
{
    int loop;
    int32 minvalue, maxvalue;

    minvalue = 0xFFFF;
    maxvalue = 0;
    
    for (loop = 0; loop < len; loop++)
    {
        if (pwADCData[loop] < minvalue)
        {
            minvalue = pwADCData[loop];
        }

        if (pwADCData[loop] > maxvalue)
        {
            maxvalue = pwADCData[loop];
        }
    }
    *p_min = minvalue;
    *p_max = maxvalue;

}



void CalcXYValue()
{
    int32   vsum, vx, vy;

    vsum = gstPsd.x2 + gstPsd.x1 + gstPsd.y1 + gstPsd.y2;       
    vx   = gstPsd.x2 - gstPsd.x1 + gstPsd.y1 - gstPsd.y2;
    vy   = gstPsd.x2 - gstPsd.x1 + gstPsd.y2 - gstPsd.y1;

    gXvalue = vx*10000/vsum;
    gYvalue = vy*10000/vsum;
}

interrupt void ADCINT_ISR(void)     // ADC
{

    volatile Uint16 *pResultReg;
    int32   readOfst;
    
  // Insert ISR Code here
    IFR=0x0000;

    DINT;
    EALLOW;
    PieCtrlRegs.PIEIFR1.all = 0;
    PieCtrlRegs.PIEACK.all=0xFFFF;
  // To recieve more interrupts from this PIE group, acknowledge this interrupt 
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;         //响应中断  
    EDIS;
    
//-------------------------------AD采样处理-----------------------------//

    if (0 == (gAdcIsrTimes & IntervalHex))
    {
        for (readOfst = 0; readOfst < 3; readOfst++)
        {
            gawADCAvg[readOfst] = 0;
        }
    }
    
    pResultReg = &AdcRegs.ADCRESULT2;

    // 读入10个寄存器的数值到gauwADCAvg
    for (readOfst = 0; readOfst < 4; readOfst++)
    {
        gawADCAvg[readOfst] += GetSensorValue(pResultReg++);
        //gawADCAvg[readOfst] += GetSensorValue(pResultReg++);
    }
    
    gAdcIsrTimes++;


    if (0 == (gAdcIsrTimes & IntervalHex))
    {
        for (readOfst = 0; readOfst < 4; readOfst++)
        {
            gawADCAvg[readOfst] = gawADCAvg[readOfst] >> (CycleTimes);
            gPsdValue[readOfst] = gawADCAvg[readOfst];
            //gawPsdV[gReadTimes&127][readOfst] = gPsdValue[readOfst];
            PSDValueJudge(gPsdValue);
            CalcXYValue();
        }
        gReadTimes++;
    }

    AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;           //复位SEQ1
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;         //清除INT SEQ1位
    EINT;
    return; 
}




int32 MIN_THRESHLOD_FOR_PSD = 1600;
#define MAX_PSD_VALUE    3500
extern Uint16 SensorType;
int32 gvsum;
// return  value ;  0; Sucess  other ; fail 
int PSDValueJudge(int32*    pwADCData)
{
    int32   x1;
    int32   x2;
    int32   y1;
    int32   y2;
    int32   vsum, vx, vy;
    int32   max, min;
#ifdef PARANS
    x1 = pwADCData[3];   // 5
    x2 = pwADCData[2];   // 4
    y1 = pwADCData[1];   // 3
    y2 = pwADCData[0];   // 2


#else
    x1 = pwADCData[2];  // 4
    x2 = pwADCData[0];  // 2
    y1 = pwADCData[3];  // 5
    y2 = pwADCData[1];  // 3


#endif

    gstPsd.x1 = (int32)MAX_PSD_VALUE - x1;
    gstPsd.x2 = (int32)MAX_PSD_VALUE - x2;
    gstPsd.y1 = (int32)MAX_PSD_VALUE - y1;
    gstPsd.y2 = (int32)MAX_PSD_VALUE - y2;
    
    vsum = gstPsd.x2 + gstPsd.x1 + gstPsd.y1 + gstPsd.y2;       
    vx   = gstPsd.x2 - gstPsd.x1 + gstPsd.y1 - gstPsd.y2;
    vy   = gstPsd.x2 - gstPsd.x1 + gstPsd.y2 - gstPsd.y1;
    findmaxmin((int32*)&gstPsd,4,&min,&max);
    if (min < 0)
    {
        gXvalue = 0;
    }
    

    if (SensorType == 0x5050)
    {
        if ( vsum > MIN_THRESHLOD_FOR_PSD )   //   小  孔 模 式  ,当前用的是小孔模式
        {        
             return 0;  //  0: 
        }
    }
    else
    {
        if (   (x1 < MIN_THRESHLOD_FOR_PSD)   //MIN_THRESHLOD_FOR_PSD = 1800;
            && (x2 < MIN_THRESHLOD_FOR_PSD) 
            && (y1 < MIN_THRESHLOD_FOR_PSD)
            && (y2 < MIN_THRESHLOD_FOR_PSD))
        {
            findmaxmin(pwADCData,4,&min,&max);
        
            if (((gXvalue < 200) && (gXvalue > -200)) && ((gYvalue < 200) && (gYvalue > -200)))
            {
                return 0;
            }
            
        
            return 0;
        }
    }

    return -1;
}


// INT1.7

#define MAX_STEP   80000
#define MIN_STEP   (0-MAX_STEP)


extern unsigned char GetSwitchValue();

extern int PSDCtl(int32 * pwADCData);



int32 gOffset = 0;
int gDirection = 0;


int gAutoReset = 1;
int32   gStepCount = 0;
int32   gLostTime  = 0;
volatile unsigned char gswitchon = 0;
int FirstUpdategCurrentstepFlag = 0;
extern enCtrlState gCtrlState;

interrupt void  TINT0_ISR(void)      // CPU-Timer 0
{ 
    int result = 0;
    enDIRECTION   enEW, enNS;
    DINT;
    AdcRegs.ADCTRL2.bit.RST_SEQ1=1;
    AdcRegs.ADCTRL2.bit.SOC_SEQ1=1; 


    // To recieve more interrupts from this PIE group, acknowledge this interrupt 
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1; 
    // Next two lines for debug only - remove after inserting
    // ISR Code
    CpuTimer0Regs.TCR.bit.TIF=1;//clear interrupt flag

	gswitchon = 0;
    gswitchon = GetSwitchValue();//检查限位器传感器的读数状态
    if(gswitchon==1 &&  gCtrlState == NIGHT && FirstUpdategCurrentstepFlag == 0)   //这里是自己添加的
	{
		gDirection=1;
		gOffset=13357;//15534;
		gCurrentStep = 13357;//15534;
		FirstUpdategCurrentstepFlag = 1;	
	}
	/*
	if(gswitchon==1)
	{
		gCurrentStep=gCurrentStep;
	}
	*/
	if(gCtrlState != NIGHT)
	{
		FirstUpdategCurrentstepFlag = 0;
	}	

    if (gCounters1 < (int32)30000000L)
        gCounters1++;
    else
        gCounters1 = 0;

    

    if (gCounters < 64)
    {
        gCounters++;
    }
    else
    {
//	if(gswitchon==0)
	 {
        enEW = INVALIDDIRECT;
        enNS = INVALIDDIRECT;   
        if (gManualCtrl == 0)
        {
            MotorCtrl(&enEW, &enNS);  // 获取垂直和水平的方向
        }
        else if (gManualCtrl == 1)
        {
            AutoReset(&enEW, &enNS);
        }
        else
        {
            ManualCtrl(&enEW, &enNS);
        }
        HoriContrl(enEW);             ////注意这里才是电机控制的最终函数点，以上只是指示控制方向而已
        VeriContrl(enNS);
        gCounters = 0;
	 }

    }
    EINT;
    return; 
}


// INT1.8
interrupt void  WAKEINT_ISR(void)    // WD, LOW Power
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP1; 
  
  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}


// -----------------------------------------------------------
// PIE Group 2 - MUXed into CPU INT2
// -----------------------------------------------------------

// INT2.1
interrupt void EPWM1_TZINT_ISR(void)    // EPWM-1
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT2.2
interrupt void EPWM2_TZINT_ISR(void)    // EPWM-2
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT2.3
interrupt void EPWM3_TZINT_ISR(void)    // EPWM-3
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;
  
  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}


// INT2.4
interrupt void EPWM4_TZINT_ISR(void)    // EPWM-4
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;
  
  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}


// INT2.5
interrupt void EPWM5_TZINT_ISR(void)    // EPWM-5
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT2.6
interrupt void EPWM6_TZINT_ISR(void)   // EPWM-6
{
  // Insert ISR Code here

   
  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;
  
  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT2.7 - Reserved
// INT2.8 - Reserved

// -----------------------------------------------------------
// PIE Group 3 - MUXed into CPU INT3
// -----------------------------------------------------------
   
// INT 3.1         
interrupt void EPWM1_INT_ISR(void)     // EPWM-1
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT3.2
interrupt void EPWM2_INT_ISR(void)     // EPWM-2
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT3.3
interrupt void EPWM3_INT_ISR(void)    // EPWM-3
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT3.4
interrupt void EPWM4_INT_ISR(void)    // EPWM-4
{
  // Insert ISR Code here
  
  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;  

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT3.5
interrupt void EPWM5_INT_ISR(void)    // EPWM-5
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;  

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT3.6
interrupt void EPWM6_INT_ISR(void)    // EPWM-6
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT3.7 - Reserved
// INT3.8 - Reserved


// -----------------------------------------------------------
// PIE Group 4 - MUXed into CPU INT4
// -----------------------------------------------------------

// INT 4.1
interrupt void ECAP1_INT_ISR(void)    // ECAP-1
{
  // Insert ISR Code here
  
  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;  

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT4.2
interrupt void ECAP2_INT_ISR(void)    // ECAP-2
{
  // Insert ISR Code here
  
  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT4.3
interrupt void ECAP3_INT_ISR(void)    // ECAP-3
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;
  
  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT4.4
interrupt void ECAP4_INT_ISR(void)     // ECAP-4
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT4.5 - Reserved
// INT4.6 - Reserved
// INT4.7 - Reserved
// INT4.8 - Reserved

// -----------------------------------------------------------
// PIE Group 5 - MUXed into CPU INT5
// -----------------------------------------------------------

// INT 5.1

interrupt void EQEP1_INT_ISR(void)    // EQEP-1
{

  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP5;
 
  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT5.2
interrupt void EQEP2_INT_ISR(void)    // EQEP-2
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP5;
 
  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT5.3 - Reserved
// INT5.4 - Reserved
// INT5.5 - Reserved
// INT5.6 - Reserved
// INT5.7 - Reserved
// INT5.8 - Reserved

// -----------------------------------------------------------
// PIE Group 6 - MUXed into CPU INT6
// -----------------------------------------------------------

// INT6.1
interrupt void SPIRXINTA_ISR(void)    // SPI-A
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT6.2
interrupt void SPITXINTA_ISR(void)     // SPI-A
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT6.3 
interrupt void SPIRXINTB_ISR(void)     // SPI-B
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT6.4
interrupt void SPITXINTB_ISR(void)     // SPI-B
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT6.5
interrupt void SPIRXINTC_ISR(void)     // SPI-C
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT6.6
interrupt void SPITXINTC_ISR(void)     // SPI-C
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT6.7
interrupt void SPIRXINTD_ISR(void)     // SPI-D
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT6.8
interrupt void SPITXINTD_ISR(void)     // SPI-D
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}


// -----------------------------------------------------------
// PIE Group 7 - MUXed into CPU INT7
// -----------------------------------------------------------

// INT7.1 - Reserved
// INT7.2 - Reserved
// INT7.3 - Reserved
// INT7.4 - Reserved
// INT7.5 - Reserved
// INT7.6 - Reserved
// INT7.7 - Reserved
// INT7.8 - Reserved

// -----------------------------------------------------------
// PIE Group 8 - MUXed into CPU INT8
// -----------------------------------------------------------

// INT8.1
interrupt void I2CINT1A_ISR(void)     // I2C-A
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT8.2
interrupt void I2CINT2A_ISR(void)     // I2C-A
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT8.3 - Reserved
// INT8.4 - Reserved
// INT8.5 - Reserved
// INT8.6 - Reserved
// INT8.7 - Reserved
// INT8.8 - Reserved


// -----------------------------------------------------------
// PIE Group 9 - MUXed into CPU INT9
// -----------------------------------------------------------

// INT9.1
char GPSBuffer[256];// = "$GPRMC,024813.640,A,3158.4608,N,11848.3737,E,10.05,324.27,150706,,,A*50";
int32 GPSIndex=0,GPSBufNum=0;
char GPSBufferFlag=0;

//  $GPRMC
interrupt void SCIRXINTA_ISR(void)     // SCI-A
{
    unsigned char dat;
    dat = SciaRegs.SCIRXBUF.all;
    if(GPSIndex>0)
    {
        GPSBuffer[GPSIndex++]= dat; 
        if(GPSIndex==6)
        {
            if((GPSBuffer[3]!='R') ||  (GPSBuffer[4]!='M') || (GPSBuffer[5]!='C'))
                GPSIndex =0;                
        }
        if(dat==0x0D)
        {
            PieCtrlRegs.PIEIER9.bit.INTx1 = 0;//SCI_RX  
            GPSBufNum = GPSIndex;
            GPSIndex = 0;
            GPSBufferFlag = 1;
        }
    }
    if(dat == '$' )   //GPRMC //GPGGA
    {
        GPSIndex = 0;
        GPSBuffer[GPSIndex++]= dat;
    }
}

// INT9.2
interrupt void SCITXINTA_ISR(void)     // SCI-A
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);

}


// INT9.3
//CRC 校验生成查询表
static const unsigned int  Crc1021Table[16] = 
{
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef
};


unsigned short GetCRC(unsigned char *pData, unsigned int nLength)
{
    unsigned short crc=0;
    unsigned char da;

    while((nLength--)!=0) 
    {   
        da=((unsigned char)(crc/256))/16;      // 暂存CRC的高四位 
        crc<<=4;                               // CRC右移4位，相当于取CRC的低12位）
        crc^=Crc1021Table[da^((*pData)/16)];   // CRC的高4位和本字节的前半字节相加后查表计算CRC，
                                               // 然后加上上一次CRC的余数 
        da=((unsigned char)(crc/256))/16;      // 暂存CRC的高4位 
        crc<<=4;                               // CRC右移4位,相当于CRC的低12位） 
        crc^=Crc1021Table[da^((*pData)&0x0f)]; // CRC的高4位和本字节的后半字节相加后查表计算 
        pData++;
    }
    return crc;
}



unsigned char rdata[16]={0};
unsigned char rdata_tmp[16]={0}; 

Uint16 SCI_RFlag=0;
Uint16 RX_crc;
unsigned char TX_crc;
unsigned int receiveddata=0;
interrupt void SCIRXINTB_ISR(void)                                      // SCI-B  接收
{
 	DINT;
   	receiveddata=ScibRegs.SCIRXBUF.all;
	PieCtrlRegs.PIEACK.all |= BIT9;   //????
	EINT;   
	/**************************************************
     0x0f:开机信号
	 0xf0：关机信号
	 0xcc：上下班时间设定信号

	****************************************************/
 	/*********CRC校验***********
    Uint16 i;
    Uint16 crch;
    for(i=0;i<16;i++)
    {
        rdata[i] = ScibRegs.SCIRXBUF.all;
//      rdata_tmp[i] = rdata[i];    
    }
    crch = rdata[14]*256;
    RX_crc =  crch + rdata[15];
    if (!SCI_RFlag) 
    {
        if((rdata[0] == 0x5D) && (rdata[2] == 0x00))// && (GetCRC(&rdata[0],14) == RX_crc)) //判断帧头
        {
            SCI_RFlag = 1;                                              //正确接收到一个数据包
        }
        else
        {
            SCI_RFlag = 0;
        }
    }

    rdata[2] = 0x01;                                                    //回发
    TX_crc = GetCRC(&rdata[0],14);
    for(i=0;i<14;i++)
    {
        ScibRegs.SCITXBUF = rdata[i];
    }
    ScibRegs.SCITXBUF = TX_crc / 0x100;     //CRC校验码  高8位
    ScibRegs.SCITXBUF = TX_crc & 0xFF;      //CRC校验码  低8位

    ScibRegs.SCIFFRX.bit.RXFFOVRCLR=1;   // Clear Overflow flag
    ScibRegs.SCIFFRX.bit.RXFFINTCLR=1;   // Clear Interrupt flag

    PieCtrlRegs.PIEACK.all|=0x100;       // Issue PIE ack
	*///////////////////////////////////////////////////////////////////////
}

// INT9.4
interrupt void SCITXINTB_ISR(void)     // SCI-B
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);

}

// INT9.5
interrupt void ECAN0INTA_ISR(void)  // eCAN-A
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);

}

// INT9.6
interrupt void ECAN1INTA_ISR(void)  // eCAN-A
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);

}

// INT9.7
interrupt void ECAN0INTB_ISR(void)  // eCAN-B
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);

}

// INT9.8
interrupt void ECAN1INTB_ISR(void)  // eCAN-B
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt 
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);

}

// -----------------------------------------------------------
// PIE Group 10 - MUXed into CPU INT10
// -----------------------------------------------------------

// INT10.1 - Reserved
// INT10.2 - Reserved
// INT10.3 - Reserved
// INT10.4 - Reserved
// INT10.5 - Reserved
// INT10.6 - Reserved
// INT10.7 - Reserved
// INT10.8 - Reserved


// -----------------------------------------------------------
// PIE Group 11 - MUXed into CPU INT11
// -----------------------------------------------------------

// INT11.1 - Reserved
// INT11.2 - Reserved
// INT11.3 - Reserved
// INT11.4 - Reserved
// INT11.5 - Reserved
// INT11.6 - Reserved
// INT11.7 - Reserved
// INT11.8 - Reserved

// -----------------------------------------------------------
// PIE Group 12 - MUXed into CPU INT12
// -----------------------------------------------------------

// INT12.1 - Reserved
// INT12.2 - Reserved
// INT12.3 - Reserved
// INT12.4 - Reserved
// INT12.5 - Reserved
// INT12.6 - Reserved
// INT12.7 - Reserved
// INT12.8 - Reserved

//---------------------------------------------------------------------------
// Catch All Default ISRs: 
//

interrupt void EMPTY_ISR(void)  // Empty ISR - only does a return.
{

}

interrupt void PIE_RESERVED(void)  // Reserved space.  For test.
{
  asm ("      ESTOP0");
  for(;;);
}

interrupt void rsvd_ISR(void)      // For test
{
  asm ("      ESTOP0");
  for(;;);
}

//===========================================================================
// End of file.
//===========================================================================

