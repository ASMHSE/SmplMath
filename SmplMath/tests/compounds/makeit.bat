@echo off

    if exist "bitwise.obj" del "bitwise.obj"
    if exist "bitwise.exe" del "bitwise.exe"

    \masm32\bin\ml /c /coff "bitwise.asm"
    if errorlevel 1 goto errasm

    \masm32\bin\PoLink /SUBSYSTEM:CONSOLE "bitwise.obj"
    if errorlevel 1 goto errlink
    dir "bitwise.*"
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
