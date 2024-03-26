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

FSBE_REGISTER_BACKEND <FSLV_X86_SSE2_FPU_ENTITY> 

FSLV_X86_SSE2_FPU_ENTITY macro code:req,param1,param2
	IF code EQ FSEN_INIT
		IF fslv_lcl_real_type NE 4 AND fslv_lcl_real_type NE 8
			fslv_glb_error_msg TEXTEQU <[#FS1] SSE2 does only support REAL4 and REAL8 - change the back end or the global/local precision by the corresponding macros or attributes>
			fslv_glb_error = 1
			EXITM <0>
		ENDIF
		IF fslv_lcl_int_type EQ 8
			fslv_glb_error_msg TEXTEQU <[#FS2] QWORD constants maybe not excat representable as REAL4/8 - change the local or global integer size by the corresponding macros or attributes>
			fslv_glb_error = 1
			EXITM <0>
		ENDIF
		IF @Version LT 700
			fslv_glb_error_msg TEXTEQU <[#FS3] This back end requires MASM version 7+ or JWASM (lates version): SSE2 is not usable with this assembler>
			fslv_glb_error = 1
			EXITM <0>
		ENDIF
		IF param2
			fsxsf_compare = -1
		ELSE
			fsxsf_compare = 0
		ENDIF
		EXITM <-1>
	ELSEIF code EQ FSEN_END
		EXITM <0>
	ELSEIF code EQ FSEN_MAX_STACK
		EXITM @GetArgCount(%fxsfet_regs)
	ELSEIF code EQ FSEN_STCK_USAGE
		EXITM <0>
	ELSEIF code EQ FSEN_COPY_SUPPORTED
		EXITM <-1>
	ELSEIF code EQ FSEN_TEST_ATTR
		IFIDNI <stck>,<&param1>
			EXITM <-1>
		ELSEIFIDNI <Volatile>,<&param1>
			EXITM <-1>
		ELSEIFIDNI <last>,<&param1>
			EXITM <-1>
		ELSE
			EXITM <0>
		ENDIF
	ELSEIF code EQ FSEN_MAPPING
		FSBE_ENTITY				TEXTEQU <FSLV_X86_SSE2_FPU_ENTITY>
		FSBE_ENUM_TKN			TEXTEQU <FSLV_X86_SSE2_FPU_ENUM_TKN>         
		FSBE_IS_TKN_REG       	TEXTEQU <FSLV_X86_SSE2_FPU_IS_TKN_REG>
		FSBE_IS_TKN_CONST		TEXTEQU <FSLV_X86_SSE2_FPU_IS_TKN_CONST>     
		FSBE_LOAD_CONST       	TEXTEQU <FSLV_X86_SSE2_FPU_LOAD_CONST>
		FSBE_PREPARE_CONST		TEXTEQU <FSLV_X86_SSE2_FPU_PREPARE_CONST>   
		FSBE_LOAD_ARG         	TEXTEQU <FSLV_X86_SSE2_FPU_LOAD_ARG>
		FSBE_PREPARE_ARG      	TEXTEQU <FSLV_X86_SSE2_FPU_PREPARE_ARG>
		FSBE_LOAD_REG         	TEXTEQU <FSLV_X86_SSE2_FPU_LOAD_REG>
		FSBE_REG0_OPRT_MEM    	TEXTEQU <FSLV_X86_SSE2_FPU_REG0_OPRT_MEM>      
		FSBE_MEM_OPRT_REG0    	TEXTEQU <FSLV_X86_SSE2_FPU_MEM_OPRT_REG0>		
		FSBE_REG0_OPRT_REG1_P 	TEXTEQU <FSLV_X86_SSE2_FPU_REG0_OPRT_REG1_P>
		FSBE_REG0_OPRT_REGX   	TEXTEQU <FSLV_X86_SSE2_FPU_REG0_OPRT_REGX>  
		FSBE_REG1_OPRT_REG0_P 	TEXTEQU <FSLV_X86_SSE2_FPU_REG1_OPRT_REG0_P>
		FSBE_REGX_OPRT_REG0  	TEXTEQU <FSLV_X86_SSE2_FPU_REGX_OPRT_REG0>
		FSBE_FREE_REG         	TEXTEQU <FSLV_X86_SSE2_FPU_FREE_REG>
		FSBE_SET_REG          	TEXTEQU <FSLV_X86_SSE2_FPU_SET_REG>
		FSBE_ASSIGN_DEST		TEXTEQU <FSLV_X86_SSE2_FPU_ASSIGN_DEST>             
		FSBE_REG0_OPRT_RCVAL	TEXTEQU <FSLV_X86_SSE2_FPU_REG0_OPRT_RCVAL>
		FSBE_NEG              	TEXTEQU <FSLV_X86_SSE2_FPU_NEG>       
		FSBE_XCHG             	TEXTEQU <FSLV_X86_SSE2_FPU_XCHG>
		FSBE_FNC_DSCPTR			TEXTEQU <FSLV_X86_SSE2_FPU_FNC_DSCPTR>
		FSBE_CALL_FNC			TEXTEQU <FSLV_X86_SSE2_FPU_CALL_FNC>
		FSBE_CMP				TEXTEQU <FSLV_X86_SSE2_FPU_CMP>
		fslv_glb_instrset = FSIS_FPU or FSIS_SSE2
		EXITM <-1>
	ELSEIF code EQ FSEN_CMP_SUPPORTED
		EXITM <2>
	ELSEIF code EQ FSEN_INSTRSET
		IFIDNI <x86>,<&param1>
			EXITM <-1>
		ELSEIFIDNI <x64>,<&param1>
			EXITM <-1>
		ELSEIFIDNI <FPU>,<&param1>
			EXITM <1>
		ELSEIFIDNI <SSE2>,<&param1>
			EXITM <-1>
		ELSE
			EXITM <0>
		ENDIF
	ELSEIF code EQ FSEN_ALL_INSTRSETS
		EXITM <x86,x64,FPU,SSE2>
	ELSEIF code EQ FSEN_REQ_RETVAL
		EXITM <1>
	ELSEIF code EQ FSEN_TEST_TYPE
		IF @WordSize EQ 4
			EXITM @GetArgByIndex(@MatchStrI(param1,0,BOOLEAN_BYTE,SWORD,SDWORD,REAL4,REAL8),0,0,1,2,4,4,8)
		ELSE
			EXITM @GetArgByIndex(@MatchStrI(param1,0,BOOLEAN_BYTE,SWORD,SDWORD,SQWORD,REAL4,REAL8),0,0,1,2,4,8,4,8)
		ENDIF
	ELSEIF code EQ FSEN_NO_INT_CONST
		EXITM <-1>
	ELSEIF code EQ FSEN_IMM_FREE
		EXITM <-1>
	ELSEIF code EQ FSEN_NAME
		fsbe_txt TEXTEQU <x86-32/64,SSE2,FPU>
		EXITM <-1>
	ELSE
		EXITM <0>
	ENDIF
endm

; type: procedure
; allows to scan all tokens before evaluation begins. If the back end is called in context to a compare operation,
; the raw input tokens are passed with the flag TMT_TXT.
; The impl. below use these feature to detect XMM register and remove them from the
; virtual register stack.
FSLV_X86_SSE2_FPU_ENUM_TKN macro flg,tkn:=<>,tkn_flg:=<0>
	IF flg EQ FSB_ENUM_TKN
		IF tkn_flg AND TMT_TXT
			IF @IsTypedXmmReg(<&tkn>,REAL4,REAL8)
				IF istxr_type EQ -1
					fslv_glb_error_msg TEXTEQU <[#FS13] invalid typecasted XMM register found: tkn>
					fslv_glb_error = 1
					EXITM
				ENDIF
				IF @Version GE 800
					fxsfet_regs TEXTEQU @RemoveArgI(@CatStr(<xmm>,%isxr_index-1),%fxsfet_regs)
				ELSE
					fxsfet_regs TEXTEQU @RemoveArgI(@CatStr(<XMM!(>,%isxr_index-1,<!)>),%fxsfet_regs)
				ENDIF
			ENDIF
		ENDIF
		EXITM
	ELSEIF flg EQ FSB_ENUM_START
		fxsfet_regs TEXTEQU fslv_volatile_xmm_regs
	ELSEIF flg EQ FSB_ENUM_END
		IF @GetArgCount(%fxsfet_regs) EQ 0
			fslv_glb_error_msg TEXTEQU <[#FS4] no free XMM registers: reduce the number of XMM registers that are used as operands>
			fslv_glb_error = 1
		ELSE
			stcksim_create XMMSTK,%fxsfet_regs
		ENDIF
	ENDIF
endm

FSLV_X86_SSE2_FPU_FNC_DSCPTR macro fnc_name,flags,bTest:=<0>
	IFE bTest
		IF flags AND FNCD_NREGS
			IF (fslv_fnc_&fnc_name&_dscptr(FNCD_INSTR_SET) AND FSIS_SSE2) NE 0
				EXITM fslv_fnc_&fnc_name&_dscptr(flags,%FSIS_SSE2)
			ELSE
				EXITM fslv_fnc_&fnc_name&_dscptr(flags,%FSIS_FPU)
			ENDIF
		ELSE
			EXITM fslv_fnc_&fnc_name&_dscptr(flags)
		ENDIF
	ELSE
		IFDEF fslv_fnc_&fnc_name&_dscptr
			IF fslv_fnc_&fnc_name&_dscptr(FNCD_IREG) NE 0 AND (fslv_fnc_&fnc_name&_dscptr(FNCD_INSTR_SET) AND FSIS_SSE2) NE 0
				EXITM <-1>
			ENDIF
			IF fslv_fnc_&fnc_name&_dscptr(FNCD_ISTCK) NE 0 AND (fslv_fnc_&fnc_name&_dscptr(FNCD_INSTR_SET) AND FSIS_FPU) NE 0
				EXITM <-1>
			ENDIF
			EXITM <0>
		ELSE
			EXITM <0>
		ENDIF
	ENDIF
endm

FSLV_X86_SSE2_FPU_CALL_FNC macro fnc_name
	fsxsfcf_nargs = fslv_fnc_&fnc_name&_dscptr(%FNCD_NARGS)
	IF (fslv_fnc_&fnc_name&_dscptr(%FNCD_INSTR_SET) AND FSIS_SSE2) NE 0
		IF fsxsfcf_nargs
			fsxsfcf_args TEXTEQU stcksim_get_range(XMMSTK,%fsxsfcf_nargs-1,0)
			fsxsfcf_free TEXTEQU stcksim_get_all_free(XMMSTK)
			IF FSBE_FNC(%FSBE_FNC_DSCPTR,fnc_name,%FNCD_IREG_RECORD_SIG)
				fsxsfcf_nregs = fslv_fnc_&fnc_name&_dscptr(%FNCD_NREGS,%FSIS_SSE2)
				FSBE_IRegCallBySignature BEFpuSee2,fnc_name,%fslv_lcl_real_type,0,%FSIS_SSE2,%fsxsfcf_nregs,<%fsxsfcf_args>,<%fsxsfcf_free>
			ELSE
			%	fslv_fnc_ireg_&fnc_name& &fsxsfcf_args&,&fsxsfcf_free&
			ENDIF
		ELSE
			fsxsfcf_free TEXTEQU stcksim_get_all_free(XMMSTK)
		%	fslv_fnc_ireg_&fnc_name& &fsxsfcf_free&
		ENDIF
	%	REPEAT fsxsfcf_nargs-1
			stcksim_pop XMMSTK
		ENDM
	ELSE
		fsxsfcf_cntr = fsxsfcf_nargs
		WHILE fsxsfcf_cntr-1 GE 0
			IF fslv_lcl_real_type EQ 4
				movss REAL4 ptr T_EXPR(<[esp-4]>,<[rsp]>),stcksim_get(XMMSTK,%fsxsfcf_cntr-1,<name>)
				fld REAL4 ptr T_EXPR(<[esp-4]>,<[rsp]>)
			ELSE
				movsd REAL8 ptr T_EXPR(<[esp-8]>,<[rsp]>),stcksim_get(XMMSTK,%fsxsfcf_cntr-1,<name>)
				fld REAL8 ptr T_EXPR(<[esp-8]>,<[rsp]>)
			ENDIF
			fsxsfcf_cntr = fsxsfcf_cntr - 1
		ENDM
		fslv_fnc_&fnc_name&
	%	REPEAT fsxsfcf_nargs-1
			stcksim_pop XMMSTK
		ENDM
		IF fslv_lcl_real_type EQ 4
			fstp REAL4 ptr T_EXPR(<[esp-4]>,<[rsp]>)
			movss stcksim_get(XMMSTK,0,<name>),REAL4 ptr T_EXPR(<[esp-4]>,<[rsp]>)
		ELSE
			fstp REAL8 ptr T_EXPR(<[esp-8]>,<[rsp]>)
			movsd stcksim_get(XMMSTK,0,<name>),REAL8 ptr T_EXPR(<[esp-8]>,<[rsp]>)
		ENDIF
	ENDIF
endm


; type: function, return: through EXITM and text macros
;
; check if tkn is a register (releated to calculations)
; tkn is trimmed.
; return  = 0 => no register
;        != 0 => register , fsbeitr_index = TEXTEQU = register index      
FSLV_X86_SSE2_FPU_IS_TKN_REG macro tkn:=<>
	EXITM <0>
endm

; return: true/false , fsbeic_const_name TEXTEQU <const name> , fsbeic_mem_arg = true/false , fsbeic_neg_mem_arg = true/false
FSLV_X86_SSE2_FPU_IS_TKN_CONST macro tkn:=<>
	IFIDNI <&tkn>,<pi>
		fsbeic_const_name TEXTEQU <pi>
		fsbeic_mem_arg = -1
		fsbeic_neg_mem_arg = -1
		EXITM <-1>
	ENDIF
	EXITM <0>
endm

FSLV_X86_SSE2_FPU_SET_REG macro index,bPop:=<0>
	IF fslv_lcl_real_type EQ 4
		IF index NE 0
			movss stcksim_get(XMMSTK,index,<name>),stcksim_get(XMMSTK,0,<name>)
		ENDIF
	ELSE
		IF index NE 0
			movsd stcksim_get(XMMSTK,index,<name>),stcksim_get(XMMSTK,0,<name>)
		ENDIF
	ENDIF
	stcksim_set XMMSTK,index,<tag_inuse>,<1>
	IF bPop NE 0
		stcksim_pop XMMSTK
	ENDIF
endm

; negate top of stack
FSLV_X86_SSE2_FPU_NEG macro 
	IF fslv_lcl_real_type EQ 4
		xorps stcksim_get(XMMSTK,0,<name>),sse2_ss_neg_msk
	ELSE
		xorpd stcksim_get(XMMSTK,0,<name>),sse2_sd_neg_msk
	ENDIF
endm

FSLV_X86_SSE2_FPU_XCHG macro index:=<1>
	stcksim_xchg XMMSTK,0,index,<name>
endm

; type: procedure
FSLV_X86_SSE2_FPU_LOAD_REG macro index,neg_arg:=<0>
	fsxsflr_tmp TEXTEQU stcksim_get(XMMSTK,index,<name>)
	stcksim_push XMMSTK
	IF fslv_lcl_real_type EQ 4
		movss stcksim_get(XMMSTK,0,<name>),fsxsflr_tmp
	ELSE
		movsd stcksim_get(XMMSTK,0,<name>),fsxsflr_tmp
	ENDIF
	IF neg_arg
		FSLV_X86_SSE2_FPU_NEG
	ENDIF
endm

; type: procedure
; load a argument on top of the stack.
; If neg_arg is not zero, then the sign of the loaded arg must be negated.
;
FSLV_X86_SSE2_FPU_LOAD_ARG macro tkn,neg_arg:=<0>

	stcksim_push XMMSTK
	IF @IsGPReg(<&tkn>)
		IF igpr_size EQ 8 OR igpr_size EQ 4
			IF fslv_lcl_real_type EQ 4
				cvtsi2ss stcksim_get(XMMSTK,0,<name>),tkn
			ELSE
				cvtsi2sd stcksim_get(XMMSTK,0,<name>),tkn
			ENDIF
		ELSE
			IFE fslv_volatile_gprs AND FSVGPR_EAX
				mov T_EXPR(<DWORD ptr [esp-4]>,<QWORD ptr [rsp]>),T_REG(eax)
			ENDIF
			movsx eax,tkn
			IF fslv_lcl_real_type EQ 4
				cvtsi2ss stcksim_get(XMMSTK,0,<name>),eax
			ELSE
				cvtsi2sd stcksim_get(XMMSTK,0,<name>),eax
			ENDIF
			IFE fslv_volatile_gprs AND FSVGPR_EAX
				mov T_REG(eax),T_EXPR(<DWORD ptr [esp-4]>,<QWORD ptr [rsp]>)
			ENDIF
		ENDIF
	ELSE
		IF @IsTypedXmmReg(<&tkn>,REAL4,REAL8)
			istxr_type = istxr_type*4
			IF (istxr_type EQ 0) OR (istxr_type EQ fslv_lcl_real_type)
				IFDIFI istxr_xmm_reg,stcksim_get(XMMSTK,0,<name>)
					IF fslv_lcl_real_type EQ 4
						movss stcksim_get(XMMSTK,0,<name>),istxr_xmm_reg
					ELSE
						movsd stcksim_get(XMMSTK,0,<name>),istxr_xmm_reg
					ENDIF
				ENDIF
			ELSE
				; if dest == src --> in place conversion
				IF fslv_lcl_real_type EQ 4
					cvtsd2ss stcksim_get(XMMSTK,0,<name>),istxr_xmm_reg
				ELSE
					cvtss2sd stcksim_get(XMMSTK,0,<name>),istxr_xmm_reg
				ENDIF
			ENDIF
		ELSE
			fsola_txt TEXTEQU <&tkn>
			IF @IsIntegerType(%fsola_txt)
				IF iit_type GT 4
					fslv_glb_error_msg TEXTEQU <[#FS6] only SBYTE, SWORD and SDWORD as integer variable supported: >,%fsola_txt
					fslv_glb_error = 1
					EXITM
				ENDIF
				IF iit_type EQ 1 OR iit_type EQ 2
					IFE fslv_volatile_gprs AND FSVGPR_EAX
						mov T_EXPR(<DWORD ptr [esp-4]>,<QWORD ptr [rsp]>),T_REG(eax)
					ENDIF
					movsx eax,fsola_txt
					fsola_txt TEXTEQU <eax>
				ENDIF
				IF fslv_lcl_real_type EQ 4
					cvtsi2ss stcksim_get(XMMSTK,0,<name>),fsola_txt
				ELSE
					cvtsi2sd stcksim_get(XMMSTK,0,<name>),fsola_txt
				ENDIF
				IFE fslv_volatile_gprs AND FSVGPR_EAX
					IF iit_type EQ 1 OR iit_type EQ 2
						mov T_REG(eax),T_EXPR(<DWORD ptr [esp-4]>,<QWORD ptr [rsp]>)
					ENDIF
				ENDIF
			ELSE
				IF @IsRealType(%fsola_txt) EQ 4
					IF fslv_lcl_real_type EQ 4
						movss stcksim_get(XMMSTK,0,<name>),fsola_txt
					ELSE
						cvtss2sd stcksim_get(XMMSTK,0,<name>),fsola_txt
					ENDIF
				ELSEIF irt_type EQ 8
					IF fslv_lcl_real_type EQ 4
						cvtsd2ss stcksim_get(XMMSTK,0,<name>),fsola_txt
					ELSE
						movsd stcksim_get(XMMSTK,0,<name>),fsola_txt
					ENDIF
				ELSE
					fslv_glb_error_msg TEXTEQU <[#FS7] type not supported: &tkn>
					fslv_glb_error = 1
				ENDIF
			ENDIF
		ENDIF
	ENDIF
	IF neg_arg NE 0
		FSLV_X86_SSE2_FPU_NEG
	ENDIF
endm

; type: procedure, return: through EXITM
; prepare a argument so that it can be used as memory operand. The memory operand is returned through
; EXITM <...>. The returned operand must not be a register.
; If it is unavoidable to immediately load the argument on top of the stack, the return value must be blank <>.
; If the token is already a read-to-use memory operand, it is simply passed through: EXITM <tkn>
FSLV_X86_SSE2_FPU_PREPARE_ARG macro tkn,neg_arg:=<0>
	fsopa_reg = @IsGPReg(<&tkn>)
	IF fsopa_reg NE 0 OR neg_arg NE 0
		FSLV_X86_SSE2_FPU_LOAD_ARG <&tkn>,neg_arg
		EXITM <>
	ELSE
		EXITM <&tkn>
	ENDIF
endm

; type: procedure
; load special constant on top of stack.
FSLV_X86_SSE2_FPU_LOAD_CONST macro neg_arg:=<0>,name
	IFIDNI <name>,<pi>
		IF fslv_lcl_real_type EQ 4
			IFE neg_arg
				FSLV_X86_SSE2_FPU_LOAD_ARG sse2_ss_pi,0
			ELSE
				FSLV_X86_SSE2_FPU_LOAD_ARG sse2_ss_mpi,0
			ENDIF
		ELSE
			IFE neg_arg
				FSLV_X86_SSE2_FPU_LOAD_ARG sse2_sd_pi,0
			ELSE
				FSLV_X86_SSE2_FPU_LOAD_ARG sse2_sd_mpi,0
			ENDIF
		ENDIF
	ENDIF
endm

; return: memory operand or blank if loaded
FSLV_X86_SSE2_FPU_PREPARE_CONST macro neg_arg:=<0>,name
	IFIDNI <name>,<pi>
		IF fslv_lcl_real_type EQ 4
			IFE neg_arg
				EXITM <sse2_ss_pi>
			ENDIF
			EXITM <sse2_ss_mpi>
		ELSE
			IFE neg_arg
				EXITM <sse2_sd_pi>
			ENDIF
			EXITM <sse2_sd_mpi>
		ENDIF
	ELSE
		; error: unknown constant
		EXITM <>
	ENDIF
endm


; type: function
; tkn is a real or integer constant.
; If there is an special operation/optimization for tkn as rvalue, the function creates the
; corresponding code and return a none-zero value.
; If the constant and/or operator should be replaced, set the equate fsberor_new_params to a none zero value.
; The new operator must placed in text macro fsberor_new_op; the constant in fsberor_new_const. In this case, the macro must return zero.
FSLV_X86_SSE2_FPU_REG0_OPRT_RCVAL macro oprt,tkn,neg_tkn
	IFIDN <&oprt>,<^>
		fxsfror_pos INSTR 1,<2.0 3.0-1.0-2.0-3.0 0.5-0.5>,<&tkn>
		IF fxsfror_pos
			IF @SizeStr(<&tkn>) GE @SubStr(<100010020002000200003004000>,%fxsfror_pos,1)
				fxsfror_powflg = @SubStr(<200030010004000500008009000>,%fxsfror_pos,1)
				IF fxsfror_powflg EQ 2 OR fxsfror_powflg EQ 4
					IF fslv_lcl_real_type EQ 4
						mulss stcksim_get(XMMSTK,0,<name>),stcksim_get(XMMSTK,0,<name>)
					ELSE
						mulsd stcksim_get(XMMSTK,0,<name>),stcksim_get(XMMSTK,0,<name>)
					ENDIF
				ELSEIF fxsfror_powflg EQ 3 OR fxsfror_powflg EQ 5
					FSLV_X86_SSE2_FPU_LOAD_REG 0,0
					IF fslv_lcl_real_type EQ 4
						mulss stcksim_get(XMMSTK,0,<name>),stcksim_get(XMMSTK,0,<name>)
					ELSE
						mulsd stcksim_get(XMMSTK,0,<name>),stcksim_get(XMMSTK,0,<name>)
					ENDIF
					IF fslv_lcl_real_type EQ 4
						mulss stcksim_get(XMMSTK,0,<name>),stcksim_get(XMMSTK,1,<name>)
					ELSE
						mulsd stcksim_get(XMMSTK,0,<name>),stcksim_get(XMMSTK,1,<name>)
					ENDIF
					stcksim_pop XMMSTK
				ELSEIF fxsfror_powflg EQ 8 OR fxsfror_powflg EQ 9
					IF fslv_lcl_real_type EQ 4
						sqrtss stcksim_get(XMMSTK,0,<name>),stcksim_get(XMMSTK,0,<name>)
					ELSE
						sqrtsd stcksim_get(XMMSTK,0,<name>),stcksim_get(XMMSTK,0,<name>)
					ENDIF
				ENDIF
				IF (fxsfror_powflg EQ 4 OR fxsfror_powflg EQ 5 OR fxsfror_powflg EQ 1 OR fxsfror_powflg EQ 9) XOR (neg_tkn NE 0)
					stcksim_push XMMSTK
					IF fslv_lcl_real_type EQ 4
						movss stcksim_get(XMMSTK,0,<name>),sse2_ss_one
						divss stcksim_get(XMMSTK,0,<name>),stcksim_get(XMMSTK,1,<name>)
					ELSE
						movsd stcksim_get(XMMSTK,0,<name>),sse2_sd_one
						divsd stcksim_get(XMMSTK,0,<name>),stcksim_get(XMMSTK,1,<name>)
					ENDIF
					stcksim_xchg XMMSTK,0,1,<name>
					stcksim_pop XMMSTK
				ENDIF
				EXITM <-1>
			ENDIF
		ENDIF
	ELSEIFIDN <&oprt>,</>
		IF @InStr(1,<1.0>,<&tkn>) EQ 1
			IF neg_tkn
				FSLV_X86_SSE2_FPU_NEG
			ENDIF
			EXITM <-1>
		ELSEIF @InStr(1,<-1.0>,<&tkn>) EQ 1
			IFE neg_tkn
				FSLV_X86_SSE2_FPU_NEG
			ENDIF
			EXITM <-1>
		ELSE
			fxsfror_pos INSTR 1,<2.0 3.0 4.0 5.0 6.0 7.0 8.0 9.0 10.0 100.0 1000.0-2.0-3.0-4.0-5.0-6.0-7.0-8.0-9.0-10.0-100.0-1000.0>,<&tkn>
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
FSLV_X86_SSE2_FPU_REG1_OPRT_REG0_P macro tkn
	stcksim_xchg XMMSTK,0,1,<name>
	FSLV_X86_SSE2_FPU_REG0_OPRT_REGX <&tkn>,1
	stcksim_xchg XMMSTK,0,1,<name>
	stcksim_pop XMMSTK
endm

; type procedure
; perform the operation tkn={+-*/^} on ST(1) and ST(0), store the result in ST(1) and pop the stack.
; e.g.: ST(0) - ST(1)
FSLV_X86_SSE2_FPU_REG0_OPRT_REG1_P macro tkn
	FSLV_X86_SSE2_FPU_REG0_OPRT_REGX <&tkn>,1
	stcksim_xchg XMMSTK,0,1,<name>
	stcksim_pop XMMSTK
endm

FSLV_X86_SSE2_FPU_REG0_OPRT_REGX macro oprt,index
	IF fslv_lcl_real_type EQ 4
		IFIDN <&oprt>,<+>
			addss stcksim_get(XMMSTK,0,<name>),stcksim_get(XMMSTK,index,<name>)
		ELSEIFIDN <&oprt>,<->
			subss stcksim_get(XMMSTK,0,<name>),stcksim_get(XMMSTK,index,<name>)
		ELSEIFIDN <&oprt>,<*>
			mulss stcksim_get(XMMSTK,0,<name>),stcksim_get(XMMSTK,index,<name>)
		ELSEIFIDN <&oprt>,</>
			divss stcksim_get(XMMSTK,0,<name>),stcksim_get(XMMSTK,index,<name>)
		ELSEIFIDN <&oprt>,<^>
			T_EXPR(<sub esp,8>,<>)
			movss REAL4 ptr [T_REG(esp)][0],stcksim_get(XMMSTK,0,<name>)
			movss REAL4 ptr [T_REG(esp)][4],stcksim_get(XMMSTK,index,<name>)
			fld REAL4 ptr [T_REG(esp)][0]
			fld REAL4 ptr [T_REG(esp)][4]
			call fpu_st1_pow_st0
			fstp REAL4 ptr [T_REG(esp)][0]
			movss stcksim_get(XMMSTK,0,<name>),REAL4 ptr [T_REG(esp)][0]
			T_EXPR(<add esp,8>,<>)
		ENDIF
	ELSE
		IFIDN <&oprt>,<+>
			addsd stcksim_get(XMMSTK,0,<name>),stcksim_get(XMMSTK,index,<name>)
		ELSEIFIDN <&oprt>,<->
			subsd stcksim_get(XMMSTK,0,<name>),stcksim_get(XMMSTK,index,<name>)
		ELSEIFIDN <&oprt>,<*>
			mulsd stcksim_get(XMMSTK,0,<name>),stcksim_get(XMMSTK,index,<name>)
		ELSEIFIDN <&oprt>,</>
			divsd stcksim_get(XMMSTK,0,<name>),stcksim_get(XMMSTK,index,<name>)
		ELSEIFIDN <&oprt>,<^>
			T_EXPR(<sub esp,16>,<>)
			movsd REAL8 ptr [T_REG(esp)][0],stcksim_get(XMMSTK,0,<name>)
			movsd REAL8 ptr [T_REG(esp)][8],stcksim_get(XMMSTK,index,<name>)
			fld REAL8 ptr [T_REG(esp)][0]
			fld REAL8 ptr [T_REG(esp)][8]
			call fpu_st1_pow_st0
			fstp REAL8 ptr [T_REG(esp)][0]
			movsd stcksim_get(XMMSTK,0,<name>),REAL8 ptr [T_REG(esp)][0]
			T_EXPR(<add esp,16>,<>)
		ENDIF
	ENDIF
endm

FSLV_X86_SSE2_FPU_REGX_OPRT_REG0 macro oprt,index
	IF fslv_lcl_real_type EQ 4
		IFIDN <&oprt>,<+>
			addss stcksim_get(XMMSTK,0,<name>),stcksim_get(XMMSTK,index,<name>)
		ELSEIFIDN <&oprt>,<->
			xorps stcksim_get(XMMSTK,0,<name>),sse2_ss_neg_msk
			addss stcksim_get(XMMSTK,0,<name>),stcksim_get(XMMSTK,index,<name>)
		ELSEIFIDN <&oprt>,<*>
			mulss stcksim_get(XMMSTK,0,<name>),stcksim_get(XMMSTK,index,<name>)
		ELSEIFIDN <&oprt>,</>
			stcksim_push XMMSTK
			movss stcksim_get(XMMSTK,0,<name>),stcksim_get(XMMSTK,%(index+1),<name>)
			divss stcksim_get(XMMSTK,0,<name>),stcksim_get(XMMSTK,1,<name>)
			stcksim_xchg XMMSTK,0,1,<name>
			stcksim_pop XMMSTK
		ELSEIFIDN <&oprt>,<^>
			T_EXPR(<sub esp,8>,<>)
			movss REAL4 ptr [T_REG(esp)][0],stcksim_get(XMMSTK,index,<name>)
			movss REAL4 ptr [T_REG(esp)][4],stcksim_get(XMMSTK,0,<name>)
			fld REAL4 ptr [T_REG(esp)][0]
			fld REAL4 ptr [T_REG(esp)][4]
			call fpu_st1_pow_st0
			fstp REAL4 ptr [T_REG(esp)][0]
			movss stcksim_get(XMMSTK,0,<name>),REAL4 ptr [T_REG(esp)][0]
			T_EXPR(<add esp,8>,<>)
		ENDIF
	ELSE
		IFIDN <&oprt>,<+>
			addsd stcksim_get(XMMSTK,0,<name>),stcksim_get(XMMSTK,index,<name>)
		ELSEIFIDN <&oprt>,<->
			xorpd stcksim_get(XMMSTK,0,<name>),sse2_sd_neg_msk
			addsd stcksim_get(XMMSTK,0,<name>),stcksim_get(XMMSTK,index,<name>)
		ELSEIFIDN <&oprt>,<*>
			mulsd stcksim_get(XMMSTK,0,<name>),stcksim_get(XMMSTK,index,<name>)
		ELSEIFIDN <&oprt>,</>
			stcksim_push XMMSTK
			movsd stcksim_get(XMMSTK,0,<name>),stcksim_get(XMMSTK,%(index+1),<name>)
			divsd stcksim_get(XMMSTK,0,<name>),stcksim_get(XMMSTK,1,<name>)
			stcksim_xchg XMMSTK,0,1,<name>
			stcksim_pop XMMSTK
		ELSEIFIDN <&oprt>,<^>
			T_EXPR(<sub esp,16>,<>)
			movsd REAL8 ptr [T_REG(esp)][0],stcksim_get(XMMSTK,index,<name>)
			movsd REAL8 ptr [T_REG(esp)][8],stcksim_get(XMMSTK,0,<name>)
			fld REAL8 ptr [T_REG(esp)][0]
			fld REAL8 ptr [T_REG(esp)][8]
			call fpu_st1_pow_st0
			fstp REAL8 ptr [T_REG(esp)][0]
			movsd stcksim_get(XMMSTK,0,<name>),REAL8 ptr [T_REG(esp)][0]
			T_EXPR(<add esp,16>,<>)
		ENDIF
	ENDIF	
endm

; st(0) <+-*/^> real4/8/10/int32/64
FSLV_X86_SSE2_FPU_REG0_OPRT_MEM macro tkn,oprt,int_type,real_type
	fxsfrom_txt TEXTEQU <&tkn>
	IF int_type
		FSLV_X86_SSE2_FPU_LOAD_ARG <&tkn>,0
		FSLV_X86_SSE2_FPU_REG1_OPRT_REG0_P <&oprt>
	ELSE
		fxsfrom_reg = @IsTypedXmmReg(<tkn>,REAL4,REAL8)
		IF istxr_type
			fxsfrom_txt TEXTEQU istxr_xmm_reg
		ENDIF
		IF (real_type EQ 4) OR (fxsfrom_reg NE 0 AND fslv_lcl_real_type EQ 4)
			IF fslv_lcl_real_type EQ 8
				stcksim_push XMMSTK
				cvtss2sd stcksim_get(XMMSTK,0,<name>),fxsfrom_txt
				FSLV_X86_SSE2_FPU_REG1_OPRT_REG0_P <&oprt>
			ELSE
				IFIDN <&oprt>,<+>
					addss stcksim_get(XMMSTK,0,<name>),fxsfrom_txt
				ELSEIFIDN <&oprt>,<->
					subss stcksim_get(XMMSTK,0,<name>),fxsfrom_txt
				ELSEIFIDN <&oprt>,<*>
					mulss stcksim_get(XMMSTK,0,<name>),fxsfrom_txt
				ELSEIFIDN <&oprt>,</>
					divss stcksim_get(XMMSTK,0,<name>),fxsfrom_txt
				ELSEIFIDN <&oprt>,<^>
					T_EXPR(<sub esp,4>,<>)
					movss REAL4 ptr [T_REG(esp)][0],stcksim_get(XMMSTK,0,<name>)
					fld REAL4 ptr [T_REG(esp)][0]
					IFE fxsfrom_reg
						fld fxsfrom_txt
					ELSE
						movss REAL4 ptr [T_REG(esp)][4],fxsfrom_txt
						fld REAL4 ptr [T_REG(esp)][4]
					ENDIF
					call fpu_st1_pow_st0
					fstp REAL4 ptr [T_REG(esp)][0]
					movss stcksim_get(XMMSTK,0,<name>),REAL4 ptr [T_REG(esp)][0]
					T_EXPR(<add esp,4>,<>)
				ENDIF
			ENDIF
		ELSEIF (real_type EQ 8) OR (fxsfrom_reg NE 0 AND fslv_lcl_real_type EQ 8)
			IF fslv_lcl_real_type EQ 4
				stcksim_push XMMSTK
				cvtsd2ss stcksim_get(XMMSTK,0,<name>),fxsfrom_txt
				FSLV_X86_SSE2_FPU_REG1_OPRT_REG0_P <&oprt>
			ELSE
				IFIDN <&oprt>,<+>
					addsd stcksim_get(XMMSTK,0,<name>),fxsfrom_txt
				ELSEIFIDN <&oprt>,<->
					subsd stcksim_get(XMMSTK,0,<name>),fxsfrom_txt
				ELSEIFIDN <&oprt>,<*>
					mulsd stcksim_get(XMMSTK,0,<name>),fxsfrom_txt
				ELSEIFIDN <&oprt>,</>
					divsd stcksim_get(XMMSTK,0,<name>),fxsfrom_txt
				ELSEIFIDN <&oprt>,<^>
					T_EXPR(<sub esp,8>,<>)
					movsd REAL8 ptr [T_REG(esp)][0],stcksim_get(XMMSTK,0,<name>)
					fld REAL8 ptr [T_REG(esp)][0]
					IFE fxsfrom_reg
						fld fxsfrom_txt
					ELSE
						movsd REAL8 ptr [T_REG(esp)][8],fxsfrom_txt
						fld REAL8 ptr [T_REG(esp)][8]
					ENDIF
					call fpu_st1_pow_st0
					fstp REAL8 ptr [T_REG(esp)][0]
					movsd stcksim_get(XMMSTK,0,<name>),REAL8 ptr [T_REG(esp)][0]
					T_EXPR(<add esp,8>,<>)
				ENDIF
			ENDIF
		ELSE
			.err <[#FS8] type not supported: &tkn>
		ENDIF
	ENDIF
endm


; real4/8/10/int32/64 <+-*/^> st(0)
FSLV_X86_SSE2_FPU_MEM_OPRT_REG0 macro tkn,oprt,int_type,real_type
	fxsfrom_txt TEXTEQU <&tkn>
	IF int_type
		FSLV_X86_SSE2_FPU_LOAD_ARG <&tkn>,0
		FSLV_X86_SSE2_FPU_REG0_OPRT_REG1_P <&oprt>
	ELSE
		fxsfrom_reg = @IsTypedXmmReg(<tkn>,REAL4,REAL8)
		IF istxr_type
			fxsfrom_txt TEXTEQU istxr_xmm_reg
		ENDIF
		IF (real_type EQ 4) OR (fxsfrom_reg NE 0 AND fslv_lcl_real_type EQ 4)
			IF fslv_lcl_real_type EQ 8
				stcksim_push XMMSTK
				cvtss2sd stcksim_get(XMMSTK,0,<name>),fxsfrom_txt
				FSLV_X86_SSE2_FPU_REG0_OPRT_REG1_P <&oprt>
			ELSE
				IFIDN <&oprt>,<+>
					addss stcksim_get(XMMSTK,0,<name>),fxsfrom_txt
				ELSEIFIDN <&oprt>,<->
					xorps stcksim_get(XMMSTK,0,<name>),sse2_ss_neg_msk
					addss stcksim_get(XMMSTK,0,<name>),fxsfrom_txt
				ELSEIFIDN <&oprt>,<*>
					mulss stcksim_get(XMMSTK,0,<name>),fxsfrom_txt
				ELSEIFIDN <&oprt>,</>
					stcksim_push XMMSTK
					movss stcksim_get(XMMSTK,0,<name>),fxsfrom_txt
					divss stcksim_get(XMMSTK,0,<name>),stcksim_get(XMMSTK,1,<name>)
					stcksim_xchg XMMSTK,0,1,<name>
					stcksim_pop XMMSTK
				ELSEIFIDN <&oprt>,<^>
					T_EXPR(<sub esp,4>,<>)
					movss REAL4 ptr [T_REG(esp)][0],stcksim_get(XMMSTK,0,<name>)
					IFE fxsfrom_reg
						fld fxsfrom_txt
					ELSE
						movss REAL4 ptr [T_REG(esp)][4],fxsfrom_txt
						fld REAL4 ptr [T_REG(esp)][4]
					ENDIF
					fld REAL4 ptr [T_REG(esp)][0]
					call fpu_st1_pow_st0
					fstp REAL4 ptr [T_REG(esp)][0]
					movss stcksim_get(XMMSTK,0,<name>),REAL4 ptr [T_REG(esp)][0]
					T_EXPR(<add esp,4>,<>)
				ENDIF
			ENDIF
		ELSEIF (real_type EQ 8) OR (fxsfrom_reg NE 0 AND fslv_lcl_real_type EQ 8)
			IF fslv_lcl_real_type EQ 4
				stcksim_push XMMSTK
				cvtsd2ss stcksim_get(XMMSTK,0,<name>),fxsfrom_txt
				FSLV_X86_SSE2_FPU_REG0_OPRT_REG1_P <&oprt>
			ELSE
				IFIDN <&oprt>,<+>
					addsd stcksim_get(XMMSTK,0,<name>),fxsfrom_txt
				ELSEIFIDN <&oprt>,<->
					xorpd stcksim_get(XMMSTK,0,<name>),sse2_sd_neg_msk
					addsd stcksim_get(XMMSTK,0,<name>),fxsfrom_txt
				ELSEIFIDN <&oprt>,<*>
					mulsd stcksim_get(XMMSTK,0,<name>),fxsfrom_txt
				ELSEIFIDN <&oprt>,</>
					stcksim_push XMMSTK
					movsd stcksim_get(XMMSTK,0,<name>),fxsfrom_txt
					divsd stcksim_get(XMMSTK,0,<name>),stcksim_get(XMMSTK,1,<name>)
					stcksim_xchg XMMSTK,0,1,<name>
					stcksim_pop XMMSTK
				ELSEIFIDN <&oprt>,<^>
					T_EXPR(<sub esp,8>,<>)
					movsd REAL8 ptr [T_REG(esp)][0],stcksim_get(XMMSTK,0,<name>)
					IFE fxsfrom_reg
						fld fxsfrom_txt
					ELSE
						movsd REAL8 ptr [T_REG(esp)][8],fxsfrom_txt
						fld REAL8 ptr [T_REG(esp)][8]
					ENDIF
					fld REAL8 ptr [T_REG(esp)][0]
					call fpu_st1_pow_st0
					fstp REAL8 ptr [T_REG(esp)][0]
					movsd stcksim_get(XMMSTK,0,<name>),REAL8 ptr [T_REG(esp)][0]
					T_EXPR(<add esp,8>,<>)
				ENDIF
			ENDIF
		ELSE
			fslv_glb_error_msg TEXTEQU <[#FS9] type not supported: &tkn>
			fslv_glb_error = 1
		ENDIF
	ENDIF
endm

FSLV_X86_SSE2_FPU_FREE_REG macro index
	stcksim_set XMMSTK,index,<tag_inuse>,<0>
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
;		In this case the macro must store the result in a variable, which can be obtained by FSBE_ALLOC_RETVAL().
;		The obtained variable is of the type described through the paramters retval_int/real_type. After storing,
;       the stack must be poped.
;				
;	retval_int_type,
;	retval_real_type:
;		This parameters are valid, if bRetValue is not zero. They describe the return type of the calling macro (@fSlv4/8/I())
;
;   fCopy: This parameter is only used, if the Entity returns TRUE for FSEN_COPY_SUPPORTED.
;          If (fCopy&1) , the expression to solve contains only one memory or constant operand.
;              In that case the value is directly pass to the macro (->CpySrc = mem. operand) and not load on the stack. The macro must
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
FSLV_X86_SSE2_FPU_ASSIGN_DEST macro dest,dest_int_type,dest_real_type,bRetValue,retval_int_type,retval_real_type,fCopy,CpySrc
LOCAL retval
	fxsfad_cpy_ld = 0
	IF fCopy
		
		fxsfad_cs_ir = @IsTypedXmmReg(CpySrc,REAL4,REAL8)
		fxsfad_cs TEXTEQU istxr_xmm_reg
		IF istxr_type
			fxsfad_cs_type = istxr_type*4
		ELSE
			fxsfad_cs_type = fslv_lcl_real_type
		ENDIF
		
		fxsfad_d_ir = @IsTypedXmmReg(dest,REAL4,REAL8)
		fxsfad_d TEXTEQU istxr_xmm_reg
		fxsfad_d_type = istxr_type
		IF istxr_type
			fxsfad_d_type = istxr_type*4
		ELSE
			fxsfad_d_type = fslv_lcl_real_type
		ENDIF

		IF fxsfad_cs_ir NE 0 AND fxsfad_d_ir NE 0 AND fxsfad_d_type NE 0 AND bRetValue EQ 0 
			IF fxsfad_d_type EQ fxsfad_cs_type
				IFDIFI fxsfad_d,fxsfad_cs
					IF fxsfad_d_type EQ 8
						movsd fxsfad_d,fxsfad_cs
					ELSE
						movss fxsfad_d,fxsfad_cs
					ENDIF
				ENDIF
			ELSEIF fxsfad_d_type EQ 8
				cvtss2sd fxsfad_d,fxsfad_cs
			ELSEIF fxsfad_d_type EQ 4
				cvtsd2ss fxsfad_d,fxsfad_cs
			ENDIF
			EXITM <>
		ELSEIF fxsfad_cs_ir
			fxsfad_cs_isvr = @IsVolatileXmmReg(%fxsfad_cs)
			IF fxsfad_cs_type EQ dest_real_type AND fxsfad_d_ir EQ 0 AND bRetValue EQ 0
					IF fxsfad_cs_type EQ 8
						movsd dest,fxsfad_cs
					ELSE
						movss dest,fxsfad_cs
					ENDIF
					EXITM <>
			ELSEIF fxsfad_cs_type EQ fslv_lcl_real_type AND fxsfad_cs_isvr NE 0
				stcksim_push XMMSTK
				stcksim_set XMMSTK,0,<name>,%fxsfad_cs
			ELSE
				FSLV_X86_SSE2_FPU_LOAD_ARG <&CpySrc>,0
			ENDIF
		ELSE
			IF fxsfad_d_ir NE 0 AND bRetValue EQ 0 AND fxsfad_d_type EQ fslv_lcl_real_type
				stcksim_push XMMSTK
				stcksim_set XMMSTK,0,<name>,%fxsfad_d
				stcksim_pop XMMSTK
				FSLV_X86_SSE2_FPU_LOAD_ARG <&CpySrc>,0
				stcksim_pop XMMSTK
				EXITM <>
			ELSEIF fxsfad_d_ir NE 0
				FSLV_X86_SSE2_FPU_LOAD_ARG <&CpySrc>,0
			ELSE
				IF (type dest NE type CpySrc) OR ((fslv_volatile_gprs AND FSVGPR_EAX) EQ 0) OR bRetValue NE 0
					FSLV_X86_SSE2_FPU_LOAD_ARG <CpySrc>,0
				ELSE
					IF type dest EQ 8
						IF @WordSize EQ 4
							mov eax,DWORD ptr CpySrc
							mov DWORD ptr dest,eax
							mov eax,DWORD ptr CpySrc+4
							mov DWORD ptr dest+4,eax
						ELSE
							mov rax,CpySrc
							mov dest,rax
						ENDIF
					ELSEIF type dest EQ 4
						mov eax,DWORD ptr CpySrc
						mov DWORD ptr dest,eax
					ELSEIF type dest EQ 2
						mov ax,WORD ptr CpySrc
						mov WORD ptr dest,ax
					ELSEIF type dest EQ 1
						mov al,BYTE ptr CpySrc
						mov BYTE ptr dest,al
					ELSE
						fslv_glb_error_msg TEXTEQU <[#FS12] type not supported: &dest>
						fslv_glb_error = 1
						EXITM <>
					ENDIF
					EXITM <>
				ENDIF
			ENDIF
		ENDIF
	ENDIF
	IFNB <dest>
		IF @IsGPReg(<&dest>,<edx::eax>)
			IF igpr_size EQ 4 OR igpr_size EQ 8
				IF fslv_lcl_real_type EQ 4
					cvtss2si dest,stcksim_get(XMMSTK,0,<name>)
				ELSE
					cvtsd2si dest,stcksim_get(XMMSTK,0,<name>)
				ENDIF
				IF bRetValue NE 0 AND retval_int_type EQ igpr_size
					stcksim_pop XMMSTK
					EXITM <dest>
				ENDIF
			ELSEIF igpr_size NE -1
				IFE ((igpr_size AND 14) NE 0 AND igpr_index EQ 1) OR (igpr_size EQ 1 AND igpr_index LE 2)
					fxsfas_reg TEXTEQU T_REG(eax)
				ELSE
					fxsfas_reg TEXTEQU T_REG(ecx)
				ENDIF
				IFE @IsVolatileGPR(%fxsfas_reg)
					IF @WordSize EQ 4
						push fxsfas_reg
					ELSE
						mov QWORD ptr [rsp],fxsfas_reg
					ENDIF
				ENDIF
				IF fslv_lcl_real_type EQ 4
					cvtss2si fxsfas_reg,stcksim_get(XMMSTK,0,<name>)
				ELSE
					cvtsd2si fxsfas_reg,stcksim_get(XMMSTK,0,<name>)
				ENDIF
				IF  igpr_size EQ 2
					mov dest,@SubStr(2,%fxsfas_reg)
				ELSE
					IFE ((igpr_size AND 14) NE 0 AND igpr_index EQ 1) OR (igpr_size EQ 1 AND igpr_index LE 2)
						mov dest,al
					ELSE
						mov dest,cl
					ENDIF
				ENDIF
				IFE @IsVolatileGPR(%fxsfas_reg)
					IF @WordSize EQ 4
						pop fxsfas_reg
					ELSE
						mov fxsfas_reg,QWORD ptr [rsp]
					ENDIF
				ENDIF
			ELSE
				IF fslv_lcl_real_type EQ 4
					cvtss2si eax,stcksim_get(XMMSTK,0,<name>)
				ELSE
					cvtsd2si eax,stcksim_get(XMMSTK,0,<name>)
				ENDIF
				cdq
			ENDIF
		ELSE
			;fxsfas_xmm_reg = @IsTypedXmmReg(<&dest>,REAL8,REAL4)
			fxsfad_d_ir = @IsTypedXmmReg(dest,REAL4,REAL8)
			fxsfad_d TEXTEQU istxr_xmm_reg
			fxsfad_d_type = istxr_type
			IF istxr_type
				fxsfad_d_type = istxr_type*4
			ELSE
				fxsfad_d_type = fslv_lcl_real_type
			ENDIF
			IFE fxsfad_d_ir
				fxsfad_d TEXTEQU <dest>
			ENDIF
			
			IF dest_int_type NE 0
				IFE fslv_volatile_gprs AND FSVGPR_EAX
					mov T_EXPR(<DWORD ptr [esp-4]>,<QWORD ptr [rsp]>),T_REG(eax)
				ENDIF
				IF fslv_lcl_real_type EQ 4
					cvtss2si T_REG(eax),stcksim_get(XMMSTK,0,<name>)
				ELSE
					cvtsd2si T_REG(eax),stcksim_get(XMMSTK,0,<name>)
				ENDIF
				IF dest_int_type EQ 1
					mov dest,al
				ELSEIF dest_int_type EQ 2
					mov dest,ax
				ELSEIF dest_int_type EQ 4
					mov dest,eax
				ELSE
					mov dest,rax
				ENDIF
				IFE fslv_volatile_gprs AND FSVGPR_EAX
					mov T_REG(eax),T_EXPR(<DWORD ptr [esp-4]>,<QWORD ptr [rsp]>)
				ENDIF
				IF bRetValue NE 0 AND dest_int_type EQ retval_int_type
					stcksim_pop XMMSTK
					EXITM <dest>
				ENDIF
			ELSE
				IF (dest_real_type EQ 4) OR (fxsfad_d_ir NE 0 AND fxsfad_d_type EQ 4)
					IF fslv_lcl_real_type EQ 4
						IFDIFI fxsfad_d,stcksim_get(XMMSTK,0,<name>)
							movss fxsfad_d,stcksim_get(XMMSTK,0,<name>)
						ENDIF
					ELSE
						cvtsd2ss stcksim_get(XMMSTK,0,<name>),stcksim_get(XMMSTK,0,<name>)
						IFDIFI fxsfad_d,stcksim_get(XMMSTK,0,<name>)
							movss fxsfad_d,stcksim_get(XMMSTK,0,<name>)
						ENDIF
					ENDIF
				ELSEIF (dest_real_type EQ 8)  OR (fxsfad_d_ir NE 0 AND fxsfad_d_type EQ 8)
					IF fslv_lcl_real_type EQ 8
						IFDIFI fxsfad_d,stcksim_get(XMMSTK,0,<name>)
							movsd fxsfad_d,stcksim_get(XMMSTK,0,<name>)
						ENDIF
					ELSE
						cvtss2sd stcksim_get(XMMSTK,0,<name>),stcksim_get(XMMSTK,0,<name>)
						IFDIFI fxsfad_d,stcksim_get(XMMSTK,0,<name>)
							movsd fxsfad_d,stcksim_get(XMMSTK,0,<name>)
						ENDIF
					ENDIF
				ELSE
					fslv_glb_error_msg TEXTEQU <[#FS10] type not supported as destination: dest>
					fslv_glb_error = 1
				ENDIF
				IF bRetValue NE 0 AND dest_real_type EQ retval_real_type
					stcksim_pop XMMSTK
					EXITM fxsfad_d
				ENDIF
			ENDIF
		ENDIF
		IFE bRetValue
			stcksim_pop XMMSTK
		ENDIF
	ENDIF
	IFE bRetValue
		EXITM <>
	ELSE
		retval TEXTEQU FSBE_ALLOC_RETVAL()
		IF retval_int_type GE 4
			IFE fslv_volatile_gprs AND FSVGPR_EAX
				mov T_EXPR(<DWORD ptr [esp-4]>,<QWORD ptr [rsp]>),T_REG(eax)
			ENDIF
			IF retval_int_type EQ 4
				fxsfas_reg TEXTEQU <eax>
			ELSE
				fxsfas_reg TEXTEQU <rax>
			ENDIF
			IF fslv_lcl_real_type EQ 4
				cvtss2si fxsfas_reg,stcksim_get(XMMSTK,0,<name>)
			ELSE
				cvtsd2si fxsfas_reg,stcksim_get(XMMSTK,0,<name>)
			ENDIF
			mov retval,fxsfas_reg
			IFE fslv_volatile_gprs AND FSVGPR_EAX
				mov T_REG(eax),T_EXPR(<DWORD ptr [esp-4]>,<QWORD ptr [rsp]>)
			ENDIF
		ELSEIF retval_real_type EQ 4
			IF fslv_lcl_real_type EQ 4
				IFDIFI <retval>,stcksim_get(XMMSTK,0,<name>)
					movss retval,stcksim_get(XMMSTK,0,<name>)
				ENDIF
			ELSE
				cvtsd2ss stcksim_get(XMMSTK,0,<name>),stcksim_get(XMMSTK,0,<name>)
				IFDIFI <retval>,stcksim_get(XMMSTK,0,<name>)
					movss retval,stcksim_get(XMMSTK,0,<name>)
				ENDIF
			ENDIF
		ELSEIF retval_real_type EQ 8
			IF fslv_lcl_real_type EQ 8
				IFDIFI <retval>,stcksim_get(XMMSTK,0,<name>)
					movsd retval,stcksim_get(XMMSTK,0,<name>)
				ENDIF
			ELSE
				cvtss2sd stcksim_get(XMMSTK,0,<name>),stcksim_get(XMMSTK,0,<name>)
				IFDIFI <retval>,stcksim_get(XMMSTK,0,<name>)
					movsd retval,stcksim_get(XMMSTK,0,<name>)
				ENDIF
			ENDIF
		ELSE
			fslv_glb_error_msg TEXTEQU <[#FS11] type of return value not supported>
			fslv_glb_error = 1
		ENDIF
		stcksim_pop XMMSTK
		EXITM <retval>
	ENDIF
endm

FSLV_X86_SSE2_FPU_CMP macro cmpType,cmpSuffix,cParam,bool_byte,MemParam
	IFIDNI <&cmpType>,<cmp>
		fsxsfc_arg TEXTEQU <&MemParam>
		IFNB <MemParam>
			IF fslv_lcl_real_type NE @IsRealType(<&MemParam>) 
				FSLV_X86_SSE2_FPU_LOAD_ARG <&MemParam>,0
				fsxsfc_arg TEXTEQU <>
			ENDIF
		ENDIF
		IFB fsxsfc_arg
			IF fslv_lcl_real_type EQ 4
				comiss stcksim_get(XMMSTK,1,<name>),stcksim_get(XMMSTK,0,<name>)
			ELSE
				comisd stcksim_get(XMMSTK,1,<name>),stcksim_get(XMMSTK,0,<name>)
			ENDIF
			stcksim_pop XMMSTK
			stcksim_pop XMMSTK
		ELSE
			IF fslv_lcl_real_type EQ 4
				comiss stcksim_get(XMMSTK,0,<name>),fsxsfc_arg
			ELSE
				comisd stcksim_get(XMMSTK,0,<name>),fsxsfc_arg
			ENDIF
			stcksim_pop XMMSTK
		ENDIF		
		IFNB <&cmpSuffix>
			set&cmpSuffix& bool_byte
		ENDIF
	ELSEIFIDN <&cmpType>,<zero test>
		IF fslv_lcl_real_type EQ 4
			movss REAL4 ptr T_EXPR([esp-4],[rsp]),stcksim_get(XMMSTK,0,<name>)
			movss REAL4 ptr T_EXPR([esp-8],[rsp+4]),stcksim_get(XMMSTK,1,<name>)
			fld REAL4 ptr T_EXPR([esp-8],[rsp+4])
			fld REAL4 ptr T_EXPR([esp-4],[rsp])
		ELSE
			movsd REAL8 ptr T_EXPR([esp-8],[rsp]),stcksim_get(XMMSTK,0,<name>)
			movsd REAL8 ptr T_EXPR([esp-16],[rsp+8]),stcksim_get(XMMSTK,1,<name>)
			fld REAL8 ptr T_EXPR([esp-16],[rsp+8])
			fld REAL8 ptr T_EXPR([esp-8],[rsp])
		ENDIF
		T_EXPR(<push eax>)
		IF @WordSize EQ 8
			mov [rsp+2*8],rax
			mov [rsp+3*8],rdx
			sub rsp,2*8
		ENDIF
		invoke fpu_zero_test,cParam,0
		fstp st
		fstp st
		stcksim_pop XMMSTK
		stcksim_pop XMMSTK
		test eax,eax
		setnz fcmp_bool_byte
		T_EXPR(<pop eax>)
		IF @WordSize EQ 8
			add rsp,2*8
			mov rax,[rsp+2*8]
			mov rdx,[rsp+3*8]
		ENDIF
	ELSEIFIDNI <&cmpType>,<approx. eq.>
		fsxsfc_arg TEXTEQU <&MemParam>
		IFNB <MemParam>
			IF fslv_lcl_real_type NE @IsRealType(<&MemParam>) 
				FSLV_X86_SSE2_FPU_LOAD_ARG <&MemParam>,0
				fsxsfc_arg TEXTEQU <>
			ENDIF
		ENDIF
		
		IFB fsxsfc_arg
			FSLV_X86_SSE2_FPU_LOAD_REG 0,0
			fsxsfc_factor TEXTEQU <&cParam>
			IFNB <cParam>
				IF fslv_lcl_real_type NE @IsRealType(<&cParam>) 
					FSLV_X86_SSE2_FPU_LOAD_ARG <&cParam>,0
					fsxsfc_factor TEXTEQU <>
				ENDIF
			ENDIF
			IFNB fsxsfc_factor
				FSLV_X86_SSE2_FPU_REG0_OPRT_MEM %fsxsfc_factor,<*>,@IsIntegerType(%fsxsfc_factor),@IsRealType(%fsxsfc_factor)
			ELSE
				FSLV_X86_SSE2_FPU_REG1_OPRT_REG0_P <*>
			ENDIF
			FSLV_X86_SSE2_FPU_CALL_FNC <abs>
			FSLV_X86_SSE2_FPU_XCHG 2
			FSLV_X86_SSE2_FPU_REG1_OPRT_REG0_P <->
			FSLV_X86_SSE2_FPU_CALL_FNC <abs>
			IF fslv_lcl_real_type EQ 4
				comiss stcksim_get(XMMSTK,0,<name>),stcksim_get(XMMSTK,1,<name>)
			ELSE
				comisd stcksim_get(XMMSTK,0,<name>),stcksim_get(XMMSTK,1,<name>)
			ENDIF
			stcksim_pop XMMSTK
			stcksim_pop XMMSTK
		ELSE
			FSLV_X86_SSE2_FPU_LOAD_ARG %fsxsfc_arg,0
			fsxsfc_factor TEXTEQU <&cParam>
			IFNB <cParam>
				IF fslv_lcl_real_type NE @IsRealType(<&cParam>)
					FSLV_X86_SSE2_FPU_LOAD_ARG <&cParam>,0
					fsxsfc_factor TEXTEQU <>
				ENDIF
			ENDIF
			IFNB fsxsfc_factor
				FSLV_X86_SSE2_FPU_REG0_OPRT_MEM %fsxsfc_factor,<*>,@IsIntegerType(%fsxsfc_factor),@IsRealType(%fsxsfc_factor)
			ELSE
				FSLV_X86_SSE2_FPU_REG1_OPRT_REG0_P <*>
			ENDIF
			FSLV_X86_SSE2_FPU_CALL_FNC <abs>
			FSLV_X86_SSE2_FPU_XCHG
			FSLV_X86_SSE2_FPU_REG0_OPRT_MEM %fsxsfc_arg,<->,@IsIntegerType(%fsxsfc_arg),@IsRealType(%fsxsfc_arg)
			FSLV_X86_SSE2_FPU_CALL_FNC <abs>
			IF fslv_lcl_real_type EQ 4
				comiss stcksim_get(XMMSTK,0,<name>),stcksim_get(XMMSTK,1,<name>)
			ELSE
				comisd stcksim_get(XMMSTK,0,<name>),stcksim_get(XMMSTK,1,<name>)
			ENDIF
			stcksim_pop XMMSTK
			stcksim_pop XMMSTK
		ENDIF
		setbe fcmp_bool_byte
	ENDIF
endm
