#include "Parameter.h"

using namespace Symbols;
Parameter::Parameter(ParameterType parType, Type type, char name[255], int scope) throw() :
    Variable(SymbolType_Parameter, type, name, scope)
{
    this->parameterType = parType;
}

ParameterType Parameter::getParameterType() const throw()
{
    return this->parameterType;
}
