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
	
	;------------------------------------------------------------------
	; Check if st(0) is negligible in compare to st(1). This is done 
	; by comparing the exponent values. If the value is negligible and bPop!=0
	; then the value is removed from the stack.
	; return: ( exponent(st(1)) - exponent(st(0)) <= exp_diff )?1:0
	; e.g.: exp_diff = 20 = log(2^20) ~ 6 decimal digits
	;_-----------------------------------------------------------------
	fpu_zero_test proc FRAME uses r8 exp_diff:DWORD,bPop:DWORD
		LOCAL val1:REAL10 ; st(0)
		LOCAL val2:REAL10 ; st(1)
		
		fstp val1
		fstp val2
		fld val2
		
		movzx eax,WORD ptr val1[8]
		movzx r8d,WORD ptr val2[8]
		and eax,7fffh
		and r8d,7fffh
		sub r8d,eax
		xor rax,rax
		cmp r8d,ecx	
		setg al
		.if !edx || !eax
			fld val1
		.endif
		ret	
		
	fpu_zero_test endp

SmplMath ENDS
end


