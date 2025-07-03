#ifndef _Defines_
#define _Defines_

//-------------------------------------------------

#define KT1414 1
#define KG2525 2
#define KG3925 3
#define KG5050 4
#define KG9850 5
//-------------------------------------------------
#define MACHINE_TYPE KG3925
//-------------------------------------------------
//-------------------------------------------------


#define LPT_PIN_1 	24	
#define LPT_PIN_16	32
#define LPT_PIN_17	33

#define ESTOP_SW 16
#define STOP_SW  17
#define RUN_SW   18
#define EN_SW 	 19
#define FAN      13

#define pInput1	 20
#define pInput2	 21
#define pInput3	 22
#define pInput4	 23

#define X 0
#define Y 1
#define Z 2
#define A 3

// SPI
#define pSS   8
#define pMOSI 9
#define pMISO 10
#define pSCK  11

#define EN_DRV 	 24
#define pOutput_Enable 25

#define pRelay1 32
#define pRelay2 33
#define pRelay3 34
#define pRelay4 35

//#define Port0 0 //  axis is pluged into port
//#define Port1 1 //
//#define Port2 2 //
//#define Port3 3 //
char PortArray[4] = {0,1,2,3};

#define SETTINGS_PERSIST 0
#define pXP 1
#define pXI 2
#define pXD 3
#define pYP 4
#define pYI 5
#define pYD 6
#define pZP 7
#define pZI 8
#define pZD 9
#define pAP 10
#define pAI 11
#define pAD 12
#define HOMING_PERSIST 13
#define HOMING_THREAD 6
#define XSPI 14 //Steps Per Inch
#define YSPI 15
#define ZSPI 16
#define ASPI 17
#define JOG_SLOW 18
#define JOG_FAST 19
//#define JOG_FLAGS 20
#define XSoftMin 21
#define XSoftMax 22
#define YSoftMin 23
#define YSoftMax 24
#define ZSoftMin 25
#define ZSoftMax 26
#define ASoftMin 27
#define ASoftMax 28
#define XWorking 29 //working zero
#define XWorking 30 //working zero
#define XWorking 31 //working zero
#define XWorking 32 //working zero
#define CurrentTool 33
#define NewTool 34
#define XHomeOffset 35
#define YHomeOffset 36
#define ZHomeOffset 37
#define AHomeOffset 38
#define AHomeMaxDiff 39
#define StatusBlink 40


#define pX_SW 50
#define pY_SW 51
#define pZ_SW 52
#define pA_SW 53

#define pX_EncInx 54
#define pY_EncInx 55
#define pZ_EncInx 56
#define pA_EncInx 57

#define CHECK_SOFT_LIMT 1055

#define JOG_X_NEG_BIT 1024
#define JOG_X_POS_BIT 1025
#define JOG_Y_NEG_BIT 1026
#define JOG_Y_POS_BIT 1027
#define JOG_Z_NEG_BIT 1028
#define JOG_Z_POS_BIT 1029
#define JOG_A_NEG_BIT 1030
#define JOG_A_POS_BIT 1031
#define JOG_SPEED_BIT 1032

#define INIT_FLAG_BIT 63
#define HOMED_BIT 61
#define HOMED_OVERRIDE 62
#define OT_ENABLE 25
#define HomingStart 58
#define ToolChangePause 59
#define ACTIVE 60
#define Spindle_Running 1033
#define GCode_Running 1034
#define Spindle_SW 23
#define SpindleBit 33
#define ToolChangeThreadRunning 1050
#define StatusLed 35 //32 old, 35 new
#define CycleStart 20
#define CycleStartEnable 1054

typedef void USERCALLBACK(void);
extern USERCALLBACK *UserCallBack;

int X_Mult;
int Y_Mult;
int Z_Mult;
int A_Mult;

int HomeSwitchArray[4] = {pX_SW,pY_SW,pZ_SW,pA_SW};
int EncInxArray[4] = {pX_EncInx,pY_EncInx,pZ_EncInx,pA_EncInx};

int XMoving = 0;
int YMoving = 0;
int ZMoving = 0;
int AMoving = 0;

int fEnable;
int rev0;
int rev1;
int rev2;
int rev3;
int rev_was;
int notset;
double time = 0.0;
int count = 0;
int HomingRunning = 0;
int JogRunning = 0;
int JogFlagState = 0;
int JogFlagStateSav = 0;
int HasHomed = 0;

int taskSlice = 0;

int spiData;

int i;
#endif
