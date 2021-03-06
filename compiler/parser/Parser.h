#ifndef _PARSER_INCLUDED_

#define _PARSER_INCLUDED_
#include <vector>
#include "../lexer/Lexer.h"
#include "../symbols/SymbolTable.h"
#include "../symbols/Method.h"
#include "../symbols/method/Procedure.h"
#include "../symbols/method/Function.h"
#include "../symbols/Variable.h"
#include "../symbols/variable/Parameter.h"
#include "../symbols/variable/LocalVariable.h"

class Parser
{
  public:
    Parser(char*) throw(std::invalid_argument);
    Parser(const Parser&) throw();
    ~Parser() throw();
    void compileProgram() const throw(std::runtime_error);
  private:
    Lexer* lexer;
    Symbols::SymbolTable* symbolTable;
    bool redeclaredVariable(char*) const;
    void addVariables(std::vector<Symbols::LocalVariable*>, SliceType) const;
    void addParameters(std::vector<Symbols::Parameter*>, SliceType) const;
    void compileProgramInit() const;
    void compileVariable() const;
    void compileProcedure() const;
    void compileFunction() const;
    void compileRead() const;
    void compileWrite() const;
    void compileMethodParameters(Symbols::Method*) const;
    void compileMain() const;
    void compileIf() const;
    void compileWhile() const;
    void compileCommand(bool) const;
    void compileCompoundCommand() const;

    //Expression Parser
    void arithmeticExpression() const;
    void arithmeticTerm() const;
    void arithmeticFactor() const;
    void relationalExpression() const;
    void relationalTerm() const;
    void relationalFactor() const;
    void booleanExpression() const;
};

#endif
