#ifndef _SYMBOL_TYPE_H_INCLUDED_

#define _SYMBOL_TYPE_H_INCLUDED_

namespace Symbols
{
    typedef
        enum
        {
            SymbolType_Variable,
            SymbolType_Procedure,
            SymbolType_Function,
            SymbolType_Parameter
        }
        SymbolType;
}

#endif
