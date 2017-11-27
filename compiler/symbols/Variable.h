#ifndef _VARIABLE_INCLUDED_

#define _VARIABLE_INCLUDED_
#include "Symbol.h"

namespace Symbols
{
    class Variable: public Symbol
    {
        public:
            Variable(SymbolType, Type, char[255], int) throw();
            Type getType() const throw();
        private:
            Type type;
    };
}

#endif
