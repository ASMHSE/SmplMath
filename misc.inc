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

; remove leading/trailing Tabs and Spaces
@TrimStr macro txt:=<>
	ts_txt TEXTEQU <>
	ts__size SIZESTR <&txt>
	IF ts__size NE 0
		IF @InStr(1,<&txt>,<,>) EQ 0
			FOR _arg_,<&txt>
				ts_txt TEXTEQU <&_arg_>
			ENDM
			EXITM ts_txt
		ELSE
			ts__txt TEXTEQU <&txt>
			ts__cntr = 1
			ts__flg = 0
			WHILE ts__cntr LE ts__size
				ts__char SUBSTR ts__txt,ts__cntr,1
				ts__in INSTR 1,< 	>,ts__char
				IF ts__flg EQ 0 AND ts__in EQ 0
					ts__flg = 1
					ts_txt TEXTEQU ts_txt,ts__char
				ELSEIF ts__flg NE 0
					ts_txt TEXTEQU ts_txt,ts__char
				ENDIF
				IF ts__cntr+1 LE ts__size
					ts__rest SUBSTR ts__txt,ts__cntr+1
				ELSE
					ts__rest TEXTEQU <>
				ENDIF
				IFB	ts__rest
					EXITM
				ENDIF
				ts__cntr = ts__cntr + 1
			ENDM
		ENDIF
	ENDIF
	EXITM ts_txt
endm

; Replaces all occurrences of txt2 in txt1 with txt3
@RepAllStr macro pos:req,txt1:=<>,txt2:=<>,txt3:=<>

	rpa_txt1 	TEXTEQU <&txt1>
	rpa_txt2 	TEXTEQU <&txt2>
	rpa_txt3 	TEXTEQU <&txt3>

	rpa_size1	SIZESTR rpa_txt1
	rpa_size2	SIZESTR rpa_txt2
	rpa_size3	SIZESTR	rpa_txt3
	rpa_flag	= 0

	IF pos GT rpa_size1
		.err <index value past end of string - parameter 1>
		EXITM rpa_txt1
	ELSEIF pos LE 0
		.err <invalid index - paramter 1>
		EXITM rpa_txt1
	ELSEIF rpa_size1 EQ 0
		.err <string required - parameter 2>
		EXITM <>
	ELSEIF rpa_size2 EQ 0
		EXITM rpa_txt1	
	ELSEIF rpa_size2 GT rpa_size1
		EXITM rpa_txt1
	ENDIF

	rpa_pos INSTR pos,rpa_txt1,rpa_txt2
	WHILE rpa_pos
		rpa_pre SUBSTR rpa_txt1,1,rpa_pos-1
		IF rpa_pos+rpa_size2 LE rpa_size1
			rpa_suff SUBSTR rpa_txt1,rpa_pos+rpa_size2
		ELSE
			rpa_suff TEXTEQU <>
		ENDIF
		rpa_txt1  CATSTR rpa_pre,rpa_txt3,rpa_suff
		rpa_size1 SIZESTR rpa_txt1
		rpa_pos   INSTR pos,rpa_txt1,rpa_txt2
	ENDM
	EXITM rpa_txt1
endm

; Replaces all occurrences of rpa_txt2 in rpa_txt1 with rpa_txt3
; In: text macros rpa_txt1/2/3
; Out: rpa_txt1
@REPALLSTR macro pos:req

	IFNDEF rpa_txt1
		.err <invalid macro usage>
		EXITM
	ELSEIFNDEF rpa_txt2
		.err <invalid macro usage>
		EXITM
	ELSEIFNDEF rpa_txt3
		.err <invalid macro usage>
		EXITM
	ENDIF

	rpa_size1	SIZESTR rpa_txt1
	rpa_size2	SIZESTR rpa_txt2
	rpa_size3	SIZESTR	rpa_txt3
	rpa_flag	= 0

	IF pos GT rpa_size1
		.err <index value past end of string - parameter 1>
		EXITM
	ELSEIF pos LE 0
		.err <invalid index - paramter 1>
		EXITM
	ELSEIF rpa_size1 EQ 0
		.err <string required - parameter 2>
		EXITM
	ELSEIF rpa_size2 EQ 0
		EXITM	
	ELSEIF rpa_size2 GT rpa_size1
		EXITM
	ENDIF

	rpa_pos INSTR pos,rpa_txt1,rpa_txt2
	WHILE rpa_pos
		rpa_pre SUBSTR rpa_txt1,1,rpa_pos-1
		IF rpa_pos+rpa_size2 LE rpa_size1
			rpa_suff SUBSTR rpa_txt1,rpa_pos+rpa_size2
		ELSE
			rpa_suff TEXTEQU <>
		ENDIF
		rpa_txt1  CATSTR rpa_pre,rpa_txt3,rpa_suff
		rpa_size1 SIZESTR rpa_txt1
		rpa_pos   INSTR pos,rpa_txt1,rpa_txt2
	ENDM
	EXITM
endm

@ToLowerCase macro txt:=<>
	tlc_txt TEXTEQU <>
	FORC char,<&txt>
		tlc_pos INSTR 1,<ABCDEFGHIJKLMNOPQRSTUVWXYZ>,<&char>
		IF tlc_pos
			tlc_txt TEXTEQU tlc_txt,@SubStr(<abcdefghijklmnopqrstuvwxyz>,%tlc_pos,1)
		ELSE
			tlc_txt TEXTEQU tlc_txt,<&char>
		ENDIF
	ENDM
	EXITM tlc_txt
endm

@ToUpperCase macro txt:=<>
	tlc_txt TEXTEQU <>
	FORC char,<&txt>
		tlc_pos INSTR 1,<abcdefghijklmnopqrstuvwxyz>,<&char>
		IF tlc_pos
			tlc_txt TEXTEQU tlc_txt,@SubStr(<ABCDEFGHIJKLMNOPQRSTUVWXYZ>,%tlc_pos,1)
		ELSE
			tlc_txt TEXTEQU tlc_txt,<&char>
		ENDIF
	ENDM
	EXITM tlc_txt
endm

; remove all occurrences of args[i] in txt.
; Working with blanks requires the usage of double
; angle bracket: << >>
; OUT: EXITM <> = modified string
;      rasi_out = EXITM <>
;      rasi_txt = modefied string in lower case
;      rasi_lst = comma separated list of indexes (zero based) in args.
;                 Each index equals an occurrences args[i] in txt
@RemoveAllStrI macro txt:=<>,args:VARARG
	rasi_txt TEXTEQU @ToLowerCase(<&txt>)
	rasi_out TEXTEQU <&txt>
	rasi_index = 0
	rasi_lst TEXTEQU <>
	FOR _arg,<&args>
		rasi_sub TEXTEQU <&_arg>
		rasi_sub TEXTEQU @ToLowerCase(%rasi_sub)
		IF @SizeStr(%rasi_txt)
			rasi_pos INSTR 1,rasi_txt,rasi_sub
			rasi_size SIZESTR rasi_sub
			WHILE rasi_pos
				IFB rasi_lst
					rasi_lst TEXTEQU %rasi_lst
				ELSE
					rasi_lst TEXTEQU <,>,%rasi_lst
				ENDIF
				IF rasi_pos+rasi_size LE @SizeStr(%rasi_txt)
					rasi_txt TEXTEQU @SubStr(%rasi_txt,1,%rasi_pos-1),@SubStr(%rasi_txt,%rasi_pos+rasi_size,)
					rasi_out TEXTEQU @SubStr(%rasi_out,1,%rasi_pos-1),@SubStr(%rasi_out,%rasi_pos+rasi_size,)
				ELSE
					rasi_txt TEXTEQU @SubStr(%rasi_txt,1,%rasi_pos-1)
					rasi_out TEXTEQU @SubStr(%rasi_out,1,%rasi_pos-1)
				ENDIF
				IF @SizeStr(%rasi_txt)
					rasi_pos INSTR 1,rasi_txt,rasi_sub
				ELSE
					EXITM
				ENDIF
			ENDM
		ENDIF
		rasi_index = rasi_index + 1
	ENDM
	EXITM rasi_out
endm

; index = zero-based
@GetArgByIndex macro index:req,default:=<>,args:VARARG
	gabi_index = 0
	gabi_arg TEXTEQU <>
	FOR _arg,<&args>
		IF index EQ gabi_index
			gabi_arg TEXTEQU <&_arg>
			EXITM
		ENDIF
		gabi_index = gabi_index + 1
	ENDM
	IFB gabi_arg
		gabi_arg TEXTEQU <&default>
	ENDIF
	EXITM gabi_arg
endm

@GetArgCount macro args:VARARG
	gabi_index = 0
	FOR _arg,<&args>
		gabi_index = gabi_index + 1
	ENDM
	EXITM %gabi_index
endm

@ReversedArgList macro args:VARARG
	@ral_args TEXTEQU <>
	FOR arg0,<&args>
		IFB @ral_args
			@ral_args TEXTEQU <&arg0>
		ELSE
			@ral_args TEXTEQU <&arg0>,<,>,@ral_args
		ENDIF		
	ENDM
	EXITM @ral_args
endm

; trim: trim txt if != 0 , msi_txt = trimmed text
; return: EXITM <>  = msi_index = one based index in args
; or zero if no match
@MatchStrI macro txt:=<>,trim:=<0>,args:VARARG
	msi_txt TEXTEQU <&txt>
	msi_index = 1
	msi_found = 0
	IF trim
		msi_txt TEXTEQU @TrimStr(%msi_txt)
	ENDIF
	FOR _arg,<&args>
		msi_sub TEXTEQU <&_arg>
		IFIDNI msi_txt,msi_sub
			msi_found = 1
			EXITM
		ENDIF
		msi_index = msi_index + 1
	ENDM
	IF msi_found
		EXITM %msi_index
	ENDIF
	msi_index = 0
	EXITM <0>
endm

@MatchStr macro txt:=<>,trim:=<0>,args:VARARG
	msi_txt TEXTEQU <&txt>
	msi_index = 1
	msi_found = 0
	IF trim
		msi_txt TEXTEQU @TrimStr(%msi_txt)
	ENDIF
	FOR _arg,<&args>
		msi_sub TEXTEQU <&_arg>
		IFIDN msi_txt,msi_sub
			msi_found = 1
			EXITM
		ENDIF
		msi_index = msi_index + 1
	ENDM
	IF msi_found
		EXITM %msi_index
	ENDIF
	msi_index = 0
	EXITM <0>
endm

;-----------------------------------------------------------------------------------------------------------;
;Name:          @ScanForFlt(pos,txt,Chr) - scan for float                                                   ;
;Description:   scans in given string for a floating point or integer value. The integer value can          ;
;               be written in hexadecimal or decimal notation.                                              ;
;               There is no size-checking, only syntax (MASM-specific)                                      ;
;               Number format:   1. [+-]{0-9}[Tt]                   e.g.: -12345     , +12345t              ;
;                                2. [+-]{0-9}[0-9a-fA-F]{Hh}        e.g.: +0ab3fh    , 0FFFFFFFFh           ;
;                                3. [+-]{0-9}{.}[0-9]               e.g.: -999.99    , 12.0  , 1.           ;
;                                4. [+-]{0-9}{.}[0-9]{Ee}[+-]{0-9}  e.g.: +10.9E-12  , 3.E8                 ;
;                                5. [+-][0]{0-9a-fA-F}{Rr}          e.g.: 3F800000r  , 03F800000r           ;
;                                   {}=needed , []=optional                                                 ;
;                                   leading blanks are ignored (space/tab)                                  ;
;               The size for (5.) is restricted by the target type: 8 digits for REAL4, 16 for REAL8 and    ;
;               20 for REAL10. One more leading zero is allowed and required if the numbers begins          ;
;               with A-F or has leading zeros. For example the REAL4 0x0123 must be written as:             ;
;               000000123r (9 digits). Furthermore, the numeric string may be modified so that it           ;
;               has always a leading zero and the possible negative sign is moved into the hex-encoding     ;
;               (because MASM do not allow signs).                                                          ;
;                                                                                                           ;
;                                                                                                           ;
; Parameter:                                                                                                ;
;       pos:    start position in given expression                                                          ;
;       txt:    expression to scan                                                                          ;
;       Chr:    [optional] a string containing all characters that can follow a numeric expression.         ;
;               This test applies to the character directly following the numeric expression (one char.     ;
;               is tested)                                                                                  ;
;               If you specify "default" then for the follow char. is tested:                               ;
;                                         space,tab,+,-,*,/,),","                                           ;
;               Another key word is "blank", which cause to test for space and tab.                         ;
;               The key word "blanks" let the macro test all(!) remaining characters in the string          ;
;               If this parameter is blank, the macro doesn't care the char.                                ;
;               following a valid numeric expression, even if this invalidate it.                           ;
;               (see Example 6,7,8)                                                                         ;
;                                                                                                           ;
; Out:                                                                                                      ;
;   EXITM <>    0 => not a numeric expression   (if sff_type == -1: pos past end of string)                 ;
;               1 => floating point number                                                                  ;
;                    To differ between hexadecimal and decimal notation test for the                        ;
;                    SFF_HEX_REAL-flag.                                                                     ;
;               2 => integer                                                                                ;
;                    To differ between hexadecimal and decimal notation test for the                        ;
;                    SFF_HEX_SUFFIX-flag.                                                                   ;
;                                                                                                           ;
;   sff_type:   0,1,2 (same as EXITM <>)                                                                    ;
;                                                                                                           ;
;   sff_flag:   a combination of the following flags (according to num-type):                               ;
;   +-----------------------------+--------------------+-------------------------------------------------+  ;
;   |       valid for:            |        Flag        |    Description                                  |  ;
;   |float|float(hex)|int|int(hex)|                    |                                                 |  ;
;   +-----|----------+---+--------+--------------------+-------------------------------------------------+  ;
;   |  X  |          | X |    X   | SFF_SIGN           | the number has an sign                          |  ;
;   |  X  |          |   |        | SFF_EXPONENT       | an exponent is present                          |  ;
;   |  X  |          |   |        | SFF_EXP_SIGN       | the exponent has a sign                         |  ;
;   |     |          | X |        | SFF_DECIMAL_SUFFIX | the decimal-number ends with an "t" or "T"      |  ;
;   |     |          |   |    X   | SFF_HEX_SUFFIX     | the integer is writen in hexadecimal notation   |  ;
;   |     |    X     |   |        | SFF_HEX_REAL       | floating point value in hexadecimal notation    |  ;
;   |     |    X     |   |        | SFF_HEX_REAL4/8/10 | the type of FP value when written in            |  ;
;   |     |          |   |        |                    | hexadecimal notation                            |  ;
;   +-----+----------+---+--------+--------------------+-------------------------------------------------+  ;
;                                                                                                           ;
;   sff_numstr:         extracted numeric expression (all blanks are removed)                               ;
;   sff_pos:            points to next char. following numeric string (this could be out of string-range)   ;
;   sff_num_pos:        points to begin of num (in txt)                                                     ;
;                                                                                                           ;
;   The following TEXTEQUs depends on sff_flag and/or the type of num:                                      ;
;    +------------------+------------------+-----+------------------------+-------------------------+       ;
;    |      name        |      flag        |type |      description       |  characters             |       ;
;    +------------------+------------------+-----+------------------------+-------------------------+       ;
;    | sff_sign         | SFF_SIGN         | 1,2 | sign of num. Not used  | <+->                    |       ;
;    |                  |                  |     | for FP values in       |                         |       ;
;    |                  |                  |     | hex. notation.         |                         |       ;
;    | sff_pre_decimal  |                  | 1,2 | pre-decimal positions  | <0123456789>            |       ;
;    | sff_fract_digits | SFF_FRACT_DIGITS | 1   | post decimal positions,| <0123456789>            |       ;
;    | sff_exp_sign     | SFF_EXP_SIGN     | 1   | sign of the exponent   | <+->                    |       ;
;    |                  | +SFF_EXPONENT    |     |                        |                         |       ;
;    | sff_exp_digits   | SFF_EXPONENT     | 1   | exponent-digits        | <0123456789>            |       ;
;    | sff_hex_digits   | SFF_HEX_SUFFIX   | 1,2 | hex-digits             | <0123456789abcdefABCDEF>|       ;
;    +------------------+------------------+-----+------------------------+-------------------------+       ;
;                                                                                                           ;
; Examples:                                                                                                 ;
;   1       @ScanForFlt(1,< 1.0E-19 >)     --> 1 , sff_numstr=<1.0E-19> , sff_pos=9                         ;
;   2       @ScanForFlt(1,<-1.0*x>)        --> 1 , sff_numstr=<-1.0>    , sff_pos=5                         ;
;   3       @ScanForFlt(1,< -100 + x>)     --> 2 , sff_numstr=<-100>    , sff_pos=6                         ;
;   4     ! @ScanForFlt(1,<-1E-2>)         --> 2 , sff_numstr=<-1>      , sff_pos=3                         ;
;   5       @ScanForFlt(1,< 0ffh >)        --> 2 , sff_numstr=<0ffh>    , sff_pos=6                         ;
;                                                                                                           ;
;   6       @ScanForFlt(1,<12()>)          --> 2 , sff_numstr=<12>      , sff_pos=3                         ;
;   7       @ScanForFlt(1,<12()>,default)  --> 0                                                            ;
;   8       myChars TEXTEQU <   +-*/()>                                                                     ;
;           @ScanForFlt(1,<12()>,myChars)  --> 2 , sff_numstr=<12>      , sff_pos=3                         ;
;                                                                                       by qWord, 2008-2014 ;
;-----------------------------------------------------------------------------------------------------------;
@ScanForFlt macro pos:req,txt:=<>,Chr

	IFNDEF SFF_PRE_DECIMAL
		SFF_PRE_DECIMAL		EQU 1
		SFF_FRACT_DIGITS	EQU 2
		SFF_DOT				EQU 4
		SFF_EXPONENT		EQU 8
		SFF_EXP_DIGITS		EQU 16
		SFF_SIGN			EQU 32
		SFF_EXP_SIGN		EQU 64
		SFF_DECIMAL_SUFFIX	EQU 128
		SFF_HEX_DIGITS		EQU 256
		SFF_HEX_SUFFIX		EQU 512
		SFF_HEX_REAL		EQU 1024
		SFF_HEX_REAL4		EQU 2048
		SFF_HEX_REAL8		EQU 4096
		SFF_HEX_REAL10		EQU 8192
	ENDIF

	sff_txt 			TEXTEQU <txt>
	sff_size 			SIZESTR sff_txt	
	sff_err 			= 0				
	sff_last_decimal	= 0				
	
	sff_pos				= pos			
	sff_num_pos 		= 0             
	sff_flag			= 0             
	sff_type			= 0             
	sff_numstr			TEXTEQU <>      
	sff_sign			TEXTEQU <>      
	sff_pre_decimal		TEXTEQU <>      
	sff_fract_digits	TEXTEQU <>      
	sff_exp_sign		TEXTEQU <>      
	sff_exp_digits		TEXTEQU <>      
	sff_hex_digits		TEXTEQU <>		
	sff_skip_to_check  = 0
	IF sff_pos GT sff_size
		sff_flag = -1
		EXITM <0>
	ENDIF
	
	sff_cntr = 1 
	REPEAT sff_size-pos+1
		sff_blank SUBSTR sff_txt,pos,sff_cntr
		IFNB sff_blank
			sff_num_pos = sff_pos
			EXITM
		ENDIF
		sff_pos = sff_pos + 1
		sff_cntr = sff_cntr + 1	
	ENDM

	IF sff_pos GT sff_size
		EXITM <0>
	ENDIF

	sff_char SUBSTR sff_txt,sff_pos,1
	IFIDN sff_char,<+>
		sff_flag = sff_flag OR SFF_SIGN
		sff_sign TEXTEQU <+>
		sff_pos = sff_pos + 1
	ELSEIFIDN sff_char,<->
		sff_flag = sff_flag OR SFF_SIGN
		sff_sign TEXTEQU <->
		sff_pos = sff_pos + 1
	ENDIF
	
	IF sff_pos GT sff_size
		EXITM <0>
	ENDIF
;:sff_pre_dig	
	REPEAT sff_size-sff_pos+1
		sff_char SUBSTR sff_txt,sff_pos,1
		sff_cntr INSTR 1,<0123456789abcdefABCDEF>,sff_char
		IF sff_cntr NE 0
			sff_pos = sff_pos + 1
			IF sff_cntr GE 11
				IF (sff_flag AND SFF_PRE_DECIMAL) EQ 0
					sff_pos = sff_pos - 1
					sff_err = 1
					EXITM
				ENDIF
				IF sff_last_decimal EQ 0
					sff_last_decimal = sff_pos-1
				ENDIF
				sff_flag = sff_flag OR SFF_HEX_DIGITS
			ELSE
				sff_last_decimal = sff_pos-1
				sff_flag = sff_flag OR SFF_PRE_DECIMAL
			ENDIF
			sff_pre_decimal CATSTR sff_pre_decimal,sff_char
			sff_err = 0
		ELSE
			sff_err = 1
			EXITM
		ENDIF
	ENDM

	IF sff_err EQ 1
		IFIDN sff_char,<.>
			IF (sff_flag AND SFF_HEX_DIGITS) EQ 0
				sff_pos  = sff_pos + 1
				sff_flag = sff_flag OR SFF_DOT
			ELSE
				sff_skip_to_check = 1
			ENDIF
		ELSEIFIDNI sff_char,<t>
			sff_flag = sff_flag OR SFF_DECIMAL_SUFFIX
			sff_pos  = sff_pos + 1
			sff_skip_to_check = 1
		ELSEIFIDNI sff_char,<h>
			sff_flag = sff_flag OR SFF_HEX_SUFFIX
			sff_pos  = sff_pos + 1
			sff_skip_to_check = 1
		ELSEIFIDNI sff_char,<r>
			sff_flag = sff_flag OR SFF_HEX_REAL
			sff_pos  = sff_pos + 1
			sff_skip_to_check = 1
		ELSE
			sff_skip_to_check = 1
		ENDIF
	ELSE
		sff_skip_to_check = 1
	ENDIF

;:sff_fract	
	IF sff_pos GT sff_size
		sff_skip_to_check = 1
	ENDIF
	IF sff_skip_to_check EQ 0
		REPEAT sff_size-sff_pos+1
			sff_char SUBSTR sff_txt,sff_pos,1
			sff_cntr INSTR 1,<0123456789>,sff_char
			IF sff_cntr NE 0
				sff_pos = sff_pos + 1
				sff_flag = sff_flag OR SFF_FRACT_DIGITS
				sff_fract_digits CATSTR sff_fract_digits,sff_char
				sff_err = 0
			ELSE
				sff_err = 1
				EXITM
			ENDIF
		ENDM
		
		IF sff_err EQ 1
			IFIDNI sff_char,<E>
				sff_pos  = sff_pos + 1
				sff_flag = sff_flag OR SFF_EXPONENT
			ELSE
				sff_skip_to_check = 1
			ENDIF
		ELSE
			sff_skip_to_check = 1
		ENDIF
	
		IF sff_pos GT sff_size
			sff_skip_to_check = 1
		ENDIF
		IF sff_skip_to_check EQ 0
			sff_char SUBSTR sff_txt,sff_pos,1
			IFIDN sff_char,<+>
				sff_flag = sff_flag OR SFF_EXP_SIGN
				sff_exp_sign TEXTEQU <+>
				sff_pos = sff_pos + 1
			ELSEIFIDN sff_char,<->
				sff_flag = sff_flag OR SFF_EXP_SIGN
				sff_exp_sign TEXTEQU <->
				sff_pos = sff_pos + 1
			ENDIF
		
;		:sff_exp_dig
			IF sff_pos GT sff_size
				sff_skip_to_check = 1
			ENDIF
			
			IF sff_skip_to_check EQ 0
				REPEAT sff_size-sff_pos+1
					sff_char SUBSTR sff_txt,sff_pos,1
					sff_cntr INSTR 1,<0123456789>,sff_char
					IF sff_cntr NE 0
						sff_pos = sff_pos + 1
						sff_flag = sff_flag OR SFF_EXP_DIGITS
						sff_exp_digits CATSTR sff_exp_digits,sff_char
					ELSE
						EXITM
					ENDIF
				ENDM
			ENDIF
		ENDIF
	ENDIF	
;:sff_check
	IF (sff_flag AND (NOT (SFF_SIGN OR SFF_DECIMAL_SUFFIX))) EQ SFF_PRE_DECIMAL 
		sff_numstr SUBSTR sff_txt,sff_num_pos,sff_pos-sff_num_pos
		sff_type = 2
	ELSEIF (sff_flag AND (NOT (SFF_SIGN OR SFF_FRACT_DIGITS))) EQ (SFF_PRE_DECIMAL OR SFF_DOT )
		sff_numstr SUBSTR sff_txt,sff_num_pos,sff_pos-sff_num_pos
		sff_type = 1
	ELSEIF (sff_flag AND (NOT (SFF_SIGN OR SFF_EXP_SIGN OR SFF_FRACT_DIGITS))) EQ (SFF_PRE_DECIMAL OR SFF_DOT OR SFF_EXPONENT OR SFF_EXP_DIGITS)
		sff_numstr SUBSTR sff_txt,sff_num_pos,sff_pos-sff_num_pos
		sff_type = 1
	ELSEIF (sff_flag AND (NOT (SFF_SIGN OR SFF_HEX_DIGITS))) EQ (SFF_PRE_DECIMAL OR SFF_HEX_SUFFIX) 
		sff_numstr SUBSTR sff_txt,sff_num_pos,sff_pos-sff_num_pos
		sff_hex_digits TEXTEQU sff_pre_decimal
		sff_type = 2
	ELSEIF (sff_flag AND (NOT (SFF_SIGN OR SFF_HEX_DIGITS))) EQ (SFF_PRE_DECIMAL OR SFF_HEX_REAL)
		sff_hex_digits TEXTEQU sff_pre_decimal
		sff_size2 SIZESTR sff_hex_digits
		IF NOT ((sff_size2 AND -2) EQ 8 OR (sff_size2 AND -2) EQ 16 OR (sff_size2 AND -2) EQ 20)  ; MASM bug (all versions) : when using a ELSE clausel here, MASM lost track of nesting.
			sff_type = 0
			EXITM <0>
		ENDIF
		sff_type = 1
		IF (sff_size2 MOD 2) EQ 0
			sff_hex_digits CATSTR <0>,sff_hex_digits
	%	ELSEIFDIF <0>,<@SubStr(%sff_hex_digits,1,1)>
			sff_type = 0
			EXITM <0>
		ENDIF
		IF (sff_size2 AND -2) EQ 8
			sff_flag = sff_flag or SFF_HEX_REAL4
		ELSEIF (sff_size2 AND -2) EQ 16
			sff_flag = sff_flag or SFF_HEX_REAL8
		ELSEIF (sff_size2 AND -2) EQ 20
			sff_flag = sff_flag or SFF_HEX_REAL10
		ENDIF
		IF (sff_flag AND SFF_SIGN) NE 0
			IFIDN sff_sign,<->
				sff_hex_digits TEXTEQU @SubStr(%sff_hex_digits,1,1),@SubStr(<89abcdef01234567>,%1+@CatStr(<0>,%@SubStr(%sff_hex_digits,2,1),<h>),1),@SubStr(%sff_hex_digits,3)
			ENDIF
			sff_flag = sff_flag AND (NOT SFF_SIGN)
		ENDIF
		sff_numstr TEXTEQU sff_hex_digits,sff_char
	ELSEIF (sff_flag AND SFF_PRE_DECIMAL) NE 0 AND (sff_flag AND SFF_HEX_DIGITS) EQ 0
		sff_pos = sff_last_decimal
		sff_numstr CATSTR sff_sign,sff_pre_decimal
		sff_type = 2
	ELSE
		EXITM <0>
	ENDIF

	IFB <&Chr>
		EXITM %sff_type
	ELSE
		IF sff_pos LE sff_size
			IFIDNI <&Chr>,<default>
				sff_validchar TEXTEQU < 	+-*/!)!,>
			ELSEIFIDNI <&Chr>,<blank>
				sff_validchar TEXTEQU < 	>
			ELSE
				sff_validchar TEXTEQU <&Chr>
			ENDIF
			IFDIFI <&Chr>,<blanks>
				sff_char SUBSTR sff_txt,sff_pos,1
				sff_valid INSTR 1,sff_validchar,sff_char
				IF sff_valid EQ 0
					sff_type = 0
					EXITM <0>
				ENDIF
			ELSE
				sff_char SUBSTR sff_txt,sff_pos
				IFNB sff_char
					sff_type = 0
					EXITM <0>
				ENDIF
			ENDIF
		ENDIF
	ENDIF
	EXITM %sff_type
endm

; value = value*(-1) ; requires preceding call to @ScanForFlt()
@SFF_NegValue macro
	IF sff_type EQ 0
		EXITM
	ENDIF
	IF (sff_flag AND SFF_HEX_REAL) NE 0
		sff_hex_digits TEXTEQU @SubStr(%sff_hex_digits,1,1),@SubStr(<89abcdef01234567>,%1+@CatStr(<0>,%@SubStr(%sff_hex_digits,2,1),<h>),1),@SubStr(%sff_hex_digits,3)
		sff_numstr TEXTEQU sff_hex_digits,<r>
	ELSE
		IF (sff_flag AND SFF_SIGN) NE 0
			IFIDN sff_sign,<+>
				sff_sign TEXTEQU <->
			ELSEIFIDN sff_sign,<->
				sff_sign TEXTEQU <+>
			ENDIF
			sff_numstr TEXTEQU sff_sign,@SubStr(%sff_numstr,2)
		ELSE
			sff_flag = sff_flag OR SFF_SIGN
			sff_sign TEXTEQU <->
			sff_numstr TEXTEQU <->,sff_numstr
		ENDIF
	ENDIF
endm

;---------------------------------
;
; index management list
;
;---------------------------------
iml_create macro IMLName:req
	iml_&IMLName&_cnt = 0
	iml_&IMLName&_nfree = 0
	iml_&IMLName&_max = -1	
endm

iml_request_index macro IMLName:req
	IFE iml_&IMLName&_nfree
		@CatStr(<iml_&IMLName&_>,%iml_&IMLName&_cnt)=1
		iml_&IMLName&_cnt = iml_&IMLName&_cnt + 1
		IF iml_&IMLName&_cnt-1 GT iml_&IMLName&_max
			iml_&IMLName&_max=iml_&IMLName&_cnt-1
		ENDIF
		EXITM %iml_&IMLName&_cnt-1
	ELSE
		iri_cntr = 0
		REPEAT iml_&IMLName&_cnt
			IFE @CatStr(<iml_&IMLName&_>,%iri_cntr)
				EXITM
			ENDIF
			iri_cntr=iri_cntr+1
		ENDM
		@CatStr(<iml_&IMLName&_>,%iri_cntr)=1
		iml_&IMLName&_nfree=iml_&IMLName&_nfree-1
		EXITM %iri_cntr
	ENDIF	
endm

iml_free_index macro IMLName:req,index:req
	IF index EQ iml_&IMLName&_cnt-1
		iml_&IMLName&_cnt=iml_&IMLName&_cnt-1
	ELSE
		iml_&IMLName&_nfree=iml_&IMLName&_nfree+1
	ENDIF
	iml_&IMLName&_&index&=0
endm

iml_get_max macro IMLName:req
	IFE iml_&IMLName&_max AND 080000000h 
		EXITM %iml_&IMLName&_max
	ENDIF
	EXITM <-1>
endm

iml_adjust_max macro IMLName:req
	iml_&IMLName&_max=iml_&IMLName&_cnt-1
endm

;------------------------------
;
; list/table macros
;
;------------------------------
lst_create macro lstName:req,nLines:=<1>,nDataRecords:=<1>
	lst_&lstName&_cnt = nLines
	lc_cntr2 = 1
	REPEAT nLines
		lc_cntr = 1
		REPEAT nDataRecords
			@CatStr(<lst_&lstName&_>,%lc_cntr2,<_>,%lc_cntr) TEXTEQU <>
			lc_cntr = lc_cntr + 1
		ENDM
		lc_cntr2 = lc_cntr2 + 1
	ENDM
endm

lst_exist? macro lstName:req
	IFDEF lst_&lstName&_cnt
		EXITM <-1>
	ENDIF
	EXITM <0>
endm

lst_set_data macro lstName:req,iLine:req,iDataRecord:req,data:=<>
	lst_&lstName&_&iLine&_&iDataRecord& TEXTEQU <&data>
endm

lst_get_data macro lstName:req,iLine:req,iDataRecord:req
	EXITM lst_&lstName&_&iLine&_&iDataRecord&
endm

lst_get_cnt macro lstName:req
	EXITM %lst_&lstName&_cnt
endm

lst_append macro lstName:req,nDataRecords:=<1>
	lst_&lstName&_cnt = lst_&lstName&_cnt + 1
	la_cntr = 1
	REPEAT nDataRecords
		@CatStr(<lst_&lstName&_>,%lst_&lstName&_cnt,<_>,%la_cntr) TEXTEQU <>
		la_cntr = la_cntr + 1
	ENDM
	EXITM %lst_&lstName&_cnt
endm

lst_delete macro lstName:req
	lst_&lstName&_cnt = lst_&lstName&_cnt - 1
endm

lst_inc_value macro lstName:req,iLine:req,iDataRecord:req
	lsiv_tmp = lst_&lstName&_&iLine&_&iDataRecord&+1		; for jwasm
	lst_&lstName&_&iLine&_&iDataRecord& TEXTEQU %lsiv_tmp
	EXITM %lst_&lstName&_&iLine&_&iDataRecord&
endm

lst_dec_value macro lstName:req,iLine:req,iDataRecord:req
	lsdv_tmp = lst_&lstName&_&iLine&_&iDataRecord&-1		; for jwasm
	lst_&lstName&_&iLine&_&iDataRecord& TEXTEQU %lsdv_tmp
	EXITM %lst_&lstName&_&iLine&_&iDataRecord&
endm

lst_tm_search macro lstName:req,iDataRecord:=<1>,txt:=<>
	lts_iLine=lst_&lstName&_cnt
	REPEAT lst_&lstName&_cnt
	%	IFIDN <@CatStr(<lst_&lstName&_>,%lts_iLine,<_&iDataRecord&>)>,<&txt>
			EXITM
		ENDIF
		lts_iLine=lts_iLine-1
	ENDM
	EXITM %lts_iLine
endm

lst_DebugOut macro lstName:req,nDataRecords:=<1>,ColumnWidth:=<4>,ClmnOptn:VARARG
	IFNDEF lst_&lstName&_cnt
		echo --------- list debug out ---------------
		echo list <&lstName> does not exist
		echo ----------------------------------------
		EXITM
	ENDIF
	
	lsdo_cntr = 1
	FOR clmn,<&ClmnOptn>
		IFNB <&clmn>
			FOR optn,<&clmn>
				IFNB <%optn>
					@CatStr(<lsdo_co_>,%lsdo_cntr,<_&optn>)
				ENDIF
			ENDM
		ENDIF
		lsdo_cntr = lsdo_cntr + 1
	ENDM
	
	lsdo_cntr = 1
	lsdo_ralign = 0
	lsdo_columns TEXTEQU <|i   |>
	lsdo_txt3	 TEXTEQU <+----+>
	lsdo_txt4	 SUBSTR <-------------------------------------------------------------------------------->,1,ColumnWidth
	lsdo_spc 	 SUBSTR <                                                                                >,1,ColumnWidth
	lsdo_txt4	 CATSTR lsdo_txt4,<+>
	lsdo_cntr2 = 1
	REPEAT nDataRecords
		lsdo_txt TEXTEQU %lsdo_cntr2				
		lsdo_size = @SizeStr(<%lsdo_txt>)
		IF lsdo_is_optn_def(%lsdo_cntr2,<width>)
			lsdo_clmnw = @CatStr(<lsdo_co_>,%lsdo_cntr2,<_width>)
			IF lsdo_clmnw EQ -1
				lsdo_clmnw = ColumnWidth
			ENDIF
		ELSE
			lsdo_clmnw = ColumnWidth
		ENDIF
		IF lsdo_size GT lsdo_clmnw AND lsdo_clmnw
			IF lsdo_is_optn_def(%lsdo_cntr2,<ralign>,<lsdo_ralign>) AND lsdo_ralign NE 0
				lsdo_txt SUBSTR lsdo_txt,lsdo_size-lsdo_clmnw+1,lsdo_clmnw
			ELSE
				lsdo_txt SUBSTR lsdo_txt,1,lsdo_clmnw
			ENDIF
		ELSEIFE lsdo_clmnw
				lsdo_txt TEXTEQU <>		
		ELSE
			IF lsdo_size NE lsdo_clmnw
				IF lsdo_is_optn_def(%lsdo_cntr2,<width>)
					lsdo_txt5 SUBSTR <                                                                                >,1,lsdo_clmnw
					IF lsdo_is_optn_def(%lsdo_cntr2,<ralign>,<lsdo_ralign>) AND lsdo_ralign NE 0
						lsdo_txt CATSTR @SubStr(<%lsdo_txt5>,%lsdo_size+1),lsdo_txt
					ELSE
						lsdo_txt CATSTR lsdo_txt,@SubStr(<%lsdo_txt5>,%lsdo_size+1)
					ENDIF				
				ELSE
					IF lsdo_is_optn_def(%lsdo_cntr2,<ralign>,<lsdo_ralign>) AND lsdo_ralign NE 0
						lsdo_txt CATSTR @SubStr(<%lsdo_spc>,%lsdo_size+1),lsdo_txt
					ELSE
						lsdo_txt CATSTR lsdo_txt,@SubStr(<%lsdo_spc>,%lsdo_size+1)
					ENDIF					
				ENDIF
			ENDIF
		ENDIF
		lsdo_txt6 SUBSTR <-------------------------------------------------------------------------------->,1,lsdo_clmnw
		lsdo_txt3 CATSTR lsdo_txt3,lsdo_txt6,<+>
		
		lsdo_columns CATSTR lsdo_columns,lsdo_txt,<|>
		lsdo_cntr2 = lsdo_cntr2 + 1
	ENDM
	%echo lsdo_columns
	%echo lsdo_txt3

	lsdo_cntr = 1
	REPEAT lst_&lstName&_cnt
		lsdo_i CATSTR %lsdo_cntr,@SubStr(<   >,%@SizeStr(%@CatStr(%lsdo_cntr)))
		lsdo_columns TEXTEQU <|>,lsdo_i,<|>
		lsdo_cntr2 = 1
		REPEAT nDataRecords
			IF lsdo_is_tm_def(&lstName,%lsdo_cntr,%lsdo_cntr2)
				lsdo_txt TEXTEQU @CatStr(<lst_&lstName&_>,%lsdo_cntr,<_>,%lsdo_cntr2)
				IF lsdo_is_optn_def(%lsdo_cntr2,<hex>)
					IF @CatStr(<lsdo_co_>,%lsdo_cntr2,<_hex>) NE 0
						lsdo_hex=lsdo_txt
						pushcontext radix
						.radix 16
						lsdo_txt TEXTEQU %lsdo_hex
						popcontext radix
					ENDIF						
				ENDIF
			ELSE
				lsdo_txt TEXTEQU <!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!>
			ENDIF				
			lsdo_size = @SizeStr(<%lsdo_txt>)

			
			IF lsdo_is_optn_def(%lsdo_cntr2,<width>)
				lsdo_clmnw = @CatStr(<lsdo_co_>,%lsdo_cntr2,<_width>)
				IF lsdo_clmnw EQ -1
					lsdo_clmnw = ColumnWidth
				ENDIF
			ELSE
				lsdo_clmnw = ColumnWidth
			ENDIF
			IF lsdo_size GT lsdo_clmnw AND lsdo_clmnw
				IF lsdo_is_optn_def(%lsdo_cntr2,<ralign>,<lsdo_ralign>) AND lsdo_ralign NE 0
					lsdo_txt SUBSTR lsdo_txt,lsdo_size-lsdo_clmnw+1,lsdo_clmnw
				ELSE
					lsdo_txt SUBSTR lsdo_txt,1,lsdo_clmnw
				ENDIF
			ELSEIFE lsdo_clmnw
				lsdo_txt TEXTEQU <>
			ELSE
				IF lsdo_size NE lsdo_clmnw
					IF lsdo_is_optn_def(%lsdo_cntr2,<width>)
						lsdo_txt5 SUBSTR <                                                                                >,1,lsdo_clmnw
						IF lsdo_is_optn_def(%lsdo_cntr2,<ralign>,<lsdo_ralign>) AND lsdo_ralign NE 0
							lsdo_txt CATSTR @SubStr(<%lsdo_txt5>,%lsdo_size+1),lsdo_txt
						ELSE
							lsdo_txt CATSTR lsdo_txt,@SubStr(<%lsdo_txt5>,%lsdo_size+1)
						ENDIF				
					ELSE
						IF lsdo_is_optn_def(%lsdo_cntr2,<ralign>,<lsdo_ralign>) AND lsdo_ralign NE 0
							lsdo_txt CATSTR @SubStr(<%lsdo_spc>,%lsdo_size+1),lsdo_txt
						ELSE
							lsdo_txt CATSTR lsdo_txt,@SubStr(<%lsdo_spc>,%lsdo_size+1)
						ENDIF					
					ENDIF
				ENDIF
			ENDIF				
			lsdo_columns CATSTR lsdo_columns,lsdo_txt,<|>
			lsdo_cntr2 = lsdo_cntr2 + 1
		ENDM
		%echo lsdo_columns
		;%echo lsdo_txt3
	:Z
		lsdo_cntr = lsdo_cntr + 1
	ENDM

	lsdo_cntr = 1
	FOR clmn,<&ClmnOptn>
		IFNB <&clmn>
			FOR optn,<&clmn>
				IFNB <%optn>
					IFIDNI @SubStr(<&optn>,1,@InStr(1,<&optn>,<=>)-1),<width>
						@CatStr(<lsdo_co_>,%lsdo_cntr,<_>,@SubStr(<&optn>,1,@InStr(1,<&optn>,<=>)-1),<=-1>)
					ELSE
						@CatStr(<lsdo_co_>,%lsdo_cntr,<_>,@SubStr(<&optn>,1,@InStr(1,<&optn>,<=>)-1),<=0>)
					ENDIF
				ENDIF
			ENDM
		ENDIF
		lsdo_cntr = lsdo_cntr + 1
	ENDM	
endm
; used by lst_DebugOut
lsdo_is_tm_def macro lstName,iLine,iDataRecord
	IFDEF lst_&lstName&_&iLine&_&iDataRecord&
		EXITM <1>
	ENDIF
	EXITM <0>	
endm

; used by lst_DebugOut
lsdo_is_optn_def macro num,optn,var
	IFDEF lsdo_co_&num&_&optn&
		IFNB <var>
			var = lsdo_co_&num&_&optn&
		ENDIF
		EXITM <1>
	ENDIF
	IFNB <var>
		var = 0
	ENDIF
	EXITM <0>
endm

IFNDEF __JWASM__  ; fix for version 2.10 

	@IsRealType macro memOpattr:=<>
		irt_type = 0
		IFB <&memOpattr>
			EXITM <0>
		ELSEIF @InStr(1,<&memOpattr>,<::>) NE 0
			EXITM <0>
		ELSEIFE OPATTR memOpattr
			IF @InStr(1,<&memOpattr>,< x>) OR @InStr(1,<&memOpattr>,< X>) OR @InStr(1,<&memOpattr>,<	x>) OR @InStr(1,<&memOpattr>,<	X>)
				IFNDEF @IsTypedXmmReg
					EXITM <0>
				ENDIF
				IF @IsTypedXmmReg(<&memOpattr>,REAL4,REAL8) NE 0
					IF istxr_type GT 0
						EXITM %(istxr_type*4)
					ENDIF
				ENDIF
			ENDIF
			EXITM <0>
		ENDIF
		IF type (memOpattr) EQ REAL4
			irt_type = 4
			EXITM <4>
		ELSEIF type (memOpattr) EQ REAL8
			irt_type = 8
			EXITM <8>
		ELSEIF type (memOpattr) EQ REAL10
			irt_type = 10
			EXITM <10>
		ENDIF
		EXITM <0>
	endm
	
	@IsIntegerType macro memOpattr:=<>
		iit_type = 0
		IFB <&memOpattr>
			EXITM <0>
		ELSEIF @InStr(1,<&memOpattr>,<::>) NE 0
			FOR _arg,<&memOpattr>
				IFIDNI <&_arg>,<edx::eax>
					iit_type = 8
				ENDIF
				EXITM
			ENDM
			EXITM @CatStr(%iit_type)
		ELSEIFE OPATTR memOpattr
			EXITM <0>
		ENDIF
		IF (type (memOpattr) EQ BYTE) OR (type (memOpattr) EQ SBYTE)
			iit_type = 1
			EXITM <1>
		ELSEIF (type (memOpattr) EQ WORD) OR (type (memOpattr) EQ SWORD)
			iit_type = 2
			EXITM <2>
		ELSEIF (type (memOpattr) EQ DWORD) OR (type (memOpattr) EQ SDWORD)
			iit_type = 4
			EXITM <4>
		ELSE
			IF @Version GE 800
				IF (type (memOpattr) EQ QWORD) OR (type (memOpattr) EQ SQWORD)
					iit_type = 8
					EXITM <8>
				ENDIF
			ELSE
				IF (type (memOpattr) EQ QWORD)
					iit_type = 8
					EXITM <8>
				ENDIF
			ENDIF
		ENDIF
		EXITM <0>
	endm
ELSE
	@IsRealType macro memOpattr:=<>
		irt_type = 0
		IFB <&memOpattr>
			EXITM <0>
		ELSEIF @InStr(1,<&memOpattr>,<::>) NE 0
			EXITM <0>
		ELSE
			IF @InStr(1,<&memOpattr>,< x>) OR @InStr(1,<&memOpattr>,< X>) OR @InStr(1,<&memOpattr>,<	x>) OR @InStr(1,<&memOpattr>,<	X>)
				IFDEF @IsTypedXmmReg
					IF @IsTypedXmmReg(<&memOpattr>,REAL4,REAL8) NE 0
						IF istxr_type GT 0
							EXITM %(istxr_type*4)
						ENDIF
						EXITM <0>
					ENDIF
				ENDIF
			ENDIF
		ENDIF
		IF type (memOpattr) EQ REAL4
			irt_type = 4
			EXITM <4>
		ELSEIF type (memOpattr) EQ REAL8
			irt_type = 8
			EXITM <8>
		ELSEIF type (memOpattr) EQ REAL10
			irt_type = 10
			EXITM <10>
		ENDIF
		EXITM <0>
	endm
	
	@IsIntegerType macro memOpattr:=<>
		iit_type = 0
		IFB <&memOpattr>
			EXITM <0>
		ELSEIF @InStr(1,<&memOpattr>,<::>) NE 0
			FOR _arg,<&memOpattr>
				IFIDNI <&_arg>,<edx::eax>
					iit_type = 8
				ENDIF
				EXITM
			ENDM
			EXITM @CatStr(%iit_type)
		ELSE
			IF @InStr(1,<&memOpattr>,< x>) OR @InStr(1,<&memOpattr>,< X>) OR @InStr(1,<&memOpattr>,<	x>) OR @InStr(1,<&memOpattr>,<	X>)
				IFDEF @IsTypedXmmReg
					IF @IsTypedXmmReg(<&memOpattr>,REAL4,REAL8) NE 0
						EXITM <0>
					ENDIF
				ENDIF
			ENDIF
		ENDIF
		IF (type (memOpattr) EQ BYTE) OR (type (memOpattr) EQ SBYTE)
			iit_type = 1
			EXITM <1>
		ELSEIF (type (memOpattr) EQ WORD) OR (type (memOpattr) EQ SWORD)
			iit_type = 2
			EXITM <2>
		ELSEIF (type (memOpattr) EQ DWORD) OR (type (memOpattr) EQ SDWORD)
			iit_type = 4
			EXITM <4>
		ELSE
			IF @Version GE 800
				IF (type (memOpattr) EQ QWORD) OR (type (memOpattr) EQ SQWORD)
					iit_type = 8
					EXITM <8>
				ENDIF
			ELSE
				IF (type (memOpattr) EQ QWORD)
					iit_type = 8
					EXITM <8>
				ENDIF
			ENDIF
		ENDIF
		EXITM <0>
	endm
ENDIF

unique_num_cntr = 0
get_unique_lbl macro name:req
	unique_num_cntr = unique_num_cntr + 1
	EXITM @CatStr(<&name>,%unique_num_cntr)
endm

_IF$ macro cond:req,true:=<>,false:=<>
	IF cond
		EXITM <&true>
	ENDIF
	EXITM <&false>
endm

_IFE$ macro cond:req,true:=<>,false:=<>
	IFE cond
		EXITM <&true>
	ENDIF
	EXITM <&false>
endm

_IFDEF$ macro tkn,true:=<>,false:=<>
	IFDEF <&tkn>
		EXITM <&true>
	ENDIF
	EXITM <&false>
endm

_IFNDEF$ macro tkn,true:=<>,false:=<>
	IFNDEF <&tkn>
		EXITM <&true>
	ENDIF
	EXITM <&false>
endm

_IFB$ macro tkn,true:=<>,false:=<>
	IFB <&tkn>
		EXITM <&true>
	ENDIF
	EXITM <&false>
endm

_IFNB$ macro tkn,true:=<>,false:=<>
	IFNB <&tkn>
		EXITM <&true>
	ENDIF
	EXITM <&false>
endm

_IFIDNI$ macro tkn1,tkn2,true:=<>,false:=<>
	IFIDNI <&tkn1>,<&tkn2>
		EXITM <&true>
	ENDIF
	EXITM <&false>
endm

_IFDIFI$ macro tkn1,tkn2,true:=<>,false:=<>
	IFDIFI <&tkn1>,<&tkn2>
		EXITM <&true>
	ENDIF
	EXITM <&false>
endm

_IFIDN$ macro tkn1,tkn2,true:=<>,false:=<>
	IFIDN <&tkn1>,<&tkn2>
		EXITM <&true>
	ENDIF
	EXITM <&false>
endm

_IFDIF$ macro tkn1,tkn2,true:=<>,false:=<>
	IFDIF <&tkn1>,<&tkn2>
		EXITM <&true>
	ENDIF
	EXITM <&false>
endm

@IndexedList macro start_index:=<1>,inc_value:=<1>,count:req,prefix:=<>,suffix:=<>
	@ixl_cntr = 0 
	@ixl_index = start_index
	@ixl_list TEXTEQU <>
	WHILE @ixl_cntr LT count
		IF @ixl_cntr NE 0
			@ixl_list CATSTR @ixl_list,<,>,<&prefix>,%@ixl_index,<&suffix>
		ELSE
			@ixl_list CATSTR <&prefix>,%@ixl_index,<&suffix>
		ENDIF
		@ixl_index = @ixl_index + inc_value
		@ixl_cntr = @ixl_cntr + 1
	ENDM
	EXITM @ixl_list
endm

@TypeMatch macro tkn,types:VARARG
	IFB <tkn>
		EXITM <0>
	ELSEIFB <types>
		EXITM <0>
	ENDIF
	@typmtch_match = 0
	@typmtch_cntr = 1
	FOR typ,<&types>
		IF type (tkn) EQ typ
			@typmtch_match = @typmtch_cntr
			EXITM
		ENDIF
		@typmtch_cntr = @typmtch_cntr + 1
	ENDM
	EXITM %@typmtch_match
endm
