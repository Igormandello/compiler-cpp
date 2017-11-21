#include <iostream>
#include "Parser.h"

Parser::Parser(Lexer l)
{
    this->lexer = l;
}

void Parser::compileProgramInit()
{

}

void Parser::compileVariable()
{
    SliceType next = this->lexer.nextSlice(false);
    if (next == Variable)
    {
        this->lexer.nextSlice(true);
        next = this->lexer.nextSlice(true);
        if (next != Identifier)
            throw std::invalid_argument("Unexpected token, expected an identifier");

        next = this->lexer.nextSlice(true);
        if (next != Colon)
            throw std::invalid_argument("Unexpected token, expected a colon");

        next = this->lexer.nextSlice(true);
        if (next != Identifier)
            throw std::invalid_argument("Unexpected token, expected a type identifier");

        next = this->lexer.nextSlice(true);
        if (next != Semicolon)
            throw std::invalid_argument("Unexpected token, expected a semicolon");
    }
}

void Parser::compileProcedure()
{

}

void Parser::compileFunction()
{

}

void Parser::compileMain()
{
  compileCompoundCommand();

  SliceType next = this->lexer.nextSlice(true);
  if (next != Point)
    throw "Missing point after main";
}

void Parser::compileCompoundCommand()
{
    SliceType next;

    next = this->lexer.nextSlice(true);
    if (next != Begin)
        throw "Missing opening";

    //Commands

    next = this->lexer.nextSlice(true);
    if (next != End)
        throw "Missing closing";
}
