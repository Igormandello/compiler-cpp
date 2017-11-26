#include "Procedure.h"

using namespace Symbols;
Procedure::Procedure(char name[255], int scope) : Symbol(SymbolType_Procedure, name, scope + 1)
{}

Type Procedure::getParameter(unsigned int i)
{
    this->paramatersType[i];
}

void Procedure::addParameter(Type type)
{
    this->paramatersType.push_back(type);
}
