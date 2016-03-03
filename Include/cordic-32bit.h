//
//  cordic-32bit.h
//  AstronomyGPS
//
//  Created by young on 14-5-2.
//  Copyright (c) 2014年 Development. All rights reserved.
//

#ifndef CORDIC_32bit_H
#define CORDIC_32bit_H

#include "Typedef.h"

#define cordic_1K 0x26DD3B6A
#define half_pi 0x6487ED51   // PI*0x20000000
#define MUL 1073741824.000000 //    0x40000000
#define CORDIC_NTAB 32

void cordic(int32 theta, int32 *s, int32 *c, int32 n);

#endif
