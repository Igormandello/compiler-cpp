#ifndef _VARIABLE_INCLUDED_

#define _VARIABLE_INCLUDED_
#include "enums/Type.h"

class Variable: public Symbol
{
    public:
        Variable(Type, char[255], int);
    private:
        Type type;
};

#endif
