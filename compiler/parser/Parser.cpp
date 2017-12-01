#include <iostream>
#include <stdlib.h>
#include <stdexcept>
#include <string>
#include "Parser.h"

Parser::Parser(char* fileName)
{
    //Creates the Lexer and SymbolTable and stores they
    Lexer* l = new Lexer(fileName);
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

void Parser::compileProgram() const throw(std::runtime_error)
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

bool Parser::redeclaredVariable(char* variableName) const
{
    Symbols::Symbol* symbol = this->symbolTable->getSymbol(variableName);

    //Check if the variable already exists in the local
    if (symbol != NULL)
        if (symbol->getSymbolType() == Symbols::SymbolType_Function || symbol->getSymbolType() == Symbols::SymbolType_Procedure)
            return true;
        else if (symbol->getSymbolType() == Symbols::SymbolType_Parameter)
            return true;

    return false;
}

void Parser::addVariables(std::vector<Symbols::LocalVariable*> pending, SliceType type) const
{
    //Run through the pending variables and add they to the symbolTable
    if (type == Boolean)
        for (int n = 0; n < pending.size(); n++)
            this->symbolTable->add(new Symbols::LocalVariable(Symbols::Boolean, (char*)pending[n]->getName(), pending[n]->getScope()));
    else
        for (int n = 0; n < pending.size(); n++)
            this->symbolTable->add(pending[n]);
}

void Parser::addParameters(std::vector<Symbols::Parameter*> pending, SliceType type) const
{
    //Run through the pending parameters and add they to the symbolTable
    if (type == Boolean)
        for (int n = 0; n < pending.size(); n++)
        {
            if (this->redeclaredVariable((char*)pending[n]->getName()))
                this->lexer->throwError("Redeclared variable", Unknown);

            this->symbolTable->add(new Symbols::Parameter(pending[n]->getParameterType(), Symbols::Boolean, (char*)pending[n]->getName(), pending[n]->getScope()));
        }
    else
        for (int n = 0; n < pending.size(); n++)
        {
            if (this->redeclaredVariable((char*)pending[n]->getName()))
                this->lexer->throwError("Redeclared variable", Unknown);

            this->symbolTable->add(pending[n]);
        }
}

void Parser::compileProgramInit() const
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

void Parser::compileVariable() const
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
        std::vector<Symbols::LocalVariable*> pending;

        //Check the disponibility of the name
        char* variableName = this->lexer->getName();
        if (this->redeclaredVariable(variableName))
            this->lexer->throwError("Redeclaration of variable", next);

        Symbols::LocalVariable* v = new Symbols::LocalVariable(Symbols::Integer, variableName, this->symbolTable->getActualScope());
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

            Symbols::LocalVariable* v = new Symbols::LocalVariable(Symbols::Integer, variableName, this->symbolTable->getActualScope());
            pending.push_back(v);

            next = this->lexer->nextSlice(true);
        }

        if (next != Colon)
            this->lexer->throwError("Unexpected token, expected a colon", next);

        next = this->lexer->nextSlice(true);
        if (next != Integer && next != Boolean)
            this->lexer->throwError("Unexpected token, expected a type", next);

        //Now the type is known, so we can add the pending variables
        this->addVariables(pending, next);

        next = this->lexer->nextSlice(true);
        if (next != Semicolon)
            this->lexer->throwError("Unexpected token, expected a semicolon", next);

        next = this->lexer->nextSlice(false);
        if (next == Identifier)
            this->lexer->nextSlice(true);
    }
}

void Parser::compileProcedure() const
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
    if (s != NULL)
        if (s->getSymbolType() == Symbols::SymbolType_Variable || (s->getSymbolType() == Symbols::SymbolType_Function || s->getSymbolType() == Symbols::SymbolType_Procedure))
            this->lexer->throwError("Redeclared identifier", next);

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
            this->compileMethodParameters(procedure);
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

void Parser::compileFunction() const
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
    if (s != NULL)
        if ((s->getSymbolType() == Symbols::SymbolType_Variable && s->getScope() == 0) || (s->getSymbolType() == Symbols::SymbolType_Function || s->getSymbolType() == Symbols::SymbolType_Procedure))
            this->lexer->throwError("Redeclared identifier", next);

    Symbols::Function* func = new Symbols::Function(Symbols::Integer, functionName, this->symbolTable->getActualScope());
    this->symbolTable->add(func);

    next = this->lexer->nextSlice(false);
    if (next == LeftParenthesis)
    {
        this->lexer->nextSlice(true);

        //Variables loop
        next = this->lexer->nextSlice(false);
        while (next != RightParenthesis)
        {
            this->compileMethodParameters(func);
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

    func->setReturnType((next == Integer ? Symbols::Integer : Symbols::Boolean));

    next = this->lexer->nextSlice(true);
    if (next != Semicolon)
        this->lexer->throwError("Unexpected token, expected a semicolon", next);

    //Procedure body (vars and commands)
    this->compileVariable();
    this->compileCompoundCommand(false);
}

void Parser::compileRead() const
{
    SliceType next = this->lexer->nextSlice(false);
    if (next != Read)
        return;

    this->lexer->nextSlice(true);
    next = this->lexer->nextSlice(true);
    if (next != LeftParenthesis)
        this->lexer->throwError("Expected parenthesis after a procedure call", next);

    next = this->lexer->nextSlice(true);
    if (next != Identifier)
        this->lexer->throwError("Invalid parameter type, expected a variable after \"Read\"", next);
    else
    {
        Symbols::Symbol* s = this->symbolTable->getSymbol(this->lexer->getName());

        //Only variables of any type are accepted in read
        if (s == NULL)
            this->lexer->throwError("Use of undeclared variable", next);
        else if (s->getSymbolType() == Symbols::SymbolType_Function || s->getSymbolType() == Symbols::SymbolType_Procedure)
            this->lexer->throwError("Procedures and functions can't be used as a parameter for read", next);
    }

    next = this->lexer->nextSlice(true);
    if (next != RightParenthesis)
        this->lexer->throwError("Expected parenthesis after a parameters list", next);

    next = this->lexer->nextSlice(true);
    if (next != Semicolon)
       this->lexer->throwError("Unexpected token, expected semicolon after expression", next);
}

void Parser::compileWrite() const
{
    SliceType next = this->lexer->nextSlice(false);
    if (next != Write)
        return;

    this->lexer->nextSlice(true);
    next = this->lexer->nextSlice(true);
    if (next != LeftParenthesis)
        this->lexer->throwError("Expected parenthesis after a procedure call", next);

    next = this->lexer->nextSlice(false);
    if (next != Number && next != True && next != False && next != Identifier)
        this->lexer->throwError("Invalid parameter type", next);
    else if (next == Identifier)
    {
        Symbols::Symbol* s = this->symbolTable->getSymbol(this->lexer->getName());

        //Any type of variables or function can be used as a parameter in write
        if (s == NULL)
            this->lexer->throwError("Use of undeclared variable", next);
        else if (s->getSymbolType() == Symbols::SymbolType_Function)
            compileCommand(false);
        else if (s->getSymbolType() == Symbols::SymbolType_Procedure)
            this->lexer->throwError("Procedures can't be used as a parameter", next);
        else if (s->getSymbolType() == Symbols::SymbolType_Variable || s->getSymbolType() == Symbols::SymbolType_Parameter)
            this->lexer->nextSlice(true);
    }
    else
        this->lexer->nextSlice(true);

    next = this->lexer->nextSlice(true);
    if (next != RightParenthesis)
        this->lexer->throwError("Expected parenthesis after a parameters list", next);

    next = this->lexer->nextSlice(true);
    if (next != Semicolon)
       this->lexer->throwError("Unexpected token, expected semicolon after expression", next);
}

void Parser::compileMethodParameters(Symbols::Method* meth) const
{
    int actualScope = this->symbolTable->getActualScope();

    //All variables are added to pending, because we already don't know the actual type of they
    std::vector<Symbols::Parameter*> pending;
    int variablesNumber = 0;

    SliceType next = this->lexer->nextSlice(true);
    if (next == Variable)
    {
        next = this->lexer->nextSlice(true);
        if (next != Identifier)
            this->lexer->throwError("Unexpected token, expected a variable identifier", next);

        pending.push_back(new Symbols::Parameter(Symbols::Reference, Symbols::Integer, this->lexer->getName(), actualScope));
    }
    else if (next != Identifier)
        this->lexer->throwError("Unexpected token, expected a variable identifier", next);
    else
        pending.push_back(new Symbols::Parameter(Symbols::Value, Symbols::Integer, this->lexer->getName(), actualScope));

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

            pending.push_back(new Symbols::Parameter(Symbols::Reference, Symbols::Integer, this->lexer->getName(), actualScope));
        }
        else if (next != Identifier)
            this->lexer->throwError("Unexpected token, expected an identifier after comma", next);
        else
            pending.push_back(new Symbols::Parameter(Symbols::Value, Symbols::Integer, this->lexer->getName(), actualScope));
        variablesNumber++;

        next = this->lexer->nextSlice(true);
    }

    if (next != Colon)
        this->lexer->throwError("Unexpected token, expected a colon", next);

    next = this->lexer->nextSlice(true);
    if (next != Integer && next != Boolean)
        this->lexer->throwError("Unexpected token, expected a type", next);

    //Now we know the type of the variables on pending, so we add they in symbolTable
    this->addParameters(pending, next);

    Symbols::Type type = (next == Integer ? Symbols::Integer : Symbols::Boolean);
    for (int n = 0; n < variablesNumber; n++)
        meth->addParameter(type);

    next = this->lexer->nextSlice(true);
    if (next != Semicolon)
        this->lexer->throwError("Unexpected token, expected a semicolon", next);
}

void Parser::compileMain() const
{
  this->symbolTable->addScope();
  compileCompoundCommand(true);

  SliceType next = this->lexer->nextSlice(true);
  if (next != Point)
    this->lexer->throwError("Missing point after main", next);
}

void Parser::compileIf() const
{
    SliceType next = this->lexer->nextSlice(false);
    if (next != If)
        return;

    this->lexer->nextSlice(true);
    this->booleanExpression();

    next = this->lexer->nextSlice(true);
    if (next != Then)
        this->lexer->throwError("Unexpected token, expected \"Then\" after condition", next);

    next = this->lexer->nextSlice(false);
    if (next == Begin)
        this->compileCompoundCommand(false);
    else
        this->compileCommand(true);
}

void Parser::compileWhile() const
{
    SliceType next = this->lexer->nextSlice(false);
    if (next != While)
        return;

    this->lexer->nextSlice(true);
    this->booleanExpression();

    next = this->lexer->nextSlice(true);
    if (next != Do)
        this->lexer->throwError("Unexpected token, expected \"Do\" after condition", next);

    next = this->lexer->nextSlice(false);
    if (next == Begin)
        this->compileCompoundCommand(false);
    else
        this->compileCommand(true);
}

void Parser::compileCommand(bool endLine) const
{
    SliceType next = this->lexer->nextSlice(false);
    if (next != Identifier)
        return;

    this->lexer->nextSlice(true);
    Symbols::Symbol* actualSymbol = this->symbolTable->getSymbol(this->lexer->getName());
    if (actualSymbol == NULL)
        this->lexer->throwError("Use of undeclared variable", next);

    //Check what's the type of the identifier that the user writed
    if (actualSymbol->getSymbolType() == Symbols::SymbolType_Procedure || actualSymbol->getSymbolType() == Symbols::SymbolType_Function)
    {
        Symbols::Method* actualMethod = (Symbols::Method*) actualSymbol;

        next = this->lexer->nextSlice(false);
        //If the method have parameters, the user must pass they
        if (next != LeftParenthesis && actualMethod->getParametersCount() != 0)
            this->lexer->throwError("Expected parenthesis after a procedure or function call", next);

        //But, if the method doesn't have any parameters, the user can call with or without parentheses
        if (!(next != LeftParenthesis && actualMethod->getParametersCount() == 0))
        {
            this->lexer->nextSlice(true);
            for (int n = 0; n < actualMethod->getParametersCount(); n++)
            {
                next = this->lexer->nextSlice(false);

                Symbols::Type parameterType = actualMethod->getParameter(n);
                if ((parameterType == Symbols::Integer && (next != Number && next != Identifier)) || (parameterType == Symbols::Boolean && (next != True && next != False && next != Identifier)))
                    this->lexer->throwError("Invalid parameter type", next);
                else if (next == Identifier)
                {
                    Symbols::Symbol* s = this->symbolTable->getSymbol(this->lexer->getName());
                    if (s == NULL)
                        this->lexer->throwError("Use of undeclared variable", next);
                    else if (s->getSymbolType() == Symbols::SymbolType_Function)
                    {
                        Symbols::Function* var = (Symbols::Function*) s;
                        if (var->getReturnType() != parameterType)
                            this->lexer->throwError("Invalid parameter type", next);

                        //If it's a function, calls recursively compileCommand, to make the same checks on the function used as a parameter
                        compileCommand(false);
                    }
                    else if (s->getSymbolType() == Symbols::SymbolType_Procedure)
                        this->lexer->throwError("Procedures can't be used as a parameter", next);
                    else
                    {
                        Symbols::Variable* var = (Symbols::Variable*) s;
                        if (var->getType() != parameterType)
                           this->lexer->throwError("Invalid parameter type", next);

                        this->lexer->nextSlice(true);
                    }
                }
                else
                    this->lexer->nextSlice(true);

                if (n != actualMethod->getParametersCount() - 1)
                {
                    next = this->lexer->nextSlice(true);
                    if (next != Comma)
                        this->lexer->throwError("Unexpected token, expected a comma beetween parameters", next);
                }
            }

            next = this->lexer->nextSlice(true);
            if (next != RightParenthesis)
                this->lexer->throwError("Expected parenthesis after a parameters list", next);
        }
    }
    else if (actualSymbol->getSymbolType() == Symbols::SymbolType_Parameter || actualSymbol->getSymbolType() == Symbols::SymbolType_Variable)
        if (actualSymbol->getScope() == 0 || actualSymbol->getScope() == this->symbolTable->getActualScope())
        {
            next = this->lexer->nextSlice(true);
            if (next != Colon)
                this->lexer->throwError("Expected assignment after a variable", next);

            next = this->lexer->nextSlice(true);
            if (next != Equal)
                this->lexer->throwError("Expected assignment after a variable", next);

            Symbols::Variable* var = (Symbols::Variable*) actualSymbol;
            Symbols::Type type = var->getType();

            if (type == Symbols::Integer)
                this->arithmeticExpression();
            else
                this->booleanExpression();
        }
        else
            this->lexer->throwError("Use of undeclared variable", next);

    //If endLine is true, it expects a semicolon after the command, so it can be used on recursions like function(function(1))
    if (endLine)
    {
        next = this->lexer->nextSlice(true);
        if (next != Semicolon)
           this->lexer->throwError("Unexpected token, expected semicolon after expression", next);
    }
}

void Parser::compileCompoundCommand(bool isMainCommand) const
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
        else if (next == Write)
            this->compileWrite();
        else if (next == Read)
            this->compileRead();
        else if (next != Identifier)
            this->lexer->throwError("Unexpected token", next);
        else
            this->compileCommand(true);

        next = this->lexer->nextSlice(false);
    }
    this->lexer->nextSlice(true);

    //If isn't the main compound command, it expects a semicolon after end
    if (!isMainCommand)
    {
        next = this->lexer->nextSlice(true);
        if (next != Semicolon)
            this->lexer->throwError("Missing semicolon after end", next);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////EXPRESSION PARSER////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Parser::arithmeticExpression() const
{
    this->arithmeticTerm();

    SliceType next = this->lexer->nextSlice(false);
    while (next == Plus || next == Minus)
    {
        this->lexer->nextSlice(true);
        this->arithmeticTerm();

        next = this->lexer->nextSlice(false);
    }
}

void Parser::arithmeticTerm() const
{
    this->arithmeticFactor();

    SliceType next = this->lexer->nextSlice(false);
    while (next == Times || next == Slash)
    {
        this->lexer->nextSlice(true);
        this->arithmeticFactor();

        next = this->lexer->nextSlice(false);
    }
}

void Parser::arithmeticFactor() const
{
    SliceType next = this->lexer->nextSlice(true);
    if (next == Minus)
        next = this->lexer->nextSlice(true);

    if (next == Identifier)
    {
        Symbols::Symbol* s = this->symbolTable->getSymbol(this->lexer->getName());

        if (s == NULL)
            this->lexer->throwError("Use of undeclared variable", next);
        if (s->getSymbolType() == Symbols::SymbolType_Function && ((Symbols::Function*)s)->getReturnType() == Symbols::Integer)
            this->compileCommand(false);
        else if (s->getSymbolType() == Symbols::SymbolType_Procedure || ((Symbols::Variable*)s)->getType() != Symbols::Integer)
            this->lexer->throwError("Can't convert to integer", next);
    }
    else if (next == LeftParenthesis)
    {
        this->arithmeticExpression();

        next = this->lexer->nextSlice(true);
        if (next != RightParenthesis)
            this->lexer->throwError("No matching parenthesis found", next);
    }
    else if (next != Number)
        this->lexer->throwError("Can't convert to integer", next);
}

void Parser::relationalExpression() const
{
    this->arithmeticExpression();

    SliceType next = this->lexer->nextSlice(true);
    if (next != Minor && next != Greater && next != Equal)
        this->lexer->throwError("Unexpected symbol after a integer", next);

    this->arithmeticExpression();
}

void Parser::relationalTerm() const
{
   this->relationalFactor();

   SliceType next = this->lexer->nextSlice(false);
   while (next == And)
   {
      this->lexer->nextSlice(true);
      this->relationalFactor();

      next = this->lexer->nextSlice(false);
   }
}

void Parser::relationalFactor() const
{
    SliceType next = this->lexer->nextSlice(false);
    if (next == Not)
    {
        this->lexer->nextSlice(true);
        next = this->lexer->nextSlice(false);
    }

    if (next == Identifier)
    {
        Symbols::Symbol* s = this->symbolTable->getSymbol(this->lexer->getName());

        if (s == NULL)
            this->lexer->throwError("Use of undeclared variable", next);
        if (s->getSymbolType() == Symbols::SymbolType_Function && ((Symbols::Function*)s)->getReturnType() == Symbols::Boolean)
            this->compileCommand(false);
        else if (s->getSymbolType() == Symbols::SymbolType_Procedure)
            this->lexer->throwError("Can't convert to boolean", next);
        else if (((Symbols::Variable*)s)->getType() != Symbols::Boolean)
            this->relationalExpression();
        else
            this->lexer->nextSlice(true);
    }
    else if(next == LeftParenthesis)
	{
	    this->lexer->nextSlice(true);
        this->booleanExpression();

        next = this->lexer->nextSlice(true);
        if (next != RightParenthesis)
            this->lexer->throwError("No matching parenthesis found", next);
	}
    else if(next != True && next != False)
        this->lexer->throwError("Can't convert to boolean", next);
    else
        this->lexer->nextSlice(true);
}

void Parser::booleanExpression() const
{
    this->relationalTerm();

    SliceType next = this->lexer->nextSlice(false);
    while(next == Or)
    {
        this->lexer->nextSlice(true);
        this->relationalTerm();

        next = this->lexer->nextSlice(false);
    }
}
