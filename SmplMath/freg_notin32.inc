
;/************************************************************
; * This file is an extention of the SmplMath macros-system. *
; *                                                          *
; *            Copyright by HSE, 2019-2022                   *
; *                                                          *
; *          https://github.com/ASMHSE/SmplMath/             *
; *                                                          *
; *    Further Copyright and Legal statements are located    *
; *    in the documentation (Documentation.pdf).             *
; *                                                          *
; ************************************************************/

; use push/pop opcodes in 32 bits 
; usefull in dual code, just as example here
;
; Usage:
;
;  include \masm32\macros\SmplMath\math.inc
;  include \masm32\macros\SmplMath\freg_notin32.inc
  
    if @WordSize eq 4
        freg_push macro reg
            push &reg
        endm 
        freg_pop macro reg
            pop &reg
        endm 
        freg_peek macro reg
            pop &reg
            push &reg
        endm 
        freg_pushv macro reg
            push &reg
        endm 
        freg_popv macro reg
            pop &reg
        endm 
        freg_peekv macro reg
            pop &reg
            push &reg
        endm 
       freg_pushad macro
            pushad
        endm 
        freg_popad macro
            popad
        endm 
        fregTLS macro cual1:VARARG
           exitm< SmplMathNotPresent : xword>
        endm
     endif
