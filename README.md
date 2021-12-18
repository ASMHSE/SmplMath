# SmplMath
The SmplMath macros system is a collection of MASM macros for evaluating mathematic expression developed by qWord. There is a whole bunch of macros for expression evaluation, comparison and FPU related topics.

example:

     .if __fGT__(Pb, __@fSlv8__(710.2740349*0.63))
						
        __fSlv8__ AaPbB = (spba + spbb /( 1 + (( Pb - spbc)/ spbd)^2)) \ 
                     * ((710.2740349 /  bDNAmx) - ( Pb /  BDNA))\
                     / (1.0 +  ( KAaPbB /  cAa1))
    .else
		
        __fSlv8__ AaPbB = (spbpa + spbpb * ( Pb ^  spbpc)) \ 
                     * ((710.2740349 /  bDNAmx) - ( Pb /  BDNA))\
                     / (1.0 +  ( KAaPbB /  cAa1))
    .endif

Well tested for 32 bits with ML, JWASM, ASMC (-Znk option) and UASM32.

In 64 bits was tested with JWASM by qWord, and recently some test for ML64 using Masm64SDK (see Masm32.com).
