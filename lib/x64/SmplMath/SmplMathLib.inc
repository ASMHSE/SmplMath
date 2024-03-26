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

option casemap :none
option frame:auto
option procalign:16

JWASM_STORE_REGISTER_ARGUMENTS	EQU 1
JWASM_STACK_SPACE_RESERVATION	EQU 2

option win64:JWASM_STACK_SPACE_RESERVATION

SSE2_EXPD struct
	xMax	REAL8	?
	xMin	REAL8	?
	r_ln2_0	REAL8	?
	mln2_0	REAL8	?
	f1		REAL8	?
	f2		REAL8	?
	plusINF	REAL8	?
	NAN		REAL8	?
	P		REAL8	4 dup (?)
	mln2_1	REAL8	?
	tbl		REAL8	0 dup (?)
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

fpu_st1_pow_st0 proto 
fpu_exp proto 
fpu_root proto 
fpu_root2 proto 
fpu_fit2pi proto
fpu_is_integer proto 
fpu_zero_test proto exp_diff:DWORD,bPop:DWORD