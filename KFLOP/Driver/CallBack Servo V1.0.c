#include "Defines.c"
char last22 = 0;
void CallBack(void)
{

	if (chan[PortArray[0]].Enable)
		if (rev0>0)
		{
			FPGA(IO_PWMS+0) = 512 - (chan[PortArray[0]].Output + 128);  // +128 converts to anti-phase
		}
		else
		{
			FPGA(IO_PWMS+0) = (chan[PortArray[0]].Output + 128);  // +128 converts to anti-phase
		}
	else
		FPGA(IO_PWMS+0) = 128;  // whenever not enabled put 50% duty cycle

	if (chan[PortArray[1]].Enable)
		if (rev1>0)
		{
			FPGA(IO_PWMS+2) = 512 - (chan[PortArray[1]].Output + 128);  // +128 converts to anti-phase
		}
		else
		{
			FPGA(IO_PWMS+2) = (chan[PortArray[1]].Output + 128);  // +128 converts to anti-phase
		}
	else
		FPGA(IO_PWMS+2) = 128;  // whenever not enabled put 50% duty cycle

	if (chan[PortArray[2]].Enable)
		if (rev2>0)
		{
			FPGA(IO_PWMS+4) = 512 - (chan[PortArray[2]].Output + 128);  // +128 converts to anti-phase
		}
		else
		{
			FPGA(IO_PWMS+4) = (chan[PortArray[2]].Output + 128);  // +128 converts to anti-phase
		}
	else
		FPGA(IO_PWMS+4) = 128;  // whenever not enabled put 50% duty cycle

	if (chan[PortArray[3]].Enable)
		if (rev3>0)
		{
			FPGA(IO_PWMS+6) = 512 - (chan[PortArray[3]].Output + 128);  // +128 converts to anti-phase
		}
		else
		{
			FPGA(IO_PWMS+6) = (chan[PortArray[3]].Output + 128);  // +128 converts to anti-phase
		}
	else
		FPGA(IO_PWMS+6) = 128;  // whenever not enabled put 50% duty cycle
	

	switch (taskSlice)
	{
	case 0:	
		SetStateBit(HOMED_BIT,HasHomed); //make Homed_Bit a read only indicator of the homed state
		SetStateBit(StatusLed,ReadBit(EN_DRV));
		//Blink();
		if (ReadBit(HomingStart))//(persist.UserData[HOMING_PERSIST] & 0x10)
		{
			if (HomingRunning == 0)
			{
				HomingRunning = 1;
				StartThread(HOMING_THREAD);
			}
		}
		else
		{
			HomingRunning = 0;
		}
		taskSlice++;
		break;
	case 1:
		JogFlagState = (ReadBit(JOG_X_NEG_BIT) || ReadBit(JOG_X_POS_BIT) || ReadBit(JOG_Y_NEG_BIT) || ReadBit(JOG_Y_POS_BIT) || ReadBit(JOG_Z_NEG_BIT) || ReadBit(JOG_Z_POS_BIT) || ReadBit(JOG_A_NEG_BIT) || ReadBit(JOG_A_POS_BIT));
		if (JogFlagState != JogFlagStateSav)
		{
			JogFlagStateSav = JogFlagState;

			persist.UserData[HOMING_PERSIST] = 0x100;
			StartThread(HOMING_THREAD);
		}
		taskSlice++;
		break;
	case 2:
/* // Comment out for Servo Tunning in KMotion Step Response	
		if (HomingRunning == 0)
		{
			if ((!chan[PortArray[0]].Enable) |
				(!chan[PortArray[1]].Enable) |
				(!chan[PortArray[2]].Enable) |
				(!chan[PortArray[3]].Enable))		    
			{
				if (ReadBit(EN_DRV))
				{
					if (!chan[PortArray[0]].Enable)
						SetBit(1040);
					if (!chan[PortArray[1]].Enable)
						SetBit(1041);
					if (!chan[PortArray[2]].Enable)
						SetBit(1042);
					if (!chan[PortArray[3]].Enable)
						SetBit(1043);
				}	
				ClearBit(EN_DRV);
			}		
		}
//*/
		if (!ReadBit(ESTOP_SW)) 
		{	
			fEnable = 0;
			DisableAxis(0);
			DisableAxis(1);
			DisableAxis(2);
			DisableAxis(3);
			//SetBit(EN_DRV);
			ClearBit(EN_DRV);
			ClearBit(pOutput_Enable);
			ClearBit(pRelay1);
			ClearBit(pRelay2);
			ClearBit(pRelay3);
			ClearBit(pRelay4);
			
			ClearBit(1040);
			ClearBit(1041);
			ClearBit(1042);
			ClearBit(1043);
		}
		taskSlice++;
		break;
	case 3:
		if (ReadBit(EN_SW) && time == 0 )
		{	
			time=Time_sec();
		}
		if (ReadBit(EN_SW) && Time_sec()-time >.5 )
		{
			EnableAxis(0);
			EnableAxis(1);
			EnableAxis(2);
			EnableAxis(3);
			//ClearBit(EN_DRV);
			SetBit(EN_DRV);
			SetBit(pOutput_Enable);
			time=0;
		}
		if (!ReadBit(EN_SW) && Time_sec()-time >.5 )
			time=0;
		taskSlice++;
		break;
	case 4:	
		spiData = SPI_OUT(0);
		
			if ((spiData & 0xf0) == 0x00)
			{
				if (spiData & 0x01)
					SetBit(HomeSwitchArray[PortArray[0]]); 
				else
					ClearBit(HomeSwitchArray[PortArray[0]]);
				if (spiData & 0x02)
					SetBit(EncInxArray[PortArray[0]]); 
				else
					ClearBit(EncInxArray[PortArray[0]]);
			}
			if (((spiData & 0xf0) >> 4) == 1)
			{
				if (spiData & 0x01)
					SetBit(HomeSwitchArray[PortArray[1]]); 
				else
					ClearBit(HomeSwitchArray[PortArray[1]]);
				if (spiData & 0x02)
					SetBit(EncInxArray[PortArray[1]]); 
				else
					ClearBit(EncInxArray[PortArray[1]]);
			}
			if ((spiData & 0xf0) == 0x20)
			{
				if (spiData & 0x01)
					SetBit(HomeSwitchArray[PortArray[2]]);
				else
					ClearBit(HomeSwitchArray[PortArray[2]]);
				if (spiData & 0x02)
					SetBit(EncInxArray[PortArray[2]]); 
				else
					ClearBit(EncInxArray[PortArray[2]]);
			}
			if ((spiData & 0xf0) == 0x30)
			{
				if (spiData & 0x01)
					SetBit(HomeSwitchArray[PortArray[3]]); 
				else
					ClearBit(HomeSwitchArray[PortArray[3]]);
				if (spiData & 0x02)
					SetBit(EncInxArray[PortArray[3]]); 
				else
					ClearBit(EncInxArray[PortArray[3]]);
			}
		taskSlice++;
		break;
		case 5:
/*
if (ReadBit(22) != last22)
{
last22 = ReadBit(22);
	if (last22 == 1)//changed to high
		StopCoordinatedMotion();  	// bring any coordinated motion to a emergency stop ASAP
	else
		ResumeCoordinatedMotion();	// resume coordinated/Indep motion after an emergency stop
}
*/
		UpdatePersistant();
		if (!ReadBit(ToolChangeThreadRunning))//(!ReadBit(Spindle_Running) && !ReadBit(GCode_Running) && !ReadBit(ToolChangeThreadRunning))
		{
//			if (ReadBit(Spindle_SW))
//				SetBit(SpindleBit);
//			else
//				ClearBit(SpindleBit);
		}
		taskSlice = 0;
		break;
	}
}

