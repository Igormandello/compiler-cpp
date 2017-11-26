#include "Function.h"

Function::Function(char name[255], int scope) : Symbol(SymbolType_Function, name, scope + 1)
{}

Type Function::getParameter(unsigned int i)
{
    this->paramatersType[i];
}

void Function::addParameter(Type type)
{
    this->paramatersType.push_back(type);
}
