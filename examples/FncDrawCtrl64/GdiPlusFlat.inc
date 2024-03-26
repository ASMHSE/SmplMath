; translation: WinSDK 7.0A, GdiPlusFlat.h + GdiPlusInit.h
; by qWord, 2012

GdiplusStartup proto token: ptr ULONG_PTR,input: ptr GdiplusStartupInput, output: ptr GdiplusStartupOutput
GdiplusShutdown proto token:ULONG_PTR

GpFillMode		typedef DWORD
GpPointF		typedef PointF
GpPoint			typedef POINT
GpPath			typedef ptr
GpPathData		typedef ptr
GpRectF			typedef RectF
GpFontFamily	typedef ptr
GpStringFormat	typedef ptr
GpRect			typedef RECT
GpMatrix		typedef ptr
GpPen			typedef ptr
GpGraphics		typedef ptr
GpPathIterator	typedef ptr
GpMatrixOrder	typedef DWORD
GpRegion		typedef ptr
GpBrush			typedef ptr
GpBrushType		typedef DWORD
GpHatchStyle	typedef DWORD
GpHatch			typedef ptr
GpImage			typedef ptr
GpWrapMode		typedef ptr
GpImageAttributes	typedef ptr
GpTexture		typedef ptr
GpSolidFill		typedef ptr
GpLineGradient	typedef ptr
GpPathGradient	typedef ptr
GpUnit			typedef DWORD
GpLineCap		typedef DWORD
GpDashCap		typedef DWORD
GpLineJoin		typedef DWORD
GpCustomLineCap	typedef ptr
GpPenAlignment	typedef DWORD
GpPenType		typedef DWORD
GpDashStyle		typedef DWORD
GpAdjustableArrowCap	typedef ptr
GpBitmap		typedef ptr
GpEffect		typedef ptr
GpFlushIntention	typedef ptr
GpCoordinateSpace	typedef ptr
GpMetafile		typedef ptr
GpFontCollection	typedef ptr
GpFont			typedef ptr
GpCachedBitmap	typedef ptr
GpTestControlEnum	typedef DWORD


ColorMatrixFlags		typedef DWORD
ColorChannelFlags		typedef DWORD
EmfPlusRecordType		typedef DWORD
EmfType					typedef DWORD
Unit					typedef DWORD
StringTrimming			typedef DWORD
StringDigitSubstitute	typedef DWORD
WarpMode				typedef DWORD
CombineMode             typedef DWORD
LinearGradientMode      typedef DWORD
GetThumbnailImageAbort  typedef DWORD
RotateFlipType          typedef DWORD
DitherType              typedef DWORD
PaletteType             typedef DWORD
HistogramFormat         typedef DWORD
ColorAdjustType         typedef DWORD
WrapMode                typedef DWORD
CompositingMode         typedef DWORD
CompositingQuality      typedef DWORD
SmoothingMode           typedef DWORD
PixelOffsetMode         typedef DWORD
TextRenderingHint       typedef DWORD
InterpolationMode       typedef DWORD
DrawImageAbort          typedef ptr
EnumerateMetafileProc   typedef DWORD
MetafileFrameUnit		typedef DWORD
StringAlignment			typedef DWORD


GdipCreatePath proto  brushMode:GpFillMode, path: ptr PVOID


GdipCreatePath2 proto : ptr GpPointF,: ptr BYTE, :SDWORD, :GpFillMode, path: ptr PVOID


GdipCreatePath2I proto : ptr GpPoint,: ptr BYTE, :SDWORD, :GpFillMode, path: ptr PVOID


GdipClonePath proto path: ptr GpPath, clonePath: ptr PVOID


GdipDeletePath proto path: ptr GpPath


GdipResetPath proto path: ptr GpPath


GdipGetPointCount proto path: ptr GpPath,count: ptr SDWORD


GdipGetPathTypes proto path: ptr GpPath,types: ptr BYTE, count:SDWORD


GdipGetPathPoints proto : ptr GpPath,points: ptr GpPointF, count:SDWORD


GdipGetPathPointsI proto : ptr GpPath,points: ptr GpPoint, count:SDWORD


GdipGetPathFillMode proto path: ptr GpPath,fillmode: ptr GpFillMode


GdipSetPathFillMode proto path: ptr GpPath, fillmode:GpFillMode


GdipGetPathData proto path: ptr GpPath,pathData: ptr GpPathData


GdipStartPathFigure proto path: ptr GpPath


GdipClosePathFigure proto path: ptr GpPath


GdipClosePathFigures proto path: ptr GpPath


GdipSetPathMarker proto path: ptr GpPath


GdipClearPathMarkers proto path: ptr GpPath


GdipReversePath proto path: ptr GpPath


GdipGetPathLastPoint proto path: ptr GpPath,lastPoint: ptr GpPointF


GdipAddPathLine proto path: ptr GpPath, x1:REAL4, y1:REAL4, x2:REAL4, y2:REAL4


GdipAddPathLine2 proto path: ptr GpPath,points: ptr GpPointF, count:SDWORD


GdipAddPathArc proto path: ptr GpPath, x:REAL4, y:REAL4, _width:REAL4, height:REAL4, startAngle:REAL4, sweepAngle:REAL4


GdipAddPathBezier proto path: ptr GpPath, x1:REAL4, y1:REAL4, x2:REAL4, y2:REAL4, x3:REAL4, y3:REAL4, x4:REAL4, y4:REAL4


GdipAddPathBeziers proto path: ptr GpPath,points: ptr GpPointF, count:SDWORD


GdipAddPathCurve proto path: ptr GpPath,points: ptr GpPointF, count:SDWORD


GdipAddPathCurve2 proto path: ptr GpPath,points: ptr GpPointF, count:SDWORD, tension:REAL4


GdipAddPathCurve3 proto path: ptr GpPath,points: ptr GpPointF, count:SDWORD, _offset:SDWORD, numberOfSegments:SDWORD, tension:REAL4


GdipAddPathClosedCurve proto path: ptr GpPath,points: ptr GpPointF, count:SDWORD


GdipAddPathClosedCurve2 proto path: ptr GpPath,points: ptr GpPointF, count:SDWORD, tension:REAL4


GdipAddPathRectangle proto path: ptr GpPath, x:REAL4, y:REAL4, _width:REAL4, height:REAL4


GdipAddPathRectangles proto path: ptr GpPath,rects: ptr GpRectF, count:SDWORD


GdipAddPathEllipse proto path: ptr GpPath, x:REAL4, y:REAL4, _width:REAL4, height:REAL4


GdipAddPathPie proto path: ptr GpPath, x:REAL4, y:REAL4, _width:REAL4, height:REAL4, startAngle:REAL4, sweepAngle:REAL4


GdipAddPathPolygon proto path: ptr GpPath,points: ptr GpPointF, count:SDWORD


GdipAddPathPath proto path: ptr GpPath,addingPath: ptr GpPath, connect:BOOL


GdipAddPathString proto path: ptr GpPath,string: ptr WCHAR, _length:SDWORD,family: ptr GpFontFamily, style:SDWORD, emSize:REAL4,layoutRect: ptr RectF,format: ptr GpStringFormat


GdipAddPathStringI proto path: ptr GpPath,string: ptr WCHAR, _length:SDWORD,family: ptr GpFontFamily, style:SDWORD, emSize:REAL4,layoutRect: ptr Rect,format: ptr GpStringFormat


GdipAddPathLineI proto path: ptr GpPath, x1:SDWORD, y1:SDWORD, x2:SDWORD, y2:SDWORD


GdipAddPathLine2I proto path: ptr GpPath,points: ptr GpPoint, count:SDWORD


GdipAddPathArcI proto path: ptr GpPath, x:SDWORD, y:SDWORD, _width:SDWORD, height:SDWORD, startAngle:REAL4, sweepAngle:REAL4


GdipAddPathBezierI proto path: ptr GpPath, x1:SDWORD, y1:SDWORD, x2:SDWORD, y2:SDWORD, x3:SDWORD, y3:SDWORD, x4:SDWORD, y4:SDWORD


GdipAddPathBeziersI proto path: ptr GpPath,points: ptr GpPoint, count:SDWORD


GdipAddPathCurveI proto path: ptr GpPath,points: ptr GpPoint, count:SDWORD


GdipAddPathCurve2I proto path: ptr GpPath,points: ptr GpPoint, count:SDWORD, tension:REAL4


GdipAddPathCurve3I proto path: ptr GpPath,points: ptr GpPoint, count:SDWORD, _offset:SDWORD, numberOfSegments:SDWORD, tension:REAL4


GdipAddPathClosedCurveI proto path: ptr GpPath,points: ptr GpPoint, count:SDWORD


GdipAddPathClosedCurve2I proto path: ptr GpPath,points: ptr GpPoint, count:SDWORD, tension:REAL4


GdipAddPathRectangleI proto path: ptr GpPath, x:SDWORD, y:SDWORD, _width:SDWORD, height:SDWORD


GdipAddPathRectanglesI proto path: ptr GpPath,rects: ptr GpRect, count:SDWORD


GdipAddPathEllipseI proto path: ptr GpPath, x:SDWORD, y:SDWORD, _width:SDWORD, height:SDWORD


GdipAddPathPieI proto path: ptr GpPath, x:SDWORD, y:SDWORD, _width:SDWORD, height:SDWORD, startAngle:REAL4, sweepAngle:REAL4


GdipAddPathPolygonI proto path: ptr GpPath,points: ptr GpPoint, count:SDWORD


GdipFlattenPath proto path: ptr GpPath,matrix: ptr GpMatrix, flatness:REAL4


GdipWindingModeOutline proto path: ptr GpPath,matrix: ptr GpMatrix, flatness:REAL4



GdipWidenPath proto nativePath: ptr GpPath,pen: ptr GpPen,matrix: ptr GpMatrix, flatness:REAL4



GdipWarpPath proto path: ptr GpPath,matrix: ptr GpMatrix,points: ptr GpPointF, count:SDWORD, srcx:REAL4, srcy:REAL4, srcwidth:REAL4, srcheight:REAL4, warpMode:WarpMode, flatness:REAL4


GdipTransformPath proto path: ptr GpPath,matrix: ptr GpMatrix


GdipGetPathWorldBounds proto path: ptr GpPath,bounds: ptr GpRectF,matrix: ptr GpMatrix,pen: ptr GpPen


GdipGetPathWorldBoundsI proto path: ptr GpPath,bounds: ptr GpRect,matrix: ptr GpMatrix,pen: ptr GpPen


GdipIsVisiblePathPoint proto path: ptr GpPath, x:REAL4, y:REAL4,graphics: ptr GpGraphics,result: ptr BOOL


GdipIsVisiblePathPointI proto path: ptr GpPath, x:SDWORD, y:SDWORD,graphics: ptr GpGraphics,result: ptr BOOL


GdipIsOutlineVisiblePathPoint proto path: ptr GpPath, x:REAL4, y:REAL4,pen: ptr GpPen,graphics: ptr GpGraphics,result: ptr BOOL


GdipIsOutlineVisiblePathPointI proto path: ptr GpPath, x:SDWORD, y:SDWORD,pen: ptr GpPen,graphics: ptr GpGraphics,result: ptr BOOL


;//---------------------------------------------------------------------------- 
;// PathIterator APIs 
;//----------------------------------------------------------------------------


GdipCreatePathIter proto  iterator: ptr PVOID,path: ptr GpPath


GdipDeletePathIter proto iterator: ptr GpPathIterator


GdipPathIterNextSubpath proto iterator: ptr GpPathIterator,resultCount: ptr SDWORD,startIndex: ptr SDWORD,endIndex: ptr SDWORD,isClosed: ptr BOOL


GdipPathIterNextSubpathPath proto iterator: ptr GpPathIterator,resultCount: ptr SDWORD,path: ptr GpPath,isClosed: ptr BOOL


GdipPathIterNextPathType proto iterator: ptr GpPathIterator,resultCount: ptr SDWORD,pathType: ptr BYTE,startIndex: ptr SDWORD,endIndex: ptr SDWORD


GdipPathIterNextMarker proto iterator: ptr GpPathIterator,resultCount: ptr SDWORD,startIndex: ptr SDWORD,endIndex: ptr SDWORD


GdipPathIterNextMarkerPath proto iterator: ptr GpPathIterator,resultCount: ptr SDWORD,path: ptr GpPath


GdipPathIterGetCount proto iterator: ptr GpPathIterator,count: ptr SDWORD


GdipPathIterGetSubpathCount proto iterator: ptr GpPathIterator,count: ptr SDWORD


GdipPathIterIsValid proto iterator: ptr GpPathIterator,valid: ptr BOOL


GdipPathIterHasCurve proto iterator: ptr GpPathIterator,hasCurve: ptr BOOL


GdipPathIterRewind proto iterator: ptr GpPathIterator


GdipPathIterEnumerate proto iterator: ptr GpPathIterator,resultCount: ptr SDWORD,points: ptr GpPointF,types: ptr BYTE, count:SDWORD


GdipPathIterCopyData proto iterator: ptr GpPathIterator,resultCount: ptr SDWORD,points: ptr GpPointF,types: ptr BYTE, startIndex:SDWORD, endIndex:SDWORD

;//---------------------------------------------------------------------------- 
;// Matrix APIs
;//----------------------------------------------------------------------------


GdipCreateMatrix proto  matrix: ptr PVOID


GdipCreateMatrix2 proto  m11:REAL4, m12:REAL4, m21:REAL4, m22:REAL4, _dx:REAL4, dy:REAL4, matrix: ptr PVOID


GdipCreateMatrix3 proto rect: ptr GpRectF,dstplg: ptr GpPointF, matrix: ptr PVOID


GdipCreateMatrix3I proto rect: ptr GpRect,dstplg: ptr GpPoint, matrix: ptr PVOID


GdipCloneMatrix proto matrix: ptr GpMatrix, cloneMatrix: ptr PVOID


GdipDeleteMatrix proto matrix: ptr GpMatrix


GdipSetMatrixElements proto matrix: ptr GpMatrix, m11:REAL4, m12:REAL4, m21:REAL4, m22:REAL4, _dx:REAL4, dy:REAL4


GdipMultiplyMatrix proto matrix: ptr GpMatrix,matrix2: ptr GpMatrix, order:GpMatrixOrder


GdipTranslateMatrix proto matrix: ptr GpMatrix, offsetX:REAL4, offsetY:REAL4, order:GpMatrixOrder


GdipScaleMatrix proto matrix: ptr GpMatrix, scaleX:REAL4, scaleY:REAL4, order:GpMatrixOrder


GdipRotateMatrix proto matrix: ptr GpMatrix, angle:REAL4, order:GpMatrixOrder


GdipShearMatrix proto matrix: ptr GpMatrix, shearX:REAL4, shearY:REAL4, order:GpMatrixOrder


GdipInvertMatrix proto matrix: ptr GpMatrix


GdipTransformMatrixPoints proto matrix: ptr GpMatrix,pts: ptr GpPointF, count:SDWORD


GdipTransformMatrixPointsI proto matrix: ptr GpMatrix,pts: ptr GpPoint, count:SDWORD


GdipVectorTransformMatrixPoints proto matrix: ptr GpMatrix,pts: ptr GpPointF, count:SDWORD


GdipVectorTransformMatrixPointsI proto matrix: ptr GpMatrix,pts: ptr GpPoint, count:SDWORD


GdipGetMatrixElements proto matrix: ptr GpMatrix,matrixOut: ptr REAL4


GdipIsMatrixInvertible proto matrix: ptr GpMatrix,result: ptr BOOL


GdipIsMatrixIdentity proto matrix: ptr GpMatrix,result: ptr BOOL


GdipIsMatrixEqual proto matrix: ptr GpMatrix,matrix2: ptr GpMatrix,result: ptr BOOL

;//---------------------------------------------------------------------------- 
;// Region APIs
;//----------------------------------------------------------------------------


GdipCreateRegion proto  region: ptr PVOID


GdipCreateRegionRect proto rect: ptr GpRectF, region: ptr PVOID


GdipCreateRegionRectI proto rect: ptr GpRect, region: ptr PVOID


GdipCreateRegionPath proto path: ptr GpPath, region: ptr PVOID


GdipCreateRegionRgnData proto regionData: ptr BYTE, _size:SDWORD, region: ptr PVOID


GdipCreateRegionHrgn proto  hRgn:HRGN, region: ptr PVOID


GdipCloneRegion proto region: ptr GpRegion, cloneRegion: ptr PVOID


GdipDeleteRegion proto region: ptr GpRegion


GdipSetInfinite proto region: ptr GpRegion


GdipSetEmpty proto region: ptr GpRegion


GdipCombineRegionRect proto region: ptr GpRegion,rect: ptr GpRectF, combineMode:CombineMode


GdipCombineRegionRectI proto region: ptr GpRegion,rect: ptr GpRect, combineMode:CombineMode


GdipCombineRegionPath proto region: ptr GpRegion,path: ptr GpPath, combineMode:CombineMode


GdipCombineRegionRegion proto region: ptr GpRegion,region2: ptr GpRegion, combineMode:CombineMode


GdipTranslateRegion proto region: ptr GpRegion, _dx:REAL4, dy:REAL4


GdipTranslateRegionI proto region: ptr GpRegion, _dx:SDWORD, dy:SDWORD


GdipTransformRegion proto region: ptr GpRegion,matrix: ptr GpMatrix


GdipGetRegionBounds proto region: ptr GpRegion,graphics: ptr GpGraphics,rect: ptr GpRectF


GdipGetRegionBoundsI proto region: ptr GpRegion,graphics: ptr GpGraphics,rect: ptr GpRect


GdipGetRegionHRgn proto region: ptr GpRegion,graphics: ptr GpGraphics,hRgn: ptr HRGN


GdipIsEmptyRegion proto region: ptr GpRegion,graphics: ptr GpGraphics,result: ptr BOOL


GdipIsInfiniteRegion proto region: ptr GpRegion,graphics: ptr GpGraphics,result: ptr BOOL


GdipIsEqualRegion proto region: ptr GpRegion,region2: ptr GpRegion,graphics: ptr GpGraphics,result: ptr BOOL


GdipGetRegionDataSize proto region: ptr GpRegion,bufferSize: ptr UINT


GdipGetRegionData proto region: ptr GpRegion,buffer: ptr BYTE, bufferSize:UINT,sizeFilled: ptr UINT


GdipIsVisibleRegionPoint proto region: ptr GpRegion, x:REAL4, y:REAL4,graphics: ptr GpGraphics,result: ptr BOOL


GdipIsVisibleRegionPointI proto region: ptr GpRegion, x:SDWORD, y:SDWORD,graphics: ptr GpGraphics,result: ptr BOOL


GdipIsVisibleRegionRect proto region: ptr GpRegion, x:REAL4, y:REAL4, _width:REAL4, height:REAL4,graphics: ptr GpGraphics,result: ptr BOOL


GdipIsVisibleRegionRectI proto region: ptr GpRegion, x:SDWORD, y:SDWORD, _width:SDWORD, height:SDWORD,graphics: ptr GpGraphics,result: ptr BOOL

GdipGetRegionScansCount proto region: ptr GpRegion,count: ptr UINT,matrix: ptr GpMatrix


GdipGetRegionScans proto region: ptr GpRegion,rects: ptr GpRectF,count: ptr SDWORD,matrix: ptr GpMatrix


GdipGetRegionScansI proto region: ptr GpRegion,rects: ptr GpRect,count: ptr SDWORD,matrix: ptr GpMatrix

;//---------------------------------------------------------------------------- 
;// Brush APIs
;//----------------------------------------------------------------------------


GdipCloneBrush proto brush: ptr GpBrush, cloneBrush: ptr PVOID


GdipDeleteBrush proto brush: ptr GpBrush


GdipGetBrushType proto brush: ptr GpBrush,_type: ptr GpBrushType

;//---------------------------------------------------------------------------- 
;// HatchBrush APIs
;//----------------------------------------------------------------------------


GdipCreateHatchBrush proto  hatchstyle:GpHatchStyle, forecol:ARGB, backcol:ARGB, brush: ptr PVOID


GdipGetHatchStyle proto brush: ptr GpHatch,hatchstyle: ptr GpHatchStyle


GdipGetHatchForegroundColor proto brush: ptr GpHatch,forecol: ptr ARGB


GdipGetHatchBackgroundColor proto brush: ptr GpHatch,backcol: ptr ARGB

;//---------------------------------------------------------------------------- 
;// TextureBrush APIs 
;//----------------------------------------------------------------------------


GdipCreateTexture proto image: ptr GpImage, wrapmode:GpWrapMode, texture: ptr PVOID


GdipCreateTexture2 proto image: ptr GpImage, wrapmode:GpWrapMode, x:REAL4, y:REAL4, _width:REAL4, height:REAL4, texture: ptr PVOID


GdipCreateTextureIA proto image: ptr GpImage,imageAttributes: ptr GpImageAttributes, x:REAL4, y:REAL4, _width:REAL4, height:REAL4, texture: ptr PVOID


GdipCreateTexture2I proto image: ptr GpImage, wrapmode:GpWrapMode, x:SDWORD, y:SDWORD, _width:SDWORD, height:SDWORD, texture: ptr PVOID


GdipCreateTextureIAI proto image: ptr GpImage,imageAttributes: ptr GpImageAttributes, x:SDWORD, y:SDWORD, _width:SDWORD, height:SDWORD, texture: ptr PVOID



GdipGetTextureTransform proto brush: ptr GpTexture,matrix: ptr GpMatrix


GdipSetTextureTransform proto brush: ptr GpTexture,matrix: ptr GpMatrix


GdipResetTextureTransform proto brush: ptr GpTexture


GdipMultiplyTextureTransform proto brush: ptr GpTexture,matrix: ptr GpMatrix, order:GpMatrixOrder


GdipTranslateTextureTransform proto brush: ptr GpTexture, _dx:REAL4, dy:REAL4, order:GpMatrixOrder


GdipScaleTextureTransform proto brush: ptr GpTexture, sx:REAL4, sy:REAL4, order:GpMatrixOrder


GdipRotateTextureTransform proto brush: ptr GpTexture, angle:REAL4, order:GpMatrixOrder


GdipSetTextureWrapMode proto brush: ptr GpTexture, wrapmode:GpWrapMode


GdipGetTextureWrapMode proto brush: ptr GpTexture,wrapmode: ptr GpWrapMode


GdipGetTextureImage proto brush: ptr GpTexture, image: ptr PVOID

;//---------------------------------------------------------------------------- 
;// SolidBrush APIs
;//----------------------------------------------------------------------------


GdipCreateSolidFill proto  color:ARGB, brush: ptr PVOID


GdipSetSolidFillColor proto brush: ptr GpSolidFill, color:ARGB


GdipGetSolidFillColor proto brush: ptr GpSolidFill,color: ptr ARGB

;//---------------------------------------------------------------------------- 
;// LineBrush APIs
;//----------------------------------------------------------------------------


GdipCreateLineBrush proto point1: ptr GpPointF,point2: ptr GpPointF, color1:ARGB, color2:ARGB, wrapMode:GpWrapMode, lineGradient: ptr PVOID


GdipCreateLineBrushI proto point1: ptr GpPoint,point2: ptr GpPoint, color1:ARGB, color2:ARGB, wrapMode:GpWrapMode, lineGradient: ptr PVOID


GdipCreateLineBrushFromRect proto rect: ptr GpRectF, color1:ARGB, color2:ARGB, mode:LinearGradientMode, wrapMode:GpWrapMode, lineGradient: ptr PVOID


GdipCreateLineBrushFromRectI proto rect: ptr GpRect, color1:ARGB, color2:ARGB, mode:LinearGradientMode, wrapMode:GpWrapMode, lineGradient: ptr PVOID


GdipCreateLineBrushFromRectWithAngle proto rect: ptr GpRectF, color1:ARGB, color2:ARGB, angle:REAL4, isAngleScalable:BOOL, wrapMode:GpWrapMode, lineGradient: ptr PVOID


GdipCreateLineBrushFromRectWithAngleI proto rect: ptr GpRect, color1:ARGB, color2:ARGB, angle:REAL4, isAngleScalable:BOOL, wrapMode:GpWrapMode, lineGradient: ptr PVOID


GdipSetLineColors proto brush: ptr GpLineGradient, color1:ARGB, color2:ARGB


GdipGetLineColors proto brush: ptr GpLineGradient,colors: ptr ARGB


GdipGetLineRect proto brush: ptr GpLineGradient,rect: ptr GpRectF


GdipGetLineRectI proto brush: ptr GpLineGradient,rect: ptr GpRect


GdipSetLineGammaCorrection proto brush: ptr GpLineGradient, useGammaCorrection:BOOL


GdipGetLineGammaCorrection proto brush: ptr GpLineGradient,useGammaCorrection: ptr BOOL


GdipGetLineBlendCount proto brush: ptr GpLineGradient,count: ptr SDWORD


GdipGetLineBlend proto brush: ptr GpLineGradient,blend: ptr REAL4,positions: ptr REAL4, count:SDWORD


GdipSetLineBlend proto brush: ptr GpLineGradient,blend: ptr REAL4,positions: ptr REAL4, count:SDWORD


GdipGetLinePresetBlendCount proto brush: ptr GpLineGradient,count: ptr SDWORD


GdipGetLinePresetBlend proto brush: ptr GpLineGradient,blend: ptr ARGB,positions: ptr REAL4, count:SDWORD


GdipSetLinePresetBlend proto brush: ptr GpLineGradient,blend: ptr ARGB,positions: ptr REAL4, count:SDWORD


GdipSetLineSigmaBlend proto brush: ptr GpLineGradient, focus:REAL4, scale:REAL4


GdipSetLineLinearBlend proto brush: ptr GpLineGradient, focus:REAL4, scale:REAL4


GdipSetLineWrapMode proto brush: ptr GpLineGradient, wrapmode:GpWrapMode


GdipGetLineWrapMode proto brush: ptr GpLineGradient,wrapmode: ptr GpWrapMode


GdipGetLineTransform proto brush: ptr GpLineGradient,matrix: ptr GpMatrix


GdipSetLineTransform proto brush: ptr GpLineGradient,matrix: ptr GpMatrix


GdipResetLineTransform proto brush: ptr GpLineGradient


GdipMultiplyLineTransform proto brush: ptr GpLineGradient,matrix: ptr GpMatrix, order:GpMatrixOrder


GdipTranslateLineTransform proto brush: ptr GpLineGradient, _dx:REAL4, dy:REAL4, order:GpMatrixOrder


GdipScaleLineTransform proto brush: ptr GpLineGradient, sx:REAL4, sy:REAL4, order:GpMatrixOrder


GdipRotateLineTransform proto brush: ptr GpLineGradient, angle:REAL4, order:GpMatrixOrder

;//---------------------------------------------------------------------------- 
;// PathGradientBrush APIs 
;//----------------------------------------------------------------------------


GdipCreatePathGradient proto points: ptr GpPointF, count:SDWORD, wrapMode:GpWrapMode, polyGradient: ptr PVOID


GdipCreatePathGradientI proto points: ptr GpPoint, count:SDWORD, wrapMode:GpWrapMode, polyGradient: ptr PVOID


GdipCreatePathGradientFromPath proto path: ptr GpPath, polyGradient: ptr PVOID


GdipGetPathGradientCenterColor proto brush: ptr GpPathGradient,colors: ptr ARGB


GdipSetPathGradientCenterColor proto brush: ptr GpPathGradient, colors:ARGB


GdipGetPathGradientSurroundColorsWithCount proto brush: ptr GpPathGradient,color: ptr ARGB,count: ptr SDWORD


GdipSetPathGradientSurroundColorsWithCount proto brush: ptr GpPathGradient,color: ptr ARGB,count: ptr SDWORD


GdipGetPathGradientPath proto brush: ptr GpPathGradient,path: ptr GpPath


GdipSetPathGradientPath proto brush: ptr GpPathGradient,path: ptr GpPath


GdipGetPathGradientCenterPoint proto brush: ptr GpPathGradient,points: ptr GpPointF


GdipGetPathGradientCenterPointI proto brush: ptr GpPathGradient,points: ptr GpPoint


GdipSetPathGradientCenterPoint proto brush: ptr GpPathGradient,points: ptr GpPointF


GdipSetPathGradientCenterPointI proto brush: ptr GpPathGradient,points: ptr GpPoint


GdipGetPathGradientRect proto brush: ptr GpPathGradient,rect: ptr GpRectF


GdipGetPathGradientRectI proto brush: ptr GpPathGradient,rect: ptr GpRect


GdipGetPathGradientPointCount proto brush: ptr GpPathGradient,count: ptr SDWORD


GdipGetPathGradientSurroundColorCount proto brush: ptr GpPathGradient,count: ptr SDWORD


GdipSetPathGradientGammaCorrection proto brush: ptr GpPathGradient, useGammaCorrection:BOOL


GdipGetPathGradientGammaCorrection proto brush: ptr GpPathGradient,useGammaCorrection: ptr BOOL


GdipGetPathGradientBlendCount proto brush: ptr GpPathGradient,count: ptr SDWORD


GdipGetPathGradientBlend proto brush: ptr GpPathGradient,blend: ptr REAL4,positions: ptr REAL4, count:SDWORD


GdipSetPathGradientBlend proto brush: ptr GpPathGradient,blend: ptr REAL4,positions: ptr REAL4, count:SDWORD


GdipGetPathGradientPresetBlendCount proto brush: ptr GpPathGradient,count: ptr SDWORD


GdipGetPathGradientPresetBlend proto brush: ptr GpPathGradient,blend: ptr ARGB,positions: ptr REAL4, count:SDWORD


GdipSetPathGradientPresetBlend proto brush: ptr GpPathGradient,blend: ptr ARGB,positions: ptr REAL4, count:SDWORD


GdipSetPathGradientSigmaBlend proto brush: ptr GpPathGradient, focus:REAL4, scale:REAL4


GdipSetPathGradientLinearBlend proto brush: ptr GpPathGradient, focus:REAL4, scale:REAL4


GdipGetPathGradientWrapMode proto brush: ptr GpPathGradient,wrapmode: ptr GpWrapMode


GdipSetPathGradientWrapMode proto brush: ptr GpPathGradient, wrapmode:GpWrapMode


GdipGetPathGradientTransform proto brush: ptr GpPathGradient,matrix: ptr GpMatrix


GdipSetPathGradientTransform proto brush: ptr GpPathGradient,matrix: ptr GpMatrix


GdipResetPathGradientTransform proto brush: ptr GpPathGradient


GdipMultiplyPathGradientTransform proto brush: ptr GpPathGradient,matrix: ptr GpMatrix, order:GpMatrixOrder


GdipTranslatePathGradientTransform proto brush: ptr GpPathGradient, _dx:REAL4, dy:REAL4, order:GpMatrixOrder


GdipScalePathGradientTransform proto brush: ptr GpPathGradient, sx:REAL4, sy:REAL4, order:GpMatrixOrder


GdipRotatePathGradientTransform proto brush: ptr GpPathGradient, angle:REAL4, order:GpMatrixOrder


GdipGetPathGradientFocusScales proto brush: ptr GpPathGradient,xScale: ptr REAL4,yScale: ptr REAL4


GdipSetPathGradientFocusScales proto brush: ptr GpPathGradient, xScale:REAL4, yScale:REAL4

;//---------------------------------------------------------------------------- 
;// Pen APIs
;//----------------------------------------------------------------------------


GdipCreatePen1 proto  color:ARGB, _width:REAL4, unit:GpUnit, pen: ptr PVOID


GdipCreatePen2 proto brush: ptr GpBrush, _width:REAL4, unit:GpUnit, pen: ptr PVOID


GdipClonePen proto pen: ptr GpPen, clonepen: ptr PVOID


GdipDeletePen proto pen: ptr GpPen


GdipSetPenWidth proto pen: ptr GpPen, _width:REAL4


GdipGetPenWidth proto pen: ptr GpPen,_width: ptr REAL4


GdipSetPenUnit proto pen: ptr GpPen, unit:GpUnit


GdipGetPenUnit proto pen: ptr GpPen,unit: ptr GpUnit


GdipSetPenLineCap197819 proto pen: ptr GpPen, startCap:GpLineCap, endCap:GpLineCap, dashCap:GpDashCap


GdipSetPenStartCap proto pen: ptr GpPen, startCap:GpLineCap


GdipSetPenEndCap proto pen: ptr GpPen, endCap:GpLineCap


GdipSetPenDashCap197819 proto pen: ptr GpPen, dashCap:GpDashCap


GdipGetPenStartCap proto pen: ptr GpPen,startCap: ptr GpLineCap


GdipGetPenEndCap proto pen: ptr GpPen,endCap: ptr GpLineCap


GdipGetPenDashCap197819 proto pen: ptr GpPen,dashCap: ptr GpDashCap


GdipSetPenLineJoin proto pen: ptr GpPen, lineJoin:GpLineJoin


GdipGetPenLineJoin proto pen: ptr GpPen,lineJoin: ptr GpLineJoin


GdipSetPenCustomStartCap proto pen: ptr GpPen,customCap: ptr GpCustomLineCap


GdipGetPenCustomStartCap proto pen: ptr GpPen, customCap: ptr PVOID


GdipSetPenCustomEndCap proto pen: ptr GpPen,customCap: ptr GpCustomLineCap


GdipGetPenCustomEndCap proto pen: ptr GpPen, customCap: ptr PVOID


GdipSetPenMiterLimit proto pen: ptr GpPen, miterLimit:REAL4


GdipGetPenMiterLimit proto pen: ptr GpPen,miterLimit: ptr REAL4


GdipSetPenMode proto pen: ptr GpPen, penMode:GpPenAlignment


GdipGetPenMode proto pen: ptr GpPen,penMode: ptr GpPenAlignment


GdipSetPenTransform proto pen: ptr GpPen,matrix: ptr GpMatrix


GdipGetPenTransform proto pen: ptr GpPen,matrix: ptr GpMatrix


GdipResetPenTransform proto pen: ptr GpPen


GdipMultiplyPenTransform proto pen: ptr GpPen,matrix: ptr GpMatrix, order:GpMatrixOrder


GdipTranslatePenTransform proto pen: ptr GpPen, _dx:REAL4, dy:REAL4, order:GpMatrixOrder


GdipScalePenTransform proto pen: ptr GpPen, sx:REAL4, sy:REAL4, order:GpMatrixOrder


GdipRotatePenTransform proto pen: ptr GpPen, angle:REAL4, order:GpMatrixOrder


GdipSetPenColor proto pen: ptr GpPen, argb:ARGB


GdipGetPenColor proto pen: ptr GpPen,argb: ptr ARGB


GdipSetPenBrushFill proto pen: ptr GpPen,brush: ptr GpBrush


GdipGetPenBrushFill proto pen: ptr GpPen, brush: ptr PVOID


GdipGetPenFillType proto pen: ptr GpPen,_type: ptr GpPenType


GdipGetPenDashStyle proto pen: ptr GpPen,dashstyle: ptr GpDashStyle


GdipSetPenDashStyle proto pen: ptr GpPen, dashstyle:GpDashStyle


GdipGetPenDashOffset proto pen: ptr GpPen,_offset: ptr REAL4


GdipSetPenDashOffset proto pen: ptr GpPen, _offset:REAL4


GdipGetPenDashCount proto pen: ptr GpPen,count: ptr SDWORD


GdipSetPenDashArray proto pen: ptr GpPen,dash: ptr REAL4, count:SDWORD


GdipGetPenDashArray proto pen: ptr GpPen,dash: ptr REAL4, count:SDWORD


GdipGetPenCompoundCount proto pen: ptr GpPen,count: ptr SDWORD


GdipSetPenCompoundArray proto pen: ptr GpPen,dash: ptr REAL4, count:SDWORD


GdipGetPenCompoundArray proto pen: ptr GpPen,dash: ptr REAL4, count:SDWORD

;//---------------------------------------------------------------------------- 
;// CustomLineCap APIs 
;//----------------------------------------------------------------------------


GdipCreateCustomLineCap proto fillPath: ptr GpPath,strokePath: ptr GpPath, baseCap:GpLineCap, baseInset:REAL4, customCap: ptr PVOID


GdipDeleteCustomLineCap proto customCap: ptr GpCustomLineCap


GdipCloneCustomLineCap proto customCap: ptr GpCustomLineCap, clonedCap: ptr PVOID


GdipGetCustomLineCapType proto customCap: ptr GpCustomLineCap,capType: ptr CustomLineCapType


GdipSetCustomLineCapStrokeCaps proto customCap: ptr GpCustomLineCap, startCap:GpLineCap, endCap:GpLineCap


GdipGetCustomLineCapStrokeCaps proto customCap: ptr GpCustomLineCap,startCap: ptr GpLineCap,endCap: ptr GpLineCap


GdipSetCustomLineCapStrokeJoin proto customCap: ptr GpCustomLineCap, lineJoin:GpLineJoin


GdipGetCustomLineCapStrokeJoin proto customCap: ptr GpCustomLineCap,lineJoin: ptr GpLineJoin


GdipSetCustomLineCapBaseCap proto customCap: ptr GpCustomLineCap, baseCap:GpLineCap


GdipGetCustomLineCapBaseCap proto customCap: ptr GpCustomLineCap,baseCap: ptr GpLineCap


GdipSetCustomLineCapBaseInset proto customCap: ptr GpCustomLineCap, inset:REAL4


GdipGetCustomLineCapBaseInset proto customCap: ptr GpCustomLineCap,inset: ptr REAL4


GdipSetCustomLineCapWidthScale proto customCap: ptr GpCustomLineCap, widthScale:REAL4


GdipGetCustomLineCapWidthScale proto customCap: ptr GpCustomLineCap,widthScale: ptr REAL4

;//---------------------------------------------------------------------------- 
;// AdjustableArrowCap APIs 
;//----------------------------------------------------------------------------


GdipCreateAdjustableArrowCap proto  height:REAL4, _width:REAL4, isFilled:BOOL, cap: ptr PVOID


GdipSetAdjustableArrowCapHeight proto cap: ptr GpAdjustableArrowCap, height:REAL4


GdipGetAdjustableArrowCapHeight proto cap: ptr GpAdjustableArrowCap,height: ptr REAL4


GdipSetAdjustableArrowCapWidth proto cap: ptr GpAdjustableArrowCap, _width:REAL4


GdipGetAdjustableArrowCapWidth proto cap: ptr GpAdjustableArrowCap,_width: ptr REAL4


GdipSetAdjustableArrowCapMiddleInset proto cap: ptr GpAdjustableArrowCap, middleInset:REAL4


GdipGetAdjustableArrowCapMiddleInset proto cap: ptr GpAdjustableArrowCap,middleInset: ptr REAL4


GdipSetAdjustableArrowCapFillState proto cap: ptr GpAdjustableArrowCap, fillState:BOOL


GdipGetAdjustableArrowCapFillState proto cap: ptr GpAdjustableArrowCap,fillState: ptr BOOL

;//---------------------------------------------------------------------------- 
;// Image APIs
;//----------------------------------------------------------------------------


GdipLoadImageFromStream proto stream: ptr IStream, image: ptr PVOID


GdipLoadImageFromFile proto filename: ptr WCHAR, image: ptr PVOID


GdipLoadImageFromStreamICM proto stream: ptr IStream, image: ptr PVOID


GdipLoadImageFromFileICM proto filename: ptr WCHAR, image: ptr PVOID


GdipCloneImage proto image: ptr GpImage, cloneImage: ptr PVOID


GdipDisposeImage proto image: ptr GpImage


GdipSaveImageToFile proto image: ptr GpImage,filename: ptr WCHAR,clsidEncoder: ptr CLSID,encoderParams: ptr EncoderParameters


GdipSaveImageToStream proto image: ptr GpImage,stream: ptr IStream,clsidEncoder: ptr CLSID,encoderParams: ptr EncoderParameters


GdipSaveAdd proto image: ptr GpImage,encoderParams: ptr EncoderParameters


GdipSaveAddImage proto image: ptr GpImage,newImage: ptr GpImage,encoderParams: ptr EncoderParameters


GdipGetImageGraphicsContext proto image: ptr GpImage, graphics: ptr PVOID


GdipGetImageBounds proto image: ptr GpImage,srcRect: ptr GpRectF,srcUnit: ptr GpUnit


GdipGetImageDimension proto image: ptr GpImage,_width: ptr REAL4,height: ptr REAL4


GdipGetImageType proto image: ptr GpImage,_type: ptr ImageType


GdipGetImageWidth proto image: ptr GpImage,_width: ptr UINT


GdipGetImageHeight proto image: ptr GpImage,height: ptr UINT


GdipGetImageHorizontalResolution proto image: ptr GpImage,resolution: ptr REAL4


GdipGetImageVerticalResolution proto image: ptr GpImage,resolution: ptr REAL4


GdipGetImageFlags proto image: ptr GpImage,flags: ptr UINT


GdipGetImageRawFormat proto image: ptr GpImage,format: ptr GUID


GdipGetImagePixelFormat proto image: ptr GpImage,format: ptr PixelFormat


GdipGetImageThumbnail proto image: ptr GpImage, thumbWidth:UINT, thumbHeight:UINT, thumbImage: ptr PVOID, callback:GetThumbnailImageAbort,callbackData: ptr VOID


GdipGetEncoderParameterListSize proto image: ptr GpImage,clsidEncoder: ptr CLSID,_size: ptr UINT


GdipGetEncoderParameterList proto image: ptr GpImage,clsidEncoder: ptr CLSID, _size:UINT,buffer: ptr EncoderParameters


GdipImageGetFrameDimensionsCount proto image: ptr GpImage,count: ptr UINT


GdipImageGetFrameDimensionsList proto image: ptr GpImage,dimensionIDs: ptr GUID, count:UINT


GdipImageGetFrameCount proto image: ptr GpImage,dimensionID: ptr GUID,count: ptr UINT


GdipImageSelectActiveFrame proto image: ptr GpImage,dimensionID: ptr GUID, frameIndex:UINT


GdipImageRotateFlip proto image: ptr GpImage, rfType:RotateFlipType


GdipGetImagePalette proto image: ptr GpImage,palette: ptr ColorPalette, _size:SDWORD


GdipSetImagePalette proto image: ptr GpImage,palette: ptr ColorPalette


GdipGetImagePaletteSize proto image: ptr GpImage,_size: ptr SDWORD


GdipGetPropertyCount proto image: ptr GpImage,numOfProperty: ptr UINT


GdipGetPropertyIdList proto image: ptr GpImage, numOfProperty:UINT,list: ptr PROPID


GdipGetPropertyItemSize proto image: ptr GpImage, propId:PROPID,_size: ptr UINT


GdipGetPropertyItem proto image: ptr GpImage, propId:PROPID, propSize:UINT,buffer: ptr PropertyItem


GdipGetPropertySize proto image: ptr GpImage,totalBufferSize: ptr UINT,numProperties: ptr UINT


GdipGetAllPropertyItems proto image: ptr GpImage, totalBufferSize:UINT, numProperties:UINT,allItems: ptr PropertyItem


GdipRemovePropertyItem proto image: ptr GpImage, propId:PROPID


GdipSetPropertyItem proto image: ptr GpImage,item: ptr PropertyItem



GdipFindFirstImageItem proto image: ptr GpImage,item: ptr ImageItemData


GdipFindNextImageItem proto image: ptr GpImage,item: ptr ImageItemData


GdipGetImageItemData proto image: ptr GpImage,item: ptr ImageItemData


GdipImageForceValidation proto image: ptr GpImage

;//---------------------------------------------------------------------------- 
;// Bitmap APIs
;//----------------------------------------------------------------------------


GdipCreateBitmapFromStream proto stream: ptr IStream, bitmap: ptr PVOID


GdipCreateBitmapFromFile proto filename: ptr WCHAR, bitmap: ptr PVOID


GdipCreateBitmapFromStreamICM proto stream: ptr IStream, bitmap: ptr PVOID


GdipCreateBitmapFromFileICM proto filename: ptr WCHAR, bitmap: ptr PVOID


GdipCreateBitmapFromScan0 proto  _width:SDWORD, height:SDWORD, stride:SDWORD, format:PixelFormat,scan0: ptr BYTE, bitmap: ptr PVOID


GdipCreateBitmapFromGraphics proto  _width:SDWORD, height:SDWORD,target: ptr GpGraphics, bitmap: ptr PVOID


GdipCreateBitmapFromDirectDrawSurface proto surface: ptr IDirectDrawSurface7, bitmap: ptr PVOID


GdipCreateBitmapFromGdiDib proto gdiBitmapInfo: ptr BITMAPINFO,gdiBitmapData: ptr VOID, bitmap: ptr PVOID


GdipCreateBitmapFromHBITMAP proto  hbm:HBITMAP, hpal:HPALETTE, bitmap: ptr PVOID


GdipCreateHBITMAPFromBitmap proto bitmap: ptr GpBitmap,hbmReturn: ptr HBITMAP, background:ARGB


GdipCreateBitmapFromHICON proto  hicon:HICON, bitmap: ptr PVOID


GdipCreateHICONFromBitmap proto bitmap: ptr GpBitmap,hbmReturn: ptr HICON


GdipCreateBitmapFromResource proto  hInstance:HINSTANCE,lpBitmapName: ptr WCHAR, bitmap: ptr PVOID


GdipCloneBitmapArea proto  x:REAL4, y:REAL4, _width:REAL4, height:REAL4, format:PixelFormat,srcBitmap: ptr GpBitmap, dstBitmap: ptr PVOID


GdipCloneBitmapAreaI proto  x:SDWORD, y:SDWORD, _width:SDWORD, height:SDWORD, format:PixelFormat,srcBitmap: ptr GpBitmap, dstBitmap: ptr PVOID


GdipBitmapLockBits proto bitmap: ptr GpBitmap,rect: ptr GpRect, flags:UINT, format:PixelFormat,lockedBitmapData: ptr BitmapData


GdipBitmapUnlockBits proto bitmap: ptr GpBitmap,lockedBitmapData: ptr BitmapData


GdipBitmapGetPixel proto bitmap: ptr GpBitmap, x:SDWORD, y:SDWORD,color: ptr ARGB


GdipBitmapSetPixel proto bitmap: ptr GpBitmap, x:SDWORD, y:SDWORD, color:ARGB


GdipImageSetAbort proto pImage: ptr GpImage,pIAbort: ptr GdiplusAbort


GdipGraphicsSetAbort proto pGraphics: ptr GpGraphics,pIAbort: ptr GdiplusAbort



GdipBitmapConvertFormat proto pInputBitmap: ptr GpBitmap, format:PixelFormat, dithertype:DitherType, palettetype:PaletteType,palette: ptr ColorPalette, alphaThresholdPercent:REAL4



GdipInitializePalette proto palette: ptr ColorPalette, palettetype:PaletteType, optimalColors:SDWORD, useTransparentColor:BOOL,bitmap: ptr GpBitmap


GdipBitmapApplyEffect proto bitmap: ptr GpBitmap,effect: ptr CGpEffect,roi: ptr RECT, useAuxData:BOOL, auxData: ptr PVOID,auxDataSize: ptr SDWORD



GdipBitmapCreateApplyEffect proto  inputBitmaps: ptr PVOID, numInputs:SDWORD,effect: ptr CGpEffect,roi: ptr RECT,outputRect: ptr RECT, outputBitmap: ptr PVOID, useAuxData:BOOL, auxData: ptr PVOID,auxDataSize: ptr SDWORD



GdipBitmapGetHistogram proto bitmap: ptr GpBitmap, format:HistogramFormat, NumberOfEntries:UINT,channel0: ptr UINT,channel1: ptr UINT,channel2: ptr UINT,channel3: ptr UINT



GdipBitmapGetHistogramSize proto  format:HistogramFormat,NumberOfEntries: ptr UINT




GdipBitmapSetResolution proto bitmap: ptr GpBitmap, xdpi:REAL4, ydpi:REAL4

;//---------------------------------------------------------------------------- 
;// ImageAttributes APIs 
;//----------------------------------------------------------------------------


GdipCreateImageAttributes proto  imageattr: ptr PVOID


GdipCloneImageAttributes proto imageattr: ptr GpImageAttributes, cloneImageattr: ptr PVOID


GdipDisposeImageAttributes proto imageattr: ptr GpImageAttributes


GdipSetImageAttributesToIdentity proto imageattr: ptr GpImageAttributes, _type:ColorAdjustType

GdipResetImageAttributes proto imageattr: ptr GpImageAttributes, _type:ColorAdjustType


GdipSetImageAttributesColorMatrix proto imageattr: ptr GpImageAttributes, _type:ColorAdjustType, enableFlag:BOOL,colorMatrix: ptr ColorMatrix,grayMatrix: ptr ColorMatrix, flags:ColorMatrixFlags


GdipSetImageAttributesThreshold proto imageattr: ptr GpImageAttributes, _type:ColorAdjustType, enableFlag:BOOL, threshold:REAL4


GdipSetImageAttributesGamma proto imageattr: ptr GpImageAttributes, _type:ColorAdjustType, enableFlag:BOOL, gamma:REAL4


GdipSetImageAttributesNoOp proto imageattr: ptr GpImageAttributes, _type:ColorAdjustType, enableFlag:BOOL


GdipSetImageAttributesColorKeys proto imageattr: ptr GpImageAttributes, _type:ColorAdjustType, enableFlag:BOOL, colorLow:ARGB, colorHigh:ARGB


GdipSetImageAttributesOutputChannel proto imageattr: ptr GpImageAttributes, _type:ColorAdjustType, enableFlag:BOOL, channelFlags:ColorChannelFlags


GdipSetImageAttributesOutputChannelColorProfile proto imageattr: ptr GpImageAttributes, _type:ColorAdjustType, enableFlag:BOOL,colorProfileFilename: ptr WCHAR


GdipSetImageAttributesRemapTable proto imageattr: ptr GpImageAttributes, _type:ColorAdjustType, enableFlag:BOOL, mapSize:UINT,map: ptr ColorMap

GdipSetImageAttributesWrapMode proto imageAttr: ptr GpImageAttributes, wrap:WrapMode, argb:ARGB, clamp:BOOL



GdipSetImageAttributesICMMode proto imageAttr: ptr GpImageAttributes, on:BOOL



GdipGetImageAttributesAdjustedPalette proto imageAttr: ptr GpImageAttributes,colorPalette: ptr ColorPalette, colorAdjustType:ColorAdjustType


;//---------------------------------------------------------------------------- 
;// Graphics APIs
;//----------------------------------------------------------------------------


GdipFlush proto graphics: ptr GpGraphics, intention:GpFlushIntention


GdipCreateFromHDC proto  hdc:HDC, graphics: ptr PVOID


GdipCreateFromHDC2 proto  hdc:HDC, hDevice:HANDLE, graphics: ptr PVOID


GdipCreateFromHWND proto  hwnd:HWND, graphics: ptr PVOID


GdipCreateFromHWNDICM proto  hwnd:HWND, graphics: ptr PVOID


GdipDeleteGraphics proto graphics: ptr GpGraphics


GdipGetDC proto graphics: ptr GpGraphics,hdc: ptr HDC


GdipReleaseDC proto graphics: ptr GpGraphics, hdc:HDC


GdipSetCompositingMode proto graphics: ptr GpGraphics, compositingMode:CompositingMode


GdipGetCompositingMode proto graphics: ptr GpGraphics,compositingMode: ptr CompositingMode


GdipSetRenderingOrigin proto graphics: ptr GpGraphics, x:SDWORD, y:SDWORD


GdipGetRenderingOrigin proto graphics: ptr GpGraphics,x: ptr SDWORD,y: ptr SDWORD


GdipSetCompositingQuality proto graphics: ptr GpGraphics, compositingQuality:CompositingQuality


GdipGetCompositingQuality proto graphics: ptr GpGraphics,compositingQuality: ptr CompositingQuality


GdipSetSmoothingMode proto graphics: ptr GpGraphics, smoothingMode:SmoothingMode


GdipGetSmoothingMode proto graphics: ptr GpGraphics,smoothingMode: ptr SmoothingMode


GdipSetPixelOffsetMode proto graphics: ptr GpGraphics, pixelOffsetMode:PixelOffsetMode


GdipGetPixelOffsetMode proto graphics: ptr GpGraphics,pixelOffsetMode: ptr PixelOffsetMode


GdipSetTextRenderingHint proto graphics: ptr GpGraphics, mode:TextRenderingHint


GdipGetTextRenderingHint proto graphics: ptr GpGraphics,mode: ptr TextRenderingHint


GdipSetTextContrast proto graphics: ptr GpGraphics, contrast:UINT


GdipGetTextContrast proto graphics: ptr GpGraphics,contrast: ptr UINT


GdipSetInterpolationMode proto graphics: ptr GpGraphics, interpolationMode:InterpolationMode


GdipGetInterpolationMode proto graphics: ptr GpGraphics,interpolationMode: ptr InterpolationMode


GdipSetWorldTransform proto graphics: ptr GpGraphics,matrix: ptr GpMatrix


GdipResetWorldTransform proto graphics: ptr GpGraphics


GdipMultiplyWorldTransform proto graphics: ptr GpGraphics,matrix: ptr GpMatrix, order:GpMatrixOrder


GdipTranslateWorldTransform proto graphics: ptr GpGraphics, _dx:REAL4, dy:REAL4, order:GpMatrixOrder


GdipScaleWorldTransform proto graphics: ptr GpGraphics, sx:REAL4, sy:REAL4, order:GpMatrixOrder


GdipRotateWorldTransform proto graphics: ptr GpGraphics, angle:REAL4, order:GpMatrixOrder


GdipGetWorldTransform proto graphics: ptr GpGraphics,matrix: ptr GpMatrix


GdipResetPageTransform proto graphics: ptr GpGraphics


GdipGetPageUnit proto graphics: ptr GpGraphics,unit: ptr GpUnit


GdipGetPageScale proto graphics: ptr GpGraphics,scale: ptr REAL4


GdipSetPageUnit proto graphics: ptr GpGraphics, unit:GpUnit


GdipSetPageScale proto graphics: ptr GpGraphics, scale:REAL4


GdipGetDpiX proto graphics: ptr GpGraphics,dpi: ptr REAL4


GdipGetDpiY proto graphics: ptr GpGraphics,dpi: ptr REAL4


GdipTransformPoints proto graphics: ptr GpGraphics, destSpace:GpCoordinateSpace, srcSpace:GpCoordinateSpace,points: ptr GpPointF, count:SDWORD


GdipTransformPointsI proto graphics: ptr GpGraphics, destSpace:GpCoordinateSpace, srcSpace:GpCoordinateSpace,points: ptr GpPoint, count:SDWORD


GdipGetNearestColor proto graphics: ptr GpGraphics,argb: ptr ARGB

;;// Creates the Win9x Halftone Palette (even on NT) with correct Desktop colors

GdipCreateHalftonePalette proto


GdipDrawLine proto graphics: ptr GpGraphics,pen: ptr GpPen, x1:REAL4, y1:REAL4, x2:REAL4, y2:REAL4


GdipDrawLineI proto graphics: ptr GpGraphics,pen: ptr GpPen, x1:SDWORD, y1:SDWORD, x2:SDWORD, y2:SDWORD


GdipDrawLines proto graphics: ptr GpGraphics,pen: ptr GpPen,points: ptr GpPointF, count:SDWORD


GdipDrawLinesI proto graphics: ptr GpGraphics,pen: ptr GpPen,points: ptr GpPoint, count:SDWORD


GdipDrawArc proto graphics: ptr GpGraphics,pen: ptr GpPen, x:REAL4, y:REAL4, _width:REAL4, height:REAL4, startAngle:REAL4, sweepAngle:REAL4


GdipDrawArcI proto graphics: ptr GpGraphics,pen: ptr GpPen, x:SDWORD, y:SDWORD, _width:SDWORD, height:SDWORD, startAngle:REAL4, sweepAngle:REAL4


GdipDrawBezier proto graphics: ptr GpGraphics,pen: ptr GpPen, x1:REAL4, y1:REAL4, x2:REAL4, y2:REAL4, x3:REAL4, y3:REAL4, x4:REAL4, y4:REAL4


GdipDrawBezierI proto graphics: ptr GpGraphics,pen: ptr GpPen, x1:SDWORD, y1:SDWORD, x2:SDWORD, y2:SDWORD, x3:SDWORD, y3:SDWORD, x4:SDWORD, y4:SDWORD


GdipDrawBeziers proto graphics: ptr GpGraphics,pen: ptr GpPen,points: ptr GpPointF, count:SDWORD


GdipDrawBeziersI proto graphics: ptr GpGraphics,pen: ptr GpPen,points: ptr GpPoint, count:SDWORD


GdipDrawRectangle proto graphics: ptr GpGraphics,pen: ptr GpPen, x:REAL4, y:REAL4, _width:REAL4, height:REAL4


GdipDrawRectangleI proto graphics: ptr GpGraphics,pen: ptr GpPen, x:SDWORD, y:SDWORD, _width:SDWORD, height:SDWORD


GdipDrawRectangles proto graphics: ptr GpGraphics,pen: ptr GpPen,rects: ptr GpRectF, count:SDWORD


GdipDrawRectanglesI proto graphics: ptr GpGraphics,pen: ptr GpPen,rects: ptr GpRect, count:SDWORD


GdipDrawEllipse proto graphics: ptr GpGraphics,pen: ptr GpPen, x:REAL4, y:REAL4, _width:REAL4, height:REAL4


GdipDrawEllipseI proto graphics: ptr GpGraphics,pen: ptr GpPen, x:SDWORD, y:SDWORD, _width:SDWORD, height:SDWORD


GdipDrawPie proto graphics: ptr GpGraphics,pen: ptr GpPen, x:REAL4, y:REAL4, _width:REAL4, height:REAL4, startAngle:REAL4, sweepAngle:REAL4


GdipDrawPieI proto graphics: ptr GpGraphics,pen: ptr GpPen, x:SDWORD, y:SDWORD, _width:SDWORD, height:SDWORD, startAngle:REAL4, sweepAngle:REAL4


GdipDrawPolygon proto graphics: ptr GpGraphics,pen: ptr GpPen,points: ptr GpPointF, count:SDWORD


GdipDrawPolygonI proto graphics: ptr GpGraphics,pen: ptr GpPen,points: ptr GpPoint, count:SDWORD


GdipDrawPath proto graphics: ptr GpGraphics,pen: ptr GpPen,path: ptr GpPath


GdipDrawCurve proto graphics: ptr GpGraphics,pen: ptr GpPen,points: ptr GpPointF, count:SDWORD


GdipDrawCurveI proto graphics: ptr GpGraphics,pen: ptr GpPen,points: ptr GpPoint, count:SDWORD


GdipDrawCurve2 proto graphics: ptr GpGraphics,pen: ptr GpPen,points: ptr GpPointF, count:SDWORD, tension:REAL4


GdipDrawCurve2I proto graphics: ptr GpGraphics,pen: ptr GpPen,points: ptr GpPoint, count:SDWORD, tension:REAL4


GdipDrawCurve3 proto graphics: ptr GpGraphics,pen: ptr GpPen,points: ptr GpPointF, count:SDWORD, _offset:SDWORD, numberOfSegments:SDWORD, tension:REAL4


GdipDrawCurve3I proto graphics: ptr GpGraphics,pen: ptr GpPen,points: ptr GpPoint, count:SDWORD, _offset:SDWORD, numberOfSegments:SDWORD, tension:REAL4


GdipDrawClosedCurve proto graphics: ptr GpGraphics,pen: ptr GpPen,points: ptr GpPointF, count:SDWORD


GdipDrawClosedCurveI proto graphics: ptr GpGraphics,pen: ptr GpPen,points: ptr GpPoint, count:SDWORD


GdipDrawClosedCurve2 proto graphics: ptr GpGraphics,pen: ptr GpPen,points: ptr GpPointF, count:SDWORD, tension:REAL4


GdipDrawClosedCurve2I proto graphics: ptr GpGraphics,pen: ptr GpPen,points: ptr GpPoint, count:SDWORD, tension:REAL4


GdipGraphicsClear proto graphics: ptr GpGraphics, color:ARGB


GdipFillRectangle proto graphics: ptr GpGraphics,brush: ptr GpBrush, x:REAL4, y:REAL4, _width:REAL4, height:REAL4


GdipFillRectangleI proto graphics: ptr GpGraphics,brush: ptr GpBrush, x:SDWORD, y:SDWORD, _width:SDWORD, height:SDWORD


GdipFillRectangles proto graphics: ptr GpGraphics,brush: ptr GpBrush,rects: ptr GpRectF, count:SDWORD


GdipFillRectanglesI proto graphics: ptr GpGraphics,brush: ptr GpBrush,rects: ptr GpRect, count:SDWORD


GdipFillPolygon proto graphics: ptr GpGraphics,brush: ptr GpBrush,points: ptr GpPointF, count:SDWORD, fillMode:GpFillMode


GdipFillPolygonI proto graphics: ptr GpGraphics,brush: ptr GpBrush,points: ptr GpPoint, count:SDWORD, fillMode:GpFillMode


GdipFillPolygon2 proto graphics: ptr GpGraphics,brush: ptr GpBrush,points: ptr GpPointF, count:SDWORD


GdipFillPolygon2I proto graphics: ptr GpGraphics,brush: ptr GpBrush,points: ptr GpPoint, count:SDWORD


GdipFillEllipse proto graphics: ptr GpGraphics,brush: ptr GpBrush, x:REAL4, y:REAL4, _width:REAL4, height:REAL4


GdipFillEllipseI proto graphics: ptr GpGraphics,brush: ptr GpBrush, x:SDWORD, y:SDWORD, _width:SDWORD, height:SDWORD


GdipFillPie proto graphics: ptr GpGraphics,brush: ptr GpBrush, x:REAL4, y:REAL4, _width:REAL4, height:REAL4, startAngle:REAL4, sweepAngle:REAL4


GdipFillPieI proto graphics: ptr GpGraphics,brush: ptr GpBrush, x:SDWORD, y:SDWORD, _width:SDWORD, height:SDWORD, startAngle:REAL4, sweepAngle:REAL4


GdipFillPath proto graphics: ptr GpGraphics,brush: ptr GpBrush,path: ptr GpPath


GdipFillClosedCurve proto graphics: ptr GpGraphics,brush: ptr GpBrush,points: ptr GpPointF, count:SDWORD


GdipFillClosedCurveI proto graphics: ptr GpGraphics,brush: ptr GpBrush,points: ptr GpPoint, count:SDWORD


GdipFillClosedCurve2 proto graphics: ptr GpGraphics,brush: ptr GpBrush,points: ptr GpPointF, count:SDWORD, tension:REAL4, fillMode:GpFillMode


GdipFillClosedCurve2I proto graphics: ptr GpGraphics,brush: ptr GpBrush,points: ptr GpPoint, count:SDWORD, tension:REAL4, fillMode:GpFillMode


GdipFillRegion proto graphics: ptr GpGraphics,brush: ptr GpBrush,region: ptr GpRegion


GdipDrawImageFX proto graphics: ptr GpGraphics,image: ptr GpImage,source: ptr GpRectF,xForm: ptr GpMatrix,effect: ptr CGpEffect,imageAttributes: ptr GpImageAttributes, srcUnit:GpUnit



GdipDrawImage proto graphics: ptr GpGraphics,image: ptr GpImage, x:REAL4, y:REAL4


GdipDrawImageI proto graphics: ptr GpGraphics,image: ptr GpImage, x:SDWORD, y:SDWORD


GdipDrawImageRect proto graphics: ptr GpGraphics,image: ptr GpImage, x:REAL4, y:REAL4, _width:REAL4, height:REAL4


GdipDrawImageRectI proto graphics: ptr GpGraphics,image: ptr GpImage, x:SDWORD, y:SDWORD, _width:SDWORD, height:SDWORD


GdipDrawImagePoints proto graphics: ptr GpGraphics,image: ptr GpImage,dstpoints: ptr GpPointF, count:SDWORD


GdipDrawImagePointsI proto graphics: ptr GpGraphics,image: ptr GpImage,dstpoints: ptr GpPoint, count:SDWORD


GdipDrawImagePointRect proto graphics: ptr GpGraphics,image: ptr GpImage, x:REAL4, y:REAL4, srcx:REAL4, srcy:REAL4, srcwidth:REAL4, srcheight:REAL4, srcUnit:GpUnit


GdipDrawImagePointRectI proto graphics: ptr GpGraphics,image: ptr GpImage, x:SDWORD, y:SDWORD, srcx:SDWORD, srcy:SDWORD, srcwidth:SDWORD, srcheight:SDWORD, srcUnit:GpUnit


GdipDrawImageRectRect proto graphics: ptr GpGraphics,image: ptr GpImage, dstx:REAL4, dsty:REAL4, dstwidth:REAL4, dstheight:REAL4, srcx:REAL4, srcy:REAL4, srcwidth:REAL4, srcheight:REAL4, srcUnit:GpUnit,imageAttributes: ptr GpImageAttributes, callback:DrawImageAbort,callbackData: ptr VOID


GdipDrawImageRectRectI proto graphics: ptr GpGraphics,image: ptr GpImage, dstx:SDWORD, dsty:SDWORD, dstwidth:SDWORD, dstheight:SDWORD, srcx:SDWORD, srcy:SDWORD, srcwidth:SDWORD, srcheight:SDWORD, srcUnit:GpUnit,imageAttributes: ptr GpImageAttributes, callback:DrawImageAbort,callbackData: ptr VOID


GdipDrawImagePointsRect proto graphics: ptr GpGraphics,image: ptr GpImage,points: ptr GpPointF, count:SDWORD, srcx:REAL4, srcy:REAL4, srcwidth:REAL4, srcheight:REAL4, srcUnit:GpUnit,imageAttributes: ptr GpImageAttributes, callback:DrawImageAbort,callbackData: ptr VOID


GdipDrawImagePointsRectI proto graphics: ptr GpGraphics,image: ptr GpImage,points: ptr GpPoint, count:SDWORD, srcx:SDWORD, srcy:SDWORD, srcwidth:SDWORD, srcheight:SDWORD, srcUnit:GpUnit,imageAttributes: ptr GpImageAttributes, callback:DrawImageAbort,callbackData: ptr VOID


GdipEnumerateMetafileDestPoint proto graphics: ptr GpGraphics,metafile: ptr GpMetafile,   destPoint:ptr PointF, callback:EnumerateMetafileProc,callbackData: ptr VOID,imageAttributes: ptr GpImageAttributes



GdipEnumerateMetafileDestPointI proto graphics: ptr GpGraphics,metafile: ptr GpMetafile, destPoint: ptr Point, callback:EnumerateMetafileProc,callbackData: ptr VOID,imageAttributes: ptr GpImageAttributes



GdipEnumerateMetafileDestRect proto graphics: ptr GpGraphics,metafile: ptr GpMetafile, destRect :  ptr RectF, callback:EnumerateMetafileProc,callbackData: ptr VOID,imageAttributes: ptr GpImageAttributes



GdipEnumerateMetafileDestRectI proto graphics: ptr GpGraphics,metafile: ptr GpMetafile, destRect: ptr Rect, callback:EnumerateMetafileProc,callbackData: ptr VOID,imageAttributes: ptr GpImageAttributes



GdipEnumerateMetafileDestPoints proto graphics: ptr GpGraphics,metafile: ptr GpMetafile,destPoints: ptr PointF, count:SDWORD, callback:EnumerateMetafileProc,callbackData: ptr VOID,imageAttributes: ptr GpImageAttributes



GdipEnumerateMetafileDestPointsI proto graphics: ptr GpGraphics,metafile: ptr GpMetafile,destPoints: ptr Point, count:SDWORD, callback:EnumerateMetafileProc,callbackData: ptr VOID,imageAttributes: ptr GpImageAttributes



GdipEnumerateMetafileSrcRectDestPoint proto graphics: ptr GpGraphics,metafile: ptr GpMetafile, destPoint:ptr PointF,   srcRect: ptr RectF, srcUnit:Unit, callback:EnumerateMetafileProc,callbackData: ptr VOID,imageAttributes: ptr GpImageAttributes



GdipEnumerateMetafileSrcRectDestPointI proto graphics: ptr GpGraphics,metafile: ptr GpMetafile, destPoint: ptr Point, srcRect: ptr Rect, srcUnit:Unit, callback:EnumerateMetafileProc,callbackData: ptr VOID,imageAttributes: ptr GpImageAttributes



GdipEnumerateMetafileSrcRectDestRect proto graphics: ptr GpGraphics,metafile: ptr GpMetafile, destRect: ptr RectF, srcRect: ptr RectF, srcUnit:Unit, callback:EnumerateMetafileProc,callbackData: ptr VOID,imageAttributes: ptr GpImageAttributes



GdipEnumerateMetafileSrcRectDestRectI proto graphics: ptr GpGraphics,metafile: ptr GpMetafile, destRect: ptr Rect, srcRect: ptr Rect, srcUnit:Unit, callback:EnumerateMetafileProc,callbackData: ptr VOID,imageAttributes: ptr GpImageAttributes



GdipEnumerateMetafileSrcRectDestPoints proto graphics: ptr GpGraphics,metafile: ptr GpMetafile,destPoints: ptr PointF, count:SDWORD,  srcRect :  ptr RectF, srcUnit:Unit, callback:EnumerateMetafileProc,callbackData: ptr VOID,imageAttributes: ptr GpImageAttributes



GdipEnumerateMetafileSrcRectDestPointsI proto graphics: ptr GpGraphics,metafile: ptr GpMetafile,destPoints: ptr Point, count:SDWORD, srcRect: ptr Rect, srcUnit:Unit, callback:EnumerateMetafileProc,callbackData: ptr VOID,imageAttributes: ptr GpImageAttributes



GdipPlayMetafileRecord proto metafile: ptr GpMetafile, recordType:EmfPlusRecordType, flags:UINT, dataSize:UINT,data: ptr BYTE



GdipSetClipGraphics proto graphics: ptr GpGraphics,srcgraphics: ptr GpGraphics, combineMode:CombineMode


GdipSetClipRect proto graphics: ptr GpGraphics, x:REAL4, y:REAL4, _width:REAL4, height:REAL4, combineMode:CombineMode


GdipSetClipRectI proto graphics: ptr GpGraphics, x:SDWORD, y:SDWORD, _width:SDWORD, height:SDWORD, combineMode:CombineMode


GdipSetClipPath proto graphics: ptr GpGraphics,path: ptr GpPath, combineMode:CombineMode


GdipSetClipRegion proto graphics: ptr GpGraphics,region: ptr GpRegion, combineMode:CombineMode


GdipSetClipHrgn proto graphics: ptr GpGraphics, hRgn:HRGN, combineMode:CombineMode


GdipResetClip proto graphics: ptr GpGraphics


GdipTranslateClip proto graphics: ptr GpGraphics, _dx:REAL4, dy:REAL4


GdipTranslateClipI proto graphics: ptr GpGraphics, _dx:SDWORD, dy:SDWORD


GdipGetClip proto graphics: ptr GpGraphics,region: ptr GpRegion


GdipGetClipBounds proto graphics: ptr GpGraphics,rect: ptr GpRectF


GdipGetClipBoundsI proto graphics: ptr GpGraphics,rect: ptr GpRect


GdipIsClipEmpty proto graphics: ptr GpGraphics,result: ptr BOOL


GdipGetVisibleClipBounds proto graphics: ptr GpGraphics,rect: ptr GpRectF


GdipGetVisibleClipBoundsI proto graphics: ptr GpGraphics,rect: ptr GpRect


GdipIsVisibleClipEmpty proto graphics: ptr GpGraphics,result: ptr BOOL


GdipIsVisiblePoint proto graphics: ptr GpGraphics, x:REAL4, y:REAL4,result: ptr BOOL


GdipIsVisiblePointI proto graphics: ptr GpGraphics, x:SDWORD, y:SDWORD,result: ptr BOOL


GdipIsVisibleRect proto graphics: ptr GpGraphics, x:REAL4, y:REAL4, _width:REAL4, height:REAL4,result: ptr BOOL


GdipIsVisibleRectI proto graphics: ptr GpGraphics, x:SDWORD, y:SDWORD, _width:SDWORD, height:SDWORD,result: ptr BOOL


GdipSaveGraphics proto graphics: ptr GpGraphics,state: ptr GraphicsState


GdipRestoreGraphics proto graphics: ptr GpGraphics, state:GraphicsState


GdipBeginContainer proto graphics: ptr GpGraphics,dstrect: ptr GpRectF,srcrect: ptr GpRectF, unit:GpUnit,state: ptr GraphicsContainer


GdipBeginContainerI proto graphics: ptr GpGraphics,dstrect: ptr GpRect,srcrect: ptr GpRect, unit:GpUnit,state: ptr GraphicsContainer


GdipBeginContainer2 proto graphics: ptr GpGraphics,state: ptr GraphicsContainer


GdipEndContainer proto graphics: ptr GpGraphics, state:GraphicsContainer


GdipGetMetafileHeaderFromWmf proto  hWmf:HMETAFILE,wmfPlaceableFileHeader: ptr WmfPlaceableFileHeader,header: ptr MetafileHeader



GdipGetMetafileHeaderFromEmf proto  hEmf:HENHMETAFILE,header: ptr MetafileHeader



GdipGetMetafileHeaderFromFile proto filename: ptr WCHAR,header: ptr MetafileHeader


GdipGetMetafileHeaderFromStream proto stream: ptr IStream,header: ptr MetafileHeader


GdipGetMetafileHeaderFromMetafile proto metafile: ptr GpMetafile,header: ptr MetafileHeader



GdipGetHemfFromMetafile proto metafile: ptr GpMetafile,hEmf: ptr HENHMETAFILE



GdipCreateStreamOnFile proto filename: ptr WCHAR, access:UINT, stream: ptr PVOID


GdipCreateMetafileFromWmf proto  hWmf:HMETAFILE, deleteWmf:BOOL,wmfPlaceableFileHeader: ptr WmfPlaceableFileHeader, metafile: ptr PVOID


GdipCreateMetafileFromEmf proto  hEmf:HENHMETAFILE, deleteEmf:BOOL, metafile: ptr PVOID


GdipCreateMetafileFromFile proto file: ptr WCHAR, metafile: ptr PVOID


GdipCreateMetafileFromWmfFile proto file: ptr WCHAR,wmfPlaceableFileHeader: ptr WmfPlaceableFileHeader, metafile: ptr PVOID


GdipCreateMetafileFromStream proto stream: ptr IStream, metafile: ptr PVOID



GdipRecordMetafile proto  referenceHdc:HDC, _type:EmfType,frameRect: ptr GpRectF, frameUnit:MetafileFrameUnit,description: ptr WCHAR, metafile: ptr PVOID



GdipRecordMetafileI proto  referenceHdc:HDC, _type:EmfType,frameRect: ptr GpRect, frameUnit:MetafileFrameUnit,description: ptr WCHAR, metafile: ptr PVOID



GdipRecordMetafileFileName proto fileName: ptr WCHAR, referenceHdc:HDC, _type:EmfType,frameRect: ptr GpRectF, frameUnit:MetafileFrameUnit,description: ptr WCHAR, metafile: ptr PVOID



GdipRecordMetafileFileNameI proto fileName: ptr WCHAR, referenceHdc:HDC, _type:EmfType,frameRect: ptr GpRect, frameUnit:MetafileFrameUnit,description: ptr WCHAR, metafile: ptr PVOID



GdipRecordMetafileStream proto stream: ptr IStream, referenceHdc:HDC, _type:EmfType,frameRect: ptr GpRectF, frameUnit:MetafileFrameUnit,description: ptr WCHAR, metafile: ptr PVOID



GdipRecordMetafileStreamI proto stream: ptr IStream, referenceHdc:HDC, _type:EmfType,frameRect: ptr GpRect, frameUnit:MetafileFrameUnit,description: ptr WCHAR, metafile: ptr PVOID



GdipSetMetafileDownLevelRasterizationLimit proto metafile: ptr GpMetafile, metafileRasterizationLimitDpi:UINT



GdipGetMetafileDownLevelRasterizationLimit proto metafile: ptr GpMetafile,metafileRasterizationLimitDpi: ptr UINT



GdipGetImageDecodersSize proto numDecoders: ptr UINT,_size: ptr UINT


GdipGetImageDecoders proto  numDecoders:UINT, _size:UINT,decoders: ptr ImageCodecInfo


GdipGetImageEncodersSize proto numEncoders: ptr UINT,_size: ptr UINT


GdipGetImageEncoders proto  numEncoders:UINT, _size:UINT,encoders: ptr ImageCodecInfo


GdipComment proto graphics: ptr GpGraphics, sizeData:UINT,data: ptr BYTE

;//---------------------------------------------------------------------------- 
;// FontFamily APIs
;//----------------------------------------------------------------------------


GdipCreateFontFamilyFromName proto _name: ptr WCHAR,fontCollection: ptr GpFontCollection, fontFamily: ptr PVOID


GdipDeleteFontFamily proto fontFamily: ptr GpFontFamily


GdipCloneFontFamily proto fontFamily: ptr GpFontFamily, clonedFontFamily: ptr PVOID


GdipGetGenericFontFamilySansSerif proto  nativeFamily: ptr PVOID


GdipGetGenericFontFamilySerif proto  nativeFamily: ptr PVOID


GdipGetGenericFontFamilyMonospace proto  nativeFamily: ptr PVOID



GdipGetFamilyName proto family: ptr GpFontFamily,  _name:LPWSTR, language:LANGID


GdipIsStyleAvailable proto family: ptr GpFontFamily, style:SDWORD,IsStyleAvailable: ptr BOOL


GdipFontCollectionEnumerable proto fontCollection: ptr GpFontCollection,graphics: ptr GpGraphics,numFound: ptr SDWORD


GdipFontCollectionEnumerate proto fontCollection: ptr GpFontCollection, numSought:SDWORD, gpfamilies: ptr GpFontFamily,numFound: ptr SDWORD,graphics: ptr GpGraphics



GdipGetEmHeight proto family: ptr GpFontFamily, style:SDWORD,EmHeight: ptr UINT16


GdipGetCellAscent proto family: ptr GpFontFamily, style:SDWORD,CellAscent: ptr UINT16


GdipGetCellDescent proto family: ptr GpFontFamily, style:SDWORD,CellDescent: ptr UINT16


GdipGetLineSpacing proto family: ptr GpFontFamily, style:SDWORD,LineSpacing: ptr UINT16


;//---------------------------------------------------------------------------- ;// Font APIs
;//----------------------------------------------------------------------------


GdipCreateFontFromDC proto  hdc:HDC, font: ptr PVOID



GdipCreateFontFromLogfontA proto  hdc:HDC,logfont: ptr LOGFONTA, font: ptr PVOID



GdipCreateFontFromLogfontW proto  hdc:HDC,logfont: ptr LOGFONTW, font: ptr PVOID



GdipCreateFont proto fontFamily: ptr GpFontFamily, emSize:REAL4, style:SDWORD, unit:Unit, font: ptr PVOID



GdipCloneFont proto font: ptr GpFont, cloneFont: ptr PVOID


GdipDeleteFont proto font: ptr GpFont


GdipGetFamily proto font: ptr GpFont, family: ptr PVOID


GdipGetFontStyle proto font: ptr GpFont,style: ptr SDWORD


GdipGetFontSize proto font: ptr GpFont,_size: ptr REAL4


GdipGetFontUnit proto font: ptr GpFont,unit: ptr Unit


GdipGetFontHeight proto font: ptr GpFont,graphics: ptr GpGraphics,height: ptr REAL4


GdipGetFontHeightGivenDPI proto font: ptr GpFont, dpi:REAL4,height: ptr REAL4


GdipGetLogFontA proto font: ptr GpFont,graphics: ptr GpGraphics,logfontA: ptr LOGFONTA


GdipGetLogFontW proto font: ptr GpFont,graphics: ptr GpGraphics,logfontW: ptr LOGFONTW


GdipNewInstalledFontCollection proto  fontCollection: ptr PVOID


GdipNewPrivateFontCollection proto  fontCollection: ptr PVOID


GdipDeletePrivateFontCollection proto  fontCollection: ptr PVOID


GdipGetFontCollectionFamilyCount proto fontCollection: ptr GpFontCollection,numFound: ptr SDWORD



GdipGetFontCollectionFamilyList proto fontCollection: ptr GpFontCollection, numSought:SDWORD, gpfamilies: ptr GpFontFamily,numFound: ptr SDWORD



GdipPrivateAddFontFile proto fontCollection: ptr GpFontCollection,filename: ptr WCHAR



GdipPrivateAddMemoryFont proto fontCollection: ptr GpFontCollection,memory: ptr void, _length:SDWORD


;//---------------------------------------------------------------------------- ;// Text APIs
;//----------------------------------------------------------------------------


GdipDrawString proto graphics: ptr GpGraphics,string: ptr WCHAR, _length:SDWORD,font: ptr GpFont,layoutRect: ptr RectF,stringFormat: ptr GpStringFormat,brush: ptr GpBrush



GdipMeasureString proto graphics: ptr GpGraphics,string: ptr WCHAR, _length:SDWORD,font: ptr GpFont,layoutRect: ptr RectF,stringFormat: ptr GpStringFormat,boundingBox: ptr RectF,codepointsFitted: ptr SDWORD,linesFilled: ptr SDWORD


GdipMeasureCharacterRanges proto graphics: ptr GpGraphics,string: ptr WCHAR, _length:SDWORD,font: ptr GpFont, layoutRect: ptr RectF,stringFormat: ptr GpStringFormat, regionCount:SDWORD, regions: ptr PVOID



GdipDrawDriverString proto graphics: ptr GpGraphics,text: ptr UINT16, _length:SDWORD,font: ptr GpFont,brush: ptr GpBrush,positions: ptr PointF, flags:SDWORD,matrix: ptr GpMatrix



GdipMeasureDriverString proto graphics: ptr GpGraphics,text: ptr UINT16, _length:SDWORD,font: ptr GpFont,positions: ptr PointF, flags:SDWORD,matrix: ptr GpMatrix,boundingBox: ptr RectF


;//----------------------------------------------------------------------------
 ;// String format APIs 
 ;//----------------------------------------------------------------------------


GdipCreateStringFormat proto  formatAttributes:SDWORD, language:LANGID, format: ptr PVOID



GdipStringFormatGetGenericDefault proto  format: ptr PVOID


GdipStringFormatGetGenericTypographic proto  format: ptr PVOID


GdipDeleteStringFormat proto format: ptr GpStringFormat


GdipCloneStringFormat proto format: ptr GpStringFormat, newFormat: ptr PVOID


GdipSetStringFormatFlags proto format: ptr GpStringFormat, flags:SDWORD

GdipGetStringFormatFlags proto format: ptr GpStringFormat,flags: ptr SDWORD


GdipSetStringFormatAlign proto format: ptr GpStringFormat, _align:StringAlignment


GdipGetStringFormatAlign proto format: ptr GpStringFormat,_align: ptr StringAlignment


GdipSetStringFormatLineAlign proto format: ptr GpStringFormat, _align:StringAlignment


GdipGetStringFormatLineAlign proto format: ptr GpStringFormat,_align: ptr StringAlignment


GdipSetStringFormatTrimming proto format: ptr GpStringFormat, trimming:StringTrimming



GdipGetStringFormatTrimming proto format: ptr GpStringFormat,trimming: ptr StringTrimming



GdipSetStringFormatHotkeyPrefix proto format: ptr GpStringFormat, hotkeyPrefix:SDWORD


GdipGetStringFormatHotkeyPrefix proto format: ptr GpStringFormat,hotkeyPrefix: ptr SDWORD


GdipSetStringFormatTabStops proto format: ptr GpStringFormat, firstTabOffset:REAL4, count:SDWORD,tabStops: ptr REAL4


GdipGetStringFormatTabStops proto format: ptr GpStringFormat, count:SDWORD,firstTabOffset: ptr REAL4,tabStops: ptr REAL4


GdipGetStringFormatTabStopCount proto format: ptr GpStringFormat,count: ptr SDWORD


GdipSetStringFormatDigitSubstitution proto format: ptr GpStringFormat, language:LANGID, substitute:StringDigitSubstitute


GdipGetStringFormatDigitSubstitution proto format: ptr GpStringFormat,language: ptr LANGID,substitute: ptr StringDigitSubstitute


GdipGetStringFormatMeasurableCharacterRangeCount proto format: ptr GpStringFormat,count: ptr SDWORD



GdipSetStringFormatMeasurableCharacterRanges proto format: ptr GpStringFormat, rangeCount:SDWORD,ranges: ptr CharacterRange


;//---------------------------------------------------------------------------- 
;// Cached Bitmap APIs 
;//----------------------------------------------------------------------------


GdipCreateCachedBitmap proto bitmap: ptr GpBitmap,graphics: ptr GpGraphics, cachedBitmap: ptr PVOID

GdipDeleteCachedBitmap proto cachedBitmap: ptr GpCachedBitmap


GdipDrawCachedBitmap proto graphics: ptr GpGraphics,cachedBitmap: ptr GpCachedBitmap, x:SDWORD, y:SDWORD


GdipEmfToWmfBits proto  hemf:HENHMETAFILE, cbData16:UINT, pData16:LPBYTE, iMapMode:SDWORD, eFlags:SDWORD



GdipSetImageAttributesCachedBackground proto imageattr: ptr GpImageAttributes, enableFlag:BOOL



GdipTestControl proto  control:GpTestControlEnum,param: ptr void


GdiplusNotificationHook proto token: ptr ULONG_PTR



GdiplusNotificationUnhook proto  token:ULONG_PTR



GdipConvertToEmfPlus proto refGraphics: ptr GpGraphics,metafile: ptr GpMetafile,conversionFailureFlag: ptr SDWORD, emfType:EmfType,description: ptr WCHAR, out_metafile: ptr PVOID



GdipConvertToEmfPlusToFile proto refGraphics: ptr GpGraphics,metafile: ptr GpMetafile,conversionFailureFlag: ptr SDWORD,filename: ptr WCHAR, emfType:EmfType,description: ptr WCHAR, out_metafile: ptr PVOID



GdipConvertToEmfPlusToStream proto refGraphics: ptr GpGraphics,metafile: ptr GpMetafile,conversionFailureFlag: ptr SDWORD,stream: ptr IStream, emfType:EmfType,description: ptr WCHAR, out_metafile: ptr PVOID
