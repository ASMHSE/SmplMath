@echo off

    if exist "Loop32.obj" del "Loop32.obj"
    if exist "Loop32.exe" del "Loop32.exe"

    \masm32\bin\ml /c /coff "Loop32.asm"
    if errorlevel 1 goto errasm

    \masm32\bin\PoLink /SUBSYSTEM:CONSOLE "Loop32.obj"
    if errorlevel 1 goto errlink
    dir "Loop32.*"
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
