#include "Function.h"

using namespace Symbols;
Function::Function(Type returnType, char name[255], int scope) : Symbol(SymbolType_Function, name, scope + 1)
{
    this->returnType = returnType;
}

Type Function::getParameter(unsigned int i)
{
    this->parametersType[i];
}

int Function::getParametersCount()
{
    return this->parametersType.size();
}

void Function::addParameter(Type type)
{
    this->parametersType.push_back(type);
}
