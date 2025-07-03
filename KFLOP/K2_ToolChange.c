#include "KMotionDef.h"
#include "Driver\Defines.c
//#define DustHoodBit 33
int Current_Tool;
int New_Tool;

main()
{
	SetBit(ToolChangeThreadRunning);
	//printf("Tool Set to %d\n",persist.UserData[NewTool]);  // print the desired speed
	Current_Tool = persist.UserData[CurrentTool];
	New_Tool = persist.UserData[NewTool];
	if (ReadBit(HOMED_BIT) && (Current_Tool != New_Tool) && ReadBit(ACTIVE) && ReadBit(OT_ENABLE))
	{
		SetBit(ToolChangePause);
		ChangeTool(New_Tool);
		persist.UserData[CurrentTool] = Current_Tool;//change current tool to new tool
	}
	Delay_sec(0.3);
	ClearBit(ToolChangePause);
	ClearBit(ToolChangeThreadRunning);
}
int ChangeTool(int toolNum) //1-8
{
	double tool1x = -31.1331;
	double tool1y = -49.39;
	double tool1z = -5.99;//-5.99
	double offsetclearz = 4.0; //must be less than tool1z
	double offsetx = 0.0; //y or x has to be zero
	double offsety = 3.0; 
	double offsetout = 2.0;

	MoveInchesWait(Z, 0.0);  // Z Up to home
	if (Current_Tool > 0)
	{
		MoveInches(X, tool1x + (Current_Tool-1) * offsetx + (offsetx == 0 ? offsetout : 0));
		MoveInchesWait(Y, tool1y + (Current_Tool-1) * offsety + (offsety == 0 ? offsetout : 0));
		//SetBit(DustHoodBit);
		MoveInchesWait(Z, tool1z);  // Z down to rack height
		MoveInches(X, tool1x + (Current_Tool - 1) * offsetx);
		MoveInchesWait(Y, tool1y + (Current_Tool - 1) * offsety);
		Delay_sec(0.5);//Thread.Sleep(500);  //Wait 0.5 sec
		ReleaseTool();
		Delay_sec(0.5);//Thread.Sleep(500);  //Wait 0.5 sec
		MoveInchesWait(Z, tool1z + offsetclearz);  // Z Up to clear height
	}
	if (toolNum > 0)
	{
		MoveInches(X, tool1x + (toolNum - 1) * offsetx);
		MoveInchesWait(Y, tool1y + (toolNum - 1) * offsety);
		if (Current_Tool == 0)
		{
			//SetBit(DustHoodBit);
			ReleaseTool();
		}
		MoveInchesWait(Z, tool1z);  // Z down to rack height
		Delay_sec(0.5);//Thread.Sleep(500);  //Wait 0.5 sec
		LockTool();
		Delay_sec(0.5);//Thread.Sleep(500);  //Wait 0.5 sec
		MoveInches(X, tool1x + (toolNum - 1) * offsetx + (offsetx == 0 ? offsetout : 0));
		MoveInchesWait(Y, tool1y + (toolNum - 1) * offsety + (offsety == 0 ? offsetout : 0));
	}
	else
		LockTool();
	MoveInchesWait(Z, 0.0);  // Z Up to home
	//ClearBit(DustHoodBit);
	Current_Tool = toolNum;
	return 1;
}
void ReleaseTool()
{
	SetBit(13);
	
}
void LockTool()
{
	ClearBit(13);
}

void MoveInches(int axis, double position)
{
	double steps = 0;

	switch (axis)
	{
		case X:
			steps = position * persist.UserData[XSPI];
			break;
		case Y:
			steps = position * persist.UserData[YSPI];
			break;
		case Z:
			steps = position * persist.UserData[ZSPI];
			break;
		case A:
			steps = position * persist.UserData[ASPI];
			break;
	}
	Move(axis,steps);
}

void MoveInchesWait(int axis, double position)
{
	MoveInches(axis,position);
	while(!CheckDoneXYZABC() || !ReadBit(ToolChangePause)) {
		if (!ReadBit(ACTIVE)) //abort the toolchange if active unchecked
		{
			ClearBit(ToolChangeThreadRunning);
			ThreadDone();
		}
			
		Delay_sec(0.01);
		//WaitNextTimeSlice();
	}
}
