#include "Defines.c"

void Home_Abort_If_Needed(){
	if (!ReadBit(ACTIVE) || !ReadBit(ESTOP_SW)){
		Jog(X,0);
		Jog(Y,0);
		Jog(Z,0);
		Jog(A,0);
		ClearBit(58);
		ThreadDone();
	}
}

int Home_Single(int axis, int sw, int index,int Multiplier, int nomove)
{
//	printf("axis: %d switch: %d\n",axis,sw);
	int index_last;
	int index_this;
	int home_vel = Multiplier * 9000;
	int backup_vel = Multiplier * -1000;
	int offset;
	int SPI; //steps per inch

	if (!ReadBit(ESTOP_SW)) return;

	Jog(axis, home_vel);		

	while (ReadBit(sw)) // wait for switch to change
	{
//		printf("%d\n",test);
		Delay_sec(.001);	// little delay for debounce
		Home_Abort_If_Needed();
		if (CheckDone(axis))
			break;
	}
	Jog(axis,0);		      	// StopMotion

	if (!ReadBit(ESTOP_SW)) return;

	while(!CheckDone(axis)) ;

	index_this = ReadBit(index);
	index_last = index_last;
	Jog(axis, backup_vel);          	// start moving
	while (!ReadBit(sw) || (index_this == index_last) ) 	// wait for switch to change
	{
	//	SetStateBit(46,ReadBit(index));
		index_last = index_this;
		index_this = ReadBit(index);
		Home_Abort_If_Needed();
		if (CheckDone(axis))
			break;
	}
	Jog(axis,0);		      	// StopMotion

	while (!CheckDone(axis)) ;

	Delay_sec(.1);
	Zero(axis);
	Delay_sec(.1);
///*	
	SPI=persist.UserData[XSPI + axis];
	offset=persist.UserData[XHomeOffset + axis] * SPI / 10000 ; //using axis to chose which offset to USER_INPUT_MODE
	//printf("SPI %d\n",SPI);  // print the desired speed
	//printf("Offset %d\n",offset);  // print the desired speed
	if ((offset/SPI) < 2 && (offset/SPI) > -2)
	{
		Home_Abort_If_Needed();
		chan[axis].Position = offset;
		chan[axis].Dest = offset;
		Delay_sec(.1);
		if (nomove == 0)
			Move(axis,0.0);
	}
//*/	
	//EnableAxisDest(axis,0.0);
	persist.UserData[HOMING_PERSIST]&=~(0x01<<axis);
	
	return 1;
}
int Home_YA(int nomove)
{
//	printf("Home_YA\n");
	int indexY_last;
	int indexY_this;
	int indexA_last;
	int indexA_this;
	float temp=0.0;
	int home_vel = Y_Mult * 9000;		//Init Home Speed to switch (fast)
	int backup_vel = Y_Mult * -1000;	//Back-off-swith Speed (slow)
	int offset;
	int SPI; //steps per inch
	char Y_move=0;
	char A_move=0;
	int yindexloc = 0;
	int aindexloc = 0;

	if (!ReadBit(ESTOP_SW)) return;
//printf("y: %d a: %d\n",ReadBit(pY_SW),ReadBit(pA_SW));
	//-- Start Joging if switch is not pressed
	if (ReadBit(pY_SW) || ReadBit(pA_SW)) 
	{
		//temp=home_vel * Y_Mult;
		//printf("vel: %f\n",temp);
		Jog(Y, home_vel);		
		Y_move=1;
		A_move=1;
	}
//printf("waiting for switch\n");
	//-- Check if Either switch gets pressed
	while (ReadBit(pY_SW) && ReadBit(pA_SW)) // wait for one switch to change
	{
		Delay_sec(.001);	// little delay for debounce
		Home_Abort_If_Needed();
		if (CheckDone(Y))
			break;
	}
	Jog(Y,0);		      	// StopMotion
//return;
	if (!ReadBit(ESTOP_SW)) return;

	while(!CheckDone(Y)) ;
//printf("song and dance\n");

	//-- "Song and Dance" to sync A to Y if A is too far off
	int i;
	for(i=0;;i++)
	{
		if (!ReadBit(pY_SW)  && !ReadBit(pA_SW))
			break;
		if (ReadBit(pA_SW))
		{
			MoveRel(A, 50 * Y_Mult);
			printf("a %d\n",i);
		}
		else if (ReadBit(pY_SW))
		{
			MoveRel(A, -50 * Y_Mult);
			MoveRel(Y, 50 * Y_Mult);
			printf("y %d\n",i);
		}
		while(!CheckDoneXYZABC()) ;//wait for moves to finish
		Delay_sec(.5);
		Home_Abort_If_Needed();
		if (i>20)
			ThreadDone();
		
	}
//printf("back off both\n");

	//-- Back away from switch to see swith change and change of Index pulse
	indexY_this = ReadBit(pY_EncInx);
	indexY_last = indexY_last;
	indexA_this = ReadBit(pA_EncInx);
	indexA_last = indexA_last;
	Jog(Y, backup_vel);          	// start moving
//printf("Jog: %d\n",backup_vel);

//SetBit(46);
//	while (((!ReadBit(pY_SW)  || (indexY_this == indexY_last))&&((!ReadBit(pY_SW) || (indexA_this == indexA_last)))   
	while (1)   
	{
		if(!ReadBit(pY_SW) && indexY_this != indexY_last)
			break;
		if(!ReadBit(pA_SW) && indexA_this != indexA_last)
			break;

		indexY_last = indexY_this;
		indexY_this = ReadBit(pY_EncInx);
		indexA_last = indexA_this;
		indexA_this = ReadBit(pA_EncInx);
		Home_Abort_If_Needed();
		if (CheckDone(Y))
			break;

	}
	Jog(Y,0);		      	// StopMotion
//ClearBit(46);

	while (!CheckDone(Y)) ;

	// Determine which swith changed first and zero that axis
	Delay_sec(.1);
	int leftover;

	if (indexY_this == indexY_last) //it found Aindex
	{
		Zero(A);
		leftover=Y;
	}
	else
	{
		Zero(Y);
		leftover=A;
	}
	Delay_sec(.1);
		
	//-- Home the other switch that's left over
//printf("left: %d\n",leftover);
	indexY_this = ReadBit(EncInxArray[leftover]); //not always y this time i'm just reusing the variables
	indexY_last = indexY_this;
//printf("this: %d\nlast: %d\n",indexY_this,indexY_last);
	Jog(Y, backup_vel); //leftover         // start moving for other index
	while ((indexY_this == indexY_last) || !ReadBit(HomeSwitchArray[leftover]))
	{
		indexY_last = indexY_this;
		indexY_this = ReadBit(EncInxArray[leftover]);
		Home_Abort_If_Needed();
		if (CheckDone(Y))
			break;
	}
//printf("this: %d\nlast: %d\n",indexY_this,indexY_last);
	Jog(Y,0);		      	// StopMotion

	Delay_sec(.1);
	Zero(leftover);
	Delay_sec(.1);
///*	

	//-- Apply offset from index pulse to A and Y

	SPI=persist.UserData[XSPI + Y];
	offset=persist.UserData[XHomeOffset + Y] * SPI / 10000 ; //using axis to chose which offset to USE
	if ((offset/SPI) < 2 && (offset/SPI) > -2)
	{
		Home_Abort_If_Needed();
		chan[PortArray[1]].Position = offset;
		chan[PortArray[1]].Dest = offset;
		Delay_sec(.1);
		if (nomove == 0)
			Move(Y,0.0);
	}
	while (!CheckDone(Y));
	SPI=persist.UserData[XSPI + A];
	offset=persist.UserData[XHomeOffset + A] * SPI / 10000 ; //using axis to chose which offset to USE
	if ((offset/SPI) < 2 && (offset/SPI) > -2)
	{
		Home_Abort_If_Needed();
		chan[PortArray[3]].Position = offset;
		chan[PortArray[3]].Dest = offset;
		Delay_sec(.1);
		if (nomove == 0)
			Move(A,0.0);
	}
	while (!CheckDone(A));
//*/	

	//-- Set offset off A to "square" off the gantry
	//EnableAxisDest(axis,0.0);
	int steps = persist.UserData[AHomeMaxDiff];
	printf("steps: %d\n",steps);
	MoveRel(A,steps);
	while (!CheckDone(A)) ;
	Zero(A);
	persist.UserData[HOMING_PERSIST]&=~(0x01<<Y);
	printf("Done\n");
	return 1;
}

//-----------------------------------------
int Home_All(nomove)
{
	int index_last;
	int index_this;
	float temp=0.0;
	char X_move=0;
	char Y_move=0;
	char A_move=0;
	Home_Single(Z, pZ_SW, pZ_EncInx, Z_Mult, nomove); 
	int home_vel = 9000;
	int backup_vel = -1000;
	float zero = 0.0;
	int offset;
	int SPI; //steps per inch
	
	if (!ReadBit(ESTOP_SW)) return;

	if (ReadBit(pX_SW)) {
		temp=home_vel * X_Mult;
		Jog(X, temp);
		X_move=1;
	}		
	if (ReadBit(pY_SW)) {
		temp=home_vel * Y_Mult;
		Jog(Y, temp);		
		Y_move=1;
	}
	if (ReadBit(pA_SW)) {
		temp=home_vel * A_Mult;
		Jog(A, temp);
		A_move=1;
	}
	if (A_Mult!=0){
		while ((ReadBit(pX_SW) || ReadBit(pY_SW) || ReadBit(pA_SW)) && ReadBit(ESTOP_SW) && ReadBit(EN_DRV)) // wait for switchs to change
		{
			Home_Abort_If_Needed();
			Delay_sec(.001);	// little delay for debounce
			if (!ReadBit(pX_SW) && X_move)	
			{
				Jog(X, 0); 
				X_move=0;
			}
			if (!ReadBit(pY_SW) && Y_move)
			{
				Jog(Y, 0); 
				Y_move=0;
			}
			if (!ReadBit(pA_SW) && A_move)
			{
				Jog(A, 0); 
				A_move=0;
			}
		}
	}
	else
	{
		while ( (ReadBit(pX_SW) || ReadBit(pY_SW)) && ReadBit(ESTOP_SW) && ReadBit(EN_DRV)) // wait for switchs to change
		{
			Home_Abort_If_Needed();
			Delay_sec(.001);	// little delay for debounce
			if (!ReadBit(pX_SW) && X_move)
			{
				Jog(X, zero); 
				X_move=0;
			}
			if (!ReadBit(pY_SW) && Y_move)
			{
				Jog(Y,0);
				Y_move=0;
			}
		}
	}

	if (!ReadBit(ESTOP_SW))
	{
		Jog(X,0);
		Jog(Y,0);
		Jog(A,0);
		return;
	}
	while(!CheckDoneXYZABC()) ;

	temp = backup_vel * X_Mult;
	index_this = ReadBit(pX_EncInx);
	index_last = index_last;
	Jog(X, temp);          	// start moving
	while (!ReadBit(pX_SW) || (index_this == index_last) ) 	// wait for switch to change
	{
		Home_Abort_If_Needed();
		index_last = index_this;
		index_this = ReadBit(pX_EncInx);
		if (CheckDone(X))
			break;
	}
	Jog(X,0);		      	// StopMotion

	persist.UserData[HOMING_PERSIST]&=~0x01;
	temp = backup_vel * Y_Mult;
	index_this = ReadBit(pY_EncInx);
	index_last = index_last;
	Jog(Y, temp);          	// start moving
	while (!ReadBit(pY_SW) || (index_this == index_last) ) 	// wait for switch to change
	{
		Home_Abort_If_Needed();
		index_last = index_this;
		index_this = ReadBit(pY_EncInx);
		if (CheckDone(Y))
			break;
	}
	Jog(Y,0);		      	// StopMotion

	persist.UserData[HOMING_PERSIST]&=~0x02;
	temp = backup_vel * A_Mult;
	index_this = ReadBit(pA_EncInx);
	index_last = index_last;
	Jog(A, temp);          	// start moving
	while ((!ReadBit(pA_SW) || (index_this == index_last) ) && pA_SW != 0) 	// wait for switch to change or cancel if axis unused
	{
		Home_Abort_If_Needed();
		index_last = index_this;
		index_this = ReadBit(pA_EncInx);
		if (CheckDone(A))
			break;
	}
	Jog(A,0);		      	// StopMotion

	persist.UserData[HOMING_PERSIST]&=~0x08;

	while (!CheckDoneXYZABC()) ;

	Delay_sec(.4);
	
	Zero(X);
	Delay_sec(.1);
///*
	SPI=persist.UserData[XSPI];
	offset=persist.UserData[XHomeOffset] * SPI / 10000 ; //using axis to chose which offset to USER_INPUT_MODE
	if ((offset/SPI) < 2 && (offset/SPI) > -2)
	{
		Home_Abort_If_Needed();
		chan[PortArray[0]].Position = offset;
		chan[PortArray[0]].Dest = offset;
		Delay_sec(.1);
		if (nomove == 0)
			Move(X,0.0);
	}
	Delay_sec(.1);
//*/
	Zero(Y);
	Delay_sec(.1);
///*
	SPI=persist.UserData[YSPI];
	offset=persist.UserData[YHomeOffset] * SPI / 10000 ; //using axis to chose which offset to USER_INPUT_MODE
	if ((offset/SPI) < 2 && (offset/SPI) > -2)
	{
		Home_Abort_If_Needed();
		chan[PortArray[1]].Position = offset;
		chan[PortArray[1]].Dest = offset;
		Delay_sec(.1);
		if (nomove == 0)
			Move(Y,0.0);
	}
	Delay_sec(.1);
//*/
	Zero(A);
	Delay_sec(.1);
///*
	SPI=persist.UserData[ASPI];
	offset=persist.UserData[AHomeOffset] * SPI / 10000 ; //using axis to chose which offset to USER_INPUT_MODE
	if ((offset/SPI) < 2 && (offset/SPI) > -2)
	{
		Home_Abort_If_Needed();
		chan[PortArray[3]].Position = offset;
		chan[PortArray[3]].Dest = offset;
		Delay_sec(.1);
		if (nomove == 0)
			Move(A,0.0);
	}
	Delay_sec(.1);
//*/
	//EnableAxisDest(X,0.0);
	//EnableAxisDest(Y,0.0);
	//EnableAxisDest(A,0.0);
	HasHomed = 1;
	return 1;
}

Homing_Choose()
{
	ClearBit(CHECK_SOFT_LIMT);

	X_Mult = (persist.UserData[HOMING_PERSIST] & 0x01)>0 ? ((persist.UserData[SETTINGS_PERSIST] & 0x10)>0 ? -1 : 1) : 0;
	Y_Mult = (persist.UserData[HOMING_PERSIST] & 0x02)>0 ? ((persist.UserData[SETTINGS_PERSIST] & 0x20)>0 ? -1 : 1) : 0;
	Z_Mult = (persist.UserData[HOMING_PERSIST] & 0x04)>0 ? ((persist.UserData[SETTINGS_PERSIST] & 0x40)>0 ? -1 : 1) : 0;
	A_Mult = (persist.UserData[HOMING_PERSIST] & 0x08)>0 ? ((persist.UserData[SETTINGS_PERSIST] & 0x80)>0 ? -1 : 1) : 0;
	//printf("x %d, y %d, z %d, a %d\n",X_Mult,Y_Mult,Z_Mult,A_Mult);
	int msg = persist.UserData[HOMING_PERSIST]; 
	if (ReadBit(ACTIVE))
	{
		switch ((msg>>4) & 0x03)
		{
		case 0:
			if (((msg & 0x0f) == 0x0f) || ((msg & 0x0f) == 0x07)) {
				Home_All(msg & 0x40);
			}
			else
			{
				if (msg & 4)
					Home_Single(Z, pZ_SW, pZ_EncInx, Z_Mult, (msg & 0x40));
				if (msg & 1)
					Home_Single(X, pX_SW, pX_EncInx, X_Mult, (msg & 0x40));	
				if (msg & 2)
					Home_Single(Y, pY_SW, pY_EncInx, Y_Mult, (msg & 0x40));
				if (msg & 8)
					Home_Single(A, pA_SW, pA_EncInx, A_Mult, (msg & 0x40));
			}	
			break;
		case 1:
			if (msg & 4)
				Home_Single(Z, pZ_SW, pZ_EncInx, Z_Mult, (msg & 0x40));
			if (msg & 1)
				Home_Single(X, pX_SW, pX_EncInx, X_Mult, (msg & 0x40));	
			if (msg & 2)
				Home_Single(Y, pY_SW, pY_EncInx, Y_Mult, (msg & 0x40));
			if (msg & 8)
				Home_Single(A, pA_SW, pA_EncInx, A_Mult, (msg & 0x40));
			break;
		case 2:
			if (msg & 4)
				Home_Single(Z, pZ_SW, pZ_EncInx, Z_Mult, (msg & 0x40));
			if (msg & 2)
				Home_Single(Y, pY_SW, pY_EncInx, Y_Mult, (msg & 0x40));
			if (msg & 1)
				Home_Single(X, pX_SW, pX_EncInx, X_Mult, (msg & 0x40));	
			if (msg & 8)
				Home_Single(A, pA_SW, pA_EncInx, A_Mult, (msg & 0x40));
			break;
		case 3:
			if (msg & 4)
				Home_Single(Z, pZ_SW, pZ_EncInx, Z_Mult, (msg & 0x40));
			if (msg & 1)
				Home_Single(X, pX_SW, pX_EncInx, X_Mult, (msg & 0x40));	
			if (msg & 2)
			{
				if (msg & 8)
					Home_YA((msg & 0x40));
				else
					Home_Single(Y, pY_SW, pY_EncInx, Y_Mult, (msg & 0x40));	
//				Home_Single(Y, pY_SW, pY_EncInx, Y_Mult, (msg & 0x40));
			}
//			if (msg & 8)
//				Home_Single(A, pA_SW, pA_EncInx, A_Mult, (msg & 0x40));
			break;
		}
		if (((msg & 0x0f) == 0x07) || ((msg & 0x0f) == 0x0f))
			HasHomed = 1;
		persist.UserData[HOMING_PERSIST] = 0;	
		ClearBit(HomingStart);
		SetBit(CHECK_SOFT_LIMT);
	}
}

