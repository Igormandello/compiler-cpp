#ifndef _FUNCTION_INCLUDED_

#define _FUNCTION_INCLUDED_
#include <vector>
#include "Symbol.h"

namespace Symbols
{
    class Function: public Symbol
    {
        public:
            Function(Type, char[255], int);
            Type getReturnType();
            Type getParameter(unsigned int i);
            int getParametersCount();
            void addParameter(Type);
        private:
            std::vector<Type> parametersType;
            Type returnType;
    };
}

#endif
