#include "KMotionDef.h"
#include "Defines.c"
int main() 
{
    persist.UserData[HOMING_PERSIST]=0x17;
    ClearBit(HomingStart);
    SetBit(HomingStart);
    return 0;
}
