@echo off

    if exist "mabm2.obj" del "mabm2.obj"
    if exist "mabm2.exe" del "mabm2.exe"

    \masm32\bin\ml /c /coff "mabm2.asm"
    if errorlevel 1 goto errasm

    \masm32\bin\PoLink /SUBSYSTEM:CONSOLE "mabm2.obj"
    if errorlevel 1 goto errlink
    dir "mabm2.*"
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
