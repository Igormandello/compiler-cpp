#include "Parameter.h"

Parameter::Parameter(ParameterType parType, Type type, char name[255], int scope) :
    Symbol(SymbolType_Parameter, name, scope)
{
    this->type = type;
    this->parameterType = parType;
}
