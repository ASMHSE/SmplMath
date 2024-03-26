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

EXTERNDEF swprintf: proto buffer: ptr WCHAR,frmt:ptr CHAR,args:VARARG

include \macros\smplmath\math.inc
include \macros\macros.inc

include gdip.inc
include GdiPlusFlat.inc
include FncDrawCtrl64.inc

m2m macro a,b
	push b
	pop a
endm

alloc macro _size:req
	mov [rsp],rsi
	mov rsi,_size
	sub rsp,4*8
	invoke GetProcessHeap
	add rsp,4*8
	mov r8,rsi
	mov rsi,[rsp]
	invoke HeapAlloc,rax,HEAP_ZERO_MEMORY,r8
	EXITM <rax>
endm

free macro p:req
	mov [rsp],rsi
	mov rsi,p
	sub rsp,4*8
	invoke GetProcessHeap
	add rsp,4*8
	mov r8,rsi
	mov rsi,[rsp]
	invoke HeapFree,rax,0,r8
endm

pwsz$ macro wsz:VARARG
LOCAL txt
	.data
		WSTR txt,wsz,0
	.code
	mov [rsp],rsi
	mov rsi,alloc(SIZEOF txt)
	sub rsp,4*8
	invoke lstrcpyW,rsi,ADDR txt
	add rsp,4*8
	mov rax,rsi
	mov rsi,[rsp]	
	EXITM <rax>
endm

GdipGetClientRectF proto hWnd:HWND,pRectF:ptr RectF
draw_axes proto graphics:PVOID,pAxesInfo:ptr AXES_INFO
calc_metrics proto hWnd:HWND,pAxesInfo:ptr AXES_INFO
plot proto graphics:PVOID,pen:PVOID,nPoints:DWORD,pAxesInfo: ptr AXES_INFO,pfnCallBack:ptr CB_FNC_VALUE
scale_axes proto pAxesInfo: ptr AXES_INFO,xMultipleOf:REAL8,yMultipleOf:REAL8,nMarksX:DWORD,nMarksY:DWORD
ApproxEqual proto fValue1:REAL8,fValue2:REAL8,fRange:REAL8
pix2mm proto x:DWORD,y:DWORD,pPointD: ptr PointD
mm2pix proto pPointD: ptr PointD,pPoint: ptr POINT
ClientPoint2Unit proto x:DWORD,y:DWORD,pAxesInfo: ptr AXES_INFO,pPointD: ptr PointD
Unit2ClientPoint proto pPointD: ptr PointD,pAxesInfo: ptr AXES_INFO,pPoint: ptr POINT

.code
DllMain proc FRAME hInstance:HINSTANCE,fdwReason:DWORD,lpvReserved:LPVOID
LOCAL wc:WNDCLASSEXW
	
	.if fdwReason == DLL_PROCESS_ATTACH

		m2m wc.hInstance,hInstance
		mov wc.cbSize,SIZEOF wc
		mov wc.style,CS_HREDRAW or CS_VREDRAW or CS_DBLCLKS or CS_SAVEBITS or CS_GLOBALCLASS
		mov rax,WndProc
		mov wc.lpfnWndProc,rax
		mov wc.cbClsExtra,0
		mov wc.cbWndExtra,sizeof PFNCDRAW_CNTRL
		mov rax,w$("FncDrawCtrl64")
		mov wc.lpszClassName,rax
		mov wc.hIcon,0;rv(LoadIcon,NULL,IDI_APPLICATION)
		mov wc.hIconSm,0;eax
		mov wc.hCursor,rv(LoadCursor,NULL,IDC_ARROW)
		mov wc.lpszMenuName,0
		mov wc.hbrBackground,0
		invoke RegisterClassExW,ADDR wc
	.elseif fdwReason == DLL_PROCESS_DETACH
		invoke UnregisterClassW,w$("FncDrawCtrl64"),hInstance
	.endif

	mov eax,TRUE
	ret
	
DllMain endp


CopyAxesInfo proc FRAME uses rbx rsi pAxesInfo: ptr AXES_INFO
	
	pushcontext assumes
	assume rsi: ptr AXES_INFO
	assume rbx: ptr AXES_INFO
	
	mov rsi,pAxesInfo
	mov rbx,alloc(SIZEOF AXES_INFO)
	
	invoke RtlMoveMemory,rbx,rsi,SIZEOF AXES_INFO
	
	invoke lstrlenW,[rsi].labels.pwszFontFam
	lea rax,[2*rax+2]
	mov [rbx].labels.pwszFontFam,alloc(rax)
	invoke lstrcpyW,[rbx].labels.pwszFontFam,[rsi].labels.pwszFontFam
	
	invoke lstrlenW,[rsi].labels.pwszFormatX
	lea rax,[2*rax+2]
	mov [rbx].labels.pwszFormatX,alloc(rax)
	invoke lstrcpyW,[rbx].labels.pwszFormatX,[rsi].labels.pwszFormatX
	
	invoke lstrlenW,[rsi].labels.pwszFormatY
	lea rax,[2*rax+2]
	mov [rbx].labels.pwszFormatY,alloc(rax)
	invoke lstrcpyW,[rbx].labels.pwszFormatY,[rsi].labels.pwszFormatY
	
	mov rax,rbx
		
	popcontext assumes
	ret
	
CopyAxesInfo endp

FreeAxesInfo proc FRAME uses rbx pAxesInfo: ptr AXES_INFO
	
	assume rbx: ptr AXES_INFO
	
	mov rbx,pAxesInfo
	free [rbx].labels.pwszFontFam
	free [rbx].labels.pwszFormatX
	free [rbx].labels.pwszFormatY
	
	.if [rbx].labels.flg & ( PEN__CTRL_OWNS  or PEN__CTRL_DESTROY or PEN__CLONE)
		invoke GdipDeletePen,[rbx].labels.pen
	.endif
	
	.if [rbx].labels.flg & ( TXTPEN__CTRL_OWNS  or TXTPEN__CTRL_DESTROY or TXTPEN__CLONE)
		invoke GdipDeletePen,[rbx].labels.penTxt
	.endif
	
;	.if [rbx].labels.flg & ( BRUSH__CTRL_OWNS  or BRUSH__CTRL_DESTROY or BRUSH__CLONE)
;		invoke GdipDeleteBrush,[rbx].labels.brush
;	.endif
	
	.if [rbx].labels.flg & ( TXTBRUSH__CTRL_OWNS  or TXTBRUSH__CTRL_DESTROY or TXTBRUSH__CLONE)
		invoke GdipDeleteBrush,[rbx].labels.brushTxt
	.endif

	free pAxesInfo
	
	assume rbx: nothing
	ret
	
FreeAxesInfo endp

IsValidFncHandle proc FRAME uses rbx pFncDrwCtrl:ptr FNCDRAW_CNTRL,hFnc:HFNC
	
	mov rbx,pFncDrwCtrl
	
	.if [rbx].FNCDRAW_CNTRL.nFunctions
		xor rax,rax
		ret		
	.endif
	mov rdx,[rbx].FNCDRAW_CNTRL.pFncDscptr
	mov rax,hFnc 
	.while edx
		.if rdx == rax
			ret
		.endif
		mov rdx,[rdx].FNC_DSCPTR.pNextDscptr
	.endw
	
	xor rax,rax	
	ret
	
IsValidFncHandle endp

fdcRedirectWmMouseWheel proc FRAME uses rbx hWnd:HWND,wParam:WPARAM,lParam:LPARAM
LOCAL szClassName[256]:CHAR
LOCAL pt:POINT
	movzx eax,WORD ptr lParam[0]
	movzx ecx,WORD ptr lParam[2]
	mov pt.x,eax
	mov pt.y,ecx
	
	invoke ScreenToClient,hWnd,ADDR pt
	.if rv(ChildWindowFromPoint,hWnd,pt)
		mov rbx,rax
		.if rv(GetClassNameA,rbx,ADDR szClassName,SIZEOF szClassName)
			.if !rv(lstrcmpA,ADDR szClassName,A"FncDrawCtrl64")
				invoke MapWindowPoints,hWnd,rbx,ADDR pt,1
				mov eax,pt.x
				mov ecx,pt.y
				movzx eax,ax
				shl ecx,16
				or eax,ecx
				invoke SendMessage,rbx,WM_MOUSEWHEEL,wParam,rax
			.endif
		.endif
	.endif
	ret
	
fdcRedirectWmMouseWheel endp

fdc_OnCreate proc FRAME uses rbx hWnd:HWND,lParam:LPARAM
	
	mov rbx,alloc(SIZEOF FNCDRAW_CNTRL)
	invoke SetWindowLongPtr,hWnd,0,rbx
	mov [rbx].FNCDRAW_CNTRL.pAxesInfo,alloc(SIZEOF AXES_INFO)
	mov [rbx].FNCDRAW_CNTRL.zoomFactor,0.1
	mov [rbx].FNCDRAW_CNTRL.zoomFlg,ZOOM_CENTER
	mov [rbx].FNCDRAW_CNTRL.argbBkColor,-1
	mov rax,lParam
	.if [rax].CREATESTRUCT.lpCreateParams
		mov [rbx].FNCDRAW_CNTRL.pAxesInfo,rv(CopyAxesInfo,[rax].CREATESTRUCT.lpCreateParams)
		mov [rbx].FNCDRAW_CNTRL.init,1
	.else
		mov [rbx].FNCDRAW_CNTRL.init,1
		mov rbx,[rbx].FNCDRAW_CNTRL.pAxesInfo
		assume rbx: ptr AXES_INFO
		mov [rbx].labels.pwszFontFam,pwsz$("Arial")
		mov [rbx].labels.pwszFormatX,pwsz$("%.1f")
		mov [rbx].labels.pwszFormatY,pwsz$("%.1f")
		mov [rbx].labels.emHeight,5.0				; 5mm
		mov [rbx].labels.flgXlbl,ALP_X_DEFAULT
		mov [rbx].labels.flgYlbl,ALP_Y_DEFAULT
		mov [rbx].labels.flg,PEN__CTRL_OWNS or BRUSH__CTRL_OWNS or TXTBRUSH__CTRL_OWNS
		mov [rbx].labels.markSize,2.0
		mov rax,REAL8 ptr 1.0
		mov [rbx].labels.xMultipleOf,rax
		mov [rbx].labels.yMultipleOf,rax
		invoke GdipCreatePen1,ColorsBlack,1.0,UnitWorld,ADDR [rbx].labels.pen
		;invoke GdipCreateSolidFill,ColorsBlack,ADDR [rbx].labels.brush
		invoke GdipCreateSolidFill,ColorsBlack,ADDR [rbx].labels.brushTxt
		mov rax,REAL8 ptr 5.0
		mov [rbx].view.xMax,rax
		mov [rbx].view.yMax,rax
		mov rax,REAL8 ptr -5.0
		mov [rbx].view.xMin,rax
		mov [rbx].view.yMin,rax
		mov [rbx].labels._frame.top,10.0
		mov [rbx].labels._frame.left,10.0
		mov [rbx].labels._frame.right,10.0
		mov [rbx].labels._frame.bottom,10.0
		mov [rbx].labels.nxMarks,10
		mov [rbx].labels.nyMarks,10
		assume rbx: nothing
	.endif
	
	
	ret
	
fdc_OnCreate endp


fdc_OnDestroy proc FRAME uses rbx rsi rdi hWnd:HWND
	
	mov rbx,rv(GetWindowLongPtr,hWnd,0)
	
	;/* free AXES_INFO */
	invoke FreeAxesInfo,[rbx].FNCDRAW_CNTRL.pAxesInfo

	;/* free function descriptors */
	.if [rbx].FNCDRAW_CNTRL.nFunctions
		mov rsi,[rbx].FNCDRAW_CNTRL.pFncDscptr
		.repeat
			.if [rsi].FNC_DSCPTR.flg & ( PEN__CTRL_OWNS  or PEN__CTRL_DESTROY or PEN__CLONE)
				invoke GdipDeletePen,[rsi].FNC_DSCPTR.pen
			.endif
			mov rdi,[rsi].FNC_DSCPTR.pNextDscptr
			free rsi
			mov rsi,rdi
		.until !rsi
	.endif
	
	;/* free FNCDRAW_CNTRL */
	free rbx
	
	ret
	
fdc_OnDestroy endp


fdc_set_label_info proc FRAME uses rbx rsi rdi pAxesInfo: ptr AXES_INFO,pLabelInfo:PVOID,uMsg:UINT
	
	;/* free old struct */
	mov rdi,pAxesInfo
	lea rdi,[rdi].AXES_INFO.labels
	mov rsi,pLabelInfo
	assume rdi: ptr LABEL_INFO
	assume rsi: ptr LABEL_INFO

	free [rdi].pwszFontFam
	free [rdi].pwszFormatX
	free [rdi].pwszFormatY
	
	.if [rdi].flg & ( PEN__CTRL_OWNS  or PEN__CTRL_DESTROY or PEN__CLONE)
		invoke GdipDeletePen,[rdi].pen
	.endif
	
	.if [rdi].flg & ( TXTPEN__CTRL_OWNS  or TXTPEN__CTRL_DESTROY or TXTPEN__CLONE)
		invoke GdipDeletePen,[rdi].penTxt
	.endif
	
;	.if [rdi].flg & ( BRUSH__CTRL_OWNS  or BRUSH__CTRL_DESTROY or BRUSH__CLONE)
;		invoke GdipDeleteBrush,[rdi].brush
;	.endif
	
	.if [rdi].flg & ( TXTBRUSH__CTRL_OWNS  or TXTBRUSH__CTRL_DESTROY or TXTBRUSH__CLONE)
		invoke GdipDeleteBrush,[rdi].brushTxt
	.endif

	;/* copy new struct */
	invoke RtlMoveMemory,rdi,rsi,SIZEOF LABEL_INFO
	
	;/* copy strings */
	.if uMsg == FDCM_SET_LABEL_INFOA
		assume rsi: nothing
		lea rbx,[2*rv(lstrlenA,[rsi].LABEL_INFOA.pszFontFam)+2]
		mov [rdi].pwszFontFam,alloc(rbx)
    	invoke MultiByteToWideChar,CP_ACP,MB_PRECOMPOSED,[rsi].LABEL_INFOA.pszFontFam,ebx,[rdi].pwszFontFam,ebx
		
		lea rbx,[2*rv(lstrlenA,[rsi].LABEL_INFOA.pszFormatX)+2]
		mov [rdi].pwszFormatX,alloc(rbx)
    	invoke MultiByteToWideChar,CP_ACP,MB_PRECOMPOSED,[rsi].LABEL_INFOA.pszFormatX,ebx,[rdi].pwszFormatX,ebx
		
		lea rbx,[2*rv(lstrlenA,[rsi].LABEL_INFOA.pszFormatY)+2]
		mov [rdi].pwszFormatY,alloc(rbx)
    	invoke MultiByteToWideChar,CP_ACP,MB_PRECOMPOSED,[rsi].LABEL_INFOA.pszFormatY,ebx,[rdi].pwszFormatY,ebx
    	assume rsi: ptr LABEL_INFO
	.else	
		lea rax,[2*rv(lstrlenW,[rsi].pwszFontFam)+2]
		mov [rdi].pwszFontFam,alloc(rax)
		invoke lstrcpyW,[rdi].pwszFontFam,[rsi].pwszFontFam

		lea rax,[2*rv(lstrlenW,[rsi].pwszFormatX)+2]
		mov [rdi].pwszFormatX,alloc(rax)
		invoke lstrcpyW,[rdi].pwszFormatX,[rsi].pwszFormatX

		lea rax,[2*rv(lstrlenW,[rsi].pwszFormatY)+2]
		mov [rdi].pwszFormatY,alloc(rax)
		invoke lstrcpyW,[rdi].pwszFormatY,[rsi].pwszFormatY	
	.endif
	
	assume rdi: ptr LABEL_INFOW
	;/* copy brushes and pens */
	.if [rdi].flg&PEN__CLONE
		invoke GdipClonePen,[rdi].pen,ADDR [rdi].pen
	.elseif [rdi].flg&_VALUE_ARGB_ && [rdi].flg&PEN__CTRL_OWNS
		invoke GdipCreatePen1,ARGB ptr [rdi].pen,[rdi].LABEL_INFOW.pen_width,UnitWorld,ADDR [rdi].pen
	.endif
	
	.if [rdi].flg&TXTPEN__CLONE
		invoke GdipClonePen,[rdi].penTxt,ADDR [rdi].penTxt
	.elseif [rdi].flg&_VALUE_ARGB_ && [rdi].flg&TXTPEN__CTRL_OWNS
		invoke GdipCreatePen1,ARGB ptr [rdi].penTxt,[rdi].LABEL_INFOW.penTxt_width,UnitWorld,ADDR [rdi].penTxt
	.endif
	
;	.if [rdi].flg&BRUSH__CLONE
;		invoke GdipCloneBrush,[rdi].brush,ADDR [rdi].brush
;	.elseif [rdi].flg&_VALUE_ARGB_ && [rdi].flg&BRUSH__CTRL_OWNS
;		invoke GdipCreateSolidFill,[rdi].brush,ADDR [rdi].brush
;	.endif
	
	.if [rdi].flg&TXTBRUSH__CLONE
		invoke GdipCloneBrush,[rdi].brushTxt,ADDR [rdi].brushTxt
	.elseif [rdi].flg&_VALUE_ARGB_ && [rdi].flg&TXTBRUSH__CTRL_OWNS
		invoke GdipCreateSolidFill,ARGB ptr [rdi].brushTxt,ADDR [rdi].brushTxt
	.endif
	
	assume rdi: nothing
	assume rsi: nothing
	
	ret
	
fdc_set_label_info endp

WndProc proc FRAME uses rbx rsi rdi hWnd:HWND,uMsg:UINT,wParam:WPARAM,lParam:LPARAM
LOCAL ps:PAINTSTRUCT
LOCAL graphics:PVOID	
LOCAL hBkDC:HDC
LOCAL hBmp:HBITMAP
LOCAL rect:RECT
LOCAL pixpincx:DWORD,pixpincy:DWORD
LOCAL hdc:HDC
LOCAL pointf:PointF
LOCAL pointd:PointD
LOCAL fdc_nmhdr:FDC_NMHDR
LOCAL r64:QWORD
LOCAL fSlvTLS()

	.if uMsg == WM_CLOSE
		invoke DestroyWindow,hWnd
	.elseif uMsg == WM_DESTROY
		invoke fdc_OnDestroy,hWnd
	.elseif uMsg == WM_CREATE
		invoke fdc_OnCreate,hWnd,lParam
	.elseif uMsg == FDCM_SET_AXES_INFO
		mov rbx,rv(GetWindowLongPtr,hWnd,0)
		.if [rbx].FNCDRAW_CNTRL.pAxesInfo
			invoke FreeAxesInfo,[rbx].FNCDRAW_CNTRL.pAxesInfo
		.endif
		mov [rbx].FNCDRAW_CNTRL.pAxesInfo,rv(CopyAxesInfo,wParam)
		mov [rbx].FNCDRAW_CNTRL.init,1
		invoke InvalidateRect,hWnd,0,0
	.elseif uMsg == FDCM_GET_STYLE
		mov rbx,rv(GetWindowLongPtr,hWnd,0)
		mov eax,[rbx].FNCDRAW_CNTRL.dwStyle
		ret
	.elseif uMsg == FDCM_SET_STYLE
		mov rbx,rv(GetWindowLongPtr,hWnd,0)
		.if wParam & (NOT FDCS_VALID_STYLES)
			xor rax,rax
			ret
		.endif
		mov eax,DWORD ptr wParam
		mov [rbx].FNCDRAW_CNTRL.dwStyle,eax
		;invoke InvalidateRect,hWnd,0,TRUE
		mov rax,1
		ret
	.elseif uMsg == FDCM_SET_CURR_VIEW
		mov rbx,rv(GetWindowLongPtr,hWnd,0)
		mov rbx,[rbx].FNCDRAW_CNTRL.pAxesInfo
		mov rsi,wParam
		m2m [rbx].AXES_INFO.view.xMax,[rsi].CURR_VIEW.xMax
		m2m [rbx].AXES_INFO.view.xMin,[rsi].CURR_VIEW.xMin
		m2m [rbx].AXES_INFO.view.yMax,[rsi].CURR_VIEW.yMax
		m2m [rbx].AXES_INFO.view.yMin,[rsi].CURR_VIEW.yMin
		invoke calc_metrics,hWnd,rbx
		invoke scale_axes,rbx,[rbx].AXES_INFO.labels.xMultipleOf,[rbx].AXES_INFO.labels.yMultipleOf,[rbx].AXES_INFO.labels.nxMarks,[rbx].AXES_INFO.labels.nyMarks
		invoke InvalidateRect,hWnd,0,TRUE
		mov rax,1
		ret
	.elseif uMsg == FDCM_CLIENT2UNIT
		mov rbx,rv(GetWindowLongPtr,hWnd,0)
		mov rdx,wParam
		invoke ClientPoint2Unit,[rdx].POINT.x,[rdx].POINT.y,rbx,lParam
		mov rax,1
		ret
	.elseif uMsg == FDCM_UNIT2CLIENT
		invoke Unit2ClientPoint,wParam,rv(GetWindowLongPtr,hWnd,0),lParam
		mov rax,1
		ret
	.elseif uMsg == FDCM_GET_CURR_VIEW
		mov rbx,rv(GetWindowLongPtr,hWnd,0)
		mov rbx,[rbx].FNCDRAW_CNTRL.pAxesInfo
		mov rsi,wParam
		m2m [rsi].CURR_VIEW.xMax,[rbx].AXES_INFO.view.xMax
		m2m [rsi].CURR_VIEW.xMin,[rbx].AXES_INFO.view.xMin
		m2m [rsi].CURR_VIEW.yMax,[rbx].AXES_INFO.view.yMax
		m2m [rsi].CURR_VIEW.yMin,[rbx].AXES_INFO.view.yMin
		mov rax,1
		ret
	.elseif uMsg == FDCM_SET_FNC_CB
		mov rbx,rv(GetWindowLongPtr,hWnd,0)
		.if rv(IsValidFncHandle,rbx,wParam)
			mov rax,[rbx].FNC_DSCPTR.pCallBack
			m2m [rbx].FNC_DSCPTR.pCallBack,lParam
			invoke InvalidateRect,hWnd,0,TRUE
			mov rax,1
		.else
			xor rax,rax
		.endif
		ret
	.elseif uMsg == FDCM_SET_FNC_NPOINTS
		mov rbx,rv(GetWindowLongPtr,hWnd,0)
		.if rv(IsValidFncHandle,rbx,wParam) && lParam >= 2
			mov eax,DWORD ptr lParam
			mov [rbx].FNC_DSCPTR.nPoints,eax
			invoke InvalidateRect,hWnd,0,TRUE
			mov rax,1
		.else
			xor rax,rax
		.endif
		ret
	.elseif uMsg == FDCM_DEL_FNC
		mov rbx,rv(GetWindowLongPtr,hWnd,0)
		.if rv(IsValidFncHandle,rbx,wParam)
			mov rdx,wParam
			.if [rdx].FNC_DSCPTR.flg & ( PEN__CTRL_OWNS  or PEN__CTRL_DESTROY or PEN__CLONE)
				invoke GdipDeletePen,[rdx].FNC_DSCPTR.pen
				mov rdx,wParam
			.endif
			.if [rdx].FNC_DSCPTR.pPrevDscptr && [rdx].FNC_DSCPTR.pNextDscptr
				mov rax,[rdx].FNC_DSCPTR.pPrevDscptr
				m2m [rax].FNC_DSCPTR.pNextDscptr,[rdx].FNC_DSCPTR.pNextDscptr
				mov rax,[rdx].FNC_DSCPTR.pNextDscptr
				m2m [rax].FNC_DSCPTR.pPrevDscptr,[rdx].FNC_DSCPTR.pPrevDscptr
			.elseif [rdx].FNC_DSCPTR.pPrevDscptr
				mov rax,[rdx].FNC_DSCPTR.pPrevDscptr
				mov [rax].FNC_DSCPTR.pNextDscptr,0
			.else;if [rdx].FNC_DSCPTR.pNextDscptr
				mov rax,[rdx].FNC_DSCPTR.pNextDscptr
				mov [rax].FNC_DSCPTR.pPrevDscptr,0
				mov [rbx].FNCDRAW_CNTRL.pFncDscptr,rax
			.endif
			free wParam
			dec [rbx].FNCDRAW_CNTRL.nFunctions
			invoke InvalidateRect,hWnd,0,TRUE
			mov rax,1
		.else
			xor rax,rax
		.endif
		ret
	.elseif uMsg == FDCM_GET_FNC_DSCRPTR
		mov rbx,rv(GetWindowLongPtr,hWnd,0)
		.if rv(IsValidFncHandle,rbx,wParam)
			mov rdx,lParam
			mov rax,wParam
			mov ecx,[rax].FNC_DSCPTR.flg
			mov [rdx].FNC_DSCPTR.flg,ecx
			mov ecx,[rax].FNC_DSCPTR.nPoints
			mov [rdx].FNC_DSCPTR.nPoints,ecx
			m2m [rdx].FNC_DSCPTR.pCallBack,[rax].FNC_DSCPTR.pCallBack
			m2m [rdx].FNC_DSCPTR.pen,[rax].FNC_DSCPTR.pen
			mov ecx,[rax].FNC_DSCPTR._width
			mov [rdx].FNC_DSCPTR._width,ecx
			mov [rdx].FNC_DSCPTR.pNextDscptr,0
			mov [rdx].FNC_DSCPTR.pPrevDscptr,0
			mov rax,1
		.else
			xor rax,rax
		.endif
		ret
	.elseif uMsg == FDCM_SET_FNC_PEN
		mov rbx,rv(GetWindowLongPtr,hWnd,0)
		.if rv(IsValidFncHandle,rbx,wParam)
			mov rsi,wParam
			mov rdi,lParam
			.if [rsi].FNC_DSCPTR.flg & ( PEN__CTRL_OWNS  or PEN__CTRL_DESTROY or PEN__CLONE)
				invoke GdipDeletePen,[rsi].FNC_DSCPTR.pen
			.endif
			m2m [rsi].FNC_DSCPTR.pen,[rdi].FNC_DSCPTR.pen
			mov ecx,[rdi].FNC_DSCPTR.flg
			mov [rsi].FNC_DSCPTR.flg,ecx
			mov ecx,[rdi].FNC_DSCPTR._width
			mov [rsi].FNC_DSCPTR._width,ecx
			
			.if [rdi].FNC_DSCPTR.flg&PEN__CLONE
				invoke GdipClonePen,[rdi].FNC_DSCPTR.pen,ADDR [rsi].FNC_DSCPTR.pen
			.elseif [rdi].FNC_DSCPTR.flg&_VALUE_ARGB_ && [rdi].FNC_DSCPTR.flg&PEN__CTRL_OWNS
				invoke GdipCreatePen1,ARGB ptr [rdi].FNC_DSCPTR.pen,[rdi].FNC_DSCPTR._width,UnitWorld,ADDR [rsi].FNC_DSCPTR.pen
			.endif
			invoke InvalidateRect,hWnd,0,TRUE
			mov rax,1
		.else
			xor rax,rax
		.endif
		ret
	.elseif uMsg == FDCM_SET_LABEL_INFOA || uMsg == FDCM_SET_LABEL_INFOW
		mov rbx,rv(GetWindowLongPtr,hWnd,0)
		invoke fdc_set_label_info,[rbx].FNCDRAW_CNTRL.pAxesInfo,wParam,uMsg
		ret
	.elseif uMsg == FDCM_SET_BKCOLOR
		mov rbx,rv(GetWindowLongPtr,hWnd,0)
		mov ecx,DWORD ptr wParam
		mov [rbx].FNCDRAW_CNTRL.argbBkColor,ecx
		invoke InvalidateRect,hWnd,0,TRUE
		mov rax,1
		ret
	.elseif uMsg == FDCM_GET_BKCOLOR
		mov rbx,rv(GetWindowLongPtr,hWnd,0)
		mov eax,[rbx].FNCDRAW_CNTRL.argbBkColor
		ret
	.elseif uMsg == FDCM_ADD_FUNCTION
		mov rsi,wParam
		assume rsi: ptr FNC_DSCPTR
		mov rbx,rv(GetWindowLongPtr,hWnd,0)
		.if rsi && [rsi].nPoints && [rsi].pCallBack
			mov rdi,alloc(SIZEOF FNC_DSCPTR)
			assume rdi: ptr FNC_DSCPTR
			.if [rbx].FNCDRAW_CNTRL.nFunctions
				mov rdx,[rbx].FNCDRAW_CNTRL.pFncDscptr
				.while 1
					mov rax,[rdx].FNC_DSCPTR.pNextDscptr
					.break .if !rax 
					mov rdx,rax
				.endw
				mov [rdx].FNC_DSCPTR.pNextDscptr,rdi
				mov [rdi].pPrevDscptr,rdx
				inc [rbx].FNCDRAW_CNTRL.nFunctions
			.else
				mov [rbx].FNCDRAW_CNTRL.pFncDscptr,rdi
				mov [rbx].FNCDRAW_CNTRL.nFunctions,1
			.endif
			.if ![rsi].flg&PEN__VALID_
				mov [rdi].flg,PEN__DEFAULT
			.else
				mov ecx,[rsi].flg
				mov [rdi].flg,ecx
			.endif
			mov ecx,[rsi].nPoints
			mov [rdi].nPoints,ecx
			m2m [rdi].pCallBack,[rsi].pCallBack
			m2m [rdi].pen,[rsi].pen
			
			.if [rsi].flg&PEN__CLONE
				invoke GdipClonePen,[rsi].pen,ADDR [rdi].pen
			.elseif [rsi].flg&_VALUE_ARGB_ && [rsi].flg&PEN__CTRL_OWNS
				invoke GdipCreatePen1,ARGB ptr [rsi].pen,[rsi]._width,UnitWorld,ADDR [rdi].pen
			.endif
			invoke InvalidateRect,hWnd,0,TRUE
			mov rax,rdi
			ret
		.else
			xor rax,rax
			ret	
		.endif
		assume rsi: nothing
		assume rdi: nothing
	.elseif uMsg == WM_PAINT
		mov rbx,rv(GetWindowLongPtr,hWnd,0)
		invoke BeginPaint,hWnd,ADDR ps
		invoke GetClientRect,hWnd,ADDR rect
		mov hBkDC,rv(CreateCompatibleDC,ps.hdc)
		mov hBmp,rv(SelectObject,hBkDC,rv(CreateCompatibleBitmap,ps.hdc,rect.right,rect.bottom))
		
		
		invoke GdipCreateFromHDC,hBkDC,ADDR graphics
		invoke GdipSetPageUnit,graphics,UnitMillimeter
		invoke GdipSetSmoothingMode,graphics,SmoothingModeHighQuality
		
		.if [rbx].FNCDRAW_CNTRL.init
			mov rsi,rbx
			mov rbx,[rbx].FNCDRAW_CNTRL.pAxesInfo
			assume rbx: ptr AXES_INFO
			assume rsi: ptr FNCDRAW_CNTRL
						
			invoke calc_metrics,hWnd,rbx
			invoke scale_axes,rbx,[rbx].labels.xMultipleOf,[rbx].labels.yMultipleOf,[rbx].labels.nxMarks,[rbx].labels.nyMarks
			
			invoke GdipGraphicsClear,graphics,[rsi].argbBkColor
			
			mov r64,rbx
			mov rbx,[rsi].pFncDscptr
			assume rbx: ptr FNC_DSCPTR
			xor edi,edi
			.while edi < [rsi].nFunctions && rbx
				invoke plot,graphics,[rbx].pen,[rbx].nPoints,[rsi].pAxesInfo,[rbx].pCallBack
				mov rbx,[rbx].pNextDscptr
				inc edi
			.endw
			mov rbx,r64

			invoke draw_axes,graphics,rbx
			
			assume rbx: nothing
			assume rsi: nothing
		.endif
		
		invoke BitBlt,ps.hdc,0,0,rect.right,rect.bottom,hBkDC,0,0,SRCCOPY

		invoke DeleteObject,rv(SelectObject,hBkDC,hBmp)
		invoke DeleteDC,hBkDC

		invoke GdipDeleteGraphics,graphics
		invoke EndPaint,hWnd,ADDR ps		
	
	.elseif uMsg == WM_LBUTTONDOWN
		mov rbx,rv(GetWindowLongPtr,hWnd,0)
		mov ecx,DWORD ptr lParam
		mov [rbx].FNCDRAW_CNTRL.MousePos,ecx
		mov [rbx].FNCDRAW_CNTRL.bMouseDown,1
		invoke SetCapture,hWnd
	.elseif uMsg == WM_LBUTTONUP
		invoke ReleaseCapture
		mov rbx,rv(GetWindowLongPtr,hWnd,0)
		mov [rbx].FNCDRAW_CNTRL.bMouseDown,0
	.elseif uMsg == WM_MOUSEMOVE
		mov rbx,rv(GetWindowLongPtr,hWnd,0)
		mov rsi,rbx

		.if wParam & MK_LBUTTON && [rsi].FNCDRAW_CNTRL.bMouseDown
			mov rbx,[rbx].FNCDRAW_CNTRL.pAxesInfo
			assume rbx: ptr AXES_INFO
			
			mov hdc,rv(GetDC,0)
			fn GetDeviceCaps,hdc,LOGPIXELSX
			mov pixpincx,eax
			fn GetDeviceCaps,hdc,LOGPIXELSY
			mov pixpincy,eax
			invoke ReleaseDC,0,hdc

			fSlv [rbx].view.xMin = [rbx].view.xMin - (WORD ptr lParam[0]-[rsi].FNCDRAW_CNTRL.MousePos.X) * (25.4 / pixpincx) / [rbx].metrics.mmpux
			fSlv [rbx].view.xMax = [rbx].view.xMax - (WORD ptr lParam[0]-[rsi].FNCDRAW_CNTRL.MousePos.X) * (25.4 / pixpincx) / [rbx].metrics.mmpux 
			fSlv [rbx].view.yMin = [rbx].view.yMin + (WORD ptr lParam[2]-[rsi].FNCDRAW_CNTRL.MousePos.Y) * (25.4 / pixpincy) / [rbx].metrics.mmpuy 
			fSlv [rbx].view.yMax = [rbx].view.yMax + (WORD ptr lParam[2]-[rsi].FNCDRAW_CNTRL.MousePos.Y) * (25.4 / pixpincy) / [rbx].metrics.mmpuy 
			mov ecx,DWORD ptr lParam
			mov [rsi].FNCDRAW_CNTRL.MousePos,ecx
			invoke calc_metrics,hWnd,rbx
			invoke InvalidateRect,hWnd,0,0
			
			assume rbx: nothing
		.endif

		.if [rsi].FNCDRAW_CNTRL.dwStyle & FDCS_MOUSE_NOTIFICATION
			movzx ecx,WORD ptr lParam
			movzx edx,WORD ptr lParam+2
			mov fdc_nmhdr.point.x,ecx
			mov fdc_nmhdr.point.y,edx
			mov rbx,[rsi].FNCDRAW_CNTRL.pAxesInfo
			invoke ClientPoint2Unit,ecx,edx,rbx,ADDR fdc_nmhdr.pointd
			m2m fdc_nmhdr.nmhdr.hwndFrom,hWnd
			m2m fdc_nmhdr.nmhdr.idFrom,rv(GetWindowLongPtr,hWnd,GWL_ID)
			mov ecx,DWORD ptr wParam
			mov fdc_nmhdr.fwKeys,ecx
			mov r64,rax
			mov fdc_nmhdr.nmhdr.code,FDCNM_MOUSE_MOVE
			mov rdx,rv(GetParent,hWnd)			
			invoke SendMessage,rdx,WM_NOTIFY,r64,ADDR fdc_nmhdr
		.endif
	.elseif uMsg == WM_MOUSEWHEEL
		mov rbx,rv(GetWindowLongPtr,hWnd,0)
		.if [rbx].FNCDRAW_CNTRL.init
			.if [rbx].FNCDRAW_CNTRL.zoomFlg ;& ZOOM_CENTER
				mov rsi,rbx
				mov rbx,[rbx].FNCDRAW_CNTRL.pAxesInfo
				assume rbx: ptr AXES_INFO
				
				fSlvVolatileXmmRegs remove,xmm0,xmm1
				
				fSlv xmm1 = abs([rbx].view.xMax-[rbx].view.xMin)*0.1
				fSlv xmm0 = abs([rbx].view.yMax-[rbx].view.yMin)*0.1
				.if SWORD ptr wParam+2 < 0
					.if !(WORD ptr wParam & MK_CONTROL)
						fSlv [rbx].view.xMin = [rbx].view.xMin - xmm1
						fSlv [rbx].view.xMax = [rbx].view.xMax + xmm1
					.endif
					.if !(WORD ptr wParam & MK_SHIFT)	
						fSlv [rbx].view.yMin = [rbx].view.yMin - xmm0
						fSlv [rbx].view.yMax = [rbx].view.yMax + xmm0
					.endif
				.else
					.if !(WORD ptr wParam & MK_CONTROL)
						fSlv [rbx].view.xMin = [rbx].view.xMin + xmm1
						fSlv [rbx].view.xMax = [rbx].view.xMax - xmm1
					.endif
					.if !(WORD ptr wParam & MK_SHIFT)
						fSlv [rbx].view.yMin = [rbx].view.yMin + xmm0
						fSlv [rbx].view.yMax = [rbx].view.yMax - xmm0
					.endif
				.endif
				
				fSlvVolatileXmmRegs default
				
				.if [rsi].FNCDRAW_CNTRL.dwStyle & FDCS_MOUSE_NOTIFICATION
					invoke calc_metrics,hWnd,rbx
					invoke GetCursorPos,ADDR fdc_nmhdr.point
					invoke ScreenToClient,hWnd,ADDR fdc_nmhdr.point
					invoke ClientPoint2Unit,fdc_nmhdr.point.x,fdc_nmhdr.point.y,rbx,ADDR fdc_nmhdr.pointd
					m2m fdc_nmhdr.nmhdr.hwndFrom,hWnd
					movzx eax,WORD ptr wParam
					mov fdc_nmhdr.fwKeys,eax
					mov fdc_nmhdr.nmhdr.idFrom,rv(GetWindowLongPtr,hWnd,GWL_ID)
					mov r64,rax
					mov fdc_nmhdr.nmhdr.code,FDCNM_MOUSE_MOVE
					mov rdx,rv(GetParent,hWnd)			
					invoke SendMessage,rdx,WM_NOTIFY,r64,ADDR fdc_nmhdr
				.endif
				assume rbx: nothing
			.endif
			
			invoke InvalidateRect,hWnd,0,0
			
		.endif
	.elseif uMsg == WM_LBUTTONDBLCLK
		mov rbx,rv(GetWindowLongPtr,hWnd,0)
		.if [rbx].FNCDRAW_CNTRL.init
			mov rbx,[rbx].FNCDRAW_CNTRL.pAxesInfo
			assume rbx: ptr AXES_INFO
			movzx ecx,WORD ptr lParam
			movzx edx,WORD ptr lParam+2
			
			invoke pix2mm,ecx,edx,ADDR pointd
			.if rv(ApproxEqual,@fSlv8([rbx].metrics.xAxe+[rbx].metrics.rectf.y),pointd.y,1.0)
				.if [rbx].labels.flgXlbl & ALP_CENTER_BOTTOM
					mov [rbx].labels.flgXlbl,ALP_QDRNT_2
				.elseif [rbx].labels.flgXlbl & ALP_QDRNT_2
					mov [rbx].labels.flgXlbl,ALP_QDRNT_1
				.elseif [rbx].labels.flgXlbl & ALP_QDRNT_1
					mov [rbx].labels.flgXlbl,ALP_CENTER_TOP
				.elseif [rbx].labels.flgXlbl & ALP_CENTER_TOP
					mov [rbx].labels.flgXlbl,ALP_QDRNT_0
				.elseif [rbx].labels.flgXlbl & ALP_QDRNT_0
					mov [rbx].labels.flgXlbl,ALP_QDRNT_3
				.elseif [rbx].labels.flgXlbl & ALP_QDRNT_3
					mov [rbx].labels.flgXlbl,ALP_CENTER_BOTTOM
				.endif
				invoke InvalidateRect,hWnd,0,0
				xor rax,rax
				ret
			.endif
			.if rv(ApproxEqual,@fSlv8([rbx].metrics.yAxe+[rbx].metrics.rectf.x),pointd.x,1.0)
				.if [rbx].labels.flgYlbl & ALP_CENTER_LEFT
					mov [rbx].labels.flgYlbl,ALP_QDRNT_1
				.elseif [rbx].labels.flgYlbl & ALP_QDRNT_1
					mov [rbx].labels.flgYlbl,ALP_QDRNT_0
				.elseif [rbx].labels.flgYlbl & ALP_QDRNT_0
					mov [rbx].labels.flgYlbl,ALP_CENTER_RIGHT
				.elseif [rbx].labels.flgYlbl & ALP_CENTER_RIGHT
					mov [rbx].labels.flgYlbl,ALP_QDRNT_3
				.elseif [rbx].labels.flgYlbl & ALP_QDRNT_3
					mov [rbx].labels.flgYlbl,ALP_QDRNT_2
				.elseif [rbx].labels.flgYlbl & ALP_QDRNT_2
					mov [rbx].labels.flgYlbl,ALP_CENTER_LEFT
				.endif
				invoke InvalidateRect,hWnd,0,0
			.endif
			assume rbx: nothing
		.endif
	.else
		invoke DefWindowProc,hWnd,uMsg,wParam,lParam
		ret
	.endif

	xor rax,rax
	ret
	
WndProc endp


Unit2ClientPoint proc FRAME uses rbx pPointD: ptr PointD,pAxesInfo: ptr AXES_INFO,pPoint: ptr POINT
	
	mov rbx,pAxesInfo
	assume rbx: ptr AXES_INFO
	mov rdx,pPointD
	
	fSlv [rdx].PointD.x = ([rdx].PointD.x - [rbx].view.xMin)*[rbx].metrics.mmpux + [rbx].metrics.rectf.x
	fSlv [rdx].PointD.y = ([rdx].PointD.y + [rbx].view.yMax)*[rbx].metrics.mmpuy + [rbx].metrics.rectf.y

	invoke mm2pix,rdx,pPoint
	
	assume rbx: nothing
	ret
	
Unit2ClientPoint endp



ClientPoint2Unit proc FRAME uses rbx x:DWORD,y:DWORD,pAxesInfo: ptr AXES_INFO,pPointD: ptr PointD

	mov rbx,pAxesInfo
	assume rbx: ptr AXES_INFO
	
	;/* pointf  to millimeters */
	invoke pix2mm,x,y,pPointD
	mov rdx,pPointD
	
	fSlv [rdx].PointD.x = [rbx].view.xMin + ([rdx].PointD.x - [rbx].metrics.rectf.x)/[rbx].metrics.mmpux
	fSlv [rdx].PointD.y = [rbx].view.yMax - ([rdx].PointD.y - [rbx].metrics.rectf.y)/[rbx].metrics.mmpuy
	
	mov eax,1
	assume rbx: nothing	
	ret
	
ClientPoint2Unit endp

;/* returns client size in millimerters */
GdipGetClientRectF proc FRAME hWnd:HWND,pRectF:ptr RectF
LOCAL pixpincHorz:SDWORD,pixpincVert:SDWORD
LOCAL hdc:HDC

	mov hdc,rv(GetDC,0)
	invoke GetClientRect,hWnd,pRectF
	fn GetDeviceCaps,hdc,LOGPIXELSX
	mov pixpincHorz,eax
	fn GetDeviceCaps,hdc,LOGPIXELSY
	mov pixpincVert,eax
	invoke ReleaseDC,0,hdc
	
	mov rdx,pRectF
	fSlv [rdx].RectF._Width = SDWORD ptr [rdx].RectF._Width * 25.4 / pixpincHorz
	fSlv [rdx].RectF.Height = SDWORD ptr [rdx].RectF.Height * 25.4 / pixpincVert 
	ret
	
GdipGetClientRectF endp

pix2mm proc FRAME x:DWORD,y:DWORD,pPointD: ptr PointD
LOCAL pixpincHorz:SDWORD,pixpincVert:SDWORD
LOCAL hdc:HDC

	mov hdc,rv(GetDC,0)
	fn GetDeviceCaps,hdc,LOGPIXELSX
	mov pixpincHorz,eax
	fn GetDeviceCaps,hdc,LOGPIXELSY
	mov pixpincVert,eax
	invoke ReleaseDC,0,hdc
	
	mov rdx,pPointD
	
	fSlv [rdx].PointD.x = x * 25.4 / pixpincHorz
	fSlv [rdx].PointD.y = y * 25.4 / pixpincVert
	
	ret
	
pix2mm endp

mm2pix proc FRAME pPointD: ptr PointD,pPoint: ptr POINT
LOCAL pixpincHorz:SDWORD,pixpincVert:SDWORD
LOCAL hdc:HDC

	mov hdc,rv(GetDC,0)
	fn GetDeviceCaps,hdc,LOGPIXELSX
	mov pixpincHorz,eax
	fn GetDeviceCaps,hdc,LOGPIXELSY
	mov pixpincVert,eax
	invoke ReleaseDC,0,hdc
	
	mov rdx,pPointD
	mov rax,pPoint
	
	fSlv [rdx].POINT.x = [rax].PointD.x * 0.03937 * pixpincHorz
	fSlv [rdx].POINT.y = [rax].PointD.y * 0.03937 * pixpincVert
	
	ret
	
mm2pix endp

ApproxEqual proc FRAME fValue1:REAL8,fValue2:REAL8,fRange:REAL8
	
	fSlvRemoveFlags FSF_USE_SSE2
	
	xor rax,rax
	fSlv = abs( fValue1 - fValue2 )
	fSlv = fRange
	fcomip st,st(1)
	fstp st
	setae al	
	
	fSlvSetFlags FSF_USE_SSE2
	
	ret
	
ApproxEqual endp

calc_metrics proc FRAME uses rbx hWnd:HWND,pAxesInfo:ptr AXES_INFO
LOCAL fSlvTLS()
	mov rbx,pAxesInfo
	assume rbx: ptr AXES_INFO

	.if fGT([rbx].view.xMin,[rbx].view.xMax)
		mov rax,[rbx].view.xMax
		mov rcx,[rbx].view.xMin
		xchg rax,rcx
		mov [rbx].view.xMax,rax
		mov [rbx].view.xMin,rcx
	.endif
	
	.if fGT([rbx].view.yMin,[rbx].view.yMax)
		mov rax,[rbx].view.yMax
		mov rcx,[rbx].view.yMin
		xchg rax,rcx
		mov [rbx].view.yMax,rax
		mov [rbx].view.yMin,rcx
	.endif
	
	invoke GdipGetClientRectF,hWnd,ADDR [rbx].metrics.rectf
	
	fSlv [rbx].metrics.rectf.x = [rbx].metrics.rectf.x + [rbx].labels._frame.left
	fSlv [rbx].metrics.rectf.y = [rbx].metrics.rectf.y + [rbx].labels._frame.top
	fSlv [rbx].metrics.rectf._Width = [rbx].metrics.rectf._Width - ([rbx].labels._frame.left+[rbx].labels._frame.right)
	fSlv [rbx].metrics.rectf.Height = [rbx].metrics.rectf.Height - ([rbx].labels._frame.top + [rbx].labels._frame.bottom)

	fSlv [rbx].metrics.mmpux = [rbx].metrics.rectf._Width/([rbx].view.xMax-[rbx].view.xMin)
	fSlv [rbx].metrics.mmpuy = [rbx].metrics.rectf.Height/([rbx].view.yMax-[rbx].view.yMin)
	fSlv [rbx].metrics.xOrgin = [rbx].metrics.rectf.x - [rbx].view.xMin * [rbx].metrics.mmpux
	fSlv [rbx].metrics.yOrgin = [rbx].view.yMax * [rbx].metrics.mmpuy + [rbx].metrics.rectf.y

	.if fGE([rbx].view.xMax,0) && fLE([rbx].view.xMin,0)
		fSlv [rbx].metrics.yAxe = abs([rbx].view.xMin)*[rbx].metrics.mmpux
	.elseif DWORD ptr [rbx].view.xMax+4 & 80000000h
		fSlv [rbx].metrics.yAxe = [rbx].metrics.rectf._Width
	.else
		fSlv [rbx].metrics.yAxe = 0
	.endif
	
	.if fGE([rbx].view.yMax,0) && fLE([rbx].view.yMin,0)
		fSlv [rbx].metrics.xAxe = ([rbx].view.yMax-[rbx].view.yMin)*[rbx].metrics.mmpuy - abs([rbx].view.yMin)*[rbx].metrics.mmpuy
	.elseif DWORD ptr [rbx].view.yMax+4 & 80000000h
		fSlv [rbx].metrics.xAxe = 0
	.else
		fSlv [rbx].metrics.xAxe = [rbx].metrics.rectf.Height
	.endif
	
	assume rbx: ptr nothing
	
	ret
	
calc_metrics endp

scale_axes proc FRAME uses rbx pAxesInfo: ptr AXES_INFO,xMultipleOf:REAL8,yMultipleOf:REAL8,nMarksX:DWORD,nMarksY:DWORD
LOCAL xmd:REAL8
LOCAL ymd:REAL8
LOCAL fSlvTLS()

	mov rbx,pAxesInfo
	assume rbx: ptr AXES_INFO
	
	.if !nMarksX || !nMarksY
		xor rax,rax
		ret
	.endif
	
	fSlv xmd = ([rbx].view.xMax - [rbx].view.xMin)/nMarksX
	fSlv ymd = ([rbx].view.yMax - [rbx].view.yMin)/nMarksY
	
	fSlv [rbx].labels.xMarkDist = xmd ;trunc(([rbx].view.xMax - [rbx].view.xMin)/xMultipleOf/nMarksX)*xMultipleOf
	fSlv [rbx].labels.yMarkDist = ymd ;trunc(([rbx].view.yMax - [rbx].view.yMin)/yMultipleOf/nMarksY)*yMultipleOf
	
	.if !DWORD ptr [rbx].labels.xMarkDist && !DWORD ptr [rbx].labels.xMarkDist+4
		fSlv [rbx].labels.xMarkDist = xMultipleOf
	.endif
	
	.if !DWORD ptr [rbx].labels.yMarkDist && !DWORD ptr [rbx].labels.yMarkDist+4
		fSlv [rbx].labels.yMarkDist = yMultipleOf
	.endif	
	
	assume rbx: nothing
	
	ret
		
scale_axes endp

align 16
plot proc FRAME uses rbx rsi rdi graphics:PVOID,pen:PVOID,nPoints:DWORD,pAxesInfo: ptr AXES_INFO,pfnCallBack:ptr CB_FNC_VALUE
LOCAL pitch:REAL8
LOCAL x:REAL8,y:REAL8
LOCAL dy:REAL8
LOCAL fSlvTLS()

	mov rbx,pAxesInfo
	assume rbx: ptr AXES_INFO
	
	.if !nPoints
		xor rax,rax
		ret
	.elseif nPoints < 2
		mov nPoints,2
	.endif

	fSlv dy = [rbx].view.yMax - [rbx].view.yMin
	fSlv [rbx].metrics.mmpux = [rbx].metrics.rectf._Width/([rbx].view.xMax-[rbx].view.xMin)
	fSlv [rbx].metrics.mmpuy = [rbx].metrics.rectf.Height/dy
	fSlv [rbx].metrics.xOrgin = [rbx].metrics.rectf.x - [rbx].view.xMin * [rbx].metrics.mmpux
	fSlv [rbx].metrics.yOrgin = [rbx].view.yMax * [rbx].metrics.mmpuy + [rbx].metrics.rectf.y

	fSlv pitch =  ([rbx].view.xMax - [rbx].view.xMin) / (nPoints-1)
	m2m x,[rbx].view.xMin
	mov edx,nPoints
	lea rdx,[rdx*SIZEOF PointF]
	mov rdi,alloc(rdx)
	assume rdi: ptr PointF
	xor rsi,rsi
	.while esi < nPoints
		fSlv [rdi+rsi*SIZEOF PointF].x = [rbx].metrics.xOrgin + x*[rbx].metrics.mmpux

		invoke pfnCallBack,x,ADDR y
		.if fLT(y,@fSlv8([rbx].view.yMin - dy*1.5)) 
			fSlv [rdi+rsi*SIZEOF PointF].y = [rbx].metrics.yOrgin - ([rbx].view.yMin - dy*1.5) * [rbx].metrics.mmpuy
		.else
			.if fGT(y,@fSlv8([rbx].view.yMax + dy*1.5))
				fSlv [rdi+rsi*SIZEOF PointF].y = [rbx].metrics.yOrgin - ( [rbx].view.yMax + dy*1.5) * [rbx].metrics.mmpuy
			.else
				fSlv [rdi+rsi*SIZEOF PointF].y = [rbx].metrics.yOrgin - y * [rbx].metrics.mmpuy
			.endif
		.endif
		
		fSlv x = x + pitch
		inc rsi
	.endw
	
	invoke GdipDrawLines,graphics,pen,rdi,nPoints
	
	free rdi
	
	assume rdx: nothing
	assume rbx: nothing
	mov rax,1
	ret
	
plot endp



draw_label proc FRAME uses rbx IsYAxe:DWORD,path:PVOID,fontFam:PVOID,strFormat:PVOID,xPos:REAL4,yPos:REAL4,Value:REAL8,pAxesInfo: ptr AXES_INFO
LOCAL wsz[64]:WCHAR 
LOCAL rectf:RectF
LOCAL pathTmp:PVOID
LOCAL cc:DWORD
LOCAL chrsWidth:REAL4
LOCAL qdrnt:DWORD
LOCAL flg:DWORD
LOCAL fSlvTLS()

	mov rbx,pAxesInfo
	assume rbx: ptr AXES_INFO

	mov rectf.x,0
	mov rectf.y,0
	mov rectf._Width,0
	mov rectf.Height,0
	mov qdrnt,0
	
	invoke GdipCreatePath,FillModeAlternate,ADDR pathTmp
	.if !IsYAxe
		mov rdx,[rbx].labels.pwszFormatX
	.else
		mov rdx,[rbx].labels.pwszFormatY
	.endif
	fn swprintf,&wsz,rdx,Value
	mov cc,eax
	invoke GdipAddPathString,pathTmp,ADDR wsz, cc,fontFam,0,[rbx].labels.emHeight,ADDR rectf,strFormat
	invoke GdipGetPathWorldBounds,pathTmp,ADDR rectf,0,0
	invoke GdipDeletePath,pathTmp
	fSlv chrsWidth = rectf._Width
	
	;/* draw label for x-axes */
	.if !IsYAxe
		mov eax,[rbx].labels.flgXlbl
		mov flg,eax
		.if [rbx].labels.flgXlbl&ALP_CENTER_BOTTOM
	@@:		fSlv rectf.x = xPos - chrsWidth*0.5 - rectf.x
			fSlv rectf.y = yPos + [rbx].labels.markSize*1.5 
		.elseif [rbx].labels.flgXlbl&ALP_CENTER_TOP
			fSlv rectf.x = xPos - chrsWidth*0.5 - rectf.x
			fSlv rectf.y = yPos - [rbx].labels.markSize*1.5 - [rbx].labels.emHeight
		.elseif [rbx].labels.flgXlbl&(ALP_QDRNT_0 or  ALP_QDRNT_1 or ALP_QDRNT_2 or ALP_QDRNT_3)
			mov qdrnt,1
		.else
			jmp @B
	@@:	.endif
		
		.if [rbx].labels.flgXlbl&ALP_USE_X_OFFSETS
			fSlv rectf.x = rectf.x + [rbx].labels.xLblOffX
			fSlv rectf.y = rectf.y + [rbx].labels.xLblOffY
		.endif
	.else
		mov eax,[rbx].labels.flgXlbl
		mov flg,eax
		.if [rbx].labels.flgYlbl&ALP_CENTER_RIGHT
			fSlv rectf.x = xPos + [rbx].labels.markSize*1.5 - rectf.x
			fSlv rectf.y = yPos - [rbx].labels.emHeight*0.5 - rectf.y*0.5
		.elseif [rbx].labels.flgYlbl&ALP_CENTER_LEFT
	@@:		fSlv rectf.x = xPos - [rbx].labels.markSize*1.5 - chrsWidth - rectf.x
			fSlv rectf.y = yPos - [rbx].labels.emHeight*0.5 - rectf.y*0.5
		.elseif [rbx].labels.flgYlbl&(ALP_QDRNT_0 or  ALP_QDRNT_1 or ALP_QDRNT_2 or ALP_QDRNT_3)
			mov qdrnt,1
		.else
			jmp @B	
	@@:	.endif
		
		.if [rbx].labels.flgYlbl&ALP_USE_Y_OFFSETS
			fSlv rectf.x = rectf.x + [rbx].labels.yLblOffX
			fSlv rectf.y = rectf.y + [rbx].labels.yLblOffY
		.endif
	.endif

	.if qdrnt
		.if flg&ALP_QDRNT_0
			fSlv rectf.x = xPos + [rbx].labels.markSize
			fSlv rectf.y = yPos - [rbx].labels.markSize - [rbx].labels.emHeight
		.elseif flg&ALP_QDRNT_1
			fSlv rectf.x = xPos - chrsWidth - rectf.x - [rbx].labels.markSize
			fSlv rectf.y = yPos - [rbx].labels.markSize - [rbx].labels.emHeight
		.elseif flg&ALP_QDRNT_2
			fSlv rectf.x = xPos - chrsWidth - rectf.x - [rbx].labels.markSize
			fSlv rectf.y = yPos + [rbx].labels.markSize 
		.elseif flg&ALP_QDRNT_3
			fSlv rectf.x = xPos + [rbx].labels.markSize
			fSlv rectf.y = yPos + [rbx].labels.markSize
		.endif
	.endif
	
	mov rectf._Width,0
	mov rectf.Height,0
	fn GdipAddPathString,path,ADDR wsz,cc,fontFam,0,[rbx].labels.emHeight,ADDR rectf,strFormat
	
	assume rbx: nothing
	mov eax,1
	ret
	
draw_label endp


draw_axes proc FRAME uses rbx graphics:PVOID,pAxesInfo: ptr AXES_INFO
LOCAL mmpux:REAL8,mmpuy:REAL8
LOCAL xCenter:REAL8,yCenter:REAL8
LOCAL xPos:REAL4,xVal:REAL8
LOCAL yPos:REAL4,yVal:REAL8
LOCAL matrix:PVOID
LOCAL fontFam:PVOID
LOCAL strFormat:PVOID
LOCAL path:PVOID
LOCAL rectf:RectF
LOCAL wsz[64]:WORD
LOCAL fSlvTLS()
	
	mov rbx,pAxesInfo
	assume rbx: ptr AXES_INFO
	
	invoke GdipCreateMatrix,ADDR matrix
	invoke GdipGetWorldTransform,graphics,matrix
	invoke GdipTranslateWorldTransform,graphics,[rbx].metrics.rectf.x,[rbx].metrics.rectf.y,MatrixOrderAppend
	
	fn GdipCreateFontFamilyFromName,[rbx].labels.pwszFontFam,0,ADDR fontFam
	invoke GdipCreateStringFormat,0,LANG_NEUTRAL,ADDR strFormat
	invoke GdipCreatePath,FillModeAlternate,ADDR path

	mov rectf._Width,0
	mov rectf.Height,0
	
	;/* draw y-axis */
	invoke GdipDrawLine,graphics,[rbx].labels.pen,[rbx].metrics.yAxe,0.0,[rbx].metrics.yAxe,[rbx].metrics.rectf.Height
	
	;/* draw x-axis */
	invoke GdipDrawLine,graphics,[rbx].labels.pen,0.0,[rbx].metrics.xAxe,[rbx].metrics.rectf._Width,[rbx].metrics.xAxe
	
	;/* draw x-axis scale */
	.if [rbx].labels.xMarkDist
		.if  rv(ApproxEqual,@fSlv8(abs(mod([rbx].view.xMin,[rbx].labels.xMarkDist))),[rbx].labels.xMarkDist,@fSlv8(1.E-3*[rbx].labels.xMarkDist))
			fSlv xVal = [rbx].view.xMin
			fSlv xPos = 0
		.else
			.if DWORD ptr [rbx].view.xMin+4 & 80000000h
				fSlv xVal = [rbx].view.xMin - mod([rbx].view.xMin,[rbx].labels.xMarkDist)
				fSlv xPos = -mod([rbx].view.xMin,[rbx].labels.xMarkDist)*[rbx].metrics.mmpux
			.else
				fSlv xVal = [rbx].view.xMin + ([rbx].labels.xMarkDist-mod([rbx].view.xMin,[rbx].labels.xMarkDist))
				fSlv xPos = ([rbx].labels.xMarkDist-mod([rbx].view.xMin,[rbx].labels.xMarkDist))*[rbx].metrics.mmpux
			.endif
		.endif
		
		.while 1
			.break .if fGT(xVal,@fSlv8([rbx].view.xMax+[rbx].labels.xMarkDist*1.E-3))

			;/* do not draw at point of origin */
			.if !rv(ApproxEqual,xVal,r8=0,r8=[rbx].labels.xMarkDist*1.E-3)
				invoke GdipDrawLine,graphics,[rbx].labels.pen,xPos,@fSlv4([rbx].metrics.xAxe-[rbx].labels.markSize),xPos,@fSlv4([rbx].metrics.xAxe+[rbx].labels.markSize)
				invoke draw_label,0,path,fontFam,strFormat,xPos,[rbx].metrics.xAxe,xVal,rbx				
			.endif
			fSlv xVal = xVal + [rbx].labels.xMarkDist
			fSlv xPos = xPos + [rbx].labels.xMarkDist * [rbx].metrics.mmpux
		.endw
	.endif
	
	;/* draw y-axis scale */
	.if [rbx].labels.yMarkDist
		.if rv(ApproxEqual,@fSlv8(abs(mod([rbx].view.yMin,[rbx].labels.yMarkDist))),[rbx].labels.yMarkDist,@fSlv8(1.E-3*[rbx].labels.yMarkDist))
			fSlv yVal = [rbx].view.yMin
			fSlv yPos = ([rbx].view.yMax - [rbx].view.yMin) * [rbx].metrics.mmpuy
		.else 
			.if DWORD ptr [rbx].view.yMin+4 & 80000000h
				fSlv yVal = [rbx].view.yMin - mod([rbx].view.yMin,[rbx].labels.yMarkDist)
				fSlv yPos = ([rbx].view.yMax-[rbx].view.yMin)*[rbx].metrics.mmpuy + mod([rbx].view.yMin,[rbx].labels.yMarkDist)*[rbx].metrics.mmpuy
			.else
				fSlv yVal = [rbx].view.yMin + ([rbx].labels.yMarkDist-mod([rbx].view.yMin,[rbx].labels.yMarkDist))
				fSlv yPos = ([rbx].view.yMax-[rbx].view.yMin)*[rbx].metrics.mmpuy - ([rbx].labels.yMarkDist-mod([rbx].view.yMin,[rbx].labels.yMarkDist))*[rbx].metrics.mmpuy
			.endif
		.endif

		.while 1
			.break .if fGT(yVal,@fSlv8([rbx].view.yMax+[rbx].labels.yMarkDist*1.E-3))
			
			;/* do not draw at point of origin */
			.if !rv(ApproxEqual,yVal,r8=0,r8=[rbx].labels.yMarkDist*1.E-3)
				invoke GdipDrawLine,graphics,[rbx].labels.pen,@fSlv4([rbx].metrics.yAxe-[rbx].labels.markSize),yPos,@fSlv4([rbx].metrics.yAxe+[rbx].labels.markSize),yPos
				invoke draw_label,1,path,fontFam,strFormat,[rbx].metrics.yAxe,yPos,yVal,rbx
			.endif
			
			fSlv yVal = yVal + [rbx].labels.yMarkDist
			fSlv yPos = yPos - [rbx].labels.yMarkDist  * [rbx].metrics.mmpuy
		.endw
	.endif
	
	.if [rbx].labels.penTxt
		invoke GdipDrawPath,graphics,[rbx].labels.penTxt,path
	.endif
	
	.if [rbx].labels.brushTxt
		invoke GdipFillPath,graphics,[rbx].labels.brushTxt,path
	.endif
	
	invoke GdipDeleteFontFamily,fontFam
	invoke GdipDeleteStringFormat,strFormat
	invoke GdipDeletePath,path	
	
	invoke GdipSetWorldTransform,graphics,matrix
	invoke GdipDeleteMatrix,matrix
	
	assume rbx: nothing
	mov eax,1
	ret
	
draw_axes endp
fSlvStatistics
end DllMain
