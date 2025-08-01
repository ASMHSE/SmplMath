# SmplMath

[![](https://img.shields.io/badge/Assembler-MASM%206.14-brightgreen.svg?style=flat-square&logo=visual-studio-code&logoColor=white&colorB=5E0000)](http://www.masm32.com/download.htm) 
[![](https://img.shields.io/badge/Assembler-UASM%20v2.52-green.svg?style=flat-square&logo=visual-studio-code&logoColor=white&colorB=1CC887)](http://www.terraspace.co.uk/uasm.html) 
[![](https://img.shields.io/badge/Assembler-JWASM%20v2.15-green.svg?style=flat-square&logo=visual-studio-code&logoColor=white&colorB=C9931E)](https://github.com/Baron-von-Riedesel/JWasm) 
[![](https://img.shields.io/badge/Assembler-ML64-blue.svg?style=flat-square&logo=visual-studio-code&logoColor=white&colorB=000093)](https://learn.microsoft.com/en-us/cpp/assembler/masm/masm-for-x64-ml64-exe) 

The SmplMath macros system is a collection of MASM macros for evaluating mathematic expression developed by qWord. There are a whole bunch of macros for expression evaluation, comparison and FPU related topics.

example:
```
     .if fGT(Pb, @fSlv8(710.2740349*0.63))
						
        fSlv8 AaPbB = (spba + spbb /( 1 + (( Pb - spbc)/ spbd)^2)) \ 
                     * ((710.2740349 /  bDNAmx) - ( Pb /  BDNA))\
                     / (1.0 +  ( KAaPbB /  cAa1))
    .else
		
        fSlv8 AaPbB = (spbpa + spbpb * ( Pb ^  spbpc)) \ 
                     * ((710.2740349 /  bDNAmx) - ( Pb /  BDNA))\
                     / (1.0 +  ( KAaPbB /  cAa1))
    .endif
```
Well tested for 32 bits with ___ML___, ___JWASM___, ___ASMC___ (-Znk option until 2.28) and ___UASM___.

In 64 bits was tested with ___JWASM___ by qWord, and recently some testing for ___ML64___ using Masm64 SDK (see [Masm32.com](http://masm32.com/board/index.php?board=53.0)). 

Currently most projects are Neutral Bitness (same code for 32 and 64 bits) with ___UASM___.


## Additional macros

  ### Complementary macros 

___Loops___ and ___Compound operators___ macros simplify in a controled way programming some calculations. ___Pseudo push/pop___ facilitate translations between 32 and 64 bits and allow dual bitness (See [ObjAsm-C.1](https://github.com/ObjAsm/ObjAsm-C.1)). ___Conditional flow___ macros solve some problems with floating point numbers comparisons and other early macro expansions.

  ### Accesory macros 

These include some ___additional___ macros provided by qWord, and an ___invoke___ macro modified from Steve Hutchesson's [Masm64 SDK](http://masm32.com/board/index.php?board=53.0).
