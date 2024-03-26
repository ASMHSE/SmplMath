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

FSBE_N_BACKENDS = 0

FSC_NEG_ARG EQU 1

FSEN_INIT 			EQU 1	; param1 = version ; param2: not zero if called in context to compare operation ; retrun: zero, if an error occurs
FSEN_END			EQU 2
FSEN_MAX_STACK		EQU 3	; return: number of registers
FSEN_TEST_ATTR		EQU 4	; param1 = attribute to test for support ; return: true/false = 1/0
FSEN_STCK_USAGE		EQU 5	; param1 = number of used registers; return: integer that is added to the curent number of used registers
FSEN_MAPPING		EQU 6   ; assigne the macros for usage ; return: true/false; must set fslv_glb_instrset
FSEN_INSTRSET		EQU 7	; test supported instruction set:; param1 = {x86,x64,FPU,SSE} ; return: true/false = 1/0
FSEN_NAME			EQU 8	; return: backend's name/ID through fsbe_txt
FSEN_TEST_TYPE		EQU 9	; param1: type specifier={BOOLEAN_BYTE,SWORD,SDWORD,SQWORD,SQWORD,REAL4,REAL8,REAL10,...} ; return: zero if not supported or the type size
FSEN_NO_INT_CONST	EQU 10	; return true (ret!=0), to forece creation of REAL constants only.
FSEN_REQ_RETVAL		EQU 11	; return: true/false =  1/0; If true: the expression's result can't remain on the reg.-stack.
FSEN_NO_REAL_CONST	EQU 12	; return true to forece creation of integer constants only.
FSEN_ALL_INSTRSETS	EQU 13	; return: a comma seperated list of all supported instruction sets: e.g.: <x86,x64,fpu>
FSEN_CMP_SUPPORTED	EQU 14	; return: 1=> arguments must be loaded into registers, 2=> one memory operand is allowed
							;         The Entity must respons to FSEN_TEST_TYPE and return 1 for BOOLEAN_BYTE.
FSEN_COPY_SUPPORTED	EQU 15	; return: true/false => If true, the FSBE_ASSIGN_DEST macro takes care of the fCopy-parameter
FSEN_TLS_SIZE		EQU 16	; return: zero or the number of bytes fSlvTLS() should allocate.

FSEN_IMM_FREE		EQU 17  ; return: non-zero if FSBE_FREE_REG should be called directly if a regiser is going into a 'unused' state.
                            ;         By default used registers are freed after all calcutions had been done and the result is stored.

FSB_ENUM_START	EQU 0
FSB_ENUM_TKN	EQU 1
FSB_ENUM_END	EQU 2

FSBE_REGISTER_BACKEND macro ENTITY:req
	@CatStr(<FSO_BACKEND_ENTITY_>,%FSBE_N_BACKENDS) TEXTEQU <&ENTITY>
	FSBE_N_BACKENDS = FSBE_N_BACKENDS + 1
endm

LOAD_DEFAULT_BACKEND macro
	IF @WordSize EQ 4
		ASSIGN_BEST_BACKEND x86,FPU
	ELSEIF @WordSize EQ 8
		ASSIGN_BEST_BACKEND x64,SSE2,FPU
	ELSE
		.err <only x86-32 and x86-64 platforms supported>
	ENDIF
endm

ASSIGN_BEST_BACKEND macro args:VARARG
	abb_cntr = 0
	abb_txt TEXTEQU <>
	abb_cnt = 0
	FOR arg1,<args>
		abb_cnt = abb_cnt - 1
	ENDM
	abb_add = 1000
	WHILE abb_cntr LT FSBE_N_BACKENDS
		abb_backend_entity CATSTR <FSO_BACKEND_ENTITY_>,%abb_cntr
		IF @WordSize EQ 4
			abb_txt1 TEXTEQU be_remove_arg(x64,<&args>)
			abb_txt2 TEXTEQU FSBE_FNC(%abb_backend_entity,%FSEN_ALL_INSTRSETS)
			abb_txt2 TEXTEQU be_remove_arg(x64,%abb_txt2)
		ELSE
			abb_txt1 TEXTEQU be_remove_arg(x86,<&args>)
			abb_txt2 TEXTEQU FSBE_FNC(%abb_backend_entity,%FSEN_ALL_INSTRSETS)
			abb_txt2 TEXTEQU be_remove_arg(x86,%abb_txt2)
		ENDIF
		abb_cnt2 = be_cmp_instrsets(%abb_txt1,%abb_txt2)
		IF abb_cnt2 EQ 0 AND beci_add EQ 0
			abb_cnt = 0
			abb_add = beci_add
			abb_txt TEXTEQU abb_backend_entity
			EXITM
		ELSEIF abb_cnt2 EQ 0 AND beci_add LT abb_add
			abb_cnt = 0
			abb_add = beci_add
			abb_txt TEXTEQU abb_backend_entity
		ELSEIF abb_cnt2 LT 0 AND abb_cnt2 GT abb_cnt
			abb_cnt = abb_cnt2
			abb_add = beci_add
			abb_txt TEXTEQU abb_backend_entity
		ENDIF
		abb_cntr = abb_cntr + 1
	ENDM
	IF abb_cnt LT 0
		.err <no matching back end found for: args>
		EXITM
	ELSEIFB abb_txt
		.err <no matching back end found for: args>
		EXITM
	ELSEIF abb_add GT 0
		%_discard_ FSBE_FNC(%abb_txt,%FSEN_NAME)
		abb_txt1 TEXTEQU FSBE_FNC(%abb_txt,%FSEN_ALL_INSTRSETS)
		IF @WordSize EQ 4
			abb_txt1 TEXTEQU be_remove_arg(x64,%abb_txt1)
		ELSE
			abb_txt1 TEXTEQU be_remove_arg(x86,%abb_txt1)
		ENDIF
		echo
		echo ############################################################
		echo 
		%echo WARNING @FileCur:@CatStr(%@Line) :: selected back end <fsbe_txt> maybe use unwished instruction sets: abb_txt1
		echo 
		echo ############################################################
		echo
	ENDIF
	%_discard_ FSBE_FNC(%abb_txt,%FSEN_MAPPING)
	fslv_glb_flags = fslv_glb_flags AND (NOT FSF_INSTRSET_MSK)
	IF fslv_glb_instrset AND FSIS_FPU
		fslv_glb_flags = fslv_glb_flags or FSF_USE_FPU
	ENDIF
	IF fslv_glb_instrset AND FSIS_SSE2
		fslv_glb_flags = fslv_glb_flags or FSF_USE_SSE2
	ENDIF
endm

FSBE_FNC macro name,args:VARARG
	EXITM name(args)
endm

FSBE_PROC macro name,args:VARARG
	name args
endm

FSBE_STCK_CHK EQU fslv_stck_chk


FSBE_PREPARE_ALLOC macro lbl,_type,_align
	FSBE_ALLOC_TYPE TEXTEQU <&_type>
	FSBE_ALLOC_LBL  TEXTEQU <&lbl>
	FSBE_ALLOC_ALIGN = _align
endm

FSBE_ALLOC_TYPE TEXTEQU <>
FSBE_ALLOC_LBL  TEXTEQU <>
FSBE_ALLOC_ALIGN = 0

FSBE_ALLOC_RETVAL macro
	IFB FSBE_ALLOC_TYPE
		.err <invalid macro usage>
		EXITM <>
	ELSE
		IFE @fslv_TLS?()
			@TLS_warning
			.data?
				align FSBE_ALLOC_ALIGN
				@CatStr(%FSBE_ALLOC_LBL) FSBE_ALLOC_TYPE ?
			.code
		ELSE
			@CatStr(%FSBE_ALLOC_LBL) TEXTEQU @fslvTLS_line_alloc(%FSBE_ALLOC_TYPE)
		ENDIF
		FSBE_ALLOC_TYPE TEXTEQU <>
		EXITM FSBE_ALLOC_LBL
	ENDIF
endm

FSBE_ENTITY				TEXTEQU <>	; function  :: code:req, param1, param2
FSBE_ENUM_TKN			TEXTEQU <>	; procedure :: flg,tkn:=<>,tkn_flg:=<0>
FSBE_IS_TKN_REG       	TEXTEQU <>	; function  :: tkn:=<>
FSBE_IS_TKN_CONST		TEXTEQU <>	; function  :: tkn:=<>
FSBE_LOAD_CONST       	TEXTEQU <>	; procedure :: neg_arg:=<0>, name
FSBE_PREPARE_CONST		TEXTEQU <>  ; function  :: neg_arg:=<0>, name
FSBE_LOAD_ARG         	TEXTEQU <>  ; procedure :: tkn, neg_arg:=<0>
FSBE_PREPARE_ARG     	TEXTEQU <>  ; function  :: tkn, neg_arg:=<0>
FSBE_LOAD_REG         	TEXTEQU <>  ; procedure :: index, neg_arg:=<0>
FSBE_REG0_OPRT_MEM    	TEXTEQU <>  ; procedure :: tkn, oprt, int_type, real_type
FSBE_MEM_OPRT_REG0    	TEXTEQU <>	; procedure :: tkn, oprt, int_type, real_type
FSBE_REG0_OPRT_REG1_P 	TEXTEQU <>	; procedure :: oprt
FSBE_REG1_OPRT_REG0_P 	TEXTEQU <>	; procedure :: oprt
FSBE_REG0_OPRT_REGX   	TEXTEQU <>  ; procedure :: oprt,index
FSBE_REGX_OPRT_REG0   	TEXTEQU <>	; procedure :: oprt,index
FSBE_FREE_REG         	TEXTEQU <>	; procedure :: 
FSBE_SET_REG          	TEXTEQU <>	; procedure :: index, bPop:=<0>
FSBE_ASSIGN_DEST		TEXTEQU <>  ; function  :: dest, dest_int_type, dest_real_type, bRetValue, retval_int_type, retval_real_type, bCopy, CpySrc
FSBE_REG0_OPRT_RCVAL	TEXTEQU <>  ; function  :: oprt,tkn,neg_tkn
FSBE_NEG              	TEXTEQU <>  ; procedure ::   
FSBE_XCHG             	TEXTEQU <>	; procedure :: index:=<1>
FSBE_FNC_DSCPTR			TEXTEQU <>	; function  :: fnc_name, flags, bTest:=<0>
FSBE_CALL_FNC			TEXTEQU <>	; procedure :: fnc_name
FSBE_CMP				TEXTEQU <>	; procedure :: cmpType,cParam,MemParam

be_arg_cnt macro args:VARARG
	beac_cntr = 0
	FOR arg1,<args>
		beac_cntr = beac_cntr + 1
	ENDM
	EXITM %beac_cntr
endm

be_remove_arg macro discard,args:VARARG
	IFNB <discard>
		bera_cntr = 0
		bera_txt TEXTEQU <>
		FOR arg1,<&args>
			IFDIFI <discard>,<&arg1>
				IFB bera_txt
					bera_txt TEXTEQU <&arg1>
				ELSE
					bera_txt TEXTEQU bera_txt,<,>,<&arg1>
				ENDIF
			ENDIF
			bera_cntr = bera_cntr + 1
		ENDM
		EXITM bera_txt		
	ENDIF
	EXITM <args>
endm

be_cmp_instrsets macro listA,listB
	beci_cntr = be_arg_cnt(listB)-be_arg_cnt(listA)
	IF beci_cntr LT 0
		beci_add = 0
		EXITM @CatStr(%-(be_arg_cnt(listA)))
	ELSE
		beci_add = beci_cntr
	ENDIF
	beci_cntr = -(be_arg_cnt(listA))
	FOR arg1,<&listA>
		beci_match = 0
		FOR arg2,<&listB>
			IFIDNI <&arg2>,<&arg1>
				beci_match = 1
				EXITM
			ENDIF
		ENDM
		IF beci_match GT 0
			beci_cntr = beci_cntr + 1
		ELSE
			beci_add = beci_add + 1
		ENDIF
	ENDM
	EXITM %beci_cntr
endm
be_get_abs macro value
	IF value LT 0
		EXITM @CatStr(%(-value))
	ENDIF
	EXITM <&value>
endm

; reg = eax,ebx,.. => if x64 : rax,rbx,...
T_REG macro reg:req
	IF @WordSize EQ 4
		EXITM <reg>
	ELSE
		EXITM @CatStr(<r>,@SubStr(<&reg>,2,2))
	ENDIF	
endm

T_EXPR macro x86_32_expr,x86_64_expr
	IF @WordSize EQ 4
		EXITM <&x86_32_expr>
	ELSE
		EXITM <&x86_64_expr>
	ENDIF
endm

IF @WordSize EQ 4
	T_INT TEXTEQU <SDWORD>
	T_UINT TEXTEQU <DWORD>
ELSE
	T_INT TEXTEQU <SQWORD>
	T_UINT TEXTEQU <QWORD>
ENDIF


; checks if tkn is a GPR. Is macro takes care of the current word size (@WordSize).
; Additional register identifiers can be specified through additional_regs - this parameter has 
; a higher priority than the GPRs.
; 	ret == 0 => no reg
;   ret 1,2,4,8,-1 => register = igpr_size
;	igpr_size = 1,2,4,8,-1 = BYTE,WORD,DWORD,QWORD,(additional register)
;   igpr_index: switch igpr_size
;					case -1: one base index in additional_regs
;					case  1: 1-20 al,ah,cl,ch,dl,dh,bl,bh,sil,dil,bpl,spl,r8b,r9b,r10b,r11b,r12b,r13b,r14b,r15b
;					case  2: 1-16 ax,bx,cx,dx,di,si,bp,sp,r8w,r9w,r10w,r11w,r12w,r13w,r14w,r15w
;					case  4: 1-16 eax,ebx,ecx,edx,edi,esi,ebp,esp,r8d,r9d,r10d,r11d,r12d,r13d,r14d,r15d
;					case  8: 1-16 rax,rbx,rcx,rdx,rsi,rdi,rbp,rsp,r8,r9,r10,r11,r12,r13,r14,r15
@IsGPReg macro tkn:=<>,additional_regs:VARARG
	igpr_index = 0
	igpr_size = 0
	IFB <tkn>
		EXITM <0>
	ENDIF
	igpr_tkn TEXTEQU @TrimStr(<&tkn>)
	IFNB <&additional_regs>
		IF @MatchStrI(%igpr_tkn,0,additional_regs)
			igpr_index = msi_index
			igpr_size = -1
			EXITM <-1>
		ENDIF
	ENDIF
	IF @WordSize EQ 4
		igpr_char SUBSTR igpr_tkn,1,1
		igpr__size SIZESTR igpr_tkn
		IF igpr__size EQ 3
			IFIDNI igpr_char,<e>
				IFE @MatchStrI(%igpr_tkn,0,eax,ebx,ecx,edx,esi,edi,ebp,esp)
					EXITM <0>
				ENDIF
				igpr_index = msi_index
				igpr_size = 4
			ENDIF
			EXITM %igpr_size
		ELSEIF igpr_size EQ 2
			IFE @MatchStrI(%igpr_tkn,0,ax,bx,cx,dx,si,di,bp,sp,al,ah,cl,ch,dl,dh,bl,bh)
				EXITM <0>
			ENDIF
			igpr_index = @SubStr(< 01020304050607080102030405060708>,%msi_index*2,2)
			igpr_size = @SubStr(<2222222211111111>,%msi_index,1)
			EXITM %igpr_size
		ELSE
			EXITM <0>
		ENDIF
	ELSEIF @WordSize EQ 8
		igpr_char SUBSTR igpr_tkn,1,1
		igpr__size SIZESTR igpr_tkn
		IFIDNI igpr_char,<r>
			IF igpr__size EQ 2
				IFE @MatchStrI(%igpr_tkn,0,r8,r9)
					EXITM <0>
				ENDIF
				igpr_index = msi_index + 8
				igpr_size = 8
			ELSEIF igpr__size EQ 3
				IFE @MatchStrI(%igpr_tkn,0,rax,rbx,rcx,rdx,rsi,rdi,rbp,rsp,r10,r11,r12,r13,r14,r15,r8d,r9d,r8w,r9w,r8b,r9b)
					EXITM <0>
				ENDIF
				igpr_index = @SubStr(< 0102030405060708111213141516091009101314>,%msi_index*2,2)
				igpr_size = @SubStr(<88888888888888442211>,%msi_index,1)
			ELSEIF igpr__size EQ 4
				IFE @MatchStrI(%igpr_tkn,0,r10d,r11d,r12d,r13d,r14d,r15d,r10w,r11w,r12w,r13w,r14w,r15w,r10b,r11b,r12b,r13b,r14b,r15b)
					EXITM <0>
				ENDIF
				igpr_index = @SubStr(< 111213141516111213141516151617181920>,%msi_index*2,2)
				igpr_size = @SubStr(<444444222222111111>,%msi_index,1)
			ENDIF
			EXITM %igpr_size
		ELSEIFIDNI igpr_char,<e>
			IF igpr__size EQ 3
				IFE @MatchStrI(%igpr_tkn,0,eax,ebx,ecx,edx,esi,edi,ebp,esp)
					EXITM <0>
				ENDIF
				igpr_index = msi_index
				igpr_size = 4
			ENDIF
			EXITM %igpr_size
		ELSE
			IF igpr__size EQ 3
				IFE @MatchStrI(%igpr_tkn,0,sil,dil,bpl,spl)
					EXITM <0>
				ENDIF
				igpr_index = msi_index + 8
				igpr_size = 1
			ELSEIF igpr__size EQ 2
				IFE @MatchStrI(%igpr_tkn,0,ax,bx,cx,dx,di,si,bp,sp,al,ah,cl,ch,dl,dh,bl,bh)
					EXITM <0>
				ENDIF
				igpr_index = @SubStr(< 01020304050607080102030405060708>,%msi_index*2,2)
				igpr_size = @SubStr(<2222222211111111>,%msi_index,1)
			ENDIF
			EXITM %igpr_size
		ENDIF
	ELSE
		.err <only x86-32 and x86-64 supported>
		EXITM <0>
	ENDIF
endm

; this macro requires a preceding call to @IsGPReg()
@GetBaseGPR macro b_r32:=<1>
	IF igpr_size EQ 8
		IFE b_r32
			EXITM @GetArgByIndex(%igpr_index-1,,rax,rbx,rcx,rdx,rsi,rdi,rbp,rsp,r8,r9,r10,r11,r12,r13,r14,r15)
		ELSE
			EXITM @GetArgByIndex(%igpr_index-1,,eax,ebx,ecx,edx,edi,esi,ebp,esp,r8d,r9d,r10d,r11d,r12d,r13d,r14d,r15d)
		ENDIF
	ELSEIF igpr_size EQ 4 OR igpr_size EQ 2
		IF @WordSize EQ 8 AND b_r32 EQ 0
			EXITM @GetArgByIndex(%igpr_index-1,,rax,rbx,rcx,rdx,rsi,rdi,rbp,rsp,r8,r9,r10,r11,r12,r13,r14,r15)
		ELSE
			EXITM @GetArgByIndex(%igpr_index-1,,eax,ebx,ecx,edx,edi,esi,ebp,esp,r8d,r9d,r10d,r11d,r12d,r13d,r14d,r15d)
		ENDIF
	ELSEIF igpr_size EQ 1
		IF @WordSize EQ 8 AND b_r32 EQ 0
			EXITM @GetArgByIndex(%igpr_index-1,,rax,rax,rcx,rcx,rdx,rdx,rbx,rbx,rsi,rdi,rbp,rsp,r8,r9,r10,r11,r12,r13,r14,r15)
		ELSE
			EXITM @GetArgByIndex(%igpr_index-1,,eax,eax,ecx,ecx,edx,edx,ebx,ebx,esi,edi,ebp,esp,r8d,r9d,r10d,r11d,r12d,r13d,r14d,r15d)
		ENDIF
	ELSE
		.err <invalid register size>
		EXITM <0>
	ENDIF
endm

; this macro requires a preceding call to @IsGPReg()
@CmpBaseGPR macro r32
	IFIDNI @GetBaseGPR(),<r32>
		EXITM <-1>
	ENDIF
	EXITM <0>
endm

; return: one based register index: 1-8 for x32 and 1-16 for x64, isxr_index = EXITM<>
;         zero if no xmm register
IF @Version GE 800
@IsXmmReg macro txt
	isxr_index = 0
	IFNB <&txt>
		isxr_txt TEXTEQU @TrimStr(<&txt>)
		isxr_size = @SizeStr(%isxr_txt)
		IF isxr_size GE 4
			IFIDNI @SubStr(%isxr_txt,1,3),<xmm>
				isxr_index = @InStr(1,<0123456789>,@SubStr(%isxr_txt,4,1))
				IF isxr_index
					IF isxr_size EQ 5 AND @WordSize EQ 8
						IF @InStr(1,<0123456789>,@SubStr(%isxr_txt,5,1))
							isxr_index = @SubStr(%isxr_txt,4,2)
							IF isxr_index LT 16
								isxr_index = isxr_index + 1
								EXITM isxr_index
							ENDIF
							EXITM <0>
						ENDIF
						EXITM <0>
					ELSE
						IF isxr_size EQ 5 AND @WordSize EQ 4
							EXITM <0>
						ELSEIF isxr_index LE @WordSize*2
							EXITM %isxr_index
						ENDIF
						EXITM <0>
					ENDIF
				ENDIF
				EXITM <0>
			ENDIF
			EXITM <0>
		ENDIF
		EXITM <0>
	ENDIF
	EXITM <0>
endm
ELSE
@IsXmmReg macro txt
	isxr_index = 0
	IFNB <&txt>
		isxr_txt TEXTEQU @TrimStr(<&txt>)
		isxr_size = @SizeStr(%isxr_txt)
		IF isxr_size GE 4
			IFIDNI @SubStr(%isxr_txt,1,3),<xmm>
				isxr_index = @InStr(1,<01234567(0)(1)(2)(3)(4)(5)(6)(7)>,@SubStr(%isxr_txt,4))
				IF isxr_index
					IF isxr_index GE 9 AND isxr_size EQ 6
						isxr_index = @SubStr(%isxr_txt,5,1)+1
						EXITM %isxr_index
					ELSEIF isxr_index LE 8 AND isxr_size EQ 4
						EXITM %isxr_index
					ENDIF
					EXITM <0>
				ENDIF
				EXITM <0>
			ENDIF
			EXITM <0>
		ENDIF
		EXITM <0>
	ENDIF
	EXITM <0>
endm
ENDIF

; input: txt = pre-parsed token tht deos not contain operators, brackets, ...
;        types = type qualifiers (REAL8,REAL4,...)
; return: EXITM      = @IsXmmReg() 
;         istxr_type = one based index in <types> or zero if no typecast found.
;                      If a xmm register is typecasted with an unknown type (not in list <types>),
;                      -1 is returned
;         istxr_xmm_reg = register name without typecast
@IsTypedXmmReg macro txt,types:VARARG
	istxr_type = 0
	istxr_xmm_reg TEXTEQU <>
	IFB <&txt>
		EXITM <0>
	ENDIF
	istxr_txt TEXTEQU @TrimStr(<&txt>)
	istxr_size = @SizeStr(%istxr_txt)
	istxr_pos1 INSTR 1,istxr_txt,< > ; space
	istxr_pos2 INSTR 1,istxr_txt,<	> ; tab
	IF (istxr_size LT 10) OR ((istxr_pos1 EQ 0) AND (istxr_pos2 EQ 0))  ; minimum size: <X ptr xmmY>
		istxr_ret = @IsXmmReg(<&txt>)
		IF istxr_ret
			istxr_xmm_reg TEXTEQU <xmm>,%istxr_ret-1
		ENDIF
		EXITM %istxr_ret
	ENDIF
	IFE istxr_pos1
		istxr_pos1 = istxr_pos2
	ENDIF
	IF (istxr_pos1 GT istxr_pos2) AND (istxr_pos2 NE 0)
		istxr_pos1 = istxr_pos2
	ENDIF
	istxr_txt1 SUBSTR istxr_txt,1,istxr_pos1-1
	istxr_txt2 SUBSTR istxr_txt,istxr_pos1+1
	istxr_txt2 TEXTEQU @TrimStr(%istxr_txt2)
	istxr_size = @SizeStr(%istxr_txt2)
	istxr_pos1 INSTR 1,istxr_txt2,< > ; space
	istxr_pos2 INSTR 1,istxr_txt2,<	> ; tab
	IF (istxr_size LT 8) OR ((istxr_pos1 EQ 0) AND (istxr_pos2 EQ 0))  ; minimum size: <ptr xmmY>
		EXITM <0> ; case: <X Y> -> no xmm reg
	ENDIF
	IFE istxr_pos1
		istxr_pos1 = istxr_pos2
	ENDIF
	IF (istxr_pos1 GT istxr_pos2) AND (istxr_pos2 NE 0)
		istxr_pos1 = istxr_pos2
	ENDIF
	istxr_txt3 SUBSTR istxr_txt2,1,istxr_pos1-1
	istxr_txt4 SUBSTR istxr_txt2,istxr_pos1+1
	IFDIFI istxr_txt3,<ptr>
		EXITM <0>	; case: <X Y Z> -> no xmm reg
	ENDIF
	istxr_ret = @IsXmmReg(%istxr_txt4)
	IFE istxr_ret
		EXITM <0>
	ELSE
		istxr_xmm_reg TEXTEQU <xmm>,%istxr_ret-1
	ENDIF
	istxr_type = @TypeMatch(%istxr_txt1,types)
	IFE istxr_type
		istxr_type = -1
	ENDIF
	EXITM %istxr_ret
endm

@RemoveArgI macro discard,args:VARARG
	IFNB <discard>
		@rai_cntr = 0
		@rai_txt TEXTEQU <>
		FOR arg1,<&args>
			IFDIFI <discard>,<&arg1>
				IFB @rai_txt
					@rai_txt TEXTEQU <&arg1>
				ELSE
					@rai_txt TEXTEQU @rai_txt,<,>,<&arg1>
				ENDIF
			ENDIF
			@rai_cntr = @rai_cntr + 1
		ENDM
		EXITM @rai_txt		
	ENDIF
	EXITM <args>
endm

@AddArgPrefix macro prefix,args:VARARG
	@aap_txt TEXTEQU <>
	FOR arg1,<&args>
		IFB @aap_txt
			@aap_txt TEXTEQU <&prefix>,arg1
		ELSE
			@aap_txt TEXTEQU @aap_txt<,&prefix>,arg1
		ENDIF
	ENDM
	EXITM @aap_txt
endm

;/**
; * stcksim_xxx : macros to simulate a virtual register stack
; */
stcksim_create macro name:req,regs:VARARG
	stcksim_nregs = 0
	FOR arg,<regs>
		@CatStr(<stcksim_&name&_reg_name_>,%stcksim_nregs) TEXTEQU <arg>
		@CatStr(<stcksim_&name&_reg_tag_inuse_>,%stcksim_nregs) TEXTEQU <0>
		stcksim_nregs = stcksim_nregs + 1
	ENDM
	IF stcksim_nregs EQ 0
		fslv_glb_error_msg TEXTEQU <empty list>
		fslv_glb_error = 1
		EXITM
	ENDIF
	stcksim_&name&_nregs = stcksim_nregs
	stcksim_&name&_index = -1
endm

stcksim_push macro name:req
	IFNDEF stcksim_&name&_nregs
		.err <virtual register stack not defined>
	ENDIF
	IF stcksim_&name&_index+1 GE stcksim_&name&_nregs
		fslv_glb_error_msg TEXTEQU <virtual register stack: overflow>
		fslv_glb_error = 1
	ELSE
		stcksim_&name&_index = stcksim_&name&_index + 1
		IF stcksim_get(name,0,<tag_inuse>) NE 0
			fslv_glb_error_msg TEXTEQU <virtual register stack: overflow (non-empty register pushed)>
			fslv_glb_error = 1
		ELSE
			stcksim_set name,0,<tag_inuse>,1
		ENDIF
	ENDIF
endm

stcksim_pop macro name:req
	IFNDEF stcksim_&name&_nregs
		.err <virtual register stack not defined>
	ENDIF
	IF stcksim_&name&_index-1 LT -1
		fslv_glb_error_msg TEXTEQU <virtual register stack: underrun>
		fslv_glb_error = 1
	ELSE
		IF (stcksim_&name&_index) LT 0
			@CatStr(<stcksim_&name&_reg_tag_inuse_>,%(stcksim_&name&_index+stcksim_&name&_nregs)) TEXTEQU <0>
		ELSE
			@CatStr(<stcksim_&name&_reg_tag_inuse_>,%(stcksim_&name&_index)) TEXTEQU <0>
		ENDIF
		stcksim_&name&_index = stcksim_&name&_index - 1
	ENDIF
endm

stcksim_get macro name:req,index:req,txtName:=<name>
	IFNDEF stcksim_&name&_nregs
		.err <virtual register stack not defined>
	ENDIF
	IF index GE stcksim_&name&_nregs
		fslv_glb_error_msg TEXTEQU <virtual register stack: register _index out of range>
		fslv_glb_error = 1
		EXITM <0>
	ELSE
		IF (stcksim_&name&_index-index) LT 0
			stcksimget_tmp TEXTEQU @CatStr(<stcksim_&name&_reg_&txtName&_>,%(stcksim_&name&_index-index+stcksim_&name&_nregs))
			EXITM stcksimget_tmp
		ELSE
			stcksimget_tmp TEXTEQU @CatStr(<stcksim_&name&_reg_&txtName&_>,%(stcksim_&name&_index-index))
			EXITM stcksimget_tmp
		ENDIF
	ENDIF
endm

stcksim_set macro name:req,index:req,txtName:=<name>,txt:=<>
	IFNDEF stcksim_&name&_nregs
		.err <virtual register stack not defined>
	ENDIF
	IF index GE stcksim_&name&_nregs
		fslv_glb_error_msg TEXTEQU <virtual register stack: register _index out of range>
		fslv_glb_error = 1
	ELSE
		IF (stcksim_&name&_index-index) LT 0
			@CatStr(<stcksim_&name&_reg_&txtName&_>,%(stcksim_&name&_index-index+stcksim_&name&_nregs)) TEXTEQU <&txt>
			IFDIF <&txtName>,<tag_inuse>
				@CatStr(<stcksim_&name&_reg_tag_inuse_>,%(stcksim_&name&_index-index+stcksim_&name&_nregs)) TEXTEQU <1>
			ENDIF
		ELSE
			@CatStr(<stcksim_&name&_reg_&txtName&_>,%(stcksim_&name&_index-index)) TEXTEQU <&txt>
			IFDIF <&txtName>,<tag_inuse>
				@CatStr(<stcksim_&name&_reg_tag_inuse_>,%(stcksim_&name&_index-index)) TEXTEQU <1>
			ENDIF
		ENDIF
	ENDIF
endm

stcksim_set_all macro name:req,txtName:=<name>,txt:=<>
	IFNDEF stcksim_&name&_nregs
		.err <virtual register stack not defined>
	ENDIF
	sssar_cntr = 0
	WHILE sssar_cntr LT stcksim_&name&_nregs
		stcksim_set name,%sssar_cntr,<&txtName>,<&txt>
		sssar_cntr = sssar_cntr + 1
	ENDM	
endm

stcksim_xchg macro name:req,index1,index2,txtNames:VARARG
	IFNDEF stcksim_&name&_nregs
		.err <virtual register stack not defined>
	ENDIF
	IF index1 GE stcksim_&name&_nregs OR index2 GE stcksim_&name&_nregs
		fslv_glb_error_msg TEXTEQU <virtual register stack: register index out of range>
		fslv_glb_error = 1
		EXITM
	ENDIF
	IF index1 NE index2
		FOR arg,<txtNames>
			IF stcksim_get(name,index1,<tag_inuse>) EQ 0 OR stcksim_get(name,index2,<tag_inuse>) EQ 0
				fslv_glb_error_msg TEXTEQU <virtual register stack: XCHG with empty register>
				fslv_glb_error = 1
				EXITM
			ELSE
				ssxch_tmp TEXTEQU stcksim_get(name,index1,<&arg>)
				stcksim_set name,index1,<&arg>,stcksim_get(name,index2,<&arg>)
				stcksim_set name,index2,<&arg>,%ssxch_tmp
			ENDIF
		ENDM
	ENDIF
endm

stcksim_get_range macro name:req,index1,index2,txtName:=<name>
	IFNDEF stcksim_&name&_nregs
		.err <virtual register stack not defined>
		EXITM <>
	ENDIF
	IF index1 GE stcksim_&name&_nregs OR index2 GE stcksim_&name&_nregs
		fslv_glb_error_msg TEXTEQU <virtual register stack: register index out of range>
		fslv_glb_error = 1
		EXITM <>
	ENDIF
	ssgr_txt TEXTEQU <>
	IF index1 LE index2
		ssgr_cntr = index1
		WHILE ssgr_cntr LE index2
			IF ssgr_cntr EQ index1
				ssgr_txt TEXTEQU stcksim_get(name,%ssgr_cntr,txtName)
			ELSE
				ssgr_txt TEXTEQU ssgr_txt,<,>,stcksim_get(name,%ssgr_cntr,txtName)
			ENDIF
			ssgr_cntr = ssgr_cntr + 1
		ENDM
	ELSE
		ssgr_cntr = index2
		WHILE ssgr_cntr LE index1
			IF ssgr_cntr EQ index2
				ssgr_txt TEXTEQU stcksim_get(name,%ssgr_cntr,txtName)
			ELSE
				ssgr_txt TEXTEQU stcksim_get(name,%ssgr_cntr,txtName),<,>,ssgr_txt
			ENDIF
			ssgr_cntr = ssgr_cntr + 1
		ENDM
	ENDIF
	EXITM ssgr_txt
endm

stcksim_get_all_free macro name:req,txtName:=<name>
	IFNDEF stcksim_&name&_nregs
		.err <virtual register stack not defined>
		EXITM <>
	ENDIF
	ssgr_txt TEXTEQU <>
	ssgr_cntr = 0
	WHILE ssgr_cntr LT stcksim_&name&_nregs
		IF stcksim_get(name,%ssgr_cntr,<tag_inuse>) EQ 0
			IFB ssgr_txt
				ssgr_txt TEXTEQU stcksim_get(name,%ssgr_cntr,txtName)
			ELSE
				ssgr_txt TEXTEQU ssgr_txt,<,>,stcksim_get(name,%ssgr_cntr,txtName)
			ENDIF
		ENDIF
		ssgr_cntr = ssgr_cntr + 1
	ENDM
	EXITM ssgr_txt
endm


stcksim_dbgout macro name:req,txtName
	IFNDEF stcksim_&name&_nregs
		%echo ERR: virtual register stack not defined: name
		EXITM
	ENDIF
	sssar_cntr = 0
	WHILE sssar_cntr LT stcksim_&name&_nregs
		%echo ST(@CatStr(%sssar_cntr)) = stcksim_get(<name>,%sssar_cntr,<txtName>)
		sssar_cntr = sssar_cntr + 1
	ENDM	
endm

stcksim_find macro name:req,case,txtName,value
	IFNDEF stcksim_&name&_nregs
		.err <virtual register stack not defined>
	ENDIF
	sssar_cntr = 0
	ssf_match = 0
	WHILE sssar_cntr LT stcksim_&name&_nregs
		ssf_value TEXTEQU stcksim_get(name,%sssar_cntr,txtName)
		IFIDNI <&case>,<insensitive>
			IFIDNI ssf_value,<&value>
				ssf_match = 1
				EXITM
			ENDIF
		ELSEIFIDNI <&case>,<sensitive>
			IFIDN ssf_value,<&value>
				ssf_match = 1
				EXITM
			ENDIF
		ELSEIFIDNI <&case>,<numeric>
			IF ssf_value EQ value
				ssf_match = 1
				EXITM
			ENDIF
		ELSEIFIDNI <&case>,<expression>
			IF &value
				ssf_match = 1
				EXITM
			ENDIF
		ENDIF
		sssar_cntr = sssar_cntr + 1
	ENDM
	IF ssf_match
		EXITM %sssar_cntr
	ELSE
		EXITM <-1>
	ENDIF
endm

FSBE_IRegCallBySignature macro BackEndId:req,fnc_name:req,real_type:req,int_type:req,InstrSets:req,nRegs:req,argList,freeRegs
	
	IFNDEF lst_&BackEndId&_cnt
		; 1:fnc_name,2:realType,3:intType,4:nRegs,5:argList,6:freeRegs,7:label,8:InstrSets
		lst_create BackEndId,1,8
	ENDIF
	
	fbcbs_match = 0
	IF lst_tm_search(BackEndId,1,fnc_name)
		fbcbs_imax = lst_get_cnt(BackEndId)
		fbcbs_index = 1
		WHILE fbcbs_index LE fbcbs_imax
			IFIDN <fnc_name>,lst_get_data(BackEndId,%fbcbs_index,1)
				IF real_type EQ lst_get_data(BackEndId,%fbcbs_index,2) AND int_type EQ lst_get_data(BackEndId,%fbcbs_index,3) AND nRegs EQ lst_get_data(BackEndId,%fbcbs_index,4) AND InstrSets EQ lst_get_data(BackEndId,%fbcbs_index,8)
					fbcbs_args TEXTEQU lst_get_data(BackEndId,%fbcbs_index,5)
					fbcbs_cntr = 0 
					fbcbs_match = 1
				%	FOR arg,<fbcbs_args>
						IFDIFI @GetArgByIndex(%fbcbs_cntr,<invalid>,argList),<arg>
							fbcbs_match = 0
							EXITM
						ENDIF
						fbcbs_cntr =  fbcbs_cntr + 1
					ENDM
					fbcbs_args TEXTEQU lst_get_data(BackEndId,%fbcbs_index,6)
					IF fbcbs_match
					%	FOR arg,<fbcbs_args>
							IFE @MatchStrI(arg,,freeRegs)
								fbcbs_match = 0
								EXITM
							ENDIF
						ENDM
					ENDIF		
				ENDIF
			ENDIF
			IF fbcbs_match
				EXITM
			ENDIF
			fbcbs_index = fbcbs_index + 1
		ENDM		
	ENDIF
	IF fbcbs_match
		call lst_get_data(BackEndId,%fbcbs_index,7)
	ELSE
		fbcbs_index = lst_append(BackEndId,8)
		fbcbs_label TEXTEQU get_unique_lbl(FSBE_IRegBySig_&fnc_name&_)
		lst_set_data BackEndId,%fbcbs_index,1,<&fnc_name>
		lst_set_data BackEndId,%fbcbs_index,2,<&real_type>
		lst_set_data BackEndId,%fbcbs_index,3,<&int_type>
		lst_set_data BackEndId,%fbcbs_index,4,<&nRegs>
		lst_set_data BackEndId,%fbcbs_index,5,<&argList>
		lst_set_data BackEndId,%fbcbs_index,7,%fbcbs_label
		lst_set_data BackEndId,%fbcbs_index,8,<&InstrSets>
		
		fbcbs_freeRegs TEXTEQU <>
		fbcbs_cntr = 0
		FOR arg,<&freeRegs>
			IF fbcbs_cntr LT nRegs AND fbcbs_cntr EQ 0
				fbcbs_freeRegs TEXTEQU <&arg>
			ELSEIF fbcbs_cntr LT nRegs
				fbcbs_freeRegs TEXTEQU fbcbs_freeRegs,<,>,<&arg>
			ELSE
				EXITM
			ENDIF
		ENDM
		lst_set_data BackEndId,%fbcbs_index,6,<%fbcbs_freeRegs>
		
		IRegFncs SEGMENT READONLY PUBLIC 'CODE'
			align 16
			fbcbs_label:
			fslv_fnc_ireg_&fnc_name& argList,freeRegs
			db 0c3h	; RET
		IRegFncs ENDS

		call fbcbs_label

	ENDIF	
endm


;TODO: global GPR save-strategy
;
;FSBE_INIT_REG_SAVE macro reserved:VARARG
;;	IF @WordSize EQ 8 AND x64_nAlloc NE 0
;;		fbirs_x64_nalloc = (x64_nAlloc+1) AND -2
;;		sub rsp,fbirs_x64_nalloc*8
;;	ELSE
;;		fbirs_x64_nalloc = 0
;;	ENDIF
;	lst_create fsbe_sv_regs,0,0
;endm
;
;; reg32 must be 32 bit register: EAX or EDX
;FSBE_SAVE_REG macro reg32
;	IF @IsVolatileGPR(reg32)
;		fbsr_reg_name TEXTEQU @ToUpperCase(<reg32>)
;		IF @MatchStrI(%fbsr_reg_name,0,EAX,EDX)
;			IFE lst_tm_search(fsbe_sv_regs,1,%fbsr_reg_name)
;				fbsr_iline = lst_append(fsbe_sv_regs)
;				lst_set_data fsbe_sv_regs,%fbsr_iline,1,%fbsr_reg_name
;			ENDIF
;			lst_get_cnt(fsbe_sv_regs)
;		ELSE
;			.err <#dev_error: invalid usage: only EAX and EDX curently supported>
;		ENDIF		
;	ENDIF
;endm
