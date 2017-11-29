#ifndef _SYMBOL_TABLE_INCLUDED_

#define _SYMBOL_TABLE_INCLUDED_
#include <vector>
#include "Symbol.h"

namespace Symbols
{
    class SymbolTable
    {
        public:
            SymbolTable() throw();
            ~SymbolTable() throw();
            void add(Symbol*) throw();
            void addScope() throw();
            Symbol* getSymbol(char*) const throw();
            void removeActualScope() throw();
            int getActualScope() const throw();
        private:
            std::vector<Symbol*> symbols;
            unsigned int actualScope;
    };
}

#endif
