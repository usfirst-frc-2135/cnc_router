#include "KMotionDef.h"
#include "Defines.c"

#include "Init Servo.c"
int main() 
{
	SetBitDirection(pX_SW,1);
	SetBitDirection(pY_SW,1);
	SetBitDirection(pZ_SW,1);
	SetBitDirection(pA_SW,1);
	
	SetBitDirection(INIT_FLAG_BIT,0);
	SetBit(INIT_FLAG_BIT);
		
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
    return 0;
}
