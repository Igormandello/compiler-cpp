#include <iostream>
#include <stdlib.h>
#include <stdexcept>
#include <string>
#include "Parser.h"

Parser::Parser(char* fileName)
{
    Lexer* l = new Lexer("C://temp//Vierte.Reich");
    this->lexer = l;
}

Parser::~Parser()
{
    delete(this->lexer);
}

void Parser::compileProgramInit()
{
    SliceType next = this->lexer->nextSlice(true);
    if (next != Program)
        this->lexer->throwError("Expected a program initialization");

    next = this->lexer->nextSlice(true);
    if (next != Identifier)
        this->lexer->throwError("Unexpected token, expected a program identifier");

    next = this->lexer->nextSlice(true);
    if (next != Semicolon)
        this->lexer->throwError("Unexpected token, expected a semicolon");
}

void Parser::compileVariable()
{
    SliceType next = this->lexer->nextSlice(true);
    if (next != Variable)
        this->lexer->throwError("Expected a variable declaration");

    next = this->lexer->nextSlice(true);
    if (next != Identifier)
        this->lexer->throwError("Unexpected token, expected an identifier");

    while (next == Identifier)
    {
        next = this->lexer->nextSlice(true);
        while (next == Comma)
        {
            next = this->lexer->nextSlice(true);
            if (next != Identifier)
                this->lexer->throwError("Unexpected token, expected an identifier after comma");

            next = this->lexer->nextSlice(true);
        }

        if (next != Colon)
            this->lexer->throwError("Unexpected token, expected a colon");

        next = this->lexer->nextSlice(true);
        if (next != Identifier)
            this->lexer->throwError("Unexpected token, expected a type identifier");

        next = this->lexer->nextSlice(true);
        if (next != Semicolon)
            this->lexer->throwError("Unexpected token, expected a semicolon");

        next = this->lexer->nextSlice(false);
        if (next == Identifier)
            this->lexer->nextSlice(true);
    }
}

void Parser::compileProcedure()
{
    SliceType next = this->lexer->nextSlice(true);
    if (next != Procedure)
        this->lexer->throwError("Expected a procedure declaration");

    next = this->lexer->nextSlice(true);
    if (next != Identifier)
        this->lexer->throwError("Unexpected token, expected a procedure name");
}

void Parser::compileFunction()
{

}

void Parser::compileMain()
{
  compileCompoundCommand();

  SliceType next = this->lexer->nextSlice(true);
  if (next != Point)
    this->lexer->throwError("Missing point after main");
}

void Parser::compileIf()
{

}

void Parser::compileWhile()
{

}

void Parser::compileCompoundCommand()
{
    SliceType next;

    next = this->lexer->nextSlice(true);
    if (next != Begin)
        this->lexer->throwError("Missing opening");

    //Commands

    next = this->lexer->nextSlice(true);
    if (next != End)
        this->lexer->throwError("Missing closing");
}
