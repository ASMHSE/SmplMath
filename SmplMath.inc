;/********************************************************
; * This file is part of the SmplMath macros-system.     *
; *                                                      *
; *          Copyright by qWord, 2011-2014               *
; *                                                      *
; *            Modified by HSE, 2020-2021                *
; *                                                      *
; *          SmplMath.Masm{at}gmx{dot}net                *
; *    http://sourceforge.net/projects/smplmath/         *
; *                                                      *
; *       https://github.com/ASMHSE/SmplMath             *
; *                                                      *
; *  Further Copyright and Legal statements are located  *
; *  in the documentation (Documentation.pdf).           *
; *                                                      *
; ********************************************************

IFNDEF BOOLEAN_BYTE
	BOOLEAN_BYTE typedef BYTE
ENDIF

fslv_glb_error = 0
fslv_glb_error_msg TEXTEQU <>

fslv_glbl_int_type  = 4 	; 2=SWORD,4=SDWORD,8=SQWORD
fslv_glbl_real_type = 8 	; 4=REAL4,8=REAL8,10=REAL10

fslv_lcl_real_type = fslv_glbl_real_type	;-fSlv,fSolve
fslv_lcl_int_type  = fslv_glbl_int_type		;/

fslv4_def_int_type = 4		;-fSlv4 
fslv4_def_real_type = 4		;/
fslv8_def_int_type = 4		;-fSlv8
fslv8_def_real_type = 8		;/
fslv10_def_int_type = 4		;-fSlv10
fslv10_def_real_type = 10	;/

@fslv4_def_int_type = 4		;-@fSlv4()
@fslv4_def_real_type = 4    ;/
@fslv8_def_int_type = 4     ;-@fSlv8()
@fslv8_def_real_type = 8    ;/
@fslvI_def_int_type = 4     ;-@fSlvI()
@fslvI_def_real_type = 8    ;/
@fslvI64_def_int_type = 4   ;-@fSlvI64()
@fslvI64_def_real_type = 8  ;/

fsTSA_glbl_int_type = 4
fsTSA_glbl_real_type = 4

fcmp_def_int_type = fslv_glbl_int_type
fcmp_def_real_type = fslv_glbl_real_type

fSolve macro expr:VARARG

	IFB FSBE_ENTITY
		EXITM
	ELSEIF fslv_glb_error
		EXITM
	ENDIF

	fslv_nc=fslv_nc+1
	fslvm_pos INSTR 1,<&expr>,<=>
	fslvm_size SIZESTR <&expr>
	IFE fslvm_pos
		.err <missing '='>
		EXITM
	ELSEIF fslvm_size LE 1
		.err <invalid argument>
		EXITM
	ELSEIF fslvm_pos EQ 1
		fslvm_dest TEXTEQU <>
		fslvm_expr TEXTEQU <&expr>
		;fslvm_expr SUBSTR <&expr>,2
	ELSE
		fslvm_dest SUBSTR <&expr>,1,fslvm_pos-1
		fslvm_dest TEXTEQU @TrimStr(%fslvm_dest)
		fslvm_expr TEXTEQU <&expr>
		;fslvm_expr SUBSTR <&expr>,fslvm_pos+1
	ENDIF

	IFNB fslvm_dest
		IFIDNI @TrimStr(<%fslvm_expr>),<st0>
			fst fslvm_dest
			EXITM
		ENDIF
	ELSE
		IF FSBE_FNC(%FSBE_ENTITY,%FSEN_REQ_RETVAL)
			.err <destination operand required>
		ENDIF
	ENDIF
	
	fslv_extract_attributes %fslvm_expr
	IF fsea_found NE 0
		fslvm_expr TEXTEQU fsea_expr
		fslv_current_attributes TEXTEQU fsea_attributes
		fslv_attributes? = 1
		fslv_local_type_from_attributes fslv_glbl_real_type,fslv_glbl_int_type,%fsea_attributes
	ELSE
		fslv_attributes? = 0
		fslv_lcl_real_type = fslv_glbl_real_type
		fslv_lcl_int_type  = fslv_glbl_int_type
	ENDIF
	ll_MathTokenize SlvPath,SubExpr,%fslvm_expr
	;ll_DebugOut SlvPath,2,8,1,width=15,<<ralign=1,tmt=1,width=30>>
	IF tmt_error
		fslv_glb_error = 1
		fslv_glb_error_msg CATSTR <PARSER: >,tmt_err_txt
		.err fslv_glb_error_msg
		EXITM
	ENDIF
	ll_fSlv SlvPath,SubExpr
	IF fslv_glb_error
		.err fslv_glb_error_msg
		EXITM
	ENDIF

endm
fSlv EQU fSolve

fSlv4 macro expr:VARARG

	IFB FSBE_ENTITY
		EXITM
	ELSEIF fslv_glb_error
		EXITM
	ENDIF

	fslv4_nc=fslv4_nc+1
	fslvm_pos INSTR 1,<&expr>,<=>
	fslvm_size SIZESTR <&expr>
	IFE fslvm_pos
		.err <missing '='>
		EXITM
	ELSEIF fslvm_size LE 1
		.err <invalid argument>
		EXITM
	ELSEIF fslvm_pos EQ 1
		fslvm_dest TEXTEQU <>
		fslvm_expr TEXTEQU <&expr>
		;fslvm_expr SUBSTR <&expr>,2
	ELSE
		fslvm_dest SUBSTR <&expr>,1,fslvm_pos-1
		fslvm_dest TEXTEQU @TrimStr(%fslvm_dest)
		fslvm_expr TEXTEQU <&expr>
		;fslvm_expr SUBSTR <&expr>,fslvm_pos+1
	ENDIF
	
	IFNB fslvm_dest
		IFIDNI @TrimStr(%fslvm_expr),<st0>
			fst fslvm_dest
			EXITM
		ENDIF
	ELSE
		IF FSBE_FNC(%FSBE_ENTITY,%FSEN_REQ_RETVAL)
			.err <destination operand required>
		ENDIF
	ENDIF

	fslv_extract_attributes %fslvm_expr
	IF fsea_found NE 0
		fslvm_expr TEXTEQU fsea_expr
		fslv_current_attributes TEXTEQU fsea_attributes
		fslv_attributes? = 1
		fslv_local_type_from_attributes fslv4_def_real_type,fslv4_def_int_type,%fsea_attributes
	ELSE
		fslv_attributes? = 0
		fslv_lcl_real_type = fslv4_def_real_type
		fslv_lcl_int_type  = fslv4_def_int_type
	ENDIF
	ll_MathTokenize SlvPath,SubExpr,%fslvm_expr
	IF tmt_error
		fslv_glb_error = 1
		fslv_glb_error_msg CATSTR <PARSER: >,tmt_err_txt
		.err fslv_glb_error_msg
		EXITM
	ENDIF

	ll_fSlv SlvPath,SubExpr
	IF fslv_glb_error
		.err fslv_glb_error_msg
		EXITM
	ENDIF
endm

fSlv8 macro expr:VARARG

	IFB FSBE_ENTITY
		EXITM
	ELSEIF fslv_glb_error
		EXITM
	ENDIF

	fslv8_nc=fslv8_nc+1
	fslvm_pos INSTR 1,<&expr>,<=>
	fslvm_size SIZESTR <&expr>
	IFE fslvm_pos
		.err <missing '='>
		EXITM
	ELSEIF fslvm_size LE 1
		.err <invalid argument>
		EXITM
	ELSEIF fslvm_pos EQ 1
		fslvm_dest TEXTEQU <>
		fslvm_expr TEXTEQU <&expr>
		;fslvm_expr SUBSTR <&expr>,2
	ELSE
		fslvm_dest SUBSTR <&expr>,1,fslvm_pos-1
		fslvm_dest TEXTEQU @TrimStr(%fslvm_dest)
		fslvm_expr TEXTEQU <&expr>
		;fslvm_expr SUBSTR <&expr>,fslvm_pos+1
	ENDIF
	
	IFNB fslvm_dest
		IFIDNI @TrimStr(%fslvm_expr),<st0>
			fst fslvm_dest
			EXITM
		ENDIF
	ELSE
		IF FSBE_FNC(%FSBE_ENTITY,%FSEN_REQ_RETVAL)
			.err <destination operand required>
		ENDIF
	ENDIF

	fslv_extract_attributes %fslvm_expr
	IF fsea_found NE 0
		fslvm_expr TEXTEQU fsea_expr
		fslv_current_attributes TEXTEQU fsea_attributes
		fslv_attributes? = 1
		fslv_local_type_from_attributes fslv8_def_real_type,fslv8_def_int_type,%fsea_attributes
	ELSE
		fslv_attributes? = 0
		fslv_lcl_real_type = fslv8_def_real_type
		fslv_lcl_int_type  = fslv8_def_int_type
	ENDIF
	
	ll_MathTokenize SlvPath,SubExpr,%fslvm_expr
	IF tmt_error
		fslv_glb_error = 1
		fslv_glb_error_msg CATSTR <PARSER: >,tmt_err_txt
		.err fslv_glb_error_msg
		EXITM
	ENDIF

	ll_fSlv SlvPath,SubExpr
	IF fslv_glb_error
		.err fslv_glb_error_msg
		EXITM
	ENDIF
endm

fSlv10 macro expr:VARARG

	IFB FSBE_ENTITY
		EXITM
	ELSEIF fslv_glb_error
		EXITM
	ENDIF

	fslv10_nc=fslv10_nc+1
	fslvm_pos INSTR 1,<&expr>,<=>
	fslvm_size SIZESTR <&expr>
	IFE fslvm_pos
		.err <missing '='>
		EXITM
	ELSEIF fslvm_size LE 1
		.err <invalid argument>
		EXITM
	ELSEIF fslvm_pos EQ 1
		fslvm_dest TEXTEQU <>
		fslvm_expr TEXTEQU <&expr>
		;fslvm_expr SUBSTR <&expr>,2
	ELSE
		fslvm_dest SUBSTR <&expr>,1,fslvm_pos-1
		fslvm_dest TEXTEQU @TrimStr(%fslvm_dest)
		fslvm_expr TEXTEQU <&expr>
		;fslvm_expr SUBSTR <&expr>,fslvm_pos+1
	ENDIF
	
	IFNB fslvm_dest
		IFIDNI @TrimStr(%fslvm_expr),<st0>
			fst fslvm_dest
			EXITM
		ENDIF
	ELSE
		IF FSBE_FNC(%FSBE_ENTITY,%FSEN_REQ_RETVAL)
			.err <destination operand required>
		ENDIF
	ENDIF

	fslv_extract_attributes %fslvm_expr
	IF fsea_found NE 0
		fslvm_expr TEXTEQU fsea_expr
		fslv_current_attributes TEXTEQU fsea_attributes
		fslv_attributes? = 1
		fslv_local_type_from_attributes fslv10_def_real_type,fslv10_def_int_type,%fsea_attributes
	ELSE
		fslv_attributes? = 0
		fslv_lcl_real_type = fslv10_def_real_type
		fslv_lcl_int_type  = fslv10_def_int_type
	ENDIF
	
	ll_MathTokenize SlvPath,SubExpr,%fslvm_expr
	IF tmt_error
		fslv_glb_error = 1
		fslv_glb_error_msg CATSTR <PARSER: >,tmt_err_txt
		.err fslv_glb_error_msg
		EXITM
	ENDIF

	ll_fSlv SlvPath,SubExpr
	IF fslv_glb_error
		.err fslv_glb_error_msg
		EXITM
	ENDIF
endm

@fSlv4 macro expr:VARARG
LOCAL r4lbl

	IFB FSBE_ENTITY
		EXITM fslv_retval@error(REAL4)
	ELSEIF fslv_glb_error
		EXITM fslv_retval@error(REAL4)
	ENDIF

	@fslv4_nc=@fslv4_nc+1
	fslvm_pos INSTR 1,<&expr>,<=>
	fslvm_size SIZESTR <&expr>
	IF fslvm_pos EQ 0
		fslvm_dest TEXTEQU <>
		fslvm_expr TEXTEQU <&expr>
	ELSE
		fslvm_dest SUBSTR <&expr>,1,fslvm_pos-1
		fslvm_dest TEXTEQU @TrimStr(%fslvm_dest)
		fslvm_expr TEXTEQU <&expr>
		;fslvm_expr SUBSTR <&expr>,fslvm_pos+1
		IFB fslvm_dest
			.err <missing lvalue: '='>
			EXITM <0>
		ENDIF
	ENDIF

	fslv_extract_attributes %fslvm_expr
	IF fsea_found NE 0
		fslvm_expr TEXTEQU fsea_expr
		fslv_current_attributes TEXTEQU fsea_attributes
		fslv_attributes? = 1
		fslv_local_type_from_attributes @fslv4_def_real_type,@fslv4_def_int_type,%fsea_attributes
	ELSE
		fslv_attributes? = 0
		fslv_lcl_real_type = @fslv4_def_real_type
		fslv_lcl_int_type  = @fslv4_def_int_type
	ENDIF

	ll_MathTokenize SlvPath,SubExpr,%fslvm_expr
	IF tmt_error
		fslv_glb_error = 1
		fslv_glb_error_msg CATSTR <PARSER: >,tmt_err_txt
		.err fslv_glb_error_msg
		EXITM fslv_retval@error(REAL4)
	ENDIF
	
	FSBE_PREPARE_ALLOC r4lbl,REAL4,4
	
	ll_fSlv SlvPath,SubExpr,1,0,4
	IF fslv_glb_error
		.err fslv_glb_error_msg
		EXITM fslv_retval@error(REAL4)
	ENDIF
	EXITM lfs_retval
endm

@fSlv8 macro expr:VARARG
LOCAL r8lbl

	IFB FSBE_ENTITY
		EXITM fslv_retval@error(REAL8)
	ELSEIF fslv_glb_error
		EXITM fslv_retval@error(REAL8)
	ENDIF

	@fslv8_nc=@fslv8_nc+1
	fslvm_pos INSTR 1,<&expr>,<=>
	fslvm_size SIZESTR <&expr>
	IF fslvm_pos EQ 0
		fslvm_dest TEXTEQU <>
		fslvm_expr TEXTEQU <&expr>
	ELSE
		fslvm_dest SUBSTR <&expr>,1,fslvm_pos-1
		fslvm_dest TEXTEQU @TrimStr(%fslvm_dest)
		fslvm_expr TEXTEQU <&expr>
		;fslvm_expr SUBSTR <&expr>,fslvm_pos+1
		IFB fslvm_dest
			.err <missing lvalue: '='>
			EXITM <0>
		ENDIF
	ENDIF

	fslv_extract_attributes %fslvm_expr
	IF fsea_found NE 0
		fslvm_expr TEXTEQU fsea_expr
		fslv_current_attributes TEXTEQU fsea_attributes
		fslv_attributes? = 1
		fslv_local_type_from_attributes @fslv8_def_real_type,@fslv8_def_int_type,%fsea_attributes
	ELSE
		fslv_attributes? = 0
		fslv_lcl_real_type = @fslv8_def_real_type
		fslv_lcl_int_type  = @fslv8_def_int_type
	ENDIF

	ll_MathTokenize SlvPath,SubExpr,%fslvm_expr
	IF tmt_error
		fslv_glb_error = 1
		fslv_glb_error_msg CATSTR <PARSER: >,tmt_err_txt
		.err fslv_glb_error_msg
		EXITM fslv_retval@error(REAL8)
	ENDIF

	FSBE_PREPARE_ALLOC r8lbl,REAL8,8
	ll_fSlv SlvPath,SubExpr,1,0,8
	IF fslv_glb_error
		.err fslv_glb_error_msg
		EXITM fslv_retval@error(REAL8)
	ENDIF
	EXITM lfs_retval
endm

@fSlvI macro expr:VARARG
LOCAL dwlbl

	IFB FSBE_ENTITY
		EXITM fslv_retval@error(SDWORD)
	ELSEIF fslv_glb_error
		EXITM fslv_retval@error(SDWORD)
	ENDIF

	@fslvI_nc=@fslvI_nc+1
	fslvm_pos INSTR 1,<&expr>,<=>
	fslvm_size SIZESTR <&expr>
	IF fslvm_pos EQ 0
		fslvm_dest TEXTEQU <>
		fslvm_expr TEXTEQU <&expr>
	ELSE
		fslvm_dest SUBSTR <&expr>,1,fslvm_pos-1
		fslvm_dest TEXTEQU @TrimStr(%fslvm_dest)
		fslvm_expr TEXTEQU <&expr>
		;fslvm_expr SUBSTR <&expr>,fslvm_pos+1
		IFB fslvm_dest
			.err <missing lvalue: '='>
			EXITM <0>
		ENDIF
	ENDIF

	fslv_extract_attributes %fslvm_expr
	IF fsea_found NE 0
		fslvm_expr TEXTEQU fsea_expr
		fslv_current_attributes TEXTEQU fsea_attributes
		fslv_attributes? = 1
		fslv_local_type_from_attributes @fslvI_def_real_type,@fslvI_def_int_type,%fsea_attributes
	ELSE
		fslv_attributes? = 0
		fslv_lcl_real_type = @fslvI_def_real_type
		fslv_lcl_int_type  = @fslvI_def_int_type
	ENDIF

	ll_MathTokenize SlvPath,SubExpr,%fslvm_expr
	IF tmt_error
		fslv_glb_error = 1
		fslv_glb_error_msg CATSTR <PARSER: >,tmt_err_txt
		.err fslv_glb_error_msg
		EXITM fslv_retval@error(SDWORD)
	ENDIF
	
	FSBE_PREPARE_ALLOC dwlbl,SDWORD,4
	ll_fSlv SlvPath,SubExpr,1,4,0
	IF fslv_glb_error
		.err fslv_glb_error_msg
		EXITM fslv_retval@error(SDWORD)
	ENDIF
	EXITM lfs_retval
endm
IF @WordSize EQ 8
@fSlvI64 macro expr:VARARG
LOCAL dqlbl

	IFB FSBE_ENTITY
		EXITM fslv_retval@error(SQWORD)
	ELSEIF fslv_glb_error
		EXITM fslv_retval@error(SQWORD)
	ENDIF

	@fslvI64_nc=@fslvI64_nc+1
	fslvm_pos INSTR 1,<&expr>,<=>
	fslvm_size SIZESTR <&expr>
	IF fslvm_pos EQ 0
		fslvm_dest TEXTEQU <>
		fslvm_expr TEXTEQU <&expr>
	ELSE
		fslvm_dest SUBSTR <&expr>,1,fslvm_pos-1
		fslvm_dest TEXTEQU @TrimStr(%fslvm_dest)
		fslvm_expr TEXTEQU <&expr>
		IFB fslvm_dest
			.err <missing lvalue: '='>
			EXITM <0>
		ENDIF
	ENDIF

	fslv_extract_attributes %fslvm_expr
	IF fsea_found NE 0
		fslvm_expr TEXTEQU fsea_expr
		fslv_current_attributes TEXTEQU fsea_attributes
		fslv_attributes? = 1
		fslv_local_type_from_attributes @fslvI64_def_real_type,@fslvI64_def_int_type,%fsea_attributes
	ELSE
		fslv_attributes? = 0
		fslv_lcl_real_type = @fslvI64_def_real_type
		fslv_lcl_int_type  = @fslvI64_def_int_type
	ENDIF

	ll_MathTokenize SlvPath,SubExpr,%fslvm_expr
	IF tmt_error
		fslv_glb_error = 1
		fslv_glb_error_msg CATSTR <PARSER: >,tmt_err_txt
		.err fslv_glb_error_msg
		EXITM fslv_retval@error(SQWORD)
	ENDIF
	
	FSBE_PREPARE_ALLOC dqlbl,SQWORD,8
	ll_fSlv SlvPath,SubExpr,1,8,0
	IF fslv_glb_error
		.err fslv_glb_error_msg
		EXITM fslv_retval@error(SQWORD)
	ENDIF
	EXITM lfs_retval
endm
ENDIF

@fSlvI_T macro expr:VARARG
	IF @WordSize EQ 8
		EXITM @fSlvI64(expr)
	ELSE
		EXITM @fSlvI(expr)
	ENDIF
endm
@fSlvI_t EQU @fSlvI_T


fSlvTLS macro name:=<fslv_tls>,cb:=<4*@WordSize>
	IFB FSBE_ENTITY
		EXITM <&name&[1]:BYTE>
	ELSEIF fslv_glb_error
		EXITM <&name&[1]:BYTE>
	ENDIF
%	IFDEF fslv_tls_name
		.err <TLS duplication>
	ELSE
		fslv_tls_curr_used = 0 ; 04/10/2020 HSE 
		fslv_tls_alloc_line = @Line
		fslv_tls_cb = FSBE_FNC(%FSBE_ENTITY,%FSEN_TLS_SIZE)
		IF fslv_tls_cb NE 0 AND fslv_tls_cb GT cb
			fslv_tls_cb = (fslv_tls_cb+15) AND (-16)
		ELSE
			fslv_tls_cb = (cb+15) AND (-16)
		ENDIF
		fslv_tls_name TEXTEQU @TrimStr(<&name>)
		EXITM @CatStr(<&name&[fslv_tls_cb]:BYTE>)
	ENDIF
endm

; instruction sets
FSIS_FPU	EQU 1		; function descriptor: requires FNCD_ISTCK
FSIS_SSE2	EQU 2		; function descriptor: requires FNCD_IREG
FSIS_SSE3	EQU 4		; "          "           "          "
FSIS_SSSE3	EQU 8		; ...
FSIS_SSE4	EQU 16
FSIS_SSE41	EQU (32 OR FSIS_SSE4)
FSIS_SSE42	EQU (64 OR FSIS_SSE4)
FSIS_SSE4A	EQU (128 OR FSIS_SSE4)
FSIS_AVX	EQU 256
FSIS_CVT16	EQU 512
FSIS_XOP	EQU 1024
FSIS_FMA3	EQU 2048
FSIS_FMA4	EQU 4096

fslv_nc = 0
fslv4_nc = 0
fslv8_nc = 0
fslv10_nc = 0
@fslv4_nc = 0
@fslv8_nc = 0
@fslvI_nc = 0
@fslvI64_nc = 0
@fEQ_nc = 0
@fNE_nc = 0
@faEQ_nc = 0
@fLT_nc = 0
@fLE_nc = 0
@fGT_nc = 0
@fGE_nc = 0
@fTest_nc = 0

FSLV_SUPPRESS_ALL_WARNINGS 	= 0
FSLV_SUPPRESS_TS_WARNIGNS 	= 0
FSLV_NO_FORMULA_OPTIMIZATION = 0

fslv_glb_flags = 0
fslv_glb_instrset = 0
fslv_current_attributes TEXTEQU <>
fslv_attributes? = 0
IF @WordSize EQ 4
	fslv_volatile_xmm_regs TEXTEQU <xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7>
ELSE
	fslv_volatile_xmm_regs TEXTEQU <xmm0,xmm1,xmm2,xmm3,xmm4,xmm5>
ENDIF
fslv_volatile_xmmregs_nesting = 0

fslv_regc_txt TEXTEQU <>
fslv_regc_val TEXTEQU <>

fslvTLS_curr_line = -1
fslv_tls_curr_used = 0
fslv_tls_name TEXTEQU <>
fslv_tls_cb = 0
fslv_tls_last TEXTEQU <>
fslv_tls_last_size = 0
fslv_tls_byte = 0
fslv_tls_alloc_line = 0

fSlvVolatileXmmRegs macro operation:=<add>,regs:VARARG
	IF @WordSize EQ 4
		fsvxr_regs TEXTEQU <xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7>
	ELSE
		fsvxr_regs TEXTEQU <xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7,xmm8,xmm9,xmm10,xmm11,xmm12,xmm13,xmm14,xmm15>
	ENDIF
	IFIDNI <&operation>,<add>
		IF @GetArgCount(regs) LE @WordSize*2
			FOR arg1,<regs>
				IF @MatchStrI(<&arg1>,0,%fsvxr_regs)
					IFE @MatchStrI(<&arg1>,0,%fslv_volatile_xmm_regs)
						IFB fslv_volatile_xmm_regs
							fslv_volatile_xmm_regs TEXTEQU <&arg1>
						ELSE
							fslv_volatile_xmm_regs TEXTEQU fslv_volatile_xmm_regs,<,>,<&arg1>
						ENDIF
					ENDIF
				ELSE
					.err <invalid register: arg1>
					EXITM
				ENDIF
			ENDM
		ELSE
			.err <to many registers>
		ENDIF
	ELSEIFIDNI <&operation>,<remove>
		IF @GetArgCount(regs) LE @WordSize*2
			FOR arg1,<regs>
				IF @MatchStrI(<&arg1>,0,%fsvxr_regs)
					ml71_wa_txt TEXTEQU <&arg1>
					fslv_volatile_xmm_regs TEXTEQU @RemoveArgI(%ml71_wa_txt,%fslv_volatile_xmm_regs) ; ML7.1: %arg1 -> XMMn -> XMM(n)
				ELSE
					.err <invalid register: arg1>
					EXITM
				ENDIF
			ENDM
		ELSE
			.err <to many registers to remove>
		ENDIF
		IFE @GetArgCount(%fslv_volatile_xmm_regs)
			.err <no volatile XMM registers left>
			fslv_volatile_xmm_regs TEXTEQU fsvxr_regs
		ENDIF
	ELSEIFIDNI <&operation>,<set>
		IFB <regs>
			.err <missing arguments>
		ELSE
			IF @GetArgCount(regs) LE @WordSize*2
				fslv_volatile_xmm_regs TEXTEQU <>
				FOR arg1,<regs>
					IF @MatchStrI(<&arg1>,0,%fsvxr_regs)
						IFE @MatchStrI(<&arg1>,0,%fslv_volatile_xmm_regs)
							IFB fslv_volatile_xmm_regs
								fslv_volatile_xmm_regs TEXTEQU <&arg1>
							ELSE
								fslv_volatile_xmm_regs TEXTEQU fslv_volatile_xmm_regs,<,>,<&arg1>
							ENDIF
						ELSE
							.err <register duplicated: arg1>
							EXITM
						ENDIF
					ELSE
						.err <invalid register: arg1>
						EXITM
					ENDIF
				ENDM
				EXITM
			ELSE
				.err <to many registers>
			ENDIF		
			IF @WordSize EQ 4
				fslv_volatile_xmm_regs TEXTEQU <xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7>
			ELSE
				fslv_volatile_xmm_regs TEXTEQU <xmm0,xmm1,xmm2,xmm3,xmm4,xmm5>
			ENDIF
		ENDIF
	ELSEIFIDNI <&operation>,<default>
		IFNB <regs>
			.err <invalid usage: no additinal arguments are allowed>
		ENDIF
		IF @WordSize EQ 4
			fslv_volatile_xmm_regs TEXTEQU <xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7>
		ELSE
			fslv_volatile_xmm_regs TEXTEQU <xmm0,xmm1,xmm2,xmm3,xmm4,xmm5>
		ENDIF
	ELSEIFIDNI <&operation>,<push>
		fslv_volatile_xmmregs_nesting = fslv_volatile_xmmregs_nesting + 1
		@CatStr(<fslv_volatile_xmmregs_stck_>,%fslv_volatile_xmmregs_nesting) TEXTEQU fslv_volatile_xmm_regs
	ELSEIFIDNI <&operation>,<pop>
		fslv_volatile_xmm_regs TEXTEQU @CatStr(<fslv_volatile_xmmregs_stck_>,%fslv_volatile_xmmregs_nesting)
		fslv_volatile_xmmregs_nesting = fslv_volatile_xmmregs_nesting - 1
	ELSE
		.err <invalid macro usage: &operation>
	ENDIF
endm

@IsVolatileXmmReg macro regName:req
	qivxmr_match = 0
%	FOR arg0,<fslv_volatile_xmm_regs>
		IFIDNI <&arg0>,<&regName>
			qivxmr_match = -1
			EXITM
		ENDIF
	ENDM
	EXITM %qivxmr_match
endm

fslv_volatile_gp_regs TEXTEQU <>
fslv_volatile_gprs = 0
fslv_volatile_regs_nesting = 0

FSVGPR_EAX	EQU 1
FSVGPR_EBX	EQU 2
FSVGPR_ECX	EQU 4
FSVGPR_EDX	EQU 8
FSVGPR_EDI	EQU 16
FSVGPR_ESI	EQU 32
FSVGPR_EBP	EQU 64
FSVGPR_ESP	EQU 128
FSVGPR_R8D	EQU 256
FSVGPR_R9D	EQU 512
FSVGPR_R10D	EQU 1024
FSVGPR_R11D	EQU 800h
FSVGPR_R12D	EQU 1000h
FSVGPR_R13D	EQU 2000h
FSVGPR_R14D	EQU 4000h
FSVGPR_R15D	EQU 8000h

fSlvVolatileGPReg macro operation:=<add>,regs:VARARG
	IFIDNI <&operation>,<add>
		IF @GetArgCount(regs) LE @WordSize*2
			FOR arg1,<regs>
				IF @IsGPReg(<&arg1>)
					fsvgpr_basereg TEXTEQU @GetBaseGPR()
					IFE @MatchStrI(%fsvgpr_basereg,0,%fslv_volatile_gp_regs)
						IFB fslv_volatile_gp_regs
							fslv_volatile_gp_regs TEXTEQU fsvgpr_basereg
						ELSE
							fslv_volatile_gp_regs TEXTEQU fslv_volatile_gp_regs,<,>,fsvgpr_basereg
						ENDIF
					ENDIF
				ELSE
					.err <invalid register: arg1>
					EXITM
				ENDIF
			ENDM
		ELSE
			.err <to many registers>
			EXITM
		ENDIF
	ELSEIFIDNI <&operation>,<remove>
		IF @GetArgCount(regs) LE @WordSize*2
			FOR arg1,<regs>
				IF @IsGPReg(<&arg1>)
					fslv_volatile_gp_regs TEXTEQU @RemoveArgI(@GetBaseGPR(),%fslv_volatile_gp_regs)
				ELSE
					.err <invalid register: arg1>
					EXITM
				ENDIF
			ENDM
		ELSE
			.err <to many registers to remove>
			EXITM
		ENDIF
	ELSEIFIDNI <&operation>,<set>
		IFB <regs>
			.err <missing arguments>
		ELSE
			IF @GetArgCount(regs) LE @WordSize*2
				fslv_volatile_gp_regs TEXTEQU <>
				FOR arg1,<regs>
					IF @IsGPReg(<&arg1>)
						fsvgpr_basereg TEXTEQU @GetBaseGPR()
						IFE @MatchStrI(<&arg1>,0,%fslv_volatile_gp_regs)
							IFB fslv_volatile_gp_regs
								fslv_volatile_gp_regs TEXTEQU fsvgpr_basereg
							ELSE
								fslv_volatile_gp_regs TEXTEQU fslv_volatile_gp_regs,<,>,fsvgpr_basereg
							ENDIF
						ELSE
							.err <register duplicated: arg1>
							EXITM
						ENDIF
					ELSE
						.err <invalid register: arg1>
						EXITM
					ENDIF
				ENDM
			ELSE
				.err <to many registers>
			ENDIF
			fslv_volatile_gp_regs TEXTEQU <>
		ENDIF
	ELSEIFIDNI <&operation>,<default>
		IFNB <regs>
			.err <invalid usage: no additinal arguments are allowed>
		ENDIF
		fslv_volatile_gp_regs TEXTEQU <>
	ELSEIFIDNI <&operation>,<push>
		fslv_volatile_regs_nesting = fslv_volatile_regs_nesting + 1
		@CatStr(<fslv_volatile_regs_stck_>,%fslv_volatile_regs_nesting) TEXTEQU fslv_volatile_gp_regs
	ELSEIFIDNI <&operation>,<pop>
		fslv_volatile_gp_regs TEXTEQU @CatStr(<fslv_volatile_regs_stck_>,%fslv_volatile_regs_nesting)
		fslv_volatile_regs_nesting = fslv_volatile_regs_nesting - 1
	ELSE
		.err <invalid macro usage: &operation>
		EXITM
	ENDIF
	fslv_volatile_gprs = 0
%	FOR arg0,<fslv_volatile_gp_regs>
		fslv_volatile_gprs = fslv_volatile_gprs OR @CatStr(<FSVGPR_>,@ToUpperCase(<&arg0>))
	ENDM
endm

@IsVolatileGPR macro regName:req
	qivgpr_match = 0
%	FOR arg0,<fslv_volatile_gp_regs>
		IFIDNI <&arg0>,<&regName>
			qivgpr_match = -1
			EXITM
		ENDIF
	ENDM
	EXITM %qivgpr_match
endm


; macroName: name of fSlv-macros, enclosed by brackets
; e.g.: fSlvSetDefaultPrecision <@fSlv4>,REAL10,SQWORD
fSlvSetPrecision macro macroName:req,realPrec,intPrec

	fssdp_int = 0
	fssdp_real = 0

	IFIDNI <&realPrec>,<REAL4>
		fssdp_real = 4
	ELSEIFIDNI <&realPrec>,<REAL8>
		fssdp_real = 8
	ELSEIFIDNI <&realPrec>,<REAL10>
		fssdp_real = 10
	ELSEIFNB <&realPrec>
		.err <invalid precision specifier: &realPrec>
		EXITM
	ENDIF
	IFIDNI <&intPrec>,<SWORD>
		fssdp_int = 2
	ELSEIFIDNI <&intPrec>,<SDWORD>
		fssdp_int = 4
	ELSEIFIDNI <&intPrec>,<QWORD>
		fssdp_int = 8
	ELSEIFIDNI <&intPrec>,<SQWORD>
		fssdp_int = 8
	ELSEIFNB <&intPrec>
		.err <invalid precision specifier: &intPrec>
		EXITM
	ENDIF
	fssdp_cmp=0
	IFIDN <&macroName>,<fSlv>
		IFNB <&realPrec>
			fslv_glbl_real_type = fssdp_real
		ENDIF
		IFNB <&intPrec>
			fslv_glbl_int_type = fssdp_int
		ENDIF
	ELSEIFIDN <&macroName>,<fSlv4>
		IFNB <&realPrec>
			fslv4_def_real_type = fssdp_real
		ENDIF
		IFNB <&intPrec>
			fslv4_def_int_type = fssdp_int
		ENDIF
	ELSEIFIDN <&macroName>,<fSlv8>
		IFNB <&realPrec>
			fslv8_def_real_type = fssdp_real
		ENDIF
		IFNB <&intPrec>
			fslv8_def_int_type = fssdp_int
		ENDIF
	ELSEIFIDN <&macroName>,<fSlv10>
		IFNB <&realPrec>
			fslv10_def_real_type = fssdp_real
		ENDIF
		IFNB <&intPrec>
			fslv10_def_int_type = fssdp_int
		ENDIF
	ELSEIFIDN <&macroName>,<@fSlv4>
		IFNB <&realPrec>
			@fslv4_def_real_type = fssdp_real
		ENDIF
		IFNB <&intPrec>
			@fslv4_def_int_type = fssdp_int
		ENDIF
	ELSEIFIDN <&macroName>,<@fSlv8>
		IFNB <&realPrec>
			@fslv8_def_real_type = fssdp_real
		ENDIF
		IFNB <&intPrec>
			@fslv8_def_int_type = fssdp_int
		ENDIF
	ELSEIFIDN <&macroName>,<@fSlvI>
		IFNB <&realPrec>
			@fslvI_def_real_type = fssdp_real
		ENDIF
		IFNB <&intPrec>
			@fslvI_def_int_type = fssdp_int
		ENDIF
	ELSEIFIDN <&macroName>,<fEQ>
		fssdp_cmp=1
	ELSEIFIDN <&macroName>,<fNE>
		fssdp_cmp=1
	ELSEIFIDN <&macroName>,<fGT>
		fssdp_cmp=1
	ELSEIFIDN <&macroName>,<fGE>
		fssdp_cmp=1
	ELSEIFIDN <&macroName>,<fLT>
		fssdp_cmp=1
	ELSEIFIDN <&macroName>,<fLE>
		fssdp_cmp=1
	ELSEIFIDN <&macroName>,<faEQ>
		fssdp_cmp=1
	ELSE
		.err <unknown macro name: macroName>
		EXITM
	ENDIF
	IF fssdp_cmp
		IFNB <&realPrec>
			fcmp_def_real_type = fssdp_real
		ENDIF
		IFNB <&intPrec>
			fcmp_def_int_type = fssdp_int
		ENDIF
	ENDIF
endm

fSlvLoadBackEnd macro name:req
	abb_cntr = 0
	abb_txt TEXTEQU <>
	abb_flg = 0
	WHILE abb_cntr LT FSBE_N_BACKENDS
		abb_backend_entity CATSTR <FSO_BACKEND_ENTITY_>,%abb_cntr
		IF FSBE_FNC(%abb_backend_entity,%FSEN_NAME) NE 0
			IFDEF fsbe_txt
				IFIDNI fsbe_txt,<&name>
					%_discard_ FSBE_FNC(%abb_backend_entity,%FSEN_MAPPING)
					fslv_glb_flags = fslv_glb_flags AND (NOT FSF_INSTRSET_MSK)
					IF fslv_glb_instrset AND FSIS_FPU
						fslv_glb_flags = fslv_glb_flags or FSF_USE_FPU
					ENDIF
					IF fslv_glb_instrset AND FSIS_SSE2
						fslv_glb_flags = fslv_glb_flags or FSF_USE_SSE2
					ENDIF
					abb_flg = 1
					EXITM
				ENDIF
			ELSE
			%	.err <faulty back end detected (Entity: abb_backend_entity)>
			ENDIF
		ENDIF
		abb_cntr = abb_cntr + 1
	ENDM
	IF abb_flg EQ 0
		.err <no matching back end found for: name>
	ENDIF
endm

FSF_USE_FPU			EQU 1	; use the FPU. This flag has a lower priority than FSF_USE_SSE2
FSF_USE_SSE2		EQU 2	; use SSE2 if possible. The FPU is also used, if necessary. (this is the default for x64) 
FSF_REUSE_BY_TYPE	EQU 4	; reuse contants only, if the types are equal -  this flag applies only to integer contants
FSF_CHECK_SQWORD	EQU 8	; check QWORD-constants for range (only decimal)
FSF_NO_INT_CONSTS	EQU 16	; do not create integer constants -> convert them to REAL4/8/10 constants
FSF_INSTRSET_MSK	EQU (FSF_USE_FPU or FSF_USE_SSE2)

;;FSF_USE_VCRT		EQU 
;;FSF_USE_VCRT_SSE2	EQU 

fSlvSetFlags macro flags:=<0>
	IF (flags AND FSF_INSTRSET_MSK) NE 0
		fslv_glb_flags = fslv_glb_flags OR flags
		IF @WordSize EQ 4
			IF (fslv_glb_flags AND FSF_USE_FPU) NE 0 AND (fslv_glb_flags AND FSF_USE_SSE2)
				ASSIGN_BEST_BACKEND x86,FPU,SSE2
			ELSEIF (fslv_glb_flags AND FSF_USE_FPU) NE 0
				ASSIGN_BEST_BACKEND x86,FPU
			ELSE
				ASSIGN_BEST_BACKEND x86
			ENDIF
		ELSE
			IF (fslv_glb_flags AND FSF_USE_FPU) NE 0 AND (fslv_glb_flags AND FSF_USE_SSE2)
				ASSIGN_BEST_BACKEND x64,FPU,SSE2
			ELSEIF (fslv_glb_flags AND FSF_USE_FPU) NE 0
				ASSIGN_BEST_BACKEND x64,FPU
			ELSE
				ASSIGN_BEST_BACKEND x64
			ENDIF
		ENDIF
	ELSE
		fslv_glb_flags = fslv_glb_flags OR flags
	ENDIF	
endm

fSlvRemoveFlags macro flags:=<0>
	IF (flags AND FSF_INSTRSET_MSK) NE 0
		fslv_glb_flags = fslv_glb_flags AND (NOT(flags))
		IF @WordSize EQ 4
			IF (fslv_glb_flags AND FSF_USE_FPU) NE 0 AND (fslv_glb_flags AND FSF_USE_SSE2)
				ASSIGN_BEST_BACKEND x86,FPU,SSE2
			ELSEIF (fslv_glb_flags AND FSF_USE_FPU) NE 0
				ASSIGN_BEST_BACKEND x86,FPU
			ELSE
				ASSIGN_BEST_BACKEND x86
			ENDIF
		ELSE
			IF (fslv_glb_flags AND FSF_USE_FPU) NE 0 AND (fslv_glb_flags AND FSF_USE_SSE2)
				ASSIGN_BEST_BACKEND x64,FPU,SSE2
			ELSEIF (fslv_glb_flags AND FSF_USE_FPU) NE 0
				ASSIGN_BEST_BACKEND x64,FPU
			ELSE
				ASSIGN_BEST_BACKEND x64
			ENDIF
		ENDIF
	ELSE
		fslv_glb_flags = fslv_glb_flags AND (NOT(flags))
	ENDIF
endm

fSlvSelectBackEnd macro args:VARARG
	IF @WordSize EQ 4
		IF @MatchStrI(<x86>,,args)
			ASSIGN_BEST_BACKEND args
		ELSE
			ASSIGN_BEST_BACKEND x86,args
		ENDIF
	ELSE
		IF @MatchStrI(<x64>,,args)
			ASSIGN_BEST_BACKEND args
		ELSE
			ASSIGN_BEST_BACKEND x64,args
		ENDIF
	ENDIF
endm

fSlvResetFlags macro
	fslv_glb_flags = 0
	LOAD_DEFAULT_BACKEND
endm

fSlvGetFlags macro
	EXITM %fslv_glb_flags
endm

fpuSetPrecision macro mem16:=<WORD ptr T_EXPR([esp-2],[rsp])>,prec:=<REAL8>
	IFIDNI <&prec>,<REAL4>
		fstcw mem16
		and mem16,0fCffh
		fldcw mem16
	ELSEIFIDNI <&prec>,<REAL8>
		fstcw mem16
		and mem16,0fCffh
		or mem16,0200h
		fldcw mem16
	ELSEIFIDNI <&prec>,<REAL10>
		fstcw mem16
		or mem16,0300h
		fldcw mem16
	ELSE
		.err <invalid precision specifier: &prec>
	ENDIF
endm

fpuSetRoundingMode macro mem16:=<WORD ptr T_EXPR([esp-2],[rsp])>,mode:=<nearest>
	IFIDNI <&mode>,<nearest>
		fstcw mem16
		and mem16,NOT 110000000000y
		fldcw mem16
	ELSEIFIDNI <&mode>,<down>
		fstcw mem16
		and mem16,NOT 110000000000y
		or mem16,010000000000y
		fldcw mem16
	ELSEIFIDNI <&mode>,<up>
		fstcw mem16
		and mem16,NOT 110000000000y
		or mem16,100000000000y
		fldcw mem16
	ELSEIFIDNI <&mode>,<truncate>
		fstcw mem16
		or mem16,110000000000y
		fldcw mem16
	ELSE
		.err <invalid rounding mode specifier: &mode>
	ENDIF
endm

fpuSetControlWord macro mem16:=<WORD ptr T_EXPR([esp-2],[rsp])>,prec:=<REAL8>,mode:=<nearest>
	fpuscw_prec_ormsk = 0
	fpuscw_prec_andmsk = 0
	
	IFIDNI <&prec>,<REAL4>
		fpuscw_prec_andmsk = 0300h
	ELSEIFIDNI <&prec>,<REAL8>
		fpuscw_prec_andmsk = 0300h
		fpuscw_prec_ormsk = 0200h
	ELSEIFIDNI <&prec>,<REAL10>
		fpuscw_prec_ormsk = 0300h
	ELSE
		.err <invalid precision specifier: &prec>
	ENDIF
	IFIDNI <&mode>,<nearest>
		fpuscw_prec_andmsk = fpuscw_prec_andmsk OR 110000000000y
	ELSEIFIDNI <&mode>,<down>
		fpuscw_prec_andmsk = fpuscw_prec_andmsk OR 110000000000y
		fpuscw_prec_ormsk = fpuscw_prec_ormsk OR 010000000000y
	ELSEIFIDNI <&mode>,<up>
		fpuscw_prec_andmsk = fpuscw_prec_andmsk OR 110000000000y
		fpuscw_prec_ormsk = fpuscw_prec_ormsk OR 100000000000y
	ELSEIFIDNI <&mode>,<truncate>
		fpuscw_prec_ormsk = fpuscw_prec_ormsk OR 110000000000y
	ELSE
		.err <invalid rounding mode specifier: &mode>
	ENDIF
	fstcw mem16
	IF fpuscw_prec_andmsk
		and mem16,NOT fpuscw_prec_andmsk
	ENDIF
	IF fpuscw_prec_ormsk
		or mem16,fpuscw_prec_ormsk
	ENDIF
	fldcw mem16
endm

ldl_expand macro _arg
	EXITM <&_arg>
endm

; load local
ldl macro args:VARARG
	FOR _arg,<&args>
		ldl_pos INSTR 1,<&_arg>,<=>
		IFE ldl_pos
			.err <syntax error: expect <=>>
			EXITM
		ENDIF
		ldl_var SUBSTR <&_arg>,1,ldl_pos-1
		ldl_value SUBSTR <&_arg>,ldl_pos+1
		ldl_value TEXTEQU @TrimStr(%ldl_value)

		IF @ScanForFlt(1,%ldl_value,blanks) EQ 0
			ldl_value TEXTEQU %ldl_value
		ENDIF

		IFE @ScanForFlt(1,%ldl_value,blanks)
		%	.err <not a number: ldl_value>
			EXITM
		ENDIF
		
	%	IF (TYPE ldl_var) EQ (TYPE REAL4)
			IF @WordSize EQ 4
				IF @InStr(1,<0 0.0>,%ldl_value) EQ 0
					push 123456789
					org $-4
					IF sff_type EQ 1
						REAL4 ldl_value
					ELSE
					%	REAL4 &ldl_value&.0
					ENDIF
					pop DWORD ptr ldl_var
				ELSE
					mov DWORD ptr ldl_var,0
				ENDIF
			ELSE
				IF sff_type NE 1
					mov ldl_var,@CatStr(%ldl_value,<.0>)
				ELSE
					mov ldl_var,ldl_value
				ENDIF
			ENDIF
	%	ELSEIF @IsIntegerType(%ldl_var) EQ 4
			IF sff_type NE 2
			%	.err <integer value expected: ldl_value>
				EXITM
			ENDIF
			mov ldl_var,ldl_value
	%	ELSEIF (TYPE ldl_var) EQ (TYPE REAL8)
			IF @WordSize EQ 4
				IF @InStr(1,<0 0.0>,%ldl_value) EQ 0
					push 123456789
					org $-4
					IF sff_type EQ 1
						REAL8 ldl_value
					ELSE
					%	REAL8 &ldl_value&.0
					ENDIF
					org $-4
					IF sff_type EQ 1
						REAL8 ldl_value
					ELSE
					%	REAL8 &ldl_value&.0
					ENDIF
					org $-8
					db 8Dh,49h,0		; lea ecx,[ecx+0] = nop
					db 068h				; push imm32
					org $+4
					pop DWORD ptr ldl_var+4
					pop DWORD ptr ldl_var
				ELSE
					mov DWORD ptr ldl_var,0
					mov DWORD ptr ldl_var+4,0
				ENDIF
			ELSE
				push rax
				IF sff_type NE 1
					mov rax,REAL8 ptr @CatStr(%ldl_value,<.0>)
				ELSE
					mov rax,REAL8 ptr ldl_value
				ENDIF
				mov ldl_var,rax
				pop rax
			ENDIF
	%	ELSEIF @IsIntegerType(%ldl_var) EQ 8
			IF sff_type NE 2
			%	.err <integer value expected: ldl_value>
				EXITM
			ENDIF
			IF @WordSize EQ 4
				IF @InStr(1,<0 0.0>,%ldl_value) EQ 0
					push 123456789
					org $-4
					QWORD ldl_value
					org $-4
					QWORD ldl_value
					org $-8
					db 8Dh,49h,0		; lea ecx,[ecx+0] = nop
					db 068h				; push imm32
					org $+4
					pop DWORD ptr ldl_var+4
					pop DWORD ptr ldl_var
				ELSE
					mov DWORD ptr ldl_var,0
					mov DWORD ptr ldl_var+4,0
				ENDIF
			ELSE
				push rax
				mov rax,ldl_value
				mov ldl_var,rax
				pop rax
			ENDIF
		ELSE
		%	.err <invalid typed variable: ldl_var - only REAL4/8,SDWORD and QWORD are supported>
			EXITM
		ENDIF

	ENDM
endm

;; mov REAL8 to mem64 using edx and eax
;llr8 macro Var,value
;	IFE @ScanForFlt(1,<&value>,blanks)
;		.err <not a number: value>
;		EXITM
;	ENDIF
;	IF (TYPE Var) NE (TYPE REAL8)
;		.err <invalid typed variable: Var>
;		EXITM
;	ENDIF
;	IF @InStr(1,<0 0.0>,<&value>) EQ 0
;		mov eax,-1
;		org $-4
;		IF sff_type EQ 1
;			REAL8 value
;		ELSE
;			REAL8 &value&.0
;		ENDIF
;		org $-4
;		IF sff_type EQ 1
;			REAL8 value
;		ELSE
;			REAL8 &value&.0
;		ENDIF
;		org $-8
;		db 8Dh,49h,0		; lea ecx,[ecx+0]
;		db 0bah				; mov edx,imm32
;		org $+4
;		mov DWORD ptr Var,eax
;		mov DWORD ptr Var+4,edx
;	ELSE
;		mov DWORD ptr Var,0
;		mov DWORD ptr Var+4,0
;	ENDIF
;endm

fSlvRegConst macro ConstName:req,Value:req
	IFNDEF fpu_regc_&ConstName&_type
		IF @ScanForFlt(1,<&Value>,blanks)
			fpu_regc_equ_&ConstName& TEXTEQU <&Value>
			fpu_regc_&ConstName&_type = sff_type
			EXITM
		ENDIF
		.err <ERR: fSlvRegConst(ConstName,Value): invalid value <&Value>>
	ELSE
		.err <ERR: fSlvRegConst(ConstName,Value): redefinition <&ConstName>>
	ENDIF
endm


fSlvRegExpr macro ExprName:req,nArgs:=<0>,Expression:VARARG
	
	IFB <&Expression>
		.err <_fSolveRegExpr(<&ExprName>,<%nArgs>,<&Expression>): blank argument>
		EXITM
	ENDIF
	
	fsre_cntr = 1
	fsre_argcnt = 0
	REPEAT nArgs
	%	IF @InStr(1,<&Expression>,@CatStr(<arg>,%fsre_cntr)) EQ 0
			fsre_argcnt = fsre_cntr
			EXITM
		ENDIF
		fsre_cntr = fsre_cntr + 1
	ENDM
	
	IF fsre_argcnt
		.err @CatStr(<fSlveRegExpr: missing argument !<arg>,%fsre_argcnt,<!>>)
		EXITM
	ENDIF
	
	FOR _arg,<&ExprName>
		fsre_name TEXTEQU <&_arg>
	ENDM

	@CatStr(<fSlv_Expr_>,%fsre_name) TEXTEQU <&Expression>	
	@CatStr(<fSlv_Expr_args_>,%fsre_name) = nArgs
	@CatStr(<fSlv_Expr_itr_>,%fsre_name) = 0
	@CatStr(<fSlv_Expr_sum_>,%fsre_name) = 0
	@CatStr(<fSlv_Expr_rest_>,%fsre_name) TEXTEQU <>
endm

fSlvRegRecursiveExpr macro ExprName:req,nArgs:=<1>,Rest:=<0>,Expression:VARARG
	
	IFB <&Expression>
		.err <_fSolveRegExpr(<&ExprName>,<%nArgs>,<&Expression>): blank argument>
		EXITM
	ENDIF
	
	fsre_argcnt = 0
	fsre_cntr = 1
	REPEAT nArgs
	%	IF @InStr(1,<&Expression>,@CatStr(<arg>,%fsre_cntr)) EQ 0
			fsre_argcnt = fsre_cntr
			EXITM
		ENDIF
		fsre_cntr = fsre_cntr + 1
	ENDM
	
	IF fsre_argcnt
		.err @CatStr(<fSlvRegRecursiveExpr: missing argument !<arg>,%fsre_argcnt,<!>>)
		EXITM
	ENDIF
	
	FOR _arg,<&ExprName>
		fsre_name TEXTEQU <&_arg>
	ENDM

	@CatStr(<fSlv_Expr_>,%fsre_name) TEXTEQU <&Expression>	
	@CatStr(<fSlv_Expr_args_>,%fsre_name) = nArgs
	@CatStr(<fSlv_Expr_itr_>,%fsre_name) = -1
	@CatStr(<fSlv_Expr_sum_>,%fsre_name) = 0
	@CatStr(<fSlv_Expr_rest_>,%fsre_name) TEXTEQU @TrimStr(<&Rest>)
endm

fSlvRegIteratedExpr macro ExprName:req,nArgs:=<1>,Rest:=<0>,Expression:VARARG
	
	IFB <&Expression>
		.err <_fSlvRegIteratedExpr(<&ExprName>,<%nArgs>,<&Expression>): blank argument>
		EXITM
	ENDIF
	

	fsre_cntr = 1
	REPEAT nArgs
	%	IF @InStr(1,<&Expression>,@CatStr(<arg>,%fsre_cntr)) EQ 0
			fsre_cntr = 0
			EXITM
		ENDIF
		fsre_cntr = fsre_cntr + 1
	ENDM
	
	IF fsre_cntr EQ 0 AND nArgs NE 0 AND fsre_cntr GT 1
		.err <_fSlvRegIteratedExpr: missing arguments (argX) in _Expression>
		EXITM
	ENDIF
	
	FOR _arg,<&ExprName>
		fsre_name TEXTEQU <&_arg>
	ENDM

	@CatStr(<fSlv_Expr_>,%fsre_name) TEXTEQU <&Expression>	
	@CatStr(<fSlv_Expr_args_>,%fsre_name) = nArgs
	@CatStr(<fSlv_Expr_itr_>,%fsre_name) = -1
	@CatStr(<fSlv_Expr_sum_>,%fsre_name) = -1
	@CatStr(<fSlv_Expr_rest_>,%fsre_name) TEXTEQU @TrimStr(<&Rest>)
endm

r4Is_Valid macro mem32:req
	IF @WordSize EQ 4
		push mem32
		and DWORD ptr [esp],7F800000h
		xor DWORD ptr [esp],7F800000h
		lea esp,[esp+4]
	ELSE
		push rax
		mov eax,mem32
		and eax,7F800000h
		xor eax,7F800000h
		pop rax
	ENDIF
endm
r4IsValid macro mem32:req
       r4Is_Valid &mem32
	EXITM <!ZERO?>
endm


r8Is_Valid macro mem64:req
	IF @WordSize EQ 4
		push DWORD ptr mem64+4
		and DWORD ptr [esp],7FF00000h
		xor DWORD ptr [esp],7FF00000h
		lea esp,[esp+4]
	ELSE
		push rax
		mov eax,DWORD ptr mem64+4
		and eax,7FF00000h
		xor eax,7FF00000h
		pop rax
	ENDIF
endm
r8IsValid macro mem64:req
       r8Is_Valid &mem64
	EXITM <!ZERO?>
endm


r4Is_NotNaN macro mem32:req
	IF @WordSize EQ 4
		push mem32
		and DWORD ptr [esp],7FF80000h
		xor DWORD ptr [esp],7FF80000h
		lea esp,[esp+4]
	ELSE
		push rax
		mov eax,mem32
		and eax,7F800000h
		xor eax,7F800000h
		pop rax
	ENDIF
endm
r4IsNotNaN macro mem32:req
       r4Is_NotNaN &mem32
	EXITM <!ZERO?>
endm

r8Is_NotNaN macro mem64:req
	IF @WordSize EQ 4
		push DWORD ptr mem64+4
		and DWORD ptr [esp],7FF80000h
		xor DWORD ptr [esp],7FF80000h
		lea esp,[esp+4]
	ELSE
		push rax
		mov eax,DWORD ptr mem64+4
		and eax,7FF80000h
		xor eax,7FF80000h
		pop rax
	ENDIF
endm
r8IsNotNaN macro mem64:req
       r8Is_NotNaN &mem64
	EXITM <!ZERO?>
endm


;/*
; * fcmp a,b
; * compare two numeric values, passed as variables or as immediate values, and set the
; * eflags according to the result (use ja,jb,jbe,jae,... ). The usable operands depends on the selected back end.
; * 
; * Example:
; *     fcmp r4Var1,1.0E-7
; *     jae xyz
; */
fcmp macro args:VARARG
	fcmpx <>,0,args
endm

fEQ macro args:VARARG ; a:req,b:req
	@fEQ_nc=@fEQ_nc+1
	fcmpx <e>,0,args
	EXITM fcmp_bool_byte
endm
fNE macro args:VARARG ; a:req,b:req
	@fNE_nc=@fNE_nc+1
	fcmpx <ne>,0,args
	EXITM fcmp_bool_byte
endm
fGT macro args:VARARG ; a:req,b:req
	@fGT_nc=@fGT_nc+1
	fcmpx <!a>,0,args
	EXITM fcmp_bool_byte
endm
fGE macro args:VARARG ; a:req,b:req
	@fGE_nc=@fGE_nc+1
	fcmpx <ae>,0,args
	EXITM fcmp_bool_byte
endm
fLT macro args:VARARG ; a:req,b:req
	@fLT_nc=@fLT_nc+1
	fcmpx <!b>,0,args
	EXITM fcmp_bool_byte
endm
fLE macro args:VARARG ; a:req,b:req
	@fLE_nc=@fLE_nc+1
	fcmpx <be>,0,args
	EXITM fcmp_bool_byte
endm

faEQ  macro args:VARARG ;a:req,b:req,factor:=<1.0E-2>
	@faEQ_nc=@faEQ_nc+1
	fcmpx <foo>,1,args
	EXITM fcmp_bool_byte
endm

fTest macro args:VARARG ;a:req,b:req,exp_diff:=<20>
	@fTest_nc=@fTest_nc+1
	fcmpx <zero test>,1,args
	EXITM fcmp_bool_byte
endm

fcmpx macro cmpSuffix,approxEQ:=<0>,args:VARARG ;cmpSuffix,approxEQ:=<0>,a:req,b:req,pByte
	IFB FSBE_ENTITY
		fcmp_bool_byte TEXTEQU fslv_retval@error(BOOLEAN_BYTE)
		EXITM
	ELSEIF fslv_glb_error
		fcmp_bool_byte TEXTEQU fslv_retval@error(BOOLEAN_BYTE)
		EXITM
	ENDIF
	lfs_nss = 0
	lfs_stack_cntr = 0
	
	fslv_lcl_real_type = fcmp_def_real_type
	fslv_lcl_int_type  = fcmp_def_int_type
	fslv_attributes? = 0
	fcmpx_args TEXTEQU <&args>
	fslv_extract_attributes <&args>
	IF fsea_found NE 0
		fcmpx_args TEXTEQU fsea_expr
		fslv_current_attributes TEXTEQU fsea_attributes
		fslv_attributes? = 1
		fslv_local_type_from_attributes fcmp_def_real_type,fcmp_def_int_type,%fsea_attributes
	ENDIF
	
	IFE FSBE_FNC(%FSBE_ENTITY,%FSEN_INIT,%__SMPLMATH__,-1)
		.err <can not initialize back end>
		fcmp_bool_byte TEXTEQU fslv_retval@error(BOOLEAN_BYTE)
		fslv_glb_error = 1
		EXITM
	ENDIF
	IF fslv_glb_error
		.err fslv_glb_error_msg
		fcmp_bool_byte TEXTEQU fslv_retval@error(BOOLEAN_BYTE)
		fslv_glb_error = 1
		EXITM
	ENDIF
	fcmpx_arg_mode = FSBE_FNC(%FSBE_ENTITY,%FSEN_CMP_SUPPORTED)
	IFE fcmpx_arg_mode
		.err <current back end does not support compare operations>
		fcmp_bool_byte TEXTEQU fslv_retval@error(BOOLEAN_BYTE)
		fslv_glb_error = 1
		EXITM
	ENDIF
	lfs_no_int_consts = FSBE_FNC(%FSBE_ENTITY,%FSEN_NO_INT_CONST)
	lfs_no_real_consts = FSBE_FNC(%FSBE_ENTITY,%FSEN_NO_REAL_CONST)
	
	IFNDEF fcmp_line
		fcmp_line = @Line
	ENDIF

	fcmpx_a TEXTEQU <>
	fcmpx_b TEXTEQU <>
	fcmpx_pByte TEXTEQU <>
	fcmpx_factor TEXTEQU <>
	fcmp_bool_byte TEXTEQU <>
	
	IFNB FSBE_ENUM_TKN
		FSBE_PROC %FSBE_ENUM_TKN,%FSB_ENUM_START,<>,0
	ENDIF
	fcmpx_cntr=0
%	FOR _arg,<fcmpx_args>
		IF fcmpx_cntr EQ 0
			fcmpx_a TEXTEQU <&_arg>
		ELSEIF fcmpx_cntr EQ 1
			fcmpx_b TEXTEQU <&_arg>
		ELSEIF fcmpx_cntr EQ 2
			IF approxEQ EQ 0
				fcmpx_pByte TEXTEQU <&_arg>
			ELSE
				fcmpx_factor TEXTEQU <&_arg>
			ENDIF
		ELSEIF approxEQ NE 0 AND fcmpx_cntr EQ 3
			fcmpx_pByte TEXTEQU <&_arg>
		ELSE
			.err <to many arguments>
			fcmp_bool_byte TEXTEQU fslv_retval@error(BOOLEAN_BYTE)
			fslv_glb_error = 1
			EXITM
		ENDIF
		IFNB FSBE_ENUM_TKN
			FSBE_PROC %FSBE_ENUM_TKN,%FSB_ENUM_TKN,<<&_arg>>,%TMT_TXT
		ENDIF
		fcmpx_cntr=fcmpx_cntr+1
	ENDM
	IFNB FSBE_ENUM_TKN
		FSBE_PROC %FSBE_ENUM_TKN,%FSB_ENUM_END,<>,0
	ENDIF
	IF fslv_glb_error
		EXITM
	ENDIF
	
	IF fcmpx_cntr LT 2
		.err <missing arguments>
		fcmp_bool_byte TEXTEQU fslv_retval@error(BOOLEAN_BYTE)
		fslv_glb_error = 1
		EXITM
	ELSEIFB <&cmpSuffix>
		IF fcmpx_cntr NE 2 AND approxEQ EQ 0
			.err <invalid number of arguments>
			fcmp_bool_byte TEXTEQU fslv_retval@error(BOOLEAN_BYTE)
			fslv_glb_error = 1
			EXITM
		ENDIF
	ENDIF
	
	IF approxEQ NE 0
		IFB fcmpx_factor
			IFIDN <&cmpSuffix>,<zero test>
				fcmpx_factor TEXTEQU <20>
			ELSE
				fcmpx_factor TEXTEQU <1.0E-2>
			ENDIF
		ENDIF
	ENDIF	

	IFB fcmpx_pByte
		IFNB <&cmpSuffix>
			IFE @fslv_TLS?()
				@TLS_warning
				fcmp_bool_byte TEXTEQU get_unique_lbl(<fcmp_bool_byte_>)
				IF fcmp_line NE @Line
					fcmp_line = @Line
					.data?
						align 4	
					.code
				ENDIF
				.data?
					fcmp_bool_byte db ?
				.code
			ELSE
				fcmp_bool_byte TEXTEQU @fslvTLS_line_alloc(BOOLEAN_BYTE)
			ENDIF
			IF fslv_glb_error
				.err fslv_glb_error_msg
				fcmp_bool_byte TEXTEQU fslv_retval@error(BOOLEAN_BYTE)
				EXITM
			ENDIF
		ENDIF
	ELSE
		.err <sorry, the feature pByte has been removed: instead share local stack space using fSlvTLS()>
		fcmp_bool_byte TEXTEQU fslv_retval@error(BOOLEAN_BYTE)
		fslv_glb_error = 1
		EXITM
	ENDIF
	
	IF FSBE_FNC(%FSBE_IS_TKN_CONST,%fcmpx_b)
		IF fcmpx_arg_mode EQ 1
			FSBE_PROC %FSBE_LOAD_CONST,0,%fsbeic_const_name
			fcmpx_b TEXTEQU <>
		ELSE
			fcmpx_b TEXTEQU FSBE_FNC(%FSBE_PREPARE_CONST,0,%fsbeic_const_name)
		ENDIF
	ELSE
		IF @ScanForFlt(1,%fcmpx_b,blanks) NE 0
			IF sff_type EQ 1
				fcmpx_b TEXTEQU @fSlvCreateConstant(get_unique_lbl(<fcmp_lbl1_>),%fcmpx_b,@CatStr(<REAL>,%fslv_lcl_real_type))
			ELSE
				fcmpx_b TEXTEQU @fSlvCreateConstant(get_unique_lbl(<fcmp_lbl1_>),%fcmpx_b,@SubStr(<SWORD SDWORD SQWORD>,%@SubStr(< 0107  14>,%fslv_lcl_int_type,2),%5+((fslv_lcl_int_type GT 2) AND 1)))
			ENDIF
		ENDIF
		IF fcmpx_arg_mode EQ 1
			FSBE_PROC %FSBE_LOAD_ARG,%fcmpx_b,0
			fcmpx_b TEXTEQU <>
		ELSE
			fcmpx_b TEXTEQU FSBE_FNC(%FSBE_PREPARE_ARG,%fcmpx_b,0)
		ENDIF
	ENDIF
	IF fslv_glb_error
		.err fslv_glb_error_msg
		EXITM
	ENDIF
	IFB fcmpx_b
		lfs_stack_cntr = lfs_stack_cntr + 1
	ENDIF
	
	IF FSBE_FNC(%FSBE_IS_TKN_CONST,%fcmpx_a)
		FSBE_PROC %FSBE_LOAD_CONST,0,%fsbeic_const_name
	ELSE
		IF @ScanForFlt(1,%fcmpx_a,blanks) NE 0
			IF sff_type EQ 1
				fcmpx_a TEXTEQU @fSlvCreateConstant(get_unique_lbl(<fcmp_lbl2_>),%fcmpx_a,@CatStr(<REAL>,%fslv_lcl_real_type))
			ELSE
				fcmpx_a TEXTEQU @fSlvCreateConstant(get_unique_lbl(<fcmp_lbl2_>),%fcmpx_a,@SubStr(<SWORD SDWORD SQWORD>,%@SubStr(< 0107  14>,%fslv_lcl_int_type,2),%5+((fslv_lcl_int_type GT 2) AND 1)))
			ENDIF
		ENDIF
		FSBE_PROC %FSBE_LOAD_ARG,%fcmpx_a,0
	ENDIF	
	IF fslv_glb_error
		.err fslv_glb_error_msg
		EXITM
	ENDIF
	lfs_stack_cntr = lfs_stack_cntr + 1
	
	IF approxEQ EQ 0
		FSBE_PROC %FSBE_CMP,<cmp>,<&cmpSuffix>,,%fcmp_bool_byte,%fcmpx_b
	ELSEIFIDN <&cmpSuffix>,<zero test>
		IF @ScanForFlt(1,%fcmpx_factor,blanks) NE 0
			IF sff_type EQ 1
				.err <fTest: invalid exp_diff: expect integer constant or [S]DWORD-variable/register>
				fslv_glb_error = 1
				EXITM
			ELSE
				fcmpx_tmpeq = lfs_no_int_consts
				lfs_no_int_consts = 0
				fcmpx_factor TEXTEQU @fSlvCreateConstant(get_unique_lbl(<fcmp_lbl3_>),%fcmpx_factor,SDWORD)
				lfs_no_int_consts = fcmpx_tmpeq
			ENDIF
		ENDIF
		IF @IsIntegerType(%fcmpx_factor) NE 4
			.err <fTest: invalid exp_diff: expect integer constant or [S]DWORD-variable/register>
			fslv_glb_error = 1
			EXITM
		ENDIF
		FSBE_PROC %FSBE_CMP,<<zero test>>,,%fcmpx_factor,%fcmp_bool_byte,%fcmpx_b
	ELSE
		IF @ScanForFlt(1,%fcmpx_factor,blanks) NE 0
			IF sff_type EQ 1
				fcmpx_factor TEXTEQU @fSlvCreateConstant(get_unique_lbl(<fcmp_lbl3_>),%fcmpx_factor,@CatStr(<REAL>,@SubStr(<   4   8 8>,%fslv_lcl_real_type,1)))
			ELSE
				.err <fEQ_approx: invalid !factor : expect REAL constant or variable>
				fslv_glb_error = 1
				EXITM
			ENDIF
		ENDIF
		IFE @IsRealType(%fcmpx_factor) XOR 2 ; match 4 or 8
			.err <fEQ_approx: invalid !factor : expect REAL constant or variable>
			fslv_glb_error = 1
			EXITM
		ENDIF
		FSBE_PROC %FSBE_CMP,<<approx. eq.>>,,%fcmpx_factor,%fcmp_bool_byte,%fcmpx_b
	ENDIF
	%_discard_ FSBE_FNC(%FSBE_ENTITY,%FSEN_END)
	IF fslv_glb_error
		.err fslv_glb_error_msg
	ENDIF
endm

IF @WordSize NE 8
	R4 macro value:req
		EXITM R??(4,value)
	endm
	
	R8 macro value:req
		EXITM R??(8,value)
	endm
	
	R10 macro value:req
		EXITM R??(10,value)
	endm
	_R4 macro value:req
		EXITM R??(4,value)
	endm
	
	_R8 macro value:req
		EXITM R??(8,value)
	endm
	
	_R10 macro value:req
		EXITM R??(10,value)
	endm
;	_R4 EQU R4   ; MASM: EQUs for macro functions can cause errors
;	_R8 EQU R8
;	_R10 EQU R10
ELSE
	_R4 macro value:req
		EXITM R??(4,value)
	endm
	
	_R8 macro value:req
		EXITM R??(8,value)
	endm
	
	_R10 macro value:req
		EXITM R??(10,value)
	endm
ENDIF

R?? macro num:req,value:req
	??R&num&_value TEXTEQU <&value>
	IF @ScanForFlt(1,<&value>,blanks) EQ 0
		.err <R&num&: invalid constant: value>
	ELSE
		IF sff_type NE 1
			IF (sff_flag AND SFF_HEX_SUFFIX) NE 0
				.err <R&num&: floating pointer initializer expected>
				EXITM <0>
			ENDIF
			??R&num&_value CATSTR ??R&num&_value,<.0>
		ENDIF
		EXITM @fSlvCreateConstant(get_unique_lbl(<??R&num&_CONST_>),%??R&num&_value,REAL&num&)
	ENDIF
	EXITM <0>
endm

fSlvStatistics macro
	%_discard_ FSBE_FNC(%FSBE_ENTITY,%FSEN_NAME,<&arg>)
	
	%echo
	%echo fslv-Statistics @ @FileCur::line(@CatStr(%@Line))
	%echo current back end: fsbe_txt
	%echo ------ macro calls ------
	%echo calls total: @CatStr(%fslv_nc+fslv4_nc+fslv8_nc+fslv10_nc+@fslv4_nc+@fslv8_nc+@fslvI_nc+@fEQ_nc+@faEQ_nc+@fNE_nc+@fLT_nc+@fLE_nc+@fGT_nc+@fGE_nc+@fslvI64_nc)
	%echo 
	%echo fSolve:  @CatStr(%fslv_nc)
	%echo fSlv4:   @CatStr(%fslv4_nc)
	%echo fSlv8:   @CatStr(%fslv8_nc)
	%echo fSlv10:  @CatStr(%fslv10_nc)
	%echo @fSlv4:  @CatStr(%@fslv4_nc)
	%echo @fSlv8:  @CatStr(%@fslv8_nc)
	%echo @fSlvI:  @CatStr(%@fslvI_nc)
	IF @WordSize EQ 8
	%echo @fSlvI64:@CatStr(%@fslvI64_nc)
	ENDIF
	%echo fEQ:     @CatStr(%@fEQ_nc)
	%echo faEQ:    @CatStr(%@faEQ_nc)
	%echo fNE:     @CatStr(%@fNE_nc)
	%echo fLT:     @CatStr(%@fLT_nc)
	%echo fLE:     @CatStr(%@fLE_nc)
	%echo fGT:     @CatStr(%@fGT_nc)
	%echo fGE:     @CatStr(%@fGE_nc)
	%echo 
	%echo ------ constants --------
	%echo created constants: @CatStr(%fscc_nconsts)
	%echo reused constants:  @CatStr(%fscc_nreused)
	%echo 
	%echo created SWORD:  @CatStr(%fscc_nc_SWORD)
	%echo created SDWORD: @CatStr(%fscc_nc_SDWORD)
	%echo created SQWORD: @CatStr(%fscc_nc_SQWORD)
	%echo created REAL4:  @CatStr(%fscc_nc_REAL4)
	%echo created REAL8:  @CatStr(%fscc_nc_REAL8)
	%echo created REAL10: @CatStr(%fscc_nc_REAL10)
	%echo
	%echo reused SWORD:   @CatStr(%fscc_nr_SWORD)
	%echo reused SDWORD:  @CatStr(%fscc_nr_SDWORD)
	%echo reused SQWORD:  @CatStr(%fscc_nr_SQWORD)
	%echo reused REAL4:   @CatStr(%fscc_nr_REAL4)
	%echo reused REAL8:   @CatStr(%fscc_nr_REAL8)
	%echo reused REAL10:  @CatStr(%fscc_nr_REAL10)
	%echo ------ end statistics ---
	%echo
endm

; test argument for [{r4,r8,i4,i8,iT}]{:,=} formular
;    ":" => result = integer
;    "=" => result = real
@fSlvTSA macro arg
	IFB <&arg>
		EXITM <&arg>
	ENDIF
	fstsa_pos1 INSTR 1,<&arg>,<">
	fstsa_pos2 INSTR 1,<&arg>,<'>
	IF fstsa_pos1 NE 0 OR fstsa_pos2 NE 0
		EXITM <&arg>
	ENDIF
	fstsa_arg TEXTEQU @TrimStr(<&arg>)
	fstsa_pos1 INSTR 1,fstsa_arg,<=>
	fstsa_pos2 INSTR 1,fstsa_arg,<:>
	IF fstsa_pos1 EQ 0 AND fstsa_pos2 EQ 0
		EXITM <&arg>
	ENDIF
	IF fstsa_pos1 NE 0 AND fstsa_pos2 NE 0
		.err <syntax error in argument: invalid formula>
		EXITM <0>
	ENDIF
	IF fstsa_pos1 NE 0
		fstsa_pos = fstsa_pos1
		fstsa_real = 1
	ELSE
		fstsa_pos = fstsa_pos2
		fstsa_real = 0
	ENDIF
	fstsa_size SIZESTR fstsa_arg
	IF fstsa_size EQ fstsa_pos
		.err <invalid expression as argument>
		EXITM <0>
	ENDIF
	IF fstsa_pos NE 1
		fstsa_expr SUBSTR fstsa_arg,fstsa_pos+1
		fstsa_dest SUBSTR fstsa_arg,1,fstsa_pos-1
		IF @MatchStrI(%fstsa_dest,1,r4,r8,i4,i8)
			IF msi_index LE 2
				fstsa_real = 1
				fsTSA_lcl_real_type = @GetArgByIndex(%msi_index-1,,4,8)
			ELSEIF msi_index LE 4
				fstsa_real = 0
				fsTSA_lcl_int_type = @GetArgByIndex(%msi_index-2,,4,8)
			ELSE
				fstsa_real = 0
				fsTSA_lcl_int_type = @WordSize
			ENDIF
			fstsa_dest TEXTEQU <>
		ELSE
			fsTSA_lcl_int_type = fsTSA_glbl_int_type
			fsTSA_lcl_real_type = fsTSA_glbl_real_type
		ENDIF
	ELSE
		fstsa_expr SUBSTR fstsa_arg,fstsa_pos+1
		fstsa_dest TEXTEQU <>
		fsTSA_lcl_int_type = fsTSA_glbl_int_type
		fsTSA_lcl_real_type = fsTSA_glbl_real_type
	ENDIF
	
	IFNB fstsa_dest
		fstsa_expr CATSTR fstsa_dest,<=>,fstsa_expr
	ENDIF
	IF fstsa_real
		IF fsTSA_lcl_real_type EQ 4
			EXITM @fSlv4(%fstsa_expr)
		ELSEIF fsTSA_lcl_real_type EQ 8
			EXITM @fSlv8(%fstsa_expr)
		ELSE
			.err <@fSlvTSA: data type not supported>
			EXITM <0>
		ENDIF
	ELSE
		IF fsTSA_lcl_int_type EQ 4
			EXITM @fSlvI(%fstsa_expr)
		ELSEIF fsTSA_lcl_int_type EQ 8 AND @WordSize EQ 8
			EXITM @fSlvI64(%fstsa_expr)
		ELSE
			.err <@fSlvTSA: data type not supported>
			EXITM <0>
		ENDIF
	ENDIF	
endm
