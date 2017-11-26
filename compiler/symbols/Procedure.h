#ifndef _PROCEDURE_INCLUDED_

#define _PROCEDURE_INCLUDED_
#include <vector>
#include "Symbol.h"

namespace Symbols
{
    class Procedure: public Symbol
    {
        public:
            Procedure(char[255], int);
            Type getParameter(unsigned int i);
            int getParametersCount();
            void addParameter(Type);
        private:
            std::vector<Type> parametersType;
    };
}

#endif
