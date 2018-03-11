#pragma once

enum NodeTag
{
	T_Invalid = 0,

	/*
	 * TAGS FOR PRIMITIVE NODES (primnodes.h)
	 */
	T_Alias = 300,
	T_RangeVar,
	T_NamedArgExpr,
	T_SubLink,
	T_CaseExpr,
	T_CaseWhen,
	T_RowExpr,
	T_CoalesceExpr,
	T_MinMaxExpr,
	T_XmlExpr,
	T_NullTest,
	T_BooleanTest,
	T_SetToDefault,
	T_JoinExpr,
	T_IntoClause,
	T_LimitOffset,
	T_HintJoinOrder,
	T_HintIndex,

	/*
	 * TAGS FOR MEMORY NODES (memnodes.h)
	 */
	T_AllocSetContext = 600,

	/*
	 * TAGS FOR VALUE NODES (value.h)
	 */
	T_Value = 650,

	/*
	 * TAGS FOR LIST NODES (pg_list.h)
	 */
	T_List = 670,
	T_IntList,
	T_OidList,

	/*
	 * TAGS FOR STATEMENT NODES (mostly in parsenodes.h)
	 */
	T_InsertStmt = 700,
	T_DeleteStmt,
	T_UpdateStmt,
	T_SelectStmt,
	T_AlterTableStmt,
	T_AlterTableCmd,
	T_AlterDomainStmt,
	T_CreateStmt,
	T_DefineStmt,
	T_DropStmt,
	T_DropIndexStmt,
	T_TruncateStmt,
	T_IndexStmt,
	T_TransactionStmt,
	T_ViewStmt,
	T_CreateDomainStmt,
	T_MapAction,
	T_CreateMapStmt,
	T_ApplyMapStmt,
	T_CreatedbStmt,
	T_DropdbStmt,
	T_ExplainStmt,
	T_CreateSeqStmt,
	T_AlterSeqStmt,
	T_DiscardStmt,
	T_CreateTrigStmt,
	T_DropPropertyStmt,
	T_LockStmt,
	T_ConstraintsSetStmt,
	T_ReindexStmt,
	T_CheckPointStmt,
	T_PrepareStmt,
	T_ExecuteStmt,
	T_DeallocateStmt,
	T_DropOwnedStmt,
	T_ReassignOwnedStmt,

	/*
	 * TAGS FOR PARSE TREE NODES (parsenodes.h)
	 */
	T_A_Expr = 900,
	T_Ident,
	T_IdentWithGenerics,
	T_ParamRef,
	T_FuncCall,
	T_A_Star,
	T_A_Indices,
	T_A_Indirection,
	T_A_ArrayExpr,
	T_ResTarget,
	T_TypeCast,
	T_CollateClause,
	T_SortBy,
	T_WindowDef,
	T_RangeSubselect,
	T_RangeFunction,
	T_TypeName,
	T_ColumnDef,
	T_IndexElem,
	T_Constraint,
	T_DefElem,
	T_InhRelation,
	T_LockingClause,
	T_XmlSerialize,
	T_WithClause,
	T_CommonTableExpr,
};
