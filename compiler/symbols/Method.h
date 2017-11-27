#ifndef _METHOD_INCLUDED_

#define _METHOD_INCLUDED_
#include <stdexcept>
#include <vector>
#include "Symbol.h"

namespace Symbols
{
    class Method: public Symbol
    {
        public:
            Method(SymbolType, char[255], int) throw();
            Type getParameter(unsigned int i) const throw(std::invalid_argument);
            int getParametersCount() const throw();
            void addParameter(Type) throw();
        private:
            std::vector<Type> parametersType;
    };
}

#endif
