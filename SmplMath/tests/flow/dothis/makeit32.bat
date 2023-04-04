@echo off

    if exist "dothis32.obj" del "dothis32.obj"
    if exist "dothis32.exe" del "dothis32.exe"

    \masm32\bin\ml /c /coff "dothis32.asm"
    if errorlevel 1 goto errasm

    \masm32\bin\PoLink /SUBSYSTEM:CONSOLE "dothis32.obj"
    if errorlevel 1 goto errlink
    dir "dothis32.*"
    goto TheEnd

  :errlink
    echo _
    echo Link error
    goto TheEnd

  :errasm
    echo _
    echo Assembly Error
    goto TheEnd
    
  :TheEnd

pause
