#include "Defines.c"

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
		SetBitDirection(FAN,1); 
	  
		SetBitDirection(pOutput_Enable,1);  
		SetBitDirection(EN_DRV, 1);

		printf("(Init Servo)\n");
		
		chan[PortArray[0]].InputMode=ENCODER_MODE;
		chan[PortArray[0]].OutputMode=DC_SERVO_MODE;
		chan[PortArray[0]].Vel=25000.0;
		chan[PortArray[0]].Accel=300000.0;
		chan[PortArray[0]].Jerk=600000.0;

		if	(notset == 0)
		{
			persist.UserData[3*PortArray[0]+1]=0.8*1000000; //p
			persist.UserData[3*PortArray[0]+2]=0.0*1000000;//i
			persist.UserData[3*PortArray[0]+3]=40*1000000;//d
		}
		chan[PortArray[0]].P=(double)persist.UserData[3*PortArray[0]+1]/1000000;
		chan[PortArray[0]].I=(double)persist.UserData[3*PortArray[0]+2]/1000000;
		chan[PortArray[0]].D=(double)persist.UserData[3*PortArray[0]+3]/1000000;

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

		if	(notset == 0)
		{
			persist.UserData[3*PortArray[1]+1]=0.8*1000000; //p
			persist.UserData[3*PortArray[1]+2]=0.0*1000000;//i
			persist.UserData[3*PortArray[1]+3]=40*1000000;//d
		}
		chan[PortArray[1]].P=(double)persist.UserData[3*PortArray[1]+1]/1000000;
		chan[PortArray[1]].I=(double)persist.UserData[3*PortArray[1]+2]/1000000;
		chan[PortArray[1]].D=(double)persist.UserData[3*PortArray[1]+3]/1000000;

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
		
		if	(notset == 0)
		{
			persist.UserData[3*PortArray[2]+1]=0.8*1000000; //p
			persist.UserData[3*PortArray[2]+2]=0*1000000;//i
			persist.UserData[3*PortArray[2]+3]=40*1000000;//d
		}
		chan[PortArray[2]].P=(double)persist.UserData[3*PortArray[2]+1]/1000000;
		chan[PortArray[2]].I=(double)persist.UserData[3*PortArray[2]+2]/1000000;
		chan[PortArray[2]].D=(double)persist.UserData[3*PortArray[2]+3]/1000000;

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
		
		if	(notset == 0)
		{
			persist.UserData[3*PortArray[3]+1]=0.8*1000000; //p
			persist.UserData[3*PortArray[3]+2]=0*1000000;//i
			persist.UserData[3*PortArray[3]+3]=40*1000000;//d
		}
		chan[PortArray[3]].P=(double)persist.UserData[3*PortArray[3]+1]/1000000;
		chan[PortArray[3]].I=(double)persist.UserData[3*PortArray[3]+2]/1000000;
		chan[PortArray[3]].D=(double)persist.UserData[3*PortArray[3]+3]/1000000;

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
