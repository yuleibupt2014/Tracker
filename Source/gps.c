//
//  gps.c
//  AstronomyGPS
//
//  Created by young on 14-5-2.
//  Copyright (c) 2014年 Development. All rights reserved.
//


#include "gps.h"

#define BUF_LEN 80
/* Get the num_th , content */
int GetComma(int num,char *str)
{
    int i,j=0;
    int len = BUF_LEN;  // GPSBuffer length is 80
    for(i=0;i<len;i++)
    {
        if(str[i]==',')
        {
            j++;
        }
        
        if(j==num)
            return i+1;
    }
    return 0;
}

int GetDot(int len,char *str)
{
    int i,j=0;

    for(i=0;i<len;i++)
    {
        if(str[i]=='.')
        {
            return i;
        }
    }
    return 0;
}



int DayOfYear(int year, int month, int day){
    int dayth = 0;
    int i = 0;
    int daypermonth[12] = {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };
    if ( year % 4 == 0){
        daypermonth[1] = 29;
    }
    for(i=1; i < month; i++){
        dayth += daypermonth[i-1];
    }
    dayth += day;
    return dayth;
}

int  GpsParse(char* GPS_BUF, GPS* gpsinfo)
{
    char c;
    //char tmp[BUF_LEN];
//    int pos = 1;
    unsigned int start = 0;
    unsigned int end = 0;

    int i = 0, j = 0;
    int32   latitude, longtitude;
    int32   tmp;

    c = GPS_BUF[5];

    if (c != 'C')
        return -1;
    
    if (c == 'C') 
    {
        /*
           格 式： $GPRMC,<1>,<2>,<3>,<4>,<5>,<6>,<7>,<8>,<9>,<10>,<11>,<12>*hh<CR><LF>
           $GPRMC,024813.640,A,3158.4608,N,11848.3737,E,10.05,324.27,150706,,,A*50
         说 明：
          字段 0：$GPRMC，语句ID，表明该语句为Recommended Minimum Specific GPS/TRANSIT Data（RMC）推荐最小定位信息
         字段 1：UTC时间，hhmmss.sss格式
           字段 2：状态，A=定位，V=未定位
              字段 3：纬度ddmm.mmmm，度分格式（前导位数不足则补0）
           字段 4：纬度N（北纬）或S（南纬）
           字段 5：经度dddmm.mmmm，度分格式（前导位数不足则补0）
           字段 6：经度E（东经）或W（西经）
           字段 7：速度，节，Knots（一节也是1.852千米／小时）
         字段 8：方位角，度（二维方向指向，相当于二维罗盘）
         字段 9：UTC日期，DDMMYY格式
         字段10：磁偏角，（000 - 180）度（前导位数不足则补0）
         字段11：磁偏角方向，E=东，W=西
         字段12：模式，A=自动，D=差分，E=估测，N=数据无效（3.0协议内容）
         字段13：校验值
         */
        // "$GPRMC,024813.640,A,3158.4608,N,11848.3737,E,10.05,324.27,150706,,,A*50";
        //  0      7          18        2830         41
        //  $GPRMC,031933.00,A,3958.20733,N,11620.71317,E,0.048,,100514,,,D*76.A*50

        //"GPRMC"
        start = GetComma(1, GPS_BUF); // now for time
        //        end = GetComma(2, GPS_BUF);
        //        tmp = SubString(start, 2);
        gpsinfo->hour = (GPS_BUF[start++] - '0') * 10;
        gpsinfo->hour += (GPS_BUF[start++] - '0');
        gpsinfo->minute = (GPS_BUF[start++] - '0') * 10;
        gpsinfo->minute += (GPS_BUF[start++] - '0');
        gpsinfo->second = (GPS_BUF[start++] - '0') * 10;
        gpsinfo->second += (GPS_BUF[start++] - '0');


        start = GetComma(3, GPS_BUF); // now for latitude (weidu)
        end   = GetDot(8,(GPS_BUF + start));
        latitude = GPS_BUF[start] - '0';
        for (i=start+1; i < (start + end); i ++)
        {
            tmp = GPS_BUF[i] - '0';
            latitude = latitude*10 + tmp;
        }
        gpsinfo->latitude = latitude;

        start = GetComma(4, GPS_BUF); // now for N/S
        gpsinfo->NS = GPS_BUF[start];

        start = GetComma(5, GPS_BUF); // now for longtitude (JingDu)
        end = GetDot(10, GPS_BUF + start);
        
        longtitude = GPS_BUF[start] - '0';
        for (i=start+1; i < (start + end); i ++)
        {
            tmp = GPS_BUF[i] - '0';
            longtitude = longtitude*10 + tmp;
        }
        gpsinfo->longtitude = longtitude;

        start = GetComma(6, GPS_BUF); // now for E/W
        gpsinfo->ES = GPS_BUF[start];


        start = GetComma(9, GPS_BUF); // now for ddmmyy


        gpsinfo->day = (GPS_BUF[start++] - '0') * 10;
        gpsinfo->day += (GPS_BUF[start++] - '0');
        gpsinfo->month = (GPS_BUF[start++] - '0') * 10;
        gpsinfo->month += (GPS_BUF[start++] - '0');
        gpsinfo->year = (GPS_BUF[start++] - '0') * 10;
        gpsinfo->year += (GPS_BUF[start++] - '0');
        gpsinfo->year += 2000;
    
    }
    return 0;
}
