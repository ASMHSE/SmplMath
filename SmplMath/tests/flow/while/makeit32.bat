@echo off

    if exist "SmplF32.obj" del "SmplF32.obj"
    if exist "SmplF32.exe" del "SmplF32.exe"

    \masm32\bin\ml /c /coff "SmplF32.asm"
    if errorlevel 1 goto errasm

    \masm32\bin\PoLink /SUBSYSTEM:CONSOLE "SmplF32.obj"
    if errorlevel 1 goto errlink
    dir "SmplF32.*"
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
