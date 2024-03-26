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

FSO_N_STACK = 0

;
; this macro creates code using the current selected back end
;
ll_fSlv macro SlvPath,SubExpr,bFunction:=<0>,retval_int_type:=<0>,retval_real_type:=<0>
	
	IFNDEF lfs_lbl
		lfs_lbl = 0
	ENDIF
	
	lfs_stack_cntr = 0
	lfs_flg = 0
	lfs_copy_only = 0
	lfs_copy_src TEXTEQU <>
	lfs_retval TEXTEQU <>
	
	iml_create StkSlot
	lfs_nss = 0

	IFE FSBE_FNC(%FSBE_ENTITY,%FSEN_INIT,%__SMPLMATH__,0)
		lfs_error = 1
		EXITM
	ENDIF
	lfs_no_int_consts = FSBE_FNC(%FSBE_ENTITY,%FSEN_NO_INT_CONST)
	lfs_no_real_consts = FSBE_FNC(%FSBE_ENTITY,%FSEN_NO_REAL_CONST)
	lfs_imm_free_regs = FSBE_FNC(%FSBE_ENTITY,%FSEN_IMM_FREE)
	
	IF lfs_no_int_consts NE 0 AND lfs_no_real_consts NE 0
		fslv_glb_error_msg TEXTEQU <[#CG22] invalid back end (nonsense response to: FSEN_NO_INT/REAL_CONST)>
		fslv_glb_error = 1
		EXITM
	ELSEIFE ((fslv_glb_flags AND FSF_NO_INT_CONSTS) NE 0) AND (lfs_no_real_consts NE 0)
		lfs_no_int_consts = lfs_no_int_consts OR ((fslv_glb_flags AND FSF_NO_INT_CONSTS) NE 0)
	ENDIF
	
	;----------------------------
	; go through list and:
	; - search for registers (e.g. FPU: st0,st1,...)
	; - delete empty nodes
	; - trim tokens
	;----------------------------
	IFNB FSBE_ENUM_TKN
		FSBE_PROC %FSBE_ENUM_TKN,%FSB_ENUM_START,<>,0
	ENDIF
	lfs_st_max = -1
	lfs_iNode = ll_get_first(SlvPath)
	WHILE lfs_iNode
		lfs_txt TEXTEQU ll_get_data(SlvPath,%lfs_iNode,1)
		IFB lfs_txt
			lfs_iNode = ll_delete(SlvPath,%lfs_iNode)		
		ELSE
			lfs_txt TEXTEQU @TrimStr(%lfs_txt)
			IFNB FSBE_ENUM_TKN
				FSBE_PROC %FSBE_ENUM_TKN,%FSB_ENUM_TKN,<<%lfs_txt>>,ll_get_data(SlvPath,%lfs_iNode,2)
			ENDIF
			IF FSBE_FNC(%FSBE_IS_TKN_REG,<<%lfs_txt>>)
				ll_or_value SlvPath,%lfs_iNode,2,%TMT_ST
				ll_set_data SlvPath,%lfs_iNode,1,%fsbeitr_index
				IF fsbeitr_index GT lfs_st_max
					lfs_st_max = fsbeitr_index
				ENDIF
			ELSE
				ll_set_data SlvPath,%lfs_iNode,1,%lfs_txt
			ENDIF
			IF fslv_glb_error
				EXITM
			ENDIF
			lfs_iNode = ll_get_next(SlvPath,%lfs_iNode)	
		ENDIF
	ENDM
	IF fslv_glb_error
		EXITM
	ENDIF
	IF lfs_st_max NE -1
		lfs_stack_cntr = lfs_st_max + 1
	ENDIF
	IFNB FSBE_ENUM_TKN
		FSBE_PROC %FSBE_ENUM_TKN,%FSB_ENUM_END,<>,0
	ENDIF
	IF fslv_glb_error
		EXITM
	ENDIF
	FSO_N_STACK = FSBE_FNC(%FSBE_ENTITY,%FSEN_MAX_STACK)
	IF FSO_N_STACK LE 0
		fslv_glb_error_msg TEXTEQU <[#CG23] invalid back end (nonsense response to: FSEN_MAX_STACK)>
		fslv_glb_error = 1
		EXITM
	ENDIF
	lfs_stack_cntr = lfs_stack_cntr + FSBE_FNC(%FSBE_ENTITY,%FSEN_STCK_USAGE,%lfs_stack_cntr)
	
	IF FSBE_FNC(%FSBE_ENTITY,%FSEN_TEST_ATTR,<stck>)
		IF @fslv_test_attribute(<stck>,<:>,0)
			IF lfs_st_max EQ -1 AND fsta_value GT 0
				lfs_stack_cntr = fsta_value
			ELSE
				IF lfs_st_max+1 GT fsta_value
					lfs_stack_cntr = lfs_st_max + 1
					fslv_glb_error_msg TEXTEQU <[#CG24] Attribute <stck:N>: the number of used registers in expression is greater than N>
					fslv_glb_error = 1
					EXITM
				ELSE
					lfs_stack_cntr = fsta_value
					lfs_st_max = fsta_value - 1
				ENDIF
			ENDIF
		ENDIF
	ENDIF
	
	lfs_copy_supported = FSBE_FNC(%FSBE_ENTITY,%FSEN_COPY_SUPPORTED)
	lfs_dest_cntr = 0
	
	;-----------------------------------------
	; - create 'streaming' buffer serving
	;   tokens from linked list.
	; - each token has a textmacro: lfs_txtX 
	;   and a constant lfs_flgX
	; - 4 tokens are stocked
	; - lfs_txt2/flg2 is the current token 
	;-----------------------------------------
	lfs_iNode = ll_stream_tm(SlvPath,,1,,lfs_txt2,lfs_txt3,lfs_txt4,lfs_txt5)
	lfs_iNode = ll_stream_const(SlvPath,,2,,lfs_flg2,lfs_flg3,lfs_flg4,lfs_flg5)
	lfs_iNode = ll_stream_links(SlvPath,,lfs_iNode2,lfs_iNode3,lfs_iNode4,lfs_iNode5)
	;-----------------------------------------------
	; - process token list
	; - lfs_skip is used to discard tokens
	;   prior lfs_txt/flg2
	; - lfs_stack_cntr holds the number of currently
	;   used register
	;-----------------------------------------------
	lfs_skip = 0
	WHILE lfs_flg2
		IF lfs_skip
			lfs_skip = lfs_skip - 1
		ELSE
			IF lfs_flg2 AND TMT_ASSIGN
				IFE lfs_flg2 AND TMT_ST
					IF lfs_dest_cntr EQ 0
						lfs_retval TEXTEQU FSBE_FNC(%FSBE_ASSIGN_DEST,%lfs_txt2,%@IsIntegerType(%lfs_txt2),%@IsRealType(%lfs_txt2),bFunction,retval_int_type,retval_real_type,0,<>)
					ELSE
						%_discard_ FSBE_FNC(%FSBE_ASSIGN_DEST,%lfs_txt2,%@IsIntegerType(%lfs_txt2),%@IsRealType(%lfs_txt2),0,0,0,0,<>)
					ENDIF
				ELSE
					FSBE_PROC %FSBE_SET_REG,%(lfs_stack_cntr-(lfs_st_max-lfs_txt2)-1),1
				ENDIF
				lfs_stack_cntr = lfs_stack_cntr - 1
				lfs_dest_cntr = lfs_dest_cntr + 1
			ELSEIF lfs_flg2 AND TMT_TXT
				IF lfs_copy_supported NE 0 AND (lfs_flg2 AND TMT_NEG) EQ 0 AND (lfs_flg3 AND TMT_ASSIGN) NE 0
					IF lfs_flg2 AND TMT_ST
						lfs__txt2 TEXTEQU %(lfs_stack_cntr-(lfs_st_max-lfs_txt2)-1
					ELSE
						lfs__txt2 TEXTEQU lfs_txt2
					ENDIF
					IF lfs_flg3 AND TMT_ST
						lfs__txt3 TEXTEQU %(lfs_stack_cntr-(lfs_st_max-lfs_txt3)-1)
					ELSE
						lfs__txt3 TEXTEQU lfs_txt3
					ENDIF
					lfs_flg = _IF$(lfs_flg2 AND TMT_ST,2,1) OR _IF$(lfs_flg3 AND TMT_ST,4,0)
					lfs__iTyp = _IF$(lfs_flg3 AND TMT_ST,0,@IsIntegerType(%lfs__txt3))
					lfs__rTyp = _IF$(lfs_flg3 AND TMT_ST,0,@IsRealType(%lfs__txt3))
					IF lfs_dest_cntr EQ 0
						lfs_retval TEXTEQU FSBE_FNC(%FSBE_ASSIGN_DEST,%lfs__txt3,%lfs__iTyp,%lfs__rTyp,bFunction,retval_int_type,retval_real_type,%lfs_flg,%lfs__txt2)
					ELSE
						%_discard_ FSBE_FNC(%FSBE_ASSIGN_DEST,%lfs__txt3,%lfs__iTyp,%lfs__rTyp,0,0,0,%lfs_flg,%lfs__txt2)
					ENDIF
					lfs_dest_cntr = lfs_dest_cntr + 1
					lfs_skip = 1
				ELSE
					IF lfs_flg2 AND TMT_ST
						FSBE_PROC %FSBE_LOAD_REG,%(lfs_stack_cntr-(lfs_st_max-lfs_txt2)-1),%(lfs_flg2 AND TMT_NEG)
					ELSE
						FSBE_PROC %FSBE_LOAD_ARG,%lfs_txt2,%(lfs_flg2 AND TMT_NEG)
					ENDIF
					lfs_stack_cntr = lfs_stack_cntr + 1
				ENDIF
			ELSEIF lfs_flg2 AND TMT_CONST
				IF lfs_copy_supported NE 0 AND (lfs_flg2 AND TMT_NEG) EQ 0 AND (lfs_flg3 AND TMT_ASSIGN) NE 0
					lfs_flg = lfs_flg2
					lfs_txt TEXTEQU lfs_txt2
					lfs__txt2 TEXTEQU llfs_make_const()
					;lfs__txt3 TEXTEQU _IF$(lfs_flg3 AND TMT_ST,%(lfs_stack_cntr-(lfs_st_max-lfs_txt3)-1),%lfs_txt3)
					IF lfs_flg3 AND TMT_ST
						lfs__txt3 TEXTEQU %(lfs_stack_cntr-(lfs_st_max-lfs_txt3)-1)
					ELSE
						lfs__txt3 TEXTEQU lfs_txt3
					ENDIF
					IFB lfs__txt2
						lfs_flg = 0
						lfs_stack_cntr = lfs_stack_cntr + 1
					ELSE
						lfs_flg = _IF$(lfs_flg2 AND TMT_ST,2,1) OR _IF$(lfs_flg3 AND TMT_ST,4,0)
					ENDIF
					lfs__iTyp = _IF$(lfs_flg3 AND TMT_ST,0,@IsIntegerType(%lfs__txt3))
					lfs__rTyp = _IF$(lfs_flg3 AND TMT_ST,0,@IsRealType(%lfs__txt3))
					IF lfs_dest_cntr EQ 0
						lfs_retval TEXTEQU FSBE_FNC(%FSBE_ASSIGN_DEST,%lfs__txt3,%lfs__iTyp,%lfs__rTyp,bFunction,retval_int_type,retval_real_type,%lfs_flg,%lfs__txt2)
					ELSE
						%_discard_ FSBE_FNC(%FSBE_ASSIGN_DEST,%lfs__txt3,%lfs__iTyp,%lfs__rTyp,0,0,0,%lfs_flg,%lfs__txt2)
					ENDIF
					IFB lfs__txt2
						lfs_stack_cntr = lfs_stack_cntr - 1
					ENDIF
					lfs_dest_cntr = lfs_dest_cntr + 1
					lfs_skip = 1
				ELSE
					fslv_on_const SlvPath,SubExpr
				ENDIF
			ELSEIF lfs_flg2 AND TMT_PUSH
				IF (lfs_flg3 AND TMT_OPRT) NE 0 AND (lfs_flg4 AND TMT_POP) NE 0
					;; st(1) <+-*/^> st(0)
					FSBE_PROC %FSBE_REG1_OPRT_REG0_P,%lfs_txt3
					lfs_stack_cntr = lfs_stack_cntr - 1
					lfs_skip = 2
				ELSEIF (lfs_flg3 AND TMT_OPERAND) NE 0 AND (lfs_flg4 AND TMT_OPRT) NE 0 AND (lfs_flg5 AND TMT_POP) NE 0
					;; ? <+-*/^> st(0)
					fslv_load_larg
					IFNB lfs_txt
						IF lfs_flg AND TMT_ST NE 0
							FSBE_PROC %FSBE_REGX_OPRT_REG0,%lfs_txt4,%lfs_txt
						ELSE
							FSBE_PROC %FSBE_MEM_OPRT_REG0,%lfs_txt,%lfs_txt4,%@IsIntegerType(%lfs_txt),%@IsRealType(%lfs_txt)
						ENDIF
					ELSE
						fslv_stck_chk 1
						lfs_stack_cntr = lfs_stack_cntr + 1
						FSBE_PROC %FSBE_REG0_OPRT_REG1_P,%lfs_txt4
						lfs_stack_cntr = lfs_stack_cntr - 1
					ENDIF
					lfs_skip = 3
				ELSEIF (lfs_flg3 AND TMT_OPRT) NE 0
					%echo debug: push,<+-*/^>,? - unknown situation - report to author
				ELSEIF lfs_flg3 AND TMT_POP
					;; push,pop
					;; hold st(0)
					lfs_skip = 1
				ELSE
					;; push
					lfs_stack_cntr = lfs_stack_cntr + 1
				ENDIF			
			ELSEIF lfs_flg2 AND TMT_POP
				%echo debug: pop - unknown situation - report to author			
			ELSEIF lfs_flg2 AND TMT_STO
				lfs_idn_23 = llfs_is_idn(<%lfs_txt2>,<%lfs_txt3>)
				lfs_idn_24 = llfs_is_idn(<%lfs_txt2>,<%lfs_txt4>)
				lfs_idn_25 = llfs_is_idn(<%lfs_txt2>,<%lfs_txt5>)
				IF (lfs_flg3 AND TMT_OPRT) NE 0 AND (lfs_flg4 AND TMT_REC) NE 0 AND lfs_idn_24 NE 0
					;; st(1) <+-*/^> st(0)
					fslv_sto SubExpr,<%lfs_txt2>,1,%lfs_stack_cntr
					IF lfs_flg4 AND TMT_NEG
						FSBE_PROC %FSBE_NEG
					ENDIF
					FSBE_PROC %FSBE_REG1_OPRT_REG0_P,%lfs_txt3
					lfs_stack_cntr = lfs_stack_cntr - 1
					lfs_skip = 2
				ELSEIF (lfs_flg3 AND TMT_OPERAND) NE 0 AND (lfs_flg4 AND TMT_OPRT) NE 0 AND (lfs_flg5 AND TMT_REC) NE 0 AND lfs_idn_25 NE 0
					;; ? <+-*/^> st(0)
					fssror_done = 0
					IF (lfs_flg3 AND TMT_REC) NE 0 AND lfs_idn_23 NE 0 AND FSLV_NO_FORMULA_OPTIMIZATION EQ 0
						fslv_sto_rec0_oprt_rec0
					ENDIF
					IF fssror_done EQ 0
						fslv_sto SubExpr,<%lfs_txt2>,1,%lfs_stack_cntr
						IF lfs_flg5 AND TMT_NEG
							FSBE_PROC %FSBE_NEG
						ENDIF
						fslv_load_larg
						IFNB lfs_txt
							IF lfs_flg AND TMT_ST NE 0
								FSBE_PROC %FSBE_REGX_OPRT_REG0,%lfs_txt4,%lfs_txt
							ELSE
								FSBE_PROC %FSBE_MEM_OPRT_REG0,%lfs_txt,%lfs_txt4,%@IsIntegerType(%lfs_txt),%@IsRealType(%lfs_txt)
							ENDIF				
						ELSE
							fslv_stck_chk 1
							lfs_stack_cntr = lfs_stack_cntr + 1
							FSBE_PROC %FSBE_REG0_OPRT_REG1_P,%lfs_txt4
							lfs_stack_cntr = lfs_stack_cntr - 1
						ENDIF
					ENDIF
					lfs_skip = 3
				ELSEIF (lfs_flg3 AND TMT_REC) NE 0 AND lfs_idn_23 NE 0 AND (lfs_flg4 AND TMT_REC) NE 0 AND lfs_idn_24 NE 0
					;; sto x , rec x , rec x
					fslv_sto SubExpr,<%lfs_txt2>,2,%lfs_stack_cntr
					IF (lfs_flg3 AND TMT_NEG) NE 0 AND (lfs_flg4 AND TMT_NEG) NE 0
						FSBE_PROC %FSBE_NEG
						FSBE_PROC %FSBE_LOAD_REG,0
						lfs_stack_cntr = lfs_stack_cntr + 1
					ELSEIF lfs_flg3 AND TMT_NEG
						FSBE_PROC %FSBE_LOAD_REG,0,1
						lfs_stack_cntr = lfs_stack_cntr + 1
						FSBE_PROC %FSBE_XCHG
					ELSEIF lfs_flg4 AND TMT_NEG
						FSBE_PROC %FSBE_LOAD_REG,0,1
						lfs_stack_cntr = lfs_stack_cntr + 1
					ELSE
						FSBE_PROC %FSBE_LOAD_REG,0
						lfs_stack_cntr = lfs_stack_cntr + 1
					ENDIF
					lfs_skip = 2
				ELSEIF (lfs_flg3 AND TMT_REC) NE 0 AND lfs_idn_23 NE 0
					;; sto x, rec x
					;; hold
					fslv_sto SubExpr,<%lfs_txt2>,1,%lfs_stack_cntr
					IF lfs_flg3 AND TMT_NEG
						FSBE_PROC %FSBE_NEG
					ENDIF
					lfs_skip = 1
				ELSE
					fslv_sto SubExpr,<%lfs_txt2>,0,%lfs_stack_cntr
					;lfs_stack_cntr = lfs_stack_cntr - 1
				ENDIF
			ELSEIF lfs_flg2 AND TMT_REC
				lfs_txt TEXTEQU fslv_rec(SubExpr,<%lfs_txt2>,%lfs_stack_cntr)
				FSBE_PROC %FSBE_LOAD_REG,%lfs_txt,%lfs_flg2 AND TMT_NEG
				lfs_stack_cntr = lfs_stack_cntr + 1
			ELSEIF lfs_flg2 AND TMT_OPRT
				;; st(0) <+-*/^> ?
				lfs_rcval_used = 0
				IF lfs_flg3 AND TMT_OPERAND
					fslv_load_rarg
				ELSE
					fslv_glb_error_msg TEXTEQU <[#CG2] missing operand>
					fslv_glb_error = 1
					EXITM
				ENDIF
				IFNB lfs_txt
					IF lfs_flg AND TMT_ST
						FSBE_PROC %FSBE_REG0_OPRT_REGX,%lfs_txt2,%lfs_txt
					ELSE
						FSBE_PROC %FSBE_REG0_OPRT_MEM,%lfs_txt,%lfs_txt2,%@IsIntegerType(%lfs_txt),%@IsRealType(%lfs_txt)
					ENDIF
				ELSE
					IF lfs_rcval_used EQ 0
						lfs_stack_cntr = lfs_stack_cntr + 1
						FSBE_PROC %FSBE_REG1_OPRT_REG0_P,%lfs_txt2
						lfs_stack_cntr = lfs_stack_cntr - 1
					ENDIF
				ENDIF
				lfs_skip = 1
			ELSEIF lfs_flg2 AND TMT_FNC
				lfs_flg = lfs_flg2
				lfs_txt TEXTEQU lfs_txt2
				fslv_call_fnc
			ELSEIF lfs_flg2 AND TMT_NEG
				FSBE_PROC %FSBE_NEG
			ELSE
				fslv_glb_error_msg TEXTEQU <[#CG2] unknown>
				fslv_glb_error = 1
			ENDIF
		ENDIF

		;%echo STK: @CatStr(%lfs_stack_cntr)
		fslv_stck_chk 0
		IF fslv_glb_error
			EXITM
		ENDIF
		;---------------------------------------
		; - make one step forward in 
		;   token-buffer
		;---------------------------------------
		lfs_flg2 = lfs_flg3
		lfs_flg3 = lfs_flg4
		lfs_flg4 = lfs_flg5
		lfs_iNode2 = lfs_iNode3
		lfs_iNode3 = lfs_iNode4
		lfs_iNode4 = lfs_iNode5
		lfs_iNode5 = lfs_iNode
		lfs_txt2 TEXTEQU lfs_txt3
		lfs_txt3 TEXTEQU lfs_txt4
		lfs_txt4 TEXTEQU lfs_txt5
		IF lfs_iNode
			lfs_txt5 TEXTEQU ll_get_data(SlvPath,%lfs_iNode,1)
			lfs_flg5 = ll_get_data(SlvPath,%lfs_iNode,2)
			lfs_iNode = ll_get_next(SlvPath,%lfs_iNode)
		ELSE
			lfs_txt5 TEXTEQU <>
			lfs_flg5 = 0
		ENDIF
		
	ENDM
	IF fslv_glb_error
		IF FSBE_FNC(%FSBE_ENTITY,%FSEN_NAME)
			fslv_glb_error_msg TEXTEQU <[back end: >,fsbe_txt,<]>,fslv_glb_error_msg
		ENDIF
		EXITM
	ENDIF
	IF lfs_dest_cntr EQ 0 AND bFunction NE 0
		lfs_retval TEXTEQU FSBE_FNC(%FSBE_ASSIGN_DEST,<>,0,0,bFunction,retval_int_type,retval_real_type,0,<>)
		lfs_stack_cntr = lfs_stack_cntr - 1
	ENDIF
	
	IF iml_get_max(StkSlot) GE 0 AND lfs_imm_free_regs EQ 0
		lfs_cntr=0
		REPEAT iml_get_max(StkSlot)+1
			FSBE_PROC %FSBE_FREE_REG,%lfs_stack_cntr+lfs_cntr
			lfs_cntr = lfs_cntr + 1
		ENDM
	ENDIF

	IF FSBE_FNC(%FSBE_ENTITY,%FSEN_TEST_ATTR,<pop>)
		IF @fslv_test_attribute(<pop>,<:>,0)
			IF fsta_value GT 0
				REPEAT fsta_value
					FSBE_PROC %FSBE_SET_REG,0,1
				ENDM
			ENDIF
		ENDIF
	ENDIF
	
	IF FSBE_FNC(%FSBE_ENTITY,%FSEN_TEST_ATTR,<free>)
		IF @fslv_test_attribute(<free>,<:>,0)
			fslv_tmp_i INSTR 1,fsta_value,<->
			IF fslv_tmp_i EQ 0
				FSBE_PROC %FSBE_FREE_REG,%fsta_value
			ELSE
				fslv_tmp_i1 SUBSTR fsta_value,1,fslv_tmp_i-1
				fslv_tmp_i2 SUBSTR fsta_value,fslv_tmp_i+1
				IF fslv_tmp_i2 LT fslv_tmp_i1
					fslv_tmp_i = fslv_tmp_i2
					fslv_tmp_i2 = fslv_tmp_i1
					fslv_tmp_i1 = fslv_tmp_i
				ENDIF
				fslv_tmp_i = fslv_tmp_i1
				REPEAT fslv_tmp_i2 - fslv_tmp_i1 + 1
					FSBE_PROC %FSBE_FREE_REG,%fslv_tmp_i
					fslv_tmp_i = fslv_tmp_i + 1
				ENDM
			ENDIF
		ENDIF
	ENDIF
	%_discard_ FSBE_FNC(%FSBE_ENTITY,%FSEN_END)
	IF fslv_glb_error
		IF FSBE_FNC(%FSBE_ENTITY,%FSEN_NAME)
			fslv_glb_error_msg TEXTEQU <[back end: >,fsbe_txt,<]>,fslv_glb_error_msg
		ENDIF
	ENDIF
endm

fslv__expand_const macro arg
	EXITM <&arg>
endm

; integer optimization
; res = [c op2] ((IFNB _neg ? -1:1)*(a op b)) ; []=optinal
; checks overflow by reversing the operation (needed for MASM 6-7)
fslv_const_calc macro a,b,op,op2,_c,_neg
	; TODO: check overflow
	fscc_done = 0
	fscc_result TEXTEQU <>
	fscc_result0 TEXTEQU <>
	
	IF @InStr(1,<+-*>,<&op>) NE 0 
		fscc_result TEXTEQU @CatStr(%((a) op (b)))
		IF a EQ (fscc_result @SubStr(<-+/>,@InStr(1,<+-*>,<&op>),1) b)
			fscc_done = 1
		ENDIF
	ELSEIFIDN <&op>,</>
		IF b EQ 0
			fslv_glb_error_msg TEXTEQU <[#CG29] division by zero in constant integer expression>
			fslv_glb_error = 1
		ELSEIF (a MOD b) EQ 0
			fscc_result TEXTEQU @CatStr(%((a) op (b)))
			IF a EQ (fscc_result * b)
				fscc_done = 1
			ENDIF
		ENDIF
	ELSEIFIDN <&op>,<^>
		IF b EQ 0
			fscc_result TEXTEQU <1>
			fscc_done = 1
		ELSEIF b EQ 1
			fscc_result TEXTEQU <&a>
			fscc_done = 1
		ELSEIF b GE 2 AND b LE 31
			fscc_im_res = a
			fscc_cntr = 0
			WHILE fscc_cntr LT b-1
				fscc_im_res = fscc_im_res * (a)
				fscc_cntr = fscc_cntr + 1
			ENDM
			fscc_result TEXTEQU @CatStr(%(fscc_im_res))
			fscc_cntr = 0
			WHILE fscc_cntr LT b-1
				fscc_im_res = fscc_im_res / (a)
				fscc_cntr = fscc_cntr + 1
			ENDM
			IF a EQ @CatStr(%fscc_im_res)
				fscc_done = 1
			ENDIF
		ENDIF
	ENDIF
	IF fscc_done NE 0
		IFNB <&op2>
			fscc_result0 TEXTEQU fscc_result
			IFB <&_neg>
				fslv_const_calc <&_c>,%fscc_result,<&op2>
			ELSE
				fslv_const_calc <&_c>,%(-(fscc_result)),<&op2>
			ENDIF
		ENDIF	
	ENDIF
	IF fscc_done
		IFE lfs_no_int_consts
			IF  fscc_result LT -2147483648 OR fscc_result GT 2147483647
				fscc_done = 0
			ENDIF
		ELSE
			IF  fslv_lcl_real_type EQ 4
				IF fscc_result LT -16777216 OR fscc_result GT 16777216	; REAL4: exact representable integers
					fscc_done = 0
				ENDIF
			ENDIF
		ENDIF
	ENDIF
	
;	IF fscc_done
;		%echo DONE: fscc_result = _c op2 (a op b)
;	ELSE
;		%echo REJ: _c op2 (a op b)
;	ENDIF
endm

fslv_repl_all_recs macro SlvPath,iNode,refVal,constValue
	fsrar_iNode = iNode
	WHILE fsrar_iNode
		fsrar_flg = ll_get_data(SlvPath,%fsrar_iNode,2)
		IF fsrar_flg AND TMT_REC
			fsrar_txt TEXTEQU ll_get_data(SlvPath,%fsrar_iNode,1)
			IFIDN fsrar_txt,<&refVal>
				IF fsrar_flg AND TMT_NEG
					ll_set_data SlvPath,%fsrar_iNode,1,%(-(constValue))
				ELSE
					ll_set_data SlvPath,%fsrar_iNode,1,constValue
				ENDIF
				ll_set_data SlvPath,%fsrar_iNode,2,%(TMT_INT OR TMT_OPTMZTN)
			ENDIF
		ENDIF		
		fsrar_iNode = ll_get_next(SlvPath,%fsrar_iNode)
	ENDM
endm

fslv_on_const macro SlvPath,SubExpr

	lfs_idn_35 = llfs_is_idn(<%lfs_txt3>,<%lfs_txt5>) NE 0
	lfs__cond =				 (lfs_flg3 AND TMT_PUSH) NE 0 AND (lfs_flg4 AND TMT_OPRT) NE 0 AND @InStr(1,<+-*>,%lfs_txt4) NE 0 AND (lfs_flg5 AND TMT_POP) NE 0
	lfs__cond = lfs__cond OR ((lfs_flg3 AND TMT_STO) NE 0 AND (lfs_flg4 AND TMT_OPRT) NE 0 AND @InStr(1,<+-*>,%lfs_txt4) NE 0 AND (lfs_flg5 AND TMT_REC) NE 0 AND lfs_idn_35)
	IF lfs__cond AND (lfs_flg2 AND TMT_INT) NE 0
		; st(0) <+-*> int
		lfs__txt2 TEXTEQU lfs_txt2
		IF (lfs_flg2 AND TMT_NEG) NE 0 AND (lfs_flg2 AND TMT_INT) NE 0 
			lfs__txt2 TEXTEQU @CatStr(%-(lfs_txt2))
		ENDIF
		IFIDNI lfs_txt4,<->
			IF (lfs_flg2 AND TMT_INT) NE 0 
				lfs__txt2 TEXTEQU @CatStr(%-(lfs__txt2))
				lfs_txt4 TEXTEQU <+>
			ENDIF
		ENDIF
		IF (lfs_flg5 AND TMT_REC) NE 0 AND (lfs_flg5 AND TMT_NEG) NE 0 AND (lfs_flg2 AND TMT_INT) NE 0
			lfs__txt2 TEXTEQU @CatStr(%-(lfs__txt2))
		ENDIF
		IF (lfs_flg5 AND TMT_REC) NE 0
			fslv_sto SubExpr,%lfs_txt5,1,%lfs_stack_cntr
		ENDIF
		lfs_flg = TMT_INT
		lfs_txt TEXTEQU lfs__txt2
		lfs_txt TEXTEQU llfs_make_const()
		FSBE_PROC %FSBE_REG0_OPRT_MEM,%lfs_txt,%lfs_txt4,%@IsIntegerType(%lfs_txt),%@IsRealType(%lfs_txt)
		lfs_skip = 3
		EXITM
	ENDIF
	IF ((fslv_lcl_int_type LE 4) OR (lfs_no_int_consts NE 0)) AND FSLV_NO_FORMULA_OPTIMIZATION EQ 0 AND (lfs_flg2 AND TMT_INT) NE 0
		lfs_flg6 = ll_get(SlvPath,%lfs_iNode,0,2,1,<0>)
		lfs_txt6 TEXTEQU ll_get(SlvPath,%lfs_iNode,0,1,1,< >)
		IF (lfs_flg3 AND TMT_OPRT) NE 0 AND (lfs_flg4 AND TMT_INT) NE 0
			; B = A <op> B
			lfs__txt2 TEXTEQU lfs_txt2
			lfs__txt4 TEXTEQU lfs_txt4
			IF lfs_flg2 AND TMT_NEG
				lfs__txt2 TEXTEQU @CatStr(%-(lfs_txt2))
			ENDIF
			IF lfs_flg4 AND TMT_NEG
				lfs__txt4 TEXTEQU @CatStr(%-(lfs_txt4))
			ENDIF
			fslv_const_calc %lfs__txt2,%lfs__txt4,%lfs_txt3
			
			IF fscc_done
				ll_set_data SlvPath,%lfs_iNode4,1,%fscc_result
				ll_set_data SlvPath,%lfs_iNode4,2,%(TMT_INT OR TMT_OPTMZTN)
				lfs_txt4 TEXTEQU fscc_result
				lfs_flg4 = TMT_INT OR TMT_OPTMZTN
				lfs_skip = 1
				EXITM
			ENDIF
		ELSEIF (lfs_flg3 AND TMT_INT) NE 0 AND (lfs_flg4 AND TMT_PUSH) NE 0 AND (lfs_flg5 AND TMT_OPRT) NE 0 AND (lfs_flg6 AND TMT_POP) NE 0
			; A <op> B
			lfs__txt2 TEXTEQU lfs_txt2
			lfs__txt3 TEXTEQU lfs_txt3
			IF lfs_flg2 AND TMT_NEG
				lfs__txt2 TEXTEQU @CatStr(%-(lfs_txt2))
			ENDIF
			IF lfs_flg3 AND TMT_NEG
				lfs__txt3 TEXTEQU @CatStr(%-(lfs_txt3))
			ENDIF
			fslv_const_calc %lfs__txt2,%lfs__txt3,%lfs_txt5
			IF fscc_done
				ll_set_data SlvPath,%lfs_iNode,1,%fscc_result
				ll_set_data SlvPath,%lfs_iNode,2,%(TMT_INT OR TMT_OPTMZTN)
				lfs_skip = 3
				EXITM
			ENDIF
		ELSE
			lfs_idn_36 = llfs_is_idn(<%lfs_txt3>,<%lfs_txt6>) NE 0
			lfs_idn_34 = llfs_is_idn(<%lfs_txt3>,<%lfs_txt4>) NE 0
			lfs_idn_46 = llfs_is_idn(<%lfs_txt4>,<%lfs_txt6>) NE 0
			IF (lfs_flg3 AND TMT_INT) NE 0 AND (lfs_flg4 AND TMT_STO) NE 0 AND (lfs_flg5 AND TMT_OPRT) NE 0 AND (lfs_flg6 AND TMT_REC) NE 0 AND lfs_idn_46
				; A <*/^> B
				lfs__txt2 TEXTEQU lfs_txt2
				lfs__txt3 TEXTEQU lfs_txt3
				IF lfs_flg2 AND TMT_NEG
					lfs__txt2 TEXTEQU @CatStr(%-(lfs_txt2))
				ENDIF
				IF lfs_flg3 AND TMT_NEG
					lfs__txt3 TEXTEQU @CatStr(%-(lfs_txt3))
				ENDIF
				IF lfs_flg6 AND TMT_NEG
					lfs__txt3 TEXTEQU @CatStr(%-(lfs__txt3))
				ENDIF
				fslv_const_calc %lfs__txt2,%lfs__txt3,%lfs_txt5
				IF fscc_done
					fslv_repl_all_recs SlvPath,%lfs_iNode,%lfs_txt4,%lfs_txt3
					ll_set_data SlvPath,%lfs_iNode,1,%fscc_result
					ll_set_data SlvPath,%lfs_iNode,2,%(TMT_INT OR TMT_OPTMZTN)
					lst_set_data SubExpr,%(lfs_txt4+2),2,0
					lfs_skip = 3
					EXITM
				ENDIF
			ELSEIF (lfs_flg3 AND TMT_STO) NE 0 AND (lfs_flg4 AND TMT_REC) NE 0 AND (lfs_flg5 AND TMT_OPRT) NE 0  AND (lfs_flg6 AND TMT_INT) NE 0 AND lfs_idn_34
				lfs__txt2 TEXTEQU lfs_txt2
				IF lfs_flg2 AND TMT_NEG
					lfs__txt2 TEXTEQU @CatStr(%-(lfs_txt2))
				ENDIF
				IF lfs_flg4 AND TMT_NEG
					lfs__txt2 TEXTEQU @CatStr(%-(lfs__txt2))
				ENDIF
				IF lfs_flg6 AND TMT_NEG
					lfs_txt6 TEXTEQU @CatStr(%-(lfs_txt6))
				ENDIF
				fslv_const_calc %lfs__txt2,%lfs_txt6,%lfs_txt5
				IF fscc_done
					ll_set_data SlvPath,%lfs_iNode,1,%fscc_result
					ll_set_data SlvPath,%lfs_iNode,2,%(TMT_INT OR TMT_OPTMZTN)
					lst_set_data SubExpr,%(lfs_txt3+2),2,0
					fslv_repl_all_recs SlvPath,%lfs_iNode,%lfs_txt3,%lfs_txt2
					lfs_skip = 3
					EXITM
				ENDIF
			ELSEIF (lfs_flg3 AND TMT_STO) NE 0 AND (lfs_flg4 AND TMT_INT) NE 0 AND (lfs_flg5 AND TMT_OPRT) NE 0  AND (lfs_flg6 AND TMT_REC) NE 0 AND lfs_idn_36
				lfs__txt2 TEXTEQU lfs_txt2
				lfs__txt4 TEXTEQU lfs_txt4
				IF lfs_flg2 AND TMT_NEG
					lfs__txt2 TEXTEQU @CatStr(%-(lfs_txt2))
				ENDIF
				IF lfs_flg4 AND TMT_NEG
					lfs__txt4 TEXTEQU @CatStr(%-(lfs_txt4))
				ENDIF
				IF lfs_flg6 AND TMT_NEG
					lfs__txt2 TEXTEQU @CatStr(%-(lfs__txt2))
				ENDIF
				fslv_const_calc %lfs__txt4,%lfs_txt2,%lfs_txt5
				IF fscc_done
					ll_set_data SlvPath,%lfs_iNode,1,%fscc_result
					ll_set_data SlvPath,%lfs_iNode,2,%(TMT_INT OR TMT_OPTMZTN)
					lst_set_data SubExpr,%(lfs_txt3+2),2,0
					fslv_repl_all_recs SlvPath,%lfs_iNode,%lfs_txt3,%lfs_txt2
					lfs_skip = 3
					EXITM
				ENDIF
			ELSEIF (lfs_flg3 AND TMT_INT) NE 0 AND (lfs_flg4 AND TMT_OPRT) NE 0 AND (lfs_flg5 AND TMT_INT) NE 0
				lfs_flg7 = ll_get(SlvPath,%lfs_iNode,1,2,1,<0>)
				lfs_flg8 = ll_get(SlvPath,%lfs_iNode,2,2,1,<0>)
				lfs_txt7 TEXTEQU ll_get(SlvPath,%lfs_iNode,1,1,1,< >)
				lfs_txt8 TEXTEQU ll_get(SlvPath,%lfs_iNode,2,1,1,< >)
				IF (lfs_flg6 AND TMT_PUSH) NE 0 AND (lfs_flg7 AND TMT_OPRT) NE 0 AND (lfs_flg8 AND TMT_POP) NE 0
					; D = C <op> A <op> B ; constant integers
					lfs__txt2 TEXTEQU lfs_txt2
					lfs__txt3 TEXTEQU lfs_txt3
					lfs__txt5 TEXTEQU lfs_txt5
					IF lfs_flg2 AND TMT_NEG
						lfs__txt2 TEXTEQU @CatStr(%-(lfs_txt2))
					ENDIF
					IF lfs_flg3 AND TMT_NEG
						lfs__txt3 TEXTEQU @CatStr(%-(lfs_txt3))
					ENDIF
					IF lfs_flg5 AND TMT_NEG
						lfs__txt5 TEXTEQU @CatStr(%-(lfs_txt5))
					ENDIF
					fslv_const_calc %lfs__txt3,%lfs__txt5,%lfs_txt4,%lfs_txt7,%lfs__txt2
					IF fscc_done
						ll_set_data SlvPath,%ll_get(SlvPath,%lfs_iNode,2,0,0,0),1,%fscc_result
						ll_set_data SlvPath,%ll_get(SlvPath,%lfs_iNode,2,0,0,0),2,%(TMT_INT OR TMT_OPTMZTN)
						lfs_skip = 5
						EXITM
					ENDIF
				ELSE
					IF (lfs_flg6 AND TMT_STO) NE 0 AND (lfs_flg7 AND TMT_OPRT) NE 0 AND (lfs_flg8 AND TMT_REC) NE 0 AND llfs_is_idn(<%lfs_txt6>,<%lfs_txt8>) NE 0 ;AND @InStr(1,<*/^>,%lfs_txt7) NE 0
						; D = A{+-*}A{+-*}B ; constant integers
						lfs__txt2 TEXTEQU lfs_txt2
						lfs__txt3 TEXTEQU lfs_txt3
						lfs__txt5 TEXTEQU lfs_txt5
						IF lfs_flg2 AND TMT_NEG
							lfs__txt2 TEXTEQU @CatStr(%-(lfs_txt2))
						ENDIF
						IF lfs_flg3 AND TMT_NEG
							lfs__txt3 TEXTEQU @CatStr(%-(lfs_txt3))
						ENDIF
						IF lfs_flg5 AND TMT_NEG
							lfs__txt5 TEXTEQU @CatStr(%-(lfs_txt5))
						ENDIF
						IF lfs_flg8 AND TMT_NEG
							fslv_const_calc %lfs__txt3,%lfs__txt5,%lfs_txt4,%lfs_txt7,%lfs__txt2,<neg>
						ELSE
							fslv_const_calc %lfs__txt3,%lfs__txt5,%lfs_txt4,%lfs_txt7,%lfs__txt2,<>
						ENDIF
						IF fscc_done
							lfs__iNode = ll_get(SlvPath,%lfs_iNode,2,0,0,0)
							ll_set_data SlvPath,%lfs__iNode,1,%fscc_result
							ll_set_data SlvPath,%lfs__iNode,2,%(TMT_INT OR TMT_OPTMZTN)
							lst_set_data SubExpr,%(lfs_txt6+2),2,0
							fslv_repl_all_recs SlvPath,%lfs_iNode,%lfs_txt6,%fscc_result0
							lfs_skip = 5
							EXITM
						ENDIF
					ENDIF
				ENDIF
			ENDIF
		ENDIF
	ENDIF
	lfs_txt TEXTEQU lfs_txt2
	lfs_flg = lfs_flg2
	llfs_load_const
	lfs_stack_cntr = lfs_stack_cntr + 1
endm

fslv_sto_rec0_oprt_rec0 macro
	fssror_done = 1
	fssror_op = @MatchStrI(%lfs_txt4,0,<+>,<->,<*>,</>,<^>)
	IF fssror_op EQ 4 AND ((lfs_flg3 AND TMT_NEG) EQ (lfs_flg5 AND TMT_NEG))
		fslv_glb_error_msg TEXTEQU <[#CG26] unnecessary division of bracket enclosed expression: will always evaluate to +1 : +-()/+-()>
		fslv_glb_error = 1
	ELSEIF fssror_op EQ 4 AND ((lfs_flg3 AND TMT_NEG) NE (lfs_flg5 AND TMT_NEG))
		fslv_glb_error_msg TEXTEQU <[#CG27] unnecessary division of bracket enclosed expression: will always evaluate to -1 : +-()/-+()>
		fslv_glb_error = 1
	ELSEIF fssror_op LE 2 AND ((lfs_flg3 AND TMT_NEG) NE ((lfs_flg5 AND TMT_NEG) XOR ((fssror_op EQ 2) AND TMT_NEG)))
		fslv_glb_error_msg TEXTEQU <[#CG28] unnecessary summation of bracket enclosed expression: will always evaluate to zero: +-()-+()>
		fslv_glb_error = 1
	ELSEIF fssror_op EQ 5 AND ((lfs_flg3 AND TMT_NEG) NE (lfs_flg5 AND TMT_NEG))
		fssror_done = 0
		EXITM
	ELSE
		fslv_sto SubExpr,<%lfs_txt2>,2,%lfs_stack_cntr
		IF fssror_op LE 2
			FSBE_PROC %FSBE_REG0_OPRT_REGX,<+>,0
		ELSE
			FSBE_PROC %FSBE_REG0_OPRT_REGX,%lfs_txt4,0
		ENDIF
		IF fssror_op LE 2 AND (lfs_flg3 AND TMT_NEG) NE 0
			FSBE_PROC %FSBE_NEG
		ENDIF	
	ENDIF
endm

fslv_stck_chk macro nstck:req
	IF lfs_stack_cntr + nstck + lfs_nss GT FSO_N_STACK
		fslv_glb_error_msg TEXTEQU <[#CG3] ERROR: register-stack overflow - reformulate expression>
		fslv_glb_error = 1
		EXITM
	ENDIF	
endm


fslv_call_fnc macro
	IF lfs_flg AND (TMT_SINGLE_FLT OR TMT_SINGLE_INT OR TMT_SINGLE_ARG)
		lfs_fncName SUBSTR lfs_txt,@InStr(1,<%lfs_txt>,<,>)+1
	ELSE
		lfs_fncName TEXTEQU lfs_txt
	ENDIF
	lfs_fncName TEXTEQU @TrimStr(<%lfs_fncName>)
	IF FSBE_FNC(%FSBE_FNC_DSCPTR,%lfs_fncName,0,1)
		??lfs_ins_test = FNCD_X32
		IF @WordSize EQ 8
			??lfs_ins_test = FNCD_X64
		ENDIF
		IF FSBE_FNC(%FSBE_FNC_DSCPTR,%lfs_fncName,%??lfs_ins_test) NE 0
			IF FSBE_FNC(%FSBE_FNC_DSCPTR,%lfs_fncName,%FNCD_TST_LCL_TYPES) NE 0
				lfs_fnc_nargs = FSBE_FNC(%FSBE_FNC_DSCPTR,%lfs_fncName,%FNCD_NARGS)
				IF (lfs_flg AND (TMT_SINGLE_INT OR TMT_SINGLE_FLT)) NE 0 AND lfs_fnc_nargs NE 1
					fslv_glb_error_msg TEXTEQU <[#CG4] function >,lfs_fncName,< require 1 argument>
					fslv_glb_error = 1
				ELSE
					IF lfs_flg AND (TMT_SINGLE_INT OR TMT_SINGLE_FLT)
						lfs_txt TEXTEQU @SubStr(<%lfs_txt>,1,%(@InStr(1,<%lfs_txt>,<,>)-1))
						lfs_txt TEXTEQU @TrimStr(%lfs_txt)
						IF lfs_flg AND TMT_SINGLE_INT
							lfs_flg = lfs_flg OR TMT_INT
						ELSE
							lfs_flg = lfs_flg OR TMT_FLT
						ENDIF
						llfs_load_const
						lfs_stack_cntr = lfs_stack_cntr + 1
						fslv_stck_chk FSBE_FNC(%FSBE_FNC_DSCPTR,%lfs_fncName,%FNCD_NREGS)
						IFE fslv_glb_error
							FSBE_PROC %FSBE_CALL_FNC,%lfs_fncName
						ENDIF
					ELSEIF lfs_flg AND TMT_SINGLE_ARG
						lfs_arg TEXTEQU @SubStr(<%lfs_txt>,1,%(@InStr(1,<%lfs_txt>,<,>)-1))
						lfs_arg TEXTEQU @TrimStr(%lfs_arg)
						fslv_stck_chk 1
						IFE fslv_glb_error
							FSBE_PROC %FSBE_LOAD_ARG,%lfs_arg,0
							lfs_stack_cntr = lfs_stack_cntr + 1
							fslv_stck_chk FSBE_FNC(%FSBE_FNC_DSCPTR,%lfs_fncName,%FNCD_NREGS)
							IFE fslv_glb_error
								FSBE_PROC %FSBE_CALL_FNC,%lfs_fncName
							ENDIF
						ENDIF
					ELSE
						fslv_stck_chk FSBE_FNC(%FSBE_FNC_DSCPTR,%lfs_fncName,%FNCD_NREGS)
						IFE fslv_glb_error
							FSBE_PROC %FSBE_CALL_FNC,%lfs_fncName
						ENDIF
						lfs_stack_cntr = lfs_stack_cntr - (lfs_fnc_nargs - 1)
					ENDIF
					IF (lfs_flg AND TMT_NEG) NE 0 AND fslv_glb_error EQ 0
						FSBE_PROC %FSBE_NEG
					ENDIF
				ENDIF
			ELSE
				fslv_glb_error_msg TEXTEQU <[#CG21] function does not support current precision settings: >,lfs_fncName
				fslv_glb_error = 1
			ENDIF
		ELSE
			IF @WordSize EQ 8
				fslv_glb_error_msg TEXTEQU <[#CG5] function has no x86-64 implementation: >,lfs_fncName
			ELSE
				fslv_glb_error_msg TEXTEQU <[#CG5] function has no x86-32 implementation: >,lfs_fncName
			ENDIF
			fslv_glb_error = 1
		ENDIF
	ELSE
		fslv_glb_error_msg TEXTEQU <[#CG6] function is undefined or not supported by current back end: >,lfs_fncName
		fslv_glb_error = 1
		IF @Version LT 800
			IFIDNI lfs_fncName,<XMM>
				fslv_glb_error_msg TEXTEQU <[#CG22] XMM registers can't be used as arguments with MASM 6 and 7 - use JWASM or ML 8+>
			ENDIF
		ENDIF
	ENDIF
endm

; return: lfs_txt = operator, lfs_flg = flags => vaild if lfs_txt not blank => TMT_ST
fslv_load_rarg macro
	IF lfs_flg3 AND TMT_TXT
		lfs_flg = 0
		IF lfs_flg3 AND TMT_ST
			;lfs_txt TEXTEQU FSBE_FNC(%FSBE_PREPARE_REG,%lfs_stack_cntr-(lfs_st_max-lfs_txt3)-1,%lfs_flg3 AND TMT_NEG)
			;lfs_flg = TMT_ST
			IF lfs_flg3 AND TMT_NEG
				FSBE_PROC %FSBE_LOAD_REG,%lfs_stack_cntr-(lfs_st_max-lfs_txt3)-1,1
				lfs_txt TEXTEQU <>
			ELSE
				lfs_txt TEXTEQU %lfs_stack_cntr-(lfs_st_max-lfs_txt3)-1
				lfs_flg = TMT_ST
			ENDIF
		ELSE
			lfs_txt TEXTEQU FSBE_FNC(%FSBE_PREPARE_ARG,%lfs_txt3,%lfs_flg3 AND TMT_NEG)
		ENDIF
	ELSEIF lfs_flg3 AND TMT_CONST
		fsberor_new_const_is_int = 0
		fsberor_new_params = 0
		fsberor_new_op TEXTEQU lfs_txt2
		fsberor_new_const TEXTEQU lfs_txt3
		IF FSBE_FNC(%FSBE_REG0_OPRT_RCVAL,%lfs_txt2,%lfs_txt3,%(lfs_flg3 AND TMT_NEG))
			lfs_rcval_used = 1
			lfs_txt TEXTEQU <>
			lfs_flg = 0
			EXITM
		ENDIF
		IF fsberor_new_params
			lfs_txt2 TEXTEQU fsberor_new_op
			lfs_txt3 TEXTEQU fsberor_new_const
			lfs_flg3 = lfs_flg3 AND (NOT TMT_CONST)
			IF fsberor_new_const_is_int
				lfs_flg3 = lfs_flg3 OR TMT_INT
			ELSE
				lfs_flg3 = lfs_flg3 OR TMT_FLT
			ENDIF
		ENDIF
		lfs_txt TEXTEQU lfs_txt3
		lfs_flg = lfs_flg3
		lfs_txt TEXTEQU llfs_make_const()
		lfs_flg = 0
	ELSEIF lfs_flg3 AND TMT_REC
		lfs_txt TEXTEQU fslv_rec(SubExpr,<%lfs_txt3>,%lfs_stack_cntr)
		lfs_flg = TMT_ST
		IF lfs_flg3 AND TMT_NEG
			IFNB lfs_txt
				FSBE_PROC %FSBE_LOAD_REG,%lfs_txt,1
				lfs_txt TEXTEQU <>
			ENDIF
		ENDIF
	ELSEIF lfs_flg3 AND (TMT_SINGLE_FLT OR TMT_SINGLE_INT OR TMT_SINGLE_ARG)
		lfs_flg = lfs_flg3
		lfs_txt TEXTEQU lfs_txt3
		fslv_call_fnc
		lfs_txt TEXTEQU <>
		lfs_stack_cntr = lfs_stack_cntr - 1	;; correction only
	ENDIF
endm

; return: lfs_txt = operator, lfs_flg = flags = vaild if lfs_txt not blank -> TMT_ST
fslv_load_larg macro
	IF lfs_flg3 AND TMT_TXT
		lfs_flg = 0
		IF lfs_flg3 AND TMT_ST
			;lfs_txt TEXTEQU FSBE_FNC(%FSBE_PREPARE_REG,%lfs_stack_cntr-(lfs_st_max-lfs_txt3)-1,%lfs_flg3 AND TMT_NEG)
			;lfs_flg = TMT_ST
			IF lfs_flg3 AND TMT_NEG
				FSBE_PROC %FSBE_LOAD_REG,%lfs_stack_cntr-(lfs_st_max-lfs_txt3)-1,1
				lfs_txt TEXTEQU <>
			ELSE
				lfs_txt TEXTEQU %lfs_stack_cntr-(lfs_st_max-lfs_txt3)-1
				lfs_flg = TMT_ST
			ENDIF
		ELSE
			lfs_txt TEXTEQU FSBE_FNC(%FSBE_PREPARE_ARG,%lfs_txt3,%lfs_flg3 AND TMT_NEG)
		ENDIF
	ELSEIF lfs_flg3 AND TMT_CONST
		lfs_txt TEXTEQU lfs_txt3
		lfs_flg = lfs_flg3
		lfs_txt TEXTEQU llfs_make_const()
		lfs_flg = 0
	ELSEIF lfs_flg3 AND TMT_REC
		lfs_txt TEXTEQU fslv_rec(SubExpr,<%lfs_txt3>,%lfs_stack_cntr)
		lfs_flg = TMT_ST
		IF lfs_flg3 AND TMT_NEG
			IFNB lfs_txt
				FSBE_PROC %FSBE_LOAD_REG,%lfs_txt,1
				lfs_txt TEXTEQU <>
			ENDIF
		ENDIF
	ELSEIF lfs_flg3 AND (TMT_SINGLE_FLT OR TMT_SINGLE_INT OR TMT_SINGLE_ARG)
		lfs_flg = lfs_flg3
		lfs_txt TEXTEQU lfs_txt3
		fslv_call_fnc
		lfs_txt TEXTEQU <>
		lfs_stack_cntr = lfs_stack_cntr - 1	;; correction only
	ENDIF
endm

fslv_sto macro SubExpr,refValue:req,RecCnt:req,nStack
	fss_nUse = lst_get_data(SubExpr,%(refValue+2),2)
	IF fss_nUse GT RecCnt AND RecCnt NE 0
		IF lfs_nss+nStack+1 LT FSO_N_STACK
			lfs_nss = lfs_nss + 1
			fss_index=iml_request_index(StkSlot)
			lst_set_data SubExpr,%(refValue+2),2,%(fss_nUse-RecCnt)
			lst_set_data SubExpr,%(refValue+2),3,%fss_index
			lst_set_data SubExpr,%(refValue+2),4,<1>					; type = stack
			FSBE_PROC %FSBE_SET_REG,%(nStack+fss_index),0
		ELSE
			fslv_glb_error_msg TEXTEQU <[#CG7] register-stack overflow>
			fslv_glb_error = 1
		ENDIF
	ELSEIF RecCnt EQ fss_nUse
		; hold
		lst_set_data SubExpr,%(refValue+2),2,0
	ELSEIF RecCnt EQ 0
		IF lfs_nss+nStack+1 LT FSO_N_STACK
			lfs_nss = lfs_nss + 1
			fss_index=iml_request_index(StkSlot)
			lst_set_data SubExpr,%(refValue+2),3,%fss_index
			lst_set_data SubExpr,%(refValue+2),4,<1>					; type = stack
			;IF nStack+fss_index NE 1
				FSBE_PROC %FSBE_SET_REG,%(nStack+fss_index),1
				lfs_stack_cntr = lfs_stack_cntr - 1
			;ENDIF
		ELSE
			fslv_glb_error_msg TEXTEQU <[#CG8] register-stack overflow>
			fslv_glb_error = 1
		ENDIF
	ENDIF
endm

fslv_rec macro SubExpr,refValue:req,nStack
	fsr_index = lst_get_data(SubExpr,%(refValue+2),3)
	fsr_type = lst_get_data(SubExpr,%(refValue+2),4)
	fsr_val=lst_dec_value(SubExpr,%(refValue+2),2)
	IF fsr_type
		IF fsr_val EQ 0
			lfs_nss = lfs_nss - 1
			iml_free_index StkSlot,%fsr_index
			IF lfs_imm_free_regs NE 0
				FSBE_PROC %FSBE_FREE_REG,%nStack+fsr_index
			ENDIF
		ENDIF
		EXITM @CatStr(%nStack+fsr_index)
	ELSE
		fslv_glb_error_msg TEXTEQU <[#CG9] internal error: slot-type = memory>
		fslv_glb_error = 1
	ENDIF
endm

llfs_is_idn macro param1,param2
	IFIDNI <param1>,<param2>
		EXITM <1>
	ENDIF
	EXITM <0>	
endm

llfs_load_const macro
	
	IF lfs_flg AND TMT_PI
		FSBE_PROC %FSBE_LOAD_CONST,%(lfs_flg AND TMT_NEG),<pi>
	ELSE
		IF FSBE_FNC(%FSBE_IS_TKN_CONST,<%lfs_txt>)
			FSBE_PROC %FSBE_LOAD_CONST,%(lfs_flg AND TMT_NEG),%fsbeic_const_name
		ELSE
			IF lfs_flg AND TMT_INT
				IF fslv_lcl_int_type EQ 2
					llfsmc_var TEXTEQU @CatStr(<fslv_int2_>,%lfs_lbl)
					llfsmc_type TEXTEQU <SWORD>
				ELSEIF fslv_lcl_int_type EQ 4
					llfsmc_var TEXTEQU @CatStr(<fslv_int4_>,%lfs_lbl)
					llfsmc_type TEXTEQU <SDWORD>
				ELSE
					llfsmc_var TEXTEQU @CatStr(<fslv_int8_>,%lfs_lbl)
					llfsmc_type TEXTEQU <SQWORD>
				ENDIF
			ELSE
				llfsmc_var TEXTEQU @CatStr(<fslv_real>,%fslv_lcl_real_type,<_>,%lfs_lbl)
				llfsmc_type TEXTEQU @CatStr(<REAL>,%fslv_lcl_real_type)
			ENDIF
	
			llfsmc_var TEXTEQU @fSlvCreateConstant(%llfsmc_var,%lfs_txt,%llfsmc_type)
	
			FSBE_PROC %FSBE_LOAD_ARG,%llfsmc_var,%(lfs_flg AND TMT_NEG)
	
			lfs_lbl = lfs_lbl + 1
		ENDIF
	ENDIF
endm

llfs_make_const macro
	IF lfs_flg AND TMT_PI
		EXITM FSBE_FNC(%FSBE_PREPARE_CONST,%(lfs_flg AND TMT_NEG),<pi>)
	ELSE
		IF FSBE_FNC(%FSBE_IS_TKN_CONST,<%lfs_txt>)
			EXITM FSBE_FNC(%FSBE_PREPARE_CONST,%(lfs_flg AND TMT_NEG),%fsbeic_const_name)
		ELSE
			IF lfs_flg AND TMT_INT
				IF fslv_lcl_int_type EQ 2
					llfsmc_var TEXTEQU @CatStr(<fslv_int2_>,%lfs_lbl)
					llfsmc_type TEXTEQU <SWORD>
				ELSEIF fslv_lcl_int_type EQ 4
					llfsmc_var TEXTEQU @CatStr(<fslv_int4_>,%lfs_lbl)
					llfsmc_type TEXTEQU <SDWORD>
				ELSE
					llfsmc_var TEXTEQU @CatStr(<fslv_int8_>,%lfs_lbl)
					llfsmc_type TEXTEQU <SQWORD>
				ENDIF
			ELSE
				llfsmc_var TEXTEQU @CatStr(<fslv_real>,%fslv_lcl_real_type,<_>,%lfs_lbl)
				llfsmc_type TEXTEQU @CatStr(<REAL>,%fslv_lcl_real_type)
			ENDIF		
			
			llfsmc_var TEXTEQU @fSlvCreateConstant(%llfsmc_var,%lfs_txt,%llfsmc_type)
			
			lfs_lbl = lfs_lbl + 1
			IF lfs_flg AND TMT_NEG
				FSBE_PROC %FSBE_LOAD_ARG,%llfsmc_var,1
			ELSE
				EXITM llfsmc_var
			ENDIF		
		ENDIF
	ENDIF
	EXITM <>
endm

llfs_is_fnc_def macro FncName:req
	IFDEF fslv_fnc_&FncName&_dscptr
		EXITM <1>
	ENDIF
	EXITM <0>
endm

llfs_get_fnc_nargs macro FncName:req
	EXITM fslv_fnc_&FncName&_dscptr(%FNCD_NARGS)
endm

llfs_get_fnc_nregs macro FncName:req
	EXITM fslv_fnc_&FncName&_dscptr(%FNCD_NREGS)
endm


fscc_nconsts = 0
fscc_nreused   = 0 ; for statistic purpose
fscc_nr_SWORD  = 0 ;
fscc_nr_SDWORD = 0 ;
fscc_nr_SQWORD = 0 ;
fscc_nr_REAL4  = 0 ;
fscc_nr_REAL8  = 0 ;
fscc_nr_REAL10 = 0 ;
fscc_nc_SWORD  = 0 ;
fscc_nc_SDWORD = 0 ;
fscc_nc_SQWORD = 0 ;
fscc_nc_REAL4  = 0 ;
fscc_nc_REAL8  = 0 ;
fscc_nc_REAL10 = 0 ;

; use ONLY!: SWORD DWORD QWORD REAL4 REAl8 REAL10
; REAL4/8/10 must be written as FP-values
@fSlvCreateConstant macro name:req,value:req,_type:req
	
	fscc_value TEXTEQU @TrimStr(<&value>)
	fscc_type TEXTEQU <&_type>
	fscc_cntr = 0
	fscc_match = 0
	IF lfs_no_int_consts NE 0 AND @InStr(1,<SWORD SDWORD SQWORD>,%fscc_type) NE 0
		IF fslv_lcl_real_type EQ 8
			;IF value LT -9007199254740992 OR value GT 9007199254740992
			;	.err <integer value maybe not exact representable as REAL8: value>
			;ELSE
				fscc_type TEXTEQU <REAL8>
				fscc_value TEXTEQU @CatStr(%fscc_value+0),<.0>
			;ENDIF
		ELSEIF fslv_lcl_real_type EQ 4
			IF value LT -16777216 OR value GT 16777216
				fslv_glb_error_msg TEXTEQU <[#CG10] integer value maybe not exact representable as REAL4: value>
				fslv_glb_error = 1
				EXITM fslv_retval@error(REAL4)
			ELSE
				fscc_type TEXTEQU <REAL4>
				fscc_value TEXTEQU @CatStr(%fscc_value+0),<.0>
			ENDIF
		ELSE
			fslv_glb_error_msg TEXTEQU <[#CG11] integer to REAL10 not implemented>
			fslv_glb_error = 1
			EXITM fslv_retval@error(REAL8)
		ENDIF
	ELSEIF lfs_no_real_consts NE 0 AND @InStr(1,<SWORD SDWORD SQWORD>,%fscc_type) EQ 0
		fslv_glb_error_msg TEXTEQU <[#CG12] floating-point constants are not supported by current back end>
		fslv_glb_error = 1
		EXITM fslv_retval@error(_type)
	ENDIF
	
	IFE @fslv_test_attribute(<no const reuse>) OR @fslv_test_attribute(<no reuse>)
		IFE (fslv_glb_flags AND FSF_REUSE_BY_TYPE)
			WHILE fscc_cntr LT fscc_nconsts
			%	IFIDNI <fscc_value>,<@CatStr(<fscc_const_value_>,%fscc_cntr)>
					fscc_txt TEXTEQU @CatStr(<fscc_const_type_>,%fscc_cntr)
					fscc_tpos1 INSTR 1,<SWORD SDWORD SQWORD REAL4 REAL8 REAL10>,fscc_txt
					fscc_tpos2 INSTR 1,<SWORD SDWORD SQWORD REAL4 REAL8 REAL10>,fscc_type
					IF fscc_tpos1 GE 21 AND fscc_tpos2 GE 21 AND fscc_tpos2 EQ fscc_tpos1
						fscc_match=1
						EXITM
					ELSEIF fscc_tpos1 NE 0 AND fscc_tpos2 NE 0 AND fscc_tpos1 LE 14 AND fscc_tpos2 LE 14 AND fscc_tpos2 GE fscc_tpos1
						fscc_match=1
						EXITM
					ENDIF
				ENDIF
				fscc_cntr=fscc_cntr+1
			ENDM
		ELSE
			WHILE fscc_cntr LT fscc_nconsts
			%	IFIDNI <fscc_type>,<@CatStr(<fscc_const_type_>,%fscc_cntr)>
				%	IFIDNI <fscc_value>,<@CatStr(<fscc_const_value_>,%fscc_cntr)>
						fscc_match=1
						EXITM
					ENDIF
				ENDIF
				fscc_cntr=fscc_cntr+1
			ENDM
		ENDIF
	ENDIF
	IF fscc_match
		fscc_nreused=fscc_nreused+1
		@CatStr(<fscc_nr_>,%@CatStr(<fscc_const_type_>,%fscc_cntr)) = @CatStr(<fscc_nr_>,%@CatStr(<fscc_const_type_>,%fscc_cntr)) + 1
		EXITM @CatStr(<fscc_const_name_>,%fscc_cntr)
	ELSE
		.const
			fscc_align = @InStr(1,<SWORD SDWORD SQWORD REAL4 REAL8 REAL10>,%fscc_type)
			IF fscc_align
				fscc_align = @SubStr(<50000010000002000000100000200000400000>,%fscc_align,1)
			ENDIF
			IFE fscc_align
			 	fslv_glb_error_msg TEXTEQU <[#CG13] invalid type qualifier:>,fscc_type
			 	fslv_glb_error = 1
			 	EXITM fslv_retval@error(%fscc_type)
			ELSE
				IF fscc_align NE 5
					align fscc_align*4
				ELSE
					align 2
				ENDIF
			ENDIF

			IFIDNI fscc_type,<SWORD>
				IF value LT -32768 OR value GT 32767
					fslv_glb_error_msg TEXTEQU <[#CG14] constant !value out of range (SWORD): &value : change local or global precision to SDWORD>
					fslv_glb_error = 1
					EXITM fslv_retval@error(SWORD)
				ENDIF
				name _type value
			ELSEIFIDNI fscc_type,<SDWORD>
				IF value LT -2147483648 OR value GT 2147483647
					fslv_glb_error_msg TEXTEQU <[#CG15] constant !value out of range (SDWORD): &value : change local or global precision to SQWORD>
					fslv_glb_error = 1
					EXITM fslv_retval@error(SDWORD)
				ENDIF
				name _type value
			ELSEIFIDNI fscc_type,<SQWORD>
				IF fslv_glb_flags AND FSF_CHECK_SQWORD
					IFE chk_sqword(&value)
						fslv_glb_error_msg TEXTEQU <[#CG16] constant !value out of range (SQWORD): &value : switch to floating point constant>
						fslv_glb_error = 1
						EXITM fslv_retval@error(QWORD)
					ENDIF
				ENDIF
				name QWORD value
			ELSE
				name fscc_type fscc_value
			ENDIF
		.code
		@CatStr(<fscc_nc_>,%fscc_type) = @CatStr(<fscc_nc_>,%fscc_type) + 1
		@CatStr(<fscc_const_value_>,%fscc_nconsts) TEXTEQU fscc_value
		@CatStr(<fscc_const_type_>,%fscc_nconsts) TEXTEQU fscc_type
		@CatStr(<fscc_const_name_>,%fscc_nconsts) TEXTEQU <&name>
		fscc_nconsts=fscc_nconsts+1
		EXITM <&name>
	ENDIF
endm

chk_sqword macro num:req
	FOR numtxt,<&num>
		chksq_num TEXTEQU <&numtxt>
	ENDM
	chksq_char SUBSTR chksq_num,1,1
	chksq_max TEXTEQU <9223372036854775807>
	IFIDN chksq_char,<+>
		chksq_num SUBSTR chksq_num,2
	ELSEIFIDN chksq_char,<->
		chksq_num SUBSTR chksq_num,2
		chksq_max TEXTEQU <9223372036854775808>
	ENDIF
	
	chksq_size1 SIZESTR chksq_num
	chksq_size2 SIZESTR <9223372036854775807>
	IF chksq_size1 LT chksq_size2
		EXITM <1>
	ELSEIF chksq_size1 GT chksq_size2
		EXITM <0>
	ENDIF
	chksq_result = 1
	chksq_pos = 1
%	FORC char,<chksq_num>
		chksq_char SUBSTR chksq_max,chksq_pos,1
		IFE @InStr(1,<0123456789>,%chksq_char) ;; probably hexa-decimal number
			chksq_result=0 
			EXITM
		ENDIF
		IF char GT chksq_char
			chksq_result=0
			EXITM
		ENDIF
		chksq_pos=chksq_pos+1
	ENDM
	EXITM @CatStr(%chksq_result)
endm

fslv_extract_attributes macro args:VARARG
	fsea_size SIZESTR <&args>
	fsea_found = 0
	IFE fsea_size
		EXITM
	ENDIF
	fsea_pos INSTR 1,<&args>,<{>
	IFE fsea_pos
		EXITM
	ELSE
		fsea_pos2 INSTR fsea_pos,<&args>,<}>
		fsea_attributes SUBSTR <&args>,fsea_pos+1,fsea_pos2-fsea_pos-1
		IF fsea_pos EQ 1
			fsea_expr SUBSTR <&args>,fsea_pos2+1
		ELSEIF fsea_pos2 GE fsea_size
			fsea_expr SUBSTR <&args>,1,fsea_pos-1
		ELSE
			fsea_expr CATSTR @SubStr(<&args>,1,%fsea_pos-1),@SubStr(<&args>,%fsea_pos2+1)
		ENDIF
		fsea_found = 1
	ENDIF
endm

fslv_local_type_from_attributes macro def_real_type:req,def_int_type:req,attributes:VARARG

fslv_lcl_real_type = def_real_type
fslv_lcl_int_type  = def_int_type

flta_int = 0
flta_real = 0
	FOR attr,<attributes>
		flta_size SIZESTR <&attr>
		IF @InStr(1,<i2 i4 i8>,<&attr>) NE 0 AND flta_size EQ 2
			IFE flta_int
				flta_int = 1
				fslv_lcl_int_type = @SubStr(<&attr>,2,1)
			ELSE
				fslv_glb_error_msg TEXTEQU <[#CG17] integer-type-attribute found two or more times>
				fslv_glb_error = 1
			ENDIF
		ELSEIF @InStr(1,<r4 r8 r10>,<&attr>) NE 0 AND (flta_size EQ 3 OR flta_size EQ 2)
			IFE flta_real
				flta_real = 1
				fslv_lcl_real_type = @SubStr(<&attr>,2)
			ELSE
				fslv_glb_error_msg TEXTEQU <[#CG16] real-type-attribute found two or more times>
				fslv_glb_error = 1
			ENDIF
		ENDIF
	ENDM
endm

; sepeartor = optional char. (exact 1) seperating attribute and value e.g. <name:value>
@fslv_test_attribute macro name:req,separator,default_value
	fsta_found = 0
	fsta_attr TEXTEQU <&name>
	fsta_value TEXTEQU <&default_value>
	IF fslv_attributes?
	%	FOR attr,<fslv_current_attributes>
			IFB <&separator>
				IFIDNI <&name>,<&attr>
					fsta_found=1
					EXITM
				ENDIF
			ELSE
				IFNB <&attr>
					fsta_pos INSTR 1,<&attr>,<&separator>
					IF fsta_pos GT 1
						fsta_attr SUBSTR <&attr>,1,fsta_pos-1
						IFIDNI fsta_attr,<&name>
							IF fsta_pos+1 LE @SizeStr(<&attr>)
								fsta_value SUBSTR <&attr>,fsta_pos+1
							ENDIF
							fsta_found=1
							EXITM
						ENDIF
					ENDIF
				ENDIF
			ENDIF
		ENDM
	ENDIF
	EXITM @CatStr(%fsta_found)	
endm

@fslv_TLS? macro
	IFB fslv_tls_name
		EXITM <0>
%	ELSEIFDEF fslv_tls_name
		EXITM <1>
	ENDIF
	EXITM <0>
endm

@fslvTLS_line_alloc macro _type:req
	IFB fslv_tls_name
		fslv_glb_error_msg TEXTEQU <[#CG17] thread local storage not initialized>
		fslv_glb_error = 1
		EXITM fslv_retval@error(_type)
%	ELSEIFNDEF fslv_tls_name
		fslv_glb_error_msg TEXTEQU <[#CG18] thread local storage not initialized>
		fslv_glb_error = 1
		EXITM fslv_retval@error(_type)
	ENDIF
	
	IF fslvTLS_curr_line NE @Line
		fslvTLS_curr_line = @Line
		fslv_tls_curr_used = 0
		fslv_tls_byte = 0
	ENDIF
	
	fsta_size = FSBE_FNC(%FSBE_ENTITY,%FSEN_TEST_TYPE,&_type)
	IFE fsta_size 
		fslv_glb_error_msg TEXTEQU <[#CG19] invalid type qualifier / type not supported by current back end: _type>
		fslv_glb_error = 1
		EXITM fslv_retval@error(_type)
	ENDIF
	IF fsta_size EQ 12
		fsta_align = 4
	ELSE
		fsta_align = fsta_size
	ENDIF
	IF fsta_align GT @WordSize
		fsta_align = @WordSize
	ENDIF

	IF ((fslv_tls_curr_used+(fsta_align-1))AND(-fsta_align))+fsta_size GT fslv_tls_cb
	%	.err <[#CG20] TLS: buffer to small - increase size at line @CatStr(%fslv_tls_alloc_line) : fSlvTLS[,@CatStr(%((fslv_tls_curr_used+(fsta_align-1))AND(-fsta_align))+fsta_size)]>
		fslv_glb_error = 1
		fslv_tls_curr_used = ((fslv_tls_curr_used+(fsta_align-1))AND(-fsta_align)) + fsta_size
		EXITM fslv_retval@error(_type)
	ENDIF
	fsta_var TEXTEQU <&_type ptr >,fslv_tls_name,<[>,%((fslv_tls_curr_used+(fsta_align-1))AND(-fsta_align)),<]>
	fslv_tls_curr_used = ((fslv_tls_curr_used+(fsta_align-1))AND(-fsta_align)) + fsta_size
	EXITM fsta_var
endm

fslv_tls_warning_line = -1
@TLS_warning macro
	IF FSLV_SUPPRESS_ALL_WARNINGS EQ 0 AND FSLV_SUPPRESS_TS_WARNIGNS EQ 0 AND fslv_tls_warning_line NE @Line
		%echo @FileCur: WARNING: code not thread save: line @CatStr(%@Line)
	ENDIF
	fslv_tls_warning_line = @Line
endm

_discard_ macro foo
endm

fslv_retval@error macro _type
LOCAL lbl
	.data?
		lbl _type ?
	.code
	EXITM <lbl>
endm