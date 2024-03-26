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
	fpu_const_r10_2pi	LABEL REAL10
						db 035h,0C2h,068h,021h,0A2h,0DAh,00Fh,0C9h,001h,040h		; = 2*pi
	align 16
	fpu_const_r10_r2d	LABEL REAL10
						db 0C3h,0BDh,00Fh,01Eh,0D3h,0E0h,02Eh,0E5h,004h,040h		; 180/pi
	align 16
	fpu_const_r10_d2r	LABEL REAL10
						db 0AEh,0C8h,0E9h,094h,012h,035h,0FAh,08Eh,0F9h,03Fh		; pi/180
SmplMthC ENDS
end






