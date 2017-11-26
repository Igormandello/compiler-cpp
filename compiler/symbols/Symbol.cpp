#include <string.h>
#include "Symbol.h"

using namespace Symbols;
Symbol::Symbol(SymbolType type, char name[255], int scope)
{
    this->type = type;
    strcpy(this->name, name);
    this->scope = scope;
}

SymbolType Symbol::getSymbolType() const
{
    return this->type;
}

const char* Symbol::getName() const
{
    return this->name;
}

unsigned int Symbol::getScope() const
{
    return this->scope;
}
