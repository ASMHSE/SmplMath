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
	
	;/* root(x,n) = x^(1/n)
	;/*
    ; * pseudo code:
    ; *    function root(x:REAL, n:INTEGER) is
    ; *      if (n == 0)                      
    ; *        throw error: division by zero  
    ; *        return INFINITE                
    ; *      if (x < 0 && (n mod 2) == 0)     
    ; *        return NaN                     
    ; *      res = pow(x,1/n)                 
    ; *      if ((n mod 2) != 0 && x<0)       
    ; *        res = -res                     
    ; *      return res                       
    ; *    end function 
    ; *
    ; */                    
	fpu_root proc FRAME
		LOCAL iTmp:SDWORD
		LOCAL r4:REAL4
		
		fistp iTmp
		fst r4
		fabs
		
		.if !iTmp
			fstp st
			fld1
			fldz
			fdivp st(1),st
			ret
		.endif
		
		.if r4&80000000h && !(iTmp & 1)
			fstp st
			;/* generate exception */
			fld1
			fld1
			fchs
			fyl2x
			ret	
		.endif
		fld1
		fidiv iTmp
		call fpu_st1_pow_st0	; st(0) = st(1)^(1/rndint(st(0)))
		.if iTmp & 1 && r4&80000000h
			fchs
		.endif
		ret

	fpu_root endp

SmplMath ENDS

end


