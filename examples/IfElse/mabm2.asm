; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤
    include \masm32\include\masm32rt.inc
    .686
    .XMM
    ifndef @WordSize
        @WordSize equ 4
    endif
    include \masm32\macros\SmplMath\Math.inc
 ;   include MabMod.inc
; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

comment * -----------------------------------------------------
                     Build this console app with
                  "MAKEIT.BAT" on the PROJECT menu.
        ----------------------------------------------------- *

    .data?
      value dd ?

    .data
      item dd 0
      NaN  REAL8 7FF8000000000000r
      uno  real8 1.0
      dos  real8 2.0
    .code

start:
   
; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

    call main
    inkey
    exit

; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

main proc

    LOCAL pMsg      :DWORD
    LOCAL wParam    :DWORD
    LOCAL lParam    :DWORD
    local alor1     :real8
    local alor2     :real8
    local alor3     :real8
    
    local fSlvTLS()

    cls

    mov wParam, 2
    mov pMsg, WM_PAINT
    mov lParam, 3

    fSlv8 alor1 = 1.5 
    fSlv8 alor2 = 2.2
    fSlv8 alor3 = NaN

    @if wParam gt 0
          print "  ----------------------------------------",13,10
          print "  algo hay ",13,10
          print "  ----------------------------------------",13,10
      @if lParam ne 3 || pMsg eq WM_COMMAND
          print "  pasa OR ",13,10

        @if wParam ua 2 || lParam ub 5
          print "  ----------------------------------------",13,10
          print "  Ain't Mabdelouahab's runtime comparisons great",13,10
          print "  ----------------------------------------",13,10
        @endif
      @elseif alor1 fGT uno && alor2 fNE dos
        @if alor3 r8Is NotNaN
          print "  ----------------------------------------",13,10
          print "  Is && working? Yes.  And is NotNaN",13,10
          print "  ----------------------------------------",13,10
        @else
          print "  ----------------------------------------",13,10
          print "  Is && working? Yes.  Ah... is NaN",13,10
          print "  ----------------------------------------",13,10
        @endif
      @elseif alor1 fGT 1.0 || alor2 fLT 4.0
          print "  ----------------------------------------",13,10
          print "  Is OR working? Yes",13,10
          print "  ----------------------------------------",13,10
      @else
          print "  ----------------------------------------",13,10
          print "  Nothing ",13,10
          print "  ----------------------------------------",13,10
      @endif
    @else
          print "  ----------------------------------------",13,10
          print "  Nada ",13,10
          print "  ----------------------------------------",13,10
    @endif

    mov eax, 1
    @if eax
          print "  ----------------------------------------",13,10
          print "  TEST used ",13,10
          print "  ----------------------------------------",13,10
    @endif

    mov eax, 1
    mov ecx, 2
    @if eax ub ecx
          print "  ----------------------------------------",13,10
          print "  CMP used ",13,10
          print "  ----------------------------------------",13,10
    @endif

    ret

main endp

; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

end start
