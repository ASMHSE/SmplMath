;/* class name: FncDrawCtrl32 */
CLASS_FNCDRWCTRL EQU "FncDrawCtrl32"

;/* define prototye of callback-functions */
CB_FNC_VALUE typedef proto STDCALL x:REAL8,py: ptr REAL8,userData:PVOID
PCB_FNC_VALUE typedef ptr CB_FNC_VALUE
CB_FNC_VALUES typedef proto STDCALL v:ptr REAL8,n:DWORD,userData:PVOID
PCB_FNC_VALUES typedef ptr CB_FNC_VALUES


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


FDCM_GET_LBL_FORMATXW	 EQU WM_USER+22         ;* wParam = lParam = 0		, return: pointer to format string used for x labels (unicode, do not mutate) 
FDCM_GET_LBL_FORMATYW	 EQU WM_USER+23         ;* wParam = lParam = 0		, return: pointer to format string used for y labels (unicode, do not mutate)         
FDCM_GET_LBL_XFLG		 EQU WM_USER+24         ;* wParam = flags		 	         
FDCM_GET_LBL_YFLG		 EQU WM_USER+25         ;* wParam = flags                                     
FDCM_GET_LBL_MARKSIZE	 EQU WM_USER+26         ;* wParam = lParam = 0	    , return: mark size (REAL4)            
FDCM_GET_LBL_XMULTIPLEOF EQU WM_USER+27         ;* wParam = ptr REAL8     	, lParam = reserved, must be 0
FDCM_GET_LBL_YMULTIPLEOF EQU WM_USER+28         ;* wParam = ptr REAL8     	, lParam = reserved, must be 0                        
FDCM_GET_LBL_NXMARKS	 EQU WM_USER+29         ;* wParam = DWORD = number of marks             
FDCM_GET_LBL_NYMARKS	 EQU WM_USER+30         ;* wParam = DWORD = number of marks
FDCM_GET_LBL_XLBLOFFX	 EQU WM_USER+31         ;* wParam = lParam = 0	    , return: label X-offset for X-axes (REAL4, millimeters) 
FDCM_GET_LBL_XLBLOFFY	 EQU WM_USER+32         ;* wParam = lParam = 0	    , return: label Y-offset for X-axes (REAL4, millimeters) 
FDCM_GET_LBL_YLBLOFFX	 EQU WM_USER+33         ;* wParam = lParam = 0	    , return: label X-offset for Y-axes (REAL4, millimeters) 
FDCM_GET_LBL_YLBLOFFY	 EQU WM_USER+34         ;* wParam = lParam = 0	    , return: label Y-offset for Y-axes (REAL4, millimeters)                                      
                                                             
FDCM_SET_LBL_FORMATXW	 EQU WM_USER+35         ;* wParam = ptr to format string (unicode). Buffer must be allocated using GlobalAlloc (alloc-macro @ masm32); lParam = reserved, must be 0
FDCM_SET_LBL_FORMATYW	 EQU WM_USER+36         ;* wParam = ptr to format string (unicode). Buffer must be allocated using GlobalAlloc (alloc-macro @ masm32); lParam = reserved, must be 0
FDCM_SET_LBL_XFLG		 EQU WM_USER+37         ;* wParam = flags     	    , lParam = reserved, must be 0        
FDCM_SET_LBL_YFLG		 EQU WM_USER+38         ;* wParam = flags     	    , lParam = reserved, must be 0                              
FDCM_SET_LBL_MARKSIZE	 EQU WM_USER+39         ;* wParam = REAL4     	    , lParam = reserved, must be 0                         
FDCM_SET_LBL_XMULTIPLEOF EQU WM_USER+40         ;* wParam = ptr REAL8     	, lParam = reserved, must be 0
FDCM_SET_LBL_YMULTIPLEOF EQU WM_USER+41         ;* wParam = ptr REAL8     	, lParam = reserved, must be 0
FDCM_SET_LBL_NXMARKS	 EQU WM_USER+42         ;* wParam = DWORD     	    , lParam = reserved, must be 0            
FDCM_SET_LBL_NYMARKS	 EQU WM_USER+43         ;* wParam = DWORD     	    , lParam = reserved, must be 0 
FDCM_SET_LBL_XLBLOFFX	 EQU WM_USER+44         ;* wParam = REAL4     	    , lParam = reserved, must be 0 
FDCM_SET_LBL_XLBLOFFY	 EQU WM_USER+45         ;* wParam = REAL4     	    , lParam = reserved, must be 0 
FDCM_SET_LBL_YLBLOFFX	 EQU WM_USER+46         ;* wParam = REAL4     	    , lParam = reserved, must be 0 
FDCM_SET_LBL_YLBLOFFY	 EQU WM_USER+47         ;* wParam = REAL4     	    , lParam = reserved, must be 0 

FDCM_GET_FNC_USER_DATA	 EQU WM_USER+48			;* wParam = HFNC            , lParam = reserved, must be 0   , return: userData or zero if failed
FDCM_SET_FNC_USER_DATA	 EQU WM_USER+49			;* wParam = HFNC            , lParam = userData              , return: true/false
FDCM_SET_FNC_FLG         EQU WM_USER+50			;* wParam = HFNC            , lParam = flags                 , return: true/false
FDCM_GET_FNC_FLG         EQU WM_USER+51			;* wParam = HFNC            , lParam = reserved, must be 0   , return: flags
FDCM_IS_VALID_HFNC       EQU WM_USER+52			;* wParam = HFNC            , lParam = reserved, must be 0   , return: true/false
FDCM_GET_FNC_CB          EQU WM_USER+53			;* wParam = HFNC            , lParam = reserved, must be 0   , return: pointer to callback or zero, if failed
FDCM_GET_N_FUNCTIONS     EQU WM_USER+54			;* wParam = lParam = 0                                       , return: number of functions
FDCM_GET_HFNC_LIST       EQU WM_USER+55			;* wParam = ptr to SIZE_T   , lParam = reserved, must be 0   , return: pointer to array of HFNC or zero if failed. The array must be deallocated using GlobalFree (free-macro @ masm32)
FDCM_MOV_FNC_UP          EQU WM_USER+56         ;* wParam = HFNC            , lParam: 0 => move one step; 1 => move to top     , return: true/false
FDCM_MOV_FNC_DOWN        EQU WM_USER+57         ;* wParam = HFNC            , lParam: 0 => move one step; 1 => move to bottom  , return: true/false



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
PEN__DEFAULT			EQU PEN__ARGB

BRUSH__CTRL_OWNS		EQU 32
BRUSH__SHARED			EQU 64
BRUSH__CTRL_DESTROY		EQU 128
BRUSH__CLONE			EQU 256
BRUSH__ARGB				EQU (_VALUE_ARGB_ OR BRUSH__CTRL_OWNS)
BRUSH__DEFAULT			EQU BRUSH__ARGB

TXTPEN__CTRL_OWNS		EQU 512
TXTPEN__SHARED			EQU 1024
TXTPEN__CTRL_DESTROY	EQU 2048
TXTPEN__CLONE			EQU 4096
TXTPEN__ARGB			EQU (_VALUE_ARGB_ OR TXTPEN__CTRL_OWNS)
TXTPEN__DEFAULT			EQU TXTPEN__ARGB

TXTBRUSH__CTRL_OWNS		EQU 8192
TXTBRUSH__SHARED		EQU 16384
TXTBRUSH__CTRL_DESTROY	EQU 32768
TXTBRUSH__CLONE			EQU 65536
TXTBRUSH__ARGB			EQU (_VALUE_ARGB_ OR TXTBRUSH__CTRL_OWNS)
TXTBRUSH__DEFAULT		EQU TXTBRUSH__ARGB

LBL_FLG_DEFAULT			EQU (PEN__DEFAULT or BRUSH__DEFAULT or TXTPEN__DEFAULT or TXTBRUSH__DEFAULT)



;/* structures */
PointD STRUCT 8
    x REAL8 ?
    y REAL8 ?
PointD ENDS

CURR_VIEW struct 8
	xMax	REAL8	?
	xMin	REAL8	?
	yMax	REAL8	?
	yMin	REAL8	?
CURR_VIEW ends

FDC_NMHDR struct 8
	nmhdr	NMHDR	<>
	fwKeys	DWORD	?	; see WM_MOUSEMOVE
	point	POINT	<>	; client-coord.
	pointd	PointD	<>	; point in world-units
FDC_NMHDR ends
PFDC_NMHDR typedef ptr FDC_NMHDR

INNER_FRAME struct 8
	top		REAL4	?	; caliper of frame
	left	REAL4	?	;
	right	REAL4	?	;
	bottom	REAL4	?	;
INNER_FRAME ends
PINNER_FRAME typedef ptr INNER_FRAME


LABEL_INFOW struct 8
	flg				DWORD		?		; flags for pens/brushes
	pen 			PVOID		?		; pen used for marks
	penTxt			PVOID		?		; pen used for printing numbers
	brushTxt		PVOID		?		; brush "   "     "        "
	emHeight		REAL4		?		; hight of drawn text in millimeters
	pszFontFam		PWCHAR		?		; name of font family. e.g.: 'Arial'
	pszFormatX		PWCHAR		?		; format string for x labels (swprintf)
	pszFormatY		PWCHAR		?		;   "       "    "  y   "        "

	flgXlbl			DWORD		?		; specific the lable placment
	flgYlbl			DWORD		?		;
	
	pen_width		REAL4		?		; only valid if flg=PEN__ARGB
	penTxt_width	REAL4		?		;  "     "   "  flg=TXTPEN__ARGB
	dwReserved		DWORD 2 dup (?)		; reserved
	

	xMultipleOf		REAL8		?		; deteminates the distance of labels 
	yMultipleOf		REAL8		?		; 

	nxMarks			DWORD		?		; number of labels on axes
	nyMarks			DWORD		?		;

	frame			INNER_FRAME	<>		; frame limiting the axes

	xLblOffX		REAL4		?		; x,y-offset for lables on x-axe (millimeters)
	xLblOffY		REAL4		?		;  /
	yLblOffX		REAL4		?		; x,y-offset for lables on y-axe (millimeters)
	yLblOffY		REAL4		?		;  /
	
	markSize		REAL4		?		; size of half mark
LABEL_INFOW ends
PLABEL_INFOW typedef ptr LABEL_INFOW


LABEL_INFOA struct 8
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
	
	
	pen_width		REAL4		?		; only valid if flg=PEN__ARGB
	penTxt_width	REAL4		?		;  "     "   "  flg=TXTPEN__ARGB
	dwReserved		DWORD 2 dup (?)		; reserved
	

	xMultipleOf		REAL8		?		; deteminates the distance of marks 
	yMultipleOf		REAL8		?		; 

	nxMarks			DWORD		?		; number of marks on axes
	nyMarks			DWORD		?		;

	frame			INNER_FRAME	<>	; frame limiting the axes

	xLblOffX		REAL4		?		; x,y-offset for lables on x-axe (millimeters)
	xLblOffY		REAL4		?		;  /
	yLblOffX		REAL4		?		; x,y-offset for lables on y-axe (millimeters)
	yLblOffY		REAL4		?		;  /
	
	markSize		REAL4		?		; size of half mark
LABEL_INFOA ends
PLABEL_INFOA typedef ptr LABEL_INFOA

FNCFLG_VECTORIAL		EQU 80000000h

PFNC_DSCPTR typedef ptr FNC_DSCPTR

FNC_DSCPTR struct 8
	flg				DWORD			?			; must be filled, {PEN__*, FNCFLG_VECTORIAL}
	nPoints			DWORD			?			; "    "    "
	union
		pCallBack			PCB_FNC_VALUE	?	; must be filled (used if flag FNCFLG_VECTORIAL is NOT set)
		pCallBackVectorial	PCB_FNC_VALUES	?	; "    "    "    (used if FNCFLG_VECTORIAL flag is set)
	ends
	pen				PVOID			?			; must be filled
	_width			REAL4			?			; "    "    "    (only used if flg=PEN__ARGB)
	userData		PVOID			?
FNC_DSCPTR ends

HFNC typedef PFNC_DSCPTR

