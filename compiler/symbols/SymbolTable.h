#ifndef _SYMBOL_TABLE_INCLUDED_

#define _SYMBOL_TABLE_INCLUDED_
#include <vector>
#include "Symbol.h"

namespace Symbols
{
    class SymbolTable
    {
        public:
            SymbolTable();
            ~SymbolTable();
            void add(Symbol*);
            Symbol* getSymbol(char*);
            void removeActualScope();
            int getActualScope();
        private:
            std::vector<Symbol*> symbols;
            unsigned int actualScope;
    };
}

#endif
