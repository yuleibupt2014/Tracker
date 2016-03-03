        
        //
        //  main.c
        //  Solar
        //
        //  Created by Li Xiaofei on 14-4-20.
        //  Copyright (c) 2014? YiHua. All rights reserved.
        //
        
#include <stdio.h>
#include <string.h>
        //#include <stdlib.h>
        //#include <math.h>
        
        
        
        int SIN_TABLE[91] = {0,
            71,142,214,285,356,428,499,570,640,711,
            781,851,921,990,1060,1128,1197,1265,1333,1400,
            1467,1534,1600,1665,1731,1795,1859,1922,1985,2047,
            2109,2170,2230,2290,2349,2407,2465,2521,2577,2632,
            2687,2740,2793,2845,2896,2946,2995,3043,3091,3137,
            3183,3227,3271,3313,3355,3395,3435,3473,3510,3547,
            3582,3616,3649,3681,3712,3741,3770,3797,3823,3848,
            3872,3895,3917,3937,3956,3974,3991,4006,4020,4033,
            4045,4056,4065,4073,4080,4086,4090,4093,4095,4096};
        
        int COS_TABLE[91] = {4096,
            4095,4093,4090,4086,4080,4073,4065,4056,4045,4033,
            4020,4006,3991,3974,3956,3937,3917,3895,3872,3848,
            3823,3797,3770,3741,3712,3681,3649,3616,3582,3547,
            3510,3473,3435,3395,3355,3313,3271,3227,3183,3137,
            3091,3043,2995,2946,2896,2845,2793,2740,2687,2632,
            2577,2521,2465,2407,2349,2290,2230,2170,2109,2048,
            1985,1922,1859,1795,1731,1666,1600,1534,1467,1400,
            1333,1265,1197,1129,1060,990,921,851,781,711,
            640,570,499,428,357,285,214,143,71,0};
        
        /*
        long int TAN_TABLE[91] = {0,
            71,143,214,286,358,430,502,575,648,722,
            796,870,945,1021,1097,1174,1252,1330,1410,1490,
            1572,1654,1738,1823,1909,1997,2086,2177,2270,2364,
            2461,2559,2659,2762,2868,2975,3086,3200,3316,3436,
            3560,3687,3819,3955,4095,4241,4392,4548,4711,4881,
            5058,5242,5435,5637,5849,6072,6307,6554,6816,7094,
            7389,7703,8038,8397,8783,9199,9649,10137,10670,11253,
            11895,12605,13396,14283,15285,16427,17740,19268,21070,23227,
            25858,29141,33355,38965,46809,58562,78132,117240,234442,250875938};
        */
        
        float ASIN_TABLE[102] = {
            0.00,
            0.57, 1.15, 1.72, 2.29, 2.87, 3.44, 4.01, 4.59, 5.16, 5.74,
            6.32, 6.89, 7.47, 8.05, 8.63, 9.21, 9.79, 10.37, 10.95, 11.54,
            12.12, 12.71, 13.30, 13.89, 14.48, 15.07, 15.66, 16.26, 16.86, 17.46,
            18.06, 18.66, 19.27, 20.49, 21.10, 21.72, 22.33, 22.95, 23.58, 24.20,
            24.83, 25.47, 26.10, 26.74, 27.39, 28.03, 28.69, 29.34, 30.00, 30.66,
            31.33, 32.01, 32.68, 33.37, 34.06, 34.75, 35.45, 36.16, 36.87, 37.59,
            38.32, 39.05, 39.79, 40.54, 41.30, 42.07, 42.84, 43.63, 44.43, 45.23,
            46.05, 46.89, 47.73, 48.59, 49.46, 50.35, 51.26, 52.19, 53.13, 54.10,
            55.08, 56.10, 57.14, 58.21, 59.32, 60.46, 61.64, 62.87, 64.16, 65.51,
            66.93, 68.43, 70.05, 71.81, 73.74, 75.93, 78.52, 81.89, 89.93, 90.00};
            // if value is 0.23, then value * 100 = 23, and use 23 as index to get value from
            // ASIN_TABLE, result is ASIN_TABLE[23] = 13.30
        
#define SIN(x) (SIN_TABLE[x])/*????90°*/
#define COS(x) (COS_TABLE[x])/*????90°*/
#define pi 3.1415926
        
#define DEBUG 1
        
        //char testStr[] = "$GPGGA,082006.000,3852.9276,N,11527.4283,E,1,08,1.0,20.6,M,,,,0000*35";
        char testStr[] = "$GPRMC,024813.640,A,3158.4608,N,11848.3737,E,10.05,324.27,150706,,,A*50";
        
        
        // start of customized atof()
        int is_digit(char ch)
        {
            if(ch>='0'&&ch<='9')
                return 1;
            else
                return 0;
        }
        int is_space(char ch)
        {
            if(ch==' ')
                return 1;
            else
                return 0;
        }
        double atof(char *s)
        {
            double power,value;
            int i,sign;
            //assert(s!=NULL);//?????????
            for(i=0;is_space(s[i]);i++);//?????????
            sign=(s[i]=='-')?-1:1;
            if(s[i]=='-'||s[i]=='+')//???????????
                i++;
            for(value=0.0;is_digit(s[i]);i++)//?????????
                value=value*10.0+(s[i]-'0');
            if(s[i]=='.')
                i++;
            for(power=1.0;is_digit(s[i]);i++)//?????????
            {
                value=value*10.0+(s[i]-'0');
                power*=10.0;
            }
            return sign*value/power;
        }
        // end of customized atof()
        
        int YH_SIN(x)
        {
            int value = 0;
            if(x >= 0 && x <= 90)
                value = SIN(x);
            else if(x > 90 && x <= 180)
                value = COS(x - 90);
            else if(x > 180 && x <= 270)
                value = - SIN(x - 180);
            else if(x >270 && x <= 360)
                value =  - COS(x - 270);
            return value;
        }
        
        int YH_COS(x)
        {
            int value = 0;
            if( x >= 0 && x <= 90)
                value = COS(x);
            else if(x >90 && x <=180)
                value = - SIN(x - 90);
            else if(x > 180 && x <= 270)
                value = - COS(x - 180);
            else if(x > 270 && x <= 360)
                value = SIN(x - 270);
            return value;
        }
        
        /* Get the num_th , content */
        int GetComma(int num,char *str)
        {
            int i,j=0;
            int len= (int)strlen(str);
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
        
        
        
        struct GPS {
            int year;
            int month;
            int day;
            int hour;
            int minute;
            int second;
            char NS;
            char E;
            double longtitude; // jingdu
            double latitude; // weidu
        };
        
        struct GPS gpsinfo;
        
        //??????????double?
        
        double GetDoubleNumber(char *s)
        {
            char buf[32];
            int i;
            double rev;
            i=GetComma(1,s);
            strncpy(buf,s,i);
            buf[i]=0;
            rev=atof(buf);
            
            return rev;
        }
        
#define BUF_LEN 32
        void GpsParse(char GPS_BUF[])
        {
            char c;
            c = GPS_BUF[5];
            char tmp[BUF_LEN];
            int pos = 1;
            unsigned int start = 0;
            unsigned int end = 0;
            printf("%c\n", c);
            int i = 0, j = 0;
            double d;
            if(c=='A')
            {
                // GPGGA?????????
                gpsinfo.year = 0;
                gpsinfo.month = 0;
                gpsinfo.day = 0;
                /* 
                 ??????
                 $GPGGA?(1)?(2)?(3)?(4)?(5)?(6)?(7)?(8)?(9)?M?(10)?M?(11)?(12)?hh(CR)(LF)
                 ???????????
                 (1) ??UTC ???08 ?20 ?06 ?
                 (2) ??(??ddmm.mmmm:?dd ??mm.mmmm ?)? latitude
                 (3) N/S(?????)???38 ?52.9276 ??
                 (4) ??(??dddmm.mmmm??ddd ??mm.mmmm ?)? longtitude
                 (5) E/W(?????)???115 ?27.4283 ??
                 (6) ????(0=?????1=??GPS?2=??GPS)?1=??GPS?
                 (7) ???????(0?8)????????=08?
                 (8) ??????(1.0?99.9)???????=1.0?
                 (9) ????(?????9999.9?99999.9????m)?????=20.6m);
                 (10) ?????????????(?999.9?9999.9????m):?;
                 (11) ??GPS ???????GPS ??:?;
                 (12) ??????(0000?1023)???GPS ??:?;
                */
                // "$GPGGA,082006.000,3852.9276,N,11527.4283,E,1,08,1.0,20.6,M,,,,0000*35";
                //  0      7          18        2830         41
                //"GPRMC"
                start = GetComma(1, GPS_BUF); // now for time
        //        end = GetComma(2, GPS_BUF);
        //        tmp = SubString(start, 2);
                gpsinfo.hour = (GPS_BUF[start++] - '0') * 10 + (GPS_BUF[start++] - '0');
                gpsinfo.minute = (GPS_BUF[start++] - '0') * 10 + (GPS_BUF[start++] - '0');
                gpsinfo.second = (GPS_BUF[start++] - '0') * 10 + (GPS_BUF[start++] - '0');
#ifdef DEBUG
                printf("%d\n", gpsinfo.hour);
                printf("%d\n", gpsinfo.minute);
                printf("%d\n", gpsinfo.second);
#endif
                start = GetComma(2, GPS_BUF); // now for latitude
                end = GetComma(3, GPS_BUF);
                j = 0;
                for(i=start; i<end -1; i ++){
                    tmp[j++] = GPS_BUF[i];
                }
        //        tmp[j++] = '\n';
                for(;j<BUF_LEN;j++)
                    tmp[j] = 0;
                gpsinfo.latitude = atof(tmp);
#ifdef DEBUG
        //        printf("%d\n", start);
        //        printf("%d\n", end);
                printf("%s\n", tmp);
                printf("%f\n", gpsinfo.latitude);
#endif
                start = GetComma(3, GPS_BUF); // now for N/S
                gpsinfo.NS = GPS_BUF[start];
#ifdef DEBUG
                printf("%c\n", gpsinfo.NS);
#endif
                start = GetComma(4, GPS_BUF); // now for longtitude
                end = GetComma(5, GPS_BUF);
                j = 0;
                for(i=start; i<end -1; i ++){
                    tmp[j++] = GPS_BUF[i];
                }
        //        tmp[j++] = '\n';
        //        tmp[j] = 0;
                for(;j<BUF_LEN;j++)
                    tmp[j] = 0;
                gpsinfo.longtitude = atof(tmp);
        //        d = atof(tmp);
                
#ifdef DEBUG
        //        printf("%f\n", d);
        //        printf("%d\n", start);
        //        printf("%d\n", end);
        //        printf("%s\n", tmp);
                printf("%f\n", gpsinfo.longtitude);
#endif
                start = GetComma(5, GPS_BUF); // now for E/W
                gpsinfo.ES = GPS_BUF[start];
#ifdef DEBUG
        //        printf("%d\n", start);
                printf("%c\n", gpsinfo.ES);
#endif
            }
            
            if (c == 'C') {
                /*
                 ? ?? $GPRMC,<1>,<2>,<3>,<4>,<5>,<6>,<7>,<8>,<9>,<10>,<11>,<12>*hh<CR><LF>
                 $GPRMC,024813.640,A,3158.4608,N,11848.3737,E,10.05,324.27,150706,,,A*50
                 ? ??
                 ?? 0?$GPRMC???ID???????Recommended Minimum Specific GPS/TRANSIT Data?RMC?????????
                 ?? 1?UTC???hhmmss.sss??
                 ?? 2????A=???V=???
                 ?? 3???ddmm.mmmm??????????????0?
                 ?? 4???N?????S????
                 ?? 5???dddmm.mmmm??????????????0?
                 ?? 6???E?????W????
                 ?? 7??????Knots?????1.852??????
                 ?? 8??????????????????????
                 ?? 9?UTC???DDMMYY??
                 ??10??????000 - 180???????????0?
                 ??11???????E=??W=?
                 ??12????A=???D=???E=???N=?????3.0?????
                 ??13????
                 */
                // "$GPRMC,024813.640,A,3158.4608,N,11848.3737,E,10.05,324.27,150706,,,A*50";
                //  0      7          18        2830         41
                //"GPRMC"
                start = GetComma(1, GPS_BUF); // now for time
                //        end = GetComma(2, GPS_BUF);
                //        tmp = SubString(start, 2);
                gpsinfo.hour = (GPS_BUF[start++] - '0') * 10 + (GPS_BUF[start++] - '0');
                gpsinfo.minute = (GPS_BUF[start++] - '0') * 10 + (GPS_BUF[start++] - '0');
                gpsinfo.second = (GPS_BUF[start++] - '0') * 10 + (GPS_BUF[start++] - '0');
#ifdef DEBUG
                printf("%d\n", gpsinfo.hour);
                printf("%d\n", gpsinfo.minute);
                printf("%d\n", gpsinfo.second);
#endif
                start = GetComma(3, GPS_BUF); // now for latitude (weidu)
                end = GetComma(4, GPS_BUF);
                j = 0;
                for(i=start; i<end -1; i ++){
                    tmp[j++] = GPS_BUF[i];
                }
                //        tmp[j++] = '\n';
                for(;j<BUF_LEN;j++)
                    tmp[j] = 0;
                gpsinfo.latitude = atof(tmp);
#ifdef DEBUG
        //        printf("%d\n", start);
        //        printf("%d\n", end);
        //        printf("%s\n", tmp);
                printf("%f\n", gpsinfo.latitude);
#endif
                start = GetComma(4, GPS_BUF); // now for N/S
                gpsinfo.NS = GPS_BUF[start];
#ifdef DEBUG
                printf("%c\n", gpsinfo.NS);
#endif
                start = GetComma(5, GPS_BUF); // now for longtitude (JingDu)
                end = GetComma(6, GPS_BUF);
                j = 0;
                for(i=start; i<end -1; i ++){
                    tmp[j++] = GPS_BUF[i];
                }
                //        tmp[j++] = '\n';
                //        tmp[j] = 0;
                for(;j<BUF_LEN;j++)
                    tmp[j] = 0;
                gpsinfo.longtitude = atof(tmp);
                //        d = atof(tmp);
                
#ifdef DEBUG
        //        printf("%f\n", d);
        //        printf("%d\n", start);
        //        printf("%d\n", end);
                printf("%s\n", tmp);
                printf("%f\n", gpsinfo.longtitude);
#endif
                start = GetComma(6, GPS_BUF); // now for E/W
                gpsinfo.ES = GPS_BUF[start];
#ifdef DEBUG
        //        printf("%d\n", start);
                printf("%c\n", gpsinfo.ES);
#endif
                start = GetComma(9, GPS_BUF); // now for ddmmyy
                //        end = GetComma(2, GPS_BUF);
                //        tmp = SubString(start, 2);
                gpsinfo.day = (GPS_BUF[start++] - '0') * 10 + (GPS_BUF[start++] - '0');
                gpsinfo.month = (GPS_BUF[start++] - '0') * 10 + (GPS_BUF[start++] - '0');
                gpsinfo.year = (GPS_BUF[start++] - '0') * 10 + (GPS_BUF[start++] - '0');
#ifdef DEBUG
                printf("%d\n", gpsinfo.day);
                printf("%d\n", gpsinfo.month);
                printf("%d\n", gpsinfo.year);
#endif
               
            }
            
        }
        double GetLocate(double temp)
        {
            int m;
            double  n;
            m=(int)temp/100;
            n=(temp-m*100)/60;
            n=n+m;
            return n;
            
        }
        
        int DayOfYear(int month, int day){
            int dayth = 0;
            int i = 0;
            int daypermonth[12] = {
              31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
            };
            for(i=1; i < month; i++){
                dayth += daypermonth[i-1];
            }
            dayth += day;
            return dayth;
        }
        
        struct SunInfo{
            double gaodujiao;
            double fangweijiao;
        };
        
        struct SunInfo si;
        
        struct SunInfo* GetSunInfo(struct GPS gps, int doy){
            float time;
            int adjust;
        //    int doy; // Day of Year
            double shijiao;
            double sin_gaodujiao;
            double sin_chiwei;
            double sin_fangweijiao;
            double chiwei;
            double temp;
            
            time = (float)gps.hour + (float)gps.minute/60.;
            shijiao = (time -12) * 15; // calc shijiao
            
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
            
            adjust = 0.98563*(doy)-173;
            if(adjust <0)
                adjust += 360;
            temp = YH_COS(adjust);
            sin_chiwei = 0.39795*temp/4096.;
            
            chiwei = ASIN_TABLE[(int)(sin_chiwei*100)];
            
            sin_gaodujiao = YH_SIN((int)(gps.latitude))/4096. *
                YH_SIN((int)(chiwei))/4096. +
                YH_COS((int)(gps.latitude))/4096. *
                YH_COS((int)(chiwei))/4096. *
                YH_COS((int)(shijiao))/4096.;
            
            si.gaodujiao = ASIN_TABLE[(int)(sin_gaodujiao*100)];
            
            if(si.gaodujiao>90)
                si.gaodujiao = 90-si.gaodujiao;
            
            sin_fangweijiao = YH_COS((int)(chiwei))/4096. *
                YH_SIN((int)(shijiao))/4096. /
                YH_COS((int)(si.gaodujiao))/4096.;
            
            si.fangweijiao = ASIN_TABLE[(int)(sin_fangweijiao*100)];
            
            return &si;
        }
        
        int GetGPSINFO(int argc, const char * argv[])
        {
            int i = 0;
            float value = 0;
            struct GPS gps_test;
            int doy = 0;
            gps_test.hour = 11;
            gps_test.minute = 0;
            gps_test.second = 0;
            gps_test.longtitude = 39.9;
            gps_test.latitude = 120;
            doy = DayOfYear(4, 13);
            
            GetSunInfo(gps_test, doy);
            
            value = (float)YH_SIN(30)/4096;
            
            printf("value is: %f\n", value);
            
            value = (float)YH_COS(45)/4096;
            
            printf("value is: %f\n", value);
            
            GpsParse(testStr);
            // for arc sin table
            value = 0;
        //    for(i=0;i<102;i++){
        //        printf("%d\t%d\t%.4f\n", i, (int)(value*100),asin(value)/pi*180);
        //        printf("%.2f, ", asin(value)/pi*180);
        //        value += 0.01;
        //    }
            // for day of year
            i = DayOfYear(11, 30);
            printf("%d\n", i);
            return 0;
        }
        

