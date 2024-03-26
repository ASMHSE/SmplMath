
;/************************************************************
; * This file is an extension of the SmplMath macros-system. *
; *                                                          *
; *            Copyright by HSE, 2019-2022                   *
; *                                                          *
; *          https://github.com/ASMHSE/SmplMath/             *
; *                                                          *
; *    Further Copyright and Legal statements are located    *
; *    in the documentation (Documentation.pdf).             *
; *                                                          *
; ************************************************************/


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

IFNDEF OWORD_BYTE
	OWORD_BYTE typedef OWORD
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

    fsta_var TEXTEQU <&_type ptr >,freg_tls_name,<[>,%((freg_tls_curr_used+(fsta_align-1))AND(-fsta_align))- fsta_size,<]>
    freg_tls_curr_used = ((freg_tls_curr_used+(fsta_align-1))AND(-fsta_align)) - fsta_size
    EXITM fsta_var
endm

@fregTLS_line_access macro _type:req
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

    fsta_var TEXTEQU <&_type ptr >,freg_tls_name,<[>,%((freg_tls_curr_used+(fsta_align-1))AND(-fsta_align))- fsta_size,<]>
 ;   freg_tls_curr_access = ((freg_tls_curr_used+(fsta_align-1))AND(-fsta_align)) - fsta_size
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
        elseif TYPE(reg) eq OWORD
            freg_bool_byte TEXTEQU @fregTLS_line_alloc(OWORD_BYTE)
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
    elseif TYPE(reg) eq OWORD
        movdqa oword ptr freg_bool_byte , reg
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
        elseif TYPE(reg) eq OWORD
            freg_bool_byte TEXTEQU @fregTLS_line_retrieve(OWORD_BYTE)
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
        mov reg, dword ptr freg_bool_byte
    elseif TYPE(reg) eq QWORD
        mov reg, qword ptr freg_bool_byte
    elseif TYPE(reg) eq OWORD
        movdqa reg, oword ptr freg_bool_byte
    else
        if @WordSize eq 8
            mov reg,qword ptr freg_bool_byte
        else
            mov reg,dword ptr freg_bool_byte
        endif
        ;.err reg is more than qword 
    endif
endm


freg_peek macro reg:REQ

    IFE @freg_TLS?()
        freg_bool_byte TEXTEQU get_last_reglbl(<freg_bool_byte_>)
;        set_previous_reglbl 
    ELSE
        if TYPE(reg) eq BYTE
            freg_bool_byte TEXTEQU @fregTLS_line_access(BOOLEAN_BYTE)
        elseif TYPE(reg) eq WORD
            freg_bool_byte TEXTEQU @fregTLS_line_access(REAL2_BYTE)
        elseif TYPE(reg) eq DWORD
            freg_bool_byte TEXTEQU @fregTLS_line_access(REAL4_BYTE)
        elseif TYPE(reg) eq QWORD
            freg_bool_byte TEXTEQU @fregTLS_line_access(REAL8_BYTE)
        elseif TYPE(reg) eq OWORD
            freg_bool_byte TEXTEQU @fregTLS_line_access(OWORD_BYTE)
        else
            if @WordSize eq 8
                freg_bool_byte TEXTEQU @fregTLS_line_access(REAL8_BYTE)
            else
                freg_bool_byte TEXTEQU @fregTLS_line_access(REAL4_BYTE)
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
    elseif TYPE(reg) eq OWORD
        movdqa reg,oword ptr freg_bool_byte
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

freg_peekv macro var:REQ, reg
    ifnb <reg>
        freg_peek &reg
        mov &var, &reg
    else
        if @WordSize eq 4
            freg_peek eax
            mov &var, eax
        else
            freg_peek r10
            mov &var, r10
        endif
    endif
endm

freg_pushad macro
    if @WordSize eq 4
        freg_push EAX
        freg_push ECX
        freg_push EDX
        freg_push EBX
        freg_push ESP
        freg_push EBP
        freg_push ESI
        freg_push EDI
    else
        freg_push RAX
        freg_push RCX
        freg_push RDX
        freg_push RBX
        freg_push RSP
        freg_push RBP
        freg_push RSI
        freg_push RDI
    endif
endm

freg_popad macro
    if @WordSize eq 4
        freg_pop EDI
        freg_pop ESI
        freg_pop EBP
        freg_pop ESP
        freg_pop EBX
        freg_pop EDX
        freg_pop ECX
        freg_pop EAX
    else
        freg_pop RDI
        freg_pop RSI
        freg_pop RBP
        freg_pop RSP
        freg_pop RBX
        freg_pop RDX
        freg_pop RCX
        freg_pop RAX
    endif
endm

; ——————————————————————————————————————————————————————————————————————————————————————————————————
;
; freg_PushAll, freg_PopAll, freg_PushAllRev and freg_PopAllRev 
;                      are adaptations from
;          PushAll, PopAll, PushAllRev and PopAllRev in

; Title:      System.inc
; Author:     G. Friedrich
; Version:    C.1.0
; Purpose:    ObjAsm collection of useful macros.
; Notes:      Version C.1.0, October 2017
;               - First release.

; ObjAsm Homepage = http://objasm.x10host.com/
; ObjAsm Development repository = https://github.com/ObjAsm
; ObjAsm Online documentation = https://biterider.gitbook.io/objasm/

; ——————————————————————————————————————————————————————————————————————————————————————————————————
; Macro:      freg_PushAll
; Purpose:    Push recursively all arguments on stack.
; Arguments:  Arg1: List of arguments.
; Returns:    Nothing.

freg_PushAll macro ArgList:vararg
  $$EaxUsed = FALSE
  %for $$Arg, <ArgList>
    ifnb <$$Arg>
      $$UArg textequ $Upper($$Arg)
      if $$EaxUsed ne FALSE
        $$IsEax = FALSE
        $$EaxPos InStr 1, $$UArg, <RAX>
        if $$EaxPos ne 0
           $$IsEax = TRUE
          if $$EaxPos gt 1
            if @InStr(1, %??WordChars, @SubStr(%$$UArg, $$EaxPos - 1, 1)) gt 0
              $$IsEax = FALSE
            endif
          endif
          if ( $$EaxPos ne 0 and $$EaxPos le @SizeStr(%$$UArg) - 3)
            if @InStr(1, %??WordChars, @SubStr(%$$UArg, $$EaxPos + 3, 1)) gt 0
              $$IsEax = FALSE
            endif
          endif
        endif
        if $$IsEax ne FALSE
          .err <RAX register already in use.>
          exitm
        else
            $$EaxPos InStr 1, $$UArg, <EAX>
            if $$EaxPos ne 0
               $$IsEax = TRUE
              if $$EaxPos gt 1
                if @InStr(1, %??WordChars, @SubStr(%$$UArg, $$EaxPos - 1, 1)) gt 0
                  $$IsEax = FALSE
                endif
              endif
              if ( $$EaxPos ne 0 and $$EaxPos le @SizeStr(%$$UArg) - 3)
                if @InStr(1, %??WordChars, @SubStr(%$$UArg, $$EaxPos + 3, 1)) gt 0
                  $$IsEax = FALSE
                endif
              endif
            endif
        endif
        if $$IsEax ne FALSE
          .err <EAX register already in use.>
          exitm
        else
          $$EaxPos InStr 1, $$UArg, <AX>
          if $$EaxPos ne 0
            $$IsEax = TRUE
            if $$EaxPos gt 1
              if @InStr(1, %??WordChars, @SubStr(%$$UArg, $$EaxPos - 1, 1)) gt 0
                $$IsEax = FALSE
              endif
            endif
            if ($$EaxPos ne 0 and $$EaxPos le @SizeStr(%$$UArg) - 2)
              if @InStr(1, %??WordChars, @SubStr(%$$UArg, $$EaxPos + 2, 1)) gt 0
                $$IsEax = FALSE
              endif
            endif
          endif
        endif
        if $$IsEax ne FALSE
          .err <AX register already in use.>
          exitm
        else
           $$EaxPos InStr 1, $$UArg, <AL>
          if $$EaxPos ne 0
             $$IsEax = TRUE
            if $$EaxPos gt 1
              if @InStr(1, %??WordChars, @SubStr(%$$UArg, $$EaxPos - 1, 1)) gt 0
                $$IsEax = FALSE
              endif
            endif
            if ($$EaxPos ne 0 and  $$EaxPos le @SizeStr(%$$UArg) - 2)
              if @InStr(1, %??WordChars, @SubStr(%$$UArg, $$EaxPos + 2, 1)) gt 0
                $$IsEax = FALSE
              endif
            endif
          endif
        endif
        if $$IsEax ne FALSE
          .err <AL register already in use.>
          exitm
        endif
      endif

      $$Pos1 InStr 1, $$UArg, <ADDR >
      if $$Pos1 eq 1
        $$Attributes = opattr(@SubStr($$Arg, $$Pos1 + 5))
        if ($$Attributes eq (OAT_RELOCATABLE or OAT_NO_ERROR)) or \
           ($$Attributes eq (OAT_RELOCATABLE or OAT_NO_ERROR or OAT_SS_RELATIVE))
          $$EaxUsed = TRUE
          lea xax, @SubStr($$Arg, 6)
          freg_push xax
        else
          if @InStr(6, $$Arg, <[>)
            $$EaxUsed = TRUE
            lea xax, @SubStr($$Arg, 6)
            freg_push xax
          else
            freg_push offset(@SubStr($$Arg, 6))
          endif
        endif
      else
        if $IsFloat($$Arg)
          db 68h
          REAL4 $$Arg                                   ;;push REAL4 value
        else
          freg_push $$Arg                                    ;;push one argument and repeat
        endif
      endif
    endif
  endm
endm

; ——————————————————————————————————————————————————————————————————————————————————————————————————
; Macro:      freg_PopAll
; Purpose:    Pop a list of arguments of the stack.
; Arguments:  Arg1: List of arguments.
; Returns:    Nothing.

freg_PopAll macro ArgList:vararg
  %for $$Arg, <ArgList>
    freg_pop $$Arg                                           ;;push one argument and repeat
  endm
endm

; ——————————————————————————————————————————————————————————————————————————————————————————————————
; Macro:      freg_PushAllRev
; Purpose:    Push all arguments on stack in reverse order.
; Arguments:  Arg1: List of arguments.
; Returns:    Nothing.

freg_PushAllRev macro ArgList:vararg
  freg_PushAll $ArgRev(ArgList)
endm

; ——————————————————————————————————————————————————————————————————————————————————————————————————
; Macro:      freg_PopAllRev
; Purpose:    Pop recursively all arguments on stack in reverse order.
; Arguments:  Arg1: List of arguments.
; Returns:    Nothing.

freg_PopAllRev macro ArgList:vararg
  $$ArgList textequ $ArgRev(ArgList)
  %for $$Arg, <$$ArgList>
    freg_pop $$Arg                                           ;;pop one argument and repeat
  endm
endm

