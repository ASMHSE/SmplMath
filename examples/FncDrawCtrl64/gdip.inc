; Translation by Alonso Murillo,
; revised by qWord, 2011

IFNDEF GpStatusOk

ARGB typedef DWORD

GdiplusStartupInput STRUCT 8
    GdiplusVersion              DWORD ?
    DebugEventCallback          PVOID ?
    SuppressBackgroundThread    DWORD ?
    SuppressExternalCodecs      DWORD ?
GdiplusStartupInput ENDS

PointF STRUCT 8
    x REAL4 ?
    y REAL4 ?
PointF ENDS

RectF STRUCT 8
    x       REAL4 ?
    y       REAL4 ?
    _Width  REAl4 ?
    Height  REAL4 ?
RectF ENDS

SizeF STRUCT 8
    _Width REAL4 ?
    Height REAL4 ?
SizeF ENDS

LF_FACESIZE = 32

AlphaShift  EQU 24
RedShift    EQU 16
GreenShift  EQU 8
BlueShift   EQU 0
AlphaMask   EQU 0FF000000h
RedMask     EQU 000FF0000h
GreenMask   EQU 00000FF00h
BlueMask    EQU 0000000FFh

ImageLockModeRead               EQU 1
ImageLockModeWrite              EQU 2
ImageLockModeReadWrite          EQU 3
ImageLockModeUserInputBuf       EQU 4

PixelFormat1bppIndexed          EQU 30101h
PixelFormat4bppIndexed          EQU 30402h
PixelFormat8bppIndexed          EQU 30803h
PixelFormat16bppGreyScale       EQU 101004h
PixelFormat16bppRGB555          EQU 21005h
PixelFormat16bppRGB565          EQU 21006h
PixelFormat16bppARGB1555        EQU 61007h
PixelFormat24bppRGB             EQU 21808h
PixelFormat32bppRGB             EQU 22009
PixelFormat32bppARGB            EQU 26200Ah
PixelFormat32bppPARGB           EQU 0E200Bh
PixelFormat48bppRGB             EQU 10300Ch
PixelFormat64bppARGB            EQU 34400Dh
PixelFormat64bppPARGB           EQU 1C400Eh
PixelFormatMax                  EQU 0fh

PropertyTagTypeByte         EQU 1
PropertyTagTypeASCII        EQU 2
PropertyTagTypeShort        EQU 3
PropertyTagTypeLong         EQU 4
PropertyTagTypeRational     EQU 5
PropertyTagTypeUndefined    EQU 7
PropertyTagTypeSLONG        EQU 9
PropertyTagTypeSRational    EQU 10


PropertyTagExifIFD  EQU 8769h
PropertyTagGpsIFD   EQU 8825h

PropertyTagNewSubfileType       EQU 0FEh
PropertyTagSubfileType          EQU 0FFh
PropertyTagImageWidth           EQU 100h
PropertyTagImageHeight          EQU 101h
PropertyTagBitsPerSample        EQU 102h
PropertyTagCompression          EQU 103h
PropertyTagPhotometricInterp    EQU 106h
PropertyTagThreshHolding        EQU 107h
PropertyTagCellWidth            EQU 108h
PropertyTagCellHeight           EQU 109h
PropertyTagFillOrder            EQU 10Ah
PropertyTagDocumentName         EQU 10Dh
PropertyTagImageDescription     EQU 10Eh
PropertyTagEquipMake            EQU 10Fh
PropertyTagEquipModel           EQU 110h
PropertyTagStripOffsets         EQU 111h
PropertyTagOrientation          EQU 112h
PropertyTagSamplesPerPixel      EQU 115h
PropertyTagRowsPerStrip         EQU 116h
PropertyTagStripBytesCount      EQU 117h
PropertyTagMinSampleValue       EQU 118h
PropertyTagMaxSampleValue       EQU 119h
PropertyTagXResolution          EQU 11Ah
PropertyTagYResolution          EQU 11Bh
PropertyTagPlanarConfig         EQU 11Ch
PropertyTagPageName             EQU 11Dh
PropertyTagXPosition            EQU 11Eh
PropertyTagYPosition            EQU 11Fh
PropertyTagFreeOffset           EQU 120h
PropertyTagFreeByteCounts       EQU 121h
PropertyTagGrayResponseUnit     EQU 122h
PropertyTagGrayResponseCurve    EQU 123h
PropertyTagT4Option             EQU 124h
PropertyTagT6Option             EQU 125h
PropertyTagResolutionUnit       EQU 128h
PropertyTagPageNumber           EQU 129h
PropertyTagTransferFuncition    EQU 12Dh
PropertyTagSoftwareUsed         EQU 131h
PropertyTagDateTime             EQU 132h
PropertyTagArtist               EQU 13Bh
PropertyTagHostComputer         EQU 13Ch
PropertyTagPredictor            EQU 13Dh
PropertyTagWhitePoint           EQU 13Eh
PropertyTagPrimaryChromaticities    EQU 13Fh
PropertyTagColorMap             EQU 140h
PropertyTagHalftoneHints        EQU 141h
PropertyTagTileWidth            EQU 142h
PropertyTagTileLength           EQU 143h
PropertyTagTileOffset           EQU 144h
PropertyTagTileByteCounts       EQU 145h
PropertyTagInkSet               EQU 14Ch
PropertyTagInkNames             EQU 14Dh
PropertyTagNumberOfInks         EQU 14Eh
PropertyTagDotRange             EQU 150h
PropertyTagTargetPrinter        EQU 151h
PropertyTagExtraSamples         EQU 152h
PropertyTagSampleFormat         EQU 153h
PropertyTagSMinSampleValue      EQU 154h
PropertyTagSMaxSampleValue      EQU 155h
PropertyTagTransferRange        EQU 156h

PropertyTagJPEGProc                 EQU 200h
PropertyTagJPEGInterFormat          EQU 201h
PropertyTagJPEGInterLength          EQU 202h
PropertyTagJPEGRestartInterval      EQU 203h
PropertyTagJPEGLosslessPredictors   EQU 205h
PropertyTagJPEGPointTransforms      EQU 206h
PropertyTagJPEGQTables              EQU 207h
PropertyTagJPEGDCTables             EQU 208h
PropertyTagJPEGACTables             EQU 209h
PropertyTagYCbCrCoefficients        EQU 211h
PropertyTagYCbCrSubsampling         EQU 212h
PropertyTagYCbCrPositioning         EQU 213h
PropertyTagREFBlackWhite            EQU 214h
PropertyTagICCProfile               EQU 8773h
PropertyTagGamma                    EQU 301h
PropertyTagICCProfileDescriptor     EQU 302h
PropertyTagSRGBRenderingIntent      EQU 303h
PropertyTagImageTitle               EQU 320h
PropertyTagCopyright                EQU 8298h


PropertyTagResolutionXUnit              EQU 5001h
PropertyTagResolutionYUnit              EQU 5002h
PropertyTagResolutionXLengthUnit        EQU 5003h
PropertyTagResolutionYLengthUnit        EQU 5004h
PropertyTagPrintFlags                   EQU 5005h
PropertyTagPrintFlagsVersion            EQU 5006h
PropertyTagPrintFlagsCrop               EQU 5007h
PropertyTagPrintFlagsBleedWidth         EQU 5008h
PropertyTagPrintFlagsBleedWidthScale    EQU 5009h
PropertyTagHalftoneLPI                  EQU 500Ah
PropertyTagHalftoneLPIUnit              EQU 500Bh
PropertyTagHalftoneDegree               EQU 500Ch
PropertyTagHalftoneShape                EQU 500Dh
PropertyTagHalftoneMisc                 EQU 500Eh
PropertyTagHalftoneScreen               EQU 500Fh
PropertyTagJPEGQuality                  EQU 5010h
PropertyTagGridSize                     EQU 5011h
PropertyTagThumbnailFormat              EQU 5012h
PropertyTagThumbnailWidth               EQU 5013h
PropertyTagThumbnailHeight              EQU 5014h
PropertyTagThumbnailColorDepth          EQU 5015h
PropertyTagThumbnailPlanes              EQU 5016h
PropertyTagThumbnailRawBytes            EQU 5017h
PropertyTagThumbnailSize                EQU 5018h
PropertyTagThumbnailCompressedSize      EQU 5019h
PropertyTagColorTransferFunction        EQU 501Ah
PropertyTagThumbnailData                EQU 501Bh


PropertyTagThumbnailImageWidth          EQU 5020h
PropertyTagThumbnailImageHeight         EQU 5021h
PropertyTagThumbnailBitsPerSample       EQU 5022h
PropertyTagThumbnailCompression         EQU 5023h
PropertyTagThumbnailPhotometricInterp   EQU 5024h
PropertyTagThumbnailImageDescription    EQU 5025h
PropertyTagThumbnailEquipMake           EQU 5026h

PropertyTagThumbnailEquipModel              EQU 5027h
PropertyTagThumbnailStripOffsets            EQU 5028h
PropertyTagThumbnailOrientation             EQU 5029h
PropertyTagThumbnailSamplesPerPixel         EQU 502Ah
PropertyTagThumbnailRowsPerStrip            EQU 502Bh
PropertyTagThumbnailStripBytesCount         EQU 502Ch
PropertyTagThumbnailResolutionX             EQU 502Dh
PropertyTagThumbnailResolutionY             EQU 502Eh
PropertyTagThumbnailPlanarConfig            EQU 502Fh
PropertyTagThumbnailResolutionUnit          EQU 5030h
PropertyTagThumbnailTransferFunction        EQU 5031h
PropertyTagThumbnailSoftwareUsed            EQU 5032h
PropertyTagThumbnailDateTime                EQU 5033h
PropertyTagThumbnailArtist                  EQU 5034h
PropertyTagThumbnailWhitePoint              EQU 5035h
PropertyTagThumbnailPrimaryChromaticities   EQU 5036h
PropertyTagThumbnailYCbCrCoefficients       EQU 5037h
PropertyTagThumbnailYCbCrSubsampling        EQU 5038h
PropertyTagThumbnailYCbCrPositioning        EQU 5039h
PropertyTagThumbnailRefBlackWhite           EQU 503Ah
PropertyTagThumbnailCopyRight               EQU 503Bh
PropertyTagLuminanceTable                   EQU 5090h
PropertyTagChrominanceTable                 EQU 5091h
PropertyTagFrameDelay                       EQU 5100h
PropertyTagLoopCount                        EQU 5101h
PropertyTagPixelUnit                        EQU 5110h
PropertyTagPixelPerUnitX                    EQU 5111h
PropertyTagPixelPerUnitY                    EQU 5112h
PropertyTagPaletteHistogram                 EQU 5113h

PropertyTagExifExposureTime     EQU 829Ah
PropertyTagExifFNumber          EQU 829Dh
PropertyTagExifExposureProg     EQU 8822h
PropertyTagExifSpectralSense    EQU 8824h
PropertyTagExifISOSpeed         EQU 8827h
PropertyTagExifOECF             EQU 8828h
PropertyTagExifVer              EQU 9000h
PropertyTagExifDTOrig           EQU 9003h
PropertyTagExifDTDigitized      EQU 9004h
PropertyTagExifCompConfig       EQU 9101h
PropertyTagExifCompBPP          EQU 9102h
PropertyTagExifShutterSpeed     EQU 9201h
PropertyTagExifAperture         EQU 9202h
PropertyTagExifBrightness       EQU 9203h
PropertyTagExifExposureBias     EQU 9204h
PropertyTagExifMaxAperture      EQU 9205h
PropertyTagExifSubjectDist      EQU 9206h
PropertyTagExifMeteringMode     EQU 9207h
PropertyTagExifLightSource      EQU 9208h
PropertyTagExifFlash            EQU 9209h
PropertyTagExifFocalLength      EQU 920Ah
PropertyTagExifMakerNote        EQU 927Ch
PropertyTagExifUserComment      EQU 9286h
PropertyTagExifDTSubsec         EQU 9290h
PropertyTagExifDTOrigSS         EQU 9291h
PropertyTagExifDTDigSS          EQU 9292h
PropertyTagExifFPXVer           EQU 0A000h
PropertyTagExifColorSpace       EQU 0A001h
PropertyTagExifPixXDim          EQU 0A002h
PropertyTagExifPixYDim          EQU 0A003h
PropertyTagExifRelatedWav       EQU 0A004h
PropertyTagExifInterop          EQU 0A005h
PropertyTagExifFlashEnergy      EQU 0A20Bh
PropertyTagExifSpatialFR        EQU 0A20Ch
PropertyTagExifFocalXRes        EQU 0A20Eh
PropertyTagExifFocalYRes        EQU 0A20Fh
PropertyTagExifFocalResUnit     EQU 0A210h
PropertyTagExifSubjectLoc       EQU 0A214h
PropertyTagExifExposureIndex    EQU 0A215h
PropertyTagExifSensingMethod    EQU 0A217h
PropertyTagExifFileSource       EQU 0A300h
PropertyTagExifSceneType        EQU 0A301h
PropertyTagExifCfaPattern       EQU 0A302h
PropertyTagGpsVer               EQU 0
PropertyTagGpsLatitudeRef       EQU 1
PropertyTagGpsLatitude          EQU 2
PropertyTagGpsLongitudeRef      EQU 3
PropertyTagGpsLongitude         EQU 4
PropertyTagGpsAltitudeRef       EQU 5
PropertyTagGpsAltitude          EQU 6
PropertyTagGpsGpsTime           EQU 7
PropertyTagGpsGpsSatellites     EQU 8
PropertyTagGpsGpsStatus         EQU 9
PropertyTagGpsGpsMeasureMode    EQU 0Ah
PropertyTagGpsGpsDop            EQU 0Bh
PropertyTagGpsSpeedRef          EQU 0Ch
PropertyTagGpsSpeed             EQU 0Dh
PropertyTagGpsTrackRef          EQU 0Eh
PropertyTagGpsTrack             EQU 0Fh
PropertyTagGpsImgDirRef         EQU 10h
PropertyTagGpsImgDir            EQU 11h
PropertyTagGpsMapDatum          EQU 12h
PropertyTagGpsDestLatRef        EQU 13h
PropertyTagGpsDestLat           EQU 14h
PropertyTagGpsDestLongRef       EQU 15h
PropertyTagGpsDestLong          EQU 16h
PropertyTagGpsDestBearRef       EQU 17h
PropertyTagGpsDestBear          EQU 18h
PropertyTagGpsDestDistRef       EQU 19h
PropertyTagGpsDestDist          EQU 1Ah

COLORBYTES STRUC
    BlueByte    BYTE ?
    GreenByte   BYTE ?
    RedByte     BYTE ?
    AlphaByte   BYTE ?
COLORBYTES ENDS

COLORLONG  STRUC
    longval DWORD ?
COLORLONG  ENDS

ImageCodecInfo STRUC 8
    ClassID            GUID  <>
    FormatID           GUID  <>
    CodecName          DWORD ?
    DllName            DWORD ?
    FormatDescription  DWORD ?
    FilenameExtension  DWORD ?
    MimeType           DWORD ?
    flags              DWORD ?
    Version            DWORD ?
    SigCount           DWORD ?
    SigSize            DWORD ?
    SigPattern         DWORD ?
    SigMask            DWORD ?
ImageCodecInfo ENDS

EncoderParameter STRUC 8
    pGUID             GUID  <>
    NumberOfValues    DWORD ?
    vType             DWORD ?
    value             DWORD ?
EncoderParameter ENDS

EncoderParameters STRUC 8
    Count       DWORD ?
    Parameter   EncoderParameter <>
EncoderParameters ENDS

ColorPalette STRUC 8
    flags   DWORD ?
    Count   DWORD ?
    Entries DWORD ?
ColorPalette ENDS

ColorMatrix STRUC 8
    m11 REAL4 ?
    m12 REAL4 ?
    m13 REAL4 ?
    m14 REAL4 ?
    m15 REAL4 ?
    m21 REAL4 ?
    m22 REAL4 ?
    m23 REAL4 ?
    m24 REAL4 ?
    m25 REAL4 ?
    m31 REAL4 ?
    m32 REAL4 ?
    m33 REAL4 ?
    m34 REAL4 ?
    m35 REAL4 ?
    m41 REAL4 ?
    m42 REAL4 ?
    m43 REAL4 ?
    m44 REAL4 ?
    m45 REAL4 ?
    m51 REAL4 ?
    m52 REAL4 ?
    m53 REAL4 ?
    m54 REAL4 ?
    m55 REAL4 ?
ColorMatrix ENDS

BitmapData STRUC 8
    nWidth      DWORD ?
    nHeight     DWORD ?
    Stride      DWORD ?
    PixelFormat DWORD ?
    Scan0       PVOID ?
    Reserved    DWORD ?
BitmapData ENDS

PathData STRUC 8
    Count   DWORD ?
    pPoint  DWORD ?
    types   DWORD ?
PathData ENDS

PropertyItem STRUC 8
    propId   DWORD ?
    _length  DWORD ?
    nType    WORD ?
    value    DWORD ?
PropertyItem ENDS

LOGFONTA STRUC 8
    lfHeight            DWORD ?
    lfWidth             DWORD ?
    lfEscapement        DWORD ?
    lfOrientation       DWORD ?
    lfWeight            DWORD ?
    lfItalic            BYTE  ?
    lfUnderline         BYTE  ?
    lfStrikeOut         BYTE  ?
    lfCharSet           BYTE  ?
    lfOutPrecision      BYTE  ?
    lfClipPrecision     BYTE  ?
    lfQuality           BYTE  ?
    lfPitchAndFamily    BYTE  ?
    lfFaceName          BYTE  LF_FACESIZE dup(?)
LOGFONTA ENDS

LOGFONTW STRUC 8
    lfHeight            DWORD ?
    lfWidth             DWORD ?
    lfEscapement        DWORD ?
    lfOrientation       DWORD ?
    lfWeight            DWORD ?
    lfItalic            BYTE  ?
    lfUnderline         BYTE  ?
    lfStrikeOut         BYTE  ?
    lfCharSet           BYTE  ?
    lfOutPrecision      BYTE  ?
    lfClipPrecision     BYTE  ?
    lfQuality           BYTE  ?
    lfPitchAndFamily    BYTE  ?
    lfFaceName          WORD  LF_FACESIZE dup(?)
LOGFONTW ENDS

CharacterRange STRUC 8
    First   DWORD ?
    _length DWORD ?
CharacterRange ENDS

PWMFRect16 STRUC 8
    nLEFT   WORD ?
    nTop    WORD ?
    nRIGHT  WORD ?
    nBottom WORD ?
PWMFRect16 ENDS

WmfPlaceableFileHeader STRUC 8
    Key            DWORD ?
    Hmf            WORD  ?
    boundingBox    PWMFRect16 <>
    Inch           WORD  ?
    Reserved       DWORD ?
    Checksum       WORD  ?
WmfPlaceableFileHeader ENDS

ENHMETAHEADER3 STRUC 8
    itype           DWORD ?
    nSize           DWORD ?
    rclBounds       RECTL <>
    rclFrame        RECTL <>
    dSignature      DWORD ?
    nVersion        DWORD ?
    nBytes          DWORD ?
    nRecords        DWORD ?
    nHandles        WORD  ?
    sReserved       WORD  ?
    nDescription    DWORD ?
    offDescription  DWORD ?
    nPalEntries     DWORD ?
    szlDevice       SIZEL <>
    szlMillimeters  SIZEL <>
ENHMETAHEADER3 ENDS

MetafileHeader STRUC 8
    mType             DWORD ?
    msize             DWORD ?
    Version           DWORD ?
    EmfPlusFlags      DWORD ?
    DpiX              REAL4 ?
    DpiY              REAL4 ?
    x                 DWORD ?
    y                 DWORD ?
    nWIDTH            DWORD ?
    Height            DWORD ?
    EmfHeader         ENHMETAHEADER3 <>
    EmfPlusHeaderSize DWORD ?
    LogicalDpiX       DWORD ?
    LogicalDpiY       DWORD ?
MetafileHeader ENDS


UnitWorld       EQU 0
UnitDisplay     EQU 1
UnitPixel       EQU 2
UnitPoint       EQU 3
UnitInch        EQU 4
UnitDocument    EQU 5
UnitMillimeter  EQU 6


ColorsAliceBlue             EQU 0FFF0F8FFh
ColorsAntiqueWhite          EQU 0FFFAEBD7h
ColorsAqua                  EQU 0FF00FFFFh
ColorsAquamarine            EQU 0FF7FFFD4h
ColorsAzure                 EQU 0FFF0FFFFh
ColorsBeige                 EQU 0FFF5F5DCh
ColorsBisque                EQU 0FFFFE4C4h
ColorsBlack                 EQU 0FF000000h
ColorsBlanchedAlmond        EQU 0FFFFEBCDh
ColorsBlue                  EQU 0FF0000FFh
ColorsBlueViolet            EQU 0FF8A2BE2h
ColorsBrown                 EQU 0FFA52A2Ah
ColorsBurlyWood             EQU 0FFDEB887h
ColorsCadetBlue             EQU 0FF5F9EA0h
ColorsChartreuse            EQU 0FF7FFF00h
ColorsChocolate             EQU 0FFD2691Eh
ColorsCoral                 EQU 0FFFF7F50h
ColorsCornflowerBlue        EQU 0FF6495EDh
ColorsCornsilk              EQU 0FFFFF8DCh
ColorsCrimson               EQU 0FFDC143Ch
ColorsCyan                  EQU 0FF00FFFFh
ColorsDarkBlue              EQU 0FF00008Bh
ColorsDarkCyan              EQU 0FF008B8Bh
ColorsDarkGoldenrod         EQU 0FFB8860Bh
ColorsDarkGray              EQU 0FFA9A9A9h
ColorsDarkGreen             EQU 0FF006400h
ColorsDarkKhaki             EQU 0FFBDB76Bh
ColorsDarkMagenta           EQU 0FF8B008Bh
ColorsDarkOliveGreen        EQU 0FF556B2Fh
ColorsDarkOrange            EQU 0FFFF8C00h
ColorsDarkOrchid            EQU 0FF9932CCh
ColorsDarkRed               EQU 0FF8B0000h
ColorsDarkSalmon            EQU 0FFE9967Ah
ColorsDarkSeaGreen          EQU 0FF8FBC8Bh
ColorsDarkSlateBlue         EQU 0FF483D8Bh
ColorsDarkSlateGray         EQU 0FF2F4F4Fh
ColorsDarkTurquoise         EQU 0FF00CED1h
ColorsDarkViolet            EQU 0FF9400D3h
ColorsDeepPink              EQU 0FFFF1493h
ColorsDeepSkyBlue           EQU 0FF00BFFFh
ColorsDimGray               EQU 0FF696969h
ColorsDodgerBlue            EQU 0FF1E90FFh
ColorsFirebrick             EQU 0FFB22222h
ColorsFloralWhite           EQU 0FFFFFAF0h
ColorsForestGreen           EQU 0FF228B22h
ColorsFuchsia               EQU 0FFFF00FFh
ColorsGainsboro             EQU 0FFDCDCDCh
ColorsGhostWhite            EQU 0FFF8F8FFh
ColorsGold                  EQU 0FFFFD700h
ColorsGoldenrod             EQU 0FFDAA520h
ColorsGray                  EQU 0FF808080h
ColorsGreen                 EQU 0FF008000h
ColorsGreenYellow           EQU 0FFADFF2Fh
ColorsHoneydew              EQU 0FFF0FFF0h
ColorsHotPink               EQU 0FFFF69B4h
ColorsIndianRed             EQU 0FFCD5C5Ch
ColorsIndigo                EQU 0FF4B0082h
ColorsIvory                 EQU 0FFFFFFF0h
ColorsKhaki                 EQU 0FFF0E68Ch
ColorsLavender              EQU 0FFE6E6FAh
ColorsLavenderBlush         EQU 0FFFFF0F5h
ColorsLawnGreen             EQU 0FF7CFC00h
ColorsLemonChiffon          EQU 0FFFFFACDh
ColorsLightBlue             EQU 0FFADD8E6h
ColorsLightCoral            EQU 0FFF08080h
ColorsLightCyan             EQU 0FFE0FFFFh
ColorsLightGoldenrodYellow  EQU 0FFFAFAD2h
ColorsLightGray             EQU 0FFD3D3D3h
ColorsLightGreen            EQU 0FF90EE90h
ColorsLightPink             EQU 0FFFFB6C1h
ColorsLightSalmon           EQU 0FFFFA07Ah
ColorsLightSeaGreen         EQU 0FF20B2AAh
ColorsLightSkyBlue          EQU 0FF87CEFAh
ColorsLightSlateGray        EQU 0FF778899h
ColorsLightSteelBlue        EQU 0FFB0C4DEh
ColorsLightYellow           EQU 0FFFFFFE0h
ColorsLime                  EQU 0FF00FF00h
ColorsLimeGreen             EQU 0FF32CD32h
ColorsLinen                 EQU 0FFFAF0E6h
ColorsMagenta               EQU 0FFFF00FFh
ColorsMaroon                EQU 0FF800000h
ColorsMediumAquamarine      EQU 0FF66CDAAh
ColorsMediumBlue            EQU 0FF0000CDh
ColorsMediumOrchid          EQU 0FFBA55D3h
ColorsMediumPurple          EQU 0FF9370DBh
ColorsMediumSeaGreen        EQU 0FF3CB371h
ColorsMediumSlateBlue       EQU 0FF7B68EEh
ColorsMediumSpringGreen     EQU 0FF00FA9Ah
ColorsMediumTurquoise       EQU 0FF48D1CCh
ColorsMediumVioletRed       EQU 0FFC71585h
ColorsMidnightBlue          EQU 0FF191970h
ColorsMintCream             EQU 0FFF5FFFAh
ColorsMistyRose             EQU 0FFFFE4E1h
ColorsMoccasin              EQU 0FFFFE4B5h
ColorsNavajoWhite           EQU 0FFFFDEADh
ColorsNavy                  EQU 0FF000080h
ColorsOldLace               EQU 0FFFDF5E6h
ColorsOlive                 EQU 0FF808000h
ColorsOliveDrab             EQU 0FF6B8E23h
ColorsOrange                EQU 0FFFFA500h
ColorsOrangeRed             EQU 0FFFF4500h
ColorsOrchid                EQU 0FFDA70D6h
ColorsPaleGoldenrod         EQU 0FFEEE8AAh
ColorsPaleGreen             EQU 0FF98FB98h
ColorsPaleTurquoise         EQU 0FFAFEEEEh
ColorsPaleVioletRed         EQU 0FFDB7093h
ColorsPapayaWhip            EQU 0FFFFEFD5h
ColorsPeachPuff             EQU 0FFFFDAB9h
ColorsPeru                  EQU 0FFCD853Fh
ColorsPink                  EQU 0FFFFC0CBh
ColorsPlum                  EQU 0FFDDA0DDh
ColorsPowderBlue            EQU 0FFB0E0E6h
ColorsPurple                EQU 0FF800080h
ColorsRed                   EQU 0FFFF0000h
ColorsRosyBrown             EQU 0FFBC8F8Fh
ColorsRoyalBlue             EQU 0FF4169E1h
ColorsSaddleBrown           EQU 0FF8B4513h
ColorsSalmon                EQU 0FFFA8072h
ColorsSandyBrown            EQU 0FFF4A460h
ColorsSeaGreen              EQU 0FF2E8B57h
ColorsSeaShell              EQU 0FFFFF5EEh
ColorsSienna                EQU 0FFA0522Dh
ColorsSilver                EQU 0FFC0C0C0h
ColorsSkyBlue               EQU 0FF87CEEBh
ColorsSlateBlue             EQU 0FF6A5ACDh
ColorsSlateGray             EQU 0FF708090h
ColorsSnow                  EQU 0FFFFFAFAh
ColorsSpringGreen           EQU 0FF00FF7Fh
ColorsSteelBlue             EQU 0FF4682B4h
ColorsTan                   EQU 0FFD2B48Ch
ColorsTeal                  EQU 0FF008080h
ColorsThistle               EQU 0FFD8BFD8h
ColorsTomato                EQU 0FFFF6347h
ColorsTransparent           EQU 0FFFFFFh
ColorsTurquoise             EQU 0FF40E0D0h
ColorsViolet                EQU 0FFEE82EEh
ColorsWheat                 EQU 0FFF5DEB3h
ColorsWhite                 EQU 0FFFFFFFFh
ColorsWhiteSmoke            EQU 0FFF5F5F5h
ColorsYellow                EQU 0FFFFFF00h
ColorsYellowGreen           EQU 0FF9ACD32h


GpStatusOk                          EQU 0
GpStatusGenericError                EQU 1
GpStatusInvalidParameter            EQU 2
GpStatusOutOfMemory                 EQU 3
GpStatusObjectBusy                  EQU 4
GpStatusInsufficientBuffer          EQU 5
GpStatusNotImplemented              EQU 6
GpStatusWin32Error                  EQU 7
GpStatusWrongState                  EQU 8
GpStatusAborted                     EQU 9
GpStatusFileNotFound                EQU 10
GpStatusValueOverflow               EQU 11
GpStatusAccessDenied                EQU 12
GpStatusUnknownImageFormat          EQU 13
GpStatusFontFamilyNotFound          EQU 14
GpStatusFontStyleNotFound           EQU 15
GpStatusNotTrueTypeFont             EQU 16
GpStatusUnsupportedGdiplusVersion   EQU 17
GpStatusGdiplusNotInitialized       EQU 18
GpStatusPropertyNotFound            EQU 19
GpStatusPropertyNotSupported        EQU 20


QualityModeInvalid  EQU -1
QualityModeDefault  EQU 0
QualityModeLow      EQU 1
QualityModeHigh     EQU 2


CompositingModeSourceOver   EQU 0
CompositingModeSourceCopy   EQU 1


CompositingQualityInvalid           EQU QualityModeInvalid
CompositingQualityDefault           EQU QualityModeDefault
CompositingQualityHighSpeed         EQU QualityModeLow
CompositingQualityHighQuality       EQU QualityModeHigh
CompositingQualityGammaCorrected    EQU 3
CompositingQualityAssumeLinear      EQU 4


GenericFontFamilySerif      EQU 0
GenericFontFamilySansSerif  EQU 1
GenericFontFamilyMonospace  EQU 2


FontStyleRegular    EQU 0
FontStyleBold       EQU 1
FontStyleItalic     EQU 2
FontStyleBoldItalic EQU 3
FontStyleUnderline  EQU 4
FontStyleStrikeout  EQU 8

SmoothingModeInvalid        EQU QualityModeInvalid
SmoothingModeDefault        EQU QualityModeDefault
SmoothingModeHighSpeed      EQU QualityModeLow
SmoothingModeHighQuality    EQU QualityModeHigh
SmoothingModeNone           EQU 3
SmoothingModeAntiAlias      EQU 4

FillModeAlternate   EQU 0
FillModeWinding     EQU 1

InterpolationModeInvalid                EQU QualityModeInvalid
InterpolationModeDefault                EQU QualityModeDefault
InterpolationModeLowQuality             EQU QualityModeLow
InterpolationModeHighQuality            EQU QualityModeHigh
InterpolationModeBilinear               EQU 3
InterpolationModeBicubic                EQU 4
InterpolationModeNearestNeighbor        EQU 5
InterpolationModeHighQualityBilinear    EQU 6
InterpolationModeHighQualityBicubic     EQU 7


WrapModeTile            EQU 0
WrapModeTileFlipX       EQU 1
WrapModeTileFlipY       EQU 2
WrapModeTileFlipXY      EQU 3
WrapModeClamp           EQU 4

LinearGradientModeHorizontal        EQU 0
LinearGradientModeVertical          EQU 1
LinearGradientModeForwardDiagonal   EQU 2
LinearGradientModeBackwardDiagonal  EQU 3

ImageTypeUnknown    EQU 0
ImageTypeBitmap     EQU 1
ImageTypeMetafile   EQU 2


HatchStyleHorizontal                EQU 0
HatchStyleVertical                  EQU 1
HatchStyleForwardDiagonal           EQU 2
HatchStyleBackwardDiagonal          EQU 3
HatchStyleCross                     EQU 4
HatchStyleDiagonalCross             EQU 5
HatchStyle05Percent                 EQU 6
HatchStyle10Percent                 EQU 7
HatchStyle20Percent                 EQU 8
HatchStyle25Percent                 EQU 9
HatchStyle30Percent                 EQU 10
HatchStyle40Percent                 EQU 11
HatchStyle50Percent                 EQU 12
HatchStyle60Percent                 EQU 13
HatchStyle70Percent                 EQU 14
HatchStyle75Percent                 EQU 15
HatchStyle80Percent                 EQU 16
HatchStyle90Percent                 EQU 17
HatchStyleLightDownwardDiagonal     EQU 18
HatchStyleLightUpwardDiagonal       EQU 19
HatchStyleDarkDownwardDiagonal      EQU 20
HatchStyleDarkUpwardDiagonal        EQU 21
HatchStyleWideDownwardDiagonal      EQU 22
HatchStyleWideUpwardDiagonal        EQU 23
HatchStyleLightVertical             EQU 24
HatchStyleLightHorizontal           EQU 25
HatchStyleNarrowVertical            EQU 26
HatchStyleNarrowHorizontal          EQU 27
HatchStyleDarkVertical              EQU 28
HatchStyleDarkHorizontal            EQU 29
HatchStyleDashedDownwardDiagonal    EQU 30
HatchStyleDashedUpwardDiagonal      EQU 31
HatchStyleDashedHorizontal          EQU 32
HatchStyleDashedVertical            EQU 33
HatchStyleSmallConfetti             EQU 34
HatchStyleLargeConfetti             EQU 35
HatchStyleZigZag                    EQU 36
HatchStyleWave                      EQU 37
HatchStyleDiagonalBrick             EQU 38
HatchStyleHorizontalBrick           EQU 39
HatchStyleWeave                     EQU 40
HatchStylePlaid                     EQU 41
HatchStyleDivot                     EQU 42
HatchStyleDottedGrid                EQU 43
HatchStyleDottedDiamond             EQU 44
HatchStyleShingle                   EQU 45
HatchStyleTrellis                   EQU 46
HatchStyleSphere                    EQU 47
HatchStyleSmallGrid                 EQU 48
HatchStyleSmallCheckerBoard         EQU 49
HatchStyleLargeCheckerBoard         EQU 50
HatchStyleOutlinedDiamond           EQU 51
HatchStyleSolidDiamond              EQU 52
HatchStyleTotal                     EQU 53
HatchStyleLargeGrid                 EQU HatchStyleCross
HatchStyleMin                       EQU HatchStyleHorizontal
HatchStyleMax                       EQU HatchStyleTotal - 1


MatrixOrderPrepend  EQU 0
MatrixOrderAppend   EQU 1


ColorAdjustTypeDefault  EQU 0
ColorAdjustTypeBitmap   EQU 1
ColorAdjustTypeBrush    EQU 2
ColorAdjustTypePen      EQU 3
ColorAdjustTypeText     EQU 4
ColorAdjustTypeCount    EQU 5
ColorAdjustTypeAny      EQU 6

ColorChannelFlagsC          EQU 0
ColorChannelFlagsM          EQU 1
ColorChannelFlagsY          EQU 2
ColorChannelFlagsK          EQU 3
ColorChannelFlagsLast       EQU 4

ColorMatrixFlagsDefault     EQU 0
ColorMatrixFlagsSkipGrays   EQU 1
ColorMatrixFlagsAltGray     EQU 2

PenAlignmentCenter  EQU 0
PenAlignmentInset   EQU 1


BrushTypeSolidColor     EQU 0
BrushTypeHatchFill      EQU 1
BrushTypeTextureFill    EQU 2
BrushTypePathGradient   EQU 3
BrushTypeLinearGradient EQU 4


DashStyleSolid          EQU 0
DashStyleDash           EQU 1
DashStyleDot            EQU 2
DashStyleDashDot        EQU 3
DashStyleDashDotDot     EQU 4
DashStyleCustom         EQU 5

DashCapFlat             EQU 0
DashCapRound            EQU 2
DashCapTriangle         EQU 3

LineCapFlat             EQU 0
LineCapSquare           EQU 1
LineCapRound            EQU 2
LineCapTriangle         EQU 3
LineCapNoAnchor         EQU 010h
LineCapSquareAnchor     EQU 011h
LineCapRoundAnchor      EQU 012h
LineCapDiamondAnchor    EQU 013h
LineCapArrowAnchor      EQU 014h
LineCapCustom           EQU 0FFh
LineCapAnchorMask       EQU 0F0h


CustomLineCapTypeDefault            EQU 0
CustomLineCapTypeAdjustableArrow    EQU 1


LineJoinMiter           EQU 0
LineJoinBevel           EQU 1
LineJoinRound           EQU 2
LineJoinMiterClipped    EQU 3


PenTypeSolidColor       EQU BrushTypeSolidColor
PenTypeHatchFill        EQU BrushTypeHatchFill
PenTypeTextureFill      EQU BrushTypeTextureFill
PenTypePathGradient     EQU BrushTypePathGradient
PenTypeLinearGradient   EQU BrushTypeLinearGradient
PenTypeUnknown          EQU -1


WarpModePerspective     EQU 0
WarpModeBilinear        EQU 1


CombineModeReplace      EQU 0
CombineModeIntersect    EQU 1
CombineModeUnion        EQU 2
CombineModeXor          EQU 3
CombineModeExclude      EQU 4
CombineModeComplement   EQU 5

RotateNoneFlipNone      EQU 0
Rotate90FlipNone        EQU 1
Rotate180FlipNone       EQU 2
Rotate270FlipNone       EQU 3
RotateNoneFlipX         EQU 4
Rotate90FlipX           EQU 5
Rotate180FlipX          EQU 6
Rotate270FlipX          EQU 7
RotateNoneFlipY         EQU Rotate180FlipX
Rotate90FlipY           EQU Rotate270FlipX
Rotate180FlipY          EQU RotateNoneFlipX
Rotate270FlipY          EQU Rotate90FlipX
RotateNoneFlipXY        EQU Rotate180FlipNone
Rotate90FlipXY          EQU Rotate270FlipNone
Rotate180FlipXY         EQU RotateNoneFlipNone
Rotate270FlipXY         EQU Rotate90FlipNone


StringFormatFlagsDirectionRightToLeft   EQU  01h
StringFormatFlagsDirectionVertical      EQU  02h
StringFormatFlagsNoFitBlackBox          EQU  04h
StringFormatFlagsDisplayFormatControl   EQU  020h
StringFormatFlagsNoFontFallback         EQU  0400h
StringFormatFlagsMeasureTrailingSpaces  EQU  0800h
StringFormatFlagsNoWrap                 EQU  01000h
StringFormatFlagsLineLimit              EQU  02000h
StringFormatFlagsNoClip                 EQU  04000h

StringTrimmingNone                  EQU  0
StringTrimmingCharacter             EQU  1
StringTrimmingWord                  EQU  2
StringTrimmingEllipsisCharacter     EQU  3
StringTrimmingEllipsisWord          EQU  4
StringTrimmingEllipsisPath          EQU  5


StringDigitSubstituteUser           EQU  0
StringDigitSubstituteNone           EQU  1
StringDigitSubstituteNational       EQU  2
StringDigitSubstituteTraditional    EQU  3


HotkeyPrefixNone    EQU  0
HotkeyPrefixShow    EQU  1
HotkeyPrefixHide    EQU  2


StringAlignmentNear     EQU  0
StringAlignmentCenter   EQU  1
StringAlignmentFar      EQU  2

FlushIntentionFlush     EQU  0
FlushIntentionSync      EQU  1


EncoderParameterValueTypeByte           EQU  1
EncoderParameterValueTypeASCII          EQU  2
EncoderParameterValueTypeShort          EQU  3
EncoderParameterValueTypeLong           EQU  4
EncoderParameterValueTypeRational       EQU  5
EncoderParameterValueTypeLongRange      EQU  6
EncoderParameterValueTypeUndefined      EQU  7
EncoderParameterValueTypeRationalRange  EQU  8
EncoderValueColorTypeCMYK               EQU  0
EncoderValueColorTypeYCCK               EQU  1
EncoderValueCompressionLZW              EQU  2
EncoderValueCompressionCCITT3           EQU  3
EncoderValueCompressionCCITT4           EQU  4
EncoderValueCompressionRle              EQU  5
EncoderValueCompressionNone             EQU  6
EncoderValueScanMethodInterlaced        EQU  7
EncoderValueScanMethodNonInterlaced     EQU  8
EncoderValueVersionGif87                EQU  9
EncoderValueVersionGif89                EQU  10
EncoderValueRenderProgressive           EQU  11
EncoderValueRenderNonProgressive        EQU  12
EncoderValueTransformRotate90           EQU  13
EncoderValueTransformRotate180          EQU  14
EncoderValueTransformRotate270          EQU  15
EncoderValueTransformFlipHorizontal     EQU  16
EncoderValueTransformFlipVertical       EQU  17
EncoderValueMultiFrame                  EQU  18
EncoderValueLastFrame                   EQU  19
EncoderValueFlush                       EQU  20
EncoderValueFrameDimensionTime          EQU  21
EncoderValueFrameDimensionResolution    EQU  22
EncoderValueFrameDimensionPage          EQU  23
EncoderCompression                      EQU  0
EncoderColorDepth                       EQU  1
EncoderScanMethod                       EQU  2
EncoderVersion                          EQU  3
EncoderRenderMethod                     EQU  4
EncoderQuality                          EQU  5
EncoderTransformation                   EQU  6
EncoderLuminanceTable                   EQU  7
EncoderChrominanceTable                 EQU  8
EncoderSaveFlag                         EQU  9

PixelOffsetModeInvalid                  EQU  QualityModeInvalid
PixelOffsetModeDefault                  EQU  QualityModeDefault
PixelOffsetModeHighSpeed                EQU  QualityModeLow
PixelOffsetModeHighQuality              EQU  QualityModeHigh
PixelOffsetModeNone                     EQU  3
PixelOffsetModeHalf                     EQU  4

TextRenderingHintSystemDefault              EQU  0
TextRenderingHintSingleBitPerPixelGridFit   EQU  1
TextRenderingHintSingleBitPerPixel          EQU  2
TextRenderingHintAntiAliasGridFit           EQU  3
TextRenderingHintAntiAlias                  EQU  4
TextRenderingHintClearTypeGridFit           EQU  5

MetafileTypeInvalid         EQU  0
MetafileTypeWmf             EQU  1
MetafileTypeWmfPlaceable    EQU  2
MetafileTypeEmf             EQU  3
MetafileTypeEmfPlusOnly     EQU  4
MetafileTypeEmfPlusDual     EQU  5


EmfTypeEmfOnly          EQU  MetafileTypeEmf
EmfTypeEmfPlusOnly      EQU  MetafileTypeEmfPlusOnly
EmfTypeEmfPlusDual      EQU  MetafileTypeEmfPlusDual


ObjectTypeInvalid           EQU  0
ObjectTypeBrush             EQU  1
ObjectTypePen               EQU  2
ObjectTypePath              EQU  3
ObjectTypeRegion            EQU  4
ObjectTypeImage             EQU  5
ObjectTypeFont              EQU  6
ObjectTypeStringFormat      EQU  7
ObjectTypeImageAttributes   EQU  8
ObjectTypeCustomLineCap     EQU  9

ObjectTypeMax   EQU  ObjectTypeCustomLineCap
ObjectTypeMin   EQU  ObjectTypeBrush


MetafileFrameUnitPixel      EQU  UnitPixel
MetafileFrameUnitPoint      EQU  UnitPoint
MetafileFrameUnitInch       EQU  UnitInch
MetafileFrameUnitDocument   EQU  UnitDocument
MetafileFrameUnitMillimeter EQU  UnitMillimeter
MetafileFrameUnitGdi        EQU  7


CoordinateSpaceWorld    EQU  0
CoordinateSpacePage     EQU  1
CoordinateSpaceDevice   EQU  2

FlatnessDefault EQU 0.25
ENDIF










 
