#include <iostream>
#include <stdlib.h>
#include <stdexcept>
#include <string>
#include "Parser.h"

Parser::Parser(char* fileName)
{
    //Creates the Lexer and SymbolTable and stores they
    Lexer* l = new Lexer("C://temp//Vierte.Reich");
    this->lexer = l;

    Symbols::SymbolTable* t = new Symbols::SymbolTable();
    this->symbolTable = t;
}

Parser::~Parser()
{
    //Deletes the Lexer and SymbolTable pointers
    delete(this->lexer);
    delete(this->symbolTable);
}

void Parser::compileProgram()
{
    //Compile the program init (Program declaration and global variables)
    compileProgramInit();
    compileVariable();

    SliceType next = this->lexer->nextSlice(false);
    while (next != Begin)
    {
        //If the next token isn't a Begin, it must be a Procedure or a Function
        switch(next)
        {
          case Procedure: this->compileProcedure(); break;
          case Function : this->compileFunction(); break;
          default       : this->lexer->throwError("Unexpected token, expected main declaration", next); break;
        }

        next = this->lexer->nextSlice(false);
    }

    //Compile the main program
    this->compileMain();
}

bool Parser::redeclaredVariable(char* variableName)
{
    Symbols::Symbol* symbol = this->symbolTable->getSymbol(variableName);

    if (symbol != NULL)
        if (symbol->getSymbolType() == Symbols::SymbolType_Function || symbol->getSymbolType() == Symbols::SymbolType_Procedure)
            return true;
        else if (symbol->getSymbolType() == Symbols::SymbolType_Parameter && symbol->getScope() == this->symbolTable->getActualScope())
                return true;

    return false;
}

void Parser::addVariables(std::vector<Symbols::Variable*> pending, SliceType type)
{
    if (type == Boolean)
        for (int n = 0; n < pending.size(); n++)
            this->symbolTable->add(new Symbols::Variable(Symbols::Boolean, (char*)pending[n]->getName(), pending[n]->getScope()));
    else
        for (int n = 0; n < pending.size(); n++)
            this->symbolTable->add(pending[n]);
}

void Parser::addParameters(std::vector<Symbols::Parameter*> pending, SliceType type)
{
    if (type == Boolean)
        for (int n = 0; n < pending.size(); n++)
            this->symbolTable->add(new Symbols::Parameter(pending[n]->getParameterType(), Symbols::Boolean, (char*)pending[n]->getName(), pending[n]->getScope()));
    else
        for (int n = 0; n < pending.size(); n++)
            this->symbolTable->add(pending[n]);
}

void Parser::compileProgramInit()
{
    SliceType next = this->lexer->nextSlice(true);
    if (next != Program)
        this->lexer->throwError("Expected a program initialization", next);

    next = this->lexer->nextSlice(true);
    if (next != Identifier)
        this->lexer->throwError("Unexpected token, expected a program identifier", next);

    next = this->lexer->nextSlice(true);
    if (next != Semicolon)
        this->lexer->throwError("Unexpected token, expected a semicolon", next);
}

void Parser::compileVariable()
{
    SliceType next = this->lexer->nextSlice(false);
    if (next != Variable)
        return;

    this->lexer->nextSlice(true);
    next = this->lexer->nextSlice(true);
    if (next != Identifier)
        this->lexer->throwError("Unexpected token, expected an identifier", next);

    //While the next is a identifier, the user is naming different types of variables
    while (next == Identifier)
    {
        //Vector to stores the variables which the type isn't known yet
        std::vector<Symbols::Variable*> pending;

        //Check the disponibility of the name
        char* variableName = this->lexer->getName();
        if (this->redeclaredVariable(variableName))
            this->lexer->throwError("Redeclaration of variable", next);

        Symbols::Variable* v = new Symbols::Variable(Symbols::Integer, variableName, this->symbolTable->getActualScope());
        pending.push_back(v);

        //While next isn't a colon, the user is naming variables
        next = this->lexer->nextSlice(true);
        while (next == Comma)
        {
            next = this->lexer->nextSlice(true);
            if (next != Identifier)
                this->lexer->throwError("Unexpected token, expected an identifier after comma", next);

            //Check the disponibility of the name
            char* variableName = this->lexer->getName();
            if (this->redeclaredVariable(variableName))
                this->lexer->throwError("Redeclaration of variable", next);

            Symbols::Variable* v = new Symbols::Variable(Symbols::Integer, variableName, this->symbolTable->getActualScope());
            pending.push_back(v);

            next = this->lexer->nextSlice(true);
        }

        if (next != Colon)
            this->lexer->throwError("Unexpected token, expected a colon", next);

        next = this->lexer->nextSlice(true);
        if (next != Integer && next != Boolean)
            this->lexer->throwError("Unexpected token, expected a type", next);

        this->addVariables(pending, next);

        next = this->lexer->nextSlice(true);
        if (next != Semicolon)
            this->lexer->throwError("Unexpected token, expected a semicolon", next);

        next = this->lexer->nextSlice(false);
        if (next == Identifier)
            this->lexer->nextSlice(true);
    }
}

void Parser::compileProcedure()
{
    //Procedure header (procedure p ( a, b : integer);)
    SliceType next = this->lexer->nextSlice(false);
    if (next != Procedure)
        return;

    this->lexer->nextSlice(true);
    next = this->lexer->nextSlice(true);
    if (next != Identifier)
        this->lexer->throwError("Unexpected token, expected a procedure name", next);

    char* procName = this->lexer->getName();
    Symbols::Symbol* s = this->symbolTable->getSymbol(procName);
    if (s != NULL && s->getScope() == 0)
        this->lexer->throwError("Procedure name can't be the same as a variable", next);
    Symbols::Procedure* procedure = new Symbols::Procedure(procName, this->symbolTable->getActualScope());
    this->symbolTable->add(procedure);

    next = this->lexer->nextSlice(false);
    if (next == LeftParenthesis)
    {
        this->lexer->nextSlice(true);

        //Variables loop
        next = this->lexer->nextSlice(false);
        while (next != RightParenthesis)
        {
            this->compileProcedureVariables(procedure);
            next = this->lexer->nextSlice(false);
        }

        next = this->lexer->nextSlice(true);
        if (next != RightParenthesis)
            this->lexer->throwError("Unexpected token, expected parenthesis", next);
    }

    next = this->lexer->nextSlice(true);
    if (next != Semicolon)
        this->lexer->throwError("Unexpected token, expected a semicolon", next);

    //Procedure body (vars and commands)
    this->compileVariable();
    this->compileCompoundCommand(false);
}

void Parser::compileFunction()
{
    //Procedure header (procedure p ( a, b : integer);)
    SliceType next = this->lexer->nextSlice(false);
    if (next != Function)
        return;

    this->lexer->nextSlice(true);
    next = this->lexer->nextSlice(true);
    if (next != Identifier)
        this->lexer->throwError("Unexpected token, expected a function name", next);

    char* functionName = this->lexer->getName();
    Symbols::Symbol* s = this->symbolTable->getSymbol(functionName);
    if (s != NULL && s->getScope() == 0)
        this->lexer->throwError("Function name can't be the same as a variable", next);

    Symbols::Function* func = new Symbols::Function(Symbols::Integer, functionName, this->symbolTable->getActualScope());

    next = this->lexer->nextSlice(false);
    if (next == LeftParenthesis)
    {
        this->lexer->nextSlice(true);

        //Variables loop
        next = this->lexer->nextSlice(false);
        while (next != RightParenthesis)
        {
            this->compileFunctionVariables(func);
            next = this->lexer->nextSlice(false);
        }

        next = this->lexer->nextSlice(true);
        if (next != RightParenthesis)
            this->lexer->throwError("Unexpected token, expected parenthesis", next);
    }

    next = this->lexer->nextSlice(true);
    if (next != Colon)
        this->lexer->throwError("Unexpected token, expected colon after function declaration", next);

    next = this->lexer->nextSlice(true);
    if (next != Integer && next != Boolean)
        this->lexer->throwError("Unexpected token, expected type identifier after function declaration", next);

    Symbols::Function* realFunc = new Symbols::Function((next == Integer ? Symbols::Integer : Symbols::Boolean), (char*)func->getName(), this->symbolTable->getActualScope());
    for (int n = 0; n < func->getParametersCount(); n++)
        realFunc->addParameter(func->getParameter(n));
    this->symbolTable->add(realFunc);

    next = this->lexer->nextSlice(true);
    if (next != Semicolon)
        this->lexer->throwError("Unexpected token, expected a semicolon", next);

    //Procedure body (vars and commands)
    this->compileVariable();
    this->compileCompoundCommand(false);
}

void Parser::compileProcedureVariables(Symbols::Procedure* proc)
{
    std::vector<Symbols::Parameter*> pending;
    int variablesNumber = 0;

    SliceType next = this->lexer->nextSlice(true);
    if (next == Variable)
    {
        next = this->lexer->nextSlice(true);
        if (next != Identifier)
            this->lexer->throwError("Unexpected token, expected a variable identifier", next);

        pending.push_back(new Symbols::Parameter(Symbols::Reference, Symbols::Integer, this->lexer->getName(), this->symbolTable->getActualScope()));
    }
    else if (next != Identifier)
        this->lexer->throwError("Unexpected token, expected a variable identifier", next);
    else
        pending.push_back(new Symbols::Parameter(Symbols::Value, Symbols::Integer, this->lexer->getName(), this->symbolTable->getActualScope()));
    variablesNumber++;

    next = this->lexer->nextSlice(true);
    //Works as compileVariable()
    while (next == Comma)
    {
        next = this->lexer->nextSlice(true);
        if (next == Variable)
        {
            next = this->lexer->nextSlice(true);
            if (next != Identifier)
                this->lexer->throwError("Unexpected token, expected a variable identifier", next);

            pending.push_back(new Symbols::Parameter(Symbols::Reference, Symbols::Integer, this->lexer->getName(), this->symbolTable->getActualScope()));
        }
        else if (next != Identifier)
            this->lexer->throwError("Unexpected token, expected an identifier after comma", next);
        else
            pending.push_back(new Symbols::Parameter(Symbols::Value, Symbols::Integer, this->lexer->getName(), this->symbolTable->getActualScope()));
        variablesNumber++;

        next = this->lexer->nextSlice(true);
    }

    if (next != Colon)
        this->lexer->throwError("Unexpected token, expected a colon", next);

    next = this->lexer->nextSlice(true);
    if (next != Integer && next != Boolean)
        this->lexer->throwError("Unexpected token, expected a type", next);

    this->addParameters(pending, next);

    Symbols::Type type = (next == Integer ? Symbols::Integer : Symbols::Boolean);
    for (int n = 0; n < variablesNumber; n++)
        proc->addParameter(type);

    next = this->lexer->nextSlice(true);
    if (next != Semicolon)
        this->lexer->throwError("Unexpected token, expected a semicolon", next);
}

void Parser::compileFunctionVariables(Symbols::Function* func)
{
    std::vector<Symbols::Parameter*> pending;
    int variablesNumber = 0;

    SliceType next = this->lexer->nextSlice(true);
    if (next == Variable)
    {
        next = this->lexer->nextSlice(true);
        if (next != Identifier)
            this->lexer->throwError("Unexpected token, expected a variable identifier", next);

        pending.push_back(new Symbols::Parameter(Symbols::Reference, Symbols::Integer, this->lexer->getName(), this->symbolTable->getActualScope()));
    }
    else if (next != Identifier)
        this->lexer->throwError("Unexpected token, expected a variable identifier", next);
    else
        pending.push_back(new Symbols::Parameter(Symbols::Value, Symbols::Integer, this->lexer->getName(), this->symbolTable->getActualScope()));
    variablesNumber++;

    next = this->lexer->nextSlice(true);
    //Works as compileVariable()
    while (next == Comma)
    {
        next = this->lexer->nextSlice(true);
        if (next == Variable)
        {
            next = this->lexer->nextSlice(true);
            if (next != Identifier)
                this->lexer->throwError("Unexpected token, expected a variable identifier", next);

            pending.push_back(new Symbols::Parameter(Symbols::Reference, Symbols::Integer, this->lexer->getName(), this->symbolTable->getActualScope()));
        }
        else if (next != Identifier)
            this->lexer->throwError("Unexpected token, expected an identifier after comma", next);
        else
            pending.push_back(new Symbols::Parameter(Symbols::Value, Symbols::Integer, this->lexer->getName(), this->symbolTable->getActualScope()));
        variablesNumber++;

        next = this->lexer->nextSlice(true);
    }

    if (next != Colon)
        this->lexer->throwError("Unexpected token, expected a colon", next);

    next = this->lexer->nextSlice(true);
    if (next != Integer && next != Boolean)
        this->lexer->throwError("Unexpected token, expected a type", next);

    this->addParameters(pending, next);

    Symbols::Type type = (next == Integer ? Symbols::Integer : Symbols::Boolean);
    for (int n = 0; n < variablesNumber; n++)
        func->addParameter(type);

    next = this->lexer->nextSlice(true);
    if (next != Semicolon)
        this->lexer->throwError("Unexpected token, expected a semicolon", next);
}

void Parser::compileMain()
{
  compileCompoundCommand(true);

  SliceType next = this->lexer->nextSlice(true);
  if (next != Point)
    this->lexer->throwError("Missing point after main", next);
}

void Parser::compileIf()
{

}

void Parser::compileWhile()
{

}

void Parser::compileCommand()
{
    SliceType next = this->lexer->nextSlice(false);
    if (next != Identifier)
        return;

    this->lexer->nextSlice(true);
}

void Parser::compileCompoundCommand(bool isMainCommand)
{
    SliceType next = this->lexer->nextSlice(true);
    if (next != Begin)
        this->lexer->throwError("Missing opening", next);

    //Commands
    next = this->lexer->nextSlice(false);
    while (next != End)
    {
        if (next == If)
            this->compileIf();
        else if (next == While)
            this->compileWhile();
        else if (next != Identifier)
            this->lexer->throwError("Unexpected token", next);
        else
            this->compileCommand();

        next = this->lexer->nextSlice(false);
    }
    this->lexer->nextSlice(true);

    if (!isMainCommand)
    {
        next = this->lexer->nextSlice(true);
        if (next != Semicolon)
            this->lexer->throwError("Missing semicolon after end", next);
    }
}
