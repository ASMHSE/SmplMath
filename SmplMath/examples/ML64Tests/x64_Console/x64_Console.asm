    include \masm32\include64\masm64rt.inc
    ifndef @WordSize
        @WordSize equ 8
    endif

include \macros\SmplMath\math.inc
include \macros\SmplMath\accs\macros.inc
include \macros\SmplMath\accs\Macros64G.inc


IFDEF UNICODE
       TCHAR textequ <WCHAR>
	EXTERNDEF swprintf: proto buffer:ptr WCHAR,frmt:ptr WCHAR,args:VARARG
	EXTERNDEF wprintf: proto frmt:ptr WCHAR,args:VARARG
	EXTERNDEF wscanf: proto frmt:ptr WCHAR,args:VARARG
	EXTERNDEF _getwch: proto
	_getch EQU _getwch
	sprintf EQU swprintf
	printf EQU wprintf
	scanf EQU wscanf
ELSE
       TCHAR textequ <CHAR>
	EXTERNDEF sprintf: proto buffer:ptr CHAR,frmt:ptr CHAR,args:VARARG
	EXTERNDEF printf: proto frmt:ptr CHAR,args:VARARG
	EXTERNDEF scanf: proto frmt:ptr CHAR,args:VARARG
	EXTERNDEF _getch: proto
ENDIF

;fSlvSelectBackEnd fpu

.code

entry_point proc
    LOCAL x:REAL8,y:REAL8
    LOCAL sz[128]:TCHAR
    LOCAL fSlvTLS()

	fnc printf,"Calculate logbx(base,x)\nEnter base: "
	fnc scanf,"%lG",&x
	fnc printf,"Enter value: "
	fnc scanf,"%lG",&y
	fnc printf,"logbx(%G,%G) = %.10G\n",x,y,@fSlv8(logbx(x,y))
	fnc printf,"press any key to continue..."
	invoke _getch
	invoke ExitProcess,0
	
entry_point endp

end
