void CheckLimits()
{
	static double SavLastPos[3];
	static double delta[3];

	int i;
	for (i = 0; i < 3; i++) 
	{
		delta[i] = (chan[i].Position - SavLastPos[i]);
		if ((int)chan[i].Position > (int)((float)persist.UserData[XSoftMax+2*i]/10*(float)persist.UserData[XSPI+i]))
		{
			if (delta[i] > 1.0)
			{
				Disable(i);	
			}	
		}
		else if ((int)chan[i].Position < (int)((float)persist.UserData[XSoftMin+2*i]/10*(float)persist.UserData[XSPI+i]))
		{
			if (delta[i] < -1.0)
				Disable(i);				
		}		
		SavLastPos[i] = chan[i].Position; 
	}
}

void Disable(int axis)
{
	DisableAxis(axis);
	ClearBit(EN_DRV);
}