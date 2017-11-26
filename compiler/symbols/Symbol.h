#ifndef _SYMBOL_INCLUDED_

#define _SYMBOL_INCLUDED_
#include "../lexer/SliceType.h"
#include "enums/SymbolType.h"
#include "enums/Type.h"

namespace Symbols
{
    class Symbol
    {
        public:
            Symbol(SymbolType, char[], int);
            SymbolType getSymbolType() const;
            const char* getName() const;
            unsigned int getScope() const;
        private:
            SymbolType   type;
            char         name [255];
            unsigned int scope;
    };
}

#endif
