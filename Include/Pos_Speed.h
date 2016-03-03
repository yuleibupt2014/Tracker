//###########################################################################
//
// FILE:   Pos_Speed.h
//
// TITLE:  Position and Speed measurement.
//
#ifndef Pos_Speed_H
#define Pos_Speed_H

//全局变量声明
extern Uint32 Pos,Pos_1;
extern Uint16 Dir;
extern _iq spd;
extern float32 angle;

//函数声明
void Pos_Spd_Calc(void);


#endif  // end of DC_Servo_PID_H definition


