;/************************************************************
; *   This file is a test of the SmplMath macros-system.     *
; *                                                          *
; *            Copyright by HSE, 2020-2023                   *
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

    mov eax, 6
    mov v1, eax
    mov ebx, 4 
    @dothis
        print str$(eax), 13, 10
       
        mov eax, v1
        sub eax, 1
        mov v1, eax
    @whilethis eax ua 0 && ebx ua 3

    print " @dothis/@whilethis finished",13,10,13,10

    mov eax, 5
    mov v1, eax
    @dothis
        print str$(eax),13,10
        .if v1 >= 10
            @breakthis
        .endif
       
        mov eax, v1
        add eax, 1
        mov v1, eax
    @whilethis eax gt 0
    
    print " @dothis/@whilethis finished",13,10,13,10

    inkey


    mov eax, 5
    mov v1, eax
    @dothis
        mov ecx, 0
        mov v2, ecx

        @dothis
            print str$(v1),9
            print str$(v2),13,10
            .if v1 >= 7 && v2 > 3
                @breakthis 1 
            .endif
           
            mov ecx, v2
            add ecx, 1
            mov v2, ecx
        @whilethis  ecx lt 6
        
        .if v1 >= 10
            @breakthis; 1 
        .endif
        mov eax, v1
        add eax, 1
        mov v1, eax
    @whilethis eax gt 0
    
    print " @dothis/@whilethis finished",13,10,13,10

    ret

main endp

; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

end start
