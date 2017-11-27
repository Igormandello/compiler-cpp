#include <string.h>
#include "SymbolTable.h"

using namespace Symbols;
SymbolTable::SymbolTable() throw()
{
    this->actualScope = 0;
}

SymbolTable::~SymbolTable() throw()
{
    for (int n = 0; n < this->symbols.size(); n++)
        delete(this->symbols[n]);
}

void SymbolTable::add(Symbol* symbol) throw()
{
    if (symbol->getSymbolType() == SymbolType_Function || symbol->getSymbolType() == SymbolType_Procedure)
        this->actualScope++;

    this->symbols.push_back(symbol);
}

void SymbolTable::addScope() throw()
{
    this->actualScope++;
}

Symbol* SymbolTable::getSymbol(char* name, bool local) const throw()
{
    if (local)
    {
        for (int n = this->symbols.size() - 1; n >= 0; n--)
            if (!strcmp(this->symbols[n]->getName(), name))
                if (this->symbols[n]->getScope() == this->actualScope
                     || this->symbols[n]->getScope() == 0)
                    return this->symbols[n];
    }
    else
        for (int n = this->symbols.size() - 1; n >= 0; n--)
            if (!strcmp(this->symbols[n]->getName(), name))
                return this->symbols[n];

    return NULL;
}

void SymbolTable::removeActualScope() throw()
{
    for (int n = this->symbols.size(); n >= 0; n++)
        if (this->symbols[n]->getScope() == this->actualScope)
            this->symbols.erase(this->symbols.begin() + n);

    this->actualScope--;
}

int SymbolTable::getActualScope() const throw()
{
    return this->actualScope;
}
