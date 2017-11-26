#ifndef _FUNCTION_INCLUDED_

#define _FUNCTION_INCLUDED_
#include <vector>
#include "Symbol.h"

class Function: public Symbol
{
    public:
        Function(char[255], int);
        Type getParameter(unsigned int i);
        void addParameter(Type);
    private:
        std::vector<Type> paramatersType;
        Type returnType;
};

#endif
