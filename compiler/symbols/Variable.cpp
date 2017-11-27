#include "Variable.h"

using namespace Symbols;
Variable::Variable(SymbolType sType, Type type, char name[255], int scope) throw() : Symbol(sType, name, scope)
{
    this->type = type;
}

Type Variable::getType() const throw()
{
    return this->type;
}
