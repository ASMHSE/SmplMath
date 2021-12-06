; Mab Macros 2017
; HSE extension floating point comparisons 2020
; HSE extension unsigned comparisons 2021
; HSE extension floating point checks 2021
; HSE oblieration instead of remotion of ab and space 2021


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
      % echo     fLE signed less than or equal
      % echo    
      % echo     --------------------
      % echo     floating point check
      % echo     --------------------
      % echo     r4Is  check real4 is NotNaN/Valid
      % echo     r8Is  check real8 is NotNaN/Valid
      % echo     **************************************
    ENDM
  ; ************************************************
if 0
r4Is_NotNaN macro mem32:req
	IF @WordSize EQ 4
		push mem32
		and DWORD ptr [esp],7FF80000h
		xor DWORD ptr [esp],7FF80000h
		lea esp,[esp+4]
	ELSE
		push rax
		mov eax,mem32
		and eax,7F800000h
		xor eax,7F800000h
		pop rax
	ENDIF
endm

r8Is_NotNaN macro mem64:req
	IF @WordSize EQ 4
		push DWORD ptr mem64+4
		and DWORD ptr [esp],7FF80000h
		xor DWORD ptr [esp],7FF80000h
		lea esp,[esp+4]
	ELSE
		push rax
		mov eax,DWORD ptr mem64+4
		and eax,7FF80000h
		xor eax,7FF80000h
		pop rax
	ENDIF
endm
r4Is_Valid macro mem32:req
	IF @WordSize EQ 4
		push mem32
		and DWORD ptr [esp],7F800000h
		xor DWORD ptr [esp],7F800000h
		lea esp,[esp+4]
	ELSE
		push rax
		mov eax,mem32
		and eax,7F800000h
		xor eax,7F800000h
		pop rax
	ENDIF
endm
r8Is_Valid macro mem64:req
	IF @WordSize EQ 4
		push DWORD ptr mem64+4
		and DWORD ptr [esp],7FF00000h
		xor DWORD ptr [esp],7FF00000h
		lea esp,[esp+4]
	ELSE
		push rax
		mov eax,DWORD ptr mem64+4
		and eax,7FF00000h
		xor eax,7FF00000h
		pop rax
	ENDIF
endm
endif

    TypeOf MACRO arg
      EXITM % opattr(arg)
    ENDM

	TrimString macro string___
		LOCAL ?Ltmp_string___,Ltmp_string___,Ltmp_Char,L_TabPos,L_SpcPos
		?Ltmp_string___  	TEXTEQU <string___>
		Ltmp_string___ 		TEXTEQU <>
	%	FORC Ltmp_Char,<?Ltmp_string___>
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




	__IfLogicalOperationNON	=	0
	__IfLogicalOperationAND	=	1
	__IfLogicalOperationOR	=	2
    Ty_Variable_GLOBAL 	EQU < 42 >
    Ty_Variable_LOCAL 	EQU < 98 >
    Ty_Register 		EQU < 48 >
    Ty_Constant		EQU < 36 >
	

	GetCloseOfbracket macro ConditionsString
		LOCAL n_bracket,StringCondLen,Ltmp_Char,L_open_br_Pos,L_close_br_Pos
		n_bracket = 0
		StringCondLen=0
	%	FORC Ltmp_Char,<ConditionsString>
			StringCondLen=StringCondLen+1
			L_open_br_Pos  INSTR 1,<(>,<Ltmp_Char>
			L_close_br_Pos INSTR 1,<)>,<Ltmp_Char>
			IF L_open_br_Pos
				n_bracket = n_bracket + 1
			ENDIF
			IF L_close_br_Pos
				n_bracket = n_bracket - 1
				IF n_bracket eq 0
					exitm
				ENDIF
			ENDIF
			IF n_bracket lt 0
				.ERR <Syntax Error.>
				exitm<>
			ENDIF
		ENDM
		IF n_bracket gt 0
			.ERR <Syntax Error.>
			exitm<>
		ENDIF
		exitm<@CatStr(%StringCondLen)> 
	endm

	GetLogicalOperation macro ConditionsString
		LOCAL ConditionsStringln 

		__CurrentIfLogicalOperation = __IfLogicalOperationNON
		__CurrentIfLogicalPos       = 0

	       Cond_OR textequ ObliterateOperation(||)
	       Cond_AND textequ ObliterateOperation(&&)
              Cond_extra equ 6


		ConditionsStringln SIZESTR <ConditionsString>
		IF ConditionsStringln lt 2
			exitm
		ENDIF
              IF ConditionsStringln ge 8
                	L_IfLogicalOperationOR  INSTR 1,<ConditionsString>, Cond_OR
        		L_IfLogicalOperationAND INSTR 1,<ConditionsString>, Cond_AND
        		First_Char  			SUBSTR <ConditionsString>,1,2+Cond_extra
               	IFIDN First_Char, Cond_OR
        			__CurrentIfLogicalOperation = __IfLogicalOperationOR
        			__CurrentIfLogicalPos		= 0
        			exitm
        		ENDIF
        		IFIDN First_Char, Cond_AND
        			__CurrentIfLogicalOperation = __IfLogicalOperationAND
        			__CurrentIfLogicalPos		= 0
        			exitm
        		ENDIF
        		IF L_IfLogicalOperationOR
        			IF L_IfLogicalOperationAND
        				IF L_IfLogicalOperationAND lt L_IfLogicalOperationOR
        					__CurrentIfLogicalOperation = __IfLogicalOperationAND
        					__CurrentIfLogicalPos		= L_IfLogicalOperationAND
        				ELSE
        					__CurrentIfLogicalOperation = __IfLogicalOperationOR
        					__CurrentIfLogicalPos		= L_IfLogicalOperationOR
        				ENDIF
        			ELSE 
        				__CurrentIfLogicalOperation = __IfLogicalOperationOR
        				__CurrentIfLogicalPos		= L_IfLogicalOperationOR
        			ENDIF
        			EXITM
        		ELSEIF L_IfLogicalOperationAND
        			__CurrentIfLogicalOperation = __IfLogicalOperationAND
        			__CurrentIfLogicalPos		= L_IfLogicalOperationAND
        		ENDIF
              ENDIF
	endm

	GetComparOperation macro ConditionsStr:req,ComOP,CmpStrln,CmpLn
	LOCAL L_ComparDis,L_ComparSrc
		L_ComparDis				TEXTEQU  <@SubStr(<ConditionsStr>,1,ComOP-1)>  
		L_ComparSrc				TEXTEQU  <@SubStr(<ConditionsStr>,ComOP+CmpLn+Obliterateln,CmpStrln-ComOP-CmpLn-Obliterateln+1)> 
		CMP L_ComparDis,L_ComparSrc;%echo
	endm

	GetfComparOperation macro ConditionsStr:req,ComOP,CmpStrln,CmpLn
	LOCAL L_ComparDis,L_ComparSrc, L_op
		L_ComparDis				TEXTEQU  <@SubStr(<ConditionsStr>,1,ComOP-1)>  
		L_ComparSrc				TEXTEQU  <@SubStr(<ConditionsStr>,ComOP+CmpLn+Obliterateln,CmpStrln-ComOP-CmpLn-Obliterateln+1)>
		L_op					TEXTEQU  <@SubStr(<ConditionsStr>,ComOP+3,CmpLn)>
		@CatStr(<cmp >, L_op,<(>,L_ComparDis,<,>,L_ComparSrc,<), 0>)
	endm

	GetfCheckOperation macro ConditionsStr:req,ComOP,CmpStrln,CmpLn
	LOCAL L_ComparDis,L_ComparSrc, L_op
		L_ComparDis				TEXTEQU  <@SubStr(<ConditionsStr>,1,ComOP-1)>  
		L_ComparSrc				TEXTEQU  <@SubStr(<ConditionsStr>,ComOP+CmpLn+Obliterateln,CmpStrln-ComOP-CmpLn-Obliterateln+1)>
		L_op					TEXTEQU  <@SubStr(<ConditionsStr>,ComOP+3,CmpLn)>
		@CatStr(%L_op,<_>,%L_ComparSrc,<(>,L_ComparDis,<)>)
	endm

	PutComparOperation macro ConditionsStr:req,InvComp:req,Lable:req
	LOCAL L_ComparOperation,ConditionsStrln 
		ConditionsStrln 		SIZESTR <ConditionsStr>

    

		L_ComparOperation  	INSTR 1,<ConditionsStr>, ObliterateOperation(<fGT>)
		IF L_ComparOperation
			GetfComparOperation ConditionsStr,L_ComparOperation,ConditionsStrln,3
				IF InvComp
					JE	Lable		
				ELSE
					JNE	Lable		
				ENDIF
				exitm
			exitm
		ENDIF
		L_ComparOperation  	INSTR 1,<ConditionsStr>,ObliterateOperation(<fGE>)
		IF L_ComparOperation
			GetfComparOperation ConditionsStr,L_ComparOperation,ConditionsStrln,3
				IF InvComp
					JE	Lable		
				ELSE
					JNE	Lable		
				ENDIF
				exitm
			exitm
		ENDIF

		L_ComparOperation  	INSTR 1,<ConditionsStr>,ObliterateOperation(<fEQ>)
		IF L_ComparOperation
			GetfComparOperation ConditionsStr,L_ComparOperation,ConditionsStrln,3
				IF InvComp
					JE	Lable		
				ELSE
					JNE	Lable		
				ENDIF
				exitm
			exitm
		ENDIF
		L_ComparOperation  	INSTR 1,<ConditionsStr>,ObliterateOperation(<fNE>)
		IF L_ComparOperation
			GetfComparOperation ConditionsStr,L_ComparOperation,ConditionsStrln,3
				IF InvComp
					JE	Lable		
				ELSE
					JNE	Lable		
				ENDIF
				exitm
			exitm
		ENDIF
		L_ComparOperation  	INSTR 1,<ConditionsStr>,ObliterateOperation(<fLT>)
		IF L_ComparOperation
			GetfComparOperation ConditionsStr,L_ComparOperation,ConditionsStrln,3
				IF InvComp
					JE	Lable		
				ELSE
					JNE	Lable		
				ENDIF
				exitm
			exitm
		ENDIF
		L_ComparOperation  	INSTR 1,<ConditionsStr>,ObliterateOperation(<fLE>)
		IF L_ComparOperation
			GetfComparOperation ConditionsStr,L_ComparOperation,ConditionsStrln,3
				IF InvComp
					JE	Lable		
				ELSE
					JNE	Lable		        
				ENDIF
				exitm
			exitm
		ENDIF
	       L_ComparOperation  	INSTR 1,<ConditionsStr>,ObliterateOperation(r8Is)
		IF L_ComparOperation
			GetfCheckOperation ConditionsStr,L_ComparOperation,ConditionsStrln,4
				IF InvComp
					JE	Lable		
				ELSE
					JNE	Lable		
				ENDIF
				exitm
			exitm
		ENDIF

  	       L_ComparOperation  	INSTR 1,<ConditionsStr>,ObliterateOperation(r4Is)
		IF L_ComparOperation
			GetfCheckOperation ConditionsStr,L_ComparOperation,ConditionsStrln,4
				IF InvComp
					JE	Lable		
				ELSE
					JNE	Lable		
				ENDIF
				exitm
			exitm
		ENDIF


		L_ComparOperation 	INSTR 1,<ConditionsStr>,ObliterateOperation(eq)
		IF L_ComparOperation 
			GetComparOperation ConditionsStr,L_ComparOperation,ConditionsStrln,2
			IF InvComp
				JNE	Lable		
			ELSE
				JE	Lable		
			ENDIF
			exitm
		ENDIF
		L_ComparOperation  	INSTR 1,<ConditionsStr>,ObliterateOperation(ne)
		IF L_ComparOperation 
			GetComparOperation ConditionsStr,L_ComparOperation,ConditionsStrln,2
			IF InvComp
				JE	Lable		
			ELSE
				JNE	Lable		
			ENDIF
			exitm
		ENDIF

		L_ComparOperation  	INSTR 1,<ConditionsStr>,ObliterateOperation(le)
		IF L_ComparOperation 
			GetComparOperation ConditionsStr,L_ComparOperation,ConditionsStrln,2
			IF InvComp
				JNLE	Lable		
			ELSE
				JLE	Lable		
			ENDIF
			exitm
		ENDIF
		L_ComparOperation  	INSTR 1,<ConditionsStr>,ObliterateOperation(be)
		IF L_ComparOperation 
			GetComparOperation ConditionsStr,L_ComparOperation,ConditionsStrln,2
			IF InvComp
				JNBE	Lable		
			ELSE
				JBE	Lable		
			ENDIF
			exitm
		ENDIF


		L_ComparOperation  	INSTR 1,<ConditionsStr>,ObliterateOperation(lt)
		IF L_ComparOperation 
			GetComparOperation ConditionsStr,L_ComparOperation,ConditionsStrln,2
			IF InvComp
				JNL	Lable		
			ELSE
				JL	Lable		
			ENDIF
			exitm
		ENDIF
		L_ComparOperation  	INSTR 1,<ConditionsStr>,ObliterateOperation(ub)
		IF L_ComparOperation 
			GetComparOperation ConditionsStr,L_ComparOperation,ConditionsStrln,2
			IF InvComp
				JNB	Lable		
			ELSE
				JB	Lable		
			ENDIF
			exitm
		ENDIF


		L_ComparOperation  	INSTR 1,<ConditionsStr>,ObliterateOperation(ge)
		IF L_ComparOperation 
			GetComparOperation ConditionsStr,L_ComparOperation,ConditionsStrln,2
			IF InvComp
				JNGE	Lable		
			ELSE
				JGE	Lable		
			ENDIF
			exitm
		ENDIF
		L_ComparOperation  	INSTR 1,<ConditionsStr>,ObliterateOperation(ae)
		IF L_ComparOperation 
			GetComparOperation ConditionsStr,L_ComparOperation,ConditionsStrln,2
			IF InvComp
				JNAE	Lable		
			ELSE
				JAE	Lable		
			ENDIF
			exitm
		ENDIF

        	L_ComparOperation  	INSTR 1,<ConditionsStr>,ObliterateOperation(gt)
		IF L_ComparOperation 
			GetComparOperation ConditionsStr,L_ComparOperation,ConditionsStrln,2
			IF InvComp
				JNG	Lable		
			ELSE
				JG	Lable		
			ENDIF
			exitm
		ENDIF


		L_ComparOperation  	INSTR 1,<ConditionsStr>,ObliterateOperation(ua)
		IF L_ComparOperation 
			GetComparOperation ConditionsStr,L_ComparOperation,ConditionsStrln,2
			IF InvComp
				JNA	Lable		
			ELSE
				JA	Lable		
			ENDIF
			exitm
		ENDIF
		
		IF ConditionsStrln EQ 5
			IFIDN <ConditionsStr>,<SIGN?>
				IF InvComp
					JNS	Lable		
				ELSE
					JS	Lable		
				ENDIF
				exitm
			ENDIF
			IFIDN <ConditionsStr>,<ZERO?>
				IF InvComp
					JNE	Lable		
				ELSE
					JE	Lable		
				ENDIF
				exitm
			ENDIF
		ENDIF
		IF ConditionsStrln EQ 6
			IFIDN <ConditionsStr>,<~SIGN?>
				IF InvComp
					JS	Lable		
				ELSE
					JNS	Lable		
				ENDIF
				exitm
			ENDIF
			IFIDN <ConditionsStr>,<~ZERO?>
				IF InvComp
					JE	Lable		
				ELSE
					JNE	Lable		
				ENDIF
				exitm
			ENDIF
			IFIDN <ConditionsStr>,<CARRY?>
				IF InvComp
					JAE	Lable		
				ELSE
					JNAE	Lable		
				ENDIF
				exitm
			ENDIF
		ENDIF
		
		IF ConditionsStrln EQ 7
			IFIDN <ConditionsStr>,<~CARRY?>
				IF InvComp
					JNAE	Lable		
				ELSE
					JAE	Lable		
				ENDIF
				exitm
			ENDIF
			IFIDN <ConditionsStr>,<PARITY?>
				IF InvComp
					JNP	Lable		
				ELSE
					JP	Lable		
				ENDIF
				exitm
			ENDIF
		ENDIF
		IF ConditionsStrln EQ 8
			IFIDN <ConditionsStr>,<~PARITY?>
				IF InvComp
					JP	Lable		
				ELSE
					JNP	Lable		
				ENDIF
				exitm
			ENDIF
		ENDIF
		IF ConditionsStrln EQ 9
			IFIDN <ConditionsStr>,<OVERFLOW?>
				IF InvComp
					JO	Lable		
				ELSE
					JNO	Lable		
				ENDIF
				exitm
			ENDIF
		ENDIF
		IF ConditionsStrln EQ 10
			IFIDN <ConditionsStr>,<~OVERFLOW?>
				IF InvComp
					JNO	Lable		
				ELSE
					JO	Lable		
				ENDIF
				exitm
			ENDIF
		ENDIF
		
		NotSymbole SUBSTR  <ConditionsStr>,1,1
		IFIDN NotSymbole,<~>
				CMP @SubStr(<ConditionsStr>,2,ConditionsStrln-1),0
			IF InvComp
				JNE	Lable		
			ELSE
				JE	Lable		
			ENDIF
			exitm
		ENDIF
		
		ConditionsStrln SIZESTR <ConditionsStr>

		TypeOf_ConditionsStr = TypeOf(ConditionsStr)
		IF 		TypeOf_ConditionsStr eq Ty_Register
				TEST ConditionsStr,ConditionsStr
			IF InvComp
				JE	Lable		
			ELSE
				JNZ	Lable		
			ENDIF
			exitm
		ENDIF

		IF 		(TypeOf_ConditionsStr eq Ty_Variable_GLOBAL	) \
		    or 	(TypeOf_ConditionsStr eq Ty_Variable_LOCAL	)
				CMP ConditionsStr,0
			IF InvComp
				JE	Lable		
			ELSE
				JNE	Lable		
			ENDIF
			exitm
		ENDIF
		IF 		TypeOf_ConditionsStr eq Ty_Constant	
			%echo	IIIIIIIMMMMMMMMMMMM_Operation:	 ConditionsStr
			exitm
		ENDIF
		.err <in Syntax>
		
	endm

;-----------------------------------------------------------------------------------------
	Extract_If_Conditions macro ConditionsStr:req,INVOperatio:req,LableJMP:req,INVLableJMP:req
		LOCAL bracketPos,bracketChar,L__CurrentIfLogicalOperation,L__CurrentIndexLbl,L__CurrentIfLogicalPos,   ConditionsStrln    
		LOCAL nnextind,L_FirstConditionsStr,L_SecondConditionsStr,L_FirstLable,L_SecondLable,L_NotINVOperatio

		ConditionsStrln SIZESTR <ConditionsStr>
		bracketChar  	SUBSTR  <ConditionsStr>,1,1
		nnextind = @CatStr(<__?if_lable_>,%__?CurrentIF)  
		IFIDN bracketChar,<(>
		 	bracketPos = GetCloseOfbracket(ConditionsStr)
			IF bracketPos eq ConditionsStrln
			%	Extract_If_Conditions @SubStr(<ConditionsStr>,2,ConditionsStrln-2) ,INVOperatio,LableJMP,INVLableJMP
				exitm
			ELSE
				GetLogicalOperation @SubStr(<ConditionsStr>,bracketPos+1,ConditionsStrln-bracketPos)
				L__CurrentIfLogicalOperation 	= 	__CurrentIfLogicalOperation
				L__CurrentIfLogicalPos			=	bracketPos+1
			ENDIF
		ENDIF
		IFDIF bracketChar,<(>
			GetLogicalOperation ConditionsStr
			L__CurrentIfLogicalOperation 	= __CurrentIfLogicalOperation
			L__CurrentIfLogicalPos			= __CurrentIfLogicalPos
		ENDIF
		;
		IF L__CurrentIfLogicalOperation ne __IfLogicalOperationNON
			L__CurrentIndexLbl=__CurrentIfIndexLable
			__CurrentIfIndexLable=__CurrentIfIndexLable+2
		%	L_FirstConditionsStr	EQU <@SubStr(<ConditionsStr>,1,L__CurrentIfLogicalPos-1)>
		%	L_SecondConditionsStr	EQU <@SubStr(<ConditionsStr>,L__CurrentIfLogicalPos+2+6,ConditionsStrln-L__CurrentIfLogicalPos-1-6)>
		%	L_FirstLable			EQU <@CatStr(<__?if_Next_lable_>,%__?CurrentIF,<_>,%nnextind,<_>,%L__CurrentIndexLbl+1)>
		%	L_SecondLable			EQU <@CatStr(<__?if_Next_lable_>,%__?CurrentIF,<_>,%nnextind,<_>,%L__CurrentIndexLbl+2)>

			IF INVOperatio
				L_NotINVOperatio=FALSE
			ELSE
				L_NotINVOperatio=TRUE
			ENDIF
			IF L__CurrentIfLogicalOperation eq __IfLogicalOperationAND
		;%echo ...1: [L_FirstConditionsStr,TRUE,L_FirstLable,INVLableJMP]
		%		Extract_If_Conditions L_FirstConditionsStr,TRUE,L_FirstLable,INVLableJMP;L_NotINVOperatio
			ELSE
		;%echo ...2: [L_FirstConditionsStr,FALSE,LableJMP,L_FirstLable]
		%		Extract_If_Conditions L_FirstConditionsStr,FALSE,LableJMP,L_FirstLable;INVOperatio;L_SecondLable
			ENDIF
			L_FirstLable:
	;%echo	L_FirstLable:
			IF L__CurrentIfLogicalOperation eq __IfLogicalOperationAND
	;	%		Extract_If_Conditions L_SecondConditionsStr,TRUE,L_SecondLable,INVLableJMP;L_NotINVOperatio
				IF	INVOperatio
		;%echo ...3: [L_SecondConditionsStr,TRUE,L_SecondLable,INVLableJMP]
		%			Extract_If_Conditions L_SecondConditionsStr,TRUE,L_SecondLable,INVLableJMP;L_NotINVOperatio
				ELSE
		;%echo ...4: [L_SecondConditionsStr,FALSE,LableJMP,L_FirstLable]
		%			Extract_If_Conditions L_SecondConditionsStr,FALSE,LableJMP,INVLableJMP;L_FirstLable;L_NotINVOperatio
				ENDIF
			ELSE
	;	%		Extract_If_Conditions L_SecondConditionsStr,TRUE,L_SecondLable,INVLableJMP;L_NotINVOperatio
	;false
				IF	INVOperatio
		;%echo ...5: [L_SecondConditionsStr,TRUE,L_SecondLable,INVLableJMP]
		%			Extract_If_Conditions L_SecondConditionsStr,TRUE,L_SecondLable,INVLableJMP;L_NotINVOperatio
				ELSE
		;%echo ...6: [L_SecondConditionsStr,FALSE,LableJMP,L_FirstLable]
		%			Extract_If_Conditions L_SecondConditionsStr,FALSE,LableJMP,INVLableJMP;L_NotINVOperatio
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
   
	Put_If_Conditions macro ConditionsString:req
		LOCAL nnextind,nnextindj
		__CurrentIfIndexLable       = 0
		nnextind = @CatStr(<__?if_lable_>,%__?CurrentIF)  
	%	Extract_If_Conditions TrimString(ConditionsString),TRUE,@CatStr(<__?if_Next_lable_>,%__?CurrentIF,<_>,%nnextind,<_0>),@CatStr(<__?if_Next_lable_>,%__?CurrentIF,<_>,%nnextind+1) 
		@CatStr(<__?if_Next_lable_>,%__?CurrentIF,<_>,%nnextind,<_0>):
	endm
;························································································
;························································································
	@if macro ___CondArg
		LOCAL nnextind
		ifndef __?ifCountGlob
			__?ifCountGlob	=	0
			__?ifCountCurr	=	0
		endif
		__?ifCountGlob	=	__?ifCountGlob + 	1
		__?ifCountCurr	=	__?ifCountCurr + 	1
		IF __?ifCountCurr eq 1
			@CatStr(<__?IF_Last_>,%__?ifCountGlob)	= 0
		ELSE
			@CatStr(<__?IF_Last_>,%__?ifCountGlob)	= __?CurrentIF
		ENDIF
		
		__?CurrentIF = __?ifCountGlob 
		@CatStr(<__?if_lable_>,%__?CurrentIF)=0
		Put_If_Conditions ___CondArg
	endm
	@elseif macro ___CondArg
		LOCAL nnextind,nnextindj
		@CatStr(<__?if_lable_>,%__?CurrentIF)=	@CatStr(<__?if_lable_>,%__?CurrentIF) + 1
		nnextind = @CatStr(<__?if_lable_>,%__?CurrentIF)
		nnextindj=nnextind+1

		JMP @CatStr(<__?if_End_lable_>,%__?CurrentIF);@CatStr(<__?if_Next_lable_>,%__?CurrentIF,<_>,%nnextindj)
		@CatStr(<__?if_Next_lable_>,%__?CurrentIF,<_>,%nnextind):
		Put_If_Conditions ___CondArg
	endm
	@else macro
		LOCAL nnextind,nnextindj
		@CatStr(<__?if_lable_>,%__?CurrentIF)=	@CatStr(<__?if_lable_>,%__?CurrentIF) + 1
		nnextind = @CatStr(<__?if_lable_>,%__?CurrentIF)
		nnextindj=nnextind+1

		JMP @CatStr(<__?if_End_lable_>,%__?CurrentIF);@CatStr(<__?if_Next_lable_>,%__?CurrentIF,<_>,%nnextindj)
		@CatStr(<__?if_Next_lable_>,%__?CurrentIF,<_>,%nnextind):
	endm
	@endif macro
		LOCAL nnextind
		@CatStr(<__?if_lable_>,%__?CurrentIF)=	@CatStr(<__?if_lable_>,%__?CurrentIF) + 1
		nnextind = @CatStr(<__?if_lable_>,%__?CurrentIF)

		@CatStr(<__?if_Next_lable_>,%__?CurrentIF,<_>,%nnextind):
		@CatStr(<__?if_End_lable_>,%__?CurrentIF):
				__?ifCountCurr	=	__?ifCountCurr - 	1
		__?CurrentIF	=@CatStr(<__?IF_Last_>,%__?CurrentIF)

	endm
;························································································
