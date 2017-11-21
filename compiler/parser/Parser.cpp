#include <iostream>
#include <stdlib.h>
#include <stdexcept>
#include <string>
#include "Parser.h"

Parser::Parser(Lexer l)
{
    this->lexer = l;
}

void Parser::compileProgramInit()
{
    SliceType next = this->lexer.nextSlice(true);
    if (next != Program)
        this->lexer.throwError("Expected a program initialization");

    next = this->lexer.nextSlice(true);
    if (next != Identifier)
        this->lexer.throwError("Unexpected token, expected a program identifier");

    next = this->lexer.nextSlice(true);
    if (next != Semicolon)
        this->lexer.throwError("Unexpected token, expected a semicolon");
}

void Parser::compileVariable()
{
    SliceType next = this->lexer.nextSlice(true);
    if (next != Variable)
        this->lexer.throwError("Expected a variable declaration");

    next = this->lexer.nextSlice(true);
    if (next != Identifier)
        this->lexer.throwError("Unexpected token, expected an identifier");

    next = this->lexer.nextSlice(true);
    if (next != Colon)
        this->lexer.throwError("Unexpected token, expected a colon");

    next = this->lexer.nextSlice(true);
    if (next != Identifier)
        this->lexer.throwError("Unexpected token, expected a type identifier");

    next = this->lexer.nextSlice(true);
    if (next != Semicolon)
        this->lexer.throwError("Unexpected token, expected a semicolon");
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
    this->lexer.throwError("Missing point after main");
}

void Parser::compileCompoundCommand()
{
    SliceType next;

    next = this->lexer.nextSlice(true);
    if (next != Begin)
        this->lexer.throwError("Missing opening");

    //Commands

    next = this->lexer.nextSlice(true);
    if (next != End)
        this->lexer.throwError("Missing closing");
}
