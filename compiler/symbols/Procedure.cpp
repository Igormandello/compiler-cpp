#include "Procedure.h"

Procedure::Procedure(char name[255], int scope) : Symbol(SymbolType_Procedure, name, scope)
{}

Type Procedure::getParameter(unsigned int i)
{
    this->paramatersType[i];
}

void Procedure::addParameter(Type type)
{
    this->paramatersType.push_back(type);
}
