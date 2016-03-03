//
//  astronomy.c
//  AstronomyGPS
//
//  Created by young on 14-5-2.
//  Copyright (c) 2014Âπ¥ Development. All rights reserved.
//

#include "gps.h"
#include "astronomy.h"
#include "yhmath.h"

int GetSunInfo(GPS* p_gps, SunInfo* p_si)
{
    int doy;
    int32   minute;
    float time;
    int adjust;
    double shijiao;
    double sin_gaodujiao;
    double sin_chiwei;
    double sin_fangweijiao;
    double chiwei;
    double temp;
    double temp1;
    double temp2;


    doy = DayOfYear(p_gps->year,p_gps->month,p_gps->day);
    minute = p_gps->hour*60 + (float)p_gps->minute;
    temp = (p_gps->longtitude *4 /100);
    minute += (p_gps->ES == 'E') ? (int32)temp : (0 - (int32)temp);
    //shijiao = (time -12) * 15; // calc shijiao
    shijiao = (minute - 60*12)/4;   // Q2    
    
    /*
     Sin_ChiWei=0.39795*cos((0.98563*(DOY(RiQi)-173))*pi/180);% http://baike.baidu.com/view/862819.htm
     ChiWei=asin(Sin_ChiWei)*180/pi;
     
     Sin_GaoDuJiao=sin(WeiDu*pi/180)*sin(ChiWei*pi/180)+cos(WeiDu*pi/180)*cos(ChiWei*pi/180)*cos(ShiJiao*pi/180);
     GaoDuJiao=asin(Sin_GaoDuJiao)*180/pi;
     
     if GaoDuJiao>90
     GaoDuJiao=90-GaoDuJiao;
     end
     
     Sin_FangWeiJiao=cos(ChiWei*pi/180)*sin(ShiJiao*pi/180)/cos(GaoDuJiao*pi/180);http://wenku.baidu.com/view/f2a30343336c1eb91a375dd7.html
     FangWeiJiao=asin(Sin_FangWeiJiao)*180/pi;
     */
    // round(0.98563*(2^15)) = 32297    
    adjust = 0.98563*((doy)-173); // Hudu
    //temp = adjust * pi / 180;
    //    if(adjust <0)
    //        adjust += 360;

    temp = YH_COS(adjust);
    sin_chiwei = 0.39795*temp;
    
    chiwei = YH_ASIN(sin_chiwei);
    
    if (shijiao < 0)
        shijiao += 360;
    
    temp1 = YH_SIN((double)(p_gps->latitude)/100.0) * sin_chiwei;
    
    temp2 =    YH_COS((double)(p_gps->latitude)/100.0) * YH_COS(chiwei) * YH_COS(shijiao);
    
    sin_gaodujiao =  temp1 + temp2;
    
    p_si->gaodujiao = YH_ASIN(sin_gaodujiao);
    
    if(p_si->gaodujiao>90)
        p_si->gaodujiao = 90-p_si->gaodujiao;
    
    sin_fangweijiao = YH_COS(chiwei) * YH_SIN(shijiao) / YH_COS(p_si->gaodujiao);
    
    p_si->fangweijiao = YH_ASIN(sin_fangweijiao);
    p_si->time = minute;
    p_si->sin_gaodujiao = YH_COS(p_si->gaodujiao)*BASIC_ACCE_VALUE;  //’‚¿Ô≤π≥•2∂»
    
    return 0;
}
