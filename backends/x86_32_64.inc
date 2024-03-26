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

FSBE_REGISTER_BACKEND <FSLV_X86_ENTITY> 

FSLV_X86_ENTITY macro code:req,param1,param2
	IF code EQ FSEN_INIT
		IFE FSLV_SUPPRESS_ALL_WARNINGS
			IFNDEF fsxe_IBEI
				echo -----------------------------------------------------------
				echo
				echo WARNING: the Integer back end should not be used seriously.
				echo
				echo -----------------------------------------------------------
				fsxe_IBEI = 0
			ENDIF
		ENDIF
		IF fslv_lcl_int_type GT @WordSize
			fslv_glb_error_msg TEXTEQU <The current back end does not support QWORDs - change the local integer size by using the Attribute i4>
			fslv_glb_error = 1
			EXITM <0>
		ENDIF
		fsx86_eax_is_dest = 0
		fsx86_edx_is_dest = 0
		
		push T_REG(eax)
		push T_REG(edx)
		sub T_REG(esp),8*@WordSize ; stack slots
		EXITM <-1>
	ELSEIF code EQ FSEN_END
		add T_REG(esp),8*@WordSize
		IF fsx86_eax_is_dest NE 0
			pop T_REG(edx)
			add T_REG(esp),@WordSize
		ELSEIF fsx86_edx_is_dest NE 0
			add T_REG(esp),@WordSize
			pop T_REG(eax)
		ELSE
			pop T_REG(edx)
			pop T_REG(eax)
		ENDIF
		EXITM <0>
	ELSEIF code EQ FSEN_MAX_STACK
		EXITM <8>
	ELSEIF code EQ FSEN_STCK_USAGE
		EXITM <0>
	ELSEIF code EQ FSEN_MAPPING
		FSBE_ENTITY				TEXTEQU <FSLV_X86_ENTITY>
		FSBE_ENUM_TKN			TEXTEQU <>          
		FSBE_IS_TKN_REG       	TEXTEQU <FSLV_X86_IS_TKN_REG>
		FSBE_IS_TKN_CONST		TEXTEQU <FSLV_X86_IS_TKN_CONST>     
		FSBE_LOAD_CONST       	TEXTEQU <FSLV_X86_LOAD_CONST>
		FSBE_PREPARE_CONST		TEXTEQU <FSLV_X86_PREPARE_CONST>   
		FSBE_LOAD_ARG         	TEXTEQU <FSLV_X86_LOAD_ARG>
		FSBE_PREPARE_ARG      	TEXTEQU <FSLV_X86_PREPARE_ARG>
		FSBE_LOAD_REG         	TEXTEQU <FSLV_X86_LOAD_REG>
		FSBE_REG0_OPRT_MEM    	TEXTEQU <FSLV_X86_REG0_OPRT_MEM>      
		FSBE_MEM_OPRT_REG0    	TEXTEQU <FSLV_X86_MEM_OPRT_REG0>		
		FSBE_REG0_OPRT_REG1_P 	TEXTEQU <FSLV_X86_REG0_OPRT_REG1_P>
		FSBE_REG0_OPRT_REGX   	TEXTEQU <FSLV_X86_REG0_OPRT_REGX>  
		FSBE_REG1_OPRT_REG0_P 	TEXTEQU <FSLV_X86_REG1_OPRT_REG0_P>
		FSBE_REGX_OPRT_REG0  	TEXTEQU <FSLV_X86_REGX_OPRT_REG0>
		FSBE_FREE_REG         	TEXTEQU <FSLV_X86_FREE_REG>
		FSBE_SET_REG          	TEXTEQU <FSLV_X86_SET_REG>
		FSBE_ASSIGN_DEST		TEXTEQU <FSLV_X86_ASSIGN_DEST>             
		FSBE_REG0_OPRT_RCVAL 	TEXTEQU <FSLV_X86_REG0_OPRT_RCVAL>
		FSBE_NEG              	TEXTEQU <FSLV_X86_NEG>       
		FSBE_XCHG             	TEXTEQU <FSLV_X86_XCHG>
		FSBE_FNC_DSCPTR			TEXTEQU <FSLV_X86_FNC_DSCPTR>
		FSBE_CALL_FNC			TEXTEQU <FSLV_X86_CALL_FNC>
		fslv_glb_instrset = 0
		EXITM <-1>
	ELSEIF code EQ FSEN_INSTRSET
		IFIDNI <x86>,<&param1>
			EXITM <-1>
		ELSEIFIDNI <x64>,<&param1>
			EXITM <-1>
		ELSE
			EXITM <0>
		ENDIF
	ELSEIF code EQ FSEN_ALL_INSTRSETS
		EXITM <x86,x64>
	ELSEIF code EQ FSEN_REQ_RETVAL
		EXITM <1>
	ELSEIF code EQ FSEN_TEST_TYPE
		IF @WordSize EQ 4
			EXITM @GetArgByIndex(@MatchStrI(param1,0,SBYTE,SWORD,SDWORD),0,0,1,2,4)
		ELSE
			EXITM @GetArgByIndex(@MatchStrI(param1,0,SBYTE,SWORD,SDWORD,SQWORD),0,0,1,2,4,8)
		ENDIF
	ELSEIF code EQ FSEN_NO_REAL_CONST
		EXITM <-1>
	ELSEIF code EQ FSEN_NAME
		fsbe_txt TEXTEQU <x86-32/64>
		EXITM <-1>
	ELSE
		EXITM <0>
	ENDIF
endm

FSLV_X86_FNC_DSCPTR macro fnc_name,flags,bTest:=<0>
	IFE bTest
		IF flags AND FNCD_VERSION
			EXITM <2>
		ELSEIF flags AND FNCD_NARGS
			IF @MatchStrI(<&fnc_name>,0,min,max,mod)
				EXITM <2>
			ENDIF
			EXITM <1>
		ELSEIF flags AND FNCD_NREGS
			EXITM <0>
		ELSEIF flags AND FNCD_ISTCK
			EXITM <-1>
		ELSEIF flags AND FNCD_X64
			EXITM <-1>
		ELSE
			EXITM <0>
		ENDIF
	ELSE
		EXITM @MatchStrI(<&fnc_name>,0,abs,sgn,min,max,mod)
	ENDIF	
endm

FSLV_X86_CALL_FNC macro fnc_name
	IFIDNI <fnc_name>,<abs>
		.if T_INT ptr T_REG(eax) < 0
			neg T_REG(eax)
		.endif
	ELSEIFIDNI <fnc_name>,<sgn>
		sar T_REG(eax),31
		or T_REG(eax),1
	ELSEIFIDNI <fnc_name>,<min>
		.if T_REG(eax) > T_INT ptr [T_REG(esp)]
			pop T_REG(eax)
		.else
			pop T_REG(edx)
		.endif
	ELSEIFIDNI <fnc_name>,<max>
		.if T_REG(eax) < T_INT ptr [T_REG(esp)]
			pop T_REG(eax)
		.else
			pop T_REG(edx)
		.endif
	ELSEIFIDNI <fnc_name>,<mod>
		T_EXPR(cdq,cqo)
		idiv T_INT ptr [T_REG(esp)]
		pop T_REG(eax)
		mov T_REG(eax),T_REG(edx)
	ELSE
		fslv_glb_error_msg TEXTEQU <function not supported by current back end: fnc_name>
		fslv_glb_error = 1
	ENDIF
endm

FSLV_X86_IS_TKN_REG macro tkn:=<>
	EXITM <0>
endm

FSLV_X86_IS_TKN_CONST macro tkn:=<>
	EXITM <0>
endm

FSLV_X86_SET_REG macro index,bPop:=<0>
	IF index NE 0
		mov T_INT ptr [T_REG(esp)][(index-1)*@WordSize],T_REG(eax)
	ENDIF
	IF bPop NE 0 AND lfs_stack_cntr NE 1
		pop T_REG(eax)
	ENDIF
endm

FSLV_X86_NEG macro 
	neg T_REG(eax)
endm

FSLV_X86_XCHG macro index:=<1>
	IF index NE 0
		xchg T_REG(eax),T_INT ptr [T_REG(esp)][(index-1)*@WordSize]
	ENDIF
endm

FSLV_X86_LOAD_REG macro index,neg_arg:=<0>
	IF lfs_stack_cntr NE 0
		push T_REG(eax)
	ENDIF
	mov T_REG(eax),T_INT ptr [T_REG(esp)][(index)*@WordSize]
	IF neg_arg
		neg T_REG(eax)
	ENDIF
endm

MOVSX_EX macro reg,oprt
	IF @WordSize EQ 8
		IF type ( oprt ) EQ 4
			movsxd reg,oprt
		ELSE
			movsx reg,oprt
		ENDIF
	ELSE
		movsx reg,oprt
	ENDIF	
endm

FSLV_X86_LOAD_ARG macro tkn,neg_arg:=<0>

	IF @IsGPReg(tkn)
		%_discard_ FSLV_X86_PREPARE_ARG(<&tkn>,neg_arg)
	ELSE
		IF lfs_stack_cntr NE 0
			push T_REG(eax)
		ENDIF
		IF @IsIntegerType(<&tkn>)
			IF iit_type GT @WordSize
				fslv_glb_error_msg TEXTEQU <only SBYTE, SWORD and SDWORD supported>
				fslv_glb_error = 1
			ELSEIF iit_type EQ @WordSize
				mov T_REG(eax),tkn
			ELSE
				MOVSX_EX T_REG(eax),tkn
			ENDIF
			IF neg_arg NE 0
				neg T_REG(eax)
			ENDIF
		ELSE
			fslv_glb_error_msg TEXTEQU T_EXPR(<only SBYTE, SWORD and SDWORD supported>,<only SBYTE, SWORD, SDWORD and SQWORD supported>)
			fslv_glb_error = 1
		ENDIF
	ENDIF
endm

FSLV_X86_PREPARE_ARG macro tkn,neg_arg:=<0>
	IF @IsGPReg(tkn)
		IF lfs_stack_cntr NE 0
			push T_REG(eax)
		ENDIF
		IF @CmpBaseGPR(eax)
			IF igpr_size EQ @WordSize
				mov T_REG(eax),T_INT ptr [T_REG(esp)+lfs_stack_cntr*@WordSize+9*@WordSize]
			ELSE
				MOVSX_EX T_REG(eax),@GetArgByIndex(%igpr_size/2,,SBYTE,SWORD,SDWORD) ptr [T_REG(esp)+lfs_stack_cntr*@WordSize+9*@WordSize]
			ENDIF
		ELSE
			IF @CmpBaseGPR(edx)
				IF igpr_size EQ @WordSize
					mov T_REG(eax),T_INT ptr [T_REG(esp)+lfs_stack_cntr*@WordSize+8*@WordSize]
				ELSE
					MOVSX_EX T_REG(eax),@GetArgByIndex(%igpr_size/2,,SBYTE,SWORD,SDWORD) ptr [T_REG(esp)+lfs_stack_cntr*@WordSize+8*@WordSize]
				ENDIF
			ELSEIF igpr_size EQ @WordSize
				mov T_REG(eax),tkn
			ELSE
				MOVSX_EX T_REG(eax),tkn
			ENDIF
		ENDIF
		IF neg_arg NE 0
			neg T_REG(eax)
		ENDIF
		EXITM <>
	ELSE
		EXITM <&tkn>
	ENDIF
endm

FSLV_X86_LOAD_CONST macro neg_arg:=<0>,name
	IFIDNI <name>,<pi>
		fslv_glb_error_msg TEXTEQU <constant PI not supported>
		fslv_glb_error = 1
	ENDIF
endm

FSLV_X86_PREPARE_CONST macro neg_arg:=<0>,name
	IFIDNI <name>,<pi>
		fslv_glb_error_msg TEXTEQU <constant PI not supported>
		fslv_glb_error = 1
	ENDIF
	EXITM <>
endm


; st(0) ^ {-1, +-2, +-3, +-0.5} 
; powflg: flg-value   exponent value
;               1   =  -1
;               2   =   2
;               3   =   3
;               4   =  -2
;               5   =  -3
;               8   =  0.5
;               9   =  -0.5
FSLV_X86_REG0_OPRT_RCVAL macro oprt,tkn,neg_tkn
	IFIDN <&oprt>,<^>
		fxsfror_pos INSTR 1,<2.0 3.0-1.0-2.0-3.0 0.5-0.5>,<&tkn>
		IF fxsfror_pos
			IF @SizeStr(<&tkn>) GE @SubStr(<100010020002000200003004000>,%fxsfror_pos,1)
				fxsfror_powflg = @SubStr(<200030010004000500008009000>,%fxsfror_pos,1)
				IF fxsfror_powflg EQ 2
					imul T_REG(eax),T_REG(eax)
				ELSEIF fxsfror_powflg EQ 3
					mov T_REG(edx),T_REG(eax)
					imul T_REG(eax),T_REG(eax)
					imul T_REG(eax),T_REG(edx)
				ELSEIF fxsfror_powflg EQ 8 OR fxsfror_powflg EQ 9
					fslv_glb_error_msg TEXTEQU <integer square root not implemented>
					fslv_glb_error = 1
				ENDIF
				IF (fxsfror_powflg EQ 4 OR fxsfror_powflg EQ 5 OR fxsfror_powflg EQ 1 OR fxsfror_powflg EQ 9) XOR (neg_tkn NE 0)
					fslv_glb_error_msg TEXTEQU <not supported: y^-x>
					fslv_glb_error = 1
				ENDIF
				EXITM <-1>
			ENDIF
		ENDIF
	ELSEIF @InStr(1,</*>,<&oprt>)
		IF @InStr(1,<1.0>,<&tkn>) EQ 1
			IF neg_tkn
				FSLV_X86_NEG
			ENDIF
			EXITM <-1>
		ELSEIF @InStr(1,<-1.0>,<&tkn>) EQ 1
			IFE neg_tkn
				FSLV_X86_NEG
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
							IFIDN <&oprt>,</>
								shr T_REG(eax),1
							ELSE
								shl T_REG(eax),1
							ENDIF
						ELSEIF fxsfror_pos EQ 9
							IFIDN <&oprt>,</>
								shr T_REG(eax),2
							ELSE
								shl T_REG(eax),2
							ENDIF
						ELSEIF fxsfror_pos EQ 25
							IFIDN <&oprt>,</>
								shr T_REG(eax),3
							ELSE
								shl T_REG(eax),3
							ENDIF
						ELSE
							EXITM <0>
						ENDIF
						IF fxsfror_pos GE 50
							neg T_REG(eax)
						ENDIF
						EXITM <-1>
					ENDIF
				ENDIF
			ENDIF
		ENDIF
	ENDIF
	EXITM <0>	
endm

FSLV_X86_REG1_OPRT_REG0_P macro tkn
	IF _IFDIF$(<&tkn>,<+>,-1,0) AND _IFDIF$(<&tkn>,<*>,-1,0)
		FSLV_X86_XCHG
	ENDIF
	FSLV_X86_REG0_OPRT_REGX <&tkn>,1
	IF lfs_stack_cntr NE 1
		add T_REG(esp),@WordSize
	ENDIF
endm

FSLV_X86_REG0_OPRT_REG1_P macro tkn
	FSLV_X86_REG0_OPRT_REGX <&tkn>,1
	IF lfs_stack_cntr NE 1
		add T_REG(esp),@WordSize
	ENDIF
endm

FSLV_X86_REG0_OPRT_REGX macro oprt,index
	IFIDN <&oprt>,<+>
		add T_REG(eax),_IF$(index EQ 0,T_REG(eax),T_INT ptr [T_REG(esp)][(index-1)*@WordSize])
	ELSEIFIDN <&oprt>,<->
		sub T_REG(eax),_IF$(index EQ 0,T_REG(eax),T_INT ptr [T_REG(esp)][(index-1)*@WordSize])
	ELSEIFIDN <&oprt>,<*>
		imul T_REG(eax),_IF$(index EQ 0,T_REG(eax),T_INT ptr [T_REG(esp)][(index-1)*@WordSize])
	ELSEIFIDN <&oprt>,</>
		T_EXPR(cdq,cqo)
		idiv _IF$(index EQ 0,T_REG(eax),T_INT ptr [T_REG(esp)][(index-1)*4])
	ELSEIFIDN <&oprt>,<^>
		push T_REG(eax)
		mov T_REG(edx),1
		.while T_REG(edx) < _IF$(index EQ 0,T_INT ptr [T_REG(esp)],T_INT ptr [T_REG(esp)][(index)*@WordSize])
			imul T_REG(eax),T_INT ptr [T_REG(esp)]
			inc T_REG(edx)
		.endw
		add T_REG(esp),@WordSize
	ENDIF
endm

FSLV_X86_REGX_OPRT_REG0 macro oprt,index
	IFIDN <&oprt>,<+>
		add T_REG(eax),T_INT ptr [T_REG(esp)][(index-1)*@WordSize]
	ELSEIFIDN <&oprt>,<->
		neg T_REG(eax)
		add T_REG(eax),T_INT ptr [T_REG(esp)][(index-1)*@WordSize]
	ELSEIFIDN <&oprt>,<*>
		imul T_REG(eax),T_INT ptr [T_REG(esp)][(index-1)*@WordSize]
	ELSEIFIDN <&oprt>,</>
		xchg T_REG(eax),T_INT ptr [T_REG(esp)][(index-1)*@WordSize]
		T_EXPR(cdq,cqo)
		idiv T_INT ptr [T_REG(esp)][(index-1)*@WordSize]
	ELSEIFIDN <&oprt>,<^>
		push T_REG(eax)
		mov T_REG(eax),T_INT ptr [T_REG(esp)][(index)*@WordSize]
		mov T_REG(edx),1
		.while T_REG(edx) < T_INT ptr [T_REG(esp)]
			imul T_REG(eax),T_INT ptr [T_REG(esp)][(index)*@WordSize]
			inc T_REG(edx)
		.endw
		add T_REG(esp),@WordSize
	ENDIF
endm

FSLV_X86_REG0_OPRT_MEM macro tkn,oprt,int_type,real_type
	
	IF real_type NE 0
		fslv_glb_error_msg TEXTEQU <no REAL types supported by current back end>
		fslv_glb_error = 1
	ELSEIF int_type GT @WordSize
		fslv_glb_error_msg TEXTEQU <QWORDs not supported by current back end>
		fslv_glb_error = 1
	ENDIF
	IFIDN <&oprt>,<+>
		IF int_type EQ @WordSize
			add T_REG(eax),tkn
		ELSE
			MOVSX_EX T_REG(edx),tkn
			add T_REG(eax),T_REG(edx)
		ENDIF
	ELSEIFIDN <&oprt>,<->
		IF int_type EQ @WordSize
			sub T_REG(eax),tkn
		ELSE
			MOVSX_EX T_REG(edx),tkn
			sub T_REG(eax),T_REG(edx)
		ENDIF
	ELSEIFIDN <&oprt>,<*>
		IF int_type EQ @WordSize
			imul T_REG(eax),tkn
		ELSE
			MOVSX_EX T_REG(edx),tkn
			imul T_REG(eax),T_REG(edx)
		ENDIF
	ELSEIFIDN <&oprt>,</>
		IF int_type EQ @WordSize
			T_EXPR(cdq,cqo)
			idiv tkn
		ELSE
			MOVSX_EX T_REG(edx),tkn
			push T_REG(edx)
			T_EXPR(cdq,cqo)
			idiv T_INT ptr [T_REG(esp)]
			add T_REG(esp),@WordSize
		ENDIF
	ELSEIFIDN <&oprt>,<^>
		IF int_type EQ @WordSize
			push tkn
		ELSE
			MOVSX_EX T_REG(edx),tkn
			push T_REG(edx)
		ENDIF
		push T_REG(eax)
		mov T_REG(edx),1
		.while T_REG(edx) < T_UINT ptr [T_REG(esp)+@WordSize]
			imul T_REG(eax),T_INT ptr [T_REG(esp)]
			inc T_REG(edx)
		.endw
		add T_REG(esp),2*@WordSize
	ENDIF
endm


FSLV_X86_MEM_OPRT_REG0 macro tkn,oprt,int_type,real_type
	IF real_type NE 0
		fslv_glb_error_msg TEXTEQU <no REAL types supported by current back end>
		fslv_glb_error = 1
	ELSEIF int_type GT @WordSize
		fslv_glb_error_msg TEXTEQU <QWORDs not supported by current back end>
		fslv_glb_error = 1
	ENDIF
	IFIDN <&oprt>,<+>
		IF int_type EQ @WordSize
			add T_REG(eax),tkn
		ELSE
			MOVSX_EX T_REG(edx),tkn
			add T_REG(eax),T_REG(edx)
		ENDIF
	ELSEIFIDN <&oprt>,<->
		neg T_REG(eax)
		IF int_type EQ @WordSize
			add T_REG(eax),tkn
		ELSE
			MOVSX_EX T_REG(edx),tkn
			add T_REG(eax),T_REG(edx)
		ENDIF
	ELSEIFIDN <&oprt>,<*>
		IF int_type EQ @WordSize
			imul T_REG(eax),tkn
		ELSE
			MOVSX_EX T_REG(edx),tkn
			imul T_REG(eax),T_REG(edx)
		ENDIF
	ELSEIFIDN <&oprt>,</>
		push T_REG(eax)
		IF int_type NE @WordSize
			MOVSX_EX T_REG(eax),tkn
		ELSE
			mov T_REG(eax),tkn
		ENDIF
		T_EXPR(cdq,cqo)
		idiv T_INT ptr [T_REG(esp)]
		add T_REG(esp),@WordSize
	ELSEIFIDN <&oprt>,<^>
		push T_REG(eax)
		IF int_type NE @WordSize
			MOVSX_EX T_REG(eax),tkn
		ELSE
			mov T_REG(eax),tkn
		ENDIF
		push T_REG(eax)
		mov T_REG(edx),1
		.while T_REG(edx) < T_UINT ptr [T_REG(esp)+@WordSize]
			imul T_REG(eax),T_INT ptr [T_REG(esp)]
			inc T_REG(edx)
		.endw
		add T_REG(esp),2*@WordSize
	ENDIF
endm

FSLV_X86_FREE_REG macro index
endm

FSLV_X86_ASSIGN_DEST macro dest,dest_int_type,dest_real_type,bRetValue,retval_int_type,retval_real_type,fCopy,CpySrc
LOCAL retval

	IFNB <dest>
		IF @IsGPReg(<&dest>)
			IF @CmpBaseGPR(eax)
				fsx86_eax_is_dest = 1
			ENDIF
			IF @CmpBaseGPR(edx)
				fsx86_edx_is_dest = 1
			ENDIF
			IFE fsx86_eax_is_dest
				IF igpr_size EQ @WordSize
					mov dest,T_REG(eax)
				ELSE
					mov dest,@GetArgByIndex(%igpr_size/2,,al,ax,eax)
				ENDIF
			ENDIF
		ELSE
			IF dest_int_type EQ @WordSize
				mov dest,T_REG(eax)
			ELSEIF dest_int_type EQ 4
				mov dest,eax
			ELSEIF dest_int_type EQ 2
				mov dest,ax
			ELSEIF dest_int_type EQ 1
				mov dest,al
			ELSEIF dest_int_type EQ 8
				fslv_glb_error_msg TEXTEQU <QWORD not supported as destination>
				fslv_glb_error = 1
			ELSE
				fslv_glb_error_msg TEXTEQU <only integer types supported as destination>
				fslv_glb_error = 1
			ENDIF
		ENDIF
	ENDIF

	IFE bRetValue
		EXITM <>
	ELSE
		retval TEXTEQU FSBE_ALLOC_RETVAL()
		IF retval_int_type EQ @WordSize
			mov retval,T_REG(eax)
		ELSEIF retval_int_type EQ 4
			mov retval,eax
		ELSEIF retval_int_type EQ 2
			mov retval,ax
		ELSEIF retval_int_type EQ 8
			fslv_glb_error_msg TEXTEQU <QWORD not supported as return value>
			fslv_glb_error = 1
		ELSE
			fslv_glb_error_msg TEXTEQU <only integer types supported as return value>
			fslv_glb_error = 1
		ENDIF
		EXITM <retval>
	ENDIF
endm