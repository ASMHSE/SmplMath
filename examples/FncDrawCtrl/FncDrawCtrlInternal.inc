

PEN__VALID_				EQU (PEN__CTRL_OWNS OR PEN__SHARED OR PEN__CTRL_DESTROY OR PEN__CLONE OR PEN__ARGB)
BRUSH_VALID_			EQU (BRUSH__CTRL_OWNS OR BRUSH__SHARED OR BRUSH__CTRL_DESTROY OR BRUSH__CLONE OR BRUSH__ARGB)
FNCFLG_VALID_			EQU (PEN__VALID_ OR FNCFLG_VECTORIAL)

FNC_DSCPTR_LL struct 8
					FNC_DSCPTR		<>			
	pNextDscptr		PFNC_DSCPTR		?
	pPrevDscptr		PFNC_DSCPTR		?
FNC_DSCPTR_LL ends
PFNC_DSCPTR_LL typedef ptr FNC_DSCPTR_LL


FDCM_SET_AXES_INFO		EQU WM_USER+1			;* wParam = ptr AXES_INFO 	, lParam = reserved, must be 0

LABEL_INFO struct 8
	flg			DWORD		?
	pen 		PVOID		?
	penTxt		PVOID		?
	brushTxt	PVOID		?
	emHeight	REAL4		?
	pwszFontFam	PWCHAR		?
	pwszFormatX	PWCHAR		?
	pwszFormatY	PWCHAR		?

	flgXlbl		DWORD		?
	flgYlbl		DWORD		?

	xMarkDist	REAL8 		?
	yMarkDist	REAL8 		?

	xMultipleOf	REAL8		?
	yMultipleOf	REAL8		?

	nxMarks		DWORD		?
	nyMarks		DWORD		?

	frame		INNER_FRAME	<>

	xLblOffX	REAL4		?
	xLblOffY	REAL4		?
	yLblOffX	REAL4		?
	yLblOffY	REAL4		?
	
	markSize	REAL4		?
LABEL_INFO ends


METRICS struct 8
	rectf		RectF 	<>		; layout rect (millimeters)
	mmpux		REAL8	?		; millimeters per unit for x-coordinates
	mmpuy		REAL8	?		;     "        "    "	"  y-coordinates
	xOrgin		REAL8	?		; virtual corrd. of orgin
	yOrgin		REAL8	?		; 

	xAxe		REAL4	?		; vertical pos. of x-Axe   (mm, absolutX = xAxe + METRICS.rectf.y ) 
	yAxe		REAL4	?		; horizontal pos. of y-Axe (mm, absolutY = yAxe + METRICS.rectf.x )
METRICS ends


AXES_INFO struct 8
	flags		DWORD	?
	view		CURR_VIEW	<>
	labels		LABEL_INFO	<>
	metrics		METRICS		<>
AXES_INFO ends
PAXES_INFO typedef ptr AXES_INFO

ZOOM_INFO struct 8
	flgs	DWORD	?
	factor	REAL4	?
ZOOM_INFO ends
PZOOM_INFO typedef ptr ZOOM_INFO

ZOOM_CENTER			EQU	1

FNCDRAW_CNTRL struct
	dwSize		DWORD 			?			; = sizeof(FNCDRAW_CNTRL)
	dwStyle		DWORD			?
	init		DWORD			?			; is ready to draw?
	pAxesInfo	PAXES_INFO		?
	argbBkColor	DWORD			?
	zoomFactor	REAL4			?
	zoomFlg		DWORD			?
	nFunctions	DWORD			?
	pFncDscptr	PFNC_DSCPTR_LL	?
	MousePos	COORD			<>
	bMouseDown	DWORD			?
	hWheelHook	HHOOK			?
FNCDRAW_CNTRL ends
PFNCDRAW_CNTRL typedef ptr FNCDRAW_CNTRL


AXES_INFO_GETSET macro member,msg_suffix,by_value,no_set,new_metrics
	.elseif uMsg == FDCM_GET_&msg_suffix
		mov ebx,rv(GetWindowLong,hWnd,0)
		mov ebx,[ebx].FNCDRAW_CNTRL.pAxesInfo
		IFB <&by_value>
			invoke RtlMoveMemory,wParam, ADDR [ebx].AXES_INFO.&member, SIZEOF AXES_INFO.&member
			mov eax,1
		ELSE
			mov eax,[ebx].AXES_INFO.&member
		ENDIF
		ret
	IFB <&no_set>
	.elseif uMsg == FDCM_SET_&msg_suffix
		mov ebx,rv(GetWindowLong,hWnd,0)
		mov ebx,[ebx].FNCDRAW_CNTRL.pAxesInfo
		IF @InStr(1,<&msg_suffix>,<FORMAT>) NE 0
			free [ebx].AXES_INFO.&member
		ENDIF
		IFB <&by_value>
			invoke RtlMoveMemory,ADDR [ebx].AXES_INFO.&member, wParam, SIZEOF AXES_INFO.&member
		ELSE
			mrm [ebx].AXES_INFO.&member, wParam
		ENDIF
		IFNB <&new_metrics>
			invoke calc_metrics,hWnd,ebx
			invoke scale_axes,ebx,[ebx].AXES_INFO.labels.xMultipleOf,[ebx].AXES_INFO.labels.yMultipleOf,[ebx].AXES_INFO.labels.nxMarks,[ebx].AXES_INFO.labels.nyMarks
		ENDIF
		invoke InvalidateRect,hWnd,0,TRUE
		mov eax,1
		ret
	ENDIF
	
endm
