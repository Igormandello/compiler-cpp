#include <string.h>
#include "Symbol.h"

using namespace Symbols;
Symbol::Symbol(SymbolType type, char name[255], int scope) throw()
{
    this->type = type;
    strcpy(this->name, name);
    this->scope = scope;
}

SymbolType Symbol::getSymbolType() const throw()
{
    return this->type;
}

const char* Symbol::getName() const throw()
{
    return this->name;
}

unsigned int Symbol::getScope() const throw()
{
    return this->scope;
}
