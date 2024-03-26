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
	
	align 16
	; preliminary version
	fpu_fit2pi proc stdcall uses eax
		LOCAL r4Tmp:REAL4

		xor eax,eax
		fst r4Tmp
		fld st
		fabs
		fld fpu_const_r10_2pi
		fcomi st,st(1)
		fstp st(1)
	@@: jae @F
		fdiv st(1),st
		fld st(1)
		test r4Tmp,080000000h
		setnz al
		.if !ZERO?
			fchs
			fxch st(2)
			fchs
			fxch st(2)
		.endif
		frndint
		fsubp st(2),st
		fxch 			; st    = 2pi/x - rndint(2pi/x)
						; st(1) = 2pi
		fst r4Tmp
		test r4Tmp,080000000h
		.if !ZERO?
			fld1
			faddp st(1),st		
		.endif
		fmulp st(1),st
		.if eax&1
			fchs
		.endif
			
		ret
	@@: fstp st
	@1:
		ret

	fpu_fit2pi endp		

SmplMath ENDS

end


