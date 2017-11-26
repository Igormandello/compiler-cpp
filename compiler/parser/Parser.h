#ifndef _PARSER_INCLUDED_

#define _PARSER_INCLUDED_
#include <vector>
#include "../lexer/Lexer.h"
#include "../symbols/SymbolTable.h"
#include "../symbols/Variable.h"

class Parser
{
  public:
    Parser(char*);
    ~Parser();
    void compileProgram();
  private:
    Lexer* lexer;
    Symbols::SymbolTable* symbolTable;
    bool redeclaredVariable(char*);
    void addVariables(std::vector<Symbols::Variable*>, SliceType);
    void compileProgramInit();
    void compileVariable();
    void compileProcedure();
    void compileFunction();
    void compileMethodVariables();
    void compileMain();
    void compileIf();
    void compileWhile();
    void compileCommand();
    void compileCompoundCommand(bool);
};

#endif
