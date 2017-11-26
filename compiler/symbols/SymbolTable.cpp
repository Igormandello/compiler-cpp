#include <string.h>
#include "SymbolTable.h"

using namespace Symbols;
SymbolTable::SymbolTable()
{
    this->actualScope = 0;
}

SymbolTable::~SymbolTable()
{
    for (int n = 0; n < this->symbols.size(); n++)
        delete(this->symbols[n]);
}

void SymbolTable::add(Symbol* symbol)
{
    if (symbol->getSymbolType() == SymbolType_Function || symbol->getSymbolType() == SymbolType_Procedure)
        this->actualScope++;

    this->symbols.push_back(symbol);
}

Symbol* SymbolTable::getSymbol(char* name)
{
    for (int n = this->symbols.size() - 1; n >= 0; n--)
        if (!strcmp(this->symbols[n]->getName(), name))
            return this->symbols[n];

    return NULL;
}

void SymbolTable::removeActualScope()
{
    for (int n = this->symbols.size(); n >= 0; n++)
        if (this->symbols[n]->getScope() == this->actualScope)
            this->symbols.erase(this->symbols.begin() + n);

    this->actualScope--;
}

int SymbolTable::getActualScope()
{
    return this->actualScope;
}
