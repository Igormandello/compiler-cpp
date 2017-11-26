#include "Variable.h"

Variable::Variable(Type type, char name[255], int scope) : Symbol(SymbolType_Variable, name, scope)
{
    this->type = type;
}
