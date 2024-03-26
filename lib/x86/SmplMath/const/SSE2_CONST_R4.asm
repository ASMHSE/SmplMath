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

include SmplMathLib.inc

SmplMthC SEGMENT READONLY
	
	align 16
	sse2_ss_neg_msk		LABEL OWORD
						DWORD 80000000h
						DWORD 3 dup (0)

	align 16
	sse2_ss_one			REAl4 1.0
						REAL4 3 dup (0.0)
	
	align 16
	sse2_ss_abs_msk		LABEL OWORD
						DWORD 7FFFFFFFh
						DWORD 3 dup (0)
SmplMthC ENDS
end





