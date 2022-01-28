# SmplMath
The SmplMath macros system is a collection of MASM macros for evaluating mathematic expression developed by qWord. There is a whole bunch of macros for expression evaluation, comparison and FPU related topics.

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
Well tested for 32 bits with ___ML___, ___JWASM___, ___ASMC___ (-Znk option) and ___UASM32___.

In 64 bits was tested with ___JWASM___ by qWord, and recently some test for ___ML64___ using Masm64SDK (see Masm32.com).


## Additional macros

Some added macros facilitate programming:

  ### Complementary macros 

     ___Loop___ and ___Compound operators__ macros make easy in a controled way programming some calculations. ___Pseudo push/pop___ facilitate translations 32-64 bits an allow dual bitness (See ObjAsm). ___Conditional flow___ macros solve some problems with floating point numbers comparisons and other early macro expansions.

  ### Accesory macros 

     These include some ___additional___ macros provided by qWord, and ___invoke___ macro modified fron Steve Hutchesson's Masm64 SDK.