;/********************************************************
; * This file is part of the SmplReg macros-system.      *
; *                                                      *
; *            Copyright by HSE, 2019-2021               *
; *              for the MASM32 SDK                      *
; *                                                      *
; ********************************************************/

;/********************************************************
; * This file is based in SmplMath macros-system.        *
; *                                                      *
; *          Copyright by qWord, 2011-2014               *
; *                                                      *
; *          SmplMath.Masm{at}gmx{dot}net                *
; *    http://sourceforge.net/projects/smplmath/         *
; *                                                      *
; ********************************************************/

IFDEF __SMPLREG__
	.err <SmplReg: double inclusion of SmplReg.inc>
	freg_glb_error = 1
ELSE
	__SMPLREG__ TEXTEQU <010001>
	freg_glb_error = 0
ENDIF

IFNDEF BOOLEAN_BYTE
	BOOLEAN_BYTE typedef BYTE
ENDIF

IFNDEF REAL2_BYTE
	REAL2_BYTE typedef WORD
ENDIF

IFNDEF REAL4_BYTE
	REAL4_BYTE typedef DWORD
ENDIF

IFNDEF REAL8_BYTE
	REAL8_BYTE typedef REAL8
ENDIF

unique_reg_cntr = 0
maximum_reg_cntr = 0
counter_reg_cntr = 0

freg_reset macro
    ifndef counter_reg_cntr
        counter_reg_cntr = 0
    else
      counter_reg_cntr = counter_reg_cntr + 1
    endif
endm

get_unique_reglbl macro name:req
   ifndef counter_reg_cntr
       counter_reg_cntr = 0
   endif
    unique_reg_cntr = unique_reg_cntr + 1
    EXITM @CatStr(<&name>,%unique_reg_cntr,<@>,counter_reg_cntr)
endm

get_last_reglbl macro name:req
	EXITM @CatStr(<&name>,%unique_reg_cntr,<@>,counter_reg_cntr)
endm

set_previous_reglbl macro
	unique_reg_cntr = unique_reg_cntr - 1
endm

@fregTLS_warning macro
    %echo @FileCur: WARNING: code not thread save: line @CatStr(%@Line)
endm

fregTLS_curr_line = -1
freg_tls_curr_used = 0
freg_tls_name TEXTEQU <>
freg_tls_cb = 0
freg_tls_last TEXTEQU <>
freg_tls_last_size = 0
freg_tls_byte = 0
freg_tls_alloc_line = 0

@freg_TLS? macro
	IFB freg_tls_name
		EXITM <0>
%	ELSEIFDEF freg_tls_name
		EXITM <1>
	ENDIF
	EXITM <0>
endm

@fregTLS_line_alloc macro _type:req
	IFB freg_tls_name
		freg_glb_error_msg TEXTEQU <[#CG17] thread local storage not initialized>
		freg_glb_error = 1
		EXITM freg_retval@error(_type)
%	ELSEIFNDEF freg_tls_name
		freg_glb_error_msg TEXTEQU <[#CG18] thread local storage not initialized>
		freg_glb_error = 1
		EXITM freg_retval@error(_type)
	ENDIF
	;IF fregTLS_curr_line NE @Line
	;	fregTLS_curr_line = @Line
	;	freg_tls_curr_used = 0
	;	freg_tls_byte = 0
       ;ELSE  
       ;    freg_tls_curr_used = 0

	;ENDIF
	
	fsta_size = sizeof _type
	IFE fsta_size 
		freg_glb_error_msg TEXTEQU <[#CG19] invalid type qualifier / type not supported by current back end: _type>
		freg_glb_error = 1
           % .err freg_glb_error_msg
		EXITM freg_retval@error(_type)
	ENDIF
	IF fsta_size EQ 12
		fsta_align = 4
	ELSE
		fsta_align = fsta_size
	ENDIF
	IF fsta_align GT @WordSize
		fsta_align = @WordSize
	ENDIF
      

	IF ((freg_tls_curr_used+(fsta_align-1))AND(-fsta_align))+fsta_size GT freg_tls_cb
     
	%	.err <[#CG20] TLS: buffer to small - increase size at line @CatStr(%freg_tls_alloc_line) : fregTLS[,@CatStr(%((freg_tls_curr_used+(fsta_align-1))AND(-fsta_align))+fsta_size)]>
		freg_glb_error = 1
		freg_tls_curr_used = ((freg_tls_curr_used+(fsta_align-1))AND(-fsta_align)) + fsta_size
		EXITM freg_retval@error(_type)
	ENDIF
	fsta_var TEXTEQU <&_type ptr >,freg_tls_name,<[>,%((freg_tls_curr_used+(fsta_align-1))AND(-fsta_align)),<]>
	freg_tls_curr_used = ((freg_tls_curr_used+(fsta_align-1))AND(-fsta_align)) + fsta_size
	EXITM fsta_var
endm

@fregTLS_line_retrieve macro _type:req
	IFB freg_tls_name
		freg_glb_error_msg TEXTEQU <[#CG17] thread local storage not initialized>
		freg_glb_error = 1
		EXITM freg_retval@error(_type)
%	ELSEIFNDEF freg_tls_name
		freg_glb_error_msg TEXTEQU <[#CG18] thread local storage not initialized>
		freg_glb_error = 1
		EXITM freg_retval@error(_type)
	ENDIF
	
	fsta_size = sizeof _type
	IFE fsta_size 
		freg_glb_error_msg TEXTEQU <[#CG19] invalid type qualifier / type not supported by current back end: _type>
		freg_glb_error = 1
           % .err freg_glb_error_msg
		EXITM freg_retval@error(_type)
	ENDIF
	IF fsta_size EQ 12
		fsta_align = 4
	ELSE
		fsta_align = fsta_size
	ENDIF
	IF fsta_align GT @WordSize
		fsta_align = @WordSize
	ENDIF
	fsta_var TEXTEQU <&_type ptr >,freg_tls_name,<[>,%((freg_tls_curr_used+(fsta_align-1))AND(-fsta_align))- fsta_size,<]>
	freg_tls_curr_used = ((freg_tls_curr_used+(fsta_align-1))AND(-fsta_align)) - fsta_size
	EXITM fsta_var
endm


freg_retval@error macro _type
LOCAL lbl
	.data?
		lbl _type ?
	.code
	EXITM <lbl>
endm
;------------------------------------------------------------------------------


unique_reg_cntr = 0
get_unique_reglbl macro name:req
	unique_reg_cntr = unique_reg_cntr + 1
	EXITM @CatStr(<&name>,%unique_reg_cntr)
endm


get_last_reglbl macro name:req
	EXITM @CatStr(<&name>,%unique_reg_cntr)
endm

set_previous_reglbl macro; name:req
	unique_reg_cntr = unique_reg_cntr - 1
    EXITM
endm

freg_correction macro val1:req
	freg_tls_curr_used = freg_tls_curr_used &val1 *@WordSize
    EXITM
endm
;------------------------------------------------------------------------------

freg_push macro reg:REQ
    IFNDEF freg_line
	freg_line = @Line
    ENDIF
    IFE @freg_TLS?()
      @fregTLS_warning
	  freg_bool_byte TEXTEQU get_unique_reglbl(<freg_bool_byte_>)
        if unique_reg_cntr GT maximum_reg_cntr
           IF freg_line NE @Line
        	freg_line = @Line
        	.data?
               align 4	
        	.code
          ENDIF
       .data?
           freg_bool_byte db 8 dup (?)
           .code
           maximum_reg_cntr = unique_reg_cntr
       endif
    ELSE
        if TYPE(reg) eq BYTE
            freg_bool_byte TEXTEQU @fregTLS_line_alloc(BOOLEAN_BYTE)
        elseif TYPE(reg) eq WORD
            freg_bool_byte TEXTEQU @fregTLS_line_alloc(REAL2_BYTE)
        elseif TYPE(reg) eq DWORD
            freg_bool_byte TEXTEQU @fregTLS_line_alloc(REAL4_BYTE)
        elseif TYPE(reg) eq QWORD
            freg_bool_byte TEXTEQU @fregTLS_line_alloc(REAL8_BYTE)
        else
            if @WordSize eq 8
                freg_bool_byte TEXTEQU @fregTLS_line_alloc(REAL8_BYTE)
            else
                freg_bool_byte TEXTEQU @fregTLS_line_alloc(REAL4_BYTE)
            endif
        endif    
    ENDIF

    if TYPE(reg) eq BYTE
        mov byte ptr freg_bool_byte , reg
    elseif TYPE(reg) eq WORD
        mov word ptr freg_bool_byte , reg
    elseif TYPE(reg) eq DWORD
        mov dword ptr freg_bool_byte , reg
    elseif TYPE(reg) eq QWORD
        mov qword ptr freg_bool_byte , reg
    else
        if @WordSize eq 8
            mov qword ptr freg_bool_byte , reg
        else
            mov dword ptr freg_bool_byte , reg
        endif
        ;.err reg is more than qword 
    endif
endm

freg_pop macro reg:REQ

    IFE @freg_TLS?()
        freg_bool_byte TEXTEQU get_last_reglbl(<freg_bool_byte_>)
        set_previous_reglbl 
    ELSE
        if TYPE(reg) eq BYTE
            freg_bool_byte TEXTEQU @fregTLS_line_retrieve(BOOLEAN_BYTE)
        elseif TYPE(reg) eq WORD
            freg_bool_byte TEXTEQU @fregTLS_line_retrieve(REAL2_BYTE)
        elseif TYPE(reg) eq DWORD
            freg_bool_byte TEXTEQU @fregTLS_line_retrieve(REAL4_BYTE)
        elseif TYPE(reg) eq QWORD
            freg_bool_byte TEXTEQU @fregTLS_line_retrieve(REAL8_BYTE)
        else
            if @WordSize eq 8
                freg_bool_byte TEXTEQU @fregTLS_line_retrieve(REAL8_BYTE)
            else
                freg_bool_byte TEXTEQU @fregTLS_line_retrieve(REAL4_BYTE)
            endif
        endif    
    ENDIF

    if TYPE(reg) eq BYTE
        mov reg, byte ptr freg_bool_byte
    elseif TYPE(reg) eq WORD
        mov reg, word ptr freg_bool_byte
    elseif TYPE(reg) eq DWORD
        mov reg,dword ptr freg_bool_byte
    elseif TYPE(reg) eq QWORD
        mov reg,qword ptr freg_bool_byte
    else
        if @WordSize eq 8
            mov reg,qword ptr freg_bool_byte
        else
            mov reg,dword ptr freg_bool_byte
        endif
        ;.err reg is more than qword 
    endif
endm

fregTLS macro name:=<freg_tls>,cb:=<4*@WordSize>
;	IFB FSBE_ENTITY
;		EXITM <&name&[1]:BYTE>
;	ELSEIF freg_glb_error
;		EXITM <&name&[1]:BYTE>
;	ENDIF
%	IFDEF freg_tls_name
		.err <TLS duplication>
	ELSE
             freg_tls_curr_used = 0 ; 04/10/2020 HSE 
		freg_tls_alloc_line = @Line
		freg_tls_cb = 0; FSBE_FNC(%FSBE_ENTITY,%FSEN_TLS_SIZE)
		IF freg_tls_cb NE 0 AND freg_tls_cb GT cb
			freg_tls_cb = (freg_tls_cb+15) AND (-16)
		ELSE
			freg_tls_cb = (cb+15) AND (-16)
		ENDIF
		freg_tls_name TEXTEQU @TrimStr(<&name>)
		EXITM @CatStr(<&name&[freg_tls_cb]:BYTE>)
	ENDIF
endm

freg_pushv macro var:REQ, reg
    ifnb <reg>
        mov  &reg, &var
        freg_push &reg
    else
        if @WordSize eq 4
            mov eax, &var
            freg_push eax
        else
            mov r10, &var
            freg_push r10
        endif
    endif
endm

freg_popv macro var:REQ, reg
    ifnb <reg>
        freg_pop &reg
        mov &var, &reg
    else
        if @WordSize eq 4
            freg_pop eax
            mov &var, eax
        else
            freg_pop r10
            mov &var, r10
        endif
    endif
endm
