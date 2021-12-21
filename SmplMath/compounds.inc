
;/************************************************************
; * This file is an extension of the SmplMath macros-system. *
; *                                                          *
; *            Copyright by HSE, 2020-2022                   *
; *                                                          *
; *          https://github.com/ASMHSE/SmplMath/             *
; *                                                          *
; *    Further Copyright and Legal statements are located    *
; *    in the documentation (Documentation.pdf).             *
; *                                                          *
; ************************************************************/

;    A little wrapper of fSlv to manage some 
;    compound operators(*=, /=, +=, -=, ++, --)
; 
;    Use:
; 
;         fSlvW res1 = 10.0 + 5.0
;         fSlvW res1 ++
;         fSlvW res1 -= -10
;         fSlvW res1 *= 2 ^ x
;   
;         mov eax, 0
;         fSlvW res1[eax*8] /= 2+ 4 *x
;


fSlvW macro Expression:=< >

    tk_dest TEXTEQU <>
    tk_exp  TEXTEQU <>
    tk_op	TEXTEQU <>
    tk_op1  = 0
    tk_op2  = 0
    FORC char,<&Expression>
        IF tk_op2 EQ 0
            tk_switch INSTR 1,<*/+->,<&char>
            IF tk_switch EQ 0
                IF tk_op1 EQ 0  
                    tk_switch4 INSTR 1,<=>,<&char>
                    If tk_switch4 EQ 1
                        tk_op1 = 1
                        tk_op CATSTR tk_op,<&char>
                        tk_op2 = 1
                    ELSE
                       tk_dest CATSTR tk_dest,<&char>
                    ENDIF
                ELSEIF tk_op1 GT 1  
                    tk_exp CATSTR tk_exp,<&char>
                ELSE
                    tk_switch2 INSTR 1,<=>,<&char>
                    IF tk_switch2 EQ 1  
                        IF tk_op1 EQ 1
                            tk_op1 = 2
                            tk_op CATSTR tk_op,<&char>
                            tk_op2 = 1
                        ELSE
                            tk_exp CATSTR tk_exp,<&char>
                        ENDIF
                    ELSE
                        tk_dest CATSTR tk_dest,tk_op
                        tk_op TEXTEQU <>
                        tk_dest CATSTR tk_dest,<&char>
                        tk_op1 = 0
                    ENDIF       
                ENDIF
            ELSE
                IF tk_op1 EQ 0  
                    tk_op1 = 1
                    tk_op CATSTR tk_op,<&char>
                    tk_switch3 = tk_switch
                ELSE
                    tk_switch2 INSTR 1,<+->,<&char>
                    IF tk_switch2 GT 0
                        tk_is INSTR 1, tk_op, <&char>
                        IF tk_is EQ 1
                            tk_op1 = 3
                        ELSE
                            .err A2052 invalid compound operator
                        ENDIF
                    ELSE
                        .err A2052  invalid compound operator
                    ENDIF
                ENDIF
            ENDIF
        ELSE
            tk_exp CATSTR tk_exp,<&char>
        ENDIF
    ENDM

    IF tk_op1 EQ 1
        tk_dest CATSTR <fSlv >, tk_dest, tk_op, tk_exp
    ELSEIf tk_op1 EQ 2
        IF tk_switch3 EQ 1
            tk_dest CATSTR <fSlv >, tk_dest,<= >,tk_dest,<* (>,tk_exp,<)>
        ELSEIF tk_switch3 EQ 2
            tk_dest CATSTR <fSlv >, tk_dest,<= >,tk_dest,</ (>,tk_exp,<)>
        ELSEIF tk_switch3 EQ 3
            tk_dest CATSTR <fSlv >, tk_dest,<= >,tk_dest,<+ (>,tk_exp,<)>
        ELSEIF tk_switch3 EQ 4
            tk_dest CATSTR <fSlv >, tk_dest,<= >,tk_dest,<- (>,tk_exp,<)>
        ENDIF
    ELSEIf tk_op1 EQ 3
        IF tk_switch3 EQ 3
            tk_dest CATSTR <fSlv >, tk_dest,<= >,tk_dest,<+ 1.0>
        ELSE
            tk_dest CATSTR <fSlv >, tk_dest,<= >,tk_dest,<- 1.0>
        ENDIF
    ENDIF
    tk_dest

endm

;    To manage some compound operators (&=, |=, ^=, {{ and }})
; 
;    Use:
; 
;         fSlvB res1 &= BIT
;         fSlvB res1 |= item
;         fSlvB BIT &= 5
;         fSlvB eax ^= BIT

;         fSlvB eax {{ BIT
;         fSlvB res1 {{ BIT
;         fSlvB res1 }} item

fSlvB macro Expression:=< >

    tk_dest 	TEXTEQU <>
    tk_exp  	TEXTEQU <>
    tk_op	TEXTEQU <>
    tk_op1  = 0
    tk_op2 = 0

    FORC char,<&Expression>
        IF tk_op2 EQ 0
            tk_switch INSTR 1,<&|^{}>,<&char>
            IF tk_switch EQ 0
                IF tk_op1 EQ 0  
                    tk_dest CATSTR tk_dest,<&char>
                ELSE
                    tk_switch2 INSTR 1,<=>,<&char>
                    IF tk_switch2 EQ 1  
                        IF tk_op1 EQ 1
                            tk_op1 = 2
                            tk_op CATSTR tk_op,<&char>
                            tk_op2 = 1
                        ELSE
                            .err A2052  invalid compound operator
                        ENDIF
                    ELSE
                        IF tk_op1 EQ 0  
                           tk_dest CATSTR tk_dest,tk_op
                           tk_op TEXTEQU <>
                           tk_dest CATSTR tk_dest,<&char>
                           tk_op1 = 0
                        ELSE
                            .err A2052  invalid bitwise operator
                        ENDIF
                    ENDIF       
                ENDIF
            ELSE
                IF tk_op1 EQ 0  
                    tk_op1 = 1
                    tk_op CATSTR tk_op,<&char>
                    tk_switch3 = tk_switch
                ELSE
                    tk_switch2 INSTR 1,<{}>,<&char>
                    IF tk_switch2 GT 0
                        echo es doble
                        tk_is INSTR 1, tk_op, <&char>
                        IF tk_is EQ 1
                            echo es igual
                            tk_op2 = 1
                        ELSE
                            .err A2052 invalid compound operator
                        ENDIF
                    ELSE
                        .err A2052  invalid compound operator
                    ENDIF
                ENDIF
           ENDIF
        ELSE
            tk_exp CATSTR tk_exp,<&char>
        ENDIF
    ENDM

    IF tk_switch3 EQ 1
        tk_op3 TEXTEQU <and>
    ELSEIF tk_switch3 EQ 2
        tk_op3 TEXTEQU <or>
    ELSEIF tk_switch3 EQ 3
        tk_op3 TEXTEQU <xor>
    ELSEIF tk_switch3 EQ 4
        tk_op3 TEXTEQU <shl>
    ELSEIF tk_switch3 EQ 5
        tk_op3 TEXTEQU <shr>
    ENDIF

    IF (OPATTR(tk_dest)) and 00010000y ;; register
 ;%echo       tk_op3 tk_dest , tk_exp
        tk_op3 tk_dest , tk_exp
    ELSEIF (OPATTR(tk_dest)) and 00000100y ;; inmediate
        .err A2052 invalid bit operation on inmediate
    ELSE
;%echo        mov eax, tk_dest 
        mov eax, tk_dest 
        IF tk_switch3 LT 4
;%echo            tk_op3 eax , tk_exp
            tk_op3 eax , tk_exp
        ELSE
            IF (OPATTR(tk_exp)) and 00000100y ;; inmediate
;%echo                tk_op3 eax , tk_exp
                tk_op3 eax , tk_exp
            ELSE
%echo                mov ecx, tk_exp 
%echo                tk_op3 eax , cl
                mov ecx, tk_exp 
                tk_op3 eax , cl
            ENDIF
        ENDIF
;%echo        mov tk_dest, eax
        mov tk_dest, eax
    ENDIF
endm

