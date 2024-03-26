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

	;---------------------------------------------
	;             st(0) = st(1)^st(0)
	;  - st(1) is poped
	;  - no additional fpu register are used
	;  - for negativ bases, the function checks if the exponent
	;    is an integer value. In this case the result's signe is
	;    changed, if the exponent is odd.
	;    The result for negativ bases in combination with a
	;    non-integer exponents is allways positiv. e.g. (-27)^(1/3) = 3 
	;    Positiv bases can have any exponent.
	;  - the following pseudo code shows the operation:
    ;       function pow(x:REAL, y:REAL) is
    ;         if (x == 0)
    ;           if(y == 0)
    ;             return 1
    ;           if(y < 0)
    ;             throw error: division by zero
    ;             return INFINITE
    ;           return 0
    ;         if (y == 0)
    ;           return 1
    ;         res = 2^(y*ld(abs(x)))
    ;         if (x < 0 && is_integer(y) && (y mod 2) != 0)
    ;           res = -res
    ;         return res
    ;       end function
    ;
	;  - algorithm inspired by
	;    Raymond Filiatreault's fpulib
	;---------------------------------------------
	fpu_st1_pow_st0 proc FRAME
		LOCAL tmp_e:REAL10
		LOCAL tmp_b:REAL10
		LOCAL iTmp:SDWORD
		LOCAL _rax:QWORD

		fstp tmp_e
		fstp tmp_b
	 	
	 	.if !(DWORD ptr tmp_b[0]) && !(DWORD ptr tmp_b[4]) && !(WORD ptr tmp_b[8]&7fffh)
			.if !(DWORD ptr tmp_e[0]) && !(DWORD ptr tmp_e[4]) && !(WORD ptr tmp_e[8]&7fffh)
				;/* definition: 0^0 == 1 */
				fld1
			.elseif WORD ptr tmp_e[8]&8000h
				;/* 0^(-x), generate exception */
				fld1
				fldz
				fdivp st(1),st
			.else
				;/* 0^x*/
				fldz
			.endif
	 		ret
	 	.elseif !(DWORD ptr tmp_e[0]) && !(DWORD ptr tmp_e[4]) && !(WORD ptr tmp_e[8]&7fffh)
	 		;/* x^0 = 1 */
	 		fld1
	 		ret
	 	.endif	 	
	 	
	 	;/* st(0) = st(1)^st(0) = 2^(st(0)*ld(st(1))) */
		fld tmp_e
		fld tmp_b
		fabs
		
		fyl2x				
		fld st(0)			
		frndint				
		fsub st(1),st(0)	
		fxch				
		f2xm1				
		fadd fpu_const_r4_one
		fscale				
		fstp st(1)
		.if WORD ptr tmp_b[8]&8000h
			;/* (-x)^y */
			fld tmp_e
			fist iTmp
			mov _rax,rax
			invoke fpu_is_integer
			.if eax && iTmp&1
				fchs
			.endif
			mov rax,_rax
		.endif
		ret
		
	fpu_st1_pow_st0 endp

SmplMath ENDS

end


