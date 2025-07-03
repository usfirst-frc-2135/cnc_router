#include "Defines.c"

void StepperCallBack(void)
{
	/*
	// force long callback
	for (i = 0; i < 30; i++)
	{
		ReadBit(ESTOP_SW);
	}
	*/
	
	switch (taskSlice)
	{
	case 0:
		if (ReadBit(ESTOP_SW))
		{
			if (!chan[PortArray[0]].Enable)
			{
				EnableAxis(0);
				EnableAxis(1);
				EnableAxis(2);
				EnableAxis(3);				
			}
			// Charge Pump
			count++;
			if (count == 6)
				SetBit(LPT_PIN_17);
			if (count >= 12)
			{
				ClearBit(LPT_PIN_17);
				count=0;
			}
		}
		else
		{
			DisableAxis(0);
			DisableAxis(1);
			DisableAxis(2);
			DisableAxis(3);
		}
		taskSlice++;
		break;
	case 1:
		SetStateBit(HOMED_BIT,HasHomed); //make Homed_Bit a read only indicator of the homed state
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
	case 2:
		JogFlagState = (ReadBit(JOG_X_NEG_BIT) || ReadBit(JOG_X_POS_BIT) || ReadBit(JOG_Y_NEG_BIT) || ReadBit(JOG_Y_POS_BIT) || ReadBit(JOG_Z_NEG_BIT) || ReadBit(JOG_Z_POS_BIT) || ReadBit(JOG_A_NEG_BIT) || ReadBit(JOG_A_POS_BIT));
		if (JogFlagState != JogFlagStateSav)
		{
			if (ReadBit(47))
				ClearBit(47);
			else
				SetBit(47);

			JogFlagStateSav = JogFlagState;
			persist.UserData[HOMING_PERSIST] = 0x100;
			StartThread(HOMING_THREAD);
		}
		taskSlice++;
		break;
	case 3:
		spiData = SPI_OUT(0);
		
		taskSlice++;
		break;
	case 4:
		if (!ReadBit(Spindle_Running) && !ReadBit(GCode_Running) && !ReadBit(ToolChangeThreadRunning))
		{
			if (ReadBit(Spindle_SW))
				SetBit(SpindleBit);
			else
				ClearBit(SpindleBit);
		}
		
		taskSlice = 0;
		break;
	}
	
}

