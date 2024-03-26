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
	sse2_sd_pi		REAL8 3.1415926535897932384626433832795
	sse2_sd_mpi		REAL8 -3.1415926535897932384626433832795
	sse2_ss_pi		REAL4 3.1415926535897932384626433832795
	sse2_ss_mpi		REAL4 -3.1415926535897932384626433832795
	
SmplMthC ENDS
end