#include "Parser.h"

Parser::Parser(Lexer l)
{
    this->lexer = l;
}

void Parser::compileProgramInit()
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

    next = this->lexer.nextSlice(true);
    if (next != End)
        throw "Missing closing";
}
