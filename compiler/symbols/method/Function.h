#ifndef _FUNCTION_INCLUDED_

#define _FUNCTION_INCLUDED_
#include "../Method.h"

namespace Symbols
{
    class Function: public Method
    {
        public:
            Function(Type, char[255], int) throw();
            Type getReturnType() const throw();
            void setReturnType(Type) throw();
        private:
            Type returnType;
    };
}

#endif
