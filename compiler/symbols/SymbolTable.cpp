#include <string.h>
#include "SymbolTable.h"

SymbolTable::SymbolTable()
{
    this->actualScope = 0;
}

SymbolTable::~SymbolTable()
{
    for (Symbol* s : this->symbols)
        delete(s);
}

void SymbolTable::add(Symbol* symbol)
{
    if (symbol->getSymbolType() == SymbolType_Function || symbol->getSymbolType() == SymbolType_Procedure)
        this->actualScope++;

    this->symbols.push_back(symbol);
}

Symbol* SymbolTable::getSymbol(char* name)
{
    for (Symbol* s : this->symbols)
        if (!strcmp(s->getName(), name))
            return s;

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
