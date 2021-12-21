;/********************************************************
; * This file is part of the SmplMath macros-system.     *
; *                                                      *
; *          Copyright by qWord, 2011-2014               *
; *                                                      *
; *            Modified by HSE, 2020-2021                *
; *                                                      *
; *          SmplMath.Masm{at}gmx{dot}net                *
; *    http://sourceforge.net/projects/smplmath/         *
; *                                                      *
; *  Further Copyright and Legal statements are located  *
; *  in the documentation (Documentation.pdf).           *
; *                                                      *
; ********************************************************/


IFDEF __SMPLMATH__
	.err <SmplMath: double inclusion of math.inc>
	fslv_glb_error = 1
ELSE
	__SMPLMATH__ TEXTEQU <020002>
ENDIF

IF @WordSize EQ 4
	includelib \macros\SmplMath\lib\x86\SmplMath.lib
ELSEIF @WordSize EQ 8
	IFNDEF __JWASM__
              echo * * *   Not full tested with ML64   * * * 
		;.err <The SmplMath System requires JWASM for x86-64 support>
		;fslv_glb_error = 1
	ENDIF
	includelib \macros\SmplMath\lib\x64\SmplMath.lib
ELSE
	.err <The SimplMath System only supports the x86-32 and x86-64 architecture>
	fslv_glb_error = 1
ENDIF

include \macros\SmplMath\SmplMath.inc
include \macros\SmplMath\misc.inc
include \macros\SmplMath\linked_list.inc
include \macros\SmplMath\math_tokenizer.inc
include \macros\SmplMath\backends.inc
include \macros\SmplMath\code_generator.inc
include \macros\SmplMath\math_functions.inc

include \macros\SmplMath\backends\x86_32_64_fpu.inc
include \macros\SmplMath\backends\x86_32_64_sse2_fpu.inc
include \macros\SmplMath\backends\x86_32_64.inc

include \macros\SmplMath\compounds.inc
include \macros\SmplMath\SmplReg.inc
include \macros\SmplMath\ForLp.inc
include \macros\SmplMath\IfElseM.inc

LOAD_DEFAULT_BACKEND
