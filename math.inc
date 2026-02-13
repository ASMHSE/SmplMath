;/********************************************************
; * This file is part of the SmplMath macros-system.     *
; *                                                      *
; *          Copyright by qWord, 2011-2014               *
; *                                                      *
; *            Modified by HSE, 2020-2026                *
; *                                                      *
; *       https://github.com/ASMHSE/SmplMath             *
; *                                                      *
; *  Further Copyright and Legal statements are located  *
; *  in the documentation (Documentation.pdf).           *
; *                                                      *
; ********************************************************/


IFDEF __SMPLMATH__
    .err <SmplMath: double inclusion of math.inc>
    fslv_glb_error = 1
ELSE
    __SMPLMATH__ TEXTEQU <020003>
ENDIF

IF @WordSize EQ 4
    includelib \masm32\macros\SmplMath\lib\x86\SmplMath.lib
ELSEIF @WordSize EQ 8
    IFNDEF __JWASM__
        ;echo * * *   Not full tested with ML64   * * * 
        ;.err <The SmplMath System requires JWASM for x86-64 support>
        ;fslv_glb_error = 1
    ENDIF
    includelib \masm32\macros\SmplMath\lib\x64\SmplMath.lib
ELSE
    .err <The SimplMath System only supports the x86-32 and x86-64 architecture>
    fslv_glb_error = 1
ENDIF

include \masm32\macros\SmplMath\SmplMath.inc
include \masm32\macros\SmplMath\misc.inc
include \masm32\macros\SmplMath\linked_list.inc
include \masm32\macros\SmplMath\math_tokenizer.inc
include \masm32\macros\SmplMath\backends.inc
include \masm32\macros\SmplMath\code_generator.inc
include \masm32\macros\SmplMath\math_functions.inc

include \masm32\macros\SmplMath\backends\x86_32_64_fpu.inc
include \masm32\macros\SmplMath\backends\x86_32_64_sse2_fpu.inc
include \masm32\macros\SmplMath\backends\x86_32_64.inc

include \masm32\macros\SmplMath\compounds.inc
include \masm32\macros\SmplMath\SmplReg.inc
include \masm32\macros\SmplMath\ForLp.inc
include \masm32\macros\SmplMath\FlowControl.inc
include \masm32\macros\SmplMath\translation3264.inc

LOAD_DEFAULT_BACKEND
