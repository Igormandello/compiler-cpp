#ifndef _PARAMETER_INCLUDED_

#define _PARAMETER_INCLUDED_
#include "Symbol.h"
#include "enums/ParameterType.h"

namespace Symbols
{
    class Parameter: public Symbol
    {
        public:
            Parameter(ParameterType, Type, char[255], int);
        private:
            Type type;
            ParameterType parameterType;
    };
}

#endif
