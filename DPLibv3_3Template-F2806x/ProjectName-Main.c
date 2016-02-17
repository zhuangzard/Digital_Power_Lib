//----------------------------------------------------------------------------------
//	FILE:			ProjectName-Main.C
//
//	Description:	Sample Template file to edit
//					The file drives duty on PWM1A using CLA and PWM2 using C28x
//					These can be deleted and modifed by the user
//					CLA Task is triggered by the PWM 1 interrupt
//					C28x ISR is triggered by the PWM 2 interrupt  
//
//	Version: 		1.0
//
//  Target:  		TMS320F2869,
//
//----------------------------------------------------------------------------------
//  Copyright Texas Instruments � 2004-2010
//----------------------------------------------------------------------------------
//  Revision History:
//----------------------------------------------------------------------------------
//  Date	  | Description / Status
//----------------------------------------------------------------------------------
// Feb 2016  - Sample template project with DPlibv3 (MB)
//----------------------------------------------------------------------------------
//
// PLEASE READ - Useful notes about this Project

// Although this project is made up of several files, the most important ones are:
//	 "{ProjectName}-Main.C"	- this file
//		- Application Initialization, Peripheral config,
//		- Application management
//		- Slower background code loops and Task scheduling
//	 "{ProjectName}-DevInit_F28xxx.C
//		- Device Initialization, e.g. Clock, PLL, WD, GPIO mapping
//		- Peripheral clock enables
//		- DevInit file will differ per each F28xxx device series, e.g. F280x, F2833x,
//	 "{ProjectName}-DPL-ISR.asm
//		- Assembly level library Macros and any cycle critical functions are found here
//	 "{ProjectName}-DPL-CLA.asm"
//		- Init code for DPlib CLA Macros run by C28x
//		- CLA Task code
//	 "{ProjectName}-Settings.h"
//		- Global defines (settings) project selections are found here
//		- This file is referenced by both C and ASM files.
//	 "{ProjectName}-CLAShared.h.h"
//		- Variable defines and header includes that are shared b/w CLA and C28x 
//
// Code is made up of sections, e.g. "FUNCTION PROTOTYPES", "VARIABLE DECLARATIONS" ,..etc
//	each section has FRAMEWORK and USER areas.
//  FRAMEWORK areas provide useful ready made "infrastructure" code which for the most part
//	does not need modification, e.g. Task scheduling, ISR call, GUI interface support,...etc
//  USER areas have functional example code which can be modified by USER to fit their appl.
//
// Code can be compiled with various build options (Incremental Builds IBx), these
//  options are selected in file "{ProjectName}-Settings.h".  Note: "Rebuild All" compile
//  tool bar button must be used if this file is modified.
//----------------------------------------------------------------------------------
#include "ProjectName-Settings.h"
#include "PeripheralHeaderIncludes.h"
#include "F2806x_EPWM_defines.h"		
#include "ProjectName-CLAShared.h"  

#include "DPlib.h"	
#include "IQmathLib.h"
#include "stdint.h"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// FUNCTION PROTOTYPES
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Add protoypes of functions being used in the project here 
void DeviceInit(void);
#ifdef FLASH		
void InitFlash();
#endif
void MemCopy();
void CLA_Init();



// Prototype statements for functions found within this file.
__interrupt void sciaTxFifoIsr(void);
__interrupt void sciaRxFifoIsr(void);
void scia_fifo_init(void);
void error(void);


//-------------------------------- DPLIB --------------------------------------------
void PWM_1ch_CNF(int16 n, int16 period, int16 mode, int16 phase);
void ADC_SOC_CNF(int ChSel[], int Trigsel[], int ACQPS[], int IntChSel,
		int mode);
// -------------------------------- FRAMEWORK --------------------------------------
// State Machine function prototypes
//----------------------------------------------------------------------------------
// Alpha states
void A0(void);	//state A0
void B0(void);	//state B0
void C0(void);	//state C0

// A branch states
void A1(void);	//state A1
void A2(void);	//state A2
void A3(void);	//state A3
void A4(void);	//state A4

// B branch states
void B1(void);	//state B1
void B2(void);	//state B2
void B3(void);	//state B3
void B4(void);	//state B4

// C branch states
void C1(void);	//state C1
void C2(void);	//state C2
void C3(void);	//state C3
void C4(void);	//state C4

// Variable declarations
void (*Alpha_State_Ptr)(void);	// Base States pointer
void (*A_Task_Ptr)(void);		// State pointer A branch
void (*B_Task_Ptr)(void);		// State pointer B branch
void (*C_Task_Ptr)(void);		// State pointer C branch
//----------------------------------------------------------------------------------

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// VARIABLE DECLARATIONS - GENERAL
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

// -------------------------------- FRAMEWORK --------------------------------------

int16 VTimer0[4];					// Virtual Timers slaved off CPU Timer 0
int16 VTimer1[4];					// Virtual Timers slaved off CPU Timer 1
int16 VTimer2[4];					// Virtual Timers slaved off CPU Timer 2

#if (INCR_BUILD == 1)
// Used for ADC Configuration
int ChSel[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int TrigSel[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int ACQPS[16] = {8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8};
#endif

#if (INCR_BUILD == 2)
// Used for running BackGround in flash, and ISR in RAM
extern Uint16 *RamfuncsLoadStart, *RamfuncsLoadEnd, *RamfuncsRunStart;
// Used for copying CLA code from load location to RUN location
extern Uint16 Cla1funcsLoadStart, Cla1funcsLoadEnd, Cla1funcsRunStart;

// Used for ADC Configuration
int ChSel_CLA[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int TrigSel_CLA[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int ACQPS_CLA[16] = { 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8 };
#endif


#if (INCR_BUILD == 3)
// Used for ADC Configuration
int ChSel[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int TrigSel[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int ACQPS[16] = {8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8};

// Used for running BackGround in flash, and ISR in RAM
extern Uint16 *RamfuncsLoadStart, *RamfuncsLoadEnd, *RamfuncsRunStart;
// Used for copying CLA code from load location to RUN location
extern Uint16 Cla1funcsLoadStart, Cla1funcsLoadEnd, Cla1funcsRunStart;

// Used for ADC Configuration
int ChSel_CLA[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int TrigSel_CLA[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int ACQPS_CLA[16] = { 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8 };
#endif

// Used to indirectly access all EPWM modules
volatile struct EPWM_REGS *ePWM[] = {
		&EPwm1Regs,			//intentional: (ePWM[0] not used)
		&EPwm1Regs, &EPwm2Regs, &EPwm3Regs, &EPwm4Regs, &EPwm5Regs, &EPwm6Regs,
		&EPwm7Regs, };

// Used to indirectly access all Comparator modules
volatile struct COMP_REGS *Comp[] = { &Comp1Regs,//intentional: (Comp[0] not used)
		&Comp1Regs, &Comp2Regs, &Comp3Regs, };
// ---------------------------------- USER -----------------------------------------
// ---------------------------- DPLIB Net Pointers ---------------------------------
// Declare net pointers that are used to connect the DP Lib Macros  here 

#if (INCR_BUILD == 1)
// PWMDRV_1ch
extern volatile long *PWMDRV_1ch_Duty2;// instance #2, EPWM2

// CONTROL_2P2Z - instance #1
extern volatile long *CNTL_2P2Z_Ref1;
extern volatile long *CNTL_2P2Z_Out1;
extern volatile long *CNTL_2P2Z_Fdbk1;
extern volatile long *CNTL_2P2Z_Coef1;

// ADCDRV_8ch
extern volatile long *ADCDRV_8ch_RltPtrA;
extern volatile long *ADCDRV_8ch_RltPtrB;
extern volatile long *ADCDRV_8ch_RltPtrC;
extern volatile long *ADCDRV_8ch_RltPtrD;
extern volatile long *ADCDRV_8ch_RltPtrE;
extern volatile long *ADCDRV_8ch_RltPtrF;
extern volatile long *ADCDRV_8ch_RltPtrG;
extern volatile long *ADCDRV_8ch_RltPtrH;
#endif

#if (INCR_BUILD == 2)
// PWMDRV_1ch_CLA
extern volatile float *PWMDRV_1ch_CLA_Duty1;	// instance #1, EPWM1

// CONTROL_2P2Z_CLA - instance #1
extern volatile float *CNTL_2P2Z_CLA_Ref1; // instance #1
extern volatile float *CNTL_2P2Z_CLA_Out1; // instance #1
extern volatile float *CNTL_2P2Z_CLA_Fdbk1; // instance #1
extern volatile float *CNTL_2P2Z_CLA_Coef1; // instance #1

//ADCDRV_8ch_CLA
extern volatile float *ADCDRV_8ch_CLA_RltPtrA;
extern volatile float *ADCDRV_8ch_CLA_RltPtrB;
extern volatile float *ADCDRV_8ch_CLA_RltPtrC;
extern volatile float *ADCDRV_8ch_CLA_RltPtrD;
extern volatile float *ADCDRV_8ch_CLA_RltPtrE;
extern volatile float *ADCDRV_8ch_CLA_RltPtrF;
extern volatile float *ADCDRV_8ch_CLA_RltPtrG;
extern volatile float *ADCDRV_8ch_CLA_RltPtrH;

#endif


#if (INCR_BUILD == 3)
// PWMDRV_1ch
extern volatile long *PWMDRV_1ch_Duty2;// instance #2, EPWM2

// CONTROL_2P2Z - instance #1
extern volatile long *CNTL_2P2Z_Ref1;
extern volatile long *CNTL_2P2Z_Out1;
extern volatile long *CNTL_2P2Z_Fdbk1;
extern volatile long *CNTL_2P2Z_Coef1;

// ADCDRV_8ch
extern volatile long *ADCDRV_8ch_RltPtrA;
extern volatile long *ADCDRV_8ch_RltPtrB;
extern volatile long *ADCDRV_8ch_RltPtrC;
extern volatile long *ADCDRV_8ch_RltPtrD;
extern volatile long *ADCDRV_8ch_RltPtrE;
extern volatile long *ADCDRV_8ch_RltPtrF;
extern volatile long *ADCDRV_8ch_RltPtrG;
extern volatile long *ADCDRV_8ch_RltPtrH;

// PWMDRV_1ch_CLA
extern volatile float *PWMDRV_1ch_CLA_Duty1;	// instance #1, EPWM1

// CONTROL_2P2Z_CLA - instance #1
extern volatile float *CNTL_2P2Z_CLA_Ref1; // instance #1
extern volatile float *CNTL_2P2Z_CLA_Out1; // instance #1
extern volatile float *CNTL_2P2Z_CLA_Fdbk1; // instance #1
extern volatile float *CNTL_2P2Z_CLA_Coef1; // instance #1

//ADCDRV_8ch_CLA
extern volatile float *ADCDRV_8ch_CLA_RltPtrA;
extern volatile float *ADCDRV_8ch_CLA_RltPtrB;
extern volatile float *ADCDRV_8ch_CLA_RltPtrC;
extern volatile float *ADCDRV_8ch_CLA_RltPtrD;
extern volatile float *ADCDRV_8ch_CLA_RltPtrE;
extern volatile float *ADCDRV_8ch_CLA_RltPtrF;
extern volatile float *ADCDRV_8ch_CLA_RltPtrG;
extern volatile float *ADCDRV_8ch_CLA_RltPtrH;

#endif


// ---------------------------- DPLIB Variables ---------------------------------
// Declare the net variables being used by the DP Lib Macro here 
//Two Phase Interleaved PFC PWM Driver CLA
#if (INCR_BUILD == 1) 
volatile long Duty2A;
// DPLIB 2p2z Veriables
long Ref , Fdbk , Out;

#pragma DATA_SECTION(CNTL_2P2Z_CoefStruct1, "CNTL_2P2Z_Coef");
struct CNTL_2P2Z_CoefStruct CNTL_2P2Z_CoefStruct1;

long Pgain, Igain, Dgain, Dmax, Dmin;

// Declare the net variables for ADC
volatile long RltA,RltB,RltC,RltD,RltE,RltF,RltG,RltH;
#endif

#if (INCR_BUILD == 2)
#pragma DATA_SECTION(Duty1A, 		"CpuToCla1MsgRAM");
volatile float Duty1A;

// DPLIB 2p2z_CLA Veriables
#pragma DATA_SECTION(Out_CLA, "Cla1ToCpuMsgRAM");
#pragma DATA_SECTION(Fdbk_CLA,"Cla1ToCpuMsgRAM");
#pragma DATA_SECTION(Ref_CLA, "CpuToCla1MsgRAM");
volatile float Out_CLA, Fdbk_CLA, Ref_CLA;

#pragma DATA_SECTION(CNTL_2P2Z_CLACoefStruct1, "CNTL_2P2Z_CLA_Coef");
struct CNTL_2P2Z_CLACoefStruct CNTL_2P2Z_CLACoefStruct1;

volatile float Pgain_CLA, Igain_CLA, Dgain_CLA, Dmax_CLA, Dmin_CLA;

// Declare the net variables for ADC CLA
#pragma DATA_SECTION(RltA_CLA,"Cla1ToCpuMsgRAM");
#pragma DATA_SECTION(RltB_CLA,"Cla1ToCpuMsgRAM");
#pragma DATA_SECTION(RltC_CLA,"Cla1ToCpuMsgRAM");
#pragma DATA_SECTION(RltD_CLA,"Cla1ToCpuMsgRAM");
#pragma DATA_SECTION(RltE_CLA,"Cla1ToCpuMsgRAM");
#pragma DATA_SECTION(RltF_CLA,"Cla1ToCpuMsgRAM");
#pragma DATA_SECTION(RltG_CLA,"Cla1ToCpuMsgRAM");
#pragma DATA_SECTION(RltH_CLA,"Cla1ToCpuMsgRAM");
volatile float RltA_CLA, RltB_CLA, RltC_CLA, RltD_CLA, RltE_CLA, RltF_CLA,
		RltG_CLA, RltH_CLA;

#endif

#if (INCR_BUILD == 3)
volatile long Duty2A;
// DPLIB 2p2z Veriables
long Ref , Fdbk , Out;

#pragma DATA_SECTION(CNTL_2P2Z_CoefStruct1, "CNTL_2P2Z_Coef");
struct CNTL_2P2Z_CoefStruct CNTL_2P2Z_CoefStruct1;

long Pgain, Igain, Dgain, Dmax, Dmin;

// Declare the net variables for ADC
volatile long RltA,RltB,RltC,RltD,RltE,RltF,RltG,RltH;

#pragma DATA_SECTION(Duty1A, 		"CpuToCla1MsgRAM");
volatile float Duty1A;

// DPLIB 2p2z_CLA Veriables
#pragma DATA_SECTION(Out_CLA, "Cla1ToCpuMsgRAM");
#pragma DATA_SECTION(Fdbk_CLA,"Cla1ToCpuMsgRAM");
#pragma DATA_SECTION(Ref_CLA, "CpuToCla1MsgRAM");
volatile float Out_CLA, Fdbk_CLA, Ref_CLA;

#pragma DATA_SECTION(CNTL_2P2Z_CLACoefStruct1, "CNTL_2P2Z_CLA_Coef");
struct CNTL_2P2Z_CLACoefStruct CNTL_2P2Z_CLACoefStruct1;

volatile float Pgain_CLA, Igain_CLA, Dgain_CLA, Dmax_CLA, Dmin_CLA;

// Declare the net variables for ADC CLA
#pragma DATA_SECTION(RltA_CLA,"Cla1ToCpuMsgRAM");
#pragma DATA_SECTION(RltB_CLA,"Cla1ToCpuMsgRAM");
#pragma DATA_SECTION(RltC_CLA,"Cla1ToCpuMsgRAM");
#pragma DATA_SECTION(RltD_CLA,"Cla1ToCpuMsgRAM");
#pragma DATA_SECTION(RltE_CLA,"Cla1ToCpuMsgRAM");
#pragma DATA_SECTION(RltF_CLA,"Cla1ToCpuMsgRAM");
#pragma DATA_SECTION(RltG_CLA,"Cla1ToCpuMsgRAM");
#pragma DATA_SECTION(RltH_CLA,"Cla1ToCpuMsgRAM");
volatile float RltA_CLA, RltB_CLA, RltC_CLA, RltD_CLA, RltE_CLA, RltF_CLA,
		RltG_CLA, RltH_CLA;

#endif

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// VARIABLE DECLARATIONS - CCS WatchWindow / GUI support
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

// -------------------------------- FRAMEWORK --------------------------------------

//GUI support variables
// sets a limit on the amount of external GUI controls - increase as necessary
int16 *varSetTxtList[16];					//16 textbox controlled variables
int16 *varSetBtnList[16];					//16 button controlled variables
int16 *varSetSldrList[16];				//16 slider controlled variables
int16 *varGetList[16];					//16 variables sendable to GUI
int16 *arrayGetList[16];					//16 arrays sendable to GUI
int16 LedBlinkCnt;

// ---------------------------------- USER -----------------------------------------

// Monitor ("Get")						// Display as:

// Configure ("Set")

// History arrays are used for Running Average calculation (boxcar filter)
// Used for CCS display and GUI only, not part of control loop processing

//Scaling Constants (values found via spreadsheet; exact value calibrated per board)

// Variables for background support only (no need to access)
int16 i;								// common use incrementer
Uint32 HistPtr, temp_Scratch; 			// Temp here means Temporary
int16 SV1, SV2, SV3; 			// GPIO1, GPIO19, GPI18


// Global variables
uint16_t sdataA[2];    // Send data for SCI-A
uint16_t rdataA[2];    // Received data for SCI-A



//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// MAIN CODE - starts here
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void main(void) {
//=================================================================================
//	INITIALISATION - General
//=================================================================================

	// The DeviceInit() configures the clocks and pin mux registers 
	// The function is declared in {ProjectName}-DevInit_F2803/2x.c,
	// Please ensure/edit that all the desired components pin muxes 
	// are configured properly that clocks for the peripherals used
	// are enabled, for example the individual PWM clock must be enabled 
	// along with the Time Base Clock 

	DeviceInit();	// Device Life support & GPIO

	// Initialize all the Device Peripherals:
		scia_fifo_init();  // Init SCI-A FIFO


//-------------------------------- FRAMEWORK --------------------------------------

// Only used if running from FLASH
// Note that the variable FLASH is defined by the compiler with -d FLASH

#ifdef FLASH		
// Copy time critical code and Flash setup code to RAM
// The  RamfuncsLoadStart, RamfuncsLoadEnd, and RamfuncsRunStart
// symbols are created by the linker. Refer to the linker files. 
	MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);

// Call Flash Initialization to setup flash waitstates
// This function must reside in RAM
	InitFlash();// Call the flash wrapper init function
#endif //(FLASH)

// Timing sync for background loops
// Timer period definitions found in PeripheralHeaderIncludes.h
	CpuTimer0Regs.PRD.all = mSec1;		// A tasks
	CpuTimer1Regs.PRD.all = mSec10;	// B tasks
	CpuTimer2Regs.PRD.all = mSec100;	// C tasks

// Tasks State-machine init
	Alpha_State_Ptr = &A0;
	A_Task_Ptr = &A1;
	B_Task_Ptr = &B1;
	C_Task_Ptr = &C1;

	VTimer0[0] = 0;
	VTimer1[0] = 0;
	VTimer2[0] = 0;
	LedBlinkCnt = 5;

// ---------------------------------- USER -----------------------------------------
//  put common initialization/variable definitions here

//===============================================================================
//	INITIALISATION - GUI connections
//=================================================================================
// Use this section only if you plan to "Instrument" your application using the 
// Microsoft C# freeware GUI Template provided by TI
	/*
	 //"Set" variables
	 //---------------------------------------
	 // assign GUI variable Textboxes to desired "setable" parameter addresses
	 varSetTxtList[0] = &Gui_TxtListVar;
	 varSetTxtList[1] = &Gui_TxtListVar;
	 varSetTxtList[2] = &Gui_TxtListVar;
	 varSetTxtList[3] = &Gui_TxtListVar;
	 varSetTxtList[4] = &Gui_TxtListVar;
	 varSetTxtList[5] = &Gui_TxtListVar;
	 varSetTxtList[6] = &Gui_TxtListVar;
	 varSetTxtList[7] = &Gui_TxtListVar;
	 varSetTxtList[8] = &Gui_TxtListVar;
	 varSetTxtList[9] = &Gui_TxtListVar;
	 varSetTxtList[10] = &Gui_TxtListVar;
	 varSetTxtList[11] = &Gui_TxtListVar;

	 // assign GUI Buttons to desired flag addresses
	 varSetBtnList[0] = &Gui_BtnListVar;
	 varSetBtnList[1] = &Gui_BtnListVar;
	 varSetBtnList[2] = &Gui_BtnListVar;
	 varSetBtnList[3] = &Gui_BtnListVar;
	 varSetBtnList[4] = &Gui_BtnListVar;

	 // assign GUI Sliders to desired "setable" parameter addresses
	 varSetSldrList[0] = &Gui_SldrListVar;
	 varSetSldrList[1] = &Gui_SldrListVar;
	 varSetSldrList[2] = &Gui_SldrListVar;
	 varSetSldrList[3] = &Gui_SldrListVar;
	 varSetSldrList[4] = &Gui_SldrListVar;

	 //"Get" variables
	 //---------------------------------------
	 // assign a GUI "getable" parameter address
	 varGetList[0] = &Gui_GetListVar;
	 varGetList[1] = &Gui_GetListVar;
	 varGetList[2] = &Gui_GetListVar;
	 varGetList[3] = &Gui_GetListVar;
	 varGetList[4] = &Gui_GetListVar;
	 varGetList[5] = &Gui_GetListVar;
	 varGetList[6] = &Gui_GetListVar;
	 varGetList[7] = &Gui_GetListVar;
	 varGetList[8] = &Gui_GetListVar;
	 varGetList[9] = &Gui_GetListVar;
	 varGetList[10] = &Gui_GetListVar;
	 varGetList[11] = &Gui_GetListVar;
	 varGetList[12] = &Gui_GetListVar;
	 varGetList[13] = &Gui_GetListVar;
	 varGetList[14] = &Gui_GetListVar;
	 varGetList[15] = &Gui_GetListVar;

	 // assign a GUI "getable" parameter array address
	 arrayGetList[0] = &DBUFF1;  	//only need to set initial position of array,
	 arrayGetList[1] = &DBUFF2;		//  program will run through it accordingly
	 arrayGetList[2] = &DBUFF3;
	 arrayGetList[3] = &DBUFF4;
	 */

//==================================================================================
//	INCREMENTAL BUILD OPTIONS - NOTE: selected via {ProjectName-Settings.h
//==================================================================================
// ---------------------------------- USER -----------------------------------------
//----------------------------------------------------------------------
#if (INCR_BUILD == 1) 	// Open Loop Two Phase Interleaved PFC PWM Driver
//----------------------------------------------------------------------
	// Configure PWM2 for 200Khz switching Frequency
	//Period Count= 80Mhz/10Khz = 8000
	PWM_1ch_CNF(2, 8000,1,0);

	// Configure ADC to be triggered from EPWM1 Period event
	//Map channel to ADC Pin
	ChSel[0] = 6;// A7 -A  ADC A0 ->0 B0 -> 8
	ChSel[1] = 2;// A3 -B
	ChSel[2] = 0;// A1 -C
	ChSel[3] = 4;// A0 -D
	ChSel[4] = 6;// B1 -E
	ChSel[5] = 2;// B3 -F
	ChSel[6] = 0;// B7 -G
	ChSel[7] = 4;// B7 -H
	/*	ChSel[8]=n;			//An
	 ChSel[9]=n;			//An
	 ChSel[10]=n;		//An
	 ChSel[11]=n;		//An
	 ChSel[12]=n;		//An
	 ChSel[13]=n;		//An
	 ChSel[14]=n;		//An
	 ChSel[15]=n;		//An
	 */

	// Select Trigger Event for ADC conversion
	TrigSel[0]= ADCTRIG_EPWM2_SOCA;
	TrigSel[1]= ADCTRIG_EPWM2_SOCA;
	TrigSel[2]= ADCTRIG_EPWM2_SOCA;
	TrigSel[3]= ADCTRIG_EPWM2_SOCA;
	TrigSel[4]= ADCTRIG_EPWM2_SOCA;
	TrigSel[5]= ADCTRIG_EPWM2_SOCA;
	TrigSel[6]= ADCTRIG_EPWM2_SOCA;
	TrigSel[7]= ADCTRIG_EPWM2_SOCA;
	/*	TrigSel[8]= ADCTRIG_EPWMn_SOCA;
	 TrigSel[9]= ADCTRIG_EPWMn_SOCA;
	 TrigSel[10]= ADCTRIG_EPWMn_SOCA;
	 TrigSel[11]= ADCTRIG_EPWMn_SOCA;
	 TrigSel[12]= ADCTRIG_EPWMn_SOCA;
	 TrigSel[13]= ADCTRIG_EPWMn_SOCA;
	 TrigSel[14]= ADCTRIG_EPWMn_SOCA;
	 TrigSel[15]= ADCTRIG_EPWMn_SOCA;*/

	// Configure the ADC with auto interrupt clear mode
	// ADC interrupt after EOC of channel 0
	ADC_SOC_CNF(ChSel,TrigSel,ACQPS,16,0);

	// Configure the EPWM2 to issue the SOC
	EPwm2Regs.ETSEL.bit.SOCAEN = 1;
	EPwm2Regs.ETSEL.bit.SOCASEL = ET_CTR_PRD;// Use PRD event as trigger for ADC SOC
	EPwm2Regs.ETPS.bit.SOCAPRD = ET_1ST;// Generate pulse on every event

	// Digital Power CLA(DP) library initialisation

	DPL_Init();

	// Lib Module connection to "nets" 
	//----------------------------------------
	// Connect the PWM Driver input to an input variable, Open Loop System
	PWMDRV_1ch_Duty2 = &Out;

	// Initialize the net variables
	//Duty1A =(float)(0.7);
	Duty2A =_IQ24(0.7);

	//=========================================================================================================
	// Connect the CNTL_2P2Z block to the variables
	CNTL_2P2Z_Fdbk1 = &RltA;
	CNTL_2P2Z_Out1 = &Out;
	CNTL_2P2Z_Ref1 = &Ref;
	CNTL_2P2Z_Coef1 = &CNTL_2P2Z_CoefStruct1.b2;

	// Coefficients for the Voltage Loop
	// PID coefficients & Clamping (Q26)
	Dmax = _IQ24(0.984375);//Q24
	Dmin = _IQ24(-0.9);//Q24
	Pgain = _IQ26(0.001953);//Q26
	Igain = _IQ26(0.001250);//Q26
	Dgain = _IQ26(0.0);//Q26

	// Initialize the Controller Coefficients
	CNTL_2P2Z_CoefStruct1.b2 = Dgain;// B2
	CNTL_2P2Z_CoefStruct1.b1 = (Igain-Pgain-Dgain-Dgain);// B1
	CNTL_2P2Z_CoefStruct1.b0 = (Pgain + Igain + Dgain);// B0
	CNTL_2P2Z_CoefStruct1.a2 = 0.0;// A2 = 0
	CNTL_2P2Z_CoefStruct1.a1 = _IQ26(1.0);// A1 = 1
	CNTL_2P2Z_CoefStruct1.max = Dmax;//Clamp Hi
	CNTL_2P2Z_CoefStruct1.i_min =_IQ24(-0.9);
	CNTL_2P2Z_CoefStruct1.min = Dmin;//Clamp Min

	//Initialize the net Variables/nodes
	Ref=_IQ24(0.5);
	Fdbk=_IQ24(0.0);
	Out=_IQ24(0.0);

	// ADCDRV_8ch block connections
	ADCDRV_8ch_RltPtrA=&RltA;
	ADCDRV_8ch_RltPtrB=&RltB;
	ADCDRV_8ch_RltPtrC=&RltC;
	ADCDRV_8ch_RltPtrD=&RltD;
	ADCDRV_8ch_RltPtrE=&RltE;
	ADCDRV_8ch_RltPtrF=&RltF;
	ADCDRV_8ch_RltPtrG=&RltG;
	ADCDRV_8ch_RltPtrH=&RltH;

	// Initialize the net variables
	RltA=_IQ24(0.0);
	RltB=_IQ24(0.0);
	RltC=_IQ24(0.0);
	RltD=_IQ24(0.0);
	RltE=_IQ24(0.0);
	RltF=_IQ24(0.0);
	RltG=_IQ24(0.0);
	RltH=_IQ24(0.0);

	SV1 = 0;
	SV2 = 0;
	SV3 = 0;
	//====================================================================================
	// INTERRUPTS & ISR INITIALIZATION (best to run this section after other initialization)
	//====================================================================================

	// Set up C28x Interrupt

	//Also Set the appropriate # define's in the {ProjectName}-Settings.h
	//to enable interrupt management in the ISR

	EALLOW;
	PieVectTable.EPWM2_INT = &DPL_ISR;// Map Interrupt
	PieCtrlRegs.PIEIER3.bit.INTx2 = 1;// PIE level enable, Grp3 / Int2
	EPwm2Regs.ETSEL.bit.INTSEL = ET_CTR_PRD;// INT on PRD event
	EPwm2Regs.ETSEL.bit.INTEN = 1;// Enable INT
	EPwm2Regs.ETPS.bit.INTPRD = ET_1ST;// Generate INT on every event

	IER |= M_INT3;// Enable CPU INT3 connected to EPWM1-6 INTs:
	EINT;// Enable Global interrupt INTM
	ERTM;// Enable Global realtime interrupt DBGM
	EDIS;

#endif // (INCR_BUILD == 1)
#if (INCR_BUILD == 2) 	// Open Loop Two Phase Interleaved PFC PWM Driver
//----------------------------------------------------------------------
	// Configure PWM1 for 10Khz switching Frequency
	//Period Count= 80M/10Khz = 8000
	PWM_1ch_CNF(1, 8000, 1, 0);

	// Configure ADC to be triggered from EPWM1 Period event
	//Map channel to ADC Pin
	ChSel_CLA[0] = 6;		//ADC-A6
	// for additional ADC conversions modify below
	ChSel_CLA[1] = 2;		//ADC-A2
	ChSel_CLA[2] = 0;		//ADC-A0
	ChSel_CLA[3] = 4;		//ADC-A4
	ChSel_CLA[4] = 6;			//An
	ChSel_CLA[5] = 2;			//An
	ChSel_CLA[6] = 0;			//An
	ChSel_CLA[7] = 4;			//An
	/*	ChSel_CLA[8]=n;			//An
	 ChSel_CLA[9]=n;			//An
	 ChSel_CLA[10]=n;		//An
	 ChSel_CLA[11]=n;		//An
	 ChSel_CLA[12]=n;		//An
	 ChSel_CLA[13]=n;		//An
	 ChSel_CLA[14]=n;		//An
	 ChSel_CLA[15]=n;		//An
	 */

	// Select Trigger Event for ADC conversion
	TrigSel_CLA[0] = ADCTRIG_EPWM1_SOCA;
	// associate the appropriate peripheral trigger to the ADC channel
	TrigSel_CLA[1] = ADCTRIG_EPWM1_SOCA;
	TrigSel_CLA[2] = ADCTRIG_EPWM1_SOCA;
	TrigSel_CLA[3] = ADCTRIG_EPWM1_SOCA;
	TrigSel_CLA[4] = ADCTRIG_EPWM1_SOCA;
	TrigSel_CLA[5] = ADCTRIG_EPWM1_SOCA;
	TrigSel_CLA[6] = ADCTRIG_EPWM1_SOCA;
	TrigSel_CLA[7] = ADCTRIG_EPWM1_SOCA;
	/*	TrigSel_CLA[8]= ADCTRIG_EPWMn_SOCA;
	 TrigSel_CLA[9]= ADCTRIG_EPWMn_SOCA;
	 TrigSel_CLA[10]= ADCTRIG_EPWMn_SOCA;
	 TrigSel_CLA[11]= ADCTRIG_EPWMn_SOCA;
	 TrigSel_CLA[12]= ADCTRIG_EPWMn_SOCA;
	 TrigSel_CLA[13]= ADCTRIG_EPWMn_SOCA;
	 TrigSel_CLA[14]= ADCTRIG_EPWMn_SOCA;
	 TrigSel_CLA[15]= ADCTRIG_EPWMn_SOCA;*/

	// Configure the ADC with auto interrupt clear mode
	// ADC interrupt after EOC of channel 0
	ADC_SOC_CNF(ChSel_CLA, TrigSel_CLA, ACQPS_CLA, 0, 2);

	// Configure the EPWM1 to issue the SOC
	EPwm1Regs.ETSEL.bit.SOCAEN = 1;
	EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_PRD;// Use PRD event as trigger for ADC SOC
	EPwm1Regs.ETPS.bit.SOCAPRD = ET_1ST;        // Generate pulse on every event

	// Digital Power CLA(DP) library initialisation
	DPL_CLAInit();

	// Lib Module connection to "nets"
	//----------------------------------------
	// Connect the PWM Driver input to an input variable, Open Loop System
	PWMDRV_1ch_CLA_Duty1 = &Out_CLA;

	// Initialize the net variables
	Duty1A = (float) (0.7);

	// Connect the CNTL_2P2Z_CLA block to the variables
	CNTL_2P2Z_CLA_Fdbk1 = &RltA_CLA;
	CNTL_2P2Z_CLA_Out1 = &Out_CLA;
	CNTL_2P2Z_CLA_Ref1 = &Ref_CLA;
	CNTL_2P2Z_CLA_Coef1 = &CNTL_2P2Z_CLACoefStruct1.b2;

	// Coefficients for the Voltage Loop
	// PID coefficients & Clamping (Q26)
	Dmax_CLA = (float) (0.984375);	//Q24
	Dmin_CLA = (float) (0.0);			//Q24
	Pgain_CLA = (float) (0.001953);	//Q26
	Igain_CLA = (float) (0.001250);	//Q26
	Dgain_CLA = (float) (0.0);			//Q26

	// Initialize the Controller Coefficients
	// Note the CLA Coefficients would reside in the CPUToCLAMsg RAM

	CNTL_2P2Z_CLACoefStruct1.b2 = Dgain_CLA;							// B2
	CNTL_2P2Z_CLACoefStruct1.b1 =
			(Igain_CLA - Pgain_CLA - Dgain_CLA - Dgain_CLA);  		// B1
	CNTL_2P2Z_CLACoefStruct1.b0 = (Pgain_CLA + Igain_CLA + Dgain_CLA);     // B0
	CNTL_2P2Z_CLACoefStruct1.a2 = 0.0;                              // A2 = 0
	CNTL_2P2Z_CLACoefStruct1.a1 = (float) (1.0);                       // A1 = 1
	CNTL_2P2Z_CLACoefStruct1.max = Dmax_CLA;					  	//Clamp Hi
	CNTL_2P2Z_CLACoefStruct1.i_min = (float) (-0.9);
	CNTL_2P2Z_CLACoefStruct1.min = Dmin_CLA; 					     //Clamp Min
	//Initialize the net Variables residing in the CPU to CLA
	Ref_CLA = (float) (0.4);

//	 ADCDRV_8ch block connections
	ADCDRV_8ch_CLA_RltPtrA = &RltA_CLA;
	ADCDRV_8ch_CLA_RltPtrB = &RltB_CLA;
	ADCDRV_8ch_CLA_RltPtrC = &RltC_CLA;
	ADCDRV_8ch_CLA_RltPtrD = &RltD_CLA;
	ADCDRV_8ch_CLA_RltPtrE = &RltE_CLA;
	ADCDRV_8ch_CLA_RltPtrF = &RltF_CLA;
	ADCDRV_8ch_CLA_RltPtrG = &RltG_CLA;
	ADCDRV_8ch_CLA_RltPtrH = &RltH_CLA;

	SV1 = 0;
	SV2 = 0;
	SV3 = 0;
	//----------------------Initialize the CLA-------------------------

	CLA_Init();

	//====================================================================================
	// INTERRUPTS & ISR INITIALIZATION (best to run this section after other initialization)
	//====================================================================================

	// Set Up CLA Task

	// Task 1 has the option to be started by either EPWM1_INT or ADCINT1
	// In this case we will allow ADCINT1 to start CLA Task 1
	EALLOW;
	// Configure PWM1 to issue interrupts
	EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_PRD;  	// INT on PRD event
	EPwm1Regs.ETSEL.bit.INTEN = 1;              // Enable INT
	EPwm1Regs.ETPS.bit.INTPRD = ET_1ST;         // Generate INT on every event
	Cla1Regs.MPISRCSEL1.bit.PERINT1SEL = CLA_INT1_EPWM1INT;	// 0=ADCINT1    1=none    2=EPWM1INT
	// Configure the interrupt that woud occur each control cycles
	Cla1Regs.MIER.all = M_INT1;
	asm("   RPT #3 || NOP");
	EDIS;
#endif // (INCR_BUILD == 2)

#if (INCR_BUILD == 3) 	// Open Loop Two Phase Interleaved PFC PWM Driver
//----------------------------------------------------------------------
//  ISR Code
//----------------------------------------------------------------------
	// Configure PWM2 for 200Khz switching Frequency
	//Period Count= 80Mhz/10Khz = 8000
	PWM_1ch_CNF(2, 8000,1,0);

	// Configure ADC to be triggered from EPWM1 Period event
	//Map channel to ADC Pin
	ChSel[0] = 6;// A7 -A  ADC A0 ->0 B0 -> 8
	ChSel[1] = 2;// A3 -B
	ChSel[2] = 0;// A1 -C
	ChSel[3] = 4;// A0 -D
	ChSel[4] = 6;// B1 -E
	ChSel[5] = 2;// B3 -F
	ChSel[6] = 0;// B7 -G
	ChSel[7] = 4;// B7 -H
	/*	ChSel[8]=n;			//An
	 ChSel[9]=n;			//An
	 ChSel[10]=n;		//An
	 ChSel[11]=n;		//An
	 ChSel[12]=n;		//An
	 ChSel[13]=n;		//An
	 ChSel[14]=n;		//An
	 ChSel[15]=n;		//An
	 */

	// Select Trigger Event for ADC conversion
	TrigSel[0]= ADCTRIG_EPWM2_SOCA;
	TrigSel[1]= ADCTRIG_EPWM2_SOCA;
	TrigSel[2]= ADCTRIG_EPWM2_SOCA;
	TrigSel[3]= ADCTRIG_EPWM2_SOCA;
	TrigSel[4]= ADCTRIG_EPWM2_SOCA;
	TrigSel[5]= ADCTRIG_EPWM2_SOCA;
	TrigSel[6]= ADCTRIG_EPWM2_SOCA;
	TrigSel[7]= ADCTRIG_EPWM2_SOCA;
	/*	TrigSel[8]= ADCTRIG_EPWMn_SOCA;
	 TrigSel[9]= ADCTRIG_EPWMn_SOCA;
	 TrigSel[10]= ADCTRIG_EPWMn_SOCA;
	 TrigSel[11]= ADCTRIG_EPWMn_SOCA;
	 TrigSel[12]= ADCTRIG_EPWMn_SOCA;
	 TrigSel[13]= ADCTRIG_EPWMn_SOCA;
	 TrigSel[14]= ADCTRIG_EPWMn_SOCA;
	 TrigSel[15]= ADCTRIG_EPWMn_SOCA;*/

	// Configure the ADC with auto interrupt clear mode
	// ADC interrupt after EOC of channel 0
	ADC_SOC_CNF(ChSel,TrigSel,ACQPS,16,0);

	// Configure the EPWM2 to issue the SOC
	EPwm2Regs.ETSEL.bit.SOCAEN = 1;
	EPwm2Regs.ETSEL.bit.SOCASEL = ET_CTR_PRD;// Use PRD event as trigger for ADC SOC
	EPwm2Regs.ETPS.bit.SOCAPRD = ET_1ST;// Generate pulse on every event

	// Digital Power CLA(DP) library initialisation

	DPL_Init();

	// Lib Module connection to "nets"
	//----------------------------------------
	// Connect the PWM Driver input to an input variable, Open Loop System
	PWMDRV_1ch_Duty2 = &Out;

	// Initialize the net variables
	//Duty1A =(float)(0.7);
	Duty2A =_IQ24(0.7);

	// Connect the CNTL_2P2Z block to the variables
	CNTL_2P2Z_Fdbk1 = &RltA;
	CNTL_2P2Z_Out1 = &Out;
	CNTL_2P2Z_Ref1 = &Ref;
	CNTL_2P2Z_Coef1 = &CNTL_2P2Z_CoefStruct1.b2;

	// Coefficients for the Voltage Loop
	// PID coefficients & Clamping (Q26)
	Dmax = _IQ24(0.984375);//Q24
	Dmin = _IQ24(-0.9);//Q24
	Pgain = _IQ26(0.001953);//Q26
	Igain = _IQ26(0.001250);//Q26
	Dgain = _IQ26(0.0);//Q26

	// Initialize the Controller Coefficients
	CNTL_2P2Z_CoefStruct1.b2 = Dgain;// B2
	CNTL_2P2Z_CoefStruct1.b1 = (Igain-Pgain-Dgain-Dgain);// B1
	CNTL_2P2Z_CoefStruct1.b0 = (Pgain + Igain + Dgain);// B0
	CNTL_2P2Z_CoefStruct1.a2 = 0.0;// A2 = 0
	CNTL_2P2Z_CoefStruct1.a1 = _IQ26(1.0);// A1 = 1
	CNTL_2P2Z_CoefStruct1.max = Dmax;//Clamp Hi
	CNTL_2P2Z_CoefStruct1.i_min =_IQ24(-0.9);
	CNTL_2P2Z_CoefStruct1.min = Dmin;//Clamp Min

	//Initialize the net Variables/nodes
	Ref=_IQ24(0.5);
	Fdbk=_IQ24(0.0);
	Out=_IQ24(0.0);

	// ADCDRV_8ch block connections
	ADCDRV_8ch_RltPtrA=&RltA;
	ADCDRV_8ch_RltPtrB=&RltB;
	ADCDRV_8ch_RltPtrC=&RltC;
	ADCDRV_8ch_RltPtrD=&RltD;
	ADCDRV_8ch_RltPtrE=&RltE;
	ADCDRV_8ch_RltPtrF=&RltF;
	ADCDRV_8ch_RltPtrG=&RltG;
	ADCDRV_8ch_RltPtrH=&RltH;

	// Initialize the net variables
	RltA=_IQ24(0.0);
	RltB=_IQ24(0.0);
	RltC=_IQ24(0.0);
	RltD=_IQ24(0.0);
	RltE=_IQ24(0.0);
	RltF=_IQ24(0.0);
	RltG=_IQ24(0.0);
	RltH=_IQ24(0.0);

	//====================================================================================
	// INTERRUPTS & ISR INITIALIZATION (best to run this section after other initialization)
	//====================================================================================

	// Set up C28x Interrupt

	//Also Set the appropriate # define's in the {ProjectName}-Settings.h
	//to enable interrupt management in the ISR

	EALLOW;
	PieVectTable.EPWM2_INT = &DPL_ISR;// Map Interrupt
	PieCtrlRegs.PIEIER3.bit.INTx2 = 1;// PIE level enable, Grp3 / Int2
	EPwm2Regs.ETSEL.bit.INTSEL = ET_CTR_PRD;// INT on PRD event
	EPwm2Regs.ETSEL.bit.INTEN = 1;// Enable INT
	EPwm2Regs.ETPS.bit.INTPRD = ET_1ST;// Generate INT on every event

	IER |= M_INT3;// Enable CPU INT3 connected to EPWM1-6 INTs:
	EINT;// Enable Global interrupt INTM
	ERTM;// Enable Global realtime interrupt DBGM
	EDIS;
//----------------------------------------------------------------------
//  CLA Code
//----------------------------------------------------------------------
	// Configure PWM1 for 10Khz switching Frequency
	//Period Count= 80M/10Khz = 8000
	PWM_1ch_CNF(1, 8000, 1, 0);

	// Configure ADC to be triggered from EPWM1 Period event
	//Map channel to ADC Pin
	ChSel_CLA[0] = 6;		//ADC-A6
	// for additional ADC conversions modify below
	ChSel_CLA[1] = 2;		//ADC-A2
	ChSel_CLA[2] = 0;		//ADC-A0
	ChSel_CLA[3] = 4;		//ADC-A4
	ChSel_CLA[4] = 6;			//An
	ChSel_CLA[5] = 2;			//An
	ChSel_CLA[6] = 0;			//An
	ChSel_CLA[7] = 4;			//An
	/*	ChSel_CLA[8]=n;			//An
	 ChSel_CLA[9]=n;			//An
	 ChSel_CLA[10]=n;		//An
	 ChSel_CLA[11]=n;		//An
	 ChSel_CLA[12]=n;		//An
	 ChSel_CLA[13]=n;		//An
	 ChSel_CLA[14]=n;		//An
	 ChSel_CLA[15]=n;		//An
	 */

	// Select Trigger Event for ADC conversion
	TrigSel_CLA[0] = ADCTRIG_EPWM1_SOCA;
	// associate the appropriate peripheral trigger to the ADC channel
	TrigSel_CLA[1] = ADCTRIG_EPWM1_SOCA;
	TrigSel_CLA[2] = ADCTRIG_EPWM1_SOCA;
	TrigSel_CLA[3] = ADCTRIG_EPWM1_SOCA;
	TrigSel_CLA[4] = ADCTRIG_EPWM1_SOCA;
	TrigSel_CLA[5] = ADCTRIG_EPWM1_SOCA;
	TrigSel_CLA[6] = ADCTRIG_EPWM1_SOCA;
	TrigSel_CLA[7] = ADCTRIG_EPWM1_SOCA;
	/*	TrigSel_CLA[8]= ADCTRIG_EPWMn_SOCA;
	 TrigSel_CLA[9]= ADCTRIG_EPWMn_SOCA;
	 TrigSel_CLA[10]= ADCTRIG_EPWMn_SOCA;
	 TrigSel_CLA[11]= ADCTRIG_EPWMn_SOCA;
	 TrigSel_CLA[12]= ADCTRIG_EPWMn_SOCA;
	 TrigSel_CLA[13]= ADCTRIG_EPWMn_SOCA;
	 TrigSel_CLA[14]= ADCTRIG_EPWMn_SOCA;
	 TrigSel_CLA[15]= ADCTRIG_EPWMn_SOCA;*/

	// Configure the ADC with auto interrupt clear mode
	// ADC interrupt after EOC of channel 0
	ADC_SOC_CNF(ChSel_CLA, TrigSel_CLA, ACQPS_CLA, 0, 2);

	// Configure the EPWM1 to issue the SOC
	EPwm1Regs.ETSEL.bit.SOCAEN = 1;
	EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_PRD;// Use PRD event as trigger for ADC SOC
	EPwm1Regs.ETPS.bit.SOCAPRD = ET_1ST;        // Generate pulse on every event

	// Digital Power CLA(DP) library initialisation
	DPL_CLAInit();

	// Lib Module connection to "nets"
	//----------------------------------------
	// Connect the PWM Driver input to an input variable, Open Loop System
	PWMDRV_1ch_CLA_Duty1 = &Out_CLA;

	// Initialize the net variables
	Duty1A = (float) (0.7);

	// Connect the CNTL_2P2Z_CLA block to the variables
	CNTL_2P2Z_CLA_Fdbk1 = &RltA_CLA;
	CNTL_2P2Z_CLA_Out1 = &Out_CLA;
	CNTL_2P2Z_CLA_Ref1 = &Ref_CLA;
	CNTL_2P2Z_CLA_Coef1 = &CNTL_2P2Z_CLACoefStruct1.b2;

	// Coefficients for the Voltage Loop
	// PID coefficients & Clamping (Q26)
	Dmax_CLA = (float) (0.984375);	//Q24
	Dmin_CLA = (float) (0.0);			//Q24
	Pgain_CLA = (float) (0.001953);	//Q26
	Igain_CLA = (float) (0.001250);	//Q26
	Dgain_CLA = (float) (0.0);			//Q26

	// Initialize the Controller Coefficients
	// Note the CLA Coefficients would reside in the CPUToCLAMsg RAM

	CNTL_2P2Z_CLACoefStruct1.b2 = Dgain_CLA;							// B2
	CNTL_2P2Z_CLACoefStruct1.b1 = (Igain_CLA - Pgain_CLA - Dgain_CLA - Dgain_CLA);  		// B1
	CNTL_2P2Z_CLACoefStruct1.b0 = (Pgain_CLA + Igain_CLA + Dgain_CLA);     // B0
	CNTL_2P2Z_CLACoefStruct1.a2 = 0.0;                              // A2 = 0
	CNTL_2P2Z_CLACoefStruct1.a1 = (float) (1.0);                       // A1 = 1
	CNTL_2P2Z_CLACoefStruct1.max = Dmax_CLA;					  	//Clamp Hi
	CNTL_2P2Z_CLACoefStruct1.i_min = (float) (-0.9);
	CNTL_2P2Z_CLACoefStruct1.min = Dmin_CLA; 					     //Clamp Min
	//Initialize the net Variables residing in the CPU to CLA
	Ref_CLA = (float) (0.4);

//	 ADCDRV_8ch block connections
	ADCDRV_8ch_CLA_RltPtrA = &RltA_CLA;
	ADCDRV_8ch_CLA_RltPtrB = &RltB_CLA;
	ADCDRV_8ch_CLA_RltPtrC = &RltC_CLA;
	ADCDRV_8ch_CLA_RltPtrD = &RltD_CLA;
	ADCDRV_8ch_CLA_RltPtrE = &RltE_CLA;
	ADCDRV_8ch_CLA_RltPtrF = &RltF_CLA;
	ADCDRV_8ch_CLA_RltPtrG = &RltG_CLA;
	ADCDRV_8ch_CLA_RltPtrH = &RltH_CLA;

	SV1 = 0;
	SV2 = 0;
	SV3 = 0;

	//----------------------Initialize the CLA-------------------------

	CLA_Init();

//====================================================================================
// INTERRUPTS & ISR INITIALIZATION (best to run this section after other initialization)
//====================================================================================

	// Set Up CLA Task

	// Task 1 has the option to be started by either EPWM1_INT or ADCINT1
	// In this case we will allow ADCINT1 to start CLA Task 1
	EALLOW;
	// Configure PWM1 to issue interrupts
	EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_PRD;  	// INT on PRD event
	EPwm1Regs.ETSEL.bit.INTEN = 1;              // Enable INT
	EPwm1Regs.ETPS.bit.INTPRD = ET_1ST;         // Generate INT on every event
	Cla1Regs.MPISRCSEL1.bit.PERINT1SEL = CLA_INT1_EPWM1INT;	// 0=ADCINT1    1=none    2=EPWM1INT
	// Configure the interrupt that woud occur each control cycles
	Cla1Regs.MIER.all = M_INT1;
	asm("   RPT #3 || NOP");
	EDIS;
#endif // (INCR_BUILD == 3)



//=================================================================================
//	BACKGROUND (BG) LOOP
//=================================================================================

//--------------------------------- FRAMEWORK -------------------------------------
	for (;;)  //infinite loop
			{
		// State machine entry & exit point
		//===========================================================
		(*Alpha_State_Ptr)();	// jump to an Alpha state (A0,B0,...)
		//===========================================================

	}
} //END MAIN CODE

//=================================================================================
//	STATE-MACHINE SEQUENCING AND SYNCRONIZATION
//=================================================================================

//--------------------------------- FRAMEWORK -------------------------------------
void A0(void) {
	// loop rate synchronizer for A-tasks
	if (CpuTimer0Regs.TCR.bit.TIF == 1) {
		CpuTimer0Regs.TCR.bit.TIF = 1;	// clear flag

		//-----------------------------------------------------------
		(*A_Task_Ptr)();		// jump to an A Task (A1,A2,A3,...)
		//-----------------------------------------------------------

		VTimer0[0]++;			// virtual timer 0, instance 0 (spare)
	}

	Alpha_State_Ptr = &B0;		// Comment out to allow only A tasks
}

void B0(void) {
	// loop rate synchronizer for B-tasks
	if (CpuTimer1Regs.TCR.bit.TIF == 1) {
		CpuTimer1Regs.TCR.bit.TIF = 1;				// clear flag

		//-----------------------------------------------------------
		(*B_Task_Ptr)();		// jump to a B Task (B1,B2,B3,...)
		//-----------------------------------------------------------
		VTimer1[0]++;			// virtual timer 1, instance 0 (spare)
	}

	Alpha_State_Ptr = &C0;		// Allow C state tasks
}

void C0(void) {
	// loop rate synchronizer for C-tasks
	if (CpuTimer2Regs.TCR.bit.TIF == 1) {
		CpuTimer2Regs.TCR.bit.TIF = 1;				// clear flag

		//-----------------------------------------------------------
		(*C_Task_Ptr)();		// jump to a C Task (C1,C2,C3,...)
		//-----------------------------------------------------------
		VTimer2[0]++;			//virtual timer 2, instance 0 (spare)
	}

	Alpha_State_Ptr = &A0;	// Back to State A0
}

//=================================================================================
//	A - TASKS
//=================================================================================
//--------------------------------------------------------
void A1(void)
//--------------------------------------------------------
{

	GpioDataRegs.GPADAT.bit.GPIO1 = SV1;

	GpioDataRegs.GPADAT.bit.GPIO18 = SV3;
	//-------------------
	//the next time CpuTimer0 'counter' reaches Period value go to A2
	A_Task_Ptr = &A2;
	//-------------------
}

//-----------------------------------------------------------------
void A2(void)
//-----------------------------------------------------------------
{
	GpioDataRegs.GPADAT.bit.GPIO19 = SV2;
	//-------------------
	//the next time CpuTimer0 'counter' reaches Period value go to A1
	A_Task_Ptr = &A3;
	//-------------------
}

//-----------------------------------------
void A3(void)
//-----------------------------------------
{

	//-----------------
	//the next time CpuTimer0 'counter' reaches Period value go to A1
	A_Task_Ptr = &A4;
	//-----------------
}

//----------------------------------------------------------
void A4(void)
//---------------------------------------------------------
{
	//-----------------
	//the next time CpuTimer0 'counter' reaches Period value go to A1
	A_Task_Ptr = &A1;
	//-----------------
}

//=================================================================================
//	B - TASKS
//=================================================================================

//----------------------------------- USER ----------------------------------------

//----------------------------------------
void B1(void)
//----------------------------------------
{
	//-----------------
	//the next time CpuTimer1 'counter' reaches Period value go to B2
	B_Task_Ptr = &B2;
	//-----------------
}

//----------------------------------------
void B2(void) // Blink LED on the control CArd
//----------------------------------------
{
	if (LedBlinkCnt == 0) {
		GpioDataRegs.GPBTOGGLE.bit.GPIO34 = 1;//turn on/off LD3 on the controlCARD
		LedBlinkCnt = 5;
	} else
		LedBlinkCnt--;

	//-----------------
	//the next time CpuTimer1 'counter' reaches Period value go to B3
	B_Task_Ptr = &B3;
	//-----------------
}

//----------------------------------------
void B3(void)
//----------------------------------------
{
	//-----------------
	//the next time CpuTimer1 'counter' reaches Period value go to B4
	B_Task_Ptr = &B4;
	//-----------------
}

//----------------------------------------
void B4(void) //  SPARE
//----------------------------------------
{
	//-----------------
	//the next time CpuTimer1 'counter' reaches Period value go to B1
	B_Task_Ptr = &B1;
	//-----------------
}

//=================================================================================
//	C - TASKS
//=================================================================================

//--------------------------------- USER ------------------------------------------

//------------------------------------------------------
void C1(void)
//------------------------------------------------------
{

	//-----------------
	//the next time CpuTimer2 'counter' reaches Period value go to C2
	C_Task_Ptr = &C2;
	//-----------------

}

//----------------------------------------
void C2(void)
//----------------------------------------
{

	//-----------------
	//the next time CpuTimer2 'counter' reaches Period value go to C3
	C_Task_Ptr = &C3;
	//-----------------
}

//-----------------------------------------
void C3(void)
//-----------------------------------------
{

	//-----------------
	//the next time CpuTimer2 'counter' reaches Period value go to C4
	C_Task_Ptr = &C4;
	//-----------------
}

//-----------------------------------------
void C4(void) //  SPARE
//-----------------------------------------
{

	//-----------------
	//the next time CpuTimer2 'counter' reaches Period value go to C1
	C_Task_Ptr = &C1;
	//-----------------
}

