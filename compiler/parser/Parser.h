#ifndef _PARSER_INCLUDED_

#define _PARSER_INCLUDED_
#include "../lexer/Lexer.h"

class Parser
{
  public:
    Parser(Lexer l);
    void compileProgramInit();
    void compileVariable();
    void compileProcedure();
    void compileFunction();
    void compileMain();
    void compileIf();
    void compileWhile();
    void compileCompoundCommand();
  private:
    Lexer lexer;
};

#endif
