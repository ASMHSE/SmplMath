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

; いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい�
    include \masm32\include\masm32rt.inc
    .686
    .XMM
    include \masm32\macros\SmplMath\Math.inc
; いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい�

comment * -----------------------------------------------------
                     Build this console app with
                  "MAKEIT.BAT" on the PROJECT menu.
        ----------------------------------------------------- *

    .data?
      value dd ?

    .data
      item dd 0
      res1 dd 0
      res2 dw 0
      BIT equ 2

    .code

start:
   
; いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい�

    call main
    inkey
    exit

; いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい�

main proc

    cls
    print "Bitwise compound operators",13,10

         fSlvB res2 &= BIT, dx
         
         fSlvB res1 |= item
         ;fSlvB BIT &= 5
         fSlvB eax ^= BIT

         fSlvB eax {{ BIT
         fSlvB res1 {{ BIT
         fSlvB res1 }} item
    ret

main endp

; いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい�

end start
