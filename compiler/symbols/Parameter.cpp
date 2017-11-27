#include "Parameter.h"

using namespace Symbols;
Parameter::Parameter(ParameterType parType, Type type, char name[255], int scope) :
    Symbol(SymbolType_Parameter, name, scope)
{
    this->type = type;
    this->parameterType = parType;
}

Type Parameter::getType()
{
    return this->type;
}

ParameterType Parameter::getParameterType()
{
    return this->parameterType;
}
