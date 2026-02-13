
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
        _push macro reg
            push &reg
        endm 
        _pop macro reg
            pop &reg
        endm 
        _peek macro reg
            pop &reg
            push &reg
        endm 
        _pushv macro reg, optional
            push &reg
        endm 
        _popv macro reg, optional
            pop &reg
        endm 
        _peekv macro reg, optional
            pop &reg
            push &reg
        endm 
        _pushad macro optional
            pushad
        endm 
        _popad macro optional
            popad
        endm
        _correction macro number
        endm 
        fregTLS macro cual1:VARARG
           exitm<SmplMathNotPresent : XWORD>
        endm
     endif
