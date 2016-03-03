/* =================================================================================
File name:       PID_REG.H                      
                    
Originator:	Digital Control Systems Group
			Texas Instruments

Description: 
Header file containing constants, data type definitions, and 
function prototypes for the PIDREG.
=====================================================================================
 History:
-------------------------------------------------------------------------------------
 28-08-2012	Version 1.0
------------------------------------------------------------------------------*/
#ifndef __PIDREG_H__
#define __PIDREG_H__

typedef struct {  float  Ref;   			// Input: Reference input 
				  float  Fdb;   			// Input: Feedback input 
				  float  Err;				// Variable: Error
				  float  Kp;				// Parameter: Proportional gain
				  float  Up;				// Variable: Proportional output 
				  float  Ui;				// Variable: Integral output 
				  float  Ud;				// Variable: Derivative output 	
				  float  OutPreSat; 		// Variable: Pre-saturated output
				  float  OutMax;		    // Parameter: Maximum output 
				  float  OutMin;	    	// Parameter: Minimum output
				  float  Out;   			// Output: PID output 
				  float  SatErr;			// Variable: Saturated difference
				  float  Ki;			    // Parameter: Integral gain
				  float  Kc;		     	// Parameter: Integral correction gain
				  float  Kd; 		        // Parameter: Derivative gain
				  float  Up1;		   	    // History: Previous proportional output
		 	 	  void  (*calc)();		  	// Pointer to calculation function
				 } PIDREG;	            

typedef PIDREG *PIDREG_handle;
/*-----------------------------------------------------------------------------
Default initalizer for the PIDREG3 object.
-----------------------------------------------------------------------------*/                     
#define PIDREG_DEFAULTS { 0, \
                           0, \
                           0, \
                           0.8, \
                           0, \
                           0, \
                           0, \
                           0, \
                           1, \
                           -1, \
                           0, \
                           0, \
                           0.02, \
                           0.5, \
                           0, \
                           0, \
              			  (void (*)(Uint32))pid_reg_calc }

/*------------------------------------------------------------------------------
Prototypes for the functions in PIDREG3.C
------------------------------------------------------------------------------*/
void pid_reg_calc(PIDREG_handle);

#endif // __PIDREG3_H__
