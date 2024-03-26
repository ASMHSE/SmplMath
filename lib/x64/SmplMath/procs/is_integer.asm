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

	; check if st(0) is an integer and pop the stack
	; return: if st(0) is an integer, eax is nonzero
	fpu_is_integer proc FRAME uses rdx rcx
		LOCAL tmp:REAL10

		fstp tmp
		movzx eax,WORD ptr tmp[8]
		and eax,7FFFh
		mov ecx,eax
		xor eax,7FFFh
		.if !ZERO? ; number is not NaN,INFINTE,...
			mov eax,DWORD ptr tmp[0]
			or eax,DWORD ptr tmp[4]
			or eax,ecx
			setz al
			.if !ZERO?	; value != +-0.0
				xor eax,eax
				sub ecx,3fffh
				.if !CARRY?		; negativ exponent => not an integer
					test ecx,0ffffffc0h
					setnz al
					.if ZERO?	; (exponent >= 64) ? integer : continue check
						; clear out all integer bits and test for zero.
						.if ecx != 63
							lea ecx,[ecx+1]
							xor eax,eax
							mov rdx,QWORD ptr tmp[0]
							shl rdx,cl
							setz al
						.else
							or rax,1
						.endif
					.endif
				.endif
			.endif
		.endif
		ret

	fpu_is_integer endp

SmplMath ENDS

end


