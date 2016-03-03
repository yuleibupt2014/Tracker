// TI File $Revision: /main/2 $
// Checkin $Date: December 2, 2004   11:55:13 $
//###########################################################################
//
// FILE:	DSP280x_I2C.c
//
// TITLE:	DSP280x SCI Initialization & Support Functions.
//
//###########################################################################
// $TI Release: DSP280x Header Files V1.60 $
// $Release Date: December 3, 2007 $
//###########################################################################

#include "DSP280x_Device.h"     // DSP280x Headerfile Include File
#include "DSP280x_I2c_defines.h"


extern Uint16 Count;
extern Uint16 errbit;

extern Uint8 readtemp[256];
extern Uint8 asdf;


void I2C_readmode(void);
void I2C_writemode(void);
void delay(void);

////////////////////////////////////////////////////////////////////////

void I2C_start(void)

{

	Uint16 i; 

	GpioDataRegs.GPBDAT.bit.GPIO33 = 1;   //I2CSCL=1;
	for(i=0;i<15;i++) asm(" NOP"); //???NOP????????I/O??????
	GpioDataRegs.GPBDAT.bit.GPIO32 = 1;   //I2CSDA=1;
	for(i=0;i<15*2;i++) asm(" NOP"); 
	GpioDataRegs.GPBDAT.bit.GPIO32 = 0; //I2CSDA=0;
	for(i=0;i<15*2;i++) asm(" NOP");
	GpioDataRegs.GPBDAT.bit.GPIO33 = 0; //I2CSCL=0;  
	for(i=0;i<15;i++) asm(" NOP");      

}



void I2C_stop(void)

{
	Uint16 i; 
	GpioDataRegs.GPBDAT.bit.GPIO32 = 0; //I2CSDA=0;
	for(i=0;i<15*2;i++) asm(" NOP"); 
	GpioDataRegs.GPBDAT.bit.GPIO33 = 1;   //I2CSCL=1;
	for(i=0;i<15*2;i++) asm(" NOP");  
	GpioDataRegs.GPBDAT.bit.GPIO32 = 1;   //I2CSDA=1;
	for(i=0;i<15;i++) asm(" NOP");
}



void I2C_writeOne(Uint16 data)

{

	Uint16 i;
	Uint16 j=8;
	while(j--)
	{
		if((data&0x80)==0x80)   
			GpioDataRegs.GPBDAT.bit.GPIO32 = 1; //I2CSDA=1;
		else
			GpioDataRegs.GPBDAT.bit.GPIO32 = 0; //I2CSDA=0;
		for(i=0;i<15;i++) asm(" NOP");
		data<<=1;
		data&=0xff;
		GpioDataRegs.GPBDAT.bit.GPIO33 = 1;   //I2CSCL=1; 
		for(i=0;i<15;i++) asm(" NOP");
		GpioDataRegs.GPBDAT.bit.GPIO33 = 0; //I2CSCL=0;
	}
}



Uint8 I2C_readOne(void)
{
	Uint16 i;
	Uint8 receivebyte=0;
	Uint16 j=8;
	I2C_readmode();//??I2CSDA???
	while(j--)
	{	
		GpioDataRegs.GPBDAT.bit.GPIO33 = 1;   //I2CSCL=1;
		if(GpioDataRegs.GPBDAT.bit.GPIO32 == 0)//IF(I2CSDA==0)
			receivebyte=(receivebyte<<1);
		else
			receivebyte=(receivebyte<<1)|0x01;
		for(i=0;i<15;i++) asm(" NOP");
		GpioDataRegs.GPBDAT.bit.GPIO33 = 0; //I2CSCL=0;
		for(i=0;i<15;i++) asm(" NOP");
	}
	I2C_writemode();//??I2CSDA???
	return receivebyte;
}

void I2C_checkack(void)//?????

{

	Uint16 i;
	I2C_readmode();
	for(i=0;i<15;i++) asm(" NOP");
	GpioDataRegs.GPBDAT.bit.GPIO33 = 1;   //I2CSCL=1;

	for(i=0;i<15;i++) asm(" NOP");

	while(GpioDataRegs.GPBDAT.bit.GPIO32 == 1);//return noack

	{ 

		errbit=0;

	//	goto I2C_out;

	}

	GpioDataRegs.GPBDAT.bit.GPIO33 = 0; //I2CSCL=0;

	for(i=0;i<15;i++) asm(" NOP");

	I2C_writemode();

I2C_out:

	if(errbit==1)

		I2C_stop();	

}



void I2C_returnack(void)

{

	Uint16 i;

	I2C_writemode();

	GpioDataRegs.GPBDAT.bit.GPIO32 = 0; //I2CSDA=0;

	for(i=0;i<15;i++) asm(" NOP");

	GpioDataRegs.GPBDAT.bit.GPIO33 = 1;   //I2CSCL=1;

	for(i=0;i<15;i++) asm(" NOP");

	GpioDataRegs.GPBDAT.bit.GPIO33 = 0; //I2CSCL=0;

	for(i=0;i<15;i++) asm(" NOP");

	I2C_readmode();

}



void I2C_returnnoack(void)

{

	Uint16 i;

	I2C_writemode();

	GpioDataRegs.GPBDAT.bit.GPIO32 = 1;   //I2CSDA=1;

	for(i=0;i<15;i++) asm(" NOP");

	GpioDataRegs.GPBDAT.bit.GPIO33 = 1;    //I2CSCL=1;

	for(i=0;i<15;i++) asm(" NOP");

	GpioDataRegs.GPBDAT.bit.GPIO33 = 0;  // I2CSCL=0;

	for(i=0;i<15;i++) asm(" NOP");

}



void I2C_readmode(void)

{

	EALLOW;

	// GpioMuxRegs.GPADIR.bit.GPIOA14 = 0;   //I2CSDA??
	GpioCtrlRegs.GPBDIR.bit.GPIO32 = 0;
	EDIS;

}



void I2C_writemode(void)

{

	EALLOW;

	//GpioMuxRegs.GPADIR.bit.GPIOA14 = 1;   //I2CSDA??
	GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1;
	EDIS;

}



void I2C_init(void)

{

	EALLOW;
	GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 0;      //SCIB????
	GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 0;
	GpioCtrlRegs.GPBDIR.bit.GPIO33 = 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1;
	// GpioMuxRegs.GPAMUX.all&=0X3fff; //I2C SDA SCL
	// GpioMuxRegs.GPADIR.bit.GPIOA14=0x01; 
	// GpioMuxRegs.GPADIR.bit.GPIOA15=0x01;
	EDIS;   
	I2C_writemode();
	I2C_start();
	I2C_stop();
}



void I2C_write(Uint16 addr,Uint8 data)

{

	Uint16 i;
	//   for(i=0;i<datanumber/PAGENO;i++)//PAGENO=32???????i???
	{
		I2C_start();
		I2C_writeOne(0xa4);
		I2C_checkack();

		if(errbit==0)
		{
			I2C_writeOne(addr/256);
			I2C_checkack();
			if(errbit==0)
			{
				I2C_writeOne(addr%256);
				I2C_checkack();
				if(errbit==0)
				{
					for(Count=0;Count<1;Count++)
					{
						I2C_writeOne(data);
						I2C_checkack();
						if(errbit==1)
							goto I2C_write_OUT;
					}
					I2C_stop();
					delay();
					//addr+=PAGENO;//??????
				}
				else
					goto I2C_write_OUT;
			}
			else
				goto I2C_write_OUT;
		}
		else
			goto I2C_write_OUT;
	}
I2C_write_OUT:
	I2C_init();

}



void I2C_read(Uint16 addr,Uint8 *data)

{

	I2C_start();
	I2C_writeOne(0xa4);
	I2C_checkack();
	I2C_writeOne(addr/256);
	I2C_checkack();
	I2C_writeOne(addr%256);
	I2C_checkack();
	I2C_start();
	I2C_writeOne(0xa5);
	I2C_checkack();
//	for(Count=0;Count<1;Count++)
	{
		*data=I2C_readOne();
		//if(Count==(data-1))
		I2C_returnnoack();//???????????
	//	else
		//	I2C_returnack();
	}
	I2C_stop();	
}



void Scib_xmit(int a)

{
	ScibRegs.SCITXBUF=(a&0xff);             //?????????
	while(ScibRegs.SCICTL2.bit.TXRDY!=1){}; //?????????????
}



void Scib_init(void)

{
	ScibRegs.SCIFFTX.all =0xE040;         //????,??FIFO,??FIFO??
	//??TXFIFINT,
	ScibRegs.SCIFFRX.all =0x2021;         //??FIFO??,??RXFFINT,16?FIFO
	ScibRegs.SCIFFCT.all =0x0000;         //???????
	ScibRegs.SCICCR.all  =0x0007;         //1????,???,?????,
	//??????,??8?    									 																																										
	ScibRegs.SCICTL1.all =0x0003;        //??

	ScibRegs.SCICTL2.all =0x0003;  

	ScibRegs.SCIHBAUD    =0x0001;        //?????9600bps

	ScibRegs.SCILBAUD    =0x00E7;        //?????9600bps

	ScibRegs.SCICTL1.all =0x0023;        //??RESET

}   



void delay(void)

{

	Uint16 i;

	for(i=0;i<0xffff;i++)

	  	asm("NOP");

}

//////////////////////////////////////  


