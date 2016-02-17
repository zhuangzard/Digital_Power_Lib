;----------------------------------------------------------------------------------
;	FILE:			{ProjectName}-DPL-CLA.asm
;
;	Description:	{ProjectName}-DPL-CLA.asm contains the code run by the CLA to service 
;					the tasks and also the initialization portion that is run by the C28x
;					for the DPlib CLA
;   
;   Revision/ Version: See {ProjectName}-Main.c
;----------------------------------------------------------------------------------

;// Include variables and constants that will be shared in the
;// C28x C-code and CLA assembly code.  This is accomplished by
;// using .cdecls to include a C-code header file that contains
;// these variables and constants

      .cdecls   C,LIST,"ProjectName-CLAShared.h"      
      
      
;// The following files have the assembly macros used in this file.
		; MACRO includes
	.include "PWMDRV_1ch_CLA.asm"
	.include "ADCDRV_8ch_CLA.asm"
	.include "CNTL_2P2Z_CLA.asm"
	 
;// To include an MDEBUGSTOP (CLA breakpoint) as the first instruction
;// of each task, set CLA_DEBUG to 1.  Use any other value to leave out
;// the MDEBUGSTOP instruction.

CLA_DEBUG .set  1

; label to DPCLA initialisation function
	.def _DPL_CLAInit
	
; dummy variable for pointer initialisation
ZeroNetCLA	.usect "ZeroNetCLA_Section",2,1,1	; output terminal 1

		.text
_DPL_CLAInit:
		ZAPA
		MOVL	XAR0, #ZeroNetCLA
		MOVL	*XAR0, ACC
		;---------------------------------------------------------
		.if(INCR_BUILD = 2)
			PWMDRV_1ch_CLA_INIT 1				; EPWM1A
			CNTL_2P2Z_CLA_INIT 1  ; CNTL_2P2Z Initialization
			ADCDRV_8ch_CLA_INIT 0,1,2,3,4,5,6,7  ; ADCDRV_4ch_CLA Initialization
		.endif
		;---------------------------------------------------------
		.if(INCR_BUILD = 3)
			PWMDRV_1ch_CLA_INIT 1				; EPWM1A
			CNTL_2P2Z_CLA_INIT 1  ; CNTL_2P2Z Initialization
			ADCDRV_8ch_CLA_INIT 0,1,2,3,4,5,6,7  ; ADCDRV_4ch_CLA Initialization
		.endif
		;---------------------------------------------------------
	LRETR
	
	
;// CLA code must be within its own assembly section and must be
;// even aligned.  
;// Note: since all CLA instructions are 32-bit
;// this alignment naturally occurs and the .align 2 is most likely
;// redundant

      .sect    "Cla1Prog"
      .align  2

_Cla1Prog_Start:

_Cla1Task1:
	MNOP
	MNOP
;instruction to halt the CLA when debugging
    .if(CLA_DEBUG  = 1)
        MDEBUGSTOP  
    .endif
;task specific code 
    .if(INCR_BUILD = 2)
		PWMDRV_1ch_CLA   1
		CNTL_2P2Z_CLA 1  ; Run CNTL_2P2Z_CLA
		ADCDRV_8ch_CLA 0,1,2,3,4,5,6,7    ; ADCDRV_4ch_CLA
	.endif
	;task specific code
    .if(INCR_BUILD = 3)
		PWMDRV_1ch_CLA   1
		CNTL_2P2Z_CLA 1  ; Run CNTL_2P2Z_CLA
		ADCDRV_8ch_CLA 0,1,2,3,4,5,6,7    ; ADCDRV_4ch_CLA
	.endif
;Clear EPWM Flag if PWM1 triggers the CLA task 
	.if(CLATASK1_EPWM1=1)
		MMOVXI		MR0,#0x01
		MMOV16      @_EPwm1Regs.ETCLR, MR0
	.endif
    MSTOP
    MNOP
    MNOP
    MNOP
_Cla1Task1End:


_Cla1Task2:
	MNOP
	MNOP
;instruction to halt the CLA when debugging
	.if(CLA_DEBUG  = 1)
        MDEBUGSTOP  
    .endif
;Clear EPWM Flag if PWM2 triggers the CLA task 
	.if(CLATASK2_EPWM2=1)
		MMOVXI		MR0,#0x01
		MMOV16      @_EPwm2Regs.ETCLR, MR0
	.endif
    MSTOP
    MNOP
    MNOP
    MNOP
_Cla1Task2End:


_Cla1Task3:
	MNOP
	MNOP
;instruction to halt the CLA when debugging
	.if(CLA_DEBUG  = 1)
        MDEBUGSTOP  
    .endif
;Clear EPWM Flag if PWM3 triggers the CLA task 
	.if(CLATASK3_EPWM3=1)
		MMOVXI		MR0,#0x01
		MMOV16      @_EPwm3Regs.ETCLR, MR0
	.endif
    MSTOP
    MNOP
    MNOP
    MNOP
_Cla1Task3End:


_Cla1Task4:
	MNOP
	MNOP
;instruction to halt the CLA when debugging
	.if(CLA_DEBUG  = 1)
        MDEBUGSTOP  
    .endif
;Clear EPWM Flag if PWM4 triggers the CLA task 
	.if(CLATASK4_EPWM4=1)
		MMOVXI		MR0,#0x01
		MMOV16      @_EPwm4Regs.ETCLR, MR0
	.endif
    MSTOP
    MNOP
    MNOP
    MNOP
_Cla1Task4End:


_Cla1Task5:
	MNOP
	MNOP
;instruction to halt the CLA when debugging
	.if(CLA_DEBUG  = 1)
        MDEBUGSTOP  
    .endif
;Clear EPWM Flag if PWM1 triggers the CLA task 
	.if(CLATASK5_EPWM5=1)
		MMOVXI		MR0,#0x01
		MMOV16      @_EPwm5Regs.ETCLR, MR0
	.endif
    MSTOP
    MNOP
    MNOP
    MNOP
_Cla1Task5End:

_Cla1Task6:
	MNOP
	MNOP
;instruction to halt the CLA when debugging
	.if(CLA_DEBUG  = 1)
        MDEBUGSTOP  
    .endif
;Clear EPWM Flag if PWM6 triggers the CLA task 
	.if(CLATASK6_EPWM6=1)
		MMOVXI		MR0,#0x01
		MMOV16      @_EPwm6Regs.ETCLR, MR0
	.endif
    MSTOP
    MNOP
    MNOP
    MNOP
_Cla1Task6End:

_Cla1Task7:
	MNOP
	MNOP
;instruction to halt the CLA when debugging
	.if(CLA_DEBUG  = 1)
        MDEBUGSTOP  
    .endif
;Clear EPWM Flag if PWM7 triggers the CLA task 
	.if(CLATASK7_EPWM7=1)
		MMOVXI		MR0,#0x01
		MMOV16      @_EPwm7Regs.ETCLR, MR0
	.endif
    MSTOP
    MNOP
    MNOP
    MNOP
_Cla1Task7End:

_Cla1Task8:
	MNOP
	MNOP
;instruction to halt the CLA when debugging
    .if(CLA_DEBUG  = 1)
        MDEBUGSTOP  
    .endif

;add any intialization code for memory in CLAtoCPUMsgRAM here
	.if(INCR_BUILD = 2)
	;.ref	_Var
	;MMOVF32	MR0,#0.0L
	;MF32TOUI32	MR0,MR0
	;MMOV32	@_Var,MR0
		; run the macro to initialize the internal data of the macro residing in ; the CLA write memory space
		CNTL_2P2Z_DBUFF_CLA_INIT 1 ;(Initialize variables in CLA writable memory space )
		.ref _Fdbk_CLA
		.ref _Out_CLA
		MMOVF32 MR0, #0.0L
		MF32TOUI32   MR0, MR0
		MMOV32 @_Fdbk_CLA,MR0
		MMOV32 @_Out_CLA,MR0
		;(Initialize variables in CLA writable memory space )
		.ref _RltA_CLA
		.ref _RltB_CLA
		.ref _RltC_CLA
		.ref _RltD_CLA
		.ref _RltE_CLA
		.ref _RltF_CLA
		.ref _RltG_CLA
		.ref _RltH_CLA
		 MMOVF32 MR0, #0.0L
		 MMOV32 @_RltA_CLA,MR0
		 MMOV32 @_RltB_CLA,MR0
		 MMOV32 @_RltC_CLA,MR0
		 MMOV32 @_RltD_CLA,MR0
		 MMOV32 @_RltE_CLA,MR0
		 MMOV32 @_RltF_CLA,MR0
		 MMOV32 @_RltG_CLA,MR0
		 MMOV32 @_RltH_CLA,MR0
	.endif
	.if(INCR_BUILD = 3)
	;.ref	_Var
	;MMOVF32	MR0,#0.0L
	;MF32TOUI32	MR0,MR0
	;MMOV32	@_Var,MR0
		; run the macro to initialize the internal data of the macro residing in ; the CLA write memory space
		CNTL_2P2Z_DBUFF_CLA_INIT 1 ;(Initialize variables in CLA writable memory space )
		.ref _Fdbk_CLA
		.ref _Out_CLA
		MMOVF32 MR0, #0.0L
		MF32TOUI32   MR0, MR0
		MMOV32 @_Fdbk_CLA,MR0
		MMOV32 @_Out_CLA,MR0
		;(Initialize variables in CLA writable memory space )
		.ref _RltA_CLA
		.ref _RltB_CLA
		.ref _RltC_CLA
		.ref _RltD_CLA
		.ref _RltE_CLA
		.ref _RltF_CLA
		.ref _RltG_CLA
		.ref _RltH_CLA
		 MMOVF32 MR0, #0.0L
		 MMOV32 @_RltA_CLA,MR0
		 MMOV32 @_RltB_CLA,MR0
		 MMOV32 @_RltC_CLA,MR0
		 MMOV32 @_RltD_CLA,MR0
		 MMOV32 @_RltE_CLA,MR0
		 MMOV32 @_RltF_CLA,MR0
		 MMOV32 @_RltG_CLA,MR0
		 MMOV32 @_RltH_CLA,MR0
	.endif
    MSTOP 
    MNOP
    MNOP
    MNOP 
_Cla1Task8End:

_Cla1Prog_End:

