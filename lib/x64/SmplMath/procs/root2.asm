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
	
	option PROLOGUE:none
	option EPILOGUE:none
	; a,b > 0
	_gcd proc private a:SDWORD,b:SDWORD
		
		.if !edx
			mov eax,ecx
		.else
			mov eax,ecx
			mov ecx,edx
			xor edx,edx
			div ecx
			call _gcd
		.endif
		ret
		
	_gcd endp
	
	option PROLOGUE:PROLOGUEDEF
	option EPILOGUE:EPILOGUEDEF
	
	_cancel_fract proc private FRAME uses rax pA:ptr SDWORD,pB: ptr SDWORD
		LOCAL a:QWORD
		LOCAL b:QWORD
		
		mov pA,rcx
		mov pB,rdx
		mov ecx,SDWORD ptr [rcx]
		mov edx,SDWORD ptr [rdx]
		mov a,rcx
		mov b,rdx
		.if SDWORD ptr ecx < 0
			neg ecx
		.endif
		.if SDWORD ptr edx < 0
			neg edx
		.endif
		call _gcd
		.if eax && eax != 1
			mov ecx,eax
			mov rax,b
			cdq
			idiv ecx
			mov rdx,pB
			mov SDWORD ptr [rdx],eax
			mov rax,a
			cdq
			idiv ecx
			mov rdx,pA
			mov SDWORD ptr [rdx],eax
		.endif
		ret
		
	_cancel_fract endp

	;/* root2(x,a,b) = x^(a/b)
	; *
	; * pseudo code:
    ; *   function root2(x:REAL, a:INTGER, b:INTEGER) is
    ; *     if (b == 0)
    ; *       throw error: division by zero 
    ; *       return INFINITE
    ; *     if(a == 0)
    ; *       return 1
    ; *     if(x < 0)
    ; *       cancel fraction a/b
    ; *       if((b mod 2) == 0)
    ; *         throw error
    ; *         return NaN
    ; *     res = pow(x,a/b)
    ; *     if(x < 0 && ((a mod 2) != 0 && (b mod 2) != 0))
    ; *       res = -res 
	; *     return res
    ; *   end function
    ; */
	fpu_root2 proc FRAME uses rdx
		LOCAL iTmpA:SDWORD
		LOCAL iTmpB:SDWORD
		LOCAL iTmp:SDWORD
		LOCAL r4:REAL4
		LOCAL _rax:QWORD

		fistp iTmpB
		fistp iTmpA
		.if !iTmpB
			fstp st
			fld1
			fldz
			fdivp st(1),st
			ret
		.endif
		.if !iTmpA
			;/* x^0 = 1 */
			fstp st
			fld1
			ret
		.endif
		fst r4
		fabs
		.if r4&80000000h
			mov _rax,rax
			invoke _cancel_fract,ADDR iTmpA,ADDR iTmpB
			mov rax,_rax
			.if !(iTmpB & 1)
				fstp st
				;/* generate exception */
				fld1
				fld1
				fchs
				fyl2x
				jmp @end	
			.endif
		.endif
		fild iTmpA
		fidiv iTmpB
		call fpu_st1_pow_st0	; st(0) = st(1)^(1/rndint(st(0)))
		.if r4&80000000h && (iTmpA&1 && iTmpB&1)
			fchs
		.endif
	@end:
		ret

	fpu_root2 endp	
	

SmplMath ENDS

end


