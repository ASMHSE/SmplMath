.nolist
include \masm32\include\masm32rt.inc
.686
.mmx
.xmm

include \masm32\include\gdiplus.inc
includelib \masm32\lib\gdiplus.lib

include \macros\smplmath\math.inc
include \macros\smplmath\expressions.inc
include \macros\macros.inc
include ..\FncDrawCtrl\FncDrawCtrl.inc
includelib FncDrawCtrl.lib

IF @Version GE 800
	fSlvSetFlags FSF_USE_SSE2
ENDIF

;/* register constant e */
fSlvRegConst e,2.718281828

d2d macro mem64_dest,mem64_src
	push DWORD ptr mem64_src
	pop DWORD ptr mem64_dest
	push DWORD ptr mem64_src+4
	pop DWORD ptr mem64_dest+4
endm

xchg8 macro arg1,arg2
	mov eax,DWORD ptr arg1
	mov edx,DWORD ptr arg1+4
	d2d arg1,arg2
	mov DWORD ptr arg2,eax
	mov DWORD ptr arg2,edx
endm

CallBack1 proto x:REAL8,py: ptr REAL8,userData:PVOID
CallBack2 proto x:REAL8,py: ptr REAL8,userData:PVOID
CallBack3 proto x:REAL8,py: ptr REAL8,userData:PVOID

WND_DATA struct
	hCtrl1	HWND	?
	hCtrl2	HWND	?
WND_DATA ends

.code
main proc
LOCAL wc:WNDCLASSEX
LOCAL msg:MSG
LOCAL gsi:GdiplusStartupInput
LOCAL gtkn:ptr ULONG
LOCAL rect:RECT
LOCAL fSlvTLS()
LOCAL x:REAL4

	mov gsi.GdiplusVersion,1
	mov gsi.DebugEventCallback,0
	mov gsi.SuppressBackgroundThread,0
	mov gsi.SuppressExternalCodecs,0
	invoke GdiplusStartup,ADDR gtkn,ADDR gsi,0

	mov wc.hInstance,rv(GetModuleHandle,0)
	mov wc.cbSize,SIZEOF wc
	mov wc.style,CS_HREDRAW or CS_VREDRAW or CS_DBLCLKS or CS_SAVEBITS
	mov wc.lpfnWndProc,WndProc
	mov wc.cbClsExtra,0
	mov wc.cbWndExtra,SIZEOF WND_DATA
	mov wc.lpszClassName,chr$("SomeClass32")
	mov wc.hIcon,rv(LoadIcon,NULL,IDI_APPLICATION)
	mov wc.hIconSm,eax
	mov wc.hCursor,rv(LoadCursor,NULL,IDC_ARROW)
	mov wc.lpszMenuName,0
	mov wc.hbrBackground,rv(GetStockObject,WHITE_BRUSH)
	invoke RegisterClassEx,ADDR wc

	fSlv rect.left   = 0.125 * rv(GetSystemMetrics,SM_CXSCREEN)
	fSlv rect.top    = 0.125 * rv(GetSystemMetrics,SM_CYSCREEN)
	fSlv rect.right  = 0.75  * rv(GetSystemMetrics,SM_CXSCREEN)
	fSlv rect.bottom = 0.75  * rv(GetSystemMetrics,SM_CYSCREEN)
	
	mov edi,rv(CreateWindowEx,0,wc.lpszClassName,"SmplMathDemo",WS_SYSMENU or WS_SIZEBOX or WS_VISIBLE or WS_MINIMIZEBOX or WS_MAXIMIZEBOX or WS_CLIPCHILDREN,rect.left,rect.top,rect.right,rect.bottom,0,0,wc.hInstance,0)
	invoke UpdateWindow,edi
	invoke ShowWindow,edi,SW_SHOWNORMAL	
	
	.while 1
		invoke GetMessage,ADDR msg,0,0,0
		.break .if !eax || eax == -1
		invoke TranslateMessage,ADDR msg
		invoke DispatchMessage,ADDR msg		
	.endw
	push eax
	invoke GdiplusShutdown,gtkn
	pop eax
	invoke ExitProcess,0
	ret
	
main endp

WndProc proc uses ebx esi edi hWnd:HWND,uMsg:UINT,wParam:WPARAM,lParam:LPARAM
LOCAL fncdscptr:FNC_DSCPTR
LOCAL x:REAL8,y:REAL8
LOCAL cv:CURR_VIEW
LOCAL li:LABEL_INFOA
LOCAL graphics:PVOID
LOCAL fSlvTLS()
LOCAL sz[256]:TCHAR

	.if uMsg == WM_CLOSE
		invoke PostQuitMessage,0
	.elseif uMsg == WM_CREATE
		;fn LoadLibrary,"FncDrwCtl.dll"
		
		;/*-------------------------------------*/
		;/* create a control for showing graphs */
		;/*-------------------------------------*/
		mov edi,rv(CreateWindowEx,0,"FncDrawCtrl32",0,WS_VISIBLE or WS_CHILD or WS_BORDER,10,50,500,500,hWnd,0,rv(GetWindowLong,hWnd,GWL_HINSTANCE),0)
		invoke SetWindowLong,hWnd,WND_DATA.hCtrl1,edi

		;/*-------------------------------------*/
		;/* request mouse notification          */
		;/*-------------------------------------*/
		invoke SendMessage,edi,FDCM_SET_STYLE,FDCS_MOUSE_NOTIFICATION,0

		;/*-------------------------------------*/
		;/* overwrite default control settings  */
		;/*-------------------------------------*/
		mov li.flg,PEN__ARGB or TXTBRUSH__ARGB
		ldl li.pen = 0ff0f0fffh
		ldl li.penTxt = 0
		ldl li.brushTxt = 0ff000000h
		ldl li.emHeight = 4.0
		mov li.pszFontFam,chr$("Arial")
		mov li.pszFormatX,chr$("%.2f")
		mov li.pszFormatY,chr$("%.1f")
		
		ldl li.flgXlbl = ALP_X_DEFAULT
		ldl li.flgYlbl = ALP_Y_DEFAULT
		ldl li.markSize = 1.5
		ldl li.pen_width = 0.75
		ldl li.penTxt_width = 0.75
		ldl li.xMultipleOf = 3.141
		ldl li.yMultipleOf  = 1.0
		ldl li.nxMarks = 5
		ldl li.nyMarks = 4
		ldl li.frame.top = 8.0
		ldl li.frame.left = 8.0
		ldl li.frame.right = 8.0
		ldl li.frame.bottom = 8.0
		ldl li.xLblOffX = 0
		ldl li.xLblOffY = 0
		ldl li.yLblOffX = 0
		ldl li.yLblOffY = 0
		invoke SendMessage,edi,FDCM_SET_LABEL_INFOA,ADDR li,0

		;/*-------------------------------------*/
		;/* set view                            */
		;/*-------------------------------------*/
		ldl cv.xMax = 9.42477
		ldl cv.xMin = -3.141
		ldl cv.yMax = 3.0
		ldl cv.yMin = -1.0		
		invoke SendMessage,edi,FDCM_SET_CURR_VIEW,ADDR cv,0

		;/*-------------------------------------*/
		;/* add functions to draw               */
		;/*-------------------------------------*/
		mov fncdscptr.flg,PEN__ARGB
		mov fncdscptr.nPoints,1000
		mov fncdscptr.pCallBack,OFFSET CallBack1
		mov fncdscptr.pen,ColorsSalmon
		ldl fncdscptr._width = 0.5
		invoke SendMessage,edi,FDCM_ADD_FUNCTION,ADDR fncdscptr,0
	
		mov fncdscptr.pCallBack,OFFSET CallBack2
		mov fncdscptr.pen,ColorsDeepSkyBlue
		invoke SendMessage,edi,FDCM_ADD_FUNCTION,ADDR fncdscptr,0

		;/*-------------------------------------*/
		;/* create a second graph-control and   */
		;/* keep default settings               */
		;/*-------------------------------------*/		
		mov edi,rv(CreateWindowEx,0,"FncDrawCtrl32",0,WS_VISIBLE or WS_CHILD or WS_BORDER ,520,50,500,500,hWnd,0,rv(GetWindowLong,hWnd,GWL_HINSTANCE),0)
		invoke SetWindowLong,hWnd,WND_DATA.hCtrl2,edi
		invoke SendMessage,edi,FDCM_SET_STYLE,FDCS_MOUSE_NOTIFICATION,0

		mov fncdscptr.pCallBack,OFFSET CallBack3
		mov fncdscptr.pen,ColorsMediumSpringGreen
		invoke SendMessage,edi,FDCM_ADD_FUNCTION,ADDR fncdscptr,0
		invoke SendMessage,edi,FDCM_SET_BKCOLOR,ColorsAliceBlue,0
		
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
		
		mov ebx,rv(GetWindowLong,hWnd,WND_DATA.hCtrl1)
		mov edi,rv(GetWindowLong,hWnd,WND_DATA.hCtrl2)
		mov edx,lParam
		.if [edx].FDC_NMHDR.nmhdr.code == FDCNM_MOUSE_MOVE && ebx == [edx].FDC_NMHDR.nmhdr.hwndFrom
			d2d x,[edx].FDC_NMHDR.pointd.x
			d2d y,[edx].FDC_NMHDR.pointd.y
			mov edi,rv(GetDC,hWnd)
			invoke GdipCreateFromHDC,edi,ADDR graphics
			invoke GdipGraphicsClear,graphics,ColorsWhite
			invoke GdipDeleteGraphics,graphics
			fncx crt_sprintf,&sz,"%.15G",x
			fn TextOut,edi,10,0,&sz,len(ADDR sz)
			fncx crt_sprintf,&sz,"%.15G",y
			fn TextOut,edi,10,20,&sz,len(ADDR sz)
			invoke ReleaseDC,hWnd,edi
		.elseif [edx].FDC_NMHDR.nmhdr.code == FDCNM_MOUSE_MOVE && edi == [edx].FDC_NMHDR.nmhdr.hwndFrom
			d2d x,[edx].FDC_NMHDR.pointd.x
			d2d y,[edx].FDC_NMHDR.pointd.y
			mov edi,rv(GetDC,hWnd)
			invoke GdipCreateFromHDC,edi,ADDR graphics
			invoke GdipGraphicsClear,graphics,ColorsWhite
			invoke GdipDeleteGraphics,graphics
			fncx crt_sprintf,&sz,"%.15G",x
			fn TextOut,edi,520,0,&sz,len(ADDR sz)
			fncx crt_sprintf,&sz,"%.15G",y
			fn TextOut,edi,520,20,&sz,len(ADDR sz)
			invoke ReleaseDC,hWnd,edi
		.endif
	.else
		invoke DefWindowProc,hWnd,uMsg,wParam,lParam
		ret
	.endif

	xor eax,eax
	ret
	
WndProc endp

align 16
CallBack1 proc x:REAL8,py: ptr REAL8,userData:PVOID

	mov edx,py
	fSlv REAL8 ptr [edx] = limit(tan(x),10,-10)
	
	ret
	
CallBack1 endp

align 16
CallBack2 proc x:REAL8,py: ptr REAL8,userData:PVOID
	
	mov edx,py
	
	;/* register recursive expression with two arguments: arg1 = number of terms , arg2 = index of first coefficient */
	fSlvRegRecursiveExpr Tri,2,<0>,Tri(#arg1-1,#(arg2+6)) + arg2^(-2)*cos(arg2*x)+(arg2+2)^-2*cos((arg2+2)*x)+(arg2+4)^-2*cos((arg2+4)*x)

	;/* fourier series: triangular pulse , 10 terms starting at coefficient 1 , precision = REAL4/SDWORD */
	fSlv REAL8 ptr [edx] = Tri(5,1) {i4,r4}

; the same as above, but using a non recursive expression:

	;/* register expression with one argument (=arg1) */
;	fSlvRegExpr <Tri>,1,arg1^(-2)*cos(arg1*x)+(arg1+2)^-2*cos((arg1+2)*x)+(arg1+4)^-2*cos((arg1+4)*x)
	
	;/* fourier series: triangular pulse */
;	fSlv REAL8 ptr [edx] = {i4,r4} Tri(1)+Tri(7)+Tri(13);+Tri(19)+Tri(25)\
;				+Tri(31)+Tri(37)+Tri(43)+Tri(49)+Tri(55)\
;				+Tri(61)+Tri(67)+Tri(73)+Tri(79)+Tri(85)\
;				+Tri(91)+Tri(97)+Tri(103)+Tri(109)

	ret
	
CallBack2 endp

align 16
CallBack3 proc x:REAL8,py: ptr REAL8,userData:PVOID

	mov edx,py
	IF @Version GE 800
		fSlv REAL8 ptr [edx] = 3*expd((-1.7*abs((x-2))))*sin(20*(x-2)) + 2
	ELSE
		fSlv REAL8 ptr [edx] = 3*exp((-1.7*abs((x-2))))*sin(20*(x-2)) + 2
	ENDIF
	
	ret
	
CallBack3 endp

fSlvStatistics
end main
  