// K2_Init Servo.c
//	This file is called when the "Init" button is pressed in KMotionCNC to initialize
//	the axis servos, soft limits, and other customizations. We have modified some
//	settings from the default that is provided by Dynomotion in KMotionCNC and KMotion.
//
//	Since K2CNC has gone out of business, there is no online reference to what the
//	default settings should be for the KG-3925HD-5 and KFLOP controller.
//
//	Note that SPI is Steps Per Inch
//
//	Presentation HS settings:
//		- Set soft limits correctly for our router table size.
//		- Changed X and Y axis PID settings to increase P and decrease D in an effort
//			to allow movements to better reach the target distance (they are falling
//			slightly short by 0.002 to 0.005. Default values #ifdef'd out below.
//		- Changed to enable backlash compensation of 0.003 (32 steps/10403.84 SPI)
//			applied at a rate of 1000 steps per second (1 step per msec)
//		- Commented out code below that allows the drive output to be plotted on an
//			axis in KMotion for tuning PID parameters. Bit 30 in persist.UserData[]

#include "KMotionDef.h"
#include "Driver\Defines.c"

void init_board();

int main() 
{
	printf("Running INIT Program!...\n");
	SetBitDirection(pX_SW,1);
	SetBitDirection(pY_SW,1);
	SetBitDirection(pZ_SW,1);
	SetBitDirection(pA_SW,1);
	SetBitDirection(FAN,1); 

	
	SetBitDirection(INIT_FLAG_BIT,0);
	SetBit(INIT_FLAG_BIT);

	persist.UserData[SETTINGS_PERSIST] = 0x8000E403;
	//	This is a bit which allows the step response to be shown in KMotion for tuning
	//		normally disabled, uncomment to test
	//persist.UserData[SETTINGS_PERSIST] |= 0x40000000;	// Get step response output for tuning

	persist.UserData[XSPI] = 10403.84;        // Steps per inch
	persist.UserData[YSPI] = 10403.84;        // Steps per inch
	persist.UserData[ZSPI] = 10403.84;        // Steps per inch
	persist.UserData[ASPI] = 204.8;           // Steps per inch

	persist.UserData[XSoftMin] = -24.7 * 10;  //inches multiplyed by ten (was -31.3)
	persist.UserData[XSoftMax] = 0.1 * 10;    //inches multiplyed by ten
	persist.UserData[YSoftMin] = -38.8 * 10;  //inches multiplyed by ten (was -50.6)
	persist.UserData[YSoftMax] = 0.1 * 10;    //inches multiplyed by ten
	persist.UserData[ZSoftMin] = -4.5 * 10;   //inches multiplyed by ten (was -7.6)
	persist.UserData[ZSoftMax] = 0.6 * 10;    //inches multiplyed by ten
	
	notset = persist.UserData[SETTINGS_PERSIST] & 0x80000000;
	PortArray[0]=(persist.UserData[SETTINGS_PERSIST] & 0x0300)>>8;
	PortArray[1]=(persist.UserData[SETTINGS_PERSIST] & 0x0c00)>>10;
	PortArray[2]=(persist.UserData[SETTINGS_PERSIST] & 0x3000)>>12;
	PortArray[3]=(persist.UserData[SETTINGS_PERSIST] & 0xc000)>>14;

	if (notset == 0) //settings flag not set
	{
		persist.UserData[JOG_SLOW] = 5000;
		persist.UserData[JOG_FAST] = 30000;
	
		persist.UserData[XSoftMin] = -10000;
		persist.UserData[XSoftMax] = 10000;
		persist.UserData[YSoftMin] = -10000;
		persist.UserData[YSoftMax] = 10000;
		persist.UserData[ZSoftMin] = -10000;
		persist.UserData[ZSoftMax] = 10000;
		persist.UserData[ASoftMin] = -10000;
		persist.UserData[ASoftMax] = 10000;
		persist.UserData[XSPI] = 10000;//Steps Per Inch
		persist.UserData[YSPI] = 10000;
		persist.UserData[ZSPI] = 10000;
		persist.UserData[ASPI] = 10000;
		persist.UserData[SETTINGS_PERSIST] = 0xe400;
		PortArray[0]=0;
		PortArray[1]=1;
		PortArray[2]=2;
		PortArray[3]=3;
	}
	init_board();	
	printf("Stopping INIT Program ...\n");
    return 0;
}

void init_board()
{
		rev_was = persist.UserData[SETTINGS_PERSIST];
		rev0 = persist.UserData[SETTINGS_PERSIST] & 1;
		rev1 = persist.UserData[SETTINGS_PERSIST] & 2;
		rev2 = persist.UserData[SETTINGS_PERSIST] & 4;
		rev3 = persist.UserData[SETTINGS_PERSIST] & 8;
		//notset = persist.UserData[SETTINGS_PERSIST] & 0x80000000;

		FPGA(STEP_PULSE_LENGTH_ADD)=63;
		
		SetBitDirection(pRelay1,1);  		
		SetBitDirection(pRelay2,1);  		
		SetBitDirection(pRelay3,1);  
		SetBitDirection(pRelay4,1);  
	  
		SetBitDirection(pOutput_Enable,1);  
		SetBitDirection(EN_DRV, 1);

		printf("(Init Servo)\n");
		
		chan[PortArray[0]].InputMode=ENCODER_MODE;
		chan[PortArray[0]].OutputMode=DC_SERVO_MODE;
		chan[PortArray[0]].Vel=25000.0;
		chan[PortArray[0]].Accel=300000.0;
		chan[PortArray[0]].Jerk=600000.0;

#if 1
		chan[PortArray[0]].P=0.8;				// Original default settings
		chan[PortArray[0]].I=0;
		chan[PortArray[0]].D=40;
#else
		chan[PortArray[0]].P=1.2;				// (2135) Tuned in KMotion
		chan[PortArray[0]].I=0;					// (2135) 
		chan[PortArray[0]].D=39.6;				// (2135) 
#endif

		chan[PortArray[0]].BacklashMode = 1;	// (2135) Enable backlash correction on X axis
		chan[PortArray[0]].BacklashAmount = 10; // (2135) Correct backlash in X axis of 0.001 in.
		chan[PortArray[0]].BacklashRate = 208;	// (2135) Correct backlash in X axis of 0.001 in. in 50 ms

		chan[PortArray[0]].FFAccel=0.000000;
		chan[PortArray[0]].FFVel=0.006000;
		chan[PortArray[0]].MaxI=127.000000;
		chan[PortArray[0]].MaxErr=127.000000;
		chan[PortArray[0]].MaxOutput=127.000000;
		chan[PortArray[0]].DeadBandGain=1.000000;
		chan[PortArray[0]].DeadBandRange=0.000000;
		chan[PortArray[0]].InputChan0=0;
		chan[PortArray[0]].InputChan1=0;
		chan[PortArray[0]].OutputChan0=0;
		chan[PortArray[0]].OutputChan1=0;
		chan[PortArray[0]].LimitSwitchOptions=0x0;
		if (rev0>0)
			chan[PortArray[0]].InputGain0= -1.000000;
		else
			chan[PortArray[0]].InputGain0= 1.000000;

		chan[PortArray[0]].InputGain1= 1.000000;
		chan[PortArray[0]].InputOffset0=0.000000;
		chan[PortArray[0]].InputOffset1=0.000000;
		chan[PortArray[0]].invDistPerCycle=1.000000;
		chan[PortArray[0]].Lead=0.000000;
		chan[PortArray[0]].MaxFollowingError=2000.000000;
		chan[PortArray[0]].StepperAmplitude=250.000000;

		chan[PortArray[0]].iir[0].B0=1.000000;
		chan[PortArray[0]].iir[0].B1=0.000000;
		chan[PortArray[0]].iir[0].B2=0.000000;
		chan[PortArray[0]].iir[0].A1=0.000000;
		chan[PortArray[0]].iir[0].A2=0.000000;

		chan[PortArray[0]].iir[1].B0=1.000000;
		chan[PortArray[0]].iir[1].B1=0.000000;
		chan[PortArray[0]].iir[1].B2=0.000000;
		chan[PortArray[0]].iir[1].A1=0.000000;
		chan[PortArray[0]].iir[1].A2=0.000000;

		chan[PortArray[0]].iir[2].B0=1.000000;
		chan[PortArray[0]].iir[2].B1=0.000000;
		chan[PortArray[0]].iir[2].B2=0.000000;
		chan[PortArray[0]].iir[2].A1=0.000000;
		chan[PortArray[0]].iir[2].A2=0.000000;
		
		EnableAxisDest(PortArray[0],chan[PortArray[0]].Position);
		
	//-----------------------
		chan[PortArray[1]].InputMode=ENCODER_MODE;
		chan[PortArray[1]].OutputMode=DC_SERVO_MODE;
		
		chan[PortArray[1]].Vel=25000.0;
		chan[PortArray[1]].Accel=300000.0;
		chan[PortArray[1]].Jerk=600000.0;

#if 1
		chan[PortArray[1]].P=0.8; 				// Original default settings
		chan[PortArray[1]].I=0;
		chan[PortArray[1]].D=40;
#else
		chan[PortArray[1]].P=1.2;				// (2135) Tuned in KMotion
		chan[PortArray[1]].I=0;					// (2135)
		chan[PortArray[1]].D=36;				// (2135)
#endif

		chan[PortArray[1]].BacklashMode = 1;	// (2135) Enable backlash correction on X axis
		chan[PortArray[1]].BacklashAmount = 30; // (2135) Correct backlash in Y axis of 0.003 in. in counts
		chan[PortArray[1]].BacklashRate = 624;	// (2135) Correct backlash in Y axis of 0.003 in. in 50 ms

		chan[PortArray[1]].FFAccel=0.000000;
		chan[PortArray[1]].FFVel=0.0060000;
		
		chan[PortArray[1]].MaxI=127.000000;
		chan[PortArray[1]].MaxErr=127.000000;
		chan[PortArray[1]].MaxOutput=127.000000;
		chan[PortArray[1]].DeadBandGain=1.000000;
		chan[PortArray[1]].DeadBandRange=0.000000;
		chan[PortArray[1]].InputChan0=1;
		chan[PortArray[1]].InputChan1=0;
		chan[PortArray[1]].OutputChan0=1;
		chan[PortArray[1]].OutputChan1=0;
		chan[PortArray[1]].LimitSwitchOptions=0x0;
		if (rev1>0)
			chan[PortArray[1]].InputGain0= -1.000000;
		else
			chan[PortArray[1]].InputGain0= 1.000000;

		chan[PortArray[1]].InputGain1=1.000000;
		chan[PortArray[1]].InputOffset0=0.000000;
		chan[PortArray[1]].InputOffset1=0.000000;
		chan[PortArray[1]].invDistPerCycle=1.000000;
		chan[PortArray[1]].Lead=0.000000;
		chan[PortArray[1]].MaxFollowingError=2000.000000;
		chan[PortArray[1]].StepperAmplitude=250.000000;

		chan[PortArray[1]].iir[0].B0=1.000000;
		chan[PortArray[1]].iir[0].B1=0.000000;
		chan[PortArray[1]].iir[0].B2=0.000000;
		chan[PortArray[1]].iir[0].A1=0.000000;
		chan[PortArray[1]].iir[0].A2=0.000000;

		chan[PortArray[1]].iir[1].B0=1.000000;
		chan[PortArray[1]].iir[1].B1=0.000000;
		chan[PortArray[1]].iir[1].B2=0.000000;
		chan[PortArray[1]].iir[1].A1=0.000000;
		chan[PortArray[1]].iir[1].A2=0.000000;

		chan[PortArray[1]].iir[2].B0=1.000000;
		chan[PortArray[1]].iir[2].B1=0.000000;
		chan[PortArray[1]].iir[2].B2=0.000000;
		chan[PortArray[1]].iir[2].A1=0.000000;
		chan[PortArray[1]].iir[2].A2=0.000000;
		
		chan[PortArray[1]].OutputGain = -1;

		EnableAxisDest(PortArray[1],chan[PortArray[1]].Position);
			
	//-----------------------
		chan[PortArray[2]].InputMode=ENCODER_MODE;
		chan[PortArray[2]].OutputMode=DC_SERVO_MODE;
		
		chan[PortArray[2]].Vel=25000.0;
		chan[PortArray[2]].Accel=300000.0;
		chan[PortArray[2]].Jerk=600000.0;
		
		chan[PortArray[2]].P=0.8;
		chan[PortArray[2]].I=0;
		chan[PortArray[2]].D=40;

		chan[PortArray[2]].FFAccel=0.000000;
		chan[PortArray[2]].FFVel=0.006000;
		chan[PortArray[2]].MaxI=127.000000;
		chan[PortArray[2]].MaxErr=127.000000;
		chan[PortArray[2]].MaxOutput=127.000000;
		chan[PortArray[2]].DeadBandGain=1.000000;
		chan[PortArray[2]].DeadBandRange=0.000000;
		chan[PortArray[2]].InputChan0=2;
		chan[PortArray[2]].InputChan1=0;
		chan[PortArray[2]].OutputChan0=2;
		chan[PortArray[2]].OutputChan1=0;
		chan[PortArray[2]].LimitSwitchOptions=0x0;
		if (rev2>0)
			chan[PortArray[2]].InputGain0= -1.000000;
		else
			chan[PortArray[2]].InputGain0= 1.000000;

		chan[PortArray[2]].InputGain1=1.000000;
		chan[PortArray[2]].InputOffset0=0.000000;
		chan[PortArray[2]].InputOffset1=0.000000;
		chan[PortArray[2]].invDistPerCycle=1.000000;
		chan[PortArray[2]].Lead=0.000000;
		chan[PortArray[2]].MaxFollowingError=3000.000000;
		chan[PortArray[2]].StepperAmplitude=250.000000;

		chan[PortArray[2]].iir[0].B0=1.000000;
		chan[PortArray[2]].iir[0].B1=0.000000;
		chan[PortArray[2]].iir[0].B2=0.000000;
		chan[PortArray[2]].iir[0].A1=0.000000;
		chan[PortArray[2]].iir[0].A2=0.000000;

		chan[PortArray[2]].iir[1].B0=1.000000;
		chan[PortArray[2]].iir[1].B1=0.000000;
		chan[PortArray[2]].iir[1].B2=0.000000;
		chan[PortArray[2]].iir[1].A1=0.000000;
		chan[PortArray[2]].iir[1].A2=0.000000;

		chan[PortArray[2]].iir[2].B0=1.000000;
		chan[PortArray[2]].iir[2].B1=0.000000;
		chan[PortArray[2]].iir[2].B2=0.000000;
		chan[PortArray[2]].iir[2].A1=0.000000;
		chan[PortArray[2]].iir[2].A2=0.000000;
		EnableAxisDest(PortArray[2],chan[PortArray[2]].Position);
		
		
	//-----------------------
		chan[PortArray[3]].InputMode=ENCODER_MODE;
		chan[PortArray[3]].OutputMode=DC_SERVO_MODE;
		
		chan[PortArray[3]].Vel=25000.0;
		chan[PortArray[3]].Accel=300000.0;
		chan[PortArray[3]].Jerk=600000.0;
		
		chan[PortArray[3]].P=0.8;
		chan[PortArray[3]].I=0;
		chan[PortArray[3]].D=40;

		chan[PortArray[3]].FFAccel=chan[PortArray[1]].FFAccel;
		chan[PortArray[3]].FFVel=chan[PortArray[1]].FFVel;
		
		chan[PortArray[3]].MaxI=127.000000;
		chan[PortArray[3]].MaxErr=127.000000;
		chan[PortArray[3]].MaxOutput=127.000000;
		chan[PortArray[3]].DeadBandGain=1.000000;
		chan[PortArray[3]].DeadBandRange=0.000000;
		chan[PortArray[3]].InputChan0=3;
		chan[PortArray[3]].InputChan1=0;
		chan[PortArray[3]].OutputChan0=3;
		chan[PortArray[3]].OutputChan1=0;
		chan[PortArray[3]].LimitSwitchOptions=0x0;
		if (rev3>0)
			chan[PortArray[3]].InputGain0= -1.000000;
		else
			chan[PortArray[3]].InputGain0= 1.000000;

		chan[PortArray[3]].InputGain1= 1.000000;
		chan[PortArray[3]].InputOffset0=0.000000;
		chan[PortArray[3]].InputOffset1=0.000000;
		chan[PortArray[3]].invDistPerCycle=1.000000;
		chan[PortArray[3]].Lead=0.000000;
		chan[PortArray[3]].MaxFollowingError = chan[PortArray[1]].MaxFollowingError;
		chan[PortArray[3]].StepperAmplitude=250.000000;

		chan[PortArray[3]].iir[0].B0=1.000000;
		chan[PortArray[3]].iir[0].B1=0.000000;
		chan[PortArray[3]].iir[0].B2=0.000000;
		chan[PortArray[3]].iir[0].A1=0.000000;
		chan[PortArray[3]].iir[0].A2=0.000000;

		chan[PortArray[3]].iir[1].B0=1.000000;
		chan[PortArray[3]].iir[1].B1=0.000000;
		chan[PortArray[3]].iir[1].B2=0.000000;
		chan[PortArray[3]].iir[1].A1=0.000000;
		chan[PortArray[3]].iir[1].A2=0.000000;

		chan[PortArray[3]].iir[2].B0=1.000000;
		chan[PortArray[3]].iir[2].B1=0.000000;
		chan[PortArray[3]].iir[2].B2=0.000000;
		chan[PortArray[3]].iir[2].A1=0.000000;
		chan[PortArray[3]].iir[2].A2=0.000000;
		
	//	chan[PortArray[3]].MasterAxis = 1;
	//	chan[PortArray[3]].SlaveGain = 1;
		
		EnableAxisDest(PortArray[3],chan[PortArray[3]].Position);
		
		DefineCoordSystem(0,1,2,-1);	

		SetBitDirection(26,1);  		// Set bit 26 (PWM 0 as an output)
		SetBitDirection(27,1);  		// Set bit 27 (PWM 1 as an output)
		SetBitDirection(28,1);  		// Set bit 28 (PWM 2 as an output)
		SetBitDirection(29,1);  		// Set bit 29 (PWM 3 as an output)
	//	FPGA(IO_PWMS_PRESCALE) = 1;  	// set pwm period to 30 KHz
		FPGA(IO_PWMS_PRESCALE) = 2;  	// set pwm period to 20 KHz
		FPGA(IO_PWMS+1) = 1;			// enable the PWM0
		FPGA(IO_PWMS+3) = 1;			// enable the PWM1
		FPGA(IO_PWMS+5) = 1;			// enable the PWM2
		FPGA(IO_PWMS+7) = 1;			// enable the PWM3
}
