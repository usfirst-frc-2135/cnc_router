#include "Defines.c"

//-----------------------------------------
// Blink status LED
//-----------------------------------------
Blink()
{
	static double timelast;
	static int index = 0;
	double timenow;
	timenow=Time_sec();
	if (((timenow - timelast)*100)>((persist.UserData[StatusBlink]>>8) & 0xff))
	{
		SetStateBit(StatusLed,(persist.UserData[StatusBlink]>>(index++%8) & 0x01));
		timelast=timenow;
	}
}
