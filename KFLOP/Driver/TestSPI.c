#include "SPI.c"

void main() 
{
	printf("(Init Servo)\n");
	SPI_Init();
	int i;
	printf("Begin\n");
	while (1)
	{
		i = SPI_OUT(0xff);
		printf("%d\n",i);
		Delay_sec(.5);
		SetStateBit(1048,(i>>7)&0x01);
		SetStateBit(1049,(i>>6)&0x01);
		SetStateBit(1050,(i>>5)&0x01);
		SetStateBit(1051,(i>>4)&0x01);
		SetStateBit(1052,(i>>3)&0x01);
		SetStateBit(1053,(i>>2)&0x01);
		SetStateBit(1054,(i>>1)&0x01);
		SetStateBit(1055,(i)&0x01);
	}
}