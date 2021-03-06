#include "reversion_function.h"
#include "reversion_function_impl.h"
#include "adapters/pg_list_adapter.h"

wchar_t* reversion_function(const List* list)
{
	PgListConstAdapter adapter(list);
	return reversion_function(adapter);
}

wchar_t* reversion_function(List* list)
{
	PgListAdapter adapter(list);
	return reversion_function(adapter);
}

