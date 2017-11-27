#include "LocalVariable.h"

using namespace Symbols;
LocalVariable::LocalVariable(Type type, char name[255], int scope) throw() : Variable(SymbolType_Variable, type, name, scope)
{}
