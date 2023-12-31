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

    GetLogicalOperationD macro ConditionsString
        LOCAL ConditionsStringln 

        __CurrentDoThisLogicalOperation = __IfLogicalOperationNON
        __CurrentDoThisLogicalPos       = 0

        ConditionsStringln SIZESTR <ConditionsString>
        IF ConditionsStringln lt 2
            exitm
        ENDIF
        IF ConditionsStringln ge 8
            L_DoThisLogicalOperationOR  INSTR 1,<ConditionsString>, Cond_OR
            L_DoThisLogicalOperationAND INSTR 1,<ConditionsString>, Cond_AND
            First_Char              SUBSTR <ConditionsString>,1,2+Cond_extra
           IFIDN First_Char, Cond_OR
                __CurrentDoThisLogicalOperation = __IfLogicalOperationOR
                __CurrentDoThisLogicalPos       = 0
                exitm
            ENDIF
            IFIDN First_Char, Cond_AND
                __CurrentDoThisLogicalOperation = __IfLogicalOperationAND
                __CurrentDoThisLogicalPos       = 0
                exitm
            ENDIF
            IF L_DoThisLogicalOperationOR
                IF L_DoThisLogicalOperationAND
                    IF L_DoThisLogicalOperationAND lt L_DoThisLogicalOperationOR
                        __CurrentDoThisLogicalOperation = __IfLogicalOperationAND
                        __CurrentDoThisLogicalPos       = L_DoThisLogicalOperationAND
                    ELSE
                        __CurrentIfLogicalOperation = __IfLogicalOperationOR
                        __CurrentIfLogicalPos       = L_IfLogicalOperationOR
                    ENDIF
                ELSE 
                    __CurrentDoThisLogicalOperation = __IfLogicalOperationOR
                    __CurrentDoThisLogicalPos       = L_IfLogicalOperationOR
                ENDIF
                EXITM
            ELSEIF L_DoThisLogicalOperationAND
                __CurrentDoThisLogicalOperation = __IfLogicalOperationAND
                __CurrentDoThisLogicalPos       = L_DoThisLogicalOperationAND
            ENDIF
        ENDIF
    endm
    
;························································································
    Extract_DoThis_Conditions macro ConditionsStr:req,INVOperatio:req,LableJMP:req,INVLableJMP:req
        LOCAL bracketPos,bracketChar,L__CurrentDoThisLogicalOperation,L__CurrentIndexLbl,L__CurrentDoThisLogicalPos,   ConditionsStrln    
        LOCAL nnextind,L_FirstConditionsStr,L_SecondConditionsStr,L_FirstLable,L_SecondLable,L_NotINVOperatio

        ConditionsStrln SIZESTR <ConditionsStr>
        bracketChar      SUBSTR  <ConditionsStr>,1,1
        nnextind = @CatStr(<__?dothis_lable_>,%__?CurrentDOTHIS)  
        IFIDN bracketChar,<(>
             bracketPos = GetCloseOfbracket(ConditionsStr)
            IF bracketPos eq ConditionsStrln
            %    Extract_DoThis_Conditions @SubStr(<ConditionsStr>,2,ConditionsStrln-2) ,INVOperatio,LableJMP,INVLableJMP
                exitm
            ELSE
                GetLogicalOperationD @SubStr(<ConditionsStr>,bracketPos+1,ConditionsStrln-bracketPos)
                L__CurrentDoThisLogicalOperation =  __CurrentDoThisLogicalOperation
                L__CurrentDoThisLogicalPos       =  bracketPos+1
            ENDIF
        ENDIF
        IFDIF bracketChar,<(>
            GetLogicalOperationD ConditionsStr
            L__CurrentDoThisLogicalOperation =  __CurrentDoThisLogicalOperation
            L__CurrentDoThisLogicalPos       =  __CurrentDoThisLogicalPos
        ENDIF
        ;
        IF L__CurrentDoThisLogicalOperation ne __IfLogicalOperationNON
            L__CurrentIndexLbl = __CurrentDoThisIndexLable
            __CurrentDoThisIndexLable = __CurrentDoThisIndexLable+2
        %    L_FirstConditionsStr    EQU <@SubStr(<ConditionsStr>,1,L__CurrentDoThisLogicalPos-1)>
        %    L_SecondConditionsStr   EQU <@SubStr(<ConditionsStr>,L__CurrentDoThisLogicalPos+2+6,ConditionsStrln-L__CurrentDoThisLogicalPos-1-6)>
        %    L_FirstLable            EQU <@CatStr(<__?dothis_Next_lable_>,%__?CurrentDOTHIS,<_>,%nnextind,<_>,%L__CurrentIndexLbl+1)>
        %    L_SecondLable           EQU <@CatStr(<__?dothis_Next_lable_>,%__?CurrentDOTHIS,<_>,%nnextind,<_>,%L__CurrentIndexLbl+2)>

            IF INVOperatio
                L_NotINVOperatio=FALSE
            ELSE
                L_NotINVOperatio=TRUE
            ENDIF
            IF L__CurrentDoThisLogicalOperation eq __IfLogicalOperationAND
        ;%echo ...1: [L_FirstConditionsStr,TRUE,L_FirstLable,INVLableJMP]
        %        Extract_DoThis_Conditions L_FirstConditionsStr,TRUE,L_FirstLable,INVLableJMP;L_NotINVOperatio
            ELSE
        ;%echo ...2: [L_FirstConditionsStr,FALSE,LableJMP,L_FirstLable]
        %        Extract_DoThis_Conditions L_FirstConditionsStr,FALSE,LableJMP,L_FirstLable;INVOperatio;L_SecondLable
            ENDIF
            L_FirstLable:
    ;%echo    L_FirstLable:
            IF L__CurrentDoThisLogicalOperation eq __IfLogicalOperationAND
    ;    %        Extract_DoThis_Conditions L_SecondConditionsStr,TRUE,L_SecondLable,INVLableJMP;L_NotINVOperatio
                IF    INVOperatio
        ;%echo ...3: [L_SecondConditionsStr,TRUE,L_SecondLable,INVLableJMP]
        %            Extract_DoThis_Conditions L_SecondConditionsStr,TRUE,L_SecondLable,INVLableJMP;L_NotINVOperatio
                ELSE
        ;%echo ...4: [L_SecondConditionsStr,FALSE,LableJMP,L_FirstLable]
        %            Extract_DoThis_Conditions L_SecondConditionsStr,FALSE,LableJMP,INVLableJMP;L_FirstLable;L_NotINVOperatio
                ENDIF
            ELSE
    ;    %        Extract_DoThis_Conditions L_SecondConditionsStr,TRUE,L_SecondLable,INVLableJMP;L_NotINVOperatio
    ;false
                IF    INVOperatio
        ;%echo ...5: [L_SecondConditionsStr,TRUE,L_SecondLable,INVLableJMP]
        %            Extract_DoThis_Conditions L_SecondConditionsStr,TRUE,L_SecondLable,INVLableJMP;L_NotINVOperatio
                ELSE
        ;%echo ...6: [L_SecondConditionsStr,FALSE,LableJMP,L_FirstLable]
        %            Extract_DoThis_Conditions L_SecondConditionsStr,FALSE,LableJMP,INVLableJMP;L_NotINVOperatio
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
    Put_DoThis_Conditions macro ConditionsString:req
        LOCAL nnextind,nnextindj
        __CurrentDoThisIndexLable       = 0
        nnextind = @CatStr(<__?dothis_lable_>,%__?CurrentDOTHIS)  
    %    Extract_DoThis_Conditions TrimString(ConditionsString),TRUE,@CatStr(<__?dothis_Next_lable_>,%__?CurrentDOTHIS,<_>,%nnextind,<_0>),@CatStr(<__?dothis_Next_lable_>,%__?CurrentDOTHIS,<_>,%nnextind+1) 
    endm

;---------------------------------------------------------------------------------------

    @dothis macro
        LOCAL nnextind
        ifndef __?dothisCountGlob
            __?dothisCountGlob  =  0
            __?dothisCountCurr  =  0
            __?dothislastopen   =  0 
        endif
          __?dothisCountGlob  =  __?dothisCountGlob + 1
          __?dothisCountCurr  =  __?dothisCountCurr + 1
          __?dothislastopen   =  __?dothislastopen  + 1
          
        IF __?dothisCountCurr eq 1
            @CatStr(<__?WHILE_Last_>,%__?dothisCountGlob) = 0
        ELSE
            @CatStr(<__?WHILE_Last_>,%__?dothisCountGlob) = __?CurrentDOTHIS
        ENDIF
          if __?dothislastopen eq 1
            __?dothislastglobalcount = __?dothisCountGlob
          endif
        
        __?CurrentDOTHIS = __?dothisCountGlob 
        @CatStr(<__?dothis_lable_>,%__?CurrentDOTHIS) = 0

        nnextind = @CatStr(<__?dothis_lable_>,%__?CurrentDOTHIS)
            
        @CatStr(<__?dothis_Next_lable_>,%__?CurrentDOTHIS,<_>,%nnextind,<_0>):

    endm

@breakthis macro nbucle:=<0>
    LOCAL nnextind
    nnextind = @CatStr(<__?dothis_lable_>,%(__?CurrentDOTHIS))+1
    jmp @CatStr(<__?dothis_Next_lable_>,%(__?CurrentDOTHIS-nbucle),<_>,%(nnextind+1))
endm

@whilethis macro  ___CondArg
    LOCAL nnextind

    @CatStr(<Conditions>,%__?dothisCountGlob) textequ <___CondArg>

    @CatStr(<__?dothis_lable_>,%__?CurrentDOTHIS) = @CatStr(<__?dothis_lable_>,%__?CurrentDOTHIS) + 1

    nnextind = @CatStr(<__?dothis_lable_>,%(__?CurrentDOTHIS))
    @CatStr(<__?dothis_Next_lable_>,%__?CurrentDOTHIS,<_>,%(nnextind)):
    __?dothislastopen  = __?dothislastopen - 1
    
    Put_DoThis_Conditions <___CondArg>

    jmp @CatStr(<__?dothis_Next_lable_>,%__?CurrentDOTHIS,<_>,%(nnextind-1),<_0>)
    @CatStr(<__?dothis_Next_lable_>,%__?CurrentDOTHIS,<_>,%(nnextind+1)):
    @CatStr(<__?dothis_End_lable_>,%__?CurrentDOTHIS):
    __?dothisCountCurr = __?dothisCountCurr - 1
    __?CurrentDOTHIS   = @CatStr(<__?WHILE_Last_>,%__?CurrentDOTHIS)
endm


;**************************************************************************************

	GetLogicalOperationU macro ConditionsString
		LOCAL ConditionsStringln 

		__CurrentUntilLogicalOperation = __IfLogicalOperationNON
		__CurrentUntilLogicalPos       = 0

		ConditionsStringln SIZESTR <ConditionsString>
		IF ConditionsStringln lt 2
			exitm
		ENDIF
              IF ConditionsStringln ge 8
                	L_UntilLogicalOperationOR  INSTR 1,<ConditionsString>, Cond_OR
        		L_UntilLogicalOperationAND INSTR 1,<ConditionsString>, Cond_AND
        		First_Char  			SUBSTR <ConditionsString>,1,2+Cond_extra
               	IFIDN First_Char, Cond_OR
        			__CurrentUntilLogicalOperation = __IfLogicalOperationOR
        			__CurrentUntilLogicalPos		= 0
        			exitm
        		ENDIF
        		IFIDN First_Char, Cond_AND
        			__CurrentUntilLogicalOperation = __IfLogicalOperationAND
        			__CurrentUntilLogicalPos		= 0
        			exitm
        		ENDIF
        		IF L_UntilLogicalOperationOR
        			IF L_UntilLogicalOperationAND
        				IF L_UntilLogicalOperationAND lt L_UntilLogicalOperationOR
        					__CurrentUntilLogicalOperation = __IfLogicalOperationAND
        					__CurrentUntilLogicalPos		= L_UntilLogicalOperationAND
        				ELSE
        					__CurrentIfLogicalOperation = __IfLogicalOperationOR
        					__CurrentIfLogicalPos		= L_IfLogicalOperationOR
        				ENDIF
        			ELSE 
        				__CurrentUntilLogicalOperation = __IfLogicalOperationOR
        				__CurrentUntilLogicalPos		= L_IfLogicalOperationOR
        			ENDIF
        			EXITM
        		ELSEIF L_UntilLogicalOperationAND
        			__CurrentUntilLogicalOperation = __IfLogicalOperationAND
        			__CurrentUntilLogicalPos		= L_UntilLogicalOperationAND
        		ENDIF
              ENDIF
	endm


;························································································
	Extract_Until_Conditions macro ConditionsStr:req,INVOperatio:req,LableJMP:req,INVLableJMP:req
		LOCAL bracketPos,bracketChar,L__CurrentUntilLogicalOperation,L__CurrentIndexLbl,L__CurrentUntilLogicalPos,   ConditionsStrln    
		LOCAL nnextind,L_FirstConditionsStr,L_SecondConditionsStr,L_FirstLable,L_SecondLable,L_NotINVOperatio

		ConditionsStrln SIZESTR <ConditionsStr>
		bracketChar  	SUBSTR  <ConditionsStr>,1,1
		nnextind = @CatStr(<__?dothis_lable_>,%__?CurrentDOTHIS)  
		IFIDN bracketChar,<(>
		 	bracketPos = GetCloseOfbracket(ConditionsStr)
			IF bracketPos eq ConditionsStrln
			%	Extract_Until_Conditions @SubStr(<ConditionsStr>,2,ConditionsStrln-2) ,INVOperatio,LableJMP,INVLableJMP
				exitm
			ELSE
				GetLogicalOperationU @SubStr(<ConditionsStr>,bracketPos+1,ConditionsStrln-bracketPos)
				L__CurrentUntilLogicalOperation 	= 	__CurrentUntilLogicalOperation
				L__CurrentUntilLogicalPos			=	bracketPos+1
			ENDIF
		ENDIF
		IFDIF bracketChar,<(>
			GetLogicalOperationU ConditionsStr
			L__CurrentUntilLogicalOperation 	= __CurrentUntilLogicalOperation
			L__CurrentUntilLogicalPos			= __CurrentUntilLogicalPos
		ENDIF
		;
		IF L__CurrentUntilLogicalOperation ne __IfLogicalOperationNON
			L__CurrentIndexLbl=__CurrentUntilIndexLable
			__CurrentUntilIndexLable=__CurrentUntilIndexLable+2
		%	L_FirstConditionsStr	EQU <@SubStr(<ConditionsStr>,1,L__CurrentUntilLogicalPos-1)>
		%	L_SecondConditionsStr	EQU <@SubStr(<ConditionsStr>,L__CurrentUntilLogicalPos+2+6,ConditionsStrln-L__CurrentUntilLogicalPos-1-6)>
		%	L_FirstLable			EQU <@CatStr(<__?dothis_Next_lable_>,%__?CurrentDOTHIS,<_>,%nnextind,<_>,%L__CurrentIndexLbl+1)>
		%	L_SecondLable			EQU <@CatStr(<__?dothis_Next_lable_>,%__?CurrentDOTHIS,<_>,%nnextind,<_>,%L__CurrentIndexLbl+2)>

			IF INVOperatio
				L_NotINVOperatio=FALSE
			ELSE
				L_NotINVOperatio=TRUE
			ENDIF
			IF L__CurrentUntilLogicalOperation eq __IfLogicalOperationAND
		;%echo ...1: [L_FirstConditionsStr,TRUE,L_FirstLable,INVLableJMP]
		%		Extract_Until_Conditions L_FirstConditionsStr,TRUE,L_FirstLable,INVLableJMP;L_NotINVOperatio
			ELSE
		;%echo ...2: [L_FirstConditionsStr,FALSE,LableJMP,L_FirstLable]
		%		Extract_Until_Conditions L_FirstConditionsStr,FALSE,LableJMP,L_FirstLable;INVOperatio;L_SecondLable
			ENDIF
			L_FirstLable:
	;%echo	L_FirstLable:
			IF L__CurrentUntilLogicalOperation eq __IfLogicalOperationAND
	;	%		Extract_Until_Conditions L_SecondConditionsStr,TRUE,L_SecondLable,INVLableJMP;L_NotINVOperatio
				IF	INVOperatio
		;%echo ...3: [L_SecondConditionsStr,TRUE,L_SecondLable,INVLableJMP]
		%			Extract_Until_Conditions L_SecondConditionsStr,TRUE,L_SecondLable,INVLableJMP;L_NotINVOperatio
				ELSE
		;%echo ...4: [L_SecondConditionsStr,FALSE,LableJMP,L_FirstLable]
		%			Extract_Until_Conditions L_SecondConditionsStr,FALSE,LableJMP,INVLableJMP;L_FirstLable;L_NotINVOperatio
				ENDIF
			ELSE
	;	%		Extract_Until_Conditions L_SecondConditionsStr,TRUE,L_SecondLable,INVLableJMP;L_NotINVOperatio
	;false
				IF	INVOperatio
		;%echo ...5: [L_SecondConditionsStr,TRUE,L_SecondLable,INVLableJMP]
		%			Extract_Until_Conditions L_SecondConditionsStr,TRUE,L_SecondLable,INVLableJMP;L_NotINVOperatio
				ELSE
		;%echo ...6: [L_SecondConditionsStr,FALSE,LableJMP,L_FirstLable]
		%			Extract_Until_Conditions L_SecondConditionsStr,FALSE,LableJMP,INVLableJMP;L_NotINVOperatio
				ENDIF
			ENDIF
			L_SecondLable:
	;%echo	L_SecondLable:
			EXITM
		ENDIF
		if INVOperatio
	%		PutComparOperation ConditionsStr,INVOperatio,INVLableJMP
		else
	%		PutComparOperation ConditionsStr,INVOperatio,LableJMP
		endif
			
	;echo .....................
	;echo ++++++++++++++++++
	endm
;---------------------------------------------------------------------------------------
	Put_Until_Conditions macro ConditionsString:req
		LOCAL nnextind,nnextindj
		__CurrentUntilIndexLable       = 0
		nnextind = @CatStr(<__?dothis_lable_>,%__?CurrentDOTHIS)  
	%	Extract_Until_Conditions TrimString(ConditionsString),FALSE,@CatStr(<__?dothis_Next_lable_>,%__?CurrentDOTHIS,<_>,%(nnextind+1)),@CatStr(<__?dothis_End_lable_>,%__?CurrentDOTHIS)
	endm

;---------------------------------------------------------------------------------------
    @repeat textequ <@dothis>

    @until macro  ___CondArg
        LOCAL nnextind

        @CatStr(<Conditions>,%__?dothisCountGlob) textequ <___CondArg>


        @CatStr(<__?dothis_lable_>,%__?CurrentDOTHIS) = @CatStr(<__?dothis_lable_>,%__?CurrentDOTHIS) + 1

        nnextind = @CatStr(<__?dothis_lable_>,%(__?CurrentDOTHIS))
        @CatStr(<__?dothis_Next_lable_>,%__?CurrentDOTHIS,<_>,%(nnextind)):
        __?dothislastopen  = __?dothislastopen - 1
        
        Put_Until_Conditions <___CondArg>

        jmp @CatStr(<__?dothis_Next_lable_>,%__?CurrentDOTHIS,<_>,%(nnextind-1),<_0>)
        @CatStr(<__?dothis_Next_lable_>,%__?CurrentDOTHIS,<_>,%(nnextind+1)):
        @CatStr(<__?dothis_End_lable_>,%__?CurrentDOTHIS):
        __?dothisCountCurr = __?dothisCountCurr - 1
        __?CurrentDOTHIS   = @CatStr(<__?WHILE_Last_>,%__?CurrentDOTHIS)
    endm

