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

	align 8
	fpu_const_r8_pi_div_2	REAL8 1.57079632679489661923132

	fpu_const_r8_2pi	LABEL REAL8
						db 018h,02Dh,044h,054h,0FBh,021h,019h,040h					; = 2*pi
						
	fpu_const_r8_r2d	LABEL REAL8
						db 0F8h,0C1h,063h,01Ah,0DCh,0A5h,04Ch,040h					; 180/pi
						
	fpu_const_r8_d2r	LABEL REAL8
						db 039h,09Dh,052h,0A2h,046h,0DFh,091h,03Fh					; pi/180
SmplMthC ENDS
end





