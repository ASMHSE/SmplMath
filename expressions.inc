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

;/* The following expressions can be used to evaluate Polynomials using Horner's method:
; *            p(x) = c[0]x^N + c[1]*x^(N-1) + c[2]*x^(N-2) + c[3]*x^(N-3) + ... 
; *                 = (((x*c[0] + c[N-1])*x + c[2])*x + c[3])*x + ...    ; <-- Horner's method
; * Functions:
; *            r4/8Horner(N, x, c)
; *            THorner(N, x, c, element size)
; *            r4/8Horner2(N, x, c)               ; c[0] = 0
; *            THorner2(N, x, c, element size)    ; c[0] = 0
; * Arguments:
; *       N            : degree of the polynominal; must be an immediate intger value >= 1
; *       x            : variable
; *       c            : lable of coefficent array. The array is in descending order of degrees and  
; *                      thus begins with the coefficent of the highest degree x^N
; *                      The array must have N+1 elements for the r4/8Horner and THorner.
; *                      For r4/8Horner2 and THorner2 the last element is zero thus the array 
; *                      must only contains N elements. 
; *       element size : size of element in the coefficent array
; * Example:
; *      .const
; *          P REAL8 1.0, 2.0, 3.0, 4.0
; *      .code
; *      fSlv y = r8Horner(3,x,P)   ; y = ((1*x+2)*x+3)*x+4 == 1*x^3 + 2*x^2 + 3*x + 4
; **/
fSlvRegExpr r4Horner,3,_horner(#arg1-1,arg2,arg3,4)
fSlvRegExpr r8Horner,3,_horner(#arg1-1,arg2,arg3,8)
fSlvRegExpr THorner,4,_horner(#arg1-1,arg2,arg3,arg4)
fSlvRegExpr r4Horner2,3,_horner2(#arg1-1,arg2,arg3,4)
fSlvRegExpr r8Horner2,3,_horner2(#arg1-1,arg2,arg3,8)
fSlvRegExpr THorner2,4,_horner2(#arg1-1,arg2,arg3,arg4)

;/* Creates a floating point constant with the value 2^n
; * Function:
; *                cpow2(n)
; * Arguments:
; *             n = exponent value, that must be in range of the maximal
; *                 exponent that the current real type can have
; *                 REAL4 : -126   <= n <= 127
; *                 REAL8 : -1022  <= n <= 1023
; *                 REAL10: -16382 <= n <= 16383
; * Remarks:
; * The returned value is sized and typed according to the local precision settings.
; **
fSlvRegExpr cpow2,1,?$_bin_pow_const(,arg1,<cpow2>)

;/****************************************************/
;/*                                                  */
;/* internal expressions (used by above expressions) */
;/*                                                  */
;/****************************************************/

fSlvRegRecursiveExpr _horner,4,<arg2*arg3[arg1*arg4]+arg3[(arg1+1)*arg4]>,_horner(#arg1-1,arg2,arg3,arg4)*arg2+arg3[(arg1+1)*arg4]
fSlvRegRecursiveExpr _horner2,4,<arg2*arg3[arg1*arg4]+arg3[(arg1+1)*arg4]>,_horner(#arg1-1,arg2,arg3,arg4)*arg2

; WARNING ML v8-11: do not directly initialze variables with this macro: due a bug MASM crash for REAL8 and REAL10 values.
_bin_pow_const macro _type,n,mname
	IFB <_type>
		?bpc_type = fslv_lcl_real_type
	ELSE
		?bpc_type = _type
	ENDIF
	
	?bpc_bias = @GetArgByIndex(%?bpc_type/3-1,,127,1023,16383)
	IF n LT @GetArgByIndex(%?bpc_type/3-1,,-126,-1022,-16382) OR n GT ?bpc_bias
		fslv_glb_error_msg TEXTEQU <[#pe1] mname(): binary exponent out of range: n : change the precision settings>
		fslv_glb_error = 1
		EXITM <0.0>
	ENDIF
	
	?bpc_pow = n
	?bpc_pow = (?bpc_pow + ?bpc_bias) SHL @GetArgByIndex(%?bpc_type/3-1,,23,(52-32),0)
	.radix 16
		?bpc_exp TEXTEQU %?bpc_pow
	.radix 10
	IF ?bpc_type EQ 8
		?bpc_exp TEXTEQU ?bpc_exp,<00000000>
	ELSEIF ?bpc_type EQ 10
		?bpc_exp TEXTEQU ?bpc_exp,<8000000000000000>
	ENDIF
	?bpc_cntr = (?bpc_type*2+1) - @SizeStr(%?bpc_exp)
%	REPEAT ?bpc_cntr
		?bpc_exp TEXTEQU <0>,?bpc_exp
	ENDM
	?bpc_exp TEXTEQU ?bpc_exp,<r>
	EXITM ?bpc_exp
endm