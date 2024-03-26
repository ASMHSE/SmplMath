;/* class name: FncDrawCtrl64 */
CLASS_FNCDRWCTRL EQU "FncDrawCtrl64"

;/* define prototye of callback-function */
CB_FNC_VALUE typedef proto x:REAL8,py: ptr REAL8
PCB_FNC_VALUE typedef ptr CB_FNC_VALUE

fdcRedirectWmMouseWheel proto :HWND,:WPARAM,:LPARAM

;/* constants for lable placment */
ALP_CENTER_BOTTOM	EQU 1  
ALP_CENTER_TOP		EQU 2  
ALP_CENTER_LEFT		EQU 4  
ALP_CENTER_RIGHT	EQU 8  
ALP_QDRNT_0			EQU 16 
ALP_QDRNT_1			EQU 32 
ALP_QDRNT_2			EQU 64 
ALP_QDRNT_3			EQU 128
ALP_USE_X_OFFSETS	EQU 256
ALP_USE_Y_OFFSETS	EQU 512
ALP_X_DEFAULT		EQU ALP_CENTER_BOTTOM
ALP_Y_DEFAULT		EQU ALP_CENTER_LEFT


;/* control messages */
FDCM_SET_AXES_INFO		EQU WM_USER+1			;* wParam = ptr AXES_INFO 	, lParam = reserved, must be 0

FDCM_ADD_FUNCTION		EQU WM_USER+3			;* wParam = ptr FNC_DSCPTR	, lParam = reserved, must be 0	, return: HFNC
FDCM_CLIENT2UNIT		EQU WM_USER+4			;* wParam = ptr POINT     	, lParam = ptr PointD
FDCM_UNIT2CLIENT		EQU WM_USER+5			;* wParam = ptr PointD    	, lParam = ptr POINT

FDCM_SET_STYLE			EQU WM_USER+7			;* wParam = flags		 	, return: true,false
FDCM_GET_STYLE			EQU WM_USER+8			;* wParam = lParam = 0		, return: flags
FDCM_SET_CURR_VIEW		EQU WM_USER+9			;* wParam = ptr CURR_VIEW 	, lParam = reserved, must be 0
FDCM_GET_CURR_VIEW		EQU WM_USER+10			;* wParam = ptr CURR_VIEW 	, lParam = reserved, must be 0
FDCM_SET_FRAME			EQU WM_USER+11			;* wParam = ptr INNER_FRAME	, lParam = reserved, must be 0
FDCM_GET_FRAME			EQU WM_USER+12			;* wParam = ptr INNER_FRAME	, lParam = reserved, must be 0

FDCM_SET_FNC_CB			EQU	WM_USER+13			;* wParam = HFNC			, lParam = function pointer		, return: old fnc. ptr. or zero if failed
FDCM_SET_FNC_PEN		EQU WM_USER+14			;* wParam = HFNC			, lParam = ptr FNC_DSCPTR (used members: flg,_width,pen)
FDCM_GET_FNC_DSCRPTR	EQU WM_USER+15			;* wParam = HFNC			, lParam = ptr FNC_DSCPTR		, return: true/false
FDCM_DEL_FNC			EQU WM_USER+16			;* wParam = HFNC			, return: true/false
FDCM_SET_FNC_NPOINTS	EQU WM_USER+17			;* wParam = HFNC			, lParam = nPoints				, return: prev. number of points or zero if failed

FDCM_SET_LABEL_INFOA	EQU WM_USER+18			;* wParam = ptr LABEL_INFOA	, lParam = reserved, must be 0
FDCM_SET_LABEL_INFOW	EQU WM_USER+19			;* wParam = ptr LABEL_INFOW	, lParam = reserved, must be 0

FDCM_SET_BKCOLOR		EQU WM_USER+20			;* wParam = ARGB-value
FDCM_GET_BKCOLOR		EQU WM_USER+21			;* wParam = lParam = 0


;/* control styles */
FDCS_MOUSE_NOTIFICATION		EQU 1

FDCS_VALID_STYLES			EQU (FDCS_MOUSE_NOTIFICATION)


;/* notification messages */
FDCNM_MOUSE_MOVE			EQU 1000h		; wParam = id        , lParam = ptr NMHDR = ptr FDC_NMHDR

;\* ARGB			a pen/brush with given color an width is created by the control  *\
;\* CTRL_OWNS:		control gets owner of handle                                     *\
;\* SHARED:			control use the handle, but doesn't destroy it                   *\
;\* CTRL_DESTROY:   control destroys the handle if nessecary                         *\
;\* CLONE:			the brush/pen is cloned - given handle is not used               *\
_VALUE_ARGB_			EQU 1
PEN__CTRL_OWNS			EQU 2
PEN__SHARED				EQU 4
PEN__CTRL_DESTROY		EQU 8
PEN__CLONE				EQU 16
PEN__ARGB				EQU (_VALUE_ARGB_ OR PEN__CTRL_OWNS)
PEN__DEFAULT			EQU PEN__CTRL_OWNS
PEN__VALID_				EQU (PEN__CTRL_OWNS OR PEN__SHARED OR PEN__CTRL_DESTROY OR PEN__CLONE OR PEN__ARGB)

BRUSH__CTRL_OWNS		EQU 32
BRUSH__SHARED			EQU 64
BRUSH__CTRL_DESTROY		EQU 128
BRUSH__CLONE			EQU 256
BRUSH__ARGB				EQU (_VALUE_ARGB_ OR BRUSH__CTRL_OWNS)
BRUSH__DEFAULT			EQU BRUSH__CTRL_OWNS
BRUSH_VALID_			EQU (BRUSH__CTRL_OWNS OR BRUSH__SHARED OR BRUSH__CTRL_DESTROY OR BRUSH__CLONE OR BRUSH__ARGB)

TXTPEN__CTRL_OWNS		EQU 512
TXTPEN__SHARED			EQU 1024
TXTPEN__CTRL_DESTROY	EQU 2048
TXTPEN__CLONE			EQU 4096
TXTPEN__ARGB			EQU (_VALUE_ARGB_ OR TXTPEN__CTRL_OWNS)
TXTPEN__DEFAULT			EQU TXTPEN__CTRL_OWNS

TXTBRUSH__CTRL_OWNS		EQU 8192
TXTBRUSH__SHARED		EQU 16384
TXTBRUSH__CTRL_DESTROY	EQU 32768
TXTBRUSH__CLONE			EQU 65536
TXTBRUSH__ARGB			EQU (_VALUE_ARGB_ OR TXTBRUSH__CTRL_OWNS)
TXTBRUSH__DEFAULT		EQU TXTBRUSH__CTRL_OWNS

LBL_FLG_DEFAULT			EQU (PEN__DEFAULT or BRUSH__DEFAULT or TXTPEN__DEFAULT or TXTBRUSH__DEFAULT)



;/* structures */
PointD STRUCT
    x REAL8 ?
    y REAL8 ?
PointD ENDS

CURR_VIEW struct
	xMax	REAL8	?
	xMin	REAL8	?
	yMax	REAL8	?
	yMin	REAL8	?
CURR_VIEW ends

FDC_NMHDR struct
	nmhdr	NMHDR	<>
	fwKeys	DWORD	?	; see WM_MOUSEMOVE
	pointd	PointD	<>	; point in world-units
	point	POINT	<>	; client-coord.
FDC_NMHDR ends
PFDC_NMHDR typedef ptr FDC_NMHDR

INNER_FRAME struct
	top		REAL4	?	; caliper of _frame
	left	REAL4	?	;
	right	REAL4	?	;
	bottom	REAL4	?	;
INNER_FRAME ends
PINNER_FRAME typedef ptr INNER_FRAME

LABEL_INFOW struct
	flg				DWORD		?		; flags for pens/brushes
	pen 			PVOID		?		; pen used for marks
	penTxt			PVOID		?		; pen used for printing numbers
	brushTxt		PVOID		?		; brush "   "     "        "
	emHeight		REAL4		?		; hight of drawn text in millimeters
	pwszFontFam		PWCHAR		?		; name of font family. e.g.: 'Arial'
	pwszFormatX		PWCHAR		?		; format string for x labels (swprintf)
	pwszFormatY		PWCHAR		?		;   "       "    "  y   "        "

	flgXlbl			DWORD		?		; specific the lable placment
	flgYlbl			DWORD		?		;
	
	markSize		REAL4		?		; size of half mark
	pen_width		REAL4		?		; only valid if flg=PEN__ARGB
	penTxt_width	REAL4		?		;  "     "   "  flg=TXTPEN__ARGB
	dwReserved		DWORD 2 dup (?)		; reserved
	

	xMultipleOf		REAL8		?		; deteminates the distance of labels 
	yMultipleOf		REAL8		?		; 

	nxMarks			DWORD		?		; number of labels on axes
	nyMarks			DWORD		?		;

	_frame			INNER_FRAME	<>		; _frame limiting the axes

	xLblOffX		REAL4		?		; x,y-offset for lables on x-axe (millimeters)
	xLblOffY		REAL4		?		;  /
	yLblOffX		REAL4		?		; x,y-offset for lables on y-axe (millimeters)
	yLblOffY		REAL4		?		;  /
LABEL_INFOW ends
PLABEL_INFOW typedef ptr LABEL_INFOW


LABEL_INFOA struct
	flg				DWORD		?		; flags for pens/brushes
	pen 			PVOID		?		; pen used for marks
	penTxt			PVOID		?		; pen used for printing numbers
	brushTxt		PVOID		?		; brush "   "     "        "
	emHeight		REAL4		?		; hight of drawn text in millimeters
	pszFontFam		PCHAR		?		; name of font family. e.g.: 'Arial'
	pszFormatX		PCHAR		?		; format string for x labels (swprintf)
	pszFormatY		PCHAR		?		;   "       "    "  y   "        "

	flgXlbl			DWORD		?		; specific the lable placment
	flgYlbl			DWORD		?		;
	
	markSize		REAL4		?		; size of half mark
	pen_width		REAL4		?		; only valid if flg=PEN__ARGB
	penTxt_width	REAL4		?		;  "     "   "  flg=TXTPEN__ARGB
	dwReserved		DWORD 2 dup (?)		; reserved
	

	xMultipleOf		REAL8		?		; deteminates the distance of marks 
	yMultipleOf		REAL8		?		; 

	nxMarks			DWORD		?		; number of marks on axes
	nyMarks			DWORD		?		;

	_frame			INNER_FRAME	<>	; _frame limiting the axes

	xLblOffX		REAL4		?		; x,y-offset for lables on x-axe (millimeters)
	xLblOffY		REAL4		?		;  /
	yLblOffX		REAL4		?		; x,y-offset for lables on y-axe (millimeters)
	yLblOffY		REAL4		?		;  /
LABEL_INFOA ends
PLABEL_INFOA typedef ptr LABEL_INFOA

LABEL_INFO struct
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
	
	markSize	REAL4		?

	xMarkDist	REAL8 		?
	yMarkDist	REAL8 		?

	xMultipleOf	REAL8		?
	yMultipleOf	REAL8		?

	nxMarks		DWORD		?
	nyMarks		DWORD		?

	_frame		INNER_FRAME	<>

	xLblOffX	REAL4		?
	xLblOffY	REAL4		?
	yLblOffX	REAL4		?
	yLblOffY	REAL4		?
LABEL_INFO ends


METRICS struct
	rectf		RectF 	<>		; layout rect (millimeters)
	mmpux		REAL8	?		; millimeters per unit for x-coordinates
	mmpuy		REAL8	?		;     "        "    "	"  y-coordinates
	xOrgin		REAL8	?		; virtual corrd. of orgin
	yOrgin		REAL8	?		; 

	xAxe		REAL4	?		; vertical pos. of x-Axe   (mm, absolutX = xAxe + METRICS.rectf.y ) 
	yAxe		REAL4	?		; horizontal pos. of y-Axe (mm, absolutY = yAxe + METRICS.rectf.x )
METRICS ends


AXES_INFO struct
	flags		DWORD	?
	view		CURR_VIEW	<>
	labels		LABEL_INFO	<>
	metrics		METRICS		<>
AXES_INFO ends
PAXES_INFO typedef ptr AXES_INFO

ZOOM_INFO struct
	flgs	DWORD	?
	factor	REAL4	?
ZOOM_INFO ends
PZOOM_INFO typedef ptr ZOOM_INFO

ZOOM_CENTER			EQU	1

FNC_DSCPTR struct
	flg				DWORD			?		; must be filled
	nPoints			DWORD			?		; "    "    "
	pCallBack		PCB_FNC_VALUE	?		; "    "    "
	pen				PVOID			?		; "    "    "
	_width			REAL4			?		; "    "    "    (only used if flg=PEN__ARGB)
	pNextDscptr		PVOID			?
	pPrevDscptr		PVOID			?
FNC_DSCPTR ends
PFNC_DSCPTR typedef ptr FNC_DSCPTR
HFNC typedef PFNC_DSCPTR

FNCDRAW_CNTRL struct
	dwSize		DWORD 		?			; = sizeof(FNCDRAW_CNTRL)
	dwStyle		DWORD		?
	init		DWORD		?			; is ready to draw?
	pAxesInfo	PAXES_INFO	?
	argbBkColor	DWORD		?
	zoomFactor	REAL4		?
	zoomFlg		DWORD		?
	nFunctions	DWORD		?
	pFncDscptr	PFNC_DSCPTR	?
	MousePos	COORD		<>
	bMouseDown	DWORD		?
	hWheelHook	HHOOK		?
FNCDRAW_CNTRL ends
PFNCDRAW_CNTRL typedef ptr FNCDRAW_CNTRL
