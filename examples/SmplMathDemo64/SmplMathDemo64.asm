option casemap :none
option frame:auto
option procalign:16
option fieldalign:8

JWASM_STORE_REGISTER_ARGUMENTS	EQU 1
JWASM_STACK_SPACE_RESERVATION	EQU 2

option win64:JWASM_STACK_SPACE_RESERVATION or JWASM_STORE_REGISTER_ARGUMENTS

UNICODE 				EQU 1
WIN32_LEAN_AND_MEAN 	EQU 1
_WIN64 					EQU 1

include \WinInc\include\windows.inc  	; Japheth's WinInc

includelib \WinInc\lib64\Kernel32.Lib 	; WSDK		
includelib \WinInc\lib64\User32.Lib  	; 
includelib \WinInc\lib64\gdiplus.lib	; 
includelib \WinInc\lib64\gdi32.lib		; 
includelib \WinInc\lib64\msvcrt.lib		; VC

IFDEF UNICODE
	EXTERNDEF swprintf: proto buffer: ptr TCHAR,frmt:ptr TCHAR,args:VARARG
	sprintf EQU swprintf
ELSE
	EXTERNDEF sprintf: proto buffer: ptr TCHAR,frmt:ptr TCHAR,args:VARARG
ENDIF

include \macros\SmplMath\math.inc
include \macros\macros.inc

include ..\FncDrawCtrl64\gdip.inc
include ..\FncDrawCtrl64\GdiPlusFlat.inc

include ..\FncDrawCtrl64\FncDrawCtrl64.inc
includelib FncDrawCtrl64.lib

;/* register constant e */
fSlvRegConst e,2.718281828

CallBack1 proto x:REAL8,py: ptr REAL8
CallBack2 proto x:REAL8,py: ptr REAL8
CallBack3 proto x:REAL8,py: ptr REAL8
CallBack4 proto x:REAL8,py: ptr REAL8

WND_DATA struct
	hCtrl1	HWND	?
	hCtrl2	HWND	?
WND_DATA ends

m2m macro a,b
	push b
	pop a
endm

.code
main proc FRAME
LOCAL wc:WNDCLASSEX
LOCAL msg:MSG
LOCAL gsi:GdiplusStartupInput
LOCAL gtkn:ptr ULONG
LOCAL rect:RECT
LOCAL fSlvTLS()

	mov gsi.GdiplusVersion,1
	mov gsi.DebugEventCallback,0
	mov gsi.SuppressBackgroundThread,0
	mov gsi.SuppressExternalCodecs,0
	invoke GdiplusStartup,ADDR gtkn,ADDR gsi,0

	mov wc.hInstance,rv(GetModuleHandle,0)
	mov wc.cbSize,SIZEOF wc
	mov wc.style,CS_HREDRAW or CS_VREDRAW or CS_DBLCLKS or CS_SAVEBITS
	mov rax,WndProc
	mov wc.lpfnWndProc,rax
	mov wc.cbClsExtra,0
	mov wc.cbWndExtra,SIZEOF WND_DATA
	mov rax,tc$("SomeClass64")
	mov wc.lpszClassName,rax
	mov wc.hIcon,rv(LoadIcon,NULL,IDI_APPLICATION)
	mov wc.hIconSm,rax
	mov wc.hCursor,rv(LoadCursor,NULL,IDC_ARROW)
	mov wc.lpszMenuName,0
	mov wc.hbrBackground,rv(GetStockObject,WHITE_BRUSH)
	invoke RegisterClassEx,ADDR wc

	 
	fSlv rect.left   = 0.125 * rv(GetSystemMetrics,SM_CXSCREEN)
	fSlv rect.top    = 0.125 * rv(GetSystemMetrics,SM_CYSCREEN)
	fSlv rect.right  = 0.75  * rv(GetSystemMetrics,SM_CXSCREEN)
	fSlv rect.bottom = 0.75  * rv(GetSystemMetrics,SM_CYSCREEN)
	
	mov rax,rv(CreateWindowEx,0,wc.lpszClassName,"SmplMathDemo",WS_SYSMENU or WS_SIZEBOX or WS_VISIBLE or WS_MINIMIZEBOX or WS_MAXIMIZEBOX or WS_CLIPCHILDREN,rect.left,rect.top,rect.right,rect.bottom,0,0,wc.hInstance,0)
	invoke UpdateWindow,rdi
	invoke ShowWindow,rdi,SW_SHOWNORMAL	
	
	.while 1
		invoke GetMessage,ADDR msg,0,0,0
		.break .if !eax || eax == -1
		invoke TranslateMessage,ADDR msg
		invoke DispatchMessage,ADDR msg		
	.endw
	mov ebx,eax
	invoke GdiplusShutdown,gtkn
	invoke ExitProcess,ebx
	ret
	
main endp


WndProc proc FRAME uses rbx rsi rdi hWnd:HWND,uMsg:UINT,wParam:WPARAM,lParam:LPARAM
LOCAL fncdscptr:FNC_DSCPTR
LOCAL x:REAL8,y:REAL8
LOCAL cv:CURR_VIEW
LOCAL li:LABEL_INFOW
LOCAL graphics:PVOID
LOCAL sz[128]:TCHAR
LOCAL fSlvTLS()

	.if uMsg == WM_CLOSE
		invoke PostQuitMessage,0
	.elseif uMsg == WM_CREATE
		;fn LoadLibrary,"FncDrwCtl.dll"
		
		;/*-------------------------------------*/
		;/* create a control for showing graphs */
		;/*-------------------------------------*/
		mov rdi,rv(CreateWindowEx,0,"FncDrawCtrl64",0,WS_VISIBLE or WS_CHILD or WS_BORDER,10,50,500,500,hWnd,0,rv(rbx=GetWindowLong,hWnd,GWL_HINSTANCE),0)
		
		invoke SetWindowLongPtr,hWnd,0,rdi ; WND_DATA.hCtrl1

		;/*-------------------------------------*/
		;/* request mouse notification          */
		;/*-------------------------------------*/
		invoke SendMessage,rdi,FDCM_SET_STYLE,FDCS_MOUSE_NOTIFICATION,0

		;/*-------------------------------------*/
		;/* overwrite default control settings  */
		;/*-------------------------------------*/
		mov li.flg,PEN__ARGB or TXTBRUSH__ARGB
		
		ldl SQWORD ptr li.pen = 0ff0f0fffh
		ldl SQWORD ptr li.penTxt = 0
		ldl SQWORD ptr li.brushTxt = 0ff000000h
		ldl li.emHeight = 4.0
		
		m2r2m li.pwszFontFam,w$("Arial")
		m2r2m li.pwszFormatX,w$("%.2f")
		m2r2m li.pwszFormatY,w$("%.1f")
		
		ldl li.flgXlbl = ALP_X_DEFAULT
		ldl li.flgYlbl = ALP_Y_DEFAULT
		ldl li.markSize = 1.5
		ldl li.pen_width = 0.75
		ldl li.penTxt_width = 0.75
		ldl li.xMultipleOf = 3.141
		ldl li.yMultipleOf  = 1.0
		ldl li.nxMarks = 5
		ldl li.nyMarks = 5
		ldl li._frame.top = 8.0
		ldl li._frame.left = 8.0
		ldl li._frame.right = 8.0
		ldl li._frame.bottom = 8.0
		ldl li.xLblOffX = 0
		ldl li.xLblOffY = 0
		ldl li.yLblOffX = 0
		ldl li.yLblOffY = 0
		invoke SendMessage,rdi,FDCM_SET_LABEL_INFOW,ADDR li,0

		;/*-------------------------------------*/
		;/* set view                            */
		;/*-------------------------------------*/
		ldl cv.xMax = 9.42477
		ldl cv.xMin = -3.141
		ldl cv.yMax = 3.0
		ldl cv.yMin = -1.0		
		invoke SendMessage,rdi,FDCM_SET_CURR_VIEW,ADDR cv,0

		;/*-------------------------------------*/
		;/* add functions to draw               */
		;/*-------------------------------------*/
		mov fncdscptr.flg,PEN__ARGB
		mov fncdscptr.nPoints,500
		mov rax,OFFSET CallBack1
		mov fncdscptr.pCallBack,rax
		mov fncdscptr.pen,ColorsSalmon
		ldl fncdscptr._width = 0.5
		invoke SendMessage,rdi,FDCM_ADD_FUNCTION,ADDR fncdscptr,0
	
		mov rax,OFFSET CallBack2
		mov fncdscptr.pCallBack,rax
		mov fncdscptr.pen,ColorsDeepSkyBlue
		invoke SendMessage,rdi,FDCM_ADD_FUNCTION,ADDR fncdscptr,0

		;/*-------------------------------------*/
		;/* create a second graph-control and   */
		;/* keep default settings               */
		;/*-------------------------------------*/		
		mov rdi,rv(CreateWindowEx,0,"FncDrawCtrl64",0,WS_VISIBLE or WS_CHILD or WS_BORDER ,520,50,500,500,hWnd,0,rv(rbx=GetWindowLong,hWnd,GWL_HINSTANCE),0)
		invoke SetWindowLongPtr,hWnd,8,rdi ; WND_DATA.hCtrl2
		invoke SendMessage,rdi,FDCM_SET_STYLE,FDCS_MOUSE_NOTIFICATION,0

		mov rax,OFFSET CallBack3
		mov fncdscptr.pCallBack,rax
		mov fncdscptr.pen,ColorsMediumSpringGreen
		invoke SendMessage,rdi,FDCM_ADD_FUNCTION,ADDR fncdscptr,0
		
		mov rax,OFFSET CallBack4
		mov fncdscptr.pCallBack,rax
		mov fncdscptr.pen,ColorsDarkGreen
		invoke SendMessage,rdi,FDCM_ADD_FUNCTION,ADDR fncdscptr,0
		
		invoke SendMessage,rdi,FDCM_SET_BKCOLOR,ColorsAliceBlue,0
		
	.elseif uMsg == WM_MOUSEWHEEL

		;/*-------------------------------------*/
		;/* redirect WM_MOUSEWHEEL to controls  */
		;/*-------------------------------------*/
		invoke fdcRedirectWmMouseWheel,hWnd,wParam,lParam
		
	.elseif uMsg == WM_NOTIFY
	
		;/*-------------------------------------*/
		;/* print mouse position in graph's     */
		;/* world units                         */
		;/*-------------------------------------*/
		
		mov rbx,rv(GetWindowLong,hWnd,0) ; WND_DATA.hCtrl1
		mov rdi,rv(GetWindowLong,hWnd,8) ; WND_DATA.hCtrl2
		mov rdx,lParam
		.if [rdx].FDC_NMHDR.nmhdr.code == FDCNM_MOUSE_MOVE && rbx == [rdx].FDC_NMHDR.nmhdr.hwndFrom
			m2m x,[rdx].FDC_NMHDR.pointd.x
			m2m y,[rdx].FDC_NMHDR.pointd.y
			mov rdi,rv(GetDC,hWnd)
			invoke GdipCreateFromHDC,rdi,ADDR graphics
			invoke GdipGraphicsClear,graphics,ColorsWhite
			invoke GdipDeleteGraphics,graphics
			fn sprintf,&sz,"%G",x
			invoke TextOut,rdi,10,0,ADDR sz,eax
			fn sprintf,&sz,"%G",y
			invoke TextOut,rdi,10,20,ADDR sz,eax
			invoke ReleaseDC,hWnd,rdi
		.elseif [rdx].FDC_NMHDR.nmhdr.code == FDCNM_MOUSE_MOVE && rdi == [rdx].FDC_NMHDR.nmhdr.hwndFrom
			m2m x,[rdx].FDC_NMHDR.pointd.x
			m2m y,[rdx].FDC_NMHDR.pointd.y
			mov rdi,rv(GetDC,hWnd)
			invoke GdipCreateFromHDC,rdi,ADDR graphics
			invoke GdipGraphicsClear,graphics,ColorsWhite
			invoke GdipDeleteGraphics,graphics
			fn sprintf,&sz,"%G",x
			invoke TextOut,rdi,520,0,ADDR sz,eax
			fn sprintf,&sz,"%G",y
			invoke TextOut,rdi,520,20,ADDR sz,eax
			invoke ReleaseDC,hWnd,rdi
		.endif
	.else
		invoke DefWindowProc,hWnd,uMsg,wParam,lParam
		ret
	.endif

	xor eax,eax
	ret
	
WndProc endp

align 16
CallBack2 proc FRAME uses xmm6 x:REAL8,py: ptr REAL8
	
	fSlvVolatileXmmRegs add,xmm6  ; mark XMM6 as volatile register
	
	;/* register expression with one argument (=arg1) */
	fSlvRegExpr <Tri>,1,arg1^(-2)*cos(arg1*xmm0)+(arg1+2)^-2*cos((arg1+2)*xmm0)+(arg1+4)^-2*cos((arg1+4)*xmm0)

	;/* fourier series: triangular pulse */
	fSlv8 REAL8 ptr [rdx] = Tri(1)+Tri(7)+Tri(13)+Tri(19)+Tri(25)\
;				+Tri(31)+Tri(37)+Tri(43)+Tri(49)+Tri(55)\
;				+Tri(61)+Tri(67)+Tri(73)+Tri(79)+Tri(85)\
;				+Tri(91)+Tri(97)+Tri(103)+Tri(109)

	fSlvVolatileXmmRegs default

	ret 
	
CallBack2 endp

;/* lets use the advantage of FASTCALL: the following functions use register arguments */
option prologue:none
option epilogue:none

align 16
CallBack1 proc x:REAL8,py: ptr REAL8
	add rsp,-5*8

	fSlv8 REAL8 ptr [rdx] = limit(tan(xmm0),10,-10)
	
	add rsp,5*8
	ret
	
CallBack1 endp

align 16
CallBack3 proc x:REAL8,py: ptr REAL8
	add rsp,-5*8
	
	fSlv8 REAL8 ptr [rdx] = 3*e^(-1.7*abs((xmm0-2)))*sin(20*(xmm0-2)) + 2

	add rsp,5*8
	ret
	
CallBack3 endp

align 16
CallBack4 proc x:REAL8,py: ptr REAL8
	add rsp,-5*8

	;/* res = x^(-5/4) */
	fSlv REAL8 ptr [rdx] = root2(xmm0,-5,4)
	.if !(r8IsValid(REAL8 ptr [rdx]))
		fSlv REAL8 ptr [rdx] = 0
	.endif
	
	add rsp,5*8
	ret
	
CallBack4 endp

fSlvStatistics
end main
  