#include "Defines.c"

//-----------------------------------------
void UpdatePersistant(void)
{
#ifdef StepperMode
	if (rev_was != persist.UserData[SETTINGS_PERSIST])
	{
		rev0=persist.UserData[SETTINGS_PERSIST] & 1;
		rev1=persist.UserData[SETTINGS_PERSIST] & 2;
		rev2=persist.UserData[SETTINGS_PERSIST] & 4;
		rev3=persist.UserData[SETTINGS_PERSIST] & 8;
		if (rev0>0)
			chan[PortArray[0]].InputGain0= -1.000000;
		else
			chan[PortArray[0]].InputGain0= 1.000000;
		if (rev1>0)
			chan[PortArray[1]].InputGain0= -1.000000;
		else
			chan[PortArray[1]].InputGain0= 1.000000;
		if (rev2>0)
			chan[PortArray[2]].InputGain0= -1.000000;
		else
			chan[PortArray[2]].InputGain0= 1.000000;
		if (rev3>0)
			chan[PortArray[3]].InputGain0= -1.000000;
		else
			chan[PortArray[3]].InputGain0= 1.000000;
		rev_was = persist.UserData[SETTINGS_PERSIST];
	}
#else	
	if (rev_was != persist.UserData[SETTINGS_PERSIST])
	{
		rev0=persist.UserData[SETTINGS_PERSIST] & 1;
		rev1=persist.UserData[SETTINGS_PERSIST] & 2;
		rev2=persist.UserData[SETTINGS_PERSIST] & 4;
		rev3=persist.UserData[SETTINGS_PERSIST] & 8;
		if (rev0>0)
			chan[PortArray[0]].InputGain0= -1.000000;
		else
			chan[PortArray[0]].InputGain0= 1.000000;
		if (rev1>0)
			chan[PortArray[1]].InputGain0= -1.000000;
		else
			chan[PortArray[1]].InputGain0= 1.000000;
		if (rev2>0)
			chan[PortArray[2]].InputGain0= -1.000000;
		else
			chan[PortArray[2]].InputGain0= 1.000000;
		if (rev3>0)
			chan[PortArray[3]].InputGain0= -1.000000;
		else
			chan[PortArray[3]].InputGain0= 1.000000;
		PortArray[0]=(persist.UserData[SETTINGS_PERSIST] & 0x0300)>>8;
		PortArray[1]=(persist.UserData[SETTINGS_PERSIST] & 0x0c00)>>10;
		PortArray[2]=(persist.UserData[SETTINGS_PERSIST] & 0x3000)>>12;
		PortArray[3]=(persist.UserData[SETTINGS_PERSIST] & 0xc000)>>14;
		init_board();

		rev_was = persist.UserData[SETTINGS_PERSIST];
	}
#endif		
}
