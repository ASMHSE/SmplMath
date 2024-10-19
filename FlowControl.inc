;/************************************************************
; *   This file is a test of the SmplMath macros-system.     *
; *                                                          *
; *            Copyright by HSE, 2020-2024                   *
; *                                                          *
; *          https://github.com/ASMHSE/SmplMath/             *
; *                                                          *
; *    Further Copyright and Legal statements are located    *
; *    in the documentation (Documentation.pdf).             *
; *                                                          *
; ************************************************************/


; Mabdelouahab's Macros 2017 [http://masm32.com/board/index.php?topic=5956.msg63211#msg63211]
; HSE extension floating point comparisons 2020
; HSE extension unsigned comparisons 2021
; HSE extension floating point checks 2021
; HSE obliteration instead of remotion of ab and space 2021
; HSE @while-@break-@endw macros 2022


  ; ************************************************

    run_time_comparisons MACRO
      % echo     **************************************
      % echo     variable run time comparison operators
      % echo     **************************************
      % echo     -------------
      % echo     equality test
      % echo     -------------
      % echo     eq equal
      % echo     ne not equal
      % echo    
      % echo     -----------------
      % echo     signed comparison
      % echo     -----------------
      % echo     gt signed greater than
      % echo     lt signed less than
      % echo     ge signed greater than or equal
      % echo     le signed less than or equal
      % echo    
      % echo     -------------------
      % echo     unsigned comparison
      % echo     -------------------
      % echo     ua unsigned above
      % echo     ub unsigned below
      % echo     ae unsigned above or equal
      % echo     be unsigned below or equal
      % echo    
      % echo     -------------------------
      % echo     floating point comparison
      % echo     -------------------------
      % echo     fEQ equal
      % echo     fNE not equal
      % echo    
      % echo     fGT greater than
      % echo     fLT less than
      % echo     fGE greater than or equal
      % echo     fLE less than or equal
      % echo    
      % echo     --------------------
      % echo     floating point check
      % echo     --------------------
      % echo     r4Is  check real4 is NotNaN/Valid
      % echo     r8Is  check real8 is NotNaN/Valid
      % echo    
      % echo     -------------------------
      % echo     doubledouble floating point comparison
      % echo     -------------------------
      % echo     rrGT greater than
      % echo     rrLT less than
      % echo     rrGE greater than or equal
      % echo     rrLE signed less than or equal
      % echo     **************************************
    ENDM
  ; ************************************************
  

    TypeOf MACRO arg
        EXITM % opattr(arg)
    ENDM

    TrimString macro string___
        LOCAL ?Ltmp_string___,Ltmp_string___,Ltmp_Char,L_TabPos,L_SpcPos
        LOCAL L_ComparOperation,firstpart,secondpart         ; Remove ptr obliteration        

        ?Ltmp_string___ TEXTEQU <string___>
        Ltmp_string___  TEXTEQU <>
        % FORC Ltmp_Char,<?Ltmp_string___>
            L_TabPos INSTR 1,<	>,<Ltmp_Char>
            L_SpcPos INSTR 1,< >,<Ltmp_Char>
            IFE L_TabPos
                IFE L_SpcPos
                    Ltmp_string___ TEXTEQU Ltmp_string___,<&Ltmp_Char>
                ELSE
                    Ltmp_string___ TEXTEQU Ltmp_string___,<___>
                ENDIF
            ELSE
                Ltmp_string___ TEXTEQU Ltmp_string___,<___>
            ENDIF
        ENDM

        ; Remove ptr obliteration        
        repeat 2        
        L_ComparOperation  	INSTR 1,Ltmp_string___, <___ptr___>
        IF L_ComparOperation
            firstpart SUBSTR Ltmp_string___,1, L_ComparOperation-1
            secondpart SUBSTR Ltmp_string___,L_ComparOperation+9
            Ltmp_string___ TEXTEQU firstpart, < ptr >,secondpart
        ENDIF
        endm
        repeat 2    
        L_ComparOperation  	INSTR 1,Ltmp_string___, <___ptr>
        IF L_ComparOperation
            firstpart SUBSTR Ltmp_string___,1, L_ComparOperation-1
            secondpart SUBSTR Ltmp_string___,L_ComparOperation+6
            Ltmp_string___ TEXTEQU firstpart, < ptr >,secondpart
        ENDIF
        endm

        EXITM <Ltmp_string___>
    endm

       ObliterateOperation macro nameop:req
           EXITM @CatStr(<___>,<nameop>,<___>)
       endm 
       LiberateOperation macro nameop:req
           LOCAL nameln
           namelm SIZESTR <&nameop>
           EXITM <@Substr(&nameop,3, nameln-6)>
       endm 
       Obliterateln equ 6


include ifendif.inc
include while.inc
include dothis.inc


