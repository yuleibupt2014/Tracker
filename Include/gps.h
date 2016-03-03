//
//  gps.h
//  AstronomyGPS
//
//  Created by young on 14-5-2.
//  Copyright (c) 2014年 Development. All rights reserved.
//

#ifndef AstronomyGPS_gps_h
#define AstronomyGPS_gps_h

#include  "DSP280x_Device.h"

typedef struct GPS {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    char NS;
    char ES;
    int32 longtitude; // jingdu
    int32 latitude; // weidu
}GPS;

int GpsParse(char* GPS_BUF, GPS* p_GPS);
int DayOfYear(int year, int month, int day);

#endif
