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

    GetLogicalOperationW macro ConditionsString
        LOCAL ConditionsStringln 

        __CurrentWhileLogicalOperation = __IfLogicalOperationNON
        __CurrentWhileLogicalPos       = 0

        ConditionsStringln SIZESTR <ConditionsString>
        IF ConditionsStringln lt 2
            exitm
        ENDIF
        IF ConditionsStringln ge 8
                L_WhileLogicalOperationOR  INSTR 1,<ConditionsString>, Cond_OR
            L_WhileLogicalOperationAND INSTR 1,<ConditionsString>, Cond_AND
            First_Char              SUBSTR <ConditionsString>,1,2+Cond_extra
            IFIDN First_Char, Cond_OR
                __CurrentWhileLogicalOperation = __IfLogicalOperationOR
                __CurrentWhileLogicalPos       = 0
                exitm
            ENDIF
            IFIDN First_Char, Cond_AND
                __CurrentWhileLogicalOperation = __IfLogicalOperationAND
                __CurrentWhileLogicalPos       = 0
                exitm
            ENDIF
            IF L_WhileLogicalOperationOR
                IF L_WhileLogicalOperationAND
                    IF L_WhileLogicalOperationAND lt L_WhileLogicalOperationOR
                        __CurrentWhileLogicalOperation = __IfLogicalOperationAND
                        __CurrentWhileLogicalPos       = L_WhileLogicalOperationAND
                    ELSE
                        __CurrentIfLogicalOperation = __IfLogicalOperationOR
                        __CurrentIfLogicalPos       = L_IfLogicalOperationOR
                    ENDIF
                ELSE 
                    __CurrentWhileLogicalOperation = __IfLogicalOperationOR
                    __CurrentWhileLogicalPos       = L_IfLogicalOperationOR
                ENDIF
                EXITM
            ELSEIF L_WhileLogicalOperationAND
                __CurrentWhileLogicalOperation = __IfLogicalOperationAND
                __CurrentWhileLogicalPos       = L_WhileLogicalOperationAND
            ENDIF
        ENDIF
    endm

;························································································
    Extract_While_Conditions macro ConditionsStr:req,INVOperatio:req,LableJMP:req,INVLableJMP:req
        LOCAL bracketPos,bracketChar,L__CurrentWhileLogicalOperation,L__CurrentIndexLbl,L__CurrentWhileLogicalPos,   ConditionsStrln    
        LOCAL nnextind,L_FirstConditionsStr,L_SecondConditionsStr,L_FirstLable,L_SecondLable,L_NotINVOperatio

        ConditionsStrln SIZESTR <ConditionsStr>
        bracketChar      SUBSTR  <ConditionsStr>,1,1
        nnextind = @CatStr(<__?while_lable_>,%__?CurrentWHILE)  
        IFIDN bracketChar,<(>
             bracketPos = GetCloseOfbracket(ConditionsStr)
            IF bracketPos eq ConditionsStrln
            %    Extract_While_Conditions @SubStr(<ConditionsStr>,2,ConditionsStrln-2) ,INVOperatio,LableJMP,INVLableJMP
                exitm
            ELSE
                GetLogicalOperationW @SubStr(<ConditionsStr>,bracketPos+1,ConditionsStrln-bracketPos)
                L__CurrentWhileLogicalOperation  =  __CurrentWhileLogicalOperation
                L__CurrentWhileLogicalPos        =  bracketPos+1
            ENDIF
        ENDIF
        IFDIF bracketChar,<(>
            GetLogicalOperationW ConditionsStr
            L__CurrentWhileLogicalOperation  =  __CurrentWhileLogicalOperation
            L__CurrentWhileLogicalPos        =  __CurrentWhileLogicalPos
        ENDIF
        ;
        IF L__CurrentWhileLogicalOperation ne __IfLogicalOperationNON
            L__CurrentIndexLbl=__CurrentWhileIndexLable
            __CurrentWhileIndexLable=__CurrentWhileIndexLable+2
        %    L_FirstConditionsStr   EQU <@SubStr(<ConditionsStr>,1,L__CurrentWhileLogicalPos-1)>
        %    L_SecondConditionsStr  EQU <@SubStr(<ConditionsStr>,L__CurrentWhileLogicalPos+2+6,ConditionsStrln-L__CurrentWhileLogicalPos-1-6)>
        %    L_FirstLable           EQU <@CatStr(<__?while_Next_lable_>,%__?CurrentWHILE,<_>,%nnextind,<_>,%L__CurrentIndexLbl+1)>
        %    L_SecondLable          EQU <@CatStr(<__?while_Next_lable_>,%__?CurrentWHILE,<_>,%nnextind,<_>,%L__CurrentIndexLbl+2)>

            IF INVOperatio
                L_NotINVOperatio=FALSE
            ELSE
                L_NotINVOperatio=TRUE
            ENDIF
            IF L__CurrentWhileLogicalOperation eq __IfLogicalOperationAND
        ;%echo ...1: [L_FirstConditionsStr,TRUE,L_FirstLable,INVLableJMP]
        %        Extract_While_Conditions L_FirstConditionsStr,TRUE,L_FirstLable,INVLableJMP;L_NotINVOperatio
            ELSE
        ;%echo ...2: [L_FirstConditionsStr,FALSE,LableJMP,L_FirstLable]
        %        Extract_While_Conditions L_FirstConditionsStr,FALSE,LableJMP,L_FirstLable;INVOperatio;L_SecondLable
            ENDIF
            L_FirstLable:
    ;%echo    L_FirstLable:
            IF L__CurrentWhileLogicalOperation eq __IfLogicalOperationAND
    ;    %        Extract_While_Conditions L_SecondConditionsStr,TRUE,L_SecondLable,INVLableJMP;L_NotINVOperatio
                IF    INVOperatio
        ;%echo ...3: [L_SecondConditionsStr,TRUE,L_SecondLable,INVLableJMP]
        %            Extract_While_Conditions L_SecondConditionsStr,TRUE,L_SecondLable,INVLableJMP;L_NotINVOperatio
                ELSE
        ;%echo ...4: [L_SecondConditionsStr,FALSE,LableJMP,L_FirstLable]
        %            Extract_While_Conditions L_SecondConditionsStr,FALSE,LableJMP,INVLableJMP;L_FirstLable;L_NotINVOperatio
                ENDIF
            ELSE
    ;    %        Extract_While_Conditions L_SecondConditionsStr,TRUE,L_SecondLable,INVLableJMP;L_NotINVOperatio
    ;false
                IF    INVOperatio
        ;%echo ...5: [L_SecondConditionsStr,TRUE,L_SecondLable,INVLableJMP]
        %            Extract_While_Conditions L_SecondConditionsStr,TRUE,L_SecondLable,INVLableJMP;L_NotINVOperatio
                ELSE
        ;%echo ...6: [L_SecondConditionsStr,FALSE,LableJMP,L_FirstLable]
        %            Extract_While_Conditions L_SecondConditionsStr,FALSE,LableJMP,INVLableJMP;L_NotINVOperatio
                ENDIF
            ENDIF
            L_SecondLable:
    ;%echo    L_SecondLable:
            EXITM
        ENDIF
        if INVOperatio
    %        PutComparOperation ConditionsStr,INVOperatio,INVLableJMP
        else
    %        PutComparOperation ConditionsStr,INVOperatio,LableJMP
        endif
            
    ;echo .....................
    ;echo ++++++++++++++++++
    endm
;---------------------------------------------------------------------------------------
    Put_While_Conditions macro ConditionsString:req
        LOCAL nnextind,nnextindj
        __CurrentWhileIndexLable       = 0
        nnextind = @CatStr(<__?while_lable_>,%__?CurrentWHILE)  
    %    Extract_While_Conditions TrimString(ConditionsString),TRUE,@CatStr(<__?while_Next_lable_>,%__?CurrentWHILE,<_>,%nnextind,<_0>),@CatStr(<__?while_Next_lable_>,%__?CurrentWHILE,<_>,%nnextind+1) 
    endm

    @while macro ___CondArg
        LOCAL nnextind
        ifndef __?whileCountGlob
            __?whileCountGlob  =  0
            __?whileCountCurr  =  0
            __?whilelastopen   =  0 
        endif
        __?whileCountGlob  =  __?whileCountGlob + 1
        __?whileCountCurr  =  __?whileCountCurr + 1
        __?whilelastopen   =  __?whilelastopen  + 1
              
        IF __?whileCountCurr eq 1
            @CatStr(<__?WHILE_Last_>,%__?whileCountGlob)  = 0
        ELSE
            @CatStr(<__?WHILE_Last_>,%__?whileCountGlob)  = __?CurrentWHILE
        ENDIF
        if __?whilelastopen eq 1
            __?whilelastglobalcount = __?whileCountGlob
        endif
        
        __?CurrentWHILE = __?whileCountGlob 
        @CatStr(<__?while_lable_>,%__?CurrentWHILE)=0
        @CatStr(<Conditions>,%__?whileCountGlob) textequ <___CondArg>

        nnextind = @CatStr(<__?while_lable_>,%__?CurrentWHILE)  
                
             jmp @CatStr(<__?while_Next_lable_>,%__?CurrentWHILE,<_>,%(nnextind+1))

        @CatStr(<__?while_Next_lable_>,%__?CurrentWHILE,<_>,%nnextind,<_0>):

    endm

    @break macro nbucle:=<0>
        LOCAL nnextind
        
        nnextind = @CatStr(<__?while_lable_>,%(__?CurrentWHILE))+1
        jmp @CatStr(<__?while_Next_lable_>,%(__?CurrentWHILE-nbucle),<_>,%(nnextind+1))
    endm

    @endw macro
        LOCAL nnextind

        @CatStr(<__?while_lable_>,%__?CurrentWHILE) = @CatStr(<__?while_lable_>,%__?CurrentWHILE) + 1

        nnextind = @CatStr(<__?while_lable_>,%(__?CurrentWHILE))
        @CatStr(<__?while_Next_lable_>,%__?CurrentWHILE,<_>,%(nnextind)):
        __?whilelastopen  = __?whilelastopen - 1
        
        Put_While_Conditions @CatStr(<Conditions>,%(__?whilelastglobalcount + __?whilelastopen))

        jmp @CatStr(<__?while_Next_lable_>,%__?CurrentWHILE,<_>,%(nnextind-1),<_0>)
        @CatStr(<__?while_Next_lable_>,%__?CurrentWHILE,<_>,%(nnextind+1)):
        @CatStr(<__?while_End_lable_>,%__?CurrentWHILE):
        __?whileCountCurr = __?whileCountCurr -     1
        __?CurrentWHILE   = @CatStr(<__?WHILE_Last_>,%__?CurrentWHILE)
    endm

