#ifndef _PROCEDURE_INCLUDED_

#define _PROCEDURE_INCLUDED_
#include "../Method.h"

namespace Symbols
{
    class Procedure: public Method
    {
        public:
            Procedure(char[255], int) throw();
    };
}

#endif
