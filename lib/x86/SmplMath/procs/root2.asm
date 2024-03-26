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
	align 16
	_gcd proc stdcall private a:SDWORD,b:SDWORD
		
		.if !SDWORD ptr [esp+8]
			mov eax,SDWORD ptr [esp+4]
		.else
			mov eax,SDWORD ptr [esp+4]
			mov ecx,SDWORD ptr [esp+8]
			xor edx,edx
			div ecx
			push edx
			push ecx
			call _gcd
		.endif
		ret 8
		
	_gcd endp
	
	option PROLOGUE:PROLOGUEDEF
	option EPILOGUE:EPILOGUEDEF
	
	align 16
	
	_cancel_fract proc stdcall private uses eax edx ecx pA:ptr SDWORD,pB: ptr SDWORD
		
		mov eax,pA
		mov edx,pB
		mov eax,SDWORD ptr [eax]
		mov edx,SDWORD ptr [edx]
		push eax
		push edx
		.if SDWORD ptr eax < 0
			neg eax
		.endif
		.if SDWORD ptr edx < 0
			neg edx
		.endif
		invoke _gcd,eax,edx
		.if eax && eax != 1
			mov ecx,eax
			pop eax
			cdq
			idiv ecx
			mov edx,pB
			mov [edx],eax
			pop eax
			cdq
			idiv ecx
			mov edx,pA
			mov [edx],eax
		.else
			add esp,8
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
	align 16
	fpu_root2 proc stdcall
		LOCAL iTmpA:SDWORD
		LOCAL iTmpB:SDWORD
		LOCAL iTmp:SDWORD
		LOCAL r4:REAL4

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
			invoke _cancel_fract,ADDR iTmpA,ADDR iTmpB
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


