#include "MotorCtrl.h"
#include "DC_Servo.h"

#define FiveMMPSD
//#define NineMMPSD
/****************俞磊的头定义***************/
//#define yulei_ReadLearnedMotorSpeedInfoTable //如果定义，则从E2PROM读取自学习的数组值
//#define yulei_gCurrentStepTable
//#define LearnMotorSpeedTwiceOneMonthdefine     //如果定义，则每个月会读取自学习记录值并算平均值
#define yulei_WriteLearnedMotorSpeedInfo     //如果定义，则写入自学习的值到E2PROM
#define WriteE2PROMEvery8Min                 //如果定义，则精调时每8分钟（2度）写入一次到E2PROM
/*******************************************/
/**********俞磊增加的变量***********/
Uint16 PreviousGotogGpsOK4_Time=0;    //先前进入精调的时间
int32 PreviousCurrentStep=0;          //先前进入精调的脉冲数
int32 PreviousCurrentFangweijiao=0;   //先前进入精调的方位角
LearnedMotorSpeedInformation LearnedMotorSpeedInfo;//结构体，自学习的脉冲数(int32),年和月(Uint8),日(Uint8),时(Uint8),分(Uint8)
Uint16 EarlyTime=0;   //记录先前进入精调的时间
Uint16 EarTime=0;
int32 ABCfangweijiao=0;
extern Uint16 ShutDownflag;
/************************************/
volatile int32   gManualCtrl = 0;
volatile Uint16   ggpioflag = 1;
volatile Uint16   ggpioflag1 = 1;
volatile int32   gCounters = 0;
volatile int32   gCounters1 = 0;
volatile int32   grunningtime = 0;
volatile int32   grunningtime1 = 0;


volatile int32  gTestVertical = 0;
volatile int32  gTestHorizont = 0;






stPsdValue gstPsd;


int32   gPsdValue[4];
volatile int32   gXvalue = 0;
volatile int32   gYvalue = 0;



 int32   gTargetStep;
 int32   gTargetAccumValue;
volatile int32   gCurrentStep = 0;
volatile int32   myCurrentStep = 0;
volatile int32   gBakStep = 0;


GPS gGpsinfo;

enCtrlState gCtrlState = IDLE;
SunInfo gSunInfoCurr;
SunInfo gSunInfoBak;
SunInfo gSunInfoTest;
int32   gMotorSpeed = 237;

int32   gNightFlag = 1;

int     gAdjustFlag = 0;
int32     gDelta = 0;

Uint16 SensorType = 0xAAAA;
factory_setting gFactorySetting;
int32 GPS_TEST_OFF = 0;
int32 gTime2work = 5*60;
int32 gTime2Bed  = (14*60);
int RealTime = 0;
trace_record gTraceRecord;//[128];
step_record gStateRecord;
Err_Record   gErrRecord;
int32  gErrNum = 0;
int32  gSearchTime = 0;
void IntializeStepRecord(void);
void ReadLearnedMotorSpeedInfo(void);
void RecordRead(Uint16* pOutPut, int num, Uint32 startaddr, int offset, int size);
void LearnMotorSpeedTwiceOneMonth();
void ReadMotorSpeed();
int32 Cmp2Thr(int32 data, int32 threshold)
{
    return ( ( data >= threshold )  ? SUCCESS : FAIL);
}


extern unsigned char GetSwitchValue();










/*--------------------------------------
Name:           motor_control()
Description:    step motor control function
Input par:      
Output par:     
---------------------------------------*/

 void SetHorizontalDirection(enDIRECTION enDirection)
{
    EALLOW;
    //enDirection = (int) enDirection;
    if (INVALIDDIRECT == enDirection)
    {
        GpioDataRegs.GPADAT.bit.GPIO2 = BIT_DISABLE;
        return;
    }
    GpioDataRegs.GPADAT.bit.GPIO0 = (enDirection & 1);  // =0时正转向，=1时反转向; 
    asm(" RPT #14 || NOP");  
    GpioDataRegs.GPADAT.bit.GPIO2 = BIT_ENABLE; // enable port    
    asm(" RPT #14 || NOP");    
    EDIS;

}


 void SetVerticalDirection(enDIRECTION enDirection)
{
     EALLOW;
    //enDirection = (int) enDirection;
    if (INVALIDDIRECT == enDirection)
    {
        GpioDataRegs.GPADAT.bit.GPIO5 = BIT_DISABLE;
        return;
    }
    GpioDataRegs.GPADAT.bit.GPIO3 = (enDirection & 1);  // =0时正转向，=1时反转向; 
    asm(" RPT #14 || NOP");  
    GpioDataRegs.GPADAT.bit.GPIO5 = BIT_ENABLE;     // enable port
    asm(" RPT #14 || NOP");  
    EDIS;

}


/**
* @ fn :  SetHorizontalSpeed
*
*
*
*/
 void SetVerticalSpeed(enAdjustSpeed s)
{
     switch(s) {
     case SLOWEST:          
            EALLOW;
            GpioDataRegs.GPADAT.bit.GPIO10 = 1;
            GpioDataRegs.GPADAT.bit.GPIO11 = 1;
            EDIS;
            break;
     case SLOWER:
            EALLOW;
            GpioDataRegs.GPADAT.bit.GPIO10 = 0;
            GpioDataRegs.GPADAT.bit.GPIO11 = 1;
            EDIS;
            break;
     case FASTER:
             EALLOW;
            GpioDataRegs.GPADAT.bit.GPIO10 = 1;
            GpioDataRegs.GPADAT.bit.GPIO11 = 0;
            EDIS;
            break;
     case FASTEST:
             EALLOW;
            GpioDataRegs.GPADAT.bit.GPIO10 = 0;
            GpioDataRegs.GPADAT.bit.GPIO11 = 0;
            EDIS;
            break;
    }
    return;
}


/**
* @ fn :  SetHorizontalSpeed
*
*
*
*/
 void SetHorizontalSpeed(enAdjustSpeed s)
{
    switch(s) 
    {
    case SLOWEST:
        EALLOW;
        GpioDataRegs.GPADAT.bit.GPIO7 = 1;
        GpioDataRegs.GPADAT.bit.GPIO8 = 1;
        EDIS;
        break;
    case SLOWER:
        EALLOW;
        GpioDataRegs.GPADAT.bit.GPIO7 = 0;
        GpioDataRegs.GPADAT.bit.GPIO8 = 1;
        EDIS;
        break;
    case FASTER:
        EALLOW;
        GpioDataRegs.GPADAT.bit.GPIO7 = 1;
        GpioDataRegs.GPADAT.bit.GPIO8 = 0;
        EDIS;
        break;
    case FASTEST:
        EALLOW;
        GpioDataRegs.GPADAT.bit.GPIO7 = 0;
        GpioDataRegs.GPADAT.bit.GPIO8 = 0;
        EDIS;
        break;
    default: 
        break;
    }
    return;
}





//  gEWValue , gNSValue  阳光垂直玻璃面时候光斑参考点
int32 gEWValue = 0;//849;//4000; 500
int32 gNSValue = 40;//-650;//-1000; -130
int32 MIN_DIFF_PSD_Y = 50;
int32 MIN_DIFF_PSD = 100;


int PSDCtl(int32*    pwADCData)
{
    enDIRECTION   directionEW = INVALIDDIRECT;   // 初始值为无效值，0:  正传，1: 反转,  其他值无效
    enAdjustSpeed adjustSpeed = INVALIDSPEED;   // 0: approx adjust; 1: precise adjust; other value: invalid.
    enDIRECTION   directionSN = INVALIDDIRECT;   // 初始值为无效值，0:  正传，1: 反转,  其他值无效

    int32   vsum = 100;
    int32   vx = 0;
    int32   vy = 0;

    int32   x1;
    int32   x2;
    int32   y1;
    int32   y2;

    
    
    if ((gXvalue - gEWValue) > MIN_DIFF_PSD)
    {
        // turn east
        directionEW = TURNEAST;
    }
    else if ((gXvalue - gEWValue) < (0-MIN_DIFF_PSD))
    {
        // turn west
        directionEW = TURNWEST;
    }
    else
    {
        directionEW = INVALIDDIRECT;
        // stay
    }
    
    if ((gYvalue - gNSValue) > MIN_DIFF_PSD_Y)
    {
        // turn north
        directionSN = TURNSOUTH;//TURNSOUTH;
    }
    else if ((gYvalue - gNSValue) < (0-MIN_DIFF_PSD_Y))
    {
        // turn south 
        directionSN = TURNNORTH;// TURNNORTH;
    }
    else
    {
        // stay
        directionSN = INVALIDDIRECT;
    }
    


    return 0;
}

extern int32 gMotorSpeed;
extern SunInfo gSunInfoCurr;
Uint16 gAutoResetFirstFlag = 0;
void AutoReset(enDIRECTION* p_EwDirect, enDIRECTION* p_NsDirect)
{

    enDIRECTION     directionEW, directionSN;

    if (gAutoResetFirstFlag == 0)
    {
        if (gCurrentStep > 0)
        {
            gOffset = gCurrentStep;
            gDirection = 1;
        }
        else if (gCurrentStep < 0)
        {
            gOffset = 0- gCurrentStep;
            gDirection = 2;
        }
        else
        {
            gOffset = 0;
            gDirection = 0;
            gAutoReset = 2;
        }
        gAutoResetFirstFlag = 1;
    }


    if (gOffset > 0)
    {
        if (gDirection == 1)
            directionEW = TURNEAST;
        else if (gDirection == 2)
            directionEW = TURNWEST;
        else
            directionEW = INVALIDDIRECT;
        
        gOffset--;
    }

    directionSN = INVALIDDIRECT;
    *p_EwDirect = directionEW;
    *p_NsDirect = directionSN;

}


int gspeed = 0xFF;
void ManualCtrl(enDIRECTION* p_EwDirect, enDIRECTION* p_NsDirect)
{
    int32 result; 


    char direction[2];

    enAdjustSpeed   adjustSpeed;
    enDIRECTION     directionEW, directionSN;


    direction[0] = gTestVertical;
    direction[1] = gTestHorizont;

    if (gTestVertical == 1)
    {
        directionSN = TURNSOUTH;
    }
    else if (gTestVertical == 2)
    {
        directionSN = TURNNORTH;
    }
    else
    {
        directionSN = INVALIDDIRECT;
    }

    if (gTestHorizont == 1)
    {
        directionEW = TURNEAST;
    }
    else if (gTestHorizont == 2)
    {
        directionEW = TURNWEST;
    }
    else
    {
        directionEW = INVALIDDIRECT;
    }


    *p_EwDirect = directionEW;
    *p_NsDirect = directionSN;

}


  
enDIRECTION  VerticalCtrl(int inputdata, int th_h, int th_l)
{
    enDIRECTION     directionSN;

    if (inputdata > th_h)
    {
        directionSN = TURNSOUTH;  //1号机：TURNSOUTH;
    }
    else if (inputdata < th_l)
    {
        directionSN = TURNNORTH;  //1号机：TURNNORTH;
    }
    else
    {
        directionSN = INVALIDDIRECT;
    }
    


    return directionSN;
    
}








void VeriContrl(enDIRECTION directionSN)
{
 
    if (directionSN != INVALIDDIRECT)
    {
        ggpioflag1 = ~ggpioflag1;
        EALLOW;
        GpioDataRegs.GPADAT.bit.GPIO4 = ggpioflag1&1;    // 0 /1  ;   
        EDIS;
        grunningtime1++;
    }
    SetVerticalDirection(directionSN);

}

void HoriContrl(enDIRECTION directionEW)
{

    if (directionEW != INVALIDDIRECT)
    {
        ggpioflag = ~ggpioflag;
        EALLOW;
        GpioDataRegs.GPADAT.bit.GPIO1 = ggpioflag&1;    // 0 /1  ;   
        EDIS;
        gCurrentStep += (directionEW == TURNEAST)? -1 : 1;
        myCurrentStep += (directionEW == TURNEAST)? -1 : 1;
    }
    SetHorizontalDirection(directionEW);
}


volatile int32    gSteadCount = 0;
volatile int32    gSteadCount0 = 0;

volatile Uint16   gInitialFlag = 0;
///////////////////////////////电机控制延时
volatile long    DelayCount0_ForMotorControl = 0;
volatile long    DelayCount1_ForMotorControl = 0;

///////////////////////////////
  
int MotorCtrl(enDIRECTION* p_EwDirect, enDIRECTION* p_NsDirect)
{
    enDIRECTION   enEwDir = INVALIDDIRECT;
    enDIRECTION   enNsDir = INVALIDDIRECT;
    int result;
    int psd_judge = 0;
	int a=0;
	int loop = 0;
    psd_judge = PSDValueJudge(gPsdValue); //  0: sucess   other: fail 
	/*
	if(GpioDataRegs.GPADAT.bit.GPIO17==1 && gCtrlState == PSD_CTRL)
	{
		if(ShutDownflag==1)   //避免在已关灯的情况下多次按关灯
			;
		else
		{
			ShutDownflag=1;
			gGpsOK=6;
			gOffset=2500;
		}	
	}
	if ( gCtrlState == PSD_CTRL&&ShutDownflag==1 && gGpsOK==6) 		// 关灯处理
	{
		if(gOffset>0)
		{
			enEwDir = TURNEAST;
            gOffset--;
			*p_EwDirect = enEwDir;
    		*p_NsDirect = INVALIDDIRECT;
		}
        else
		{
			*p_EwDirect = INVALIDDIRECT;
			*p_NsDirect = INVALIDDIRECT;
			gGpsOK=5;
			gCtrlState=SEMI_SYN;  //关灯完毕，进入乌云模式
		}
		return;
	}
	*/

    if ((gGpsOK != 4) && (gGpsOK != 0))  //  gGpsOK 0: idle  2: vertical 3: horizon, 4: PSD ctrl   5: lost
    {
		

        if (psd_judge == 0)
        {
            gSteadCount++;   //  进入精调的次数
        }
        else
        {
            gSteadCount = 0;
        }

        if (gSteadCount > 50) //  
        {
            gGpsOK = 4;   //   状态转换
            gCtrlState = PSD_CTRL;
            gSteadCount = 0;
            gSteadCount0 = 0;
            gInitialFlag = 1;
            gOffset = 0;
            gAdjustFlag = 0;
            return;
        }
        
        if (gGpsOK == 5)
        {
            if (0 == psd_judge)
                gSteadCount++;
            else
            {
                gLostTime ++;           //注明gLostTime约1秒钟长100(debug)
                gCtrlState = SEMI_SYN;
                gSteadCount = 0;
                return -1;
            }
        }    
          
        enNsDir = VerticalCtrl(gaMPUData1_1, thr_h, thr_h - 2);// 加速度传感器读取垂直角度稳定后，范围thr_h, thr_h - 2；
        if ((INVALIDDIRECT == enNsDir)&& (gSteadCount0 < 50))
        {
            gSteadCount0++; //  加速度传感器稳定计数
        }
        else
        {
            gSteadCount0 = 0;
        }

        if ((gSteadCount0 >= 50) && (gGpsOK == 2))
        {
            gGpsOK = 3;
            gSteadCount = 0;
            gSteadCount0 = 0;
        }

        if (INVALIDDIRECT != enNsDir)  //  需要进行垂直调整，一定要保证垂直方向调整好
        {
            *p_EwDirect = enEwDir;
            *p_NsDirect = enNsDir;
            return -1;
        }

        if (gGpsOK == 3) // 水平调整
        {
            if (gOffset <= 0)
            {
                *p_EwDirect = enEwDir;
                *p_NsDirect = enNsDir;
                //gDirection = 0;
                return -1;
            }
            if (gDirection == 1)  // turn east 
            {
                enEwDir = TURNEAST;
                gOffset--;
            }
            else if (gDirection == 2) // turn west
            {
                enEwDir = TURNWEST;
                gOffset--;
            }
            else
                enEwDir = INVALIDDIRECT;
        }
    }
    else if ( gGpsOK == 4 )
    {
        enEwDir = INVALIDDIRECT;
        enNsDir = INVALIDDIRECT;

        if (0 == psd_judge)
        {
		//	DelayCount0_ForMotorControl++;
		/*
			if(DelayCount0_ForMotorControl >= 1000000)  //这里的数值有待测量
			{
				DelayCount0_ForMotorControl = 0;
				DelayCount1_ForMotorControl++;
			}
			*/
            gSteadCount++;
            gLostTime = 0;
            gCtrlState = PSD_CTRL;

            gAdjustFlag = 0;  //  失调次数计数器， 超过30次将状态置为idle
        }
        else
        {
	//		DelayCount0_ForMotorControl = 0;
	//		DelayCount1_ForMotorControl = 0;
            gLostTime++;      //  
            if (gLostTime >= 10000)
            {
                gGpsOK = 5;
                gCtrlState = SEMI_SYN; //乌云挡住状态标志
                gSteadCount = 0;
            }
            *p_EwDirect = enEwDir;
            *p_NsDirect = enNsDir;
            return -1;
        }
		/*
		if(DelayCount1_ForMotorControl >= 1000)     //这里的数值有待测量
		{
			for (loop = 0; loop < 6000; loop++) //大约休息1分钟
            {
            	ServiceDog();
            	DELAY_US(3000L);
				ServiceDog();
			}
   			DelayCount0_ForMotorControl = 0;
			DelayCount1_ForMotorControl = 0;
		} 
		*/
#ifdef FiveMMPSD// 无定义
        if ((gXvalue - gEWValue) > MIN_DIFF_PSD)
        {
            // turn east
            enEwDir = TURNEAST;
        }
        else if ((gXvalue - gEWValue) < (0-MIN_DIFF_PSD))
        {
            // turn west
            enEwDir = TURNWEST;
        }
        else
        {
            enEwDir = INVALIDDIRECT;
            // stay
        }
        
        if ((gYvalue - gNSValue) > MIN_DIFF_PSD_Y)
        {
            // turn north
            enNsDir = TURNNORTH;      //1号机：TURNNORTH;
        }
        else if ((gYvalue - gNSValue) < (0-MIN_DIFF_PSD_Y))
        {
            // turn south 
            enNsDir = TURNSOUTH;      //1号机：TURNSOUTH;
        }
        else
        {
            // stay
            enNsDir = INVALIDDIRECT;
        }
#endif

#ifdef NineMMPSD //当前使用  9*9 mm 传感器
        if ((gXvalue - gEWValue) > MIN_DIFF_PSD)
        {
            // turn east
            enEwDir = TURNEAST;
        }
        else if ((gXvalue - gEWValue) < (0-MIN_DIFF_PSD))
        {
            // turn west
            enEwDir = TURNWEST;
        }
        else
        {
            enEwDir = INVALIDDIRECT;
            // stay
        }
        
        if ((gYvalue - gNSValue) > MIN_DIFF_PSD_Y)
        {
            // turn north
            enNsDir = TURNNORTH;
        }
        else if ((gYvalue - gNSValue) < (0-MIN_DIFF_PSD_Y))
        {
            // turn south 
            enNsDir = TURNSOUTH;
        }
        else
        {
            // stay
            enNsDir = INVALIDDIRECT;
        }

#endif

    }
   

    *p_EwDirect = enEwDir;
    *p_NsDirect = enNsDir;

    return 0;
}




void my_memcpy(Uint16* p_des, Uint16* p_src, int32 len)
{
    int32  loop;

    for (loop = 0; loop < len; loop++)
    {
        *p_des++ = *p_src++;
    }
}
void my_memset(Uint16* p_des, int32 len, char dat)
{
    int32  loop;

    for (loop = 0; loop < len; loop++)
    {
        *p_des++ = dat;
    }
}

Uint8 gOutputBuffer[64];
Uint8 gE2Buffer[64];
int32   gMachineOfst = 0;


void DataInit()
{
    int result = 0;
    int32 i;
    gSunInfoBak.fangweijiao = 0;
    gSunInfoBak.gaodujiao = 0;
    gSunInfoBak.sin_gaodujiao = 0;
    gSunInfoBak.time = 0;

    gSunInfoCurr.resv[0] = 0;
    gSunInfoCurr.resv[1] = 0;
    gSunInfoCurr.resv1 = 0;
    gSunInfoCurr.fangweijiao = 0;
    gSunInfoCurr.gaodujiao = 0;
    gSunInfoCurr.sin_gaodujiao = 0;
    gSunInfoCurr.time = 0; 

	LearnedMotorSpeedInfo.LearnedMotorSpeed=0;
	LearnedMotorSpeedInfo.yearandmonthandday=0;
	LearnedMotorSpeedInfo.hourandminute =0;

    my_memset((Uint16*)&gSunInfoTest,sizeof(gSunInfoTest),0);
    my_memset((Uint16*)&gSunInfoBak,sizeof(gSunInfoBak),0);

    gNightFlag = 1;
    gCurrentStep = 0;
    myCurrentStep = 0;
    gBakStep = 0;
    gGpsOK = 0;
    SetHorizontalSpeed(FASTEST);
    gMotorSpeed = 237;
    gInitialFlag = 0;
    gDelta = 30;
    gMachineOfst = 0;
    
    if (SensorType == 0x5050)
    {
        MIN_DIFF_PSD   = 10;
        MIN_DIFF_PSD_Y = 20;
    }
    else
    {
        MIN_DIFF_PSD   = 5;
        MIN_DIFF_PSD_Y = 5;
    }   
    gCtrlState = IDLE;
    
    gStateRecord.Flag = 0;
    gStateRecord.currstep   = 0;
    
    my_memset((Uint16*)gOutputBuffer, sizeof(gOutputBuffer), 0);
}
    


void CopytoU8Buff(Uint8* p_des, Uint16* p_src, Uint16 len)
{
    int iLoop;
    Uint16 tmp;
    for (iLoop = 0; iLoop < len; iLoop++)
    {
        tmp = *p_src++;
        *p_des ++ = tmp & 0xFF;
        *p_des ++ = (tmp >> 8) & 0xFF;
    }
}


void CopyFromU8Buff(Uint8* p_src, Uint16* p_des, Uint16 len)
{
    int iLoop;
    Uint16 tmp0, tmp1;
    for (iLoop = 0; iLoop < len; iLoop++)
    {
        tmp0 = *p_src++;
        tmp1 = *p_src++;
        *p_des ++ = (tmp0 & 0xFF) | ((tmp1 & 0xFF) << 8);
    }
}

extern int  MPU9150Test();
extern int  GpsParse(char* GPS_BUF, GPS* gpsinfo);
extern int  GetSunInfo(GPS* p_gps, SunInfo* p_si);

volatile int gCompensation = 0;//24;
volatile int gWrittenIdx = 0;
/**************俞磊增加的全局变量**************************/
volatile int gWrittenForLearnedMotorSpeedInfoIdx = 0; //自学习信息地址计数
volatile int HasWrittenCount = 0;
/*****************************************************/
void WriteCurrStep()
{
    gStateRecord.Flag       = 0;

    CopytoU8Buff(gOutputBuffer, (Uint16*)&gStateRecord.Flag, DSP_32_SIZE);
    EEPROM_I2CA_WriteData(gOutputBuffer, (STATE_RECORD_ADDR + 8 * gWrittenIdx), E2PROM_32_SIZE);
    DELAY_US(5000);

    gStateRecord.currstep   = gCurrentStep;
    gStateRecord.Flag       = STATE_RECORD_OCCUP;


    gWrittenIdx = (gWrittenIdx + 1) & (STATE_RECORD_COUNT - 1);
    
    CopytoU8Buff(gOutputBuffer, (Uint16*)&gStateRecord,sizeof(gStateRecord));
    EEPROM_I2CA_WriteData(gOutputBuffer, (STATE_RECORD_ADDR + 8 * gWrittenIdx), 8);
	ServiceDog();
    DELAY_US(5000);
   /* 记录写入次数值*/
    HasWrittenCount++;
	CopytoU8Buff(gOutputBuffer, (Uint16*)&HasWrittenCount, DSP_32_SIZE);
    EEPROM_I2CA_WriteData(gOutputBuffer, HasWrittenCount_RECORD_ADDR , E2PROM_32_SIZE);
	ServiceDog();
    DELAY_US(5000);
	ServiceDog();
}
void WriteLearnedMotorSpeedInfo() //写入自学习信息
{
	if(LearnedMotorSpeedInfo.LearnedMotorSpeed >= (gMotorSpeed-30) && LearnedMotorSpeedInfo.LearnedMotorSpeed <= (gMotorSpeed+30))   //控制，只存储这个范围=-30 gMotorSpeed之间的自学习数值
	{
   	gWrittenForLearnedMotorSpeedInfoIdx = (gWrittenForLearnedMotorSpeedInfoIdx + 1) & (255);
   /******************将自学习的信息写入地址***************************/
    CopytoU8Buff(gOutputBuffer, (Uint16*)&LearnedMotorSpeedInfo, sizeof(LearnedMotorSpeedInfo));
    EEPROM_I2CA_WriteData(gOutputBuffer, (LEARNEDMOTORSPEEDINFO_RECORD_ADDR  + 8 * gWrittenForLearnedMotorSpeedInfoIdx), 8);
	ServiceDog();
    DELAY_US(5000);
    ServiceDog();  
	/******************************************************************/
	
	/******************将自学习地址计数值写入地址中********************/
 	CopytoU8Buff(gOutputBuffer, (Uint16*)&gWrittenForLearnedMotorSpeedInfoIdx, DSP_32_SIZE);
    EEPROM_I2CA_WriteData(gOutputBuffer, LEARNEDMOTORSPEEDINFO_COUNT_POINTER_ADDR , E2PROM_32_SIZE);
	ServiceDog();
    DELAY_US(5000);
	ServiceDog();
	/******************************************************************/
	}
}
/////////////////////////////////////////////////////////////////////////
/*
void WriteCurrentState(int a)
{
	CopytoU8Buff(gOutputBuffer, (Uint16*)&a, DSP_32_SIZE);
    EEPROM_I2CA_WriteData(gOutputBuffer, State_ADDR , E2PROM_32_SIZE);
	ServiceDog();
    DELAY_US(5000);
	ServiceDog();
}
volatile int myggps = 0;
void ReadCurrentState()
{
	int result = 0;
	result = EEPROM_I2CA_ReadData((Uint8*)gE2Buffer, State_ADDR, E2PROM_32_SIZE);
	ServiceDog();
    DELAY_US(5000);        
    CopyFromU8Buff(gE2Buffer, (Uint16*)&myggps, DSP_32_SIZE);
	ServiceDog();
}
*/
/////////////////////////////////////////////////////////////////////////
#ifdef yulei_ReadLearnedMotorSpeedInfoTable
Uint16 ReadLearnedTable[150][8];         //读取所有自学习的信息到数组

void ReadLearnedMotorSpeedInfo()  //读出自学习的存入的信息
{
	int32 loop=0,result;
	for (loop = 0; loop < 150; loop++)
        {            
            result = EEPROM_I2CA_ReadData((Uint8*)gE2Buffer, (LEARNEDMOTORSPEEDINFO_RECORD_ADDR+8*loop), E2PROM_32_SIZE*2);//LEARNEDMOTORSPEEDINFO_RECORD_ADDR
            DELAY_US(5000); 
            ServiceDog();       
            CopyFromU8Buff(&gE2Buffer[0], (Uint16*)&ReadLearnedTable[loop][0], 1);
			CopyFromU8Buff(&gE2Buffer[2], (Uint16*)&ReadLearnedTable[loop][2], 1);
			CopyFromU8Buff(&gE2Buffer[4], (Uint16*)&ReadLearnedTable[loop][4], 1);
			CopyFromU8Buff(&gE2Buffer[6], (Uint16*)&ReadLearnedTable[loop][6], 1);
   		    DELAY_US(5000);
			ServiceDog();
        }
}
#endif

#ifdef yulei_gCurrentStepTable
Uint16 ReadCurrentTable[50][8]; 
void ReadCurrentStep()  //读出当前脉冲数的所有值存入的数组
{
	int32 loop=0,result;
	for (loop = 180; loop < 200; loop++)
        {            
          result = EEPROM_I2CA_ReadData((Uint8*)gE2Buffer, STATE_RECORD_ADDR + 8 * loop, E2PROM_32_SIZE*2);
		  ServiceDog();
          DELAY_US(5000);        
          CopyFromU8Buff(gE2Buffer, (Uint16*)&ReadCurrentTable[loop-180][0], DSP_32_SIZE);
          CopyFromU8Buff(&gE2Buffer[4], (Uint16*)&ReadCurrentTable[loop-180][4], DSP_32_SIZE);
		  ServiceDog();
   		  DELAY_US(5000);
   		  ServiceDog();        
        }
}
#endif
void ReadLearnedMotorSpeedInfo_Count_Pointer()  //读出自学习地址计数值
{
	int result = 0;
	result = EEPROM_I2CA_ReadData((Uint8*)gE2Buffer, LEARNEDMOTORSPEEDINFO_COUNT_POINTER_ADDR, E2PROM_32_SIZE);
	ServiceDog();
    DELAY_US(5000);        
    CopyFromU8Buff(gE2Buffer, (Uint16*)&gWrittenForLearnedMotorSpeedInfoIdx, DSP_32_SIZE);
	ServiceDog();
}

#ifdef LearnMotorSpeedTwiceOneMonthdefine
unsigned char learnedMotorSpeedFlag = 0;
Uint16 LearnedMotorSpeedNumber = 250;
Uint16 LearnedMotorSpeedNumber_sum = 5000;
Uint16 LearnedMotorSpeedNumber_avg = 250;
void LearnMotorSpeedTwiceOneMonth()
{   
	int loop = 0,result = 0;
	if(  (learnedMotorSpeedFlag = 0) && ( (gGpsinfo.day == 5 || gGpsinfo.day == 20) )  )
	{
		learnedMotorSpeedFlag = 1;
		ReadLearnedMotorSpeedInfo_Count_Pointer();
		if(gWrittenForLearnedMotorSpeedInfoIdx >= 20)
		LearnedMotorSpeedNumber_sum = 0;
		for (loop = gWrittenForLearnedMotorSpeedInfoIdx; loop > (gWrittenForLearnedMotorSpeedInfoIdx-20) ; loop--)
        {
                    
            result = EEPROM_I2CA_ReadData((Uint8*)gE2Buffer, (LEARNEDMOTORSPEED_ADDR+8*loop), E2PROM_32_SIZE*2);//LEARNEDMOTORSPEEDINFO_RECORD_ADDR
            DELAY_US(5000); 
            ServiceDog();       
            CopyFromU8Buff(&gE2Buffer[0], (Uint16*)&LearnedMotorSpeedNumber, 1);
			LearnedMotorSpeedNumber_sum = LearnedMotorSpeedNumber_sum + LearnedMotorSpeedNumber;
   		    DELAY_US(5000);
			ServiceDog();
        }
		LearnedMotorSpeedNumber_avg = LearnedMotorSpeedNumber_sum/20;
		LearnedMotorSpeedNumber_sum = 0;
		gMotorSpeed = LearnedMotorSpeedNumber_avg;

		/******************将自学习平均数值写入地址中********************/
 		CopytoU8Buff(gOutputBuffer, (Uint16*)&gMotorSpeed, DSP_32_SIZE);
  	    EEPROM_I2CA_WriteData(gOutputBuffer, LEARNEDMOTORSPEED_ADDR , E2PROM_32_SIZE);
		ServiceDog();
     	DELAY_US(5000);
		ServiceDog();
    	/******************************************************************/
	}
}
void ReadMotorSpeed()
{
	int result = 0;
	result = EEPROM_I2CA_ReadData((Uint8*)gE2Buffer, LEARNEDMOTORSPEED_ADDR, E2PROM_32_SIZE);
	ServiceDog();
    DELAY_US(5000);        
    CopyFromU8Buff(gE2Buffer, (Uint16*)&gMotorSpeed, DSP_32_SIZE);
	ServiceDog();
}
#endif

extern int32 gawPsdV[128][4];
extern  int thr_h;
void MainCtrl()
{
    int result = 0;
    int32 difference, diffstep;
    int gaodujiao;
    Uint32 loop=0;
    trace_record* p_trace;
    int32  record_offset;
    int32  write_interval;
    int32  record_state = 0;
    step_record  step_rec;
	Uint16 ReadyForGPS = 0;
	int32	GPSIndex = 0;
	Uint32 i=0;
	Uint32 a=0;
//	IntializeStepRecord();            //清零
 //   WriteCurrentState(0);
    MPU9150Test();
//	ReadCurrentState();
	//ReadCurrentStep();
//	ReadLearnedMotorSpeedInfo();
//WriteCurrStep();
    if (gGpsOK >= 4)
        write_interval = 1000000;

    else
        write_interval = 10000;
    // the first running for the day, reading eeprom to get the last pose.
    if ((gInitialFlag == 0)&&(gGpsOK == 0))  // these statements can be moved to the initialization function.
    {
        //  find the latest step record to get the currrent step 
	
        for (loop = 0; loop < STATE_RECORD_COUNT; loop++)
        {
            
            result = EEPROM_I2CA_ReadData((Uint8*)gE2Buffer, STATE_RECORD_ADDR + 8 * loop, E2PROM_32_SIZE*2);
            DELAY_US(5000); 
            ServiceDog();       
            CopyFromU8Buff(gE2Buffer, (Uint16*)&gStateRecord.Flag, DSP_32_SIZE);
            
            // when the buffer has been occupied, the flag will be 0xAABBCCDD
            if ( (Uint32)STATE_RECORD_OCCUP == gStateRecord.Flag )
            {
                gWrittenIdx = loop;
                CopyFromU8Buff(&gE2Buffer[4], (Uint16*)&gStateRecord.currstep, DSP_32_SIZE);
                gCurrentStep = gStateRecord.currstep;
                myCurrentStep = gCurrentStep;
                break;
            }
        }
        
        // when all the buffer is not occupied, consider that the last one has been occupied
        // so that the new record will be written to the first buffer.
        if (loop == STATE_RECORD_COUNT)   //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        {
            gWrittenIdx = STATE_RECORD_COUNT - 1;
        }
        //读出写入次数值
     	result = EEPROM_I2CA_ReadData((Uint8*)gE2Buffer, HasWrittenCount_RECORD_ADDR, E2PROM_32_SIZE);
        DELAY_US(5000); 
        ServiceDog();       
        CopyFromU8Buff(gE2Buffer, (Uint16*)&HasWrittenCount, DSP_32_SIZE);
        
        //读出自学习地址计数值
	    #ifdef yulei_WriteLearnedMotorSpeedInfo
	       ReadLearnedMotorSpeedInfo_Count_Pointer();
	    #endif

	    gInitialFlag = 0xFF;
		ServiceDog();
    }
    
    if (gInitialFlag == 1)  //表示追到太阳，要处于精调状态
    {
        if (gMachineOfst == 0)  //  记录安装位置与正南方向的夹角，指12点太阳的正南方向，安装好之后不会改变
        {
            gMachineOfst = gCurrentStep - gSunInfoCurr.fangweijiao*gMotorSpeed;   //当前状态减去目标状态(天文算法记录)
            CopytoU8Buff(gOutputBuffer, (Uint16*)&gMachineOfst,sizeof(gMachineOfst));
            result = EEPROM_I2CA_WriteData(gOutputBuffer, OFFSET_RECORD_ADDR, sizeof(gMachineOfst)*2);
        }
        my_memcpy((Uint16 * )&gSunInfoBak,(Uint16 *)&gSunInfoCurr, sizeof(gSunInfoCurr));
        
        // set the flag to 2 to granttee that the offset is calculated only once.
        gInitialFlag = 2;
    }


    // 录当前姿态 ( 水平方向 )
    //if ((gCounters1 >= write_interval)&&(gGpsOK>=3) && (gInitialFlag > 0))
    if (gCounters1 >= write_interval)
    {
        gCounters1 = 0;
        diffstep = gCurrentStep - gStateRecord.currstep;
        diffstep = (diffstep > 0) ? diffstep : -diffstep;
        if (diffstep > 500)  //write down the step when different with 2 degree
            WriteCurrStep();

    }
    if (gInitialFlag == 0xAA)
    {
        my_memset((Uint16*)gawPsdV[0], sizeof(gawPsdV),0);
        RecordRead((Uint16*)gawPsdV[0], STATE_RECORD_COUNT, (Uint32)STATE_RECORD_ADDR, 0, 4);
        gInitialFlag = 0xFF;
    }

    //  读取 GPS 信息， 初始状态，  精调状态 ( 锹脊旒?)  ，  失调 (乌云遮挡)
    //  初始状态，需要读取 GSP 时间地点，计算方位角和高度角  (gGpsOK == 0)
    //  精调状态 (gGpsOK == 4)  每隔一段时间记录轨迹
    //  乌云遮挡， (gGpsOK ==5)   隔一段时间需要调整当白刺员愠鎏糁舐砩险业?
    if ((gGpsOK == 0) || ((gGpsOK ==5)&& gLostTime >= 10000) || ((gGpsOK == 4) && (gSteadCount >= 20000)))
  //  if (0)  // when testing the E2PROM cyclic writing, set the condiction to 0 to disable the GPS searching. 
    {
        if (gGpsOK ==5)
        {
            gLostTime = 0;
        }
        
        if (gGpsOK == 4)
        {
            gSteadCount = 0;
        }
        
        if ( GPS_TEST_OFF == 0)   //  GPS_TEST_OFF   测试标志，当室内无  GPS 信号， 应设置为 1，默认不会进测试模式
        {
			if (0 == GPS_Test())
			{	
				result = GpsParse(GPSBuffer, &gGpsinfo);  //  GSP 文本解析， 算出本地时间及经纬度
			    if (result != 0)
			        return;
				ServiceDog();
			    result = GetSunInfo(&gGpsinfo, &gSunInfoCurr);   //  天文算法,  算出太阳理论方位  
				RealTime = gGpsinfo.hour*60 + gGpsinfo.minute;
				if(RealTime<0||RealTime>1439)
					return;
			    if ( (gSunInfoCurr.sin_gaodujiao < 0) ||      
			        (gSunInfoCurr.sin_gaodujiao > 265))
				    {
				        gErrRecord.ErrCode = 0xB0;
				        gErrRecord.RecordFlag = 1;
				        gErrRecord.monthdate = gGpsinfo.day | (gGpsinfo.month<<8);
				        gErrRecord.currstep = gCurrentStep;
				        gErrRecord.time = gSunInfoCurr.time;
				        gErrRecord.gaodujiao = gSunInfoCurr.gaodujiao;
				        gErrRecord.year = gSunInfoCurr.sin_gaodujiao;
				        gErrNum++;
				        gErrNum = gErrNum&127;
				        CopytoU8Buff(gOutputBuffer, (Uint16*)&gErrRecord, sizeof(gErrRecord));
				        result = EEPROM_I2CA_WriteData(gOutputBuffer, ERROR_RECORD_ADDR + gErrNum*16, sizeof(trace_record)*2);
						ServiceDog();
				        DELAY_US(1000);                        
				        gGpsOK = 0;
				        return ;
				    } // sometime, sin_gaodujiao maybe a negative value 
			    if (result != 0)
			        return;
				if(gGpsOK == 4)
				{
				  #ifdef yulei_WriteLearnedMotorSpeedInfo //写自学习循环写数据进去
			  	  if(gSunInfoCurr.time-PreviousGotogGpsOK4_Time>=60)
				    {
					  if(PreviousGotogGpsOK4_Time!=0)
					    {
						  LearnedMotorSpeedInfo.LearnedMotorSpeed = (myCurrentStep-PreviousCurrentStep)/(gSunInfoCurr.fangweijiao-PreviousCurrentFangweijiao);
						  LearnedMotorSpeedInfo.yearandmonthandday = (Uint16)(  ((((gGpsinfo.year-2000)&0x0f)<<12) | ((gGpsinfo.month&0x0f)<<8)) | (gGpsinfo.day&0xff) );
						  LearnedMotorSpeedInfo.hourandminute = (Uint16) (((gGpsinfo.hour&0xff)<<8) | (gGpsinfo.minute)&0xff);
						  WriteLearnedMotorSpeedInfo();
					    }
					  PreviousGotogGpsOK4_Time = gSunInfoCurr.time;
				 	  PreviousCurrentStep = myCurrentStep;
				      PreviousCurrentFangweijiao = gSunInfoCurr.fangweijiao;  
				    }
				  #endif
				  #ifdef WriteE2PROMEvery8Min
				  if((gSunInfoCurr.time-EarlyTime)>=8)  //大概2度角对应8分钟,精调模式下，每8分钟写入E2PROM一次gCurrentStep的值
				    {
					  WriteCurrStep();
					  EarlyTime = gSunInfoCurr.time;
				    }
				  #endif

				  gCurrentStep=gSunInfoCurr.fangweijiao*gMotorSpeed+gMachineOfst;//处于精调且算出方位角便时刻更新gCurrentStep
			    }
			}
        }
        else
        {
            
            gSunInfoCurr.time = gSunInfoBak.time;
            return ;
        }


        if (gGpsOK == 4)   //  在精调情况下，如何记录轨迹  
        {
            gDelta = 0;
            gCtrlState = PSD_CTRL;
            
            gSunInfoBak.fangweijiao = gSunInfoCurr.fangweijiao;
            gSunInfoBak.gaodujiao   = gSunInfoCurr.gaodujiao;
            gSunInfoBak.sin_gaodujiao = gSunInfoCurr.sin_gaodujiao;
            gSunInfoBak.speed         = gSunInfoCurr.speed;
            gSunInfoBak.resv[0] = 0x5A5B5C5D;
            gSunInfoBak.CurrStep = gCurrentStep;
            
            if ((gSunInfoCurr.time >> 2) == (gSunInfoBak.time >> 2))
            {
                return;
            }
            
            gSunInfoBak.time        = gSunInfoCurr.time;

            // save the data to I2C
            record_offset = ((gSunInfoCurr.time - EARLIEST_WORKTIME) >> 2) & 255;
            p_trace = &gTraceRecord;//[record_offset];
            p_trace->elem_state  = 1;
            p_trace->year        =  gGpsinfo.year;
            p_trace->monthdate   = (gGpsinfo.month << 8) | gGpsinfo.day;
            p_trace->time        = gSunInfoCurr.time;
            p_trace->fangweijiao = gSunInfoCurr.fangweijiao;
            p_trace->gaodujiao   = gSunInfoCurr.gaodujiao;
            p_trace->currstep    = gCurrentStep;
            

            
            CopytoU8Buff(gOutputBuffer, (Uint16*)p_trace, sizeof(trace_record));
            result = EEPROM_I2CA_WriteData(gOutputBuffer, TRACE_RECORD_ADDR + record_offset*16, sizeof(trace_record)*2);
			ServiceDog();
            DELAY_US(1000);
			ServiceDog();

            
            
  
            return;
        }

	//    ABCfangweijiao=(gSunInfoCurr.fangweijiao>0)?gSunInfoCurr.fangweijiao:-gSunInfoCurr.fangweijiao;
        // if night 
	//	if (gSunInfoCurr.gaodujiao <45)           /////////////////////////////////////////////////////////////////////新增的修改
   //     if ((gSunInfoCurr.fangweijiao > MAX_SUN_AZIMUTH) || (gSunInfoCurr.fangweijiao < MIN_SUN_AZIMUTH))
  //      if ((gSunInfoCurr.time > gTime2Bed) || (gSunInfoCurr.time < gTime2work))
		//if(ABCfangweijiao>=45||gGpsinfo.hour>8)
		if(RealTime<60 || RealTime>=570)  //上班时间为早上9点到下午2点半   
	//	if(gGpsinfo.hour>=6 || gGpsinfo.hour<=0)   //相当于北京时间6+8      执行下班的东西
        {
            if ((gNightFlag == 0)&& ((gCtrlState == PSD_CTRL) || (gCtrlState == SEMI_SYN)))
            {
                //difference = gSunInfoCurr.fangweijiao*gMotorSpeed + gMachineOfst;
                difference = gCurrentStep;
                
                if (difference != 0)
                {
                    gGpsOK = 3;
                    gOffset = (difference > 0) ? difference : - difference;
                    gDirection = (difference > 0) ? 1 : 2;
                }
                else
                {
                    gGpsOK = 0;
                    gDirection = 0;
                }


                
                gSunInfoCurr.resv[0] = 0xCDCDCDCD;
                gSunInfoCurr.CurrStep = 0;
                gSunInfoBak.CurrStep = 0;
                gSunInfoBak.fangweijiao = 0;
                
                gCtrlState = NIGHT;
                gSunInfoCurr.resv1 = gCtrlState;


                
                gTargetAccumValue = 200;//162;  //  用宏定义代替

                thr_h = gTargetAccumValue + gCompensation;                

                gNightFlag = 1;
                return;
            }
            else
            {
                if (gCurrentStep != 0)
                {
                    gOffset = (gCurrentStep > 0) ? gCurrentStep : - gCurrentStep;
                    gDirection = (gCurrentStep > 0) ? 1 : 2;
                    gGpsOK = 3;
                    gSunInfoBak.CurrStep = 0;
                    gSunInfoBak.fangweijiao = 0;
                    gTargetAccumValue = 200;//162;   162约为45度向上
                    thr_h = gTargetAccumValue + gCompensation; 
                }
                else
                {
                    gOffset = 0;
                    gGpsOK = 0;
                    gDirection = 0;
                }
                
            }
            gCtrlState = NIGHT;

            if (gOffset > 0)
            {
                return;
            }
            gGpsOK = 0;
            //  sleep for a hour
            for (loop = 0; loop <12000 ; loop++) //大约休息2分钟
			{
                ServiceDog();
                DELAY_US(3000L); //这里为10ms
				ServiceDog();
			}


            // close GPS / sensor value reading to save power
            // delay for a long time
            return;
        }


        gNightFlag = 0;
        record_offset = ((gSunInfoCurr.time - EARLIEST_WORKTIME) >> 2) & 255;
        result = EEPROM_I2CA_ReadData((Uint8*)gE2Buffer,TRACE_RECORD_ADDR + record_offset*16,sizeof(trace_record)*2);
		ServiceDog();
        DELAY_US(5000);
		ServiceDog();
        p_trace = &gTraceRecord;//[record_offset];
        CopyFromU8Buff(gE2Buffer, (Uint16*)p_trace, sizeof(trace_record));

        if (0)//((gStateRecord.state == (int)PSD_CTRL) && (p_trace->elem_state == 1))
        {
            record_state = 1;
            gTargetStep = gTraceRecord.currstep;//[record_offset].currstep;
        }
        else
        {
            gTargetStep = gSunInfoCurr.fangweijiao*gMotorSpeed;
        }

        if (gCtrlState == NIGHT)
        {
            gCtrlState = IDLE;
        }
        
        if (gGpsOK == 5)  // 乌云遮挡时候  
        {
            if (0)//(record_state == 1)
            {
                return;            
            }
            else
            {
                gMotorSpeed = 237;
				if(gSunInfoCurr.time - gSunInfoBak.time>=4)  //之前为4!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					{
					   gOffset = ((gSunInfoCurr.time - gSunInfoBak.time)/4) * gMotorSpeed;
					   gOffset = (gOffset >= 0) ? gOffset : - gOffset;
                       gSunInfoBak.time = gSunInfoCurr.time;
					}
				else 
				   gOffset = 0;
               // gOffset = (gSunInfoCurr.fangweijiao - gSunInfoBak.fangweijiao)*gMotorSpeed;
			//	gOffset = (gOffset >= 0) ? gOffset : - gOffset;   //请注意，因为下午很有可能检测到的gOffset为负数，所以之前导致种种问题
            }
        
            if (gOffset == 0)   
            {
                gDirection = 0;
                return;
            }
            
            gDirection = 2;
            gSearchTime = 0;  //  在乌云遮挡的时候不用左右扫描  
            gDelta = 0;
            gTargetAccumValue = gSunInfoCurr.sin_gaodujiao;
            gGpsOK = 2;
            thr_h = gTargetAccumValue + gCompensation;
            return;
        }



        




            
        //difference = (gSunInfoCurr.fangweijiao - gSunInfoBak.fangweijiao)*gMotorSpeed;
        if (record_state == 1)        
            difference = gTargetStep - gCurrentStep;
        else
            difference = gTargetStep - gCurrentStep + gMachineOfst;

        if (gMachineOfst != 0)
        {
            gDelta = 30;
        }

        

        gTargetAccumValue = gSunInfoCurr.sin_gaodujiao;

        thr_h = gTargetAccumValue + gCompensation;




        if (difference < 0)
        {
            gDirection = 1;
            difference = -difference;
        }
        else
        {
            gDirection = 2;
        }
        gOffset = difference;




        gGpsOK = 2;


    }

    
    


    
    if ((gCtrlState == NIGHT) && (gGpsOK == 3) && (gOffset == 0))
    {
        gSearchTime = 0;
        gGpsOK = 0;
        gDirection = 0;
        WriteCurrStep();
    }



    if (((gCtrlState == IDLE)||(gCtrlState == SEMI_SYN)) && (gGpsOK == 3) 
        && (gOffset == 0) && (gSearchTime < 3) && (gDirection != 0))
    {


        if (gDelta != 0)
        {
#if 1
            switch (gSearchTime)
            {
                case 0:
                {
                    gOffset = gDelta*gMotorSpeed;
                    break;
                }
                case 1:
                {
                    gDirection = (gDirection == 1) ? 2:1;
                    gOffset = gDelta*gMotorSpeed*2;
                    break;
                }
                case 2:
                {
                    gDirection = (gDirection == 1) ? 2:1;
                    gOffset = gDelta*gMotorSpeed;
                    break;
                }
                default:
                {
                    break;
                }
   
            }
#else                                            
            
        //    gDirection = ((gDirection + 1 +gSearchTime)&1)+1 ;
      //      gOffset = ((gSearchTime&0x1) == 0) ? (gDelta*gMotorSpeed) : (gDelta*gMotorSpeed)*2;
#endif
        }
        gSearchTime++;

    }

    if ((gCtrlState == IDLE) && (gGpsOK == 3) && (gOffset == 0) && (gSearchTime >= 3))     //左右扫描完毕
    {
        if (0)//(gCurrentStep != 0)
        {
            gOffset = (gCurrentStep >=0) ?  gCurrentStep : -gCurrentStep;
            gDirection = (gCurrentStep >0) ? 1 : 2;
            gDirection = (gOffset == 0) ? 0 : gDirection;
            gSearchTime++;
        }
        else
        {
            gSearchTime = 0;
            gGpsOK = 3;                   //自己加的测试代码，原来为gGpsOK = 0;
	 		gCtrlState = SEMI_SYN;        //自己加的测试代码，原来没有这一行
	 		gDelta = 0;                   //自己加的测试代码，原来没有这一行
            WriteCurrStep();            
        }
        
        for (loop = 0; loop < 10000; loop++) //大约休息5/3分钟
            {
            	ServiceDog();
            	DELAY_US(3000L);
	 			ServiceDog();
	 		}
        
    }



    if ((gCtrlState == SEMI_SYN) && (gGpsOK == 3) && (gOffset == 0))
    {
        my_memcpy((Uint16 *)&gSunInfoBak,(Uint16 *)&gSunInfoCurr, sizeof(gSunInfoCurr));
        gSunInfoBak.fangweijiao = gSunInfoCurr.fangweijiao;
		gSunInfoBak.time = gSunInfoCurr.time;
		gSearchTime = 0;
		
		gGpsOK = 5;
		gCtrlState = SEMI_SYN;

        gDirection = 0;

        gAdjustFlag++;

        if (gAdjustFlag > 5)   //自己的修改，这里主要用作测试
        {
            gGpsOK = 0; 
            gAdjustFlag = 0;
            gDelta = 30;
			gCtrlState=IDLE;    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!增加的一句 之前为SEMI_SYN
        }
    }

   
}

void IntializeStepRecord()
{
    int32 loop;
    for (loop = 0; loop < STATE_RECORD_COUNT; loop++)
    {
        gStateRecord.currstep   = gCurrentStep;
        gStateRecord.Flag       = 0;
        
        CopytoU8Buff(gOutputBuffer, (Uint16*)&gStateRecord.Flag, DSP_32_SIZE);
        EEPROM_I2CA_WriteData(gOutputBuffer, (STATE_RECORD_ADDR + 8 * loop), 4*2);
		ServiceDog();
        DELAY_US(5000);
		ServiceDog();
    }
  /******************将自学习地址计数值写入地址中********************/
 	CopytoU8Buff(gOutputBuffer, (Uint16*)&gWrittenForLearnedMotorSpeedInfoIdx, DSP_32_SIZE);
    EEPROM_I2CA_WriteData(gOutputBuffer, LEARNEDMOTORSPEEDINFO_COUNT_POINTER_ADDR , E2PROM_32_SIZE);
	ServiceDog();
    DELAY_US(5000);
    ServiceDog();  
 /******************将械偏移值写入地址中********************/
	CopytoU8Buff(gOutputBuffer, (Uint16*)&gMachineOfst,sizeof(gMachineOfst));
    EEPROM_I2CA_WriteData(gOutputBuffer, OFFSET_RECORD_ADDR, sizeof(gMachineOfst)*2);
	ServiceDog();
    DELAY_US(10000);
	ServiceDog();

	CopytoU8Buff(gOutputBuffer, (Uint16*)&HasWrittenCount,sizeof(HasWrittenCount));
    EEPROM_I2CA_WriteData(gOutputBuffer, HasWrittenCount_RECORD_ADDR, sizeof(HasWrittenCount)*2);
	ServiceDog();
    DELAY_US(10000);
	ServiceDog();
}
/***********************************************/
/****  config the  sensor type and the axis ***************/
/***** initialize the buffer for step record   **************/
/***** initialize the buffer for trace record   **************/
/***************************************************/

void ParaConfig()
{
    int result = 0;
    int loop;
    // config the sensor type , 0x5050 : pinhole   0xAAAA: lens
    gFactorySetting.sensor_type = SensorType;
    
    SensorType = 0x5050;
    gEWValue = -160;//0;//-210;//-210;//460;//-210;     gx -23  gy=24     //在这里配置"DX、DY"
    gNSValue = 185;//40;//1100;//1100;
    // YH-3 EWvalue -210 NSValue 1100
    // YH-2 EWvalue 460 NSValue -1100
    
    // config the reference x and y
    gFactorySetting.delta_x = gEWValue;
    gFactorySetting.delta_y = gNSValue;
    gFactorySetting.sensor_type = SensorType;
	gWrittenForLearnedMotorSpeedInfoIdx=0;

    if (SensorType == 0x5050)
    {
        MIN_DIFF_PSD   = 50;//50;
        MIN_DIFF_PSD_Y = 100;//100;
    }
    else
    {
        MIN_DIFF_PSD   = 5;
        MIN_DIFF_PSD_Y = 5;
    }
    CopytoU8Buff(gOutputBuffer, (Uint16*)&gFactorySetting,sizeof(gFactorySetting));
    result = EEPROM_I2CA_WriteData(gOutputBuffer, CONFIG_RECORD_ADDR, sizeof(gFactorySetting)*2);
	ServiceDog();
    DELAY_US(50000);
	ServiceDog();



    
    CopytoU8Buff(gOutputBuffer, (Uint16*)&gMachineOfst,sizeof(gMachineOfst));
    result = EEPROM_I2CA_WriteData(gOutputBuffer, OFFSET_RECORD_ADDR, sizeof(gMachineOfst)*2);
	ServiceDog();
    DELAY_US(50000);
	ServiceDog();

    my_memset((Uint16 *)&gTraceRecord, sizeof(gTraceRecord), 0);
    for (loop = 0; loop < 256; loop++)
    {
        CopytoU8Buff(gOutputBuffer, (Uint16*)&gTraceRecord/*[loop]*/,sizeof(trace_record));
        result = EEPROM_I2CA_WriteData(gOutputBuffer, TRACE_RECORD_ADDR+loop*16, sizeof(trace_record)*2);
		ServiceDog();
        DELAY_US(5000);
		ServiceDog();
    }
    
  

}


void ParaRead()
{
    int result = 0;
    result = EEPROM_I2CA_ReadData((Uint8*)gE2Buffer, CONFIG_RECORD_ADDR, sizeof(gFactorySetting)*2);
	ServiceDog();
    DELAY_US(5000);
	ServiceDog();
    CopyFromU8Buff(gE2Buffer, (Uint16*)&gFactorySetting, sizeof(gFactorySetting));
    
    // config the sensor type , 0x5050 : pinhole   0xAAAA: lens
    SensorType = gFactorySetting.sensor_type;
    SensorType = 0x5050;
    
    // config the reference x and y
    gEWValue = gFactorySetting.delta_x;
    gNSValue = gFactorySetting.delta_y;

    if (SensorType == 0x5050)
    {
        MIN_DIFF_PSD   = 50;
        MIN_DIFF_PSD_Y = 100;
    }
    else
    {
        MIN_DIFF_PSD   = 5;
        MIN_DIFF_PSD_Y = 5;
    }

    result = EEPROM_I2CA_ReadData((Uint8*)gE2Buffer, OFFSET_RECORD_ADDR, sizeof(gMachineOfst)*2);
	ServiceDog();
    DELAY_US(5000);
	ServiceDog();
    CopyFromU8Buff(gE2Buffer, (Uint16*)&gMachineOfst, sizeof(gMachineOfst));

}


void RecordRead(Uint16* pOutPut, int num, Uint32 startaddr, int offset, int size)
{
    int result = 0;

    int loop;

    for (loop = 0; loop < num; loop++)
    {
        //result = EEPROM_I2CA_ReadData((Uint8*)gE2Buffer, TRACE_RECORD_ADDR + (loop+offset)*16, sizeof(trace_record)*2);
        result = EEPROM_I2CA_ReadData((Uint8*)gE2Buffer, startaddr + (loop+offset)*size*2, size*2);
        ServiceDog();
        DELAY_US(5000);
		ServiceDog();
        CopyFromU8Buff(gE2Buffer, (Uint16*)pOutPut + loop*size, size);
    }

}

