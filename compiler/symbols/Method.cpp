#include <stdexcept>
#include "Method.h"

using namespace Symbols;
Method::Method(SymbolType type, char name[255], int scope) throw() : Symbol(type, name, scope + 1)
{}

Type Method::getParameter(unsigned int i) const throw(std::invalid_argument)
{
    return this->parametersType[i];
}

int Method::getParametersCount() const throw()
{
    return this->parametersType.size();
}

void Method::addParameter(Type type) throw()
{
    this->parametersType.push_back(type);
}
