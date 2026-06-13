#include "KMotionDef.h"

#define MAX_RPM 24000.0f
#define PWM_NUMBER 7 //0-7
#define PERSIST_INPUT 95
//#define INVERT

main()
{
	int pwm;
	float speed = *(float *)&persist.UserData[PERSIST_INPUT];  // value stored is actually a float 
	
	pwm = speed/MAX_RPM * 255.0f;
	
	if (pwm > 255) pwm=255;  // limit to max pwm value
#ifndef INVERT
	pwm = 255 - pwm; 	// set the PWM
#endif

	//pwm = 128;//165-175
	
	printf("Spindle Set to %f pwm %d\n",speed, pwm);  // print the desired speed
	SetBitDirection(26+PWM_NUMBER,1);  		// Set bit PWM pin as an output
	FPGA(IO_PWMS+2*PWM_NUMBER+1) = 1;    // enable the PWM
	FPGA(IO_PWMS+2*PWM_NUMBER) = pwm; 	// set the PWM
}
