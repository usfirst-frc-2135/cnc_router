#include "Defines.c"

int SPI_Init()
{
	SetBitDirection(pSS,1);
	SetBitDirection(pMOSI,1);
	SetBitDirection(pMISO,0);
	SetBitDirection(pSCK,1);
}

int SPI_OUT(int data)
{
	int i;
	int dataIn;
	
	dataIn = 0;
	ClearBit(pSS);
	for (i = 0; i < 8; i++)
	{
		if (data & 0x80)	// look at high bit
			SetBit(pMOSI);
		else
			ClearBit(pMOSI);
		data = data << 1;
		
		dataIn = dataIn << 1;
		if (ReadBit(pMISO))
			dataIn |= 0x01;
		
		SetBit(pSCK);
		ClearBit(pSCK);	
	}
	SetBit(pSS);
	
	return dataIn;
}

