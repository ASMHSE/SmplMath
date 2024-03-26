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

SmplMath SEGMENT PUBLIC 'CODE'
	
	fpu_exp proc FRAME
		fldl2e                  ; st(0) = e^st(0) = 2^(st(0)*ld(e))
		fmulp st(1),st          ;
		fld st(0)				;
		frndint					;
		fsub st(1),st(0)		;
		fxch					;
		f2xm1					;
		fadd fpu_const_r4_one	;
		fscale					;
		fstp st(1)
		ret
	fpu_exp endp

SmplMath ENDS

end


