#ifndef _PARAMETER_INCLUDED_

#define _PARAMETER_INCLUDED_
#include "../Variable.h"
#include "../enums/ParameterType.h"

namespace Symbols
{
    class Parameter: public Variable
    {
        public:
            Parameter(ParameterType, Type, char[255], int) throw();
            ParameterType getParameterType() const throw();
        private:
            ParameterType parameterType;
    };
}

#endif
