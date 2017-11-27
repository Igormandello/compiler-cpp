#ifndef _VARIABLE_INCLUDED_

#define _VARIABLE_INCLUDED_
#include "Symbol.h"

namespace Symbols
{
    class Variable: public Symbol
    {
        public:
            Variable(Type, char[255], int);
            Type getType();
        private:
            Type type;
    };
}

#endif
