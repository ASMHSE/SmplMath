;/********************************************************
; * This file is part of the SmplMath macros-system.     *
; *                                                      *
; *          Copyright by qWord, 2011-2014               *
; *                                                      *
; *          SmplMath.Masm{at}gmx{dot}net                *
; *    http://sourceforge.net/projects/smplmath/         *
; *                                                      *
; *  Further Copyright and Legal statements are located  *
; *  in the documentation (Documentation.pdf).           *
; *                                                      *
; ********************************************************/


;
; OPEN DOUBLY-LINKED LISTS:
;
; exsis if: IFDEF LL_&Name&_cnt
; empty lists not allowed - they must have at least one node.
; 
; LL_&Name&_cnt         = number of current used nodes (>= 1)
; LL_&Name&_iFirst      = index of first node in list
; LL_&Name&_iLast       = index of last node in list
; LL_&Name&_nFree       = number of currently unused nodes - free nodes come up through deletion of nodes.
;                         The indexes of free nodes are stored in constans that are referenc by LL_&Name&_nFree:
;
;                         i = LL_&Name&_nFree
;
;                         LL_&Name&_free_&i& = index of last deleted node
;                         ...
;                         LL_&Name&_free_2
;                         LL_&Name&_free_1   = first deleted node
;
; NODES: a node consist of an backwards and a forward link and one or more textmacros:
;
;       LL_&Name&_&iNode&_bLink         = index of previous node
;       LL_&Name&_&iNode&_fLink         = index of next node
;       LL_&Name&_&iNode&_&iDataRecord& = text-macro, the first data record is guaranteed to exist after node creation.
;                                         Further records are possible (iDataRecord > 1) but not affected by the ll_* macros.
;
; All indexes are one-based (i >= 1). A b/fLink-value of 0 marks a node as the first/last one in list.
;


;------------------------------------------------------------------
; creates an open doubly-linked list.
; LLName:	name of list
; nNodes:	number of nodes to create (>= 1)
;------------------------------------------------------------------
ll_create macro LLName:req,nNodes:=<1>
	LL_&LLName&_iFirst = 1
	LL_&LLName&_iLast  = nNodes
	LL_&LLName&_cnt    = nNodes
	LL_&LLName&_nFree  = 0	
	cll_cntr = 1
	REPEAT nNodes
		@CatStr(<LL_&LLName&_>,%cll_cntr,<_bLink>) = cll_cntr - 1
		@CatStr(<LL_&LLName&_>,%cll_cntr,<_fLink>) = cll_cntr + 1
		@CatStr(<LL_&LLName&_>,%cll_cntr,<_1>) TEXTEQU <>
		cll_cntr = cll_cntr + 1
	ENDM
	@CatStr(<LL_&LLName&_>,%(cll_cntr-1),<_fLink>) = 0
endm


;-------------------------------------------------------------------
; inserts an new node before or after given one.
; iNode:			index of current node
; bInsertBefore:	specific how to insert new created node:
;                   0 = insert after iNode
;                   1 = insert before iNode
; RETURN:			index of new created node
;-------------------------------------------------------------------
ll_insert macro LLName:req,iNode:req,bInsertBefore:=<1>
	
	LL_&LLName&_cnt = LL_&LLName&_cnt + 1
	IF LL_&LLName&_nFree NE 0
		lli_cntr = @CatStr(<LL_&LLName&_free_>,%LL_&LLName&_nFree)
		LL_&LLName&_nFree = LL_&LLName&_nFree - 1
	ELSE
		lli_cntr = LL_&LLName&_cnt
	ENDIF
	IF bInsertBefore NE 0
		lli_blink =	@CatStr(<LL_&LLName&_>,%(iNode),<_bLink>)
		LL_&LLName&_&iNode&_bLink = lli_cntr
		@CatStr(<LL_&LLName&_>,%lli_cntr,<_bLink>) = lli_blink
		@CatStr(<LL_&LLName&_>,%lli_cntr,<_fLink>) = iNode
		@CatStr(<LL_&LLName&_>,%lli_cntr,<_1>) TEXTEQU <>
		IF lli_blink NE 0
			@CatStr(<LL_&LLName&_>,%lli_blink,<_fLink>) = lli_cntr
		ELSE
			LL_&LLName&_iFirst = lli_cntr
		ENDIF
		EXITM %lli_cntr
	ENDIF
	lli_flink =	@CatStr(<LL_&LLName&_>,%(iNode),<_fLink>)
	LL_&LLName&_&iNode&_fLink = lli_cntr
	@CatStr(<LL_&LLName&_>,%lli_cntr,<_fLink>) = lli_flink
	@CatStr(<LL_&LLName&_>,%lli_cntr,<_bLink>) = iNode
	@CatStr(<LL_&LLName&_>,%lli_cntr,<_1>) TEXTEQU <>
	IF lli_flink NE 0
		@CatStr(<LL_&LLName&_>,%lli_flink,<_bLink>) = lli_cntr
	ELSE
		LL_&LLName&_iLast = lli_cntr
	ENDIF
	EXITM %lli_cntr
endm

ll_append macro LLName:req
	LL_&LLName&_cnt = LL_&LLName&_cnt + 1
	IF LL_&LLName&_nFree NE 0
		lla_cntr = @CatStr(<LL_&LLName&_free_>,%LL_&LLName&_nFree)
		LL_&LLName&_nFree = LL_&LLName&_nFree - 1
	ELSE
		lla_cntr = LL_&LLName&_cnt
	ENDIF
	@CatStr(<LL_&LLName&_>,%LL_&LLName&_iLast,<_fLink>) = lla_cntr
	@CatStr(<LL_&LLName&_>,%lla_cntr,<_fLink>) = 0
	@CatStr(<LL_&LLName&_>,%lla_cntr,<_bLink>) = LL_&LLName&_iLast
	@CatStr(<LL_&LLName&_>,%lla_cntr,<_1>) TEXTEQU <>
	LL_&LLName&_iLast = lla_cntr
	EXITM %lla_cntr
endm

;-------------------------------------------------------------------
; move a node in list (cut&past)
; iNode:			index of node to cut out
; iDestNode:		index of node used for insertion
; bInsertBefore:	specific how to insert node:
;                   0 = insert after iDestNode
;                   1 = insert before iDestNode
; RETURN:			iNode or -1 if an error occurs
;-------------------------------------------------------------------
ll_move macro LLName:req,iNode:req,iDestNode:req,bInsertBefore:=<1>
	IF iNode EQ iDestNode
	%	EXITM <&iNode>
	ENDIF

	lld_blink = LL_&LLName&_&iNode&_bLink
	lld_flink = LL_&LLName&_&iNode&_fLink
	IF lld_blink NE 0
		@CatStr(<LL_&LLName&_>,%lld_blink,<_fLink>) = lld_flink
		IF lld_flink NE 0
			@CatStr(<LL_&LLName&_>,%lld_flink,<_bLink>) = lld_blink
		ELSE
			LL_&LLName&_iLast = lld_blink
		ENDIF	
	ELSE
		IF lld_flink NE 0
			@CatStr(<LL_&LLName&_>,%lld_flink,<_bLink>) = 0
			LL_&LLName&_iFirst = lld_flink
		ELSE
			echo ERR: ll_move: TODO
			EXITM <-1>
		ENDIF
	ENDIF
	IF bInsertBefore NE 0
		lli_blink =	@CatStr(<LL_&LLName&_>,%(iDestNode),<_bLink>)
		@CatStr(<LL_&LLName&_>,%(iDestNode),<_bLink>) = iNode
		@CatStr(<LL_&LLName&_>,%(iNode),<_bLink>) = lli_blink
		@CatStr(<LL_&LLName&_>,%(iNode),<_fLink>) = iDestNode
		IF lli_blink NE 0
			@CatStr(<LL_&LLName&_>,%lli_blink,<_fLink>) = iNode
		ELSE
			LL_&LLName&_iFirst = iNode
		ENDIF
	%	EXITM <&iNode>
	ENDIF
:Z	
	lli_flink =	@CatStr(<LL_&LLName&_>,%(iDestNode),<_fLink>)
	@CatStr(<LL_&LLName&_>,%(iDestNode),<_fLink>) = iNode
	@CatStr(<LL_&LLName&_>,%(iNode),<_fLink>) = lli_flink
	@CatStr(<LL_&LLName&_>,%(iNode),<_bLink>) = iDestNode
	IF lli_flink NE 0
		@CatStr(<LL_&LLName&_>,%lli_flink,<_bLink>) = iNode
	ELSE
		LL_&LLName&_iLast = iNode
	ENDIF
%	EXITM <&iNode>
	
endm


;------------------------------------------------------------
; deletes given node.
; iNode:			node to delete
; bReturnNextNode:	specific which index to return:
;					0 = previous node
;					1 = next node
;------------------------------------------------------------
ll_delete macro LLName:req,iNode:req,bReturnNextNode:=<1>
	
	lld_blink = @CatStr(<LL_&LLName&_>,%(iNode),<_bLink>)
	lld_flink = @CatStr(<LL_&LLName&_>,%(iNode),<_fLink>)
	IF lld_blink NE 0
		@CatStr(<LL_&LLName&_>,%lld_blink,<_fLink>) = lld_flink
		IF lld_flink NE 0
			@CatStr(<LL_&LLName&_>,%lld_flink,<_bLink>) = lld_blink
		ELSE
			LL_&LLName&_iLast = lld_blink
		ENDIF	
	ELSE
		IF lld_flink NE 0
			@CatStr(<LL_&LLName&_>,%lld_flink,<_bLink>) = 0
			LL_&LLName&_iFirst = lld_flink
		ELSE
			echo ERR: ll_delete(LLName,iNode): empty lists not allowed
			EXITM <-1>
		ENDIF
	ENDIF
	LL_&LLName&_cnt = LL_&LLName&_cnt - 1
	LL_&LLName&_nFree = LL_&LLName&_nFree + 1
	@CatStr(<LL_&LLName&_free_>,%LL_&LLName&_nFree) = iNode	
	IF bReturnNextNode NE 0
		EXITM %lld_flink
	ELSE
		EXITM %lld_blink
	ENDIF
endm

ll_get_next_free macro LLName:req
	IF LL_&LLName&_nFree NE 0
		lli_cntr = @CatStr(<LL_&LLName&_free_>,%LL_&LLName&_nFree)
	ELSE
		lli_cntr = LL_&LLName&_cnt+1
	ENDIF
	EXITM %lli_cntr
endm

;-----------------------------------------------
; get index of next node in list.
; iNode: current node
;-----------------------------------------------
ll_get_next macro LLName:req,iNode:req
	EXITM %LL_&LLName&_&iNode&_fLink
endm

;-----------------------------------------------
; get index of previous node in list.
; iNode: current node
;-----------------------------------------------
ll_get_previous macro LLName:req,iNode:req
	EXITM %LL_&LLName&_&iNode&_bLink
endm

ll_get_next_data macro LLName:req,iNode:req,iDataRecord:=<1>
	IF LL_&LLName&_&iNode&_fLink
	%	EXITM <@CatStr(<LL_&LLName&_>,%LL_&LLName&_&iNode&_fLink,<_>,iDataRecord)>
	ENDIF
	EXITM <>
endm

;-----------------------------------------------
; get data record of a node.
; iNode: 		current node
; iDataRecord:	index of requested record
;-----------------------------------------------
ll_get_data macro LLName:req,iNode:req,iDataRecord:=<1>
	EXITM LL_&LLName&_&iNode&_&iDataRecord&
endm

;-----------------------------------------------
; set data record of a node.
; iNode: 		current node
; iDataRecord:	index of record to store
; Data:			string to store
;-----------------------------------------------
ll_set_data macro LLName:req,iNode:req,iDataRecord:=<1>,Data:=<>
	LL_&LLName&_&iNode&_&iDataRecord& TEXTEQU <&Data>
endm

ll_get_first macro LLName:req
	EXITM %LL_&LLName&_iFirst
endm

ll_get_last macro LLName:req
	EXITM %LL_&LLName&_iLast
endm

ll_get_cnt macro LLName:req
	EXITM %LL_&LLName&_cnt	
endm

ll_is_defined macro LLName,iNode,iDataRecord
	IFDEF LL_&LLName&_&iNode&_&iDataRecord&
		EXITM <1>
	ENDIF
	EXITM <0>	
endm

ll_cmp_next macro LLName:req,iNode:req,iDataRecord:req,literal:=<>
	llcn_flink=LL_&LLName&_&iNode&_fLink
	IF llcn_flink
		IFIDN <&literal>,@CatStr(<LL_&LLName&_>,%llcn_flink,<_&iDataRecord&>)
			EXITM <1>
		ENDIF
	ENDIF
	EXITM <0>
endm

ll_get macro LLName:req,iNode:req,nCnt:=<1>,iDataRecord:=<1>,bGetData:=<0>,DefLit:=<>
	llg_flink = iNode
	llg_cntr = nCnt
	WHILE llg_flink NE 0 AND llg_cntr NE 0
		llg_cntr = llg_cntr - 1
		llg_flink = ll_get_next(LLName,%llg_flink)
	ENDM
	IF llg_cntr EQ 0 AND llg_flink NE 0
		IF bGetData EQ 0
			EXITM %llg_flink
		ELSE
			IF ll_is_defined(LLName,%llg_flink,iDataRecord)
			%	EXITM <@CatStr(<LL_&LLName&_>,%llg_flink,<_&iDataRecord&>)>
			ELSE
				EXITM <&DefLit>
			ENDIF
		ENDIF
	ENDIF
	IF bGetData EQ 0
		EXITM <0>
	ELSE
		EXITM <&DefLit>	
	ENDIF	
endm

ll_get_index macro LLName:req,iNode:req
	llgi_flink = LL_&LLName&_iFirst
	llgi_cntr = 1
	WHILE llgi_flink
		IF llgi_flink EQ iNode
			EXITM
		ENDIF
		llgi_flink = ll_get_next(LLName,%llgi_flink)
		llgi_cntr=llgi_cntr+1
	ENDM
	EXITM %llgi_flink
endm

ll_or_value macro LLName:req,iNode:req,iDataRecord:=<1>,msk:=<0>
	llov_tmp = LL_&LLName&_&iNode&_&iDataRecord&					; for jwasm
	LL_&LLName&_&iNode&_&iDataRecord& TEXTEQU %llov_tmp OR msk
endm

ll_inc_value macro LLName:req,iNode:req,iDataRecord:=<1>
	lliv_tmp = LL_&LLName&_&iNode&_&iDataRecord&+1					; for jwasm
	LL_&LLName&_&iNode&_&iDataRecord& TEXTEQU %lliv_tmp
	EXITM LL_&LLName&_&iNode&_&iDataRecord&
endm

ll_dec_value macro LLName:req,iNode:req,iDataRecord:=<1>
	lldv_tmp = LL_&LLName&_&iNode&_&iDataRecord&-1					; for jwasm
	LL_&LLName&_&iNode&_&iDataRecord& TEXTEQU %lldv_tmp
	EXITM LL_&LLName&_&iNode&_&iDataRecord&
endm

ll_bitTest_next macro LLName,iNode,iDataRecord,value:=<0>
	llbt_flink=LL_&LLName&_&iNode&_fLink
	IF llbt_flink
		EXITM %(@CatStr(<LL_&LLName&_>,%llbt_flink,<_&iDataRecord&>) AND (value))
	ENDIF
	EXITM <0>
endm

ll_bitTest_previous macro LLName,iNode,iDataRecord,value:=<0>
	llbt_blink=LL_&LLName&_&iNode&_bLink
	IF llbt_flink
		EXITM %(@CatStr(<LL_&LLName&_>,%llbt_blink,<_&iDataRecord&>) AND (value))
	ENDIF
	EXITM <0>
endm 

ll_tm_search macro LLName:req,iDataRecord:=<1>,literal
	llts_flink = LL_&LLName&_iFirst
	llts_flg = 0
	llts_cntr = 1
	WHILE llts_flink
		IF ll_is_defined(LLName,%llts_flink,iDataRecord)
		%	IFIDN <&literal>,<@CatStr(<LL_&LLName&_>,%llts_flink,<_&iDataRecord&>)>
				llts_flg = 1
				EXITM
			ENDIF
		ENDIF
		llts_cntr = llts_cntr + 1
		llts_flink = ll_get_next(LLName,%llts_flink)
	ENDM
	IF llts_flg
		EXITM %llts_flink
	ENDIF
	EXITM <0>	
endm

ll_clear_data macro LLName:req,nNode:=<1>,nDataRecord:=<1>
	llcd_cntr = 1
	REPEAT nNode
		llcd_cntr2 = 1
		REPEAT nDataRecord
			llcd_cid LLName,%llcd_cntr,%llcd_cntr2
			llcd_cntr2=llcd_cntr2+1
		ENDM
		llcd_cntr=llcd_cntr+1		
	ENDM	
endm
llcd_cid macro LLName:req,iNode:req,iDataRecord:req
	IFDEF LL_&LLName&_&iNode&_&iDataRecord&
		LL_&LLName&_&iNode&_&iDataRecord& TEXTEQU <>
	ENDIF
endm

ll_stream_tm macro LLName:req,iNode,iDataRecord:req,DefLit:=<>,tmNames:VARARG
	IFB <iNode>
		llst_flink = ll_get_first(LLName)
	ELSE
		llst_flink = iNode
	ENDIF
	FOR tm,<&tmNames>
		IF llst_flink
			tm TEXTEQU @CatStr(<LL_&LLName&_>,%llst_flink,<_&iDataRecord&>)
			llst_flink = ll_get_next(LLName,%llst_flink)
		ELSE
			tm TEXTEQU <&DefLit>
		ENDIF
	ENDM
	EXITM %llst_flink
endm

ll_stream_const macro LLName:req,iNode,iDataRecord:req,DefVal:=<0>,constNames:VARARG
	IFB <iNode>
		llst_flink = ll_get_first(LLName)
	ELSE
		llst_flink = iNode
	ENDIF
	FOR const,<&constNames>
		IF llst_flink
			const = @CatStr(<LL_&LLName&_>,%llst_flink,<_&iDataRecord&>)
			llst_flink = ll_get_next(LLName,%llst_flink)
		ELSE
			const = DefVal
		ENDIF
	ENDM
	EXITM %llst_flink
endm

ll_stream_links macro LLName:req,iNode,linkNames:VARARG
	IFB <iNode>
		llst_flink = ll_get_first(LLName)
	ELSE
		llst_flink = iNode
	ENDIF
	FOR link,<&linkNames>
		IF llst_flink
			link = llst_flink
			llst_flink = ll_get_next(LLName,%llst_flink)
		ELSE
			link = 0
		ENDIF
	ENDM
	EXITM %llst_flink
endm


; shows a list in the build-console
; LLName:			name of list
; nDataRecords:		number of columns to show
; ColumnWidth:		global column width
; bTblEx:			show iNode,fLink and bLink of each node
; ClmnOptn:			a list of options for each column:
;					width=0...80	overwrite global column width
;					hex={0,1}		assumes data of current column are numeric - show them in hexadecimal notation
;					ralgin={0,1}	align current cells content right
;                   tmt={0,1}       <internal - do not use>
;                   If a column has more than one option, enclose them with angle brackets:
;                   ...,<>,<<width=5,ralgin=1>>,...
;                   (<> is needed, if a column has no option)
;
ll_DebugOut macro LLName:req,nDataRecords:=<1>,ColumnWidth:=<8>,bTblEx:=<0>,ClmnOptn:VARARG
	IFNDEF LL_&LLName&_cnt
		echo ------ linked list debug out -----------
		echo linked list <&LLName> does not exist
		echo ----------------------------------------
		EXITM
	ENDIF
	
	lldo_cntr = 1
	FOR clmn,<&ClmnOptn>
		IFNB <&clmn>
			FOR optn,<&clmn>
				IFNB <%optn>
					@CatStr(<lldo_co_>,%lldo_cntr,<_&optn>)
				ENDIF
			ENDM
		ENDIF
		lldo_cntr = lldo_cntr + 1
	ENDM
	
	lldo_cntr = LL_&LLName&_iFirst
	
	IF @CatStr(<LL_&LLName&_>,%lldo_cntr,<_bLink>) NE 0
		echo first list entry has an backward link => LL_&LLName&_iFirst is invalid
	ENDIF

	lldo_ralign = 0
	lldo_columns TEXTEQU <|i   |>
	lldo_txt3	 TEXTEQU <+----+>
	IF bTblEx
		lldo_columns CATSTR lldo_columns,<node|blnk|flnk|>
		lldo_txt3 TEXTEQU lldo_txt3,<----+----+----+>
	ENDIF
	lldo_txt4	 SUBSTR <-------------------------------------------------------------------------------->,1,ColumnWidth
	lldo_spc 	 SUBSTR <                                                                                >,1,ColumnWidth
	lldo_txt4	 CATSTR lldo_txt4,<+>
	lldo_cntr2 = 1
	REPEAT nDataRecords
		lldo_txt TEXTEQU %lldo_cntr2				
		lldo_size = @SizeStr(<%lldo_txt>)
		IF lldo_is_optn_def(%lldo_cntr2,<width>)
			lldo_clmnw = @CatStr(<lldo_co_>,%lldo_cntr2,<_width>)
			IF lldo_clmnw EQ -1
				lldo_clmnw = ColumnWidth
			ENDIF
		ELSE
			lldo_clmnw = ColumnWidth
		ENDIF
		IF lldo_size GT lldo_clmnw AND lldo_clmnw
			IF lldo_is_optn_def(%lldo_cntr2,<ralign>,<lldo_ralign>) AND lldo_ralign NE 0
				lldo_txt SUBSTR lldo_txt,lldo_size-lldo_clmnw+1,lldo_clmnw
			ELSE
				lldo_txt SUBSTR lldo_txt,1,lldo_clmnw
			ENDIF
		ELSEIF lldo_clmnw EQ 0
				lldo_txt TEXTEQU <>		
		ELSE
			IF lldo_size NE lldo_clmnw
				IF lldo_is_optn_def(%lldo_cntr2,<width>)
					lldo_txt5 SUBSTR <                                                                                >,1,lldo_clmnw
					IF lldo_is_optn_def(%lldo_cntr2,<ralign>,<lldo_ralign>) AND lldo_ralign NE 0
						lldo_txt CATSTR @SubStr(<%lldo_txt5>,%lldo_size+1),lldo_txt
					ELSE
						lldo_txt CATSTR lldo_txt,@SubStr(<%lldo_txt5>,%lldo_size+1)
					ENDIF				
				ELSE
					IF lldo_is_optn_def(%lldo_cntr2,<ralign>,<lldo_ralign>) AND lldo_ralign NE 0
						lldo_txt CATSTR @SubStr(<%lldo_spc>,%lldo_size+1),lldo_txt
					ELSE
						lldo_txt CATSTR lldo_txt,@SubStr(<%lldo_spc>,%lldo_size+1)
					ENDIF					
				ENDIF
			ENDIF
		ENDIF
		lldo_txt6 SUBSTR <-------------------------------------------------------------------------------->,1,lldo_clmnw
		lldo_txt3 CATSTR lldo_txt3,lldo_txt6,<+>
		
		lldo_columns CATSTR lldo_columns,lldo_txt,<|>
		lldo_cntr2 = lldo_cntr2 + 1
	ENDM
	%echo lldo_columns
	%echo lldo_txt3

	lldo_index = 1
	REPEAT LL_&LLName&_cnt
		lldo_i CATSTR %lldo_index,@SubStr(<   >,%@SizeStr(%@CatStr(%lldo_index)))
		lldo_columns TEXTEQU <|>,lldo_i,<|>
		IF bTblEx
			lldo_t_n CATSTR @SubStr(<   >,%@SizeStr(%@CatStr(%lldo_cntr))),%lldo_cntr
			lldo_t_b CATSTR @SubStr(<   >,%@SizeStr(%@CatStr(%(@CatStr(<LL_&LLName&_>,%lldo_cntr,<_bLink>))))),%(@CatStr(<LL_&LLName&_>,%lldo_cntr,<_bLink>))
			lldo_t_f CATSTR @SubStr(<   >,%@SizeStr(%@CatStr(%(@CatStr(<LL_&LLName&_>,%lldo_cntr,<_fLink>))))),%(@CatStr(<LL_&LLName&_>,%lldo_cntr,<_fLink>))
			lldo_columns CATSTR lldo_columns,lldo_t_n,<|>,lldo_t_b,<|>,lldo_t_f,<|>
		ENDIF			
		lldo_cntr2 = 1
		REPEAT nDataRecords
			IF lldo_is_tm_def(&LLName,%lldo_cntr,%lldo_cntr2)
				lldo_txt TEXTEQU @CatStr(<LL_&LLName&_>,%lldo_cntr,<_>,%lldo_cntr2)
				IF lldo_is_optn_def(%lldo_cntr2,<hex>)
					IF @CatStr(<lldo_co_>,%lldo_cntr2,<_hex>) NE 0
						lldo_hex=lldo_txt
						pushcontext radix
						.radix 16
						lldo_txt TEXTEQU %lldo_hex
						popcontext radix
					ENDIF						
				ELSE ;; bugfix for masm
					IF lldo_is_optn_def(%lldo_cntr2,<tmt>)
						lldo_txt TEXTEQU lldo_show_tmt_flags(%lldo_txt)
					ENDIF
				ENDIF
			ELSE
				lldo_txt TEXTEQU <!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!>
			ENDIF				
			lldo_size = @SizeStr(<%lldo_txt>)
			
			IF lldo_is_optn_def(%lldo_cntr2,<width>)
				lldo_clmnw = @CatStr(<lldo_co_>,%lldo_cntr2,<_width>)
				IF lldo_clmnw EQ -1
					lldo_clmnw = ColumnWidth
				ENDIF
			ELSE
				lldo_clmnw = ColumnWidth
			ENDIF
			IF lldo_size GT lldo_clmnw AND lldo_clmnw
				IF lldo_is_optn_def(%lldo_cntr2,<ralign>,<lldo_ralign>) AND lldo_ralign NE 0
					lldo_txt SUBSTR lldo_txt,lldo_size-lldo_clmnw+1,lldo_clmnw
				ELSE
					lldo_txt SUBSTR lldo_txt,1,lldo_clmnw
				ENDIF
			ELSEIF lldo_clmnw EQ 0
				lldo_txt TEXTEQU <>
			ELSE
				IF lldo_size NE lldo_clmnw
					IF lldo_is_optn_def(%lldo_cntr2,<width>)
						lldo_txt5 SUBSTR <                                                                                >,1,lldo_clmnw
						IF lldo_is_optn_def(%lldo_cntr2,<ralign>,<lldo_ralign>) AND lldo_ralign NE 0
							lldo_txt CATSTR @SubStr(<%lldo_txt5>,%lldo_size+1),lldo_txt
						ELSE
							lldo_txt CATSTR lldo_txt,@SubStr(<%lldo_txt5>,%lldo_size+1)
						ENDIF				
					ELSE
						IF lldo_is_optn_def(%lldo_cntr2,<ralign>,<lldo_ralign>) AND lldo_ralign NE 0
							lldo_txt CATSTR @SubStr(<%lldo_spc>,%lldo_size+1),lldo_txt
						ELSE
							lldo_txt CATSTR lldo_txt,@SubStr(<%lldo_spc>,%lldo_size+1)
						ENDIF					
					ENDIF
				ENDIF
			ENDIF				
			lldo_columns CATSTR lldo_columns,lldo_txt,<|>
			lldo_cntr2 = lldo_cntr2 + 1
		ENDM
		%echo lldo_columns
		;%echo lldo_txt3
	:Z
		lldo_cntr = @CatStr(<LL_&LLName&_>,%lldo_cntr,<_fLink>)
		lldo_index = lldo_index + 1
	ENDM

	lldo_cntr = 1
	FOR clmn,<&ClmnOptn>
		IFNB <&clmn>
			FOR optn,<&clmn>
				IFNB <%optn>
					IFIDNI @SubStr(<&optn>,1,@InStr(1,<&optn>,<=>)-1),<width>
						@CatStr(<lldo_co_>,%lldo_cntr,<_>,@SubStr(<&optn>,1,@InStr(1,<&optn>,<=>)-1),<=-1>)
					ELSE
						@CatStr(<lldo_co_>,%lldo_cntr,<_>,@SubStr(<&optn>,1,@InStr(1,<&optn>,<=>)-1),<=0>)
					ENDIF
				ENDIF
			ENDM
		ENDIF
		lldo_cntr = lldo_cntr + 1
	ENDM	
endm

; used by ll_DebugOut
lldo_is_tm_def macro LLName,iNode,iDataRecord
	IFDEF LL_&LLName&_&iNode&_&iDataRecord&
		EXITM <1>
	ENDIF
	EXITM <0>	
endm

; used by ll_DebugOut
lldo_is_optn_def macro num,optn,var
	IFDEF lldo_co_&num&_&optn&
		IFNB <var>
			var = lldo_co_&num&_&optn&
		ENDIF
		EXITM <1>
	ENDIF
	IFNB <var>
		var = 0
	ENDIF
	EXITM <0>
endm

; used by ll_DebugOut
lldo_show_tmt_flags macro num:req
	lldstf_txt TEXTEQU <>
	lldstf_num = num
	WHILE lldstf_num NE 0
		lldstf_match = 0
		IF lldstf_num AND TMT_TXT
			lldstf_txt CATSTR lldstf_txt,< TXT>
			lldstf_num = lldstf_num XOR TMT_TXT
			lldstf_match = 1
		ELSEIF lldstf_num AND TMT_EXPR
			lldstf_txt CATSTR lldstf_txt,< EXPR>
			lldstf_num = lldstf_num XOR TMT_EXPR
			lldstf_match = 1
		ELSEIF lldstf_num AND TMT_SINGLE_ARG
			lldstf_txt CATSTR lldstf_txt,< SINGLE_ARG>
			lldstf_num = lldstf_num XOR TMT_SINGLE_ARG
			lldstf_match = 1
		ELSEIF lldstf_num AND TMT_FLT
			lldstf_txt CATSTR lldstf_txt,< FLT>
			lldstf_num = lldstf_num XOR TMT_FLT
			lldstf_match = 1
		ELSEIF lldstf_num AND TMT_INT
			lldstf_txt CATSTR lldstf_txt,< INT>
			lldstf_num = lldstf_num XOR TMT_INT
			lldstf_match = 1
		ELSEIF lldstf_num AND TMT_END_OF_EXPR
			lldstf_txt CATSTR lldstf_txt,< END_OF_EXPR>
			lldstf_num = lldstf_num XOR TMT_END_OF_EXPR
			lldstf_match = 1
		ELSEIF lldstf_num AND TMT_BEGIN_EXPR
			lldstf_txt CATSTR lldstf_txt,< BEGIN_EXPR>
			lldstf_num = lldstf_num XOR TMT_BEGIN_EXPR
			lldstf_match = 1
		ELSEIF lldstf_num AND TMT_OPRT
			lldstf_txt CATSTR lldstf_txt,< OPRT>
			lldstf_num = lldstf_num XOR TMT_OPRT
			lldstf_match = 1
		ELSEIF lldstf_num AND TMT_MINUS
			lldstf_txt CATSTR lldstf_txt,< MINUS>
			lldstf_num = lldstf_num XOR TMT_MINUS
			lldstf_match = 1
		ELSEIF lldstf_num AND TMT_PLUS
			lldstf_txt CATSTR lldstf_txt,< PLUS>
			lldstf_num = lldstf_num XOR TMT_PLUS
			lldstf_match = 1
		ELSEIF lldstf_num AND TMT_NEG
			lldstf_txt CATSTR lldstf_txt,< NEG>
			lldstf_num = lldstf_num XOR TMT_NEG
			lldstf_match = 1
		ELSEIF lldstf_num AND TMT_COMMA
			lldstf_txt CATSTR lldstf_txt,< COMMA>
			lldstf_num = lldstf_num XOR TMT_COMMA
			lldstf_match = 1
		ELSEIF lldstf_num AND TMT_PUSH
			lldstf_txt CATSTR lldstf_txt,< PUSH>
			lldstf_num = lldstf_num XOR TMT_PUSH
			lldstf_match = 1
		ELSEIF lldstf_num AND TMT_POP
			lldstf_txt CATSTR lldstf_txt,< POP>
			lldstf_num = lldstf_num XOR TMT_POP
			lldstf_match = 1
		ELSEIF lldstf_num AND TMT_STO
			lldstf_txt CATSTR lldstf_txt,< STO>
			lldstf_num = lldstf_num XOR TMT_STO
			lldstf_match = 1
		ELSEIF lldstf_num AND TMT_REC
			lldstf_txt CATSTR lldstf_txt,< REC>
			lldstf_num = lldstf_num XOR TMT_REC
			lldstf_match = 1
		ELSEIF lldstf_num AND TMT_FNC
			lldstf_txt CATSTR lldstf_txt,< FNC>
			lldstf_num = lldstf_num XOR TMT_FNC
			lldstf_match = 1
		ELSEIF lldstf_num AND TMT_PI
			lldstf_txt CATSTR lldstf_txt,< PI>
			lldstf_num = lldstf_num XOR TMT_PI
			lldstf_match = 1
		ELSEIF lldstf_num AND TMT_SINGLE_FLT
			lldstf_txt CATSTR lldstf_txt,< SINGLE_FLT>
			lldstf_num = lldstf_num XOR TMT_SINGLE_FLT
			lldstf_match = 1
		ELSEIF lldstf_num AND TMT_SINGLE_INT
			lldstf_txt CATSTR lldstf_txt,< SINGLE_INT>
			lldstf_num = lldstf_num XOR TMT_SINGLE_INT
			lldstf_match = 1
		ELSEIF lldstf_num AND TMT_ST
			lldstf_txt CATSTR lldstf_txt,< ST>
			lldstf_num = lldstf_num XOR TMT_ST
			lldstf_match = 1
		ELSEIF lldstf_num AND TMT_ASSIGN
			lldstf_txt CATSTR lldstf_txt,< ASSIGN>
			lldstf_num = lldstf_num XOR TMT_ASSIGN
			lldstf_match = 1
		ELSEIF lldstf_num AND TMT_OPTMZTN
			lldstf_txt CATSTR lldstf_txt,< OPTMZTN>
			lldstf_num = lldstf_num XOR TMT_OPTMZTN
			lldstf_match = 1
		ENDIF
		IFE lldstf_match
			IF lldstf_num NE 0
				lldstf_txt CATSTR <Invalid>,lldstf_txt
			ENDIF
			EXITM
		ENDIF
	ENDM
	EXITM lldstf_txt
endm