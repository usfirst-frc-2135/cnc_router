#include "KMotionDef.h"

void main ()
{
	ClearBit(63);
	UserCallBack = NULL;
	Delay_sec(1); //thread stays green long enough to see it started
}