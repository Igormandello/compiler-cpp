#include <stdexcept>
#include "Procedure.h"

using namespace Symbols;
Procedure::Procedure(char name[255], int scope) throw() : Method(SymbolType_Procedure, name, scope)
{}
