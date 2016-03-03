//
//  yhmath.c
//  AstronomyGPS
//
//  Created by young on 14-5-2.
//  Copyright (c) 2014年 Development. All rights reserved.
//

#include "yhmath.h"
#include "cordic-32bit.h"

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


float YH_ASIN(float x){
    float value = 0;
    if ( x > 0)
        value = ASIN_TABLE[(int)(x*100)];
    else
        value = -ASIN_TABLE[(int)(-x*100)];
    return value;
}

double RAW_SIN(double degree){
    double radian = 0;
    int32 s, c;
    s = 0;
    c = 0;
    radian = degree * PI / 180;
    // iterate for 16 times, precision can reach 0.0000
    cordic((radian*MUL), &s, &c, 16);
    
    return s/MUL;
}

// this func can process degree between -360 to 360
double YH_SIN(double degree){
    int flag = 1;
    double converted;
    double uniform;
    
    if(degree < 0) {
        flag = -1;
        uniform = degree * flag;
    }
    else {
        flag = 1;
        uniform = degree;
    }
    
    if((uniform >= 0) && (uniform <=90)){
        converted = uniform; // no convert
    }
    else if((uniform > 90) && (uniform <=180)){
        converted = 180 - uniform; // sin(x - pi/2) = sin(x)
    }
    else if((uniform > 180) && (uniform <=270)){
        converted = - (uniform - 180);
    }
    else if((uniform > 270) && (uniform <=360)){
        converted = - (360 - uniform);
    }
    return RAW_SIN(flag * converted);
}

double YH_COS(double degree){
    double temp;
    temp = 90-degree;
    if (temp <= -360)
    {
        temp += 360;
    }

    if (temp >= 360)
    {
        temp -= 360;
    }
    return YH_SIN(temp);
}