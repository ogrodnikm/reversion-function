#pragma once

#include "pg_list.h"

/*C-style interface*/

/**
* Converts the List (pg_list) into string. 
* - Order of elements in output string is reversed. 
* - There is '.' character in between elements.
* e.g. input: List(ListCell(Ident(name:"MyApp"))->ListCell(Ident(name:"MyModule"))->ListCell(Ident(name:"MyTable"))->null)
* output: "MyTable.MyModule.MyApp"
* 
* @param	List	list	pg list
* @return	wchar_t*		c-style string
*/
wchar_t* reversion_function(const List* list);
/**
* Non-const version of reversion_function. Allowed to change the list.
*/
wchar_t* reversion_function(List* list);
