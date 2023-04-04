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
    .code

; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

entry_point proc

    conout "Howdy, your new console template here.",lf,lf
    call main
    waitkey

    invoke ExitProcess,0

    ret

entry_point endp

; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

main proc uses rbx
    local value1:QWORD

    mov rax, 5
    mov value1, rax
    @while rax gt 0
        conout str$(rax),lf
       
        mov rax, value1
        sub rax, 1
        mov value1, rax
    @endw
    conout " @while/@endw finished",lf,lf


    @while rax le 7
        conout str$(rax),lf
       
        mov rax, value1
        add rax, 1
        mov value1, rax
    @endw
    conout " @while/@endw finished",lf,lf


   mov rax, 5
   mov value1, rax
    @while rax gt 0
        conout str$(rax),lf
        .if value1 }= 10
            @break
        .endif
       
        mov rax, value1
        add rax, 1
        mov value1, rax
    @endw
    conout " @while/@endw finished",lf,lf

    ret
main endp

END 