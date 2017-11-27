#include "Variable.h"

using namespace Symbols;
Variable::Variable(Type type, char name[255], int scope) : Symbol(SymbolType_Variable, name, scope)
{
    this->type = type;
}

Type Variable::getType()
{
    return this->type;
}
