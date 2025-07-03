#include "Defines.c"

Jog_Bits()
{
	double CurrentSpeed; 
	double SoftLimitBufferZone;
	double SLXL;
	double SLXU;
	double SLYL;
	double SLYU;
	double SLZL;
	double SLZU;
	double SLAL;
	double SLAU;
	if (ReadBit(JOG_SPEED_BIT))//persist.UserData[JOG_FLAGS] & JOG_SPEED_MASK
	{
		//CurrentSpeed = persist.UserData[JOG_FAST];
		CurrentSpeed = (HasHomed==1 || ReadBit(HOMED_OVERRIDE)) ? persist.UserData[JOG_FAST] : persist.UserData[JOG_SLOW];
	}
	else
	{
		CurrentSpeed = persist.UserData[JOG_SLOW];
	}
	SoftLimitBufferZone = CurrentSpeed * (pow(CurrentSpeed,0.05)-1.4);
	SLXL = persist.UserData[XSoftMin] + SoftLimitBufferZone;
	SLXU = persist.UserData[XSoftMax] - SoftLimitBufferZone;
	SLYL = persist.UserData[YSoftMin] + SoftLimitBufferZone;
	SLYU = persist.UserData[YSoftMax] - SoftLimitBufferZone;
	SLZL = persist.UserData[ZSoftMin] + SoftLimitBufferZone;
	SLZU = persist.UserData[ZSoftMax] - SoftLimitBufferZone;
	SLAL = persist.UserData[ASoftMin] + SoftLimitBufferZone;
	SLAU = persist.UserData[ASoftMax] - SoftLimitBufferZone;
	
	if (ReadBit(JOG_X_NEG_BIT) && (XMoving == 0))//(persist.UserData[JOG_FLAGS] & JOG_X_DEC_MASK)
	{
		Jog(X,-CurrentSpeed);
		XMoving = -1;
		//SetBit(47);
	}
	else if (ReadBit(JOG_X_POS_BIT) && (XMoving == 0))//(persist.UserData[JOG_FLAGS] & JOG_X_INC_MASK)
	{
		Jog(X,CurrentSpeed);
		XMoving = 1;
		//SetBit(47);
	}
	else if (XMoving != 0)
	{
		Jog(X,0);
		XMoving = 0;
		//ClearBit(47);
	}
	if (ReadBit(JOG_Y_NEG_BIT) && (YMoving == 0))//(persist.UserData[JOG_FLAGS] & JOG_Y_DEC_MASK)
	{
		Jog(Y,-CurrentSpeed);
		YMoving = -1;
		//SetBit(47);
	}
	else if (ReadBit(JOG_Y_POS_BIT) && (YMoving == 0))//(persist.UserData[JOG_FLAGS] & JOG_Y_INC_MASK)
	{
		Jog(Y,CurrentSpeed);
		YMoving = 1;
		//SetBit(47);
	}
	else if (YMoving != 0)
	{
		Jog(Y,0);
		YMoving = 0;
		//ClearBit(47);
	}
	if (ReadBit(JOG_Z_NEG_BIT) && (ZMoving == 0))//(persist.UserData[JOG_FLAGS] & JOG_Z_DEC_MASK)
	{
		Jog(Z,-CurrentSpeed);
		ZMoving = -1;
		//SetBit(47);
	}
	else if (ReadBit(JOG_Z_POS_BIT) && (ZMoving == 0))//(persist.UserData[JOG_FLAGS] & JOG_Z_INC_MASK)
	{
		Jog(Z,CurrentSpeed);
		ZMoving = 1;
		//SetBit(47);
	}
	else if (ZMoving != 0)
	{
		Jog(Z,0);
		ZMoving = 0;
		//ClearBit(47);
	}
	if (ReadBit(JOG_A_NEG_BIT) && (AMoving == 0))
	{
		Jog(A,-CurrentSpeed);
		AMoving = -1;
	}
	else if (ReadBit(JOG_A_POS_BIT) && (AMoving == 0))
	{
		Jog(A,CurrentSpeed);
		AMoving = 1;
	}
	else if (AMoving != 0)
	{
		Jog(A,0);
		AMoving = 0;
	}
#ifdef StepperMode
///*
	//--- Check Softlimits
	
	while (((ReadBit(JOG_X_NEG_BIT) || ReadBit(JOG_X_POS_BIT) || ReadBit(JOG_Y_NEG_BIT) || ReadBit(JOG_Y_POS_BIT) || ReadBit(JOG_Z_NEG_BIT) || ReadBit(JOG_Z_POS_BIT) || ReadBit(JOG_A_NEG_BIT) || ReadBit(JOG_A_POS_BIT)) && (HasHomed == 1 || ReadBit(HOMED_OVERRIDE))))//(persist.UserData[JOG_FLAGS] & JOG_ALL_MASK)>0)
	{
		if ((chan[PortArray[0]].Dest > SLXU && XMoving == 1) || (chan[PortArray[0]].Dest < SLXL && XMoving == -1))
		{
			if (XMoving == 1)
				Move( X, persist.UserData[XSoftMax]);
			else
				Move( X, persist.UserData[XSoftMin]);
			XMoving = 0;
		}
		if ((chan[PortArray[1]].Dest > SLYU && YMoving == 1) || (chan[PortArray[1]].Dest < SLYL && YMoving == -1))
		{
			if (YMoving == 1)
				Move( Y, persist.UserData[YSoftMax]);
			else
				Move( Y, persist.UserData[YSoftMin]);
			YMoving = 0;
		}
		if ((chan[PortArray[2]].Dest > SLZU && ZMoving == 1) || (chan[PortArray[2]].Dest < SLZL && ZMoving == -1))
		{
			if (ZMoving == 1)
				Move( Z, persist.UserData[ZSoftMax]);
			else
				Move( Z, persist.UserData[ZSoftMin]);
			ZMoving = 0;
		}
		if ((chan[PortArray[3]].Dest > SLAU && AMoving == 1) || (chan[PortArray[3]].Dest < SLAL && AMoving == -1))
		{
			if (AMoving == 1)
				Move( A, persist.UserData[ASoftMax]);
			else
				Move( A, persist.UserData[ASoftMin]);
			AMoving = 0;
		}
	}
//*/
#else //Servo Mode
	while (((ReadBit(JOG_X_NEG_BIT) || ReadBit(JOG_X_POS_BIT) || ReadBit(JOG_Y_NEG_BIT) || ReadBit(JOG_Y_POS_BIT) || ReadBit(JOG_Z_NEG_BIT) || ReadBit(JOG_Z_POS_BIT) || ReadBit(JOG_A_NEG_BIT) || ReadBit(JOG_A_POS_BIT)) && (HasHomed == 1 || ReadBit(HOMED_OVERRIDE))))//(persist.UserData[JOG_FLAGS] & JOG_ALL_MASK)>0)
	{
		if (((int)chan[PortArray[0]].Position > SLXU && XMoving == 1) || ((int)chan[PortArray[0]].Position < SLXL && XMoving == -1))
		{
			if (XMoving == 1)
				Jog(X,(CurrentSpeed * (persist.UserData[XSoftMax]-chan[PortArray[0]].Position)/(persist.UserData[XSoftMax]-SLXU)));
			else
				Jog(X,-(CurrentSpeed * (persist.UserData[XSoftMin]-chan[PortArray[0]].Position)/(persist.UserData[XSoftMin]-SLXL)));
			Delay_sec(.01);
		}
		if (((int)chan[PortArray[1]].Position > SLYU && YMoving == 1) || ((int)chan[PortArray[1]].Position < SLYL && YMoving == -1))
		{
			if (YMoving == 1)
				Jog(Y,(CurrentSpeed * (persist.UserData[YSoftMax]-chan[PortArray[1]].Position)/(persist.UserData[YSoftMax]-SLYU)));
			else
				Jog(Y,-(CurrentSpeed * (persist.UserData[YSoftMin]-chan[PortArray[1]].Position)/(persist.UserData[YSoftMin]-SLYL)));
			Delay_sec(.01);
		}
		if (((int)chan[PortArray[2]].Position > SLZU && ZMoving == 1) || ((int)chan[PortArray[2]].Position < SLZL && ZMoving == -1))
		{
			if (ZMoving == 1)
				Jog(Z,(CurrentSpeed * (persist.UserData[ZSoftMax]-chan[PortArray[2]].Position)/(persist.UserData[ZSoftMax]-SLZU)));
			else
				Jog(Z,-(CurrentSpeed * (persist.UserData[ZSoftMin]-chan[PortArray[2]].Position)/(persist.UserData[ZSoftMin]-SLZL)));
			Delay_sec(.01);
		}
		if (((int)chan[PortArray[3]].Position > SLAU && AMoving == 1) || ((int)chan[PortArray[3]].Position < SLAL && AMoving == -1))
		{
			if (AMoving == 1)
				Jog(A,(CurrentSpeed * (persist.UserData[ASoftMax]-chan[PortArray[3]].Position)/(persist.UserData[ASoftMax]-SLAU)));
			else
				Jog(A,-(CurrentSpeed * (persist.UserData[ASoftMin]-chan[PortArray[3]].Position)/(persist.UserData[ASoftMin]-SLAL)));
			Delay_sec(.01);
		}
	}
#endif			
}

