#include "KMotionDef.h"
#include "Driver\Defines.c

int main() 
{
	printf("Running HOME Program ...\n");
    persist.UserData[HOMING_PERSIST]=0x17;
    if (ReadBit(HomingStart))
    {
        ClearBit(HomingStart);
        Delay_sec(.5);
    }	
    SetBit(60);
    SetBit(HomingStart);
    printf("Stopping HOME Program ...\n");
    return 0;
}
