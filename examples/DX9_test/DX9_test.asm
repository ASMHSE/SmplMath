IFNDEF __JWASM__
	.err <JWASM required>
ENDIF
.686
.model flat, stdcall
.xmm
UNICODE 				EQU 1
WIN32_LEAN_AND_MEAN 	EQU 1

include \WinInc\include\windows.inc

includelib \WinInc\lib\Kernel32.Lib	
includelib \WinInc\lib\User32.Lib
includelib \WinInc\lib\msvcrt.lib
	
include \WinInc\Include\d3dx9.inc
includelib \WinInc\lib\D3d9.lib
includelib \WinInc\lib\D3dx9.lib

include \WinInc\include\mmsystem.inc
includelib \WinInc\lib\winmm.lib

include \macros\SmplMath\math.inc
include \macros\macros.inc

EXTERNDEF wprintf: proto c :ptr,:VARARG

fSlvSetFlags FSF_USE_SSE2

D3DFVF EQU (D3DFVF_XYZ or D3DFVF_DIFFUSE)

ARGB typedef DWORD

VERTEX_XYZ_ARGB struct
	x		REAL4 	?
	y		REAL4	?
	z		REAL4	?
	color	ARGB	?
VERTEX_XYZ_ARGB ends

WndProc proto hWnd:HWND,uMsg:UINT,wParam:WPARAM,lParam:LPARAM

.data?
	hInstance		HINSTANCE				?
	pD3D 			LPDIRECT3D9 			?
	pD3DDevice		LPDIRECT3DDEVICE9		?
	pVertexBuffer	PVOID					?
	n_list 			DWORD 					?
	rot_angle		REAL4 					?
.code

; n = number of hexagons in phi-direction
; k = number of hexagons in z-direction
; a = side length of hexagon
create_mesh proc uses edi esi ebx n:DWORD,k:DWORD,a:REAL4,phase:REAL4,D3DDevice:PVOID,ppVertexBuffer: ptr PVOID,pn:ptr DWORD
LOCAL x:REAL4,y:REAL4,z:REAL4,h:REAL4,R:REAL4,alpha:REAL4,phi:REAL4,b:REAL4,z_corr1:REAL4,x_corr:REAL4,z_corr2:REAL4
LOCAL sinVal:REAL4,cosVal:REAL4,recip_n:REAL4
LOCAL pBuffer:PVOID
LOCAL pMem:PVOID
LOCAL fSlvTLS()

	;/* create buffer */
	fn vf(D3DDevice,IDirect3DDevice9,CreateVertexBuffer),:n*k*6*(%SIZEOF VERTEX_XYZ_ARGB),0,D3DFVF,D3DPOOL_MANAGED,&pBuffer,0
	fn vf(pBuffer,IDirect3DVertexBuffer9,Lock_),0,0,&pMem,0
	mov edi,pMem
	
	;/* we use xmm0-xmm3 to share date between the macro calls: these registers must be non-volatile */
	fSlvVolatileXmmRegs remove,xmm0,xmm1,xmm2,xmm3

	;/* calculate some geometric values from input */
	ldl phi = 0
	fSlv4 y = a*(-0.75*k-1) , h = y/1.2
	fSlv4 recip_n = rcpss(n)
	fSlv4 alpha = 2*pi*recip_n
	fSlv4 b = a*1.7320508075688772935 ;sqrt(3)
	
	fSlv4 xmm3 = 0.5*alpha
	fSlv4 xmm0 = rcpss(2*sin(pi*recip_n))
	fSlv4 xmm1 = cos(xmm3)
	fSlv4 R = b*xmm0*xmm1
	
	fSlv4 z_corr1 = (b*xmm0-R)*0.5
	fSlv4 x_corr = sin(xmm3)*z_corr1
	fSlv4 z_corr2 = xmm1*z_corr1

	fSlv z = R
	fSlv x = -0.5*b	
	
	;/* set FPU precision to REAL4 (for sin/cos)*/
	fpuSetPrecision ,REAL4
	
	;/* create cylindrical mesh */
	xor esi,esi
	.while esi < k
		xor ebx,ebx
		.while ebx < n
			fSlv4 sinVal = sin(phi)
			fSlv4 cosVal = cos(phi)
			
			fSlv4 xmm0 = x + x_corr
			fSlv4 xmm1 = z - z_corr2
			fSlv4 [edi][00].VERTEX_XYZ_ARGB.x = xmm0*cosVal - xmm1*sinVal
			fSlv4 [edi][00].VERTEX_XYZ_ARGB.y = y
			fSlv4 [edi][00].VERTEX_XYZ_ARGB.z = xmm0*sinVal + xmm1*cosVal
			mov  [edi][00].VERTEX_XYZ_ARGB.color,0ff0000ffh
			
			fSlv4 xmm0 = x
			fSlv4 xmm1 = y+a
			fSlv4 xmm2 = z
			fSlv4 xmm3 = xmm0
			fSlv4 xmm0 = xmm0*cosVal - xmm2*sinVal
			fSlv4 xmm2 = xmm3*sinVal + xmm2*cosVal
			
			fSlv4 [edi][16].VERTEX_XYZ_ARGB.x = xmm0
			fSlv4 [edi][16].VERTEX_XYZ_ARGB.y = xmm1
			fSlv4 [edi][16].VERTEX_XYZ_ARGB.z = xmm2
			mov  [edi][16].VERTEX_XYZ_ARGB.color,0ffFF00ffh
			
			fSlv4 [edi][32].VERTEX_XYZ_ARGB.x = xmm0
			fSlv4 [edi][32].VERTEX_XYZ_ARGB.y = xmm1
			fSlv4 [edi][32].VERTEX_XYZ_ARGB.z = xmm2
			mov  [edi][32].VERTEX_XYZ_ARGB.color,0ffFF00ffh			
			 
			fSlv4 xmm0 = x+0.5*b
			fSlv4 xmm1 = y+1.5*a
			fSlv4 xmm2 = z+z_corr1
			fSlv4 xmm3 = xmm0
			fSlv4 xmm0 = xmm0*cosVal - xmm2*sinVal
			fSlv4 xmm2 = xmm3*sinVal + xmm2*cosVal

			fSlv4 [edi][48].VERTEX_XYZ_ARGB.x = xmm0
			fSlv4 [edi][48].VERTEX_XYZ_ARGB.y = xmm1
			fSlv4 [edi][48].VERTEX_XYZ_ARGB.z = xmm2
			mov  [edi][48].VERTEX_XYZ_ARGB.color,0ff0000ffh

			fSlv4 [edi][64].VERTEX_XYZ_ARGB.x = xmm0
			fSlv4 [edi][64].VERTEX_XYZ_ARGB.y = xmm1
			fSlv4 [edi][64].VERTEX_XYZ_ARGB.z = xmm2
			mov  [edi][64].VERTEX_XYZ_ARGB.color,0ff00FFffh

			fSlv4 xmm0 = x+b
			fSlv4 [edi][80].VERTEX_XYZ_ARGB.x = xmm0*cosVal - z*sinVal
			fSlv4 [edi][80].VERTEX_XYZ_ARGB.y = y+a
			fSlv4 [edi][80].VERTEX_XYZ_ARGB.z = xmm0*sinVal + z*cosVal
			mov  [edi][80].VERTEX_XYZ_ARGB.color,0ff00FFffh

			add edi,6*(SIZEOF VERTEX_XYZ_ARGB)
			fSlv phi = phi + alpha
			inc ebx
		.endw
		fSlv phi = phi - alpha*0.5
		fSlv y = y + 1.5*a	
		inc esi
	.endw
	
	fSlvVolatileXmmRegs default
	
	;/* morph cylindrical mesh */
	mov edi,pMem
	fSlv ebx = n*k*6
	xor esi,esi
	.while esi < ebx

		;/* use multiple assignments to avoid double calculation of abs(cos(...)) */
		fSlv4 	[edi].VERTEX_XYZ_ARGB.x = 0.5*[edi].VERTEX_XYZ_ARGB.x+0.25*[edi].VERTEX_XYZ_ARGB.x*(abs(cos(2*pi/h*[edi].VERTEX_XYZ_ARGB.y+phase))), \
				[edi].VERTEX_XYZ_ARGB.z = 0.5*[edi].VERTEX_XYZ_ARGB.z+0.25*[edi].VERTEX_XYZ_ARGB.z*(abs(cos(2*pi/h*[edi].VERTEX_XYZ_ARGB.y+phase)))

		add edi,16
		inc esi
	.endw
	
	;/* default = all xmm registers are volatile (x32) */
	fSlvVolatileXmmRegs default
	
	fn vf(pBuffer,IDirect3DVertexBuffer9,Unlock)	

	mov edx,ppVertexBuffer
	mov eax,pBuffer
	mov PVOID ptr [edx],eax
	
	mov edx,pn
	fSlv SDWORD ptr [edx] = n*k*3
	
	ret
	
create_mesh endp

Render proc uses edi esi ebx hWnd:HWND,n:DWORD,angle:REAL4
LOCAL fSlvTLS()
LOCAL world:D3DXMATRIX,m1:D3DXMATRIX,m2:D3DXMATRIX
LOCAL view:D3DXMATRIX
LOCAL projection:D3DXMATRIX
LOCAL v[3]:D3DXVECTOR3
LOCAL rect:RECT
LOCAL rot:REAL8

	fn GetClientRect,hWnd,&rect

	fn vf(pD3DDevice,IDirect3DDevice9,Clear),0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255),1.0, 0 ;D3DCOLOR_XRGB(0, 40, 100)
	fn vf(pD3DDevice,IDirect3DDevice9,Clear),0, NULL, D3DCLEAR_ZBUFFER, 0,1.0, 0
	
	fn vf(pD3DDevice,IDirect3DDevice9,BeginScene)
	fn vf(pD3DDevice,IDirect3DDevice9,SetFVF),D3DFVF

	fn D3DXMatrixRotationX,&m1,angle
	fn D3DXMatrixRotationY,&m2,angle
	fn D3DXMatrixMultiply,&world,&m1,&m2
	;fn D3DXMatrixRotationYawPitchRoll,&world,angle,angle,angle
	fn vf(pD3DDevice,IDirect3DDevice9,SetTransform),D3DTS_WORLD,&world
	
	ldl v[0*12].x = 0, v[0*12].y = 0, v[0*12].z = 10
	ldl v[1*12].x = 0, v[1*12].y = 0, v[1*12].z = 0
	ldl v[2*12].x = 0, v[2*12].y = 1, v[2*12].z = 10
	
	fn D3DXMatrixLookAtLH,&view,&v[0*12],&v[1*12],&v[2*12]
	
	fn vf(pD3DDevice,IDirect3DDevice9,SetTransform),D3DTS_VIEW,&view

	fn D3DXMatrixPerspectiveFovLH,&projection,r4: rad(45),r4: rect.right/rect.bottom, 1.0, 100.0

	fn vf(pD3DDevice,IDirect3DDevice9,SetTransform),D3DTS_PROJECTION,&projection

	fn vf(pD3DDevice,IDirect3DDevice9,SetStreamSource),0,pVertexBuffer,0,SIZEOF VERTEX_XYZ_ARGB
	fn vf(pD3DDevice,IDirect3DDevice9,DrawPrimitive),D3DPT_LINELIST, 0, n
	
	fn vf(pD3DDevice,IDirect3DDevice9,EndScene)
	fn vf(pD3DDevice,IDirect3DDevice9,Present),0,0,0,0
	
	ret
	
Render endp


main proc
LOCAL wcex:WNDCLASSEX
LOCAL msg:MSG
LOCAL d3dpp:D3DPRESENT_PARAMETERS
LOCAL pMem:PVOID

	.if !rv(pD3D = Direct3DCreate9,D3D_SDK_VERSION)
		fn MessageBox,0,"can't create Direct3D-Object","error",MB_OK
		invoke ExitProcess,-1
	.endif

	mov hInstance,rv(GetModuleHandle,0)
	mov wcex.hInstance,eax
	mov wcex.cbSize,SIZEOF WNDCLASSEX
	mov wcex.style, CS_HREDRAW or CS_VREDRAW
	mov wcex.lpfnWndProc, OFFSET WndProc
	mov wcex.cbClsExtra,NULL
	mov wcex.cbWndExtra,0
	mov wcex.hbrBackground,0
	mov wcex.lpszMenuName,NULL
	mov wcex.lpszClassName,tchr$("Some Class Name")
	mov wcex.hIcon,rv(LoadIcon,NULL,IDI_APPLICATION)
	mov wcex.hIconSm,eax
	mov wcex.hCursor,rv(LoadCursor,NULL,IDC_ARROW)
	fn RegisterClassEx,&wcex
	mov esi,rv(CreateWindowEx,NULL,wcex.lpszClassName,"WinApp",WS_VISIBLE or WS_OVERLAPPEDWINDOW OR WS_CLIPCHILDREN,CW_USEDEFAULT,CW_USEDEFAULT,1000,600,NULL,NULL,hInstance,NULL)

	fn ZeroMemory,&d3dpp, sizeof d3dpp
	mov d3dpp.Windowed,TRUE
	mov d3dpp.SwapEffect,D3DSWAPEFFECT_DISCARD;D3DSWAPEFFECT_COPY
	mov d3dpp.hDeviceWindow,esi
	mov d3dpp.BackBufferFormat,D3DFMT_A8R8G8B8
	mov d3dpp.EnableAutoDepthStencil,TRUE
	mov d3dpp.AutoDepthStencilFormat,D3DFMT_D16
	
	.if FAILED(rv(vf(pD3D,IDirect3D9,CreateDevice),D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,esi,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&d3dpp,&pD3DDevice))
		fn MessageBox,0,"can't create Direct3D-Device","error",MB_OK
	.endif

	fn vf(pD3DDevice,IDirect3DDevice9,SetRenderState),D3DRS_LIGHTING,FALSE
	fn vf(pD3DDevice,IDirect3DDevice9,SetRenderState),D3DRS_CULLMODE, D3DCULL_NONE	
	fn vf(pD3DDevice,IDirect3DDevice9,SetRenderState),D3DRS_ZENABLE,FALSE

	fn create_mesh,100,30,0.1,0,pD3DDevice,&pVertexBuffer,&n_list
	
	invoke ShowWindow,esi,SW_SHOWNORMAL
	invoke UpdateWindow,esi	
	invoke Render,esi,n_list,0
	
	.while 1
		fn GetMessage,&msg,NULL,0,0
		.break .if !eax || eax == -1
		fn TranslateMessage, &msg
		fn DispatchMessage, &msg
	.endw

	invoke ExitProcess,msg.wParam

main endp

WndProc proc uses edi esi ebx hWnd:HWND,uMsg:UINT,wParam:WPARAM,lParam:LPARAM
LOCAL ps:PAINTSTRUCT
LOCAL fSlvTLS()

	.if uMsg == WM_CLOSE
		invoke PostQuitMessage,0
	.elseif uMsg == WM_DESTROY
		invoke timeEndPeriod,1
	.elseif uMsg == WM_CREATE
		invoke timeBeginPeriod,1
		invoke SetTimer,hWnd,100,20,0
	.elseif uMsg == WM_TIMER
		fSlv rot_angle = rot_angle + 0.005
		.if pVertexBuffer
			fn vf(pVertexBuffer,IDirect3DVertexBuffer9,Release)
		.endif
		fn create_mesh,100,30,0.1,r4: rot_angle*20 ,pD3DDevice,&pVertexBuffer,&n_list
		invoke InvalidateRect,hWnd,0,0
	.elseif uMsg == WM_PAINT
		fn BeginPaint,hWnd,&ps
		invoke Render,hWnd,n_list,rot_angle
		fn EndPaint,hWnd,&ps
	.else
		invoke DefWindowProc,hWnd,uMsg,wParam,lParam
		ret
	.endif
	xor eax,eax
	ret

	
WndProc endp
end main