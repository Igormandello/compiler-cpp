#ifndef _PROCEDURE_INCLUDED_

#define _PROCEDURE_INCLUDED_
#include <vector>
#include "Symbol.h"

class Procedure: public Symbol
{
    public:
        Procedure(char[255], int);
        Type getParameter(unsigned int i);
        void addParameter(Type);
    private:
        std::vector<Type> paramatersType;
};

#endif
