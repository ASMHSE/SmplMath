;/********************************************************
; * This file is part of the SmplMath macros-system.     *
; *                                                      *
; *          Copyright by qWord, 2011-2014               *
; *                                                      *
; *          SmplMath.Masm{at}gmx{dot}net                *
; *    http://sourceforge.net/projects/smplmath/         *
; *                                                      *
; *  Further Copyright and Legal statements are located  *
; *  in the documentation (Documentation.pdf).           *
; *                                                      *
; ********************************************************/

FSBE_REGISTER_BACKEND <FSLV_X86_FPU_ENTITY> 

FSLV_X86_FPU_ENTITY macro code:req,param1,param2
	IF code EQ FSEN_INIT
		EXITM <-1>
	ELSEIF code EQ FSEN_END
		EXITM <0>
	ELSEIF code EQ FSEN_MAX_STACK
		EXITM <8>	; the FPU has 8 registers ;-)
	ELSEIF code EQ FSEN_STCK_USAGE
		EXITM <0>
	ELSEIF code EQ FSEN_TEST_ATTR
		IFIDNI <stck>,<&param1>
			EXITM <-1>
		ELSEIFIDNI <free>,<&param1>
			EXITM <-1>
		ELSEIFIDNI <pop>,<&param1>
			EXITM <-1>
		ELSE
			EXITM <0>
		ENDIF
	ELSEIF code EQ FSEN_MAPPING
		FSBE_ENTITY				TEXTEQU <FSLV_X86_FPU_ENTITY>
		FSBE_ENUM_TKN			TEXTEQU <>          
		FSBE_IS_TKN_REG       	TEXTEQU <FSLV_X86_FPU_IS_TKN_REG>
		FSBE_IS_TKN_CONST		TEXTEQU <FSLV_X86_FPU_IS_TKN_CONST>     
		FSBE_LOAD_CONST       	TEXTEQU <FSLV_X86_FPU_LOAD_CONST>
		FSBE_PREPARE_CONST		TEXTEQU <FSLV_X86_FPU_PREPARE_CONST>   
		FSBE_LOAD_ARG         	TEXTEQU <FSLV_X86_FPU_LOAD_ARG>
		FSBE_PREPARE_ARG      	TEXTEQU <FSLV_X86_FPU_PREPARE_ARG>
		FSBE_LOAD_REG         	TEXTEQU <FSLV_X86_FPU_LOAD_REG>
		FSBE_REG0_OPRT_MEM    	TEXTEQU <FSLV_X86_FPU_REG0_OPRT_MEM>      
		FSBE_MEM_OPRT_REG0    	TEXTEQU <FSLV_X86_FPU_MEM_OPRT_REG0>		
		FSBE_REG0_OPRT_REG1_P 	TEXTEQU <FSLV_X86_FPU_REG0_OPRT_REG1_P>
		FSBE_REG0_OPRT_REGX   	TEXTEQU <FSLV_X86_FPU_REG0_OPRT_REGX>  
		FSBE_REG1_OPRT_REG0_P 	TEXTEQU <FSLV_X86_FPU_REG1_OPRT_REG0_P>
		FSBE_REGX_OPRT_REG0   	TEXTEQU <FSLV_X86_FPU_REGX_OPRT_REG0>
		FSBE_FREE_REG         	TEXTEQU <FSLV_X86_FPU_FREE_REG>
		FSBE_SET_REG          	TEXTEQU <FSLV_X86_FPU_SET_REG>
		FSBE_ASSIGN_DEST		TEXTEQU <FSLV_X86_FPU_ASSIGN_DEST>             
		FSBE_REG0_OPRT_RCVAL 	TEXTEQU <FSLV_X86_FPU_REG0_OPRT_RCVAL>
		FSBE_NEG              	TEXTEQU <FSLV_X86_FPU_NEG>       
		FSBE_XCHG             	TEXTEQU <FSLV_X86_FPU_XCHG>
		FSBE_FNC_DSCPTR			TEXTEQU <FSLV_X86_FPU_FNC_DSCPTR>
		FSBE_CALL_FNC			TEXTEQU <FSLV_X86_FPU_CALL_FNC>
		FSBE_CMP				TEXTEQU <FSLV_X86_FPU_CMP>
		fslv_glb_instrset = FSIS_FPU
		EXITM <-1>
	ELSEIF code EQ FSEN_CMP_SUPPORTED
		EXITM <1>
	ELSEIF code EQ FSEN_INSTRSET
		IFIDNI <x86>,<&param1>
			EXITM <-1>
		ELSEIFIDNI <x64>,<&param1>
			EXITM <-1>
		ELSEIFIDNI <FPU>,<&param1>
			EXITM <-1>
		ELSE
			EXITM <0>
		ENDIF
	ELSEIF code EQ FSEN_ALL_INSTRSETS
		EXITM <x86,x64,FPU>
	ELSEIF code EQ FSEN_REQ_RETVAL
		EXITM <0>
	ELSEIF code EQ FSEN_TEST_TYPE
		EXITM @GetArgByIndex(@MatchStrI(param1,0,BOOLEAN_BYTE,SWORD,SDWORD,SQWORD,REAL4,REAL8,REAL10),0,0,1,2,4,8,4,8,12)
	ELSEIF code EQ FSEN_NO_INT_CONST
		EXITM <0>
	ELSEIF code EQ FSEN_NAME
		fsbe_txt TEXTEQU <x86-32/64,FPU>
		EXITM <-1>
	ELSE
		EXITM <0>
	ENDIF
endm


; type: function
; Parameter:
;	fnc_name:	name the fucntion, trimmed
;	flags:		flags for the function descriptor
;              	For the FNCD_NREGS flag, the macro must add the flags, which
;               indicates the instruction set that will be used: FNCD_FPU, FNCD_SSE2, FNCD_SSEX, ...
;	bTest:		if not zero, the macro must check for
;               compatibility and existence of the function fnc_name.
;               
; Return:
;	if bTest = zero: the result of the function desciptor
;   if bTest != zero: true/fale
FSLV_X86_FPU_FNC_DSCPTR macro fnc_name,flags,bTest:=<0>
	IFE bTest
		IF flags AND FNCD_NREGS
			EXITM fslv_fnc_&fnc_name&_dscptr(flags,%FSIS_FPU)
		ELSE
			EXITM fslv_fnc_&fnc_name&_dscptr(flags)
		ENDIF
	ELSE
		IFDEF fslv_fnc_&fnc_name&_dscptr
			IF fslv_fnc_&fnc_name&_dscptr(FNCD_ISTCK) NE 0 AND (fslv_fnc_&fnc_name&_dscptr(FNCD_INSTR_SET) AND FSIS_FPU) NE 0
				EXITM <-1>
			ELSE
				EXITM <0>
			ENDIF
		ELSE
			EXITM <0>
		ENDIF
	ENDIF	
endm

; type: procedure
; Call the macro that creates the function's code
;
FSLV_X86_FPU_CALL_FNC macro fnc_name
	fslv_fnc_&fnc_name&
endm


; type: function, return: through EXITM and text macros
;
; check if tkn is a register (releated to calculations)
; tkn is trimmed.
; return  = 0 => no register
;        != 0 => register , fsbeitr_index = TEXTEQU = register index + 1
;                The index assumes an unused stack.
FSLV_X86_FPU_IS_TKN_REG macro tkn:=<>
	fsoitr_pos INSTR 1,<    st0  st1  st2  st3  st4  st5  st6>,<&tkn>
	fsoitr_size SIZESTR <&tkn>
	IF fsoitr_pos NE 0 AND (fsoitr_pos MOD 5) EQ 0 AND fsoitr_size EQ 3
		fsbeitr_index SUBSTR <&tkn>,3
		EXITM <-1>
	ELSE
		EXITM <0>
	ENDIF	
endm

; type: function
; Checks whether the given token is special constant. If so, the function
; must return an identifier for that constant through the textmacro fsbeic_const_name. The equate fsbeic_mem_arg and fsbeic_neg_mem_arg must
; be non zero, if the constant is resolved as an memory operand by FSBE_PREPARE_CONST().
; fsbeic_neg_mem_arg indicates that there is also a memory operand for the negated constant.
; The macro must also responst to tkn=<pi>.
; return: true/false , fsbeic_const_name TEXTEQU <const name>
FSLV_X86_FPU_IS_TKN_CONST macro tkn:=<>
	fsbeic_mem_arg = 0
	fsbeic_neg_mem_arg = 0
	IFIDNI <&tkn>,<pi>
		fsbeic_const_name TEXTEQU <pi>
		fsbeic_mem_arg = -1
		fsbeic_neg_mem_arg = 0
		EXITM <-1>
	ELSEIF @InStr(1,<1 1.0>,<&tkn>) LE 3 AND @InStr(1,<1 1.0>,<&tkn>) NE 0
		fsbeic_const_name TEXTEQU <one>
		EXITM <-1>
	ELSEIF @InStr(1,<-1 -1.0>,<&tkn>)  LE 5 AND @InStr(1,<-1 -1.0>,<&tkn>) NE 0
		fsbeic_const_name TEXTEQU <mone>
		EXITM <-1>
	ELSEIF @InStr(1,<0 0.0>,<&tkn>) LE 3 AND @InStr(1,<0 0.0>,<&tkn>) NE 0
		fsbeic_const_name TEXTEQU <zero>
		EXITM <-1>
	ELSE
		EXITM <0>
	ENDIF
endm

; type: procedure
; Copy the value from ST(0) to ST(index).
; If bPop is not zero, the stack is poped after the copy operation.
FSLV_X86_FPU_SET_REG macro index,bPop:=<0>
	IF bPop EQ 0
		fst @CatStr(<st!(>,index,<!)>)
	ELSE
		fstp @CatStr(<st!(>,index,<!)>)
	ENDIF
endm


; negate top of stack
FSLV_X86_FPU_NEG macro 
	fchs
endm

FSLV_X86_FPU_XCHG macro index:=<1>
	IF index EQ 1
		fxch
	ELSE
		fxch st(index)
	ENDIF
endm

; type: procedure
FSLV_X86_FPU_LOAD_REG macro index,neg_arg:=<0>
%	fld st(index)
	IF neg_arg NE 0
		fchs
	ENDIF
endm

; type: procedure
; load a argument on top of the stack.
; If neg_arg is not zero, then the sign of the loaded arg must be negated.
;
FSLV_X86_FPU_LOAD_ARG macro tkn,neg_arg:=<0>

	fsola_txt TEXTEQU <&tkn>
	IF @IsGPReg(<&tkn>)
		IF igpr_size NE 0 AND @WordSize EQ 8
			IF igpr_size EQ 8
				mov QWORD ptr [rsp],tkn
				fsola_txt TEXTEQU <QWORD ptr [rsp]>
			ELSEIF igpr_size EQ 4
				mov DWORD ptr [rsp],tkn
				fsola_txt TEXTEQU <DWORD ptr [rsp]>
			ELSEIF igpr_size EQ 2
				mov WORD ptr [rsp],tkn
				fsola_txt TEXTEQU <WORD ptr [rsp>
			ELSEIF igpr_size EQ 1
				mov BYTE ptr [rsp],tkn
				.if tkn & 80h
					mov BYTE ptr [rsp+1],0ffh
				.else
					mov BYTE ptr [rsp+1],0
				.endif
				fsola_txt TEXTEQU <WORD ptr [rsp]>
			ENDIF
		ELSE
			IF igpr_size EQ 4
				mov DWORD ptr [esp-4],tkn
				fsola_txt TEXTEQU <DWORD ptr [esp-4]>
			ELSEIF igpr_size EQ 2
				mov WORD ptr [esp-4],tkn
				fsola_txt TEXTEQU <WORD ptr [esp-4]>
			ELSEIF igpr_size EQ 1
				mov BYTE ptr [esp-4],tkn
				.if tkn & 80h
					mov BYTE ptr [esp-3],0ffh
				.else
					mov BYTE ptr [esp-3],0
				.endif
				fsola_txt TEXTEQU <WORD ptr [esp-4]>
			ENDIF
		ENDIF
	ENDIF

	IF @IsIntegerType(%fsola_txt)
		fild fsola_txt
	ELSE
		fld fsola_txt
	ENDIF
	IF neg_arg NE 0
		fchs
	ENDIF
endm

; type: procedure, return: through EXITM
; prepare a argument so that it can be used as memory operand. The memory operand is returned through
; EXITM <...>. The returned operand must not be a register.
; If it is unavoidable to immediately load the argument on top of the stack, the return value must be blank <>.
; If the token is already a read-to-use memory operand, it is simply passed through: EXITM <tkn>
FSLV_X86_FPU_PREPARE_ARG macro tkn,neg_arg:=<0>
	
	fsopa_txt TEXTEQU <tkn>
	IF @IsGPReg(<&tkn>)
		IF @WordSize EQ 8
			IF igpr_size EQ 8
				mov QWORD ptr [rsp],tkn
				fsopa_txt TEXTEQU <QWORD ptr [rsp]>
			ELSEIF igpr_size EQ 4
				mov DWORD ptr [rsp],tkn
				fsopa_txt TEXTEQU <DWORD ptr [rsp]>
			ELSEIF igpr_size EQ 2
				mov WORD ptr [rsp],tkn
				fsopa_txt TEXTEQU <WORD ptr [rsp]>
			ELSEIF igpr_size EQ 1
				mov BYTE ptr [rsp],tkn
				.if tkn & 80h
					mov BYTE ptr [rsp+1],0ffh
				.else
					mov BYTE ptr [rsp+1],0
				.endif
				fsopa_txt TEXTEQU <WORD ptr [rsp]>
			ENDIF
		ELSE
			IF igpr_size EQ 4
				mov DWORD ptr [esp-4],tkn
				fsopa_txt TEXTEQU <DWORD ptr [esp-4]>
			ELSEIF igpr_size EQ 2
				mov WORD ptr [esp-4],tkn
				fsopa_txt TEXTEQU <WORD ptr [esp-4]>
			ELSEIF igpr_size EQ 1
				mov BYTE ptr [esp-4],tkn
				.if tkn & 80h
					mov BYTE ptr [esp-3],0ffh
				.else
					mov BYTE ptr [esp-3],0
				.endif
				fsopa_txt TEXTEQU <WORD ptr [esp-4]>
			ENDIF
		ENDIF
	ENDIF
	IF neg_arg NE 0 OR @IsIntegerType(%fsopa_txt) EQ 8 OR @IsRealType(%fsopa_txt) EQ 10
		IF iit_type NE 0
			fild fsopa_txt
		ELSE
			fld fsopa_txt
		ENDIF
		IF neg_arg NE 0
			fchs
		ENDIF
		EXITM <>
	ELSE
		EXITM fsopa_txt
	ENDIF
endm

; type: procedure
; load memory operand or special contatn to top off stack.
; This macro must respons to name=<pi>. other names are 
FSLV_X86_FPU_LOAD_CONST macro neg_arg:=<0>,name
	IFIDNI <&name>,<one>
		fld1
	ELSEIFIDNI <&name>,<mone>
		fld1
		IFE neg_arg
			fchs
		ENDIF
		EXITM
	ELSEIFIDNI <&name>,<zero>
		fldz
	ELSEIFIDNI <&name>,<pi>
		fldpi
	ENDIF
	IF neg_arg NE 0
		fchs
	ENDIF
endm

; return: memory operand or blank if loaded
FSLV_X86_FPU_PREPARE_CONST macro neg_arg:=<0>,name
	IFIDNI <&name>,<one>
		fld1
	ELSEIFIDNI <&name>,<mone>
		fld1
		IFE neg_arg
			fchs
		ENDIF
		EXITM <>
	ELSEIFIDNI <&name>,<zero>
		fldz
	ELSEIFIDNI <&name>,<pi>
		fldpi
	ELSE
		fslv_glb_error_msg TEXTEQU <unkown constant: name>
		fslv_glb_error = 1
	ENDIF
	IF neg_arg NE 0
		fchs
	ENDIF
	EXITM <>
endm


; type: function
; tkn is a real or integer constant.
; If there is an specail operation/optimization for tkn as rvalue, the function creates the
; corresponding code and return a none-zero value.
;
; If the constant and/or operator should be replaced, set the equate fsberor_new_params to a none zero value.
; The new operator must placed in text macro fsberor_new_op; the constant in fsberor_new_const.
; fsberor_new_const_is_int indicates wether a ineger or real contant is passed through fsberor_new_const
; When replacing occurs, no code must be created and the macro must return zero.
FSLV_X86_FPU_REG0_OPRT_RCVAL macro oprt,tkn,neg_tkn
	IFIDN <&oprt>,<^>
		fxsfror_pos INSTR 1,<2.0 3.0-1.0-2.0-3.0 0.5-0.5>,<&tkn>
		IF fxsfror_pos
			IF @SizeStr(<&tkn>) GE @SubStr(<100010020002000200003004000>,%fxsfror_pos,1)
				fxsfror_powflg = @SubStr(<200030010004000500008009000>,%fxsfror_pos,1)
				IF fxsfror_powflg EQ 2 OR fxsfror_powflg EQ 4
					fmul st(0),st(0)
				ELSEIF fxsfror_powflg EQ 3 OR fxsfror_powflg EQ 5
					FSBE_STCK_CHK 1
					fld st
					fmul st,st
					fmulp st(1),st(0)
				ELSEIF fxsfror_powflg EQ 8 OR fxsfror_powflg EQ 9
					fsqrt
				ENDIF
				IF (fxsfror_powflg EQ 4 OR fxsfror_powflg EQ 5 OR fxsfror_powflg EQ 1 OR fxsfror_powflg EQ 9) XOR (neg_tkn NE 0)
					FSBE_STCK_CHK 1
					fld1
					fdivrp st(1),st(0)
				ENDIF
				EXITM <-1>
			ENDIF
		ENDIF
	ELSEIFIDN <&oprt>,</>
		IF @InStr(1,<1.0>,<&tkn>) EQ 1
			IF neg_tkn
				fchs
			ENDIF
			EXITM <-1>
		ELSEIF @InStr(1,<-1.0>,<&tkn>) EQ 1
			IFE neg_tkn
				fchs
			ENDIF
			EXITM <-1>
		ELSE
			fxsfror_pos INSTR <2.0 3.0 4.0 5.0 6.0 7.0 8.0 9.0 10.0 100.0 1000.0-2.0-3.0-4.0-5.0-6.0-7.0-8.0-9.0-10.0-100.0-1000.0>,<&tkn>
			IF fxsfror_pos
				IF @SizeStr(<&tkn>) GE @SubStr(<100010001000100010001000100010002000030000040000020002000200020002000200020002000300004000005000000>,%fxsfror_pos,1)
					IF @SubStr(<100010001000100010001000100010002000030000040000020002000200020002000200020002000300004000005000000>,%fxsfror_pos,1)
						IF neg_tkn NE 0 AND fxsfror_pos GE 50
							fsberor_pos = fxsfror_pos-49
						ELSEIF neg_tkn NE 0
							fsberor_pos = fxsfror_pos5+49
						ENDIF
						IF fxsfror_pos GE 50
							fsberor_pos2 = fxsfror_pos-49
						ELSE
							fxsfror_pos2 = fxsfror_pos
						ENDIF
						IF fxsfror_pos EQ 1
							fsberor_new_const TEXTEQU <0.5>
						ELSEIF fxsfror_pos EQ 5
							fsberor_new_const TEXTEQU <0.3333333333333333333>
						ELSEIF fxsfror_pos EQ 9
							fsberor_new_const TEXTEQU <0.25>
						ELSEIF fxsfror_pos EQ 13
							fsberor_new_const TEXTEQU <0.2>
						ELSEIF fxsfror_pos EQ 17
							fsberor_new_const TEXTEQU <0.1666666666666666666>
						ELSEIF fxsfror_pos EQ 21
							fsberor_new_const TEXTEQU <0.1428571428571428571>
						ELSEIF fxsfror_pos EQ 25
							fsberor_new_const TEXTEQU <0.125>
						ELSEIF fxsfror_pos EQ 29
							fsberor_new_const TEXTEQU <0.1111111111111111111>
						ELSEIF fxsfror_pos EQ 33
							fsberor_new_const TEXTEQU <0.1>
						ELSEIF fxsfror_pos EQ 38
							fsberor_new_const TEXTEQU <0.01>
						ELSEIF fxsfror_pos EQ 44
							fsberor_new_const TEXTEQU <0.001>
						ENDIF
						IF fxsfror_pos GE 50
							fsberor_new_const TEXTEQU <->,fsberor_new_const
						ENDIF
						fsberor_new_const_is_int = 0
						fsberor_new_op TEXTEQU <*>
						fsberor_new_params = 1
						EXITM <0>
					ENDIF
				ENDIF
			ENDIF
		ENDIF
	ENDIF
	EXITM <0>
endm

; type procedure
; perform the operation tkn={+-*/^} on ST(1) and ST(0), store the result in ST(1) and pop the stack.
; e.g.: ST(1) - ST(0)
FSLV_X86_FPU_REG1_OPRT_REG0_P macro tkn
	IFIDN <&tkn>,<+>
		faddp st(1),st(0)
	ELSEIFIDN <&tkn>,<->
		fsubp st(1),st(0)
	ELSEIFIDN <&tkn>,<*>
		fmulp st(1),st(0)
	ELSEIFIDN <&tkn>,</>
		fdivp st(1),st(0)
	ELSEIFIDN <&tkn>,<^>
		call fpu_st1_pow_st0
	ENDIF
endm

; type procedure
; perform the operation tkn={+-*/^} on ST(1) and ST(0), store the result in ST(1) and pop the stack.
; e.g.: ST(0) - ST(1)
FSLV_X86_FPU_REG0_OPRT_REG1_P macro tkn
	IFIDN <&tkn>,<+>
		faddp st(1),st(0)
	ELSEIFIDN <&tkn>,<->
		fsubrp st(1),st(0)
	ELSEIFIDN <&tkn>,<*>
		fmulp st(1),st(0)
	ELSEIFIDN <&tkn>,</>
		fdivrp st(1),st(0)
	ELSEIFIDN <&tkn>,<^>
		fxch
		call fpu_st1_pow_st0
	ENDIF
endm

; index >= 1
FSLV_X86_FPU_REG0_OPRT_REGX macro oprt,index
	IFIDN lfs_txt2,<+>
		fadd st(0),st(index)
	ELSEIFIDN lfs_txt2,<->
		fsub st(0),st(index)
	ELSEIFIDN lfs_txt2,<*>
		fmul st(0),st(index)
	ELSEIFIDN lfs_txt2,</>
		fdiv st(0),st(index)
	ELSEIFIDN lfs_txt2,<^>
		FSBE_STCK_CHK 1
		fld st(index)
		call fpu_st1_pow_st0
	ENDIF
endm

; Attention: index can be zero
FSLV_X86_FPU_REGX_OPRT_REG0 macro oprt,index
	IFIDN <&oprt>,<+>
		fadd st(0),st(index)
	ELSEIFIDN <&oprt>,<->
		fsubr st(0),st(index)
	ELSEIFIDN <&oprt>,<*>
		fmul st(0),st(index)
	ELSEIFIDN <&oprt>,</>
		fdivr st(0),st(index)
	ELSEIFIDN <&oprt>,<^>
		FSBE_STCK_CHK 1
		fld st(index)
		fxch
		call fpu_st1_pow_st0
	ENDIF
endm

; st(0) <+-*/^> real4/8/10/int32/64
FSLV_X86_FPU_REG0_OPRT_MEM macro tkn,oprt,int_type,real_type
	
	IF real_type EQ 10 OR int_type EQ 8
		IF int_type EQ 8
			fild tkn
		ELSE
			fld tkn
		ENDIF
		FSBE_STCK_CHK 1
		IFIDN <&oprt>,<+>
			faddp st(1),st(0)
		ELSEIFIDN <&oprt>,<->
			fsubp st(1),st(0)
		ELSEIFIDN <&oprt>,<*>
			fmulp st(1),st(0)
		ELSEIFIDN <&oprt>,</>
			fdivp st(1),st(0)
		ELSEIFIDN <&oprt>,<^>
			call fpu_st1_pow_st0
		ENDIF
		EXITM
	ELSE
		IFIDN <&oprt>,<+>
			@CatStr(<f>,@SubStr(<i>,1,%(int_type NE 0) AND 1),<add >,<&tkn>)
		ELSEIFIDN <&oprt>,<->
			@CatStr(<f>,@SubStr(<i>,1,%(int_type NE 0) AND 1),<sub >,<&tkn>)
		ELSEIFIDN <&oprt>,<*>
			@CatStr(<f>,@SubStr(<i>,1,%(int_type NE 0) AND 1),<mul >,<&tkn>)
		ELSEIFIDN <&oprt>,</>
			@CatStr(<f>,@SubStr(<i>,1,%(int_type NE 0) AND 1),<div >,<&tkn>)
		ELSEIFIDN <&oprt>,<^>
			FSBE_STCK_CHK 1
			@CatStr(<f>,@SubStr(<i>,1,%(int_type NE 0) AND 1),<ld >,<&tkn>)
			call fpu_st1_pow_st0
		ENDIF
		EXITM
	ENDIF
endm


; real4/8/10/int32/64 <+-*/^> st(0)
FSLV_X86_FPU_MEM_OPRT_REG0 macro tkn,oprt,int_type,real_type
	
	IF real_type EQ 10 OR int_type EQ 8
		IF int_type EQ 8
			fild tkn
		ELSE
			fld tkn
		ENDIF
		FSBE_STCK_CHK 1
		IFIDN <&oprt>,<+>
			faddp st(1),st(0)
		ELSEIFIDN <&oprt>,<->
			fsubrp st(1),st(0)
		ELSEIFIDN <&oprt>,<*>
			fmulp st(1),st(0)
		ELSEIFIDN <&oprt>,</>
			fdivrp st(1),st(0)
		ELSEIFIDN <&oprt>,<^>
			fxch
			call fpu_st1_pow_st0
		ENDIF
		EXITM
	ELSE
		IFIDN <&oprt>,<+>
			@CatStr(<f>,@SubStr(<i>,1,%(int_type NE 0) AND 1),<add >,<&tkn>)
		ELSEIFIDN <&oprt>,<->
			@CatStr(<f>,@SubStr(<i>,1,%(int_type NE 0) AND 1),<subr >,<&tkn>)
		ELSEIFIDN <&oprt>,<*>
			@CatStr(<f>,@SubStr(<i>,1,%(int_type NE 0) AND 1),<mul >,<&tkn>)
		ELSEIFIDN <&oprt>,</>
			@CatStr(<f>,@SubStr(<i>,1,%(int_type NE 0) AND 1),<divr >,<&tkn>)
		ELSEIFIDN <&oprt>,<^>
			FSBE_STCK_CHK 1
			@CatStr(<f>,@SubStr(<i>,1,%(int_type NE 0) AND 1),<ld >,<&tkn>)
			fxch
			call fpu_st1_pow_st0
		ENDIF
		EXITM
	ENDIF
endm

FSLV_X86_FPU_FREE_REG macro index
	ffree st(index)	
endm


; This macro saves the result of an calculated expression.
; Parameter:
;   dest:		
;   	Destination operand that has been specified in front of the equal sign: e.g.:  y = x^2 ==> y
;       This parameter can be a register or a memory expressions. If this parameter is blank and bRetValue is zero, the result must
;       stay on the stack.
;
;   dest_int_type,
;   dest_real_type:
;		the return values of @IsReal/IntegerType(dest)
;       If fCopy&4 NE 0: both values are zero.
;   
;	bRetValue:
;		If this value is not zero, this macro is called in context of a function-like macro: @fSlv4/8/I()
;		In this case, the macro must store the result in a variable, which can be obtained by FSBE_ALLOC_RETVAL().
;		The obtained variable is of the type described through the paramters retval_int/real_type. After storing,
;       the stack must be poped.
;				
;	retval_int_type,
;	retval_real_type:
;		This parameters are valid, if bRetValue is not zero. They describe the return type of the calling macro (@fSlv4/8/I())
;
;   fCopy: This parameter is only used, if the Entity returns a non-zero value for FSEN_COPY_SUPPORTED.
;          If (fCopy&1) , the expression to solve contains only one memory or constant operand.
;              In that case, the value is directly pass to the macro (->CpySrc = mem. operand) and not load on the stack. The macro must
;              determine the type of CpySrc itself.
;          If (fCopy&2), CpySrc is a register index.
;          If (fCopy&4), dest is a register index. The paramters dest_int/real_type are zero.
;          If fCopy is zero, no copy operation is requested. The bits 1 and 2 are only used, if FSBE_IS_TKN_REG() identifies registers.
;		   The pupose of this feature is to avoid unnecessary move/copy operations.
;
;   CpySrc: see previous parameter.
;
;	Return:
;		If bRetValue is zero, the return value is discarded. If not zero, a variable typed according to retval_int_type/retval_real_type
;		must be retruned. This can be the destination dest or a variable that was created through FSBE_ALLOC_RETVAL().
;  
;	Remarks:
;		It is possible that this macro must do two store operations: one for <dest> and one for the varaible that must be recived through
;       calling FSBE_ALLOC_RETVAL():
;			lbl TEXTEQU FSBE_ALLOC_RETVAL()
;		The retuned variable must be filled and the stack must be poped.
;		However, if the type of dest match the type descibed through retval_int/real_type and it is a memory operand or GPR, it is sufficent to only store dest and
;		then return with: EXITM <&dest> (do not forget to pop the stack in this case)
;	
;
FSLV_X86_FPU_ASSIGN_DEST macro dest,dest_int_type,dest_real_type,bRetValue,retval_int_type,retval_real_type,fCopy,CpySrc
LOCAL retval

	IFNB <dest>
		IF @IsGPReg(<&dest>,<edx::eax>)
			IF igpr_size EQ 8
				IFE bRetValue
					fistp QWORD ptr [rsp]
				ELSE
					fist QWORD ptr [rsp]
				ENDIF
				mov dest,QWORD ptr [rsp]
				IF bRetValue NE 0 AND retval_int_type EQ 8
					EXITM <dest>
				ENDIF
			ELSEIF igpr_size EQ 4
				IFE bRetValue
					fistp DWORD ptr T_EXPR([esp-4],[rsp])
				ELSE
					fist DWORD ptr T_EXPR([esp-4],[rsp])
				ENDIF
				mov dest,DWORD ptr T_EXPR([esp-4],[rsp])
				IF bRetValue NE 0 AND retval_int_type EQ 4
					EXITM <dest>
				ENDIF
			ELSEIF igpr_size EQ 2
				IFE bRetValue
					fistp WORD ptr T_EXPR([esp-4],[rsp])
				ELSE
					fist WORD ptr T_EXPR([esp-4],[rsp])
				ENDIF
				mov dest,WORD ptr T_EXPR([esp-4],[rsp])
			ELSEIF igpr_size EQ 1
				IFE bRetValue
					fistp WORD ptr T_EXPR([esp-4],[rsp])
				ELSE
					fist WORD ptr T_EXPR([esp-4],[rsp])
				ENDIF
				mov dest,BYTE ptr T_EXPR([esp-4],[rsp])
			ELSEIF igpr_size EQ -1
				IF bRetValue
					fld st
				ENDIF
				fistp QWORD ptr T_EXPR([esp-8],[rsp])
				mov eax,DWORD ptr T_EXPR([esp-8],[rsp])
				mov edx,DWORD ptr T_EXPR([esp-4],[rsp+4])
			ENDIF
		ELSE
			IF bRetValue EQ 0 AND @MatchStrI(<&dest>,1,st0,st1,st2,st3,st4,st5,st6) NE 0
				fstp st(msi_index)
				EXITM <>
			ELSE
				IF dest_int_type NE 0
					IFE bRetValue
						fistp dest
					ELSE
						IF dest_int_type EQ 8
							fld st
							fistp dest
						ELSE
							fist dest
						ENDIF
						IF dest_int_type EQ retval_int_type
							EXITM <dest>
						ENDIF
					ENDIF
				ELSE
					IFE bRetValue
						fstp dest
					ELSE
						IF dest_real_type EQ 10
							fld st
							fstp dest
						ELSE
							fst dest
						ENDIF
						IF dest_real_type EQ retval_real_type
							EXITM <dest>
						ENDIF
					ENDIF
				ENDIF
			ENDIF
		ENDIF
	ENDIF
	IFE bRetValue
		EXITM <>
	ELSE
		retval TEXTEQU FSBE_ALLOC_RETVAL()
		IF retval_int_type NE 0
			fistp retval
		ELSE
			fstp retval
		ENDIF
		EXITM <retval>
	ENDIF
endm

FSLV_X86_FPU_CMP macro cmpType,cmpSuffix,cParam,bool_byte,MemParam
	IFIDNI <&cmpType>,<cmp>
		fcomip st,st(1)
		fstp st
		IFNB <&cmpSuffix>
			set&cmpSuffix& bool_byte
		ENDIF
	ELSEIFIDN <&cmpType>,<zero test>
		IFE fslv_volatile_gprs AND FSVGPR_EAX
			T_EXPR(<push eax>)
		ENDIF
		IF @WordSize EQ 8 AND ((fslv_volatile_gprs AND FSVGPR_EAX ) EQ 0 OR (fslv_volatile_gprs AND FSVGPR_EDX ) EQ 0)
			mov [rsp+2*8],rax
			mov [rsp+3*8],rdx
			sub rsp,2*8
		ENDIF
		invoke fpu_zero_test,cParam,0
		fstp st
		fstp st
		test eax,eax
		setnz fcmp_bool_byte
		IFE fslv_volatile_gprs AND FSVGPR_EAX
			T_EXPR(<pop eax>)
		ENDIF
		IF @WordSize EQ 8 AND ((fslv_volatile_gprs AND FSVGPR_EAX ) EQ 0 OR (fslv_volatile_gprs AND FSVGPR_EDX ) EQ 0)
			add rsp,2*8
			mov rax,[rsp+2*8]
			mov rdx,[rsp+3*8]
		ENDIF
	ELSEIFIDNI <&cmpType>,<approx. eq.>
		fld st
		fmul cParam
		fabs
		fxch st(2)
		fsubp st(1),st
		fabs
		fcomip st,st(1)
		fstp st
		setbe fcmp_bool_byte
	ENDIF
endm


