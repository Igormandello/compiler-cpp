#ifndef _SYMBOL_INCLUDED_

#define _SYMBOL_INCLUDED_
#include "../lexer/SliceType.h"
#include "enums/SymbolType.h"
#include "enums/Type.h"

class Symbol
{
    public:
        Symbol(SymbolType, char[], int);
        //SymbolType getSymbolType();
        //char[] getName();
        //unsigned int getScope();
    private:
        SymbolType   type;
        char         name [255];
        unsigned int scope;
};

#endif
