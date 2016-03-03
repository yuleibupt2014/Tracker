

#ifndef  __MOTOR_CTRL_H__
#define  __MOTOR_CTRL_H__

#include  "DSP280x_Device.h"     // DSP280x Headerfile Include File

#include  "Gps.h"

//#define GPS_TEST_OFF



#define MIN_THRESHOLD   100
#define MIN_THRESHOLD_APPROX    1200
#define MIN_THRESHOLD_PRECISE   1300
#define MAX_THRESHOLD_PRECISE       0x1000

#define SUCCESS         0
#define FAIL            0xFFFFFFFF

#define VALID           1
#define INVALID         0

#define MIN_DIFF_APPROX        40
#define MIN_DIFF_PRECISE       50

#define PRECISE_EAST_THR 1500
#define PRECISE_WEST_THR 1500
#define PRECISE_NORTH_THR  1500
#define PRECISE_SOUTH_THR  1500

#define STEADY  1

#define BASIC_ACCE_VALUE 256
////////////////////////////
#define TESTMANUAL
////////////////////////////

#define MAX_SUN_AZIMUTH  30
#define MIN_SUN_AZIMUTH  (-60)


//  MACRO for E2PRAM   
#define E2PROM_32_SIZE      4
#define DSP_32_SIZE         2
#define CONFIG_RECORD_ADDR  1024         // PSD 传感器 阳光垂直玻璃面的 参考点 
#define OFFSET_RECORD_ADDR  (1024 + 32)  //   安装夹角记录 地址 

#define STATE_RECORD_ADDR  16384    //  状态记录地址
#define STATE_RECORD_COUNT  256    //  total count of  step record , 
#define STATE_RECORD_OCCUP  0xAABBCCDDL   //  状态记录地址

#define LEARNEDMOTORSPEEDINFO_RECORD_ADDR 19200  //记录自学习的信息地址
#define LEARNEDMOTORSPEEDINFO_COUNT_POINTER_ADDR (19200 - 64) //记录自学习地址计数值的地址
#define State_ADDR (19200 - 128) //记录当前状态

#define LEARNEDMOTORSPEEDINFO_RECORD_COUNT 128   //total count of 自学习的信息记录
#define HasWrittenCount_RECORD_ADDR 30720       //记录写入次数的值

#define TRACE_RECORD_ADDR  2048   ///  轨迹记录地址
#define LEARNEDMOTORSPEED_ADDR (19200-256)    //记录平均自学习值


#define EARLIEST_WORKTIME  (5*60)
#define LATEST_BEDTIME     (19*60)

#define ERROR_RECORD_ADDR  8192

typedef struct trace_record
{
    Uint16  elem_state;
    Uint16  year;
    Uint16  monthdate;
    Uint16  time; // 60*hour +min

    int16   fangweijiao;
    int16   gaodujiao;
    int32   currstep;
}trace_record;  // 16 char


typedef struct step_record
{
    Uint32  Flag;
    int32   currstep;  
}step_record;

typedef struct LearnedMotorSpeedInformation
{
	int32   LearnedMotorSpeed;
	Uint16  yearandmonthandday;
	Uint16   hourandminute; 
}LearnedMotorSpeedInformation;

typedef struct state_record
{
    Uint16   state;
    Uint16   year;
    Uint16   month;
    Uint16   date;
    
    int32    longtitude;
    int32    latitude;
    int32    currstep;
    int32    ofst;
}state_record;

typedef struct factory_setting
{
    int32   delta_x;
    int32   delta_y;
    int32   sensor_type;
    int32   ofst;
}factory_setting;

typedef struct Err_Record
{
    Uint16  RecordFlag;
    Uint16  ErrCode;
    Uint16  year;
    Uint16  monthdate;
    Uint16  time; // 60*hour +min
    int16   gaodujiao;
    int32   currstep;
}Err_Record;

typedef enum enDIRECTION
{
    TURNEAST = 0,
    TURNWEST = 1,
    //COUNTERCLOCKWISE = 0,
    //CLOCKWISE = 1,
    TURNNORTH = 2,
    TURNSOUTH = 3,
    INVALIDDIRECT 
}enDIRECTION;

typedef enum EN_EW_DIRECTION
{
    D_EAST = 0,
    D_WEST = 1,
    D_EW_INVALID
}EN_EW_DIRECTION;

typedef enum EN_NS_DIRECTION
{
    D_NORTH = 0,
    D_SOUTH = 1,
    D_NS_INVALID
}EN_NS_DIRECTION;


typedef enum enAdjustSpeed
{
    SLOWEST = 0,
    SLOWER  = 1,
    FASTER  = 2, 
    FASTEST = 3,
    INVALIDSPEED
}enAdjustSpeed;




typedef enum enCtrlState
{
    IDLE = 0,
    GPS_SEARCHING  = 1,
    VERICAL_CTRL,
    HORIZON_CTRL,
    PSD_CTRL,
    SEMI_SYN,
    NIGHT,
    SEARCH_FAIL
}enCtrlState;




typedef struct stPsdValue
{
    int32   x1;
    int32   x2;
    int32   y1;
    int32   y2;
}stPsdValue;


typedef struct SunInfo{
    int32   resv[2];
    int32   sin_gaodujiao;
    int32   gaodujiao;
    int32   fangweijiao;
    Uint16  time;
    Uint16  speed;
    int32   CurrStep;
    int32   Offset;
    int32   resv1;
}SunInfo;


typedef struct stOrigonP
{
    Uint16   oXp;
    Uint16   oYp;
}stOrigonP;


typedef struct stRecord{
    int32   Header;
    Uint16  fangweijiao;
    Uint16  time;
    Uint16  rsv;
    Uint16  speed;
    Uint32  currentstep;
    Uint32  Offset;
    int32   resv1;
}stRecord;



typedef struct stContrlInfo
{
    EN_EW_DIRECTION   enEWDirection;
    EN_NS_DIRECTION   enNSDirection;
    enAdjustSpeed     enSpeed;
    
}stContrlInfo;


#define MAX_THRESHLOD_FOR_PSD   3200
//#define MIN_THRESHLOD_FOR_PSD   2000


extern int32 gOffset;
extern int gDirection;


extern volatile int32    gSteadCount;


extern stPsdValue gstPsd;


extern volatile int32   gManualCtrl;
extern volatile Uint16   ggpioflag;
extern volatile Uint16   ggpioflag1;
extern volatile int32   gCounters;
extern volatile int32   gCounters1;
extern volatile int32   grunningtime;
extern volatile int32   grunningtime1;


extern volatile int32  gTestVertical;
extern volatile int32  gTestHorizont;



extern int32   gPsdValue[4];
extern volatile int32   gXvalue;
extern volatile int32   gYvalue;

extern int32   gLostTime;


extern  int32   gTargetStep;
extern  int32   gTargetAccumValue;
extern volatile int32   gCurrentStep;
extern volatile int32   myCurrentStep;


extern volatile int gGpsOK;
extern volatile short gaMPUData1_1;
extern int thr_h;
extern int thr_l;
extern int gAutoReset;

extern char GPSBuffer[256];

int32 ValueDifferenceNotInRange(int32 first, int32 second, int32 range, int minvalue);
void SetHorizontalDirection(enDIRECTION enDirection);
void SetVerticalDirection(enDIRECTION enDirection);


extern void findmaxmin(int32*    pwADCData, int len, int32* p_min, int32* p_max);
extern int PSDValueJudge(int32*    pwADCData);
extern void DataInit();
extern   Uint16 EEPROM_I2CA_WriteData(unsigned char *str,Uint16 RomAddress,Uint16 num);
extern     Uint16 EEPROM_I2CA_ReadData(unsigned char *RamAddr, Uint16  RomAddress, Uint16 number);


extern    void HoriContrl(enDIRECTION directionEW);


#endif


