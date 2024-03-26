; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

    include \masm32\include64\masm64rt.inc
    ifndef @WordSize
        @WordSize equ 8
    endif

    include \macros\SmplMath\math.inc
    include \macros\SmplMath\accs\macros.inc
    include \macros\SmplMath\accs\Macros64G.inc
    
; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

comment * -----------------------------------------------------
                     Build this console app with
                  "MAKEIT64.BAT" on the PROJECT menu.
        ----------------------------------------------------- *

    .data?
      value dd ?

    .data
      item dd 0
      NaN  REAL8 7FF8000000000000r
      uno  real8 1.0
      dos  real8 2.0
    .code

   
; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤
 entry_point proc
    call main
    waitkey
    .exit
 entry_point endp
; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

main proc

    LOCAL pMsg      :DWORD
    LOCAL wParam    :DWORD
    LOCAL lParam    :DWORD
    local alor1     :real8
    local alor2     :real8
    local alor3     :real8
    
    local fSlvTLS()

    conout "Inicia",lf

    mov wParam, 2
    mov pMsg, WM_PAINT
    mov lParam, 3

    fSlv8 alor1 = 1.5 
    fSlv8 alor2 = 2.2
    fSlv8 alor3 = NaN

    @if wParam gt 0
          conout "  ----------------------------------------", lf
          conout "  algo hay ", lf
          conout "  ----------------------------------------", lf
      @if lParam ne 3 || pMsg eq WM_COMMAND
          conout "  pasa OR ", lf

        @if wParam ua 2 || lParam ub 5
          conout "  ----------------------------------------", lf
          conout "  Ain't Mabdelouahab's runtime comparisons great", lf
          conout "  ----------------------------------------", lf
        @endif
      @elseif alor1 fGT uno && alor2 fNE dos
        @if alor3 r8Is NotNaN
          conout "  ----------------------------------------", lf
          conout "  Is && working? Yes.  And is NotNaN", lf
          conout "  ----------------------------------------", lf
        @else
          conout "  ----------------------------------------", lf
          conout "  Is && working? Yes.  Ah... is NaN", lf
          conout "  ----------------------------------------", lf
        @endif
      @elseif alor1 fGT 1.0 || alor2 fLT 4.0
          conout "  ----------------------------------------", lf
          conout "  Is OR working? Yes", lf
          conout "  ----------------------------------------", lf
      @else
          conout "  ----------------------------------------", lf
          conout "  Nothing ", lf
          conout "  ----------------------------------------", lf
      @endif
    @else
          conout "  ----------------------------------------", lf
          conout "  Nada ", lf
          conout "  ----------------------------------------", lf
    @endif

    mov rax, 1
    @if rax
          conout "  ----------------------------------------", lf
          conout "  TEST used ", lf
          conout "  ----------------------------------------", lf
    @endif

    mov r10, 1
    mov r11, 2
    @if r10 ub r11
          conout "  ----------------------------------------", lf
          conout "  CMP used ", lf
          conout "  ----------------------------------------", lf
    @endif

    ret

main endp

; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

end
