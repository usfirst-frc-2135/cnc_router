void ExecuteCurrent(char InputPin)
{
	static char InputHold = 0;
	static double TimeHold = 0.0;
	if(ReadBit(InputPin) && TimeHold==0)
	{
		TimeHold=Time_sec();
	}
	if(Time_sec()-TimeHold>.05 && ReadBit(InputPin) && !InputHold)
	{
		InputHold=1;
		//printf("Pressed\n");
		persist.UserData[PC_COMM_PERSIST]=PC_COMM_EXECUTE;//tell pc program to execute the current
		TimeHold=0;
	}
	if(Time_sec()-TimeHold>.05 && InputHold && !ReadBit(InputPin)) //Time_sec()-TimeHold>.01
	{
		TimeHold=0;
		InputHold=0;
		persist.UserData[PC_COMM_PERSIST]=0;
		//printf("Released\n");		
	}
		
}