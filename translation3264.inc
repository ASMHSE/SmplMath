@reg32_64 macro reg_32, reg_64
    if @WordSize eq 4
      @CatStr(<_>,&reg_32) textequ <reg_32>
      @CatStr(<__>,&reg_32) textequ <reg_32>
      @CatStr(<r_>,&reg_32) textequ <reg_32>
    else
      @CatStr(<_>,&reg_32) textequ <reg_64>
      @CatStr(<__>,&reg_32) textequ <@CatStr(&reg_64,<d>)>
      @CatStr(<r_>,&reg_32) textequ <reg_64>
    endif 
endm

@reg32_64d macro reg_32, reg_64
    if @WordSize eq 4
      @CatStr(<__>,&reg_32) textequ <reg_32>
    else
      @CatStr(<__>,&reg_32) textequ <reg_64>
    endif 
endm

@if64bits macro RegAux
    if @WordSize eq 4
        exitm <>
    else
        exitm <&RegAux>
    endif    
endm

