option casemap :none
option frame:auto
option procalign:16
option fieldalign:8

JWASM_STORE_REGISTER_ARGUMENTS	EQU 1
JWASM_STACK_SPACE_RESERVATION	EQU 2

option win64:JWASM_STACK_SPACE_RESERVATION

UNICODE 				EQU 1
WIN32_LEAN_AND_MEAN 	EQU 1
_WIN64 					EQU 1
NOMINMAX 				EQU 1			; needed to avoid name conflicts with the fSlv-Functions min()/max()
include \WinInc\include\windows.inc  	; Japheths WinInc
includelib \WinInc\lib64\Kernel32.Lib 	; WSDK		
includelib \WinInc\lib64\User32.Lib  	;

includelib \WinInc\lib64\msvcrt.lib		; VC

include \macros\SmplMath\math.inc
include \macros\macros.inc

IFDEF UNICODE
	EXTERNDEF swprintf: proto buffer:ptr WCHAR,frmt:ptr WCHAR,args:VARARG
	EXTERNDEF wprintf: proto frmt:ptr WCHAR,args:VARARG
	EXTERNDEF wscanf: proto frmt:ptr WCHAR,args:VARARG
	EXTERNDEF _getwch: proto
	_getch EQU _getwch
	sprintf EQU swprintf
	printf EQU wprintf
	scanf EQU wscanf
ELSE
	EXTERNDEF sprintf: proto buffer:ptr CHAR,frmt:ptr CHAR,args:VARARG
	EXTERNDEF printf: proto frmt:ptr CHAR,args:VARARG
	EXTERNDEF scanf: proto frmt:ptr CHAR,args:VARARG
	EXTERNDEF _getch: proto
ENDIF

;fSlvSelectBackEnd fpu

.code
main proc FRAME
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
	
main endp
end main