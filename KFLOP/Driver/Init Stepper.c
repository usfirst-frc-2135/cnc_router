#define StepperMode
#include "Defines.c"

void init_board()
{
	notset = persist.UserData[SETTINGS_PERSIST] & 0x80000000;
		
	FPGA(STEP_PULSE_LENGTH_ADD)=63;
		
	SetBitDirection(LPT_PIN_1,1);
	SetBitDirection(LPT_PIN_16,1);
	SetBitDirection(LPT_PIN_17,1);

	SetBit(LPT_PIN_1);
	SetBit(LPT_PIN_16);

	printf("(Init Stepper)\n");

	//if	(notset == 0)
	{
		persist.UserData[JOG_SLOW] = 5000;
		persist.UserData[JOG_FAST] = 15000;
		
		persist.UserData[XSoftMin] = -500000;
		persist.UserData[XSoftMax] = 500000;
	}

	chan[PortArray[0]].InputMode=ENCODER_MODE;
	chan[PortArray[0]].OutputMode=STEP_DIR_MODE;
	chan[PortArray[0]].Vel=200000.000000;
	chan[PortArray[0]].Accel=250000.000000;
	chan[PortArray[0]].Jerk=300000.000000;

	if	(notset == 0)
	{
		persist.UserData[3*PortArray[0]+1]=0.000*1000000;//p
		persist.UserData[3*PortArray[0]+2]=0.010*1000000;//i
		persist.UserData[3*PortArray[0]+3]=0.000*1000000;//d
	}
	chan[PortArray[0]].P=(double)persist.UserData[3*PortArray[0]+1]/1000000;
	chan[PortArray[0]].I=(double)persist.UserData[3*PortArray[0]+2]/1000000;
	chan[PortArray[0]].D=(double)persist.UserData[3*PortArray[0]+3]/1000000;

	//chan[PortArray[0]].P=0.000000;
	//chan[PortArray[0]].I=0.010000;
	//chan[PortArray[0]].D=0.000000;
	chan[PortArray[0]].FFAccel=0.000000;
	chan[PortArray[0]].FFVel=0.000000;
	chan[PortArray[0]].MaxI=200.000000;
	chan[PortArray[0]].MaxErr=1000000.000000;
	chan[PortArray[0]].MaxOutput=200.000000;
	chan[PortArray[0]].DeadBandGain=1.000000;
	chan[PortArray[0]].DeadBandRange=0.000000;
	chan[PortArray[0]].InputChan0=0;
	chan[PortArray[0]].InputChan1=0;
	//chan[PortArray[0]].OutputChan0=8;	// Pin 8,9 (Ch0)
	chan[PortArray[0]].OutputChan0=12;	// Pin 36,37 (Ch4)
	chan[PortArray[0]].OutputChan1=0;
	chan[PortArray[0]].LimitSwitchOptions=0x0;
	chan[PortArray[0]].InputGain0=1.000000;
	chan[PortArray[0]].InputGain1=1.000000;
	chan[PortArray[0]].InputOffset0=0.000000;
	chan[PortArray[0]].InputOffset1=0.000000;
	chan[PortArray[0]].invDistPerCycle=1.000000;
	chan[PortArray[0]].Lead=0.000000;
	chan[PortArray[0]].MaxFollowingError=1000000000.000000;
	chan[PortArray[0]].StepperAmplitude=20.000000;
	chan[PortArray[0]].OutputGain = -1.0;

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

	chan[PortArray[0]].iir[2].B0=0.000769;
	chan[PortArray[0]].iir[2].B1=0.001538;
	chan[PortArray[0]].iir[2].B2=0.000769;
	chan[PortArray[0]].iir[2].A1=1.920810;
	chan[PortArray[0]].iir[2].A2=-0.923885;
    EnableAxisDest(PortArray[0],0);

	chan[PortArray[1]].InputMode=ENCODER_MODE;
	chan[PortArray[1]].OutputMode=STEP_DIR_MODE;
	chan[PortArray[1]].Vel=200000.000000;
	chan[PortArray[1]].Accel=250000.000000;
	chan[PortArray[1]].Jerk=300000.000000;
		
	if	(notset == 0)
	{
		persist.UserData[3*PortArray[1]+1]=0.000*1000000;//p
		persist.UserData[3*PortArray[1]+2]=0.010*1000000;//i
		persist.UserData[3*PortArray[1]+3]=0.000*1000000;//d
	}
	chan[PortArray[1]].P=(double)persist.UserData[3*PortArray[1]+1]/1000000;
	chan[PortArray[1]].I=(double)persist.UserData[3*PortArray[1]+2]/1000000;
	chan[PortArray[1]].D=(double)persist.UserData[3*PortArray[1]+3]/1000000;

	//chan[PortArray[1]].P=0.000000;
	//chan[PortArray[1]].I=0.010000;
	//chan[PortArray[1]].D=0.000000;
	chan[PortArray[1]].FFAccel=0.000000;
	chan[PortArray[1]].FFVel=0.000000;
	chan[PortArray[1]].MaxI=200.000000;
	chan[PortArray[1]].MaxErr=1000000.000000;
	chan[PortArray[1]].MaxOutput=200.000000;
	chan[PortArray[1]].DeadBandGain=1.000000;
	chan[PortArray[1]].DeadBandRange=0.000000;
	chan[PortArray[1]].InputChan0=1;
	chan[PortArray[1]].InputChan1=0;
	//chan[PortArray[1]].OutputChan0=9;	//Pin 10,11 (Ch 1)
	chan[PortArray[1]].OutputChan0=13;	//Pin 38,39 (Ch 5)
	chan[PortArray[1]].OutputChan1=0;	
	chan[PortArray[1]].LimitSwitchOptions=0x0;
	chan[PortArray[1]].InputGain0=1.000000;
	chan[PortArray[1]].InputGain1=1.000000;
	chan[PortArray[1]].InputOffset0=0.000000;
	chan[PortArray[1]].InputOffset1=0.000000;
	chan[PortArray[1]].invDistPerCycle=1.000000;
	chan[PortArray[1]].Lead=0.000000;
	chan[PortArray[1]].MaxFollowingError=1000000000.000000;
	chan[PortArray[1]].StepperAmplitude=20.000000;
	chan[PortArray[1]].OutputGain = -1.0;

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

	chan[PortArray[1]].iir[2].B0=0.000769;
	chan[PortArray[1]].iir[2].B1=0.001538;
	chan[PortArray[1]].iir[2].B2=0.000769;
	chan[PortArray[1]].iir[2].A1=1.920810;
	chan[PortArray[1]].iir[2].A2=-0.923885;
    EnableAxisDest(PortArray[1],0);
	
	
	chan[PortArray[2]].InputMode=ENCODER_MODE;
	chan[PortArray[2]].OutputMode=STEP_DIR_MODE;
	chan[PortArray[2]].Vel=200000.000000;
	chan[PortArray[2]].Accel=250000.000000;
	chan[PortArray[2]].Jerk=300000.000000;

	if	(notset == 0)
	{
		persist.UserData[3*PortArray[2]+1]=0.000*1000000;//p
		persist.UserData[3*PortArray[2]+2]=0.010*1000000;//i
		persist.UserData[3*PortArray[2]+3]=0.000*1000000;//d
	}
	chan[PortArray[2]].P=(double)persist.UserData[3*PortArray[2]+1]/1000000;
	chan[PortArray[2]].I=(double)persist.UserData[3*PortArray[2]+2]/1000000;
	chan[PortArray[2]].D=(double)persist.UserData[3*PortArray[2]+3]/1000000;

	//chan[PortArray[2]].P=0.000000;
	//chan[PortArray[2]].I=0.010000;
	//chan[PortArray[2]].D=0.000000;
	chan[PortArray[2]].FFAccel=0.000000;
	chan[PortArray[2]].FFVel=0.000000;
	chan[PortArray[2]].MaxI=200.000000;
	chan[PortArray[2]].MaxErr=1000000.000000;
	chan[PortArray[2]].MaxOutput=200.000000;
	chan[PortArray[2]].DeadBandGain=1.000000;
	chan[PortArray[2]].DeadBandRange=0.000000;
	chan[PortArray[2]].InputChan0=2;
	chan[PortArray[2]].InputChan1=0;
	//chan[PortArray[2]].OutputChan0=10;	//Pin 12,13 (Ch 2)
	chan[PortArray[2]].OutputChan0=14;	//Pin 40,41 (Ch 6)
	chan[PortArray[2]].OutputChan1=0;
	chan[PortArray[2]].LimitSwitchOptions=0x0;
	chan[PortArray[2]].InputGain0=1.000000;
	chan[PortArray[2]].InputGain1=1.000000;
	chan[PortArray[2]].InputOffset0=0.000000;
	chan[PortArray[2]].InputOffset1=0.000000;
	chan[PortArray[2]].invDistPerCycle=1.000000;
	chan[PortArray[2]].Lead=0.000000;
	chan[PortArray[2]].MaxFollowingError=1000000000.000000;
	chan[PortArray[2]].StepperAmplitude=20.000000;
	chan[PortArray[2]].OutputGain = -1.0;

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

	chan[PortArray[2]].iir[2].B0=0.000769;
	chan[PortArray[2]].iir[2].B1=0.001538;
	chan[PortArray[2]].iir[2].B2=0.000769;
	chan[PortArray[2]].iir[2].A1=1.920810;
	chan[PortArray[2]].iir[2].A2=-0.923885;	
	EnableAxisDest(PortArray[2],0);

	chan[PortArray[3]].InputMode=ENCODER_MODE;
	chan[PortArray[3]].OutputMode=STEP_DIR_MODE;
	chan[PortArray[3]].Vel=200000.000000;
	chan[PortArray[3]].Accel=220000.000000;
	chan[PortArray[3]].Jerk=300000.000000;

	if	(notset == 0)
	{
		persist.UserData[3*PortArray[3]+1]=0.000*1000000;//p
		persist.UserData[3*PortArray[3]+2]=0.010*1000000;//i
		persist.UserData[3*PortArray[3]+3]=0.000*1000000;//d
	}
	chan[PortArray[3]].P=(double)persist.UserData[3*PortArray[3]+1]/1000000;
	chan[PortArray[3]].I=(double)persist.UserData[3*PortArray[3]+2]/1000000;
	chan[PortArray[3]].D=(double)persist.UserData[3*PortArray[3]+3]/1000000;

	//chan[PortArray[3]].P=0.000000;
	//chan[PortArray[3]].I=0.010000;
	//chan[PortArray[3]].D=0.000000;
	chan[PortArray[3]].FFAccel=0.000000;
	chan[PortArray[3]].FFVel=0.000000;
	chan[PortArray[3]].MaxI=200.000000;
	chan[PortArray[3]].MaxErr=1000000.000000;
	chan[PortArray[3]].MaxOutput=200.000000;
	chan[PortArray[3]].DeadBandGain=1.000000;
	chan[PortArray[3]].DeadBandRange=0.000000;
	chan[PortArray[3]].InputChan0=3;
	chan[PortArray[3]].InputChan1=0;
	//chan[PortArray[3]].OutputChan0=11; //Pin 14,15 (Ch 3)
	chan[PortArray[3]].OutputChan0=15; //Pin 42,43 (Ch 7)
	chan[PortArray[3]].OutputChan1=0;
	chan[PortArray[3]].LimitSwitchOptions=0x0;
	chan[PortArray[3]].InputGain0=1.000000;
	chan[PortArray[3]].InputGain1=1.000000;
	chan[PortArray[3]].InputOffset0=0.000000;
	chan[PortArray[3]].InputOffset1=0.000000;
	chan[PortArray[3]].invDistPerCycle=1.000000;
	chan[PortArray[3]].Lead=0.000000;
	chan[PortArray[3]].MaxFollowingError=1000000000.000000;
	chan[PortArray[3]].StepperAmplitude=20.000000;
	chan[PortArray[3]].OutputGain = 1.0;

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

	chan[PortArray[3]].iir[2].B0=0.000769;
	chan[PortArray[3]].iir[2].B1=0.001538;
	chan[PortArray[3]].iir[2].B2=0.000769;
	chan[PortArray[3]].iir[2].A1=1.920810;
	chan[PortArray[3]].iir[2].A2=-0.923885;	
	EnableAxisDest(PortArray[3],0);
	
	DefineCoordSystem(0,1,2,-1);
}
