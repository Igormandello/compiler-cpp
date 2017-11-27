#ifndef _LOCAL_VARIABLE_INCLUDED_

#define _LOCAL_VARIABLE_INCLUDED_
#include "../Variable.h"

namespace Symbols
{
    class LocalVariable: public Variable
    {
        public:
            LocalVariable(Type, char[255], int) throw();
    };
}

#endif
