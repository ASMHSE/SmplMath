;/********************************************************
; * This file is part of the SmplMath macros-system.     *
; *                                                      *
; *          Copyright by qWord, 2011-2014               *
; *                                                      *
; *          SmplMath.Masm{at}gmx{dot}net                *
; *    http://sourceforge.net/projects/smplmath/         *
; *                                                      *
; *  Further Copyright and Legal statements are located  *
; *  in the documentation (Documentation.pdf).           *
; *                                                      *
; ********************************************************/

include strings.inc

;/********************************************************************/
;/*                     fn - macro procedure                         */
;/* This macro enhanced the INVOKE-directive:                        */
;/*   - It adds support for quoted ASCII or unicode strings.         */
;/*     The strings can be either enclosed by double quotes or by    */
;/*     single quotation marks.                                      */
;/*     The kind of created string (Unicode or ASCII) depends on the */
;/*     __UNICODE__ or UNICODE-equate. If one of these equates is    */
;/*     defined, unicode string are created.                         */
;/*     However, Unicode strings can be forced by using              */
;/*     the 'L'-prefix:                                              */
;/*                 L"my string" or L'my string'                     */
;/*     ASCII strings can be forced by using the A-prefix:           */
;/*                 A"my string" or A'my string'                     */
;/*     MASM's reserved characters like exclamation marks, angel     */
;/*     brackets and single brackets [,...] can not be used.         */
;/*     (use fnc for escape sequences support)                       */
;/*   - ADDR-expressions can be shorten by using a ampersand '&':    */
;/*         fn MessageBoxW,0,&wsz[0],L'xyz',0                        */
;/*   - a optional destination can be specified in front of the      */
;/*     function:                                                    */
;/*         fn dest=FncName,...                                      */
;/*   - expression evaluation is possible, if                        */
;/*     the SmplMath-macros are used.                                */
;/*     A leading colon (':') or equal sign ('=') indicates an       */
;/*     expression that should be solved by the SmplMath macros.     */
;/*     If a colon is used, the expression's result is retuned as an */
;/*     integer (SDWORD). For an equal sign a REAL4 is retuned.      */
;/*     For both, colon and equal sign, the return type can be       */
;/*     overwritten by using the following prefixes: r4,r8,i4,i8     */
;/*     Examples:                                                    */
;/*             fn ... , : a+b*2 ,...    ; a SDWORD is passed        */
;/*                    , = a+b*3 ,...    ; " REAL4  "    "           */
;/*                    , r8= a+b ,...    ; " REAL8  "    "           */
;/*                    , r8: a+b ,...    ; " REAL8  "    "           */
;/*                    , i4=x^2  ,...    ; " SDWORD "    "           */
;/*     According to the expression's return type, the macros        */
;/*     @fSlv4/8/I/64() are used.                                    */
;/*                                                                  */
;/* Example:                                                         */
;/*     fn MessageBoxW,0,L"my string",&wsz[0],0                      */
;/*     fn hWnd = CreateWindowEx,...                                 */
;/*                                                     qWord, 2012  */
;/********************************************************************/
fn MACRO FncName:req,args:VARARG

    ;/* check if a optional destination is specified */
    fnex_flag = 0
    IF @InStr(1,<&FncName>,<=>)
        fnex_flag = 1
        fnex_dest SUBSTR <&FncName>,1,@InStr(1,<&FncName>,<=>)-1
        fnex_arg TEXTEQU <invoke >,@SubStr(<&FncName>,@InStr(1,<&FncName>,<=>)+1)
    ELSE    
        fnex_arg TEXTEQU <invoke FncName>
    ENDIF

    ;/* process argument list and append it to the invoke-call */
    fnex_arg CATSTR fnex_arg,repargs(0,&args)

    ;/* place the function call */
    fnex_arg

    ;/* if available, fill the specified destination */    
    IF fnex_flag EQ 1
    	IF @WordSize EQ 4
        	mov fnex_dest,eax
        ELSE
        	mov fnex_dest,rax
        ENDIF
    ENDIF
endm


;/********************************************************************/
;/*                     fnc - macro procedure                        */
;/* This macro behave like the fn-macro, except, that it adds        */
;/* support for escape sequences:                                    */
;/*     \\  ->  "\"                                                  */
;/*     \t  ->  tab                                                  */
;/*     \n  ->  new line (13,10)                                     */
;/*     \x  ->  "!"                                                  */
;/*     \a  ->  "("                                                  */
;/*     \b  ->  ")"                                                  */
;/*     \l  ->  "<"                                                  */
;/*     \r  ->  ">"                                                  */
;/*     \p  ->  "%"                                                  */
;/*     \A  ->  "&"                                                  */
;/*     \q  ->  double quote '"'                                     */
;/*     \0  ->  zero                                                 */
;/* Example:                                                         */
;/*     fnc MessageBox,0,"my string\n",&wsz[0],0                     */
;/*                                                     qWord, 2012  */
;/********************************************************************/
fnc MACRO FncName:req,args:VARARG

    ;/* check if a optional destination is specified */
    fnex_flag = 0
    IF @InStr(1,<&FncName>,<=>)
        fnex_flag = 1
        fnex_dest SUBSTR <&FncName>,1,@InStr(1,<&FncName>,<=>)-1
        fnex_arg TEXTEQU <invoke >,@SubStr(<&FncName>,@InStr(1,<&FncName>,<=>)+1)
    ELSE    
        fnex_arg TEXTEQU <invoke FncName>
    ENDIF

    ;/* process argument list and append it to the invoke-call */
    fnex_arg CATSTR fnex_arg,repargs(1,&args)

    ;/* place the function call */
    fnex_arg

    ;/* if available, fill the specified destination */    
    IF fnex_flag EQ 1
    	IF @WordSize EQ 4
        	mov fnex_dest,eax
        ELSE
        	mov fnex_dest,rax
        ENDIF
    ENDIF
endm

;/********************************************************************/
;/*                     rv - macro function                          */
;/* This macro is the function-version of the fn-macro.              */
;/*                                                                  */
;/* Example:                                                         */
;/*      mov edi,rv(myFunction,L"my string",&wsz[0],...)             */
;/*      .break .if !rv(dest=myFunction,...)                         */
;/*                                                     qWord, 2012  */
;/********************************************************************/
rv MACRO FncName:req,args:VARARG
    
    ;/* check if a optional destination is specified */
    rvex_flag = 0
    IF @InStr(1,<&FncName>,<=>)
        rvex_flag = 1
        rvex_dest SUBSTR <&FncName>,1,@InStr(1,<&FncName>,<=>)-1
        rvex_arg TEXTEQU <invoke >,@SubStr(<&FncName>,@InStr(1,<&FncName>,<=>)+1)
    ELSE    
        rvex_arg TEXTEQU <invoke FncName>
    ENDIF

    ;/* process argument list and append it to the invoke-call */
    rvex_arg CATSTR rvex_arg,repargs(0,&args)
    
    ;/* place the function call */
    rvex_arg
    
    ;/* if available, fill the specified destination */
    IF rvex_flag EQ 1
    	IF @WordSize EQ 4
        	mov rvex_dest,eax
        ELSE
        	mov rvex_dest,rax
        ENDIF
        EXITM rvex_dest
    ENDIF
    IF @WordSize EQ 4
    	EXITM <eax>
    ELSE
    	EXITM <rax>
    ENDIF
endm

;/********************************************************************/
;/*                     rvc - macro function                         */
;/* This macro behave like the rv-macros, except, that it adds       */
;/* support for escape sequences:                                    */
;/*     \\  ->  "\"                                                  */
;/*     \t  ->  tab                                                  */
;/*     \n  ->  new line (13,10)                                     */
;/*     \x  ->  "!"                                                  */
;/*     \a  ->  "("                                                  */
;/*     \b  ->  ")"                                                  */
;/*     \l  ->  "<"                                                  */
;/*     \r  ->  ">"                                                  */
;/*     \p  ->  "%"                                                  */
;/*     \A  ->  "&"                                                  */
;/*     \q  ->  double quote '"'                                     */
;/*     \0  ->  zero                                                 */
;/* Example:                                                         */
;/*     mov edi,rvc(myFunction,L"my string\x\n",&wsz[0],...)         */
;/*                                                     qWord, 2012  */
;/********************************************************************/
rvc MACRO FncName:req,args:VARARG
    
    ;/* check if a optional destination is specified */
    rvex_flag = 0
    IF @InStr(1,<&FncName>,<=>)
        rvex_flag = 1
        rvex_dest SUBSTR <&FncName>,1,@InStr(1,<&FncName>,<=>)-1
        rvex_arg TEXTEQU <invoke >,@SubStr(<&FncName>,@InStr(1,<&FncName>,<=>)+1)
    ELSE    
        rvex_arg TEXTEQU <invoke FncName>
    ENDIF

    ;/* process argument list and append it to the invoke-call */
    rvex_arg CATSTR rvex_arg,repargs(1,&args)
    
    ;/* place the function call */
    rvex_arg
    
    ;/* if available, fill the specified destination */
    IF rvex_flag EQ 1
    	IF @WordSize EQ 4
        	mov rvex_dest,eax
        ELSE
        	mov rvex_dest,rax
        ENDIF
        EXITM rvex_dest
    ENDIF
    IF @WordSize EQ 4
    	EXITM <eax>
    ELSE
    	EXITM <rax>
    ENDIF
endm

IF @WordSize EQ 8
	rv32 MACRO FncName:req,args:VARARG
	    
	    ;/* check if a optional destination is specified */
	    rvex_flag = 0
	    IF @InStr(1,<&FncName>,<=>)
	        rvex_flag = 1
	        rvex_dest SUBSTR <&FncName>,1,@InStr(1,<&FncName>,<=>)-1
	        rvex_arg TEXTEQU <invoke >,@SubStr(<&FncName>,@InStr(1,<&FncName>,<=>)+1)
	    ELSE    
	        rvex_arg TEXTEQU <invoke FncName>
	    ENDIF
	
	    ;/* process argument list and append it to the invoke-call */
	    rvex_arg CATSTR rvex_arg,repargs(0,&args)
	    
	    ;/* place the function call */
	    rvex_arg
	    
	    ;/* if available, fill the specified destination */
	    IF rvex_flag EQ 1
			mov rvex_dest,eax   
	    ENDIF
		EXITM <eax>
	endm
	
	rvc32 MACRO FncName:req,args:VARARG
	    
	    ;/* check if a optional destination is specified */
	    rvex_flag = 0
	    IF @InStr(1,<&FncName>,<=>)
	        rvex_flag = 1
	        rvex_dest SUBSTR <&FncName>,1,@InStr(1,<&FncName>,<=>)-1
	        rvex_arg TEXTEQU <invoke >,@SubStr(<&FncName>,@InStr(1,<&FncName>,<=>)+1)
	    ELSE    
	        rvex_arg TEXTEQU <invoke FncName>
	    ENDIF
	
	    ;/* process argument list and append it to the invoke-call */
	    rvex_arg CATSTR rvex_arg,repargs(1,&args)
	    
	    ;/* place the function call */
	    rvex_arg
	    
	    ;/* if available, fill the specified destination */
	    IF rvex_flag EQ 1
        	mov rvex_dest,eax
	    ENDIF
    	EXITM <eax>
	endm
ELSE
	rv32 EQU rv
	rvc32 EQU rvc
ENDIF


; internal macro
repargs MACRO cstr:=<0>,args:VARARG

    ;/* initialize global counter which */
    ;/* is used for label-creation.     */
    IFNDEF repargs_glb_cntr
        repargs_glb_cntr = 0
    ENDIF

    repargs_unicode = 0
    IFDEF __UNICODE__		; used by teh MASM32-package v11+
        repargs_unicode = 1
    ENDIF
    IFDEF UNICODE			; WinInc
    	repargs_unicode = 1
    ENDIF
    
    ;/* count arguments (needed for dereferencing operator) */
    repargs_nargs = 0
    FOR arg,<&args>
        repargs_nargs = repargs_nargs + 1
    ENDM
    repargs_eax_saved = 0

    repargs_cntr = 0
    repargs_args TEXTEQU <>
    FOR arg,<&args>
        repargs_txt  TEXTEQU <&arg>
       
        ;/* analyze current argument */
        repargs_pos1 INSTR 1,<&arg>,<">
        repargs_pos2 INSTR 1,<&arg>,<'>
        repargs_pos3 INSTR 1,<&arg>,<L">
        repargs_pos4 INSTR 1,<&arg>,<L'>
        repargs_pos5 INSTR 1,<&arg>,<A">
        repargs_pos6 INSTR 1,<&arg>,<A'>
        repargs_pos7 INSTR 1,<&arg>,<!&>
        repargs_pos8 INSTR 1,<&arg>,<*>
        repargs_pos9 INSTR 1,<&arg>,<=>
        repargs_posA INSTR 1,<&arg>,<:>

        IF repargs_pos1 EQ 1 OR repargs_pos2 EQ 1
            repargs_str_type = 1                       ; ASCII
            IF repargs_unicode
                repargs_str_type = 2                   ; Unicode
            ENDIF
        ELSEIF repargs_pos3 EQ 1 OR repargs_pos4 EQ 1
            repargs_str_type = 2                       ; Unicode
            repargs_txt SUBSTR repargs_txt,2           ; remove prefix
        ELSEIF repargs_pos5 EQ 1 OR repargs_pos6 EQ 1
            repargs_str_type = 1                       ; ASCII
            repargs_txt SUBSTR repargs_txt,2           ; remove prefix
        ELSE
            repargs_str_type = 0                       ; no string
        ENDIF
        
        IF repargs_str_type EQ 1
            ;/ create ASCII string */
            .data
                IF @SizeStr(<%repargs_txt>) GT 2
                    IFE cstr
                        @CatStr(<$$_szlbl_>,%repargs_glb_cntr) db repargs_txt,0
                    ELSE
                        ?cstr? @CatStr(<$$_szlbl_>,%repargs_glb_cntr),<%repargs_txt>,0
                    ENDIF
                ELSE
                    @CatStr(<$$_szlbl_>,%repargs_glb_cntr) db 0
                ENDIF               
            .code
            repargs_args TEXTEQU repargs_args,<,OFFSET $$_szlbl_>,%repargs_glb_cntr
            repargs_glb_cntr = repargs_glb_cntr + 1
        ELSEIF repargs_str_type EQ 2
            ;/* create Unicode string */
            .data
                IF @SizeStr(<%repargs_txt>) GT 2
                    IFE cstr
                        WSTR @CatStr(<$$_wszlbl_>,%repargs_glb_cntr),<%repargs_txt>,0
                    ELSE
                        WCSTR @CatStr(<$$_wszlbl_>,%repargs_glb_cntr),<%repargs_txt>,0
                    ENDIF
                ELSE
                    @CatStr(<$$_wszlbl_>,%repargs_glb_cntr) dw 0
                ENDIF
            .code
            repargs_args TEXTEQU repargs_args,<,OFFSET $$_wszlbl_>,%repargs_glb_cntr
            repargs_glb_cntr = repargs_glb_cntr + 1
        ELSEIF repargs_pos7 EQ 1
            ;/* replace '&' with <ADDR > and append argument to list */
            repargs_args TEXTEQU repargs_args,<,>,<ADDR >,@SubStr(%repargs_txt,2,)
        ELSEIF repargs_pos8 EQ 1 AND @WordSize NE 4
        	.err <dereferencing not supported in current CPU mode>
        	repargs_args TEXTEQU repargs_args,<,>,<0>
        ELSEIF repargs_pos8 EQ 1
            ;/* dereference operator '*' */

            repargs_txt SUBSTR repargs_txt,2  ; remove <*>

            ;/* register ? */
            IF ((OPATTR repargs_txt) AND 10000y) NE 0
               	repargs_args TEXTEQU repargs_args,<,>,<DWORD ptr [>,repargs_txt,<]>
            ELSE
                IFE repargs_eax_saved
                    mov DWORD ptr [esp-4],eax
                    repargs_eax_saved = 1
                ENDIF
                mov eax,repargs_txt
                mov eax,[eax]
                mov DWORD ptr [esp-2*repargs_nargs*4+repargs_cntr*4],eax
                repargs_args TEXTEQU repargs_args,<,>,<DWORD ptr [esp->,%(repargs_nargs*4+4),<]>
            ENDIF
        ELSE
            IF repargs_pos9 NE 0 OR repargs_posA NE 0
        		repargs_args TEXTEQU repargs_args,<,>,@fSlvTSA(%repargs_txt)
            ELSE
            	;/* record unprocessed argument */
            	repargs_args TEXTEQU repargs_args,<,>,repargs_txt
            ENDIF
        ENDIF
        repargs_cntr = repargs_cntr + 1
    ENDM
    IF repargs_eax_saved
       	mov eax,DWORD ptr [esp-4]
    ENDIF
    EXITM repargs_args
endm

m2m macro a,b
	push b
	pop a	
endm

m2r2m macro a,b
	IF type b EQ 8 AND @WordSize EQ 8
		mov rax,b
		mov a,rax
	ELSEIF type b EQ 4
		mov eax,b
		mov a,eax
	ELSEIF type b EQ 2
		mov ax,b
		mov a,ax
	ELSEIF type b EQ 1
		mov al,b
		mov a,al
	ELSE
		.err <type not supported>
	ENDIF
endm
