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
	sse2_sd_neg_msk		LABEL OWORD
						QWORD 8000000000000000h
						QWORD 0
	align 16
	sse2_sd_one			REAL8 1.0
						REAL8 0.0
	align 16
	sse2_sd_abs_msk		LABEL OWORD
						QWORD 7FFFFFFFFFFFFFFFh
						QWORD 0
SmplMthC ENDS
end

