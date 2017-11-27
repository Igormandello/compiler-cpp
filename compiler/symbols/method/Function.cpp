#include <iostream>
#include <stdexcept>
#include "Function.h"

using namespace Symbols;
Function::Function(Type returnType, char name[255], int scope) throw() : Method(SymbolType_Function, name, scope)
{
    this->returnType = returnType;
}

Type Function::getReturnType() const throw()
{
    return this->returnType;
}
