#pragma once

#include "pg_list.h"

/*C-style interface*/
wchar_t* reversion_function(const List* list);
// Non-const version is allowed to reverse the list
wchar_t* reversion_function(List* list);
