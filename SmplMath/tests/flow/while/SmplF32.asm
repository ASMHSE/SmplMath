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
    include \masm32\include\masm32rt.inc
    .686
    .XMM
    ifndef @WordSize
        @WordSize equ 4
    endif

    include \masm32\macros\SmplMath\math.inc
; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

comment * -----------------------------------------------------
                     Build this console app with
                  "MAKEIT.BAT" on the PROJECT menu.
        ----------------------------------------------------- *

    .data?
      value dd ?

    .data
      item dd 0

    .code

start:
   
; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

    call main
    inkey
    exit

; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

main proc uses esi
    local v1:dword, v2:dword
    
    cls
;    int 3

    mov eax, 6
    mov v1, eax
    mov ebx, 4 
    .while eax > 0 && ebx > 3
        print str$(eax), 13, 10
       
        mov eax, v1
        sub eax, 1
        mov v1, eax
    .endw
    print " .while/.endw finished",13,10,13,10

    mov eax, 6
    mov v1, eax
    mov esi, 1
    @while eax ua 0 && ebx ua 3 && esi eq 1
        print str$(eax), 13, 10
       
        mov eax, v1
        sub eax, 1
        mov v1, eax
    @endw
    print " @while/@endw finished",13,10,13,10

   mov eax, 5
   mov v1, eax
    @while eax gt 0
        print str$(eax),13,10
        .if v1 >= 10
            @break
        .endif
       
        mov eax, v1
        add eax, 1
        mov v1, eax
    @endw
    print " @while/@endw finished",13,10,13,10

    mov eax, 5
    mov v1, eax
    @while eax gt 0
        mov ecx, 0
        mov v2, ecx

        @while ecx lt 6
            print str$(v1),9
            print str$(v2),13,10
            .if v1 >= 7 && v2 > 3
                ;int 3
                @break 1 
            .endif
           
            mov ecx, v2
            add ecx, 1
            mov v2, ecx
        @endw
            .if v1 >= 10
                @break; 1 
            .endif
        mov eax, v1
        add eax, 1
        mov v1, eax
    @endw
    print " @while/@endw finished",13,10,13,10


    ret

main endp

; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

end start
