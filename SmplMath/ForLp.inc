
;/************************************************************
; * This file is an extension of the SmplMath macros-system. *
; *                                                          *
; *            Copyright by HSE, 2020-2022                   *
; *                                                          *
; *          https://github.com/ASMHSE/SmplMath/             *
; *                                                          *
; *    Further Copyright and Legal statements are located    *
; *    in the documentation (Documentation.pdf).             *
; *                                                          *
; ************************************************************/

;############################################################
; For-Next macros from Chapter Eight.
; See Chapter Eight for details on how this works.
; (The Art of Assembly Language - Randall Hyde)

MakeLbl	macro FirstHalf, SecondHalf
&FirstHalf&&SecondHalf&:
		endm

jgDone	macro FirstHalf, SecondHalf
		jg &FirstHalf&&SecondHalf&
		endm

jmpLoop	macro FirstHalf, SecondHalf
		jmp &FirstHalf&&SecondHalf&
		endm

ForLp	macro	LCV:req, Start:req, Stop:req,AuxReg

             inb <&AuxReg>
                  _eax textequ <&AuxReg> 
              else
                  if @WordSize eq 8
                      _eax textequ <rax>
                  else
                      _eax textequ <eax>
                  endif
              endif

		ifndef $$For&LCV&
$$For&LCV&	=	0
		else
$$For&LCV&	=	$$For&LCV& + 1
		endif

		mov	_eax , Start
		mov	LCV, xax

		MakeLbl $$For&LCV&, %$$For&LCV&
		mov	_eax , LCV
		cmp	_eax , Stop
		jgDone $$Next&LCV&, %$$For&LCV&
		endm

Next		macro	LCV:req
		inc	LCV
		jmpLoop $$For&LCV&, %$$For&LCV&
		MakeLbl $$Next&LCV&, %$$For&LCV&
		endm

ExitFor	macro	LCV:req
		jmpLoop $$Next&LCV&, %$$For&LCV&
		endm

CutFor	macro	LCV:req
		inc	LCV
		jmpLoop $$For&LCV&, %$$For&LCV&
		endm

ForLp_df macro LCV:req, Start:req, Stop:req,AuxReg       ;; not including Stop ej: i < Stop
             inb <&AuxReg>
                  _eax textequ <&AuxReg> 
              else
                  if @WordSize eq 8
                      _eax textequ <rax>
                  else
                      _eax textequ <eax>
                  endif
              endif
		mov _eax, Stop
		dec _eax
		mov LCV&_t, xax
		ForLp	&LCV, &Start, LCV&_t
		endm

;######################################################
; For-Next Descendent Loop macros
; Adaptation of For-Next (The Art of Assembly Language - Randall Hyde) by HSE

jgDoneN	macro FirstHalf, SecondHalf
		jl &FirstHalf&&SecondHalf&
		endm

ForLpN	macro	LCV:req, Start:req, Stop:req, AuxReg

              inb <&AuxReg>
                  _eax textequ <&AuxReg> 
              else
                  if @WordSize eq 8
                      _eax textequ <rax>
                  else
                      _eax textequ <eax>
                  endif
              endif
		ifndef $$For&LCV&
$$For&LCV&	=	0
		else
$$For&LCV&	=	$$For&LCV& + 1
		endif

		mov	_eax , Start
		mov	LCV, _eax

		MakeLbl $$For&LCV&, %$$For&LCV&
		mov	_eax , LCV
		cmp	_eax , Stop
		jgDoneN $$Next&LCV&, %$$For&LCV&
		endm

NextN	       macro	LCV:req
		dec	LCV
		jmpLoop $$For&LCV&, %$$For&LCV&
		MakeLbl $$Next&LCV&, %$$For&LCV&
		endm

CutForN	macro	LCV:req
		dec	LCV
		jmpLoop $$For&LCV&, %$$For&LCV&
		endm


;######################################################
; For-Next Floating point macros
; Adaptation of For-Next (The Art of Assembly Language - Randall Hyde) by HSE

        ;-----------------------------
             f2f macro a1, a2
                fld a2
                fstp a1
             endm
        	compvalR macro a1,a2
                Fld	a1
                fcomp a2
                fstsw ax
        	endm

        FP_GREATTHAN equ  00h
        FP_LESSTHAN  equ  01h
        FP_EQUALTO   equ  04h
    
        ;-----------------------------
MakeLblR	macro FirstHalf, SecondHalf
&FirstHalf&&SecondHalf&:
		endm

jgDoneR	macro FirstHalf, SecondHalf
		.if ( ah & FP_LESSTHAN )
	          jmp &FirstHalf&&SecondHalf&
             .endif
		endm

jmpLoopR	macro FirstHalf, SecondHalf
		jmp &FirstHalf&&SecondHalf&
		endm

ForLpR	macro	LCV:req, Start:req, Stop:req, Step:req

		ifndef $$For&LCV&
$$For&LCV&	=	0
		else
$$For&LCV&	=	$$For&LCV& + 1
		endif

		fld   Start
		fstp  LCV
		
		@CatStr($$For&LCV&step) textequ  <&Step>
		
		MakeLblR $$For&LCV&, %$$For&LCV&
		compvalR	 Stop, LCV 
		
             jgDoneR $$Next&LCV&, %$$For&LCV&
		endm

NextR       macro	LCV:req, stap
            ifnb <stap>
            %	 @CatStr(<echo &stap ya no es necesario en>,%@FileCur,< >, %@Line)
            endif
            fld   	LCV
            %fadd $$For&LCV&step
            fstp	LCV
            jmpLoopR $$For&LCV&, %$$For&LCV&
            MakeLblR $$Next&LCV&, %$$For&LCV&
            endm

ExitForR    macro	LCV:req
            jmpLoopR $$Next&LCV&, %$$For&LCV&
            endm

CutForR     macro	LCV:req , stap
            ifnb <stap>
            %	 @CatStr(<echo &stap ya no es necesario en>,%@FileCur,< >, %@Line)
            endif
            fld   	LCV
            %fadd $$For&LCV&step
            fstp  LCV
            jmpLoopR $$For&LCV&, %$$For&LCV&
            endm
 
