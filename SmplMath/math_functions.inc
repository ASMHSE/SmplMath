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

comment *
  current implemented functions:
    Name(Arguments)     x64 FPU SSE2    Data types      Remarks
    sin(x)               X   X          R4/8/10
    cos(x)               X   X          R4/8/10
    tan(x)               X   X          R4/8/10
    atan(x)              X   X          R4/8/10
    atan2(y,x)           X   X          R4/8/10         atan(y/x)
    asin(x)              X   X          R4/8/10
    acos(x)              X   X          R4/8/10
    exp(x)               X   X          R4/8/10         e^x ; e = 2.71828...
    ln(x)                X   X          R4/8/10         base = e = 2.71828...
    log(x)               X   X          R4/8/10         base=10
    logbx(base,x)        X   X          R4/8/10
    root(x,n)            X   X          R4/8/10         n= +-(1,2,3,...)
    root2(x,a,b)         X   X          R4/8/10         a,b = +-(1,2,3,...) , result = x ^ (a/b)
    sqrt(x)              X   X   X      R[4/8]/10
    trunc(x)             X   X          R4/8/10 
    ceil(x)              X   X          R4/8/10 
    mod(a,b)             X   X          R4/8/10 
    abs(x)               X   X   X      R[4/8]/10
    limit(x,max,min)     X   X   X      R[4/8]/10
    deg(x)               X   X   X      R[4/8]/10       rad->deg
    rad(x)               X   X   X      R[4/8]/10       deg->rad
    sgn(x)               X   X   X      R[4/8]/10       returns singe of x: -1 or +1
    min(a,b)             X   X   X      R[4/8]/10
    max(a,b)             X   X   X      R[4/8]/10
    rsqrtss              X       X      R4              error less equal 1.5x2^-12
    rcpss                X       X      R4              error less equal 1.5x2^-12
    
    - Data types in square brackest are realted to SSE2
    - R4  = REAL4  = single precision (binary32)
    - R8  = REAL8  = double precision (binary64)
    - R10 = REAl10 = x86-87 extended precision (80 Bit)
*

SSE2_EXPD struct
    xMax     REAL8	?
    xMin     REAL8	?
    r_ln2_0  REAL8	?
    mln2_0   REAL8	?
    f1       REAL8	?
    f2       REAL8	?
    plusINF  REAL8	?
    NAN      REAL8	?
    P        REAL8	4 dup (?)
    mln2_1   REAL8	?
    tbl      REAL8	0 dup (?)
SSE2_EXPD ends

EXTERNDEF fpu_const_r4_one		:REAL4
EXTERNDEF fpu_const_r4_1div2	:REAL4
EXTERNDEF fpu_const_r8_pi_div_2	:REAL8
EXTERNDEF fpu_const_r10_2pi		:REAL10
EXTERNDEF fpu_const_r10_r2d		:REAL10
EXTERNDEF fpu_const_r10_d2r		:REAL10
EXTERNDEF fpu_const_r8_2pi		:REAL8
EXTERNDEF fpu_const_r4_2pi		:REAL4
EXTERNDEF fpu_const_r8_r2d		:REAL8
EXTERNDEF fpu_const_r4_r2d		:REAL4
EXTERNDEF fpu_const_r8_d2r		:REAL8
EXTERNDEF fpu_const_r4_d2r		:REAL4
EXTERNDEF sse2_sd_neg_msk		:OWORD
EXTERNDEF sse2_ss_neg_msk		:OWORD
EXTERNDEF sse2_ss_one			:REAL4
EXTERNDEF sse2_sd_one			:REAL8
EXTERNDEF sse2_sd_abs_msk		:OWORD
EXTERNDEF sse2_ss_abs_msk		:OWORD
EXTERNDEF sse2_sd_pi			:REAL8
EXTERNDEF sse2_ss_pi			:REAL4
EXTERNDEF sse2_sd_mpi			:REAL8
EXTERNDEF sse2_ss_mpi			:REAL4
EXTERNDEF sse2_ExpD				:SSE2_EXPD
EXTERNDEF sse2_log2sd_cmp1		:REAL8
EXTERNDEF sse2_log2sd_cmp2		:REAL8
EXTERNDEF sse2_log2sd_msk_1 	:REAL8
EXTERNDEF sse2_log2sd_m1		:REAL8
EXTERNDEF sse2_log2sd_msk_2 	:REAL8
EXTERNDEF sse2_log2sd_128		:REAL8
EXTERNDEF sse2_log2sd_P			:REAL8
EXTERNDEF sse2_log2sd_tbl		:REAL8

IF @WordSize EQ 4
	fpu_st1_pow_st0 proto stdcall
	fpu_exp proto stdcall
	fpu_root proto stdcall
	fpu_root2 proto stdcall
	fpu_fit2pi proto stdcall
	fpu_zero_test proto stdcall exp_diff:DWORD,bPop:DWORD
	fpu_is_integer proto stdcall
ELSEIF @WordSize EQ 8
	fpu_st1_pow_st0 proto 
	fpu_exp proto 
	fpu_root proto 
	fpu_root2 proto 
	fpu_fit2pi proto 
	fpu_zero_test proto exp_diff:DWORD,bPop:DWORD
	fpu_is_integer proto
ELSE
	.err <[#MF1]only x86-32 and x86-64 supported>
ENDIF
	
FNCD_VERSION			EQU 1
FNCD_NARGS				EQU 2
FNCD_NREGS				EQU 4	; number of additional used registers. param1 = combination of FSIS_FPU/SSE2/....
FNCD_X32				EQU 8   ;
FNCD_X64				EQU 16	; 
FNCD_ISTCK				EQU 32	; function supports the fslv_fnc_&FncName& macro 
FNCD_IREG				EQU 128	; function supports the fslv_fnc_ireg_&FncName& macro
FNCD_INSTR_SET			EQU 512	; function has a implementation for returned instruction sets
								; Return: a combination of the FSIS_xxx-flags that indicates the used instruction sets (SSE2, SSE3, SSSE3, SSE4.1, SSE4.2, SSE4.A, AVX, CVT16, FMA3, XOP)
FNCD_TST_LCL_TYPES		EQU 1024; Check if the function supports the types described by fslv_lcl_real_type and fslv_lcl_int_type
								; Return: none zero, if function has a implementation for the local type settings.
FNCD_IREG_RECORD_SIG	EQU 2048; place the macro call in a seperated code segment, record the signature and and reuse the recorded code if the signature of furhter function calls matchs.
								; The Signature consist of: register names and their order in argument list, free registers, instruction set and local type settings
								; This flags requires that FNCD_NREGS returns valid inforamtion.

default_fnc_dscptr macro FncName:req,nArgs:=<1>,nRegs:=<0>
	fslv_fnc_&FncName&_dscptr macro flags:req,param1:=<0>
		IF flags AND FNCD_VERSION
			EXITM <1>					; compatible with version 1
		ELSEIF flags AND FNCD_NARGS
			EXITM <&nArgs>				; number of arguments
		ELSEIF flags AND FNCD_NREGS
			EXITM <&nRegs>				; number of registers used additional to the ones used for agument passing
		ELSEIF flags AND FNCD_ISTCK
			EXITM <-1>
		ELSEIF flags AND FNCD_INSTR_SET
			EXITM <FSIS_FPU>
		ELSEIF flags AND FNCD_TST_LCL_TYPES
			EXITM <-1>
		ELSE
			EXITM <0>
		ENDIF
	endm
endm

type_depending_fnc_dscptr macro FncName:req,nArgs:=<1>,nRegs_r4:=<0>,nRegs_r8:=<0>,nRegs_r10:=<0>
	fslv_fnc_&FncName&_dscptr macro flags:req,param1:=<0>
		IF flags AND FNCD_VERSION
			EXITM <1>
		ELSEIF flags AND FNCD_NARGS
			EXITM <&nArgs>
		ELSEIF flags AND FNCD_NREGS
			IF fslv_lcl_real_type EQ 4
				EXITM <&nRegs_r4>
			ELSEIF fslv_lcl_real_type EQ 8
				EXITM <&nRegs_r8>
			ELSEIF fslv_lcl_real_type EQ 10
				EXITM <&nRegs_r10>
			ENDIF
		ELSEIF flags AND FNCD_ISTCK
			EXITM <-1>
		ELSEIF flags AND FNCD_INSTR_SET
			EXITM <FSIS_FPU>
		ELSEIF flags AND FNCD_TST_LCL_TYPES
			EXITM <-1>
		ELSE
			EXITM <0>
		ENDIF	
	endm
endm

; nRegs=(flags)::<list>
default_fnc_dscptr2 macro FncName:req,attributes:VARARG
	
	dfs2_v = 2
	dfs2_nRegs = -1
	dfs2_nArgs = 1
	dfs2_IStck = -1
	dfs2_IReg = 0
	dfs2_x32 = -1
	dfs2_x64 = 0
	dfs2_is = 0
	dfs2_nr_r4 = -1
	dfs2_nr_r8 = -1
	dfs2_nr_r10 = -1
	dfs2_nXR_r4 = -1
	dfs2_nXR_r8 = -1
	dfs2_IRegSig = 0
	dfs2_rt TEXTEQU <>
	dfs2_it TEXTEQU <>
	dfs2_nrm TEXTEQU <>
	
	FOR arg1,<attributes>
		dfs2_pos INSTR 1,<&arg1>,<=>
		IF dfs2_pos GE 2
			dfs2_size SIZESTR <&arg1>
			IF dfs2_size GT dfs2_pos
				dfs2_attr SUBSTR <&arg1>,1,dfs2_pos-1
				dfs2_value SUBSTR <&arg1>,dfs2_pos+1
				dfs2_attr TEXTEQU @TrimStr(%dfs2_attr)
				IFDIFI dfs2_attr,<realTypes>
					IFDIFI dfs2_attr,<intTypes>
						dfs2_value TEXTEQU @TrimStr(%dfs2_value)
					ENDIF
				ENDIF
				IFB dfs2_attr
					.err <[#MF2]invalid macro usage: expect: ...,attr=value,...>
					EXITM
				ENDIF
				IFB dfs2_value
					.err <[#MF3]invalid macro usage: expect: ...,attr=value,...>
					EXITM
				ENDIF

; MASM-bug: @ScanForFlt() makes MASM crazy			
;				IF @ScanForFlt(1,<%dfs2_value>,<blanks>) NE 2
;					.err <[#MF4]invalid value: integer value expected: arg1>
;					EXITM
;				ENDIF

				IFIDNI dfs2_attr,<version>
					dfs2_v = dfs2_value
				ELSEIFIDNI dfs2_attr,<nArgs>
					dfs2_nArgs = dfs2_value
				ELSEIFIDNI dfs2_attr,<nRegs>
					dfs2_nRegs = dfs2_value
				ELSEIFIDNI dfs2_attr,<IReg>
					dfs2_IReg = dfs2_value
				ELSEIFIDNI dfs2_attr,<IStck>
					dfs2_IStck = dfs2_value
				ELSEIFIDNI dfs2_attr,<x32>
					dfs2_x32 = dfs2_value
				ELSEIFIDNI dfs2_attr,<x64>
					dfs2_x64 = dfs2_value
				ELSEIFIDNI dfs2_attr,<InstrSet>
					dfs2_is = dfs2_is OR dfs2_value
				ELSEIFIDNI dfs2_attr,<sse2>
					dfs2_is = (dfs2_is AND (NOT FSIS_SSE2)) OR ((dfs2_value NE 0) AND 1)*FSIS_SSE2
				ELSEIFIDNI dfs2_attr,<fpu>
					dfs2_is = (dfs2_is AND (NOT FSIS_FPU)) OR ((dfs2_value NE 0) AND 1)*FSIS_FPU
				ELSEIFIDNI dfs2_attr,<nRegs_r4>
					dfs2_nr_r4 = dfs2_value
				ELSEIFIDNI dfs2_attr,<nRegs_r8>
					dfs2_nr_r8 = dfs2_value
				ELSEIFIDNI dfs2_attr,<nRegs_r10>
					dfs2_nr_r10 = dfs2_value
				ELSEIFIDNI dfs2_attr,<nXmmRegs_r4>
					dfs2_nXR_r4 = dfs2_value
				ELSEIFIDNI dfs2_attr,<nXmmRegs_r8>
					dfs2_nXR_r8 = dfs2_value
				ELSEIFIDNI dfs2_attr,<realTypes>
					dfs2_rt TEXTEQU dfs2_value
				ELSEIFIDNI dfs2_attr,<intTypes>
					dfs2_it TEXTEQU dfs2_value
				ELSEIFIDNI dfs2_attr,<nRegMacro>
					dfs2_nrm TEXTEQU dfs2_value
				ELSEIFIDNI dfs2_attr,<Signature>
					dfs2_IRegSig = -1
				ELSE
					.err <[#MF5]unknown attribute: arg1>
					EXITM
				ENDIF
			ELSE
				.err <[#MF6]invalid macro usage: expect: ...,attr=value,...>
				EXITM
			ENDIF
		ELSE
			.err <[#MF7]invalid macro usage: expect: ...,attr=value,...>
			EXITM
		ENDIF		
	ENDM
	
	dfs2_nr_r4  = _IF$(dfs2_nRegs NE -1 AND dfs2_nr_r4  EQ -1,dfs2_nRegs,_IF$(dfs2_nr_r4 NE -1,dfs2_nr_r4 ,0))
	dfs2_nr_r8  = _IF$(dfs2_nRegs NE -1 AND dfs2_nr_r8  EQ -1,dfs2_nRegs,_IF$(dfs2_nr_r8 NE -1,dfs2_nr_r8 ,0))
	dfs2_nr_r10 = _IF$(dfs2_nRegs NE -1 AND dfs2_nr_r10 EQ -1,dfs2_nRegs,_IF$(dfs2_nr_r10 NE -1,dfs2_nr_r10,0))
	dfs2_nXR_r4 = _IF$(dfs2_nRegs NE -1 AND dfs2_nXR_r4 EQ -1,dfs2_nRegs,_IF$(dfs2_nXR_r4 NE -1,dfs2_nXR_r4,0))
	dfs2_nXR_r8 = _IF$(dfs2_nRegs NE -1 AND dfs2_nXR_r8 EQ -1,dfs2_nRegs,_IF$(dfs2_nXR_r8 NE -1,dfs2_nXR_r8,0))
	dfs2_nRegs = _IF$(dfs2_nRegs NE -1,dfs2_nRegs,0)
full_fnc_dscptr <&FncName>,%dfs2_v,%dfs2_nArgs,%dfs2_nRegs,%dfs2_IStck,%dfs2_IReg,%dfs2_nr_r4,%dfs2_nr_r8,%dfs2_nr_r10,%dfs2_x32,%dfs2_x64,%dfs2_is,%dfs2_nXR_r4,%dfs2_nXR_r8,<%dfs2_rt>,<%dfs2_it>,<%dfs2_nrm>,%dfs2_IRegSig
endm

full_fnc_dscptr macro FncName,version,nArgs,nRegs,IStck,IReg,nRegs_r4,nRegs_r8,nRegs_r10,x32,x64,InstrSet,nXmmRegs_r4,nXmmRegs_r8,real_types,int_types,nRegMacro,Sig
	fslv_fnc_&FncName&_dscptr macro flags:req,param1:=<0>
		IF flags AND FNCD_VERSION
			EXITM <&version>
		ELSEIF flags AND FNCD_NARGS
			EXITM <&nArgs>
		ELSEIF flags AND FNCD_ISTCK
			EXITM <&IStck>
		ELSEIF flags AND FNCD_IREG
			EXITM <&IReg>
		ELSEIF flags AND FNCD_NREGS
			IFNB <&nRegMacro>
				EXITM nRegMacro(param1)
			ENDIF
			IF param1 AND FSIS_FPU
				IF fslv_lcl_real_type EQ 4
					EXITM <&nRegs_r4>
				ELSEIF fslv_lcl_real_type EQ 8
					EXITM <&nRegs_r8>
				ELSEIF fslv_lcl_real_type EQ 10
					EXITM <&nRegs_r10>
				ENDIF
			ELSEIF param1 AND FSIS_SSE2
				IF fslv_lcl_real_type EQ 4
					EXITM <&nXmmRegs_r4>
				ELSEIF fslv_lcl_real_type EQ 8
					EXITM <&nXmmRegs_r8>
				ENDIF
			ELSE
				EXITM <&nRegs>
			ENDIF
		ELSEIF flags AND FNCD_TST_LCL_TYPES
			IFNB <&real_types>
				fsfd_&FncName&_real = 0
				FOR arg,<&real_types>
					IF arg EQ fslv_lcl_real_type
						fsfd_&FncName&_real = 1
						EXITM
					ENDIF
				ENDM
			ELSE
				fsfd_&FncName&_real = -1
			ENDIF
			IFNB <&int_types>
				fsfd_&FncName&_int = 0
				FOR arg,<&int_types>
					IF arg EQ fslv_lcl_real_type
						fsfd_&FncName&_int = 1
						EXITM
					ENDIF
				ENDM
			ELSE
				fsfd_&FncName&_int = -1
			ENDIF
			EXITM %((fsfd_&FncName&_real NE 0) AND (fsfd_&FncName&_int NE 0))
		ELSEIF flags AND FNCD_X32
			EXITM <&x32>
		ELSEIF flags AND FNCD_X64
			EXITM <&x64>
		ELSEIF flags AND FNCD_INSTR_SET
			EXITM <&InstrSet>
		ELSEIF flags AND FNCD_IREG_RECORD_SIG
			EXITM <&Sig>
		ELSE
			EXITM <0>
		ENDIF
	endm
endm

fslv_fnc_ireg_expd macro arg0,free1,free2,free3,free_regs:VARARG
LOCAL lbl1,lbl2,end_lbl,@unordered
	
	comisd arg0,sse2_ExpD.xMax
	jbe lbl1
	jp @unordered
		movsd arg0,sse2_ExpD.plusINF
		jmp end_lbl
lbl1:
	comisd arg0,sse2_ExpD.xMin
	jae lbl2
		xorpd arg0,arg0
		jmp end_lbl
@unordered:
	movsd arg0,sse2_ExpD.NAN
	jmp end_lbl
lbl2:
	IFE fslv_volatile_gprs AND FSVGPR_EAX
		T_EXPR(<push eax>,<mov [rsp+8],rax>)
	ENDIF
	movsd free1,arg0
	mulsd free1,sse2_ExpD.r_ln2_0	; free1 = n
	cvtsd2si T_REG(eax),free1
	cvtsi2sd free1,eax
	IF @WordSize EQ 4
		add eax,1023
		shl eax,52-32
		and eax,7fffffffh
		push eax
		push 0
	ELSE
		add rax,1023
		and rax,7ffffh
		shl rax,52
		mov QWORD ptr [rsp],rax
	ENDIF
	movsd free2,free1
	mulsd free2,sse2_ExpD.mln2_0
	addsd free2,arg0	; free2 = y
	movsd free3,free2	
	mulsd free3,sse2_ExpD.f1 		; free3 = i
	cvtsd2si eax,free3
	cvtsi2sd free3,eax
	lea T_REG(eax),[T_REG(eax)*8+8*177]
	lea eax,[T_REG(eax)*2]
	mulsd free3,sse2_ExpD.f2
	movsd arg0,sse2_ExpD.tbl[T_REG(eax)+0]
	addsd free3,sse2_ExpD.tbl[T_REG(eax)+8]
	addsd free2,free3		; free2 = z
	movsd free3,free2
	mulsd free2,sse2_ExpD.P[0*8]
	addsd free2,sse2_ExpD.P[1*8]
	mulsd free2,free3
	addsd free2,sse2_ExpD.P[2*8]
	mulsd free2,free3
	addsd free2,sse2_ExpD.P[3*8]
	mulsd free2,free3	; free2 = exp(z)
	mulsd free1,sse2_ExpD.mln2_1	; free1 = -dely
	movsd free3,free2
	mulsd free2,free1
	addsd free2,free1
	addsd free2,free3
	mulsd free2,arg0
	addsd free2,arg0
	mulsd free2,REAL8 ptr [T_REG(esp)]
	movsd arg0,free2
	T_EXPR(<add esp,8>)
	IFE fslv_volatile_gprs AND FSVGPR_EAX
		T_EXPR(<pop eax>,<mov rax,[rsp+8]>)
	ENDIF
end_lbl:
endm
default_fnc_dscptr2 <expd>,nArgs=1,nRegs=3,x64=-1,SSE2=-1,<realTypes=<8>>,IReg=-1,Signature=-1

; |relative error| < 2.5E-15
fslv_fnc_ireg_log2d macro arg0,free1,free2,free3,free_regs:VARARG
LOCAL @valid,@1,@P,@end

	IFE fslv_volatile_gprs AND FSVGPR_EAX
		push eax
	ENDIF
	
	xorpd free3,free3
	comisd arg0,free3
	ja @valid
	  push 7f800000h
	  cvtss2sd arg0,REAL4 ptr [esp]
	  pop eax
	  jmp @end
	align 16
@valid:
	comisd arg0,sse2_log2sd_cmp1
	jb @1
	db 3eh ; DS prefix
	comisd arg0,sse2_log2sd_cmp2
	ja @1
	db 3eh ; DS prefix
	addsd arg0,sse2_log2sd_m1
	jmp @P
	
@1:	
	pextrw eax,arg0,3
	shr eax,4
	sub eax,1023
	cvtsi2sd free3,eax
	andpd arg0,XMMWORD ptr sse2_log2sd_msk_1
	orpd arg0,XMMWORD ptr sse2_log2sd_msk_2
	
	mulsd arg0,sse2_log2sd_128
	cvttsd2si eax,arg0
	cvtsi2sd free1,eax
	lea eax,[eax*8-128*8]
	db 3eh ; DS prefix
	lea eax,[eax+eax]
    subsd arg0,free1
    addsd free3,sse2_log2sd_tbl[eax]
    mulsd arg0,sse2_log2sd_tbl[eax][8]
@P:
    mov eax,OFFSET sse2_log2sd_P
	movsd free2,arg0
	unpcklpd arg0,arg0
	movapd free1,arg0
	mulsd free2,free2
	mulpd arg0,OWORD ptr [eax][0*16]
	mulpd free1,OWORD ptr [eax][1*16]
	addpd arg0,OWORD ptr [eax][2*16]
	addpd free1,OWORD ptr [eax][3*16]
	unpcklpd free2,free2
	mulpd arg0,free2
	mulsd free2,free2
	addpd arg0,free1
	movapd free1,arg0
	unpckhpd free1,free1
	mulsd arg0,free2
	addsd arg0,free1
	addsd arg0,free3
@end:
	IFE fslv_volatile_gprs AND FSVGPR_EAX
		pop eax
	ENDIF
endm
default_fnc_dscptr2 <log2d>,nArgs=1,nRegs=3,x64=0,SSE2=-1,<realTypes=<8>>,IReg=-1,Signature=-1

fslv_fnc_ireg_rsqrtss macro arg0,free_regs:VARARG
	rsqrtss arg0,arg0
endm
default_fnc_dscptr2 <rsqrtss>,nArgs=1,x64=-1,SSE2=-1,<realTypes=<4>>,IReg=-1

fslv_fnc_ireg_rcpss macro arg0,free_regs:VARARG
	rcpss arg0,arg0
endm
default_fnc_dscptr2 <rcpss>,nArgs=1,x64=-1,SSE2=-1,<realTypes=<4>>,IReg=-1

; root(x,n)
; n = 2,3,4,...
; e.g. root(-27,3) = -3
fslv_fnc_root macro
	call fpu_root
endm
default_fnc_dscptr2 <root>,nArgs=2,fpu=-1,x64=-1

; root2(x,a,b) = x^(a/b)
; a = +-(1,2,3,...)
; b = +-(1,2,3,...)
; e.g. root2(-27,2,3) = (-27)^(2/3) = 0,66666...
fslv_fnc_root2 macro
	call fpu_root2
endm
default_fnc_dscptr2 <root2>,nArgs=3,fpu=-1,x64=-1

fslv_fnc_sqrt macro
	fsqrt
endm
fslv_fnc_ireg_sqrt macro arg0,free_regs:VARARG
	IF fslv_lcl_real_type EQ 4
		sqrtss arg0,arg0
	ELSE
		sqrtsd arg0,arg0
	ENDIF
endm
default_fnc_dscptr2 <sqrt>,nArgs=1,fpu=-1,x64=-1,SSE2=-1

; exp(...) = e^(...)
fslv_fnc_exp macro
	call fpu_exp
endm
default_fnc_dscptr2 <exp>,nArgs=1,nRegs=1,fpu=-1,x64=-1

fslv_fnc_sin macro
	fsin
endm
default_fnc_dscptr2 <sin>,nArgs=1,fpu=-1,x64=-1

fslv_fnc_cos macro
	fcos
endm
default_fnc_dscptr2 <cos>,nArgs=1,fpu=-1,x64=-1

fslv_fnc_tan macro	
	fptan
	fstp st
endm
default_fnc_dscptr2 <tan>,nArgs=1,nRegs=1,fpu=-1,x64=-1


fslv_fnc_abs macro
	fabs
endm
fslv_fnc_ireg_abs macro arg0,free_regs:VARARG
	IF fslv_lcl_real_type EQ 4
		andps arg0,sse2_ss_abs_msk
	ELSE
		andpd arg0,sse2_sd_abs_msk
	ENDIF
endm
default_fnc_dscptr2 <abs>,nArgs=1,fpu=-1,x64=-1,SSE2=-1,IReg=-1


fslv_fnc_rad macro
	IF fslv_lcl_real_type LE 8
		fmul @CatStr(<fpu_const_r>,%fslv_lcl_real_type,<_d2r>)
	ELSE
		fld @CatStr(<fpu_const_r>,%fslv_lcl_real_type,<_d2r>)
		fmulp
	ENDIF
endm
fslv_fnc_ireg_rad macro arg0,free_regs:VARARG
	IF fslv_lcl_real_type EQ 4
		mulss arg0,fpu_const_r4_d2r
	ELSE
		mulsd arg0,fpu_const_r8_d2r
	ENDIF
endm
default_fnc_dscptr2 <rad>,nArgs=1,fpu=-1,x64=-1,SSE2=-1,nRegs_r10=1,IReg=-1

	
fslv_fnc_deg macro
	IF fslv_lcl_real_type LE 8
		fmul @CatStr(<fpu_const_r>,%fslv_lcl_real_type,<_r2d>)
	ELSE
		fld @CatStr(<fpu_const_r>,%fslv_lcl_real_type,<_r2d>)
		fmulp
	ENDIF	
endm
fslv_fnc_ireg_deg macro arg0,free_regs:VARARG
	IF fslv_lcl_real_type EQ 4
		mulss arg0,fpu_const_r4_r2d
	ELSE
		mulsd arg0,fpu_const_r8_r2d
	ENDIF
endm
default_fnc_dscptr2 <deg>,nArgs=1,fpu=-1,x64=-1,SSE2=-1,nRegs_r10=1,IReg=-1


fslv_fnc_log macro
	; log(x) = log(2)*log2(x)
	fldlg2
	fxch
	fyl2x
endm
default_fnc_dscptr2 <log>,nArgs=1,nRegs=1,fpu=-1,x64=-1

fslv_fnc_ln macro
	; ln(x) = ln(2)*log2(x)
	fldln2
	fxch
	fyl2x
endm
default_fnc_dscptr2 <ln>,nArgs=1,nRegs=1,fpu=-1,x64=-1		

; logbx(base,num)
fslv_fnc_logbx macro
	fld1
	fxch
	fyl2x
	fxch	 ;st(1)=log2(x), st(0) = b
	fld1
	fxch
	fyl2x    ;st(1)=log2(x), st(0) = log2(b)
	fdivp st(1),st(0)
endm
default_fnc_dscptr2 <logbx>,nArgs=2,nRegs=1,fpu=-1,x64=-1

; limit(val,maximum,minimum)
; if (val>maximum) {return maximum}
; else if (val < minimum){return minimum} 
; else {return val}
fslv_fnc_limit macro
LOCAL below,above,fin
	fcomi st,st(2)
	ja below
	fstp st
	fcomi st,st(1)
	jb above
	fstp st
	jmp fin
below:
	fstp st(1)
	fstp st(1)
	jmp fin	
above:
	fstp st(1)
fin:
endm
fslv_fnc_ireg_limit macro x,max,min,free_regs:VARARG
	IF fslv_lcl_real_type EQ 4
		maxss x,min
		minss x,max
	ELSE
		maxsd x,min
		minsd x,max
	ENDIF
endm
default_fnc_dscptr2 <limit>,nArgs=3,fpu=-1,x64=-1,SSE2=-1,IReg=-1


fslv_fnc_min macro
	fcomi st,st(1)
	.if !CARRY?
		fstp st
	.else
		fstp st(1)
	.endif
endm
fslv_fnc_ireg_min macro arg0,arg1,free_regs:VARARG
	IF fslv_lcl_real_type EQ 4
		minss arg0,arg1
	ELSE
		minsd arg0,arg1
	ENDIF
endm
default_fnc_dscptr2 <min>,nArgs=2,fpu=-1,x64=-1,SSE2=-1,IReg=-1


fslv_fnc_max macro
	fcomi st,st(1)
	.if !CARRY?
		fstp st(1)
	.else
		fstp st
	.endif
endm
fslv_fnc_ireg_max macro arg0,arg1,free_regs:VARARG
	IF fslv_lcl_real_type EQ 4
		maxss arg0,arg1
	ELSE
		maxsd arg0,arg1
	ENDIF
endm
default_fnc_dscptr2 <max>,nArgs=2,fpu=-1,x64=-1,SSE2=-1,IReg=-1

; fit2pi(x)
; transform multiple of 2*PI to u*2*PI, 
; whereas 1 >= u >= 0 for positive values and
; 0 >= u >= -1 for negative values.
fslv_fnc_fit2pi macro
	call fpu_fit2pi
endm
default_fnc_dscptr2 <fit2pi>,nArgs=1,nRegs=2,fpu=-1,x64=-1

; sign function
fslv_fnc_sgn macro
	fstp REAL4 ptr T_EXPR([esp-4],[rsp])
	fld1
	.if (DWORD ptr T_EXPR([esp-4],[rsp]))&80000000h
		fchs
	.endif
endm
fslv_fnc_ireg_sgn macro arg0,free_regs:VARARG
	IF fslv_lcl_real_type EQ 4
		andps arg0,sse2_ss_neg_msk
		orps arg0,OWORD ptr sse2_ss_one
	ELSE
		andpd arg0,sse2_sd_neg_msk
		orpd arg0,OWORD ptr sse2_sd_one
	ENDIF
endm
default_fnc_dscptr2 <sgn>,nArgs=1,fpu=-1,x64=-1,SSE2=-1,IReg=-1

fslv_fnc_atan macro
	fld1
	fpatan
endm
default_fnc_dscptr2 <atan>,nArgs=1,nRegs=1,fpu=-1,x64=-1

fslv_fnc_atan2 macro
	fpatan
endm
default_fnc_dscptr2 <atan2>,nArgs=2,fpu=-1,x64=-1

fslv_fnc_asin macro
  	fld st
  	fmul st,st
  	fsubr fpu_const_r4_one
  	fsqrt
  	fpatan
endm
default_fnc_dscptr2 <asin>,nArgs=1,nRegs=1,fpu=-1,x64=-1

fslv_fnc_acos macro
  	fld st
  	fmul st,st
  	fsubr fpu_const_r4_one
  	fsqrt
  	fxch
  	fpatan
endm
default_fnc_dscptr2 <acos>,nArgs=1,nRegs=1,fpu=-1,x64=-1

fslv_fnc_trunc macro
	IFE fslv_volatile_gprs AND FSVGPR_EAX
		T_EXPR(<push eax>,<mov [rsp+8],rax>)
	ENDIF
	fstcw WORD ptr T_EXPR([esp-2],[rsp])
	movzx eax,WORD ptr T_EXPR([esp-2],[rsp])
	or eax,0c00h
	mov T_EXPR([esp-4],[rsp+2]),ax
	fldcw T_EXPR([esp-4],[rsp+2])
	frndint	
	fldcw WORD ptr T_EXPR([esp-2],[rsp])
	IFE fslv_volatile_gprs AND FSVGPR_EAX
		T_EXPR(<pop eax>,<mov rax,[rsp+8]>)
	ENDIF
endm
default_fnc_dscptr2 <trunc>,nArgs=1,fpu=-1,x64=-1

;mod(x,y) = x mod y
fslv_fnc_mod macro
	fdiv st(1),st
	fxch
	fld st
	IFE fslv_volatile_gprs AND FSVGPR_EAX
		T_EXPR(<push eax>,<mov [rsp+8],rax>)
	ENDIF
	fstcw WORD ptr T_EXPR([esp-2],[rsp])
	movzx eax,WORD ptr T_EXPR([esp-2],[rsp])
	or eax,0c00h
	mov T_EXPR([esp-4],[rsp+4]),ax
	fldcw T_EXPR([esp-4],[rsp+4])
	frndint	
	fldcw WORD ptr T_EXPR([esp-2],[rsp])
	IFE fslv_volatile_gprs AND FSVGPR_EAX
		T_EXPR(<pop eax>,<mov rax,[rsp+8]>)
	ENDIF
	fsubp st(1),st
	fmulp st(1),st
endm
default_fnc_dscptr2 <mod>,nArgs=2,nRegs=1,fpu=-1,x64=-1

fslv_fnc_ceil macro
	IFE fslv_volatile_gprs AND FSVGPR_EAX
		T_EXPR(<push eax>,<mov [rsp+8],rax>)
	ENDIF
	fstcw WORD ptr T_EXPR([esp-2],[rsp])
	movzx eax,WORD ptr T_EXPR([esp-2],[rsp])
	or eax,0800h
	mov T_EXPR([esp-4],[rsp+2]),ax
	fldcw T_EXPR([esp-4],[rsp+2])
	frndint	
	fldcw WORD ptr T_EXPR([esp-2],[rsp])
	IFE fslv_volatile_gprs AND FSVGPR_EAX
		T_EXPR(<pop eax>,<mov rax,[rsp+8]>)
	ENDIF
endm
default_fnc_dscptr2 <ceil>,nArgs=1,fpu=-1,x64=-1
