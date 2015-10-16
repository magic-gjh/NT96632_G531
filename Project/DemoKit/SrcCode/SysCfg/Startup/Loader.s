;   Copyright   Novatek Microelectronics Corp. 2006.  All rights reserved.
;
;   @file       Loader.s
;   @ingroup
;
;   @brief      Inistalize stack, ZI/RW data and CLib runtime
;               Inistalize stack, ZI/RW data and CLib runtime
;
;   @note       None
;
;   @version    V1.00.000
;   @author     Novatek FW Team
;   @date       2006/01/27

    AREA        SYSTEM_LOADER, CODE, READONLY

    GET         StdSMac.h
    GET         Chipset.h
    GET         KerCfg.h

    EXPORT      Reset_Handler

    IMPORT      InitCLib
    IMPORT      EnableCacheMPU
    IMPORT      ProgramStart

    IMPORT      ||Image$$GENERAL_ZI$$ZI$$Base||
    IMPORT      ||Image$$GENERAL_ZI$$ZI$$Limit||
    IMPORT      ||Load$$PART_ONE_RW$$Base||
    IMPORT      ||Image$$PART_ONE_RW$$Base||
    IMPORT      ||Image$$PART_ONE_RW$$Limit||
;#NT#2009/08/11#Elvis Chuang -begin
    EXPORT      NULL_Func_Handler
    IMPORT      NewResetHandler
    IMPORT      CallerAddress
;#NT#2009/08/11#Elvis Chuang -end
;------------------------ RESET HANDLER -------------------------
Reset_Handler
Loader
    BL      Init_Stack

    ; Initialize ZI and RW
    BL      Init_Data

    BL      InitCLib
    BL      EnableCacheMPU

    B       ProgramStart

Init_Stack
;   Because we will change mode, so save LR to general register
    MOV     R7, LR

;   Enter IRQ mode and set up the IRQ stack pointer
    LDR     R1, =StackEnd_IRQ                   ; end of IRQ stack
    MOV     R0, #(Mode_IRQ | I_Bit | F_Bit)     ; No interrupts
    MSR     CPSR_c, R0
    MOV     SP, R1

;   Enter FIQ mode and set up the FIQ stack pointer
    LDR     R1, =StackEnd_FIQ                   ; end of FIQ stack
    MOV     R0, #(Mode_FIQ | I_Bit | F_Bit)     ; No interrupts
    MSR     CPSR_c, R0
    MOV     SP, R1

;   Enter ABT mode and set up the ABT stack pointer
    LDR     R1, =StackEnd_ABT                   ; end of ABT stack
    MOV     R0, #(Mode_ABT | I_Bit | F_Bit)     ; No interrupts
    MSR     CPSR_c, R0
    MOV     SP, R1

;   Enter UNDEF mode and set up the UNDEF stack pointer
    LDR     R1, =StackEnd_UNDEF                 ; end of UNDEF stack
    MOV     R0, #(Mode_UNDEF | I_Bit | F_Bit)   ; No interrupts
    MSR     CPSR_c, R0
    MOV     SP, R1

;   Enter SVC mode and set up the SVC stack pointer
    LDR     R1, =StackEnd_SVC                   ; end of SVC stack
    MOV     R0, #(Mode_SVC | I_Bit | F_Bit)     ; No interrupts
    MSR     CPSR_c, R0
    MOV     SP, R1

;   Now change to SYSTEM mode and set up SYSTEM mode stack.
    LDR     R1, =StackEnd_USR                   ; end of USR stack
    MOV     R0, #(Mode_SYS | I_Bit | F_Bit)     ; No interrupts
    MSR     CPSR_c, R0
    MOV     SP, R1

;   return, LR has been saved to R7
    MOV     PC, R7

; Initialize ZI and copy part one RW
Init_Data
; Clear ZI (BSS)
    MOV     R2, #0x0
    LDR     R0, =||Image$$GENERAL_ZI$$ZI$$Base||
    LDR     R1, =||Image$$GENERAL_ZI$$ZI$$Limit||
Do_Clear
    STR     R2, [R0], #0x4
    CMP     R0, R1
    BCC     Do_Clear
; Copy RW
    LDR     R0, =||Load$$PART_ONE_RW$$Base||
    LDR     R1, =||Image$$PART_ONE_RW$$Base||
; If Load == Image, copy is unnecessary
    CMP     R0, R1
    BEQ     Copy_Done
    LDR     R2, =||Image$$PART_ONE_RW$$Limit||
Do_Copy
    LDR     R3, [R0], #0x4
    STR     R3, [R1], #0x4
    CMP     R1, R2
    BCC     Do_Copy
Copy_Done
    MOV     PC, LR
;#NT#2009/08/11#Elvis Chuang -begin
;Replace the Reset_Handler after power on
NULL_Func_Handler
    LDR     R1, p_CallerAddress
    STR     LR,[R1]
    BL      NewResetHandler
    LDR     R1, p_CallerAddress
    LDR     LR,[R1]
    MOV     PC, LR
p_CallerAddress       DCD     CallerAddress
;#NT#2009/08/11#Elvis Chuang -end
    END
