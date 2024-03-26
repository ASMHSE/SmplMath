;/************************************************************
; *   This file is a test of the SmplMath macros-system.     *
; *                                                          *
; *            Copyright by HSE, 2020-2022                   *
; *                                                          *
; *          https://github.com/ASMHSE/SmplMath/             *
; *                                                          *
; *    Further Copyright and Legal statements are located    *
; *    in the documentation (Documentation.pdf).             *
; *                                                          *
; ************************************************************/

; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

    include \masm32\include64\masm64rt.inc

    ifndef @WordSize
        @WordSize equ 8
    endif
    AllowPromotions = 0

    include \masm32\macros\SmplMath\math.inc

     FP8 MACRO value
        LOCAL vname
        .data
        align 16
          vname REAL8 value
        .code
        EXITM <vname>
     ENDM

     real8$ MACRO r8value:req
        LOCAL buffer
        .data?
            buffer BYTE 320 dup(?)
        IFNDEF r8fmt    
        .data    
            r8fmt  BYTE " %1.4e", 0
            pbuffer qword buffer
        ENDIF    
        .code
            mov    buffer[0], 0
            invoke vc_sprintf, pbuffer, ADDR r8fmt, r8value
            EXITM <pbuffer>
     ENDM

.code

; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

entry_point proc

    call main
    
    waitkey

    invoke ExitProcess,0

    ret

entry_point endp

; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

main proc
    local v1:qword
    local v2:word
    local v1_t:qword, sizeofarray: qword
    local v3: real8
    
    ForLp v1, 0, 5
        .if rax == 3
            CutFor v1
        .endif
        conout str$(v1), lf
    Next v1
    conout "ascendent loop finished",lf,lf


    mov sizeofarray, 4 
    ForLp_df v1, 0, sizeofarray, r13
        .if r13 == 1
            CutFor v1
        .endif
        conout str$(v1), lf
    Next v1
    conout "modified ascendent loop finished",lf,lf

    mov sizeofarray, 4 
    ForLpD v1, 0, sizeofarray, r13
        .if r13 == 1
            CutFor v1
        .endif
        conout str$(v1), lf

    Next v1
    conout "better modified ascendent loop finished",lf,lf

    ForLpN v2, 5, 0, cx
        .if cx == 3
            CutForN v2
        .endif
        xor r11  , r11         ; promotion by hand  
        mov r11w , v2
        conout str$(r11), lf
    NextN v2
    conout "descendent loop finished",lf,lf

    
    ForLpR v3, FP8(0.0), FP8(2.0), FP8(0.5)
        conout real8$(v3), lf
    NextR v3
    conout "floating point loop finished",lf,lf

    ret

main endp
; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

    end
