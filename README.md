# reversion-function
Implements function that converts Postgress list to string in reversed order.

Postgres list input: (pseudo-text representing the list of structs)
```
List(ListCell(Ident(name:"MyApp"))->ListCell(Ident(name:"MyModule"))->ListCell(Ident(name:"MyTable"))->null)a
```
wchar_t string output:
```
MyTable.MyModule.MyApp
```

Compatible with Visual Studio 2017 and CMake.

Tested under:
* Windows (MSVC-1912)
* Linux (gcc-7)
* Linux (clang-6)

Solution has been generalized to provide support for number of possible input containers and string types as depicted in
unittest/reversion_function_impl_test.cpp

Source directory structur:
* external/ contains provided headers, along with one new, used mostly in prototype and tests
* prototype/ contains binary target with various options of implementation
* src/ contains ready solution
* unittest/ contains unit tests (GTest)

Install directory structure:
* bin/ contains prototype binary
* include/ contains library includes
* lib/ contains library
* test/ contains unit tests
