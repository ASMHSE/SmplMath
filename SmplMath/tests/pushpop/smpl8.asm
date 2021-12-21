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

    local fregTLS()

    mov rbx, 200
    mov rax, 50
    mov rcx, 10
    
    freg_push rax
    freg_push rbx
    freg_push ecx
    
    mov rax, 100
    mov rbx, 0
    
    freg_pop ecx
    
    freg_push rax
    mov rax, 32
    conout "rax",tab
    freg_pop rax
    conout str$(rax),lf,lf

    call other

    call withconditional

    conout "rbx",tab
    
    freg_pop rbx
    conout str$(rbx),lf

    conout "rax", tab
    freg_pop rax
    conout str$(rax),lf,lf

    ret
main endp

other proc

    local fregTLS()
    
    conout "    eax", tab

    mov eax, 1350
    freg_push eax

    mov eax, 2264

    freg_pop eax
    conout str$(eax),lf,lf

    ret
other endp

withconditional proc
    local val1 : QWORD
    local val2 : QWORD
    local fregTLS()
    
    mov rax, 512
    freg_push rax
    mov rax, 1
    .if rax == 0
        mov val1, 36
        freg_popv val1
    .else
        mov val1, 48
        freg_correction +1
        freg_popv val1
    .endif

    conout "    val1", tab
    conout str$(val1), lf,lf

    .if rax == 0
        freg_pushv val1
    .else
        freg_correction -1
        freg_push rax
        freg_peekv val2 
        conout str$(val2), lf,lf

    .endif
    conout "    rax", tab
    freg_pop rax
    conout str$(rax), lf,lf

    ret
withconditional endp
    end
