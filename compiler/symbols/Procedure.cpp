#include "Procedure.h"

using namespace Symbols;
Procedure::Procedure(char name[255], int scope) : Symbol(SymbolType_Procedure, name, scope + 1)
{}

Type Procedure::getParameter(unsigned int i)
{
    this->parametersType[i];
}

int Procedure::getParametersCount()
{
    return this->parametersType.size();
}

void Procedure::addParameter(Type type)
{
    this->parametersType.push_back(type);
}
