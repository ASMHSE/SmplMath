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

	align 4
	fpu_const_r4_2pi	LABEL REAL4
						db 0DBh,00Fh,0C9h,040h										; = 2*pi
	fpu_const_r4_r2d	LABEL REAL4
						db 0E1h,02Eh,065h,042h										; 180/pi	
	fpu_const_r4_d2r	LABEL REAL4
						db 035h,0FAh,08Eh,03Ch										; pi/180

SmplMthC ENDS
end





